#include <stdio.h>

int node = 6; // ��尳��
int INF = 10000000; // �̾����� ���� ��带 ����(ū ���� �����Ͽ� �ִܰŸ��� �������� �ʵ��� �ϱ�����)
bool visit[6]; // �̹� �湮 �� ��带 ����
int length[6]; // �Ÿ��� ����
int i, min, index, asc;

// �ٸ� ������ weight�� ����
int graph[6][6] = { 
	{0,8,7,20,14,INF},
	{INF,0,INF,13,INF,INF},
	{INF,INF,0,INF,5,INF},
	{12,INF,INF,0,INF,INF},
	{11,INF,INF,6,0,4},
	{INF,INF,INF,10,INF,0},
};

// �湮���� �ʴ� ��� ��, �湮 �� ���� ���� �ּҰŸ��� �̾��� ��带 ��ȯ
int Short_index()
{
	min = INF; //�ʱⰪ�� ���Ѵ�� ����
	index = 0;
	for (i = 0; i < node; i++)
	{
		if (length[i] < min) // �ּڰ����� �� ���� ���� �������
			if (!visit[i]) // �湮���� ���� ���鸸 ���
			{
				min = length[i]; // �ּڰ� ����
				index = i;
			}
	}
	return index;
}


void dijkstra(int start)
{
	for (i = 0; i < node; i++)
	{
		length[i] = graph[start][i]; // �ʱ� �Ÿ��� ���������� �� �������� �Ÿ��� ����
	}

	visit[start] = true; //�湮�� ���� trueó���Ͽ� �湮���� ���� ���� ����

	for (int i = 0; i < node - 1; i++)
	{
		int current = Short_index(); // �湮���� ���� ��� �� ���� �Ÿ��� ����� ��带 ����
		visit[current] = true; // �ش� ��带 �湮�� ������ ó��

		for (int j = 0; j < 6; j++) // �ּ� ��� ����
		{
			if (!visit[j])
			{
				if (length[current] + graph[current][j] < length[j]) // �ش��带 ���ļ� �������� ���� ��츦 ������İ� ��
				{
					length[j] = length[current] + graph[current][j]; // �� ª����� ����
				}
			}
		}
	}
}

// start�������� ��� ���������� �ִܰ�� ���
int main(void)
{
	dijkstra(0); // start point
	asc = 65;
	printf("----�ּҰŸ�----\n");
	for (i = 0; i < node; i++)
	{
		printf("%c : %d \n", asc, length[i]);
		asc++;
	}
	printf("\n");
}