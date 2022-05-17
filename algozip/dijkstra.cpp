#include <stdio.h>

int node = 6; // 노드개수
int INF = 10000000; // 이어지지 않은 노드를 저장(큰 수를 저장하여 최단거리에 관여되지 않도록 하기위함)
bool visit[6]; // 이미 방문 한 노드를 저장
int length[6]; // 거리를 저장
int i, min, index, asc;

// 다른 노드와의 weight을 저장
int graph[6][6] = { 
	{0,8,7,20,14,INF},
	{INF,0,INF,13,INF,INF},
	{INF,INF,0,INF,5,INF},
	{12,INF,INF,0,INF,INF},
	{11,INF,INF,6,0,4},
	{INF,INF,INF,10,INF,0},
};

// 방문하지 않는 노드 중, 방문 한 노드와 가장 최소거리로 이어진 노드를 반환
int Short_index()
{
	min = INF; //초기값을 무한대로 설정
	index = 0;
	for (i = 0; i < node; i++)
	{
		if (length[i] < min) // 최솟값보다 더 작은 수가 있을경우
			if (!visit[i]) // 방문하지 않은 노드들만 고려
			{
				min = length[i]; // 최솟값 변경
				index = i;
			}
	}
	return index;
}


void dijkstra(int start)
{
	for (i = 0; i < node; i++)
	{
		length[i] = graph[start][i]; // 초기 거리를 시작점에서 각 노드까지의 거리로 설정
	}

	visit[start] = true; //방문한 노드는 true처리하여 방문하지 않은 노드와 구분

	for (int i = 0; i < node - 1; i++)
	{
		int current = Short_index(); // 방문하지 않은 노드 중 가장 거리가 가까운 노드를 선정
		visit[current] = true; // 해당 노드를 방문한 것으로 처리

		for (int j = 0; j < 6; j++) // 최소 비용 갱신
		{
			if (!visit[j])
			{
				if (length[current] + graph[current][j] < length[j]) // 해당노드를 거쳐서 인접노드로 가는 경우를 기존방식과 비교
				{
					length[j] = length[current] + graph[current][j]; // 더 짧을경우 갱신
				}
			}
		}
	}
}

// start지점에서 모든 정점까지의 최단경로 출력
int main(void)
{
	dijkstra(0); // start point
	asc = 65;
	printf("----최소거리----\n");
	for (i = 0; i < node; i++)
	{
		printf("%c : %d \n", asc, length[i]);
		asc++;
	}
	printf("\n");
}
