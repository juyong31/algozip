#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100 // SIZE값을 변경해가며 list 개수 조절
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


    printf("%d개의 데이터\n", SIZE);

    printf("---교환정렬---\n");
    for (int j = 0; j < 5; j++) // 5회 실행 평균구하기
    {
        for (int k = 0; k < SIZE; k++) // 실행마다 랜덤값 생성
        {
            arr[k] = rand() % SIZE;
            initarr();
        }
        start = clock(); // 정렬 전 시간 측정
        exchangesort(list); // 정렬
        finish = clock(); //정렬 후 시간측정
        index = totalT(); // 정렬 전후값을 통해 측정시간 도출
    }
    avgT(index); // 5회 실행 시간을 모두 종합해 평균시간 도출
    sum_time = 0; // sum_time 초기화
    printf("\n");

    printf("---병합정렬---\n");
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

    printf("---퀵정렬---\n");
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

    printf("---힙정렬---\n");
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

    printf("---기수정렬---\n");
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

void avgT(double i) // 5회의 평균 실행시간 도출
{
    avg_time = (double)i / 5;
    printf("5회시도 평균시간 : %f sec", (double)avg_time / CLOCKS_PER_SEC);
}
void exchangesort(int list[]) // 교환정렬
{
    int i, j, index;
    for (i = 0; i < SIZE; i++)
    {
        for (j = i+1; j < SIZE; j++)
        {
            if (list[i] > list[j]) // 값이 더 큰 숫자가 더 앞에 위치할경우 교환을 통해 정렬
            {
                index = list[i];
                list[i] = list[j];
                list[j] = index;
            }
        }
    }
}

void merge(int list[], int left, int mid, int right) // 병합과정
{
    int i, j, k, l;
    i = left;
    j = mid + 1;
    k = left;

    while (i <= mid && j <= right) // mid기준 왼쪽과 오른쪽 중 한 쪽이 다 소진될 때 까지 반복
    {
        if (list[i] <= list[j]) // list[i]의 값이 더 작을 경우 sorted에 추가
        {
            sorted[k] = list[i];
            k++;
            i++;
        }
        else // list[j]의 값이 더 작을 경우 sorted에 추가
        {
            sorted[k] = list[j];
            k++;
            j++;
        }
    }
    // 왼쪽과 오른쪽 중 한쪽이 모두 다 소진
    if (i > mid) // 왼쪽을 모두 소진하여 오른쪽 남은 숫자들을 sorted에 추가
        for (l = j; l <= right; l++)
        {
            sorted[k] = list[l];
            k++;
        }
    else // 오른쪽을 모두 소진하여 왼쪽 남은 숫자들을 sorted에 추가
        for (l = i; l <= right; l++)
        {
            sorted[k] = list[l];
            k++;
        }
    for (l = left; l <= right; l++) // sorted에 정렬된 값을 list에 복사
        list[l] = sorted[l];
}
void merge_sort(int list[], int left, int right) //병합정렬 함수
{
    int mid;
    if (left < right)
    {
        mid = (left + right) / 2; // 중간값 도출
        merge_sort(list, left, mid); // 중간값 기준 왼쪽 정렬
        merge_sort(list, mid + 1, right); // 중간값 기준 오른쪽 정렬
        merge(list, left, mid, right); // 정렬된 값 병합
    }
}



int partition(int list[], int left, int right) 
{
    int pivot = list[left]; 
    int index;
    int low = left + 1;
    int high = right;



    while (low <= high) // 리스트의 모든 값을 수행하기 전까지 계속 반복한다
    {

        while (low <= right && list[low] <= pivot) // 리스트 내에 pivot값보다 큰 값이 나올 경우 중지
        {
            low++; // low값을 계속 증가해가며 리스트 내 값을 pivot과 비교
        }


        while (high >= (left+1) && list[high] >= pivot) // 리스트 내에 pivot값보다 작은 값이 나올 경우 중지
        {
            high--; // high값을 계속 감소해가며 리스트 내 값을 pivot과 비교
        }

        if (low <= high) // 아직 리스트 전체를 확인한 상황이 아닐경우 swap
        {
            index = list[low];
            list[low] = list[high];
            list[high] = index;
        }
    }
    // 초기에 선정한 pivot과 high값을 교체
    index = list[left];
    list[left] = list[high];
    list[high] = index;

    return high; // high위치 return
}
void quick_sort(int list[], int left, int right)
{
    if (left <= right)
    {
        int i = partition(list, left, right);
        quick_sort(list, left, i - 1); // 왼쪽(pivot보다 작은 값)영역을 sort
        quick_sort(list, i + 1, right); // 오른쪽(pivot보다 큰 값)영역을 sort
    }
}

void heapify(int list[], int root, int n)
{
    int left, right, index;
    left = root * 2 + 1; // 자식노드 왼쪽
    right = root * 2 + 2; // 자식노드 오른쪽
    int max = root;


    if (left < n && list[left] > list[max]) // 왼쪽 자식의 값이 부모보다 클 경우 교환
        max = left;
    if (right < n && list[right] > list[max]) // 오른쪽 자식의 값이 부모보다 클 경우 교환
        max = right;

    if (max != root) // root자리에 max값이 들어갈 때 까지 교환해가며 반복
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
    for (i = n / 2 - 1; i >= 0; i--) // 부모노드값을 구해서 heapify를 역순으로 호출
    {
        heapify(list, i, n);
    }
}

void heap_sort(int list[], int n)
{
    int vol, index;
    build_heap(list, n);
    for (vol = n - 1; vol >= 0; vol--) // 가장 큰 값부터 수를 줄여가며 정렬
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

void radix_sort(int list[], int n) // 기수정렬
{
    int max = list[0];
    int num = 0;
    int div = 1;
    int i,j,k;

    for (i = 0; i < n; i++) // 최댓값 구하기
    {
        if (max < list[i])
        {
            max = list[i];
        }
    }
    for (i = max; i > 0; i /= 10) // 최댓값 자릿수 구하기
    {
        num++;
    }

    for (i = 0; i < num; i++)
    {
        for (j = 0; j < 10; j++) // 0~9까지 숫자 중 맞는 자리에 나올 때 까지
        {
            for (k = 0; k < n; k++)
            {
                if ((list[k] / div) % 10 == j) // 맞는 자릿수가 나오면 큐에 push
                {
                    push(list[k]);
                }
            }
        }
        div *= 10; // 가장 큰 숫자의 자릿수 만큼 될 때 까지 자릿수 분할
        for (i = front; i < rear; i++) // 정렬 된 숫자들을 리스트에 순서대로 추가
        {
            list[i] = get();
        }
    }
}