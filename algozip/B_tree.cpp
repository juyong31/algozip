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
	vector<int> keys; // key ���� ���� �� �迭
    vector<BTreeNode*> children; // pointer �迭�� ���� �ڽĻ��
	int t, key_num; // ���� Ű ��
	bool leaf;
	bool IsFull() const;
	bool IsEnough() const;
public:
	BTreeNode(int degree_in, bool _leaf); // ������
	void Traverse(); // Ʈ�� ��ȸ
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
	// ������
	BTree(int degree_in):
		root(nullptr),
		degree(degree_in)
	{
	}
	// �Ҹ���
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

	// Ʈ���� ��ȸ�ϴ� �Լ�
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

			// �ö� ���� ���ο� ��Ʈ�� �ǰ�, �ڽĿ��� ���� ��Ʈ
			newRoot->children[0] = root;

			// ������ �ִ� root���� split
			newRoot->SplitChild(0, root);

			// split�� ���� �����ڽ����� �������ڽ����� Ȯ��
            int i = newRoot->keys[0] < k ? 1 : 0;
			newRoot->children[i]->Insert(k);

			// ��Ʈ�� �缳��
			root = newRoot;
		}
		else root->Insert(k);
	}

	void Remove(int k)
	{
		if (!root) //Ʈ���� ������� ���
		{
			return;
		}

		root->Remove(k);

		// root�� numkey�� 0�϶��� üũ
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

// k�� ������ ���� ���ϰ�, B-Tree���� ã�� ����
BTreeNode* BTreeNode::Search(int k)
{
    if (key_num > 0)
    {
        // k���� ��ġ�� ã�´�.
        int i = 0;
        while (i < key_num && k > keys[i])
        {
            ++i;
        }
        // k���� ã�� ��� ��ȯ
        if (i < key_num && keys[i] == k)
        {
            return this;
        }
        // ã�������� ��� level�� �ٲ㰡�� Ž��
        if (children[i])
        {
            return children[i]->Search(k);
        }
        else return nullptr; //leaf node �� ��� ��ȯ (Ž������)
    }
    return nullptr; //leaf node �� ��� ��ȯ (Ž������)
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
            SplitChild(i + 1, children[i + 1]); //insert �ؾ��ϴ� �ڸ��� node�� full�ΰ�� �и�

            if (keys[i + 1] < k) //���� ���Ե� key������ ū�� Ȯ���Ѵ�.
                ++i;
        }
        children[i + 1]->Insert(k);
    }
}



// �ڽĳ�尡 full�� ��� �и����ִ� �۾��� �����Ѵ�.
void BTreeNode::SplitChild(int index, BTreeNode* left)
{
    BTreeNode* right = new BTreeNode(t, left->leaf);

    left->key_num = t - 1;
    right->key_num = t - 1;


    
    for (int i = 0; i < t - 1; ++i) //key���� �����Ѵ�.
    {
        right->keys[i] = left->keys[i + t];
        left->keys[i + t] = 0;
    }

    if (!left->leaf) // children�� �����Ѵ�.
    {
        for (int i = 0; i < t; ++i)
        {
            right->children[i] = left->children[i + t];
        }
    }

    for (int i = key_num; i > index; --i) // ���� children key���� ��ĭ�� �δ�(��� ���ο� Ű�� ���ԵǾ����)
    {
        keys[i] = keys[i - 1];
        children[i + 1] = children[i];
    }

    // �߰��� Ű�� �����ϰ� right children�� �����Ѵ�.
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

    // remove�ؾ��ϴ� index�� ��忡 ���� ���
    if (index < key_num && keys[index] == k)
    {
        // leafnode�� ���ο� ���� �ٸ� �Լ��� ����
        leaf ? Removeleaf(index) : RemoveNonleaf(index);
    }
    // remove�ؾ��ϴ� index�� �ش� ��忡 ���� ���
    else
    {
        // Ű�� ���� ���
        if (leaf)
        {
            return;
        }

        bool keyexist = (index == key_num);

        if (!children[index]->IsEnough()) //�ּ� key �������� ���� ��� ä���.
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

int BTreeNode::GetPred(int index) //�� �Ʒ� key�� �� ���� ������ key�� return
{
    BTreeNode* cur = children[index];
    while (!cur->leaf)
        cur = cur->children[cur->key_num];

    return cur->keys[cur->key_num - 1];
}

int BTreeNode::GetSucc(int index) //�� �Ʒ� key�� �� ���� ���� key�� return
{
    BTreeNode* cur = children[index + 1];
    while (!cur->leaf)
        cur = cur->children[0];

    return cur->keys[0];
}

void BTreeNode::RemoveNonleaf(int index)
{
    int k = keys[index];

    if (children[index]->IsEnough()) // k���� key���� �۾ƾ��ϴ� children�� t key ���� Ŭ ���
    {
        int pred = GetPred(index);
        keys[index] = pred;
        children[index]->Remove(pred);
    }
    else if (children[index + 1]->IsEnough()) // k���� key���� �۾ƾ��ϴ� children�� t key���� ���� ���
    {
        int succ = GetSucc(index);
        keys[index] = succ;
        children[index + 1]->Remove(succ);
    }
    else // child index�� child index+1�� t���� ���� key�� �������� ��� merge
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

    // ���� Ű�� �����Ͽ� �ִ´�.
    ++Childsize;
    for (int i = 0; i < Childsib->key_num; ++i)
        child->keys[i + Childsize] = Childsib->keys[i];

    // ���� �ڽĳ�带 �����Ͽ� �ִ´�.
    if (!child->leaf)
    {
        for (int i = 0; i <= Childsib->key_num; ++i)
            child->children[i + Childsize] = Childsib->children[i];
    }

    // �迭 ����
    for (int i = index + 1; i < key_num; ++i)
        keys[i - 1] = keys[i];
    for (int i = index + 2; i <= key_num; ++i)
        children[i - 1] = children[i];

    child->key_num += (Childsib->key_num + 1);
    --key_num;

    delete Childsib;
}

// children[index]�� key���� ������ ��� Fill
void BTreeNode::Fill(int index)
{
    // ���ʿ��� �������°��
    if (index != 0 && children[index - 1]->IsEnough())
    {
        takeFromPrev(index);
    }

    // �����ʿ��� �������� ���
    else if (index != key_num && children[index + 1]->IsEnough())
    {
        takeFromNext(index);
    }

    // ������� ������� merge (enough�� �������)
    else
    {
        index == key_num ? Merge(index - 1) : Merge(index);
    }
}

// children[index-1]�κ��� key���� �������� ���
void BTreeNode::takeFromPrev(int index)
{
    BTreeNode* child = children[index];
    BTreeNode* Childsib = children[index - 1];

    // children[index]�� ��������
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

    // �ڽİ� ���� Ű ���� �����Ѵ�.
    ++child->key_num;
    --Childsib->key_num;
}

// children[index+1]�κ��� key���� �������� ���
void BTreeNode::takeFromNext(int index)
{
    BTreeNode* child = children[index];
    BTreeNode* Childsib = children[index + 1];

    // keys[index]�� children[index]�� ������ Ű�� ���Եȴ�.
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

    system_clock::time_point start_insert = system_clock::now(); // ���Խ��۽ð�
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
    cout << "���Խ���" << endl;
    cout << "**********������**********" << endl;
    for (int i = 0; i < num; ++i)
    {
        t.Insert(arr[i]);
        
    }
    t.Traverse();
    cout << "��������" << endl;
    system_clock::time_point finish_insert = system_clock::now(); // ��������ð�
    cout << "**************************" << endl;
    system_clock::time_point start_delete = system_clock::now(); // �������۽ð�
    cout << "��������" << endl;
    cout << "**********������**********" << endl;
    for (int i = 0; i < num/2; ++i)
    {
        t.Remove(i + 1);
        t.Traverse();
    }
    cout << "��������" << endl;
    system_clock::time_point finish_delete = system_clock::now(); // ��������ð�
    cout << "**************************" << endl;

    microseconds result_insert = duration_cast<microseconds>(finish_insert - start_insert);
    microseconds result_delete = duration_cast<microseconds>(finish_delete - start_delete);

    cout << "���� : " << num << endl;
    cout << "t = : " << block_size - 1 << endl;
    cout << "���԰ɸ��ð� : " << result_insert.count() << "us" << endl;
    cout << "�����ɸ��ð� : " << result_delete.count() << "us" << endl;

    return 0;
}