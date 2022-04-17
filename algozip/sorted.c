#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100 // SIZE���� �����ذ��� list ���� ����
int arr[SIZE];
int list[SIZE];
int sorted[SIZE];
int front, rear = 0;
double start, finish, used_time = 0, avg_time = 0, sum_time = 0;

void initarr();
double totalT();
void avgT(double i);
void exchangesort(int list[]);
void merge(int list[], int left, int mid, int right);
void merge_sort(int list[], int left, int right);
int partition(int list[], int left, int right);
void quick_sort(int list[], int left, int right);
void heap_sort(int list[], int n);
void build_heap(int list[], int n);
void heapify(int list[], int root, int n);
void radix_sort(int list[], int n);
int get();
int push(int k);

int main()
{
    int i;
    double index;


    printf("%d���� ������\n", SIZE);

    printf("---��ȯ����---\n");
    for (int j = 0; j < 5; j++) // 5ȸ ���� ��ձ��ϱ�
    {
        for (int k = 0; k < SIZE; k++) // ���ึ�� ������ ����
        {
            arr[k] = rand() % SIZE;
            initarr();
        }
        start = clock(); // ���� �� �ð� ����
        exchangesort(list); // ����
        finish = clock(); //���� �� �ð�����
        index = totalT(); // ���� ���İ��� ���� �����ð� ����
    }
    avgT(index); // 5ȸ ���� �ð��� ��� ������ ��սð� ����
    sum_time = 0; // sum_time �ʱ�ȭ
    printf("\n");

    printf("---��������---\n");
    for (int j = 0; j < 5; j++)
    {
        for (int k = 0; k < SIZE; k++)
        {
            arr[k] = rand() % SIZE;
            initarr();
        }
        start = clock();
        merge_sort(list, 0, SIZE);
        finish = clock();
        index = totalT();
    }
    avgT(index);
    sum_time = 0;
    printf("\n");

    printf("---������---\n");
    for (int j = 0; j < 5; j++)
    {
        for (int k = 0; k < SIZE; k++)
        {
            arr[k] = rand() % SIZE;
            initarr();
        }
        start = clock();
        quick_sort(list, 0, SIZE - 1);
        finish = clock();
        index = totalT();
    }
    avgT(index);
    sum_time = 0;
    printf("\n");

    printf("---������---\n");
    for (int j = 0; j < 5; j++)
    {
        for (int k = 0; k < SIZE; k++)
        {
            arr[k] = rand() % SIZE;
            initarr();
        }
        start = clock();
        heap_sort(list, SIZE);
        finish = clock();
        index = totalT();
    }
    avgT(index);
    sum_time = 0;
    printf("\n");

    printf("---�������---\n");
    for (int j = 0; j < 5; j++)
    {
        for (int k = 0; k < SIZE; k++)
        {
            arr[k] = rand() % SIZE;
            initarr();
        }
        start = clock();
        radix_sort(list, SIZE);
        finish = clock();
        index = totalT();
    }
    avgT(index);
    sum_time = 0;
    printf("\n");
}


void initarr()
{
    int i;
    for (i = 0; i < SIZE; i++)
    {
        list[i] = arr[i];
    }
}

double totalT()
{
    used_time = finish - start;
    sum_time += used_time;
    return sum_time;
}

void avgT(double i) // 5ȸ�� ��� ����ð� ����
{
    avg_time = (double)i / 5;
    printf("5ȸ�õ� ��սð� : %f sec", (double)avg_time / CLOCKS_PER_SEC);
}
void exchangesort(int list[]) // ��ȯ����
{
    int i, j, index;
    for (i = 0; i < SIZE; i++)
    {
        for (j = i+1; j < SIZE; j++)
        {
            if (list[i] > list[j]) // ���� �� ū ���ڰ� �� �տ� ��ġ�Ұ�� ��ȯ�� ���� ����
            {
                index = list[i];
                list[i] = list[j];
                list[j] = index;
            }
        }
    }
}

void merge(int list[], int left, int mid, int right) // ���հ���
{
    int i, j, k, l;
    i = left;
    j = mid + 1;
    k = left;

    while (i <= mid && j <= right) // mid���� ���ʰ� ������ �� �� ���� �� ������ �� ���� �ݺ�
    {
        if (list[i] <= list[j]) // list[i]�� ���� �� ���� ��� sorted�� �߰�
        {
            sorted[k] = list[i];
            k++;
            i++;
        }
        else // list[j]�� ���� �� ���� ��� sorted�� �߰�
        {
            sorted[k] = list[j];
            k++;
            j++;
        }
    }
    // ���ʰ� ������ �� ������ ��� �� ����
    if (i > mid) // ������ ��� �����Ͽ� ������ ���� ���ڵ��� sorted�� �߰�
        for (l = j; l <= right; l++)
        {
            sorted[k] = list[l];
            k++;
        }
    else // �������� ��� �����Ͽ� ���� ���� ���ڵ��� sorted�� �߰�
        for (l = i; l <= right; l++)
        {
            sorted[k] = list[l];
            k++;
        }
    for (l = left; l <= right; l++) // sorted�� ���ĵ� ���� list�� ����
        list[l] = sorted[l];
}
void merge_sort(int list[], int left, int right) //�������� �Լ�
{
    int mid;
    if (left < right)
    {
        mid = (left + right) / 2; // �߰��� ����
        merge_sort(list, left, mid); // �߰��� ���� ���� ����
        merge_sort(list, mid + 1, right); // �߰��� ���� ������ ����
        merge(list, left, mid, right); // ���ĵ� �� ����
    }
}



int partition(int list[], int left, int right) 
{
    int pivot = list[left]; 
    int index;
    int low = left + 1;
    int high = right;



    while (low <= high) // ����Ʈ�� ��� ���� �����ϱ� ������ ��� �ݺ��Ѵ�
    {

        while (low <= right && list[low] <= pivot) // ����Ʈ ���� pivot������ ū ���� ���� ��� ����
        {
            low++; // low���� ��� �����ذ��� ����Ʈ �� ���� pivot�� ��
        }


        while (high >= (left+1) && list[high] >= pivot) // ����Ʈ ���� pivot������ ���� ���� ���� ��� ����
        {
            high--; // high���� ��� �����ذ��� ����Ʈ �� ���� pivot�� ��
        }

        if (low <= high) // ���� ����Ʈ ��ü�� Ȯ���� ��Ȳ�� �ƴҰ�� swap
        {
            index = list[low];
            list[low] = list[high];
            list[high] = index;
        }
    }
    // �ʱ⿡ ������ pivot�� high���� ��ü
    index = list[left];
    list[left] = list[high];
    list[high] = index;

    return high; // high��ġ return
}
void quick_sort(int list[], int left, int right)
{
    if (left <= right)
    {
        int i = partition(list, left, right);
        quick_sort(list, left, i - 1); // ����(pivot���� ���� ��)������ sort
        quick_sort(list, i + 1, right); // ������(pivot���� ū ��)������ sort
    }
}

void heapify(int list[], int root, int n)
{
    int left, right, index;
    left = root * 2 + 1; // �ڽĳ�� ����
    right = root * 2 + 2; // �ڽĳ�� ������
    int max = root;


    if (left < n && list[left] > list[max]) // ���� �ڽ��� ���� �θ𺸴� Ŭ ��� ��ȯ
        max = left;
    if (right < n && list[right] > list[max]) // ������ �ڽ��� ���� �θ𺸴� Ŭ ��� ��ȯ
        max = right;

    if (max != root) // root�ڸ��� max���� �� �� ���� ��ȯ�ذ��� �ݺ�
    {
        index = list[root];
        list[root] = list[max];
        list[max] = index;

        heapify(list, max, n);
    }
}

void build_heap(int list[], int n)
{
    int i, j;
    for (i = n / 2 - 1; i >= 0; i--) // �θ��尪�� ���ؼ� heapify�� �������� ȣ��
    {
        heapify(list, i, n);
    }
}

void heap_sort(int list[], int n)
{
    int vol, index;
    build_heap(list, n);
    for (vol = n - 1; vol >= 0; vol--) // ���� ū ������ ���� �ٿ����� ����
    {
        index = list[0];
        list[0] = list[vol];
        list[vol] = index;
        heapify(list, 0, vol);
    }
}

int push(int k) // push to Queue
{
    if ((rear + 1) % SIZE == front)
    {
        return;
    }
    else
    {
        list[rear] = k;
        rear = ++rear % SIZE;
        return 1;
    }
}

int get() // get from Queue
{
    int i;

    i = list[front];
    front = (front+1) % SIZE;
    return i;
}

void radix_sort(int list[], int n) // �������
{
    int max = list[0];
    int num = 0;
    int div = 1;
    int i,j,k;

    for (i = 0; i < n; i++) // �ִ� ���ϱ�
    {
        if (max < list[i])
        {
            max = list[i];
        }
    }
    for (i = max; i > 0; i /= 10) // �ִ� �ڸ��� ���ϱ�
    {
        num++;
    }

    for (i = 0; i < num; i++)
    {
        for (j = 0; j < 10; j++) // 0~9���� ���� �� �´� �ڸ��� ���� �� ����
        {
            for (k = 0; k < n; k++)
            {
                if ((list[k] / div) % 10 == j) // �´� �ڸ����� ������ ť�� push
                {
                    push(list[k]);
                }
            }
        }
        div *= 10; // ���� ū ������ �ڸ��� ��ŭ �� �� ���� �ڸ��� ����
        for (i = front; i < rear; i++) // ���� �� ���ڵ��� ����Ʈ�� ������� �߰�
        {
            list[i] = get();
        }
    }
}