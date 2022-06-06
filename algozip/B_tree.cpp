#include <chrono>
#include <iostream>
#include <time.h>
#include <cassert>
#include <vector>


using namespace std;
using namespace chrono;

class BTreeNode
{
	friend class BTree;
private:
	vector<int> keys; // key 값을 저장 할 배열
    vector<BTreeNode*> children; // pointer 배열을 통해 자식사용
	int t, key_num; // 현재 키 수
	bool leaf;
	bool IsFull() const;
	bool IsEnough() const;
public:
	BTreeNode(int degree_in, bool _leaf); // 생성자
	void Traverse(); // 트리 순회
	BTreeNode* Search(int k); // if k is not present, return NULL
	void Insert(int key);
	void SplitChild(int index, BTreeNode* left);
	void Remove(int k);
private:
	int FindKey(int k) const;
	void Removeleaf(int index);
	void RemoveNonleaf(int index);
	int GetPred(int index);
	int GetSucc(int index);
	void Fill(int index);
	void takeFromPrev(int index);
	void takeFromNext(int index);
	void Merge(int index);
};

class BTree
{
private:
	BTreeNode* root;
	int degree;
public:
	// 생성자
	BTree(int degree_in):
		root(nullptr),
		degree(degree_in)
	{
	}
	// 소멸자
	~BTree()
	{
		vector<BTreeNode*> visit;

        if (root)
		{
			visit.push_back(root);
			while (!visit.empty())
			{
				BTreeNode* cur = visit.back();
				bool allVisited = true;
				for (int i = 0; i <= cur->key_num; ++i)
				{
					if (cur->children[i] == nullptr)
						continue;
					if (cur->children[i]->leaf)
					{
						delete cur->children[i];
						cur->children[i] = nullptr;
					}
					else
					{
						visit.push_back(cur->children[i]);
						allVisited = false;
						break;
					}
				}
				if (allVisited)
				{
					cur->leaf = true;
					visit.pop_back();
				}
			}
			delete root;
		}
	}

	// 트리를 순회하는 함수
	void Traverse()
	{
		if (root)
		{
			root->Traverse();
		}
	}

	BTreeNode* Search(int k)
	{
        if (root == nullptr)
            return nullptr;
        else
            return root->Search(k);
	}

	void Insert(int k)
	{
		if (!root)
		{
			root = new BTreeNode(degree, true);
		}

		if (root->IsFull())
		{
			BTreeNode* newRoot = new BTreeNode(degree, false);

			// 올라간 값이 새로운 루트가 되고, 자식에는 기존 루트
			newRoot->children[0] = root;

			// 기존에 있던 root값을 split
			newRoot->SplitChild(0, root);

			// split한 값이 왼쪽자식인지 오른쪽자식인지 확인
            int i = newRoot->keys[0] < k ? 1 : 0;
			newRoot->children[i]->Insert(k);

			// 루트값 재설정
			root = newRoot;
		}
		else root->Insert(k);
	}

	void Remove(int k)
	{
		if (!root) //트리가 비어있을 경우
		{
			return;
		}

		root->Remove(k);

		// root의 numkey가 0일때를 체크
		if (root->key_num == 0)
		{
			BTreeNode* tmp = root;
			if (root->leaf)
				root = nullptr;
			else
				root = root->children[0];

			// free the old root
			delete tmp;
		}
	}
};

BTreeNode::BTreeNode(int degree_in, bool _leaf):
    t(degree_in),
    key_num(0),
    leaf(_leaf)
{
    keys.resize(t * 2 - 1, 0);
    children.resize(t * 2, nullptr);
}

void BTreeNode::Traverse()
{
    for (int i = 0; i < key_num; ++i)
    {
        if (!leaf)
        {
            children[i]->Traverse();
        }

    }

    if (!leaf)
    {
        children[key_num]->Traverse();
    }
}

bool BTreeNode::IsFull() const
{
    return key_num == 2 * t - 1;
}

// k를 임의의 수로 정하고, B-Tree에서 찾는 과정
BTreeNode* BTreeNode::Search(int k)
{
    if (key_num > 0)
    {
        // k값의 위치를 찾는다.
        int i = 0;
        while (i < key_num && k > keys[i])
        {
            ++i;
        }
        // k값을 찾을 경우 반환
        if (i < key_num && keys[i] == k)
        {
            return this;
        }
        // 찾지못했을 경우 level을 바꿔가며 탐색
        if (children[i])
        {
            return children[i]->Search(k);
        }
        else return nullptr; //leaf node 일 경우 반환 (탐색실패)
    }
    return nullptr; //leaf node 일 경우 반환 (탐색실패)
}


void BTreeNode::Insert(int k)
{
    // leaf node
    if (leaf)
    {
        int i = key_num - 1;
        while (i >= 0 && keys[i] > k)
        {
            keys[i + 1] = keys[i];
            --i;
        }
        keys[i + 1] = k;
        ++key_num;
    }

    // not leaf node
    else
    {
        int i = key_num - 1;
        while (i >= 0 && keys[i] > k)
        {
            --i;
        }

        if (children[i + 1]->IsFull())
        {
            SplitChild(i + 1, children[i + 1]); //insert 해야하는 자리의 node가 full인경우 분리

            if (keys[i + 1] < k) //새로 삽입된 key값보다 큰지 확인한다.
                ++i;
        }
        children[i + 1]->Insert(k);
    }
}



// 자식노드가 full일 경우 분리해주는 작업을 진행한다.
void BTreeNode::SplitChild(int index, BTreeNode* left)
{
    BTreeNode* right = new BTreeNode(t, left->leaf);

    left->key_num = t - 1;
    right->key_num = t - 1;


    
    for (int i = 0; i < t - 1; ++i) //key값을 복사한다.
    {
        right->keys[i] = left->keys[i + t];
        left->keys[i + t] = 0;
    }

    if (!left->leaf) // children을 복사한다.
    {
        for (int i = 0; i < t; ++i)
        {
            right->children[i] = left->children[i + t];
        }
    }

    for (int i = key_num; i > index; --i) // 기존 children key값을 한칸씩 민다(가운데 새로운 키가 삽입되어야함)
    {
        keys[i] = keys[i - 1];
        children[i + 1] = children[i];
    }

    // 중간에 키를 삽입하고 right children을 삽입한다.
    keys[index] = left->keys[t - 1];
    left->keys[t - 1] = 0;
    children[index + 1] = right;

    ++key_num;
}

bool BTreeNode::IsEnough() const
{
    return key_num >= t;
}

int BTreeNode::FindKey(int k) const
{
    int i = 0;
    while (i < key_num && keys[i] < k)
    {
        ++i;
    }
    return i;
}


// Remove k
void BTreeNode::Remove(int k)
{
    int index = FindKey(k);

    // remove해야하는 index가 노드에 있을 경우
    if (index < key_num && keys[index] == k)
    {
        // leafnode의 여부에 따라 다른 함수를 수행
        leaf ? Removeleaf(index) : RemoveNonleaf(index);
    }
    // remove해야하는 index가 해당 노드에 없을 경우
    else
    {
        // 키가 없을 경우
        if (leaf)
        {
            return;
        }

        bool keyexist = (index == key_num);

        if (!children[index]->IsEnough()) //최소 key 개수보다 적을 경우 채운다.
        {
            Fill(index);
        }

        if (keyexist && index > key_num)
            children[index - 1]->Remove(k);
        else
            children[index]->Remove(k);
    }
}

void BTreeNode::Removeleaf(int index)
{
    for (int i = index + 1; i < key_num; ++i)
    {
        keys[i - 1] = keys[i];
    }

    keys[key_num - 1] = 0;
    --key_num;
}

int BTreeNode::GetPred(int index) //젤 아래 key들 중 가장 오른쪽 key를 return
{
    BTreeNode* cur = children[index];
    while (!cur->leaf)
        cur = cur->children[cur->key_num];

    return cur->keys[cur->key_num - 1];
}

int BTreeNode::GetSucc(int index) //젤 아래 key들 중 가장 왼쪽 key를 return
{
    BTreeNode* cur = children[index + 1];
    while (!cur->leaf)
        cur = cur->children[0];

    return cur->keys[0];
}

void BTreeNode::RemoveNonleaf(int index)
{
    int k = keys[index];

    if (children[index]->IsEnough()) // k보다 key값이 작아야하는 children이 t key 보다 클 경우
    {
        int pred = GetPred(index);
        keys[index] = pred;
        children[index]->Remove(pred);
    }
    else if (children[index + 1]->IsEnough()) // k보다 key값이 작아야하는 children이 t key보다 작을 경우
    {
        int succ = GetSucc(index);
        keys[index] = succ;
        children[index + 1]->Remove(succ);
    }
    else // child index와 child index+1이 t보다 적은 key를 갖고있을 경우 merge
    {
        Merge(index);
        children[index]->Remove(k);
    }
}

// merge children[index] & children[index+1]
void BTreeNode::Merge(int index)
{
    BTreeNode* child = children[index];
    BTreeNode* Childsib = children[index + 1]; //

    // key -> leffchild
    int Childsize = child->key_num;
    child->keys[Childsize] = keys[index];

    // 형제 키를 복사하여 넣는다.
    ++Childsize;
    for (int i = 0; i < Childsib->key_num; ++i)
        child->keys[i + Childsize] = Childsib->keys[i];

    // 형제 자식노드를 복사하여 넣는다.
    if (!child->leaf)
    {
        for (int i = 0; i <= Childsib->key_num; ++i)
            child->children[i + Childsize] = Childsib->children[i];
    }

    // 배열 조정
    for (int i = index + 1; i < key_num; ++i)
        keys[i - 1] = keys[i];
    for (int i = index + 2; i <= key_num; ++i)
        children[i - 1] = children[i];

    child->key_num += (Childsib->key_num + 1);
    --key_num;

    delete Childsib;
}

// children[index]의 key값이 부족할 경우 Fill
void BTreeNode::Fill(int index)
{
    // 왼쪽에서 빌려오는경우
    if (index != 0 && children[index - 1]->IsEnough())
    {
        takeFromPrev(index);
    }

    // 오른쪽에서 빌려오는 경우
    else if (index != key_num && children[index + 1]->IsEnough())
    {
        takeFromNext(index);
    }

    // 충분하지 않을경우 merge (enough이 없을경우)
    else
    {
        index == key_num ? Merge(index - 1) : Merge(index);
    }
}

// children[index-1]로부터 key값을 가져오는 경우
void BTreeNode::takeFromPrev(int index)
{
    BTreeNode* child = children[index];
    BTreeNode* Childsib = children[index - 1];

    // children[index]에 공간마련
    for (int i = child->key_num - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf)
    {
        for (int i = child->key_num; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = keys[index - 1];

    if (!child->leaf)
    {
        child->children[0] = Childsib->children[Childsib->key_num];
    }

    keys[index - 1] = Childsib->keys[Childsib->key_num - 1];

    // 자식과 형제 키 수를 조정한다.
    ++child->key_num;
    --Childsib->key_num;
}

// children[index+1]로부터 key값을 가져오는 경우
void BTreeNode::takeFromNext(int index)
{
    BTreeNode* child = children[index];
    BTreeNode* Childsib = children[index + 1];

    // keys[index]는 children[index]의 마지막 키로 삽입된다.
    child->keys[child->key_num] = keys[index];

    if (!child->leaf)
        child->children[child->key_num + 1] = Childsib->children[0];

    keys[index] = Childsib->keys[0];

    for (int i = 1; i < Childsib->key_num; ++i)
        Childsib->keys[i - 1] = Childsib->keys[i];

    if (!Childsib->leaf)
    {
        for (int i = 1; i <= Childsib->key_num; ++i)
            Childsib->children[i - 1] = Childsib->children[i];
    }

    ++child->key_num;
    --Childsib->key_num;
}

int main()
{
    int num = 5000;
    int num_bound = 9999;
    int block_size = 6;
    BTree t(block_size);
    vector<int> arr(num);

    system_clock::time_point start_insert = system_clock::now(); // 삽입시작시간
    for (int i = 0; i < num; ++i)
    {
        arr[i] = rand()%num_bound;
    }

    for (int i = 0; i < 1000; ++i)
    {
        int p = rand() % num;
        int q = rand() % num;
        std::swap(arr[p], arr[q]);
    }
    cout << "**************************" << endl;
    cout << "삽입시작" << endl;
    cout << "**********삽입중**********" << endl;
    for (int i = 0; i < num; ++i)
    {
        t.Insert(arr[i]);
        
    }
    t.Traverse();
    cout << "삽입종료" << endl;
    system_clock::time_point finish_insert = system_clock::now(); // 삽입종료시간
    cout << "**************************" << endl;
    system_clock::time_point start_delete = system_clock::now(); // 삭제시작시간
    cout << "삭제시작" << endl;
    cout << "**********삭제중**********" << endl;
    for (int i = 0; i < num/2; ++i)
    {
        t.Remove(i + 1);
        t.Traverse();
    }
    cout << "삭제종료" << endl;
    system_clock::time_point finish_delete = system_clock::now(); // 삭제종료시간
    cout << "**************************" << endl;

    microseconds result_insert = duration_cast<microseconds>(finish_insert - start_insert);
    microseconds result_delete = duration_cast<microseconds>(finish_delete - start_delete);

    cout << "개수 : " << num << endl;
    cout << "t = : " << block_size - 1 << endl;
    cout << "삽입걸린시간 : " << result_insert.count() << "us" << endl;
    cout << "삭제걸린시간 : " << result_delete.count() << "us" << endl;

    return 0;
}