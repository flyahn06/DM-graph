#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_VERTICES 101
#define MAX_QUEUE_SIZE 101

// 큐(Queue) 구조체 및 함수 (BFS용)
typedef struct {
    int items[MAX_QUEUE_SIZE];  // 데이터 저장할배열
    int front;  // 큐의 맨앞의 인덱스를 가리키는 변수(출구)
    int rear;  // 큐의 맨뒤의 인덱스를 가리키는 변수(입구)
} Queue;

typedef struct {
    int matrics_count;  // 파일로부터 읽은 행렬 개수
    int *matrics_size;  // 읽어들인 행렬들의 크기
    int ***biased_adjacent_matrics;  // 실제 가중인접행렬들의 배열
} biased_matrics;

// traverse.c
void initQueue(Queue *);
int isEmpty(Queue *);
void enqueue(Queue *, int);
int dequeue(Queue *);
void dfs_recursive(int, int);
void dfs(int);
void bfs(int);

int adjMatrix[MAX_VERTICES][MAX_VERTICES];
int visited[MAX_VERTICES];
int numVertices;

// dijkstra.c
int **allocate_matrix(int);
void read_file(const char *, biased_matrics *);
int min(int *, int, int*, int);
int in(int, int*, int);
void dijkstra(biased_matrics *);
void update_path(int **, int, int, int);
void free_all(biased_matrics *);


int main() {
    int graphCount = 1;  // 몇 번째 그래프인지
    char line[1024];
    char *ptr;
    int u, v;
    FILE* fp;
    biased_matrics matrics;

    fp = fopen("test/input1_1.txt", "r");

    if (fp == NULL) {  // 파일을 읽어오지 못한 경우
        printf("파일을 열 수 없습니다\n");
        return 1;
    }


    printf("1. 그래프 탐방 수행 결과\n\n");

    // 파일에서 정점 개수를 읽어옴 (더 이상 읽을 게 없으면 종료)
    while (fscanf(fp, "%d", &numVertices) != EOF) {

        // 인접 행렬 초기화
        for (int i = 0; i < MAX_VERTICES; i++) {
            for (int j = 0; j < MAX_VERTICES; j++) {
                adjMatrix[i][j] = 0;
            }
        }

        fgets(line, sizeof(line), fp);

        // 정점의 개수만큼 줄을 읽어서 인접 행렬 구성
        for (int i = 0; i < numVertices; i++) {
            if (fgets(line, sizeof(line), fp) == NULL) {
                break; // EOF
            }

            // line: [기준 정점] [인접 정점]*
            ptr = strtok(line, " \t\n");
            u = atoi(ptr); // 기준 정점

            while ((ptr = strtok(NULL, " \t\n")) != NULL) {
                v = atoi(ptr); // 그 뒤는 인접 정점들
                adjMatrix[u][v] = 1;
            }
        }

        printf("그래프 [%d]\n", graphCount++);
        printf("--------------------------------\n");

        // DFS
        printf("깊이 우선 탐색\n");
        dfs(1);

        printf("\n");

        // BFS
        printf("너비 우선 탐색\n");
        bfs(1);

        printf("================================\n\n");
    }

    fclose(fp);

    read_file("test/input2_1.txt", &matrics);
    dijkstra(&matrics);
    free_all(&matrics);

    return 0;
}

void initQueue(Queue* q) {
    // q의 front 를 -1로 초기화함
    // 비어있다는걸로 인식하기위해
    q->front = -1;
    q->rear = -1;
}

// 큐가 비어있는지 체크해주는 함수
int isEmpty(Queue* q) {
    return q->front == -1;
}

// 탐색할 정점 enqueue
void enqueue(Queue* q, int value) {
    if (q->rear == MAX_QUEUE_SIZE - 1) {
        // 큐가 가득찬 경우 넣으면 안되므로 return함
        // (정점 100개가 최대란 가정하에)
        return;
    }
    if (q->front == -1) {
        // 초기 상태의 큐인 경우
        // front 0의 값을 0으로 바꿔 데이터가 존재함을 표시
        q->front = 0;
    }
    q->rear++;
    q->items[q->rear] = value;
}

int dequeue(Queue* q) {
    if (isEmpty(q)) {
        return -1; // 큐가 비어 있어 dequeue가 불가능한 경우
    }
    int item = q->items[q->front]; // 선입선출이라 front가 가리키는 위치의 원소 리턴
    q->front++;

    // 큐의 front가 rear를 앞지른다면 큐에 남은 데이터가 없다는 뜻이므로
    // 둘다 인덱스를 -1로해줘큐를 재사용하게 해줍니다.
    if (q->front > q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    return item;
}

// 깊이 우선 탐색 (DFS) 재귀 함수 부분
void dfs_recursive(int node, int start) {
    visited[node] = 1; // 현재 노드를 반문했다고 표시 (1 = 방문함)
    if (start)
        printf("%d", node); // 현재 방문한 정점을 출력
    else
        printf(" - %d", node);
    // 현재 노드와 연결된 정점들을 1부터 차례대로 확인
    for (int i = 1; i <= numVertices; i++) {
        // adjMatrix[node][i] == 1 : 연결됨
        // visited[i] == 0 : 아직 방문 x
        if (adjMatrix[node][i] == 1 && visited[i] == 0) {
            dfs_recursive(i, 0); // 재귀 DFS
        }
    }
}

// DFS 전체 실행 함수 (시작 시 visited 배열 초기화)
void dfs(int startNode) {
    // 방문 배열 초기화
    // 0 = 방문하지 않은 상테
    // 이전 그래프 방문 혼작이 남아있지 않도록 완전히 초기화
    for (int i = 1; i <= numVertices; i++)
        visited[i] = 0;
    // 재귀 DFC 시작
    // startNode(보통 1번)에서 탐색을 시작함
    dfs_recursive(startNode, 1);
    // 3. 줄바꿈 (출력 마무리)
    printf("\n");
}

// 너비 우선 탐색 (BFS) 함수
void bfs(int startNode) {
    Queue q;
    initQueue(&q);


    // 1. 방문 배열 초기화 (0: 방문안함)
    for (int i = 1;  // 0이아니라 1인이유 맨위에서 말했듯이 점점=인덱스해서 방문한정점 보게할려고
        i <= numVertices; i++) {
        visited[i] = 0;  // 그리고 이전그래프에서 1로 막 되어있으니깐 0으로 다시 다 초기화
        }

    // 2. 시작 정점 처리
    enqueue(&q, startNode);  // 1 넣음으로써 시작
    visited[startNode] = 1;  // 1이면 방문한것
    printf("%d", startNode);  // 첫 정점 출력

    // 3. 큐가 빌 때까지 반복
    while (!isEmpty(&q)) {
        int current = dequeue(&q);  // 맨앞에있는걸 빼고 그게 지금 현재정점으로 넣어줌

        // 현재 정점(current)과 연결된 모든 정점(i)을 확인
        // 인접 행렬을 1부터 N까지 순회하므로 자동으로 오름차순 방문이 됨
        for (int i = 1; i <= numVertices; i++) {
            // 현재 정점과연결되어 있고(adjMatrix==1), 아직 방문하지 않았다면(visted==0)
            if (adjMatrix[current][i] == 1 && visited[i] == 0) {
                visited[i] = 1;       // 방문 처리
                printf(" - %d", i);
                enqueue(&q, i);       // 큐에 삽입 (나중에 이정점가서 이정점이랑 연결된것도 찾아야하니깐 )
            }
        }
    }
    printf("\n");
}

void dijkstra(biased_matrics *matrics) {
    int **current_matrix;
    int current_iter;
    int **path;
    int size;
    int dist;
    int *pp;
    int *D;
    int *S;
    int w;

    if (matrics->matrics_count == 0) return;

    printf("\n2. 최단 경로 구하기 수행 결과\n\n");

    for (int k = 0; k < matrics->matrics_count; k++) {
        printf("그래프 [%d]\n", k + 1);
        printf("----------------------------\n");
        printf("시작점: 1\n");

        size = matrics->matrics_size[k];
        current_matrix = matrics->biased_adjacent_matrics[k];

        // 시작 노드
        w = 0;

        // 결과 배열
        D = malloc(sizeof(int) * size);
        for (int i = 0; i < size; i++) {
            D[i] = current_matrix[w][i];
        }

        S = malloc(sizeof(int) * size);
        S[0] = 0;  // 시작 노드는 포함되어야 함
        for (int i = 1; i < size; i++)
            S[i] = -1;  // 비어 있는 공간이라는 표시로 -1을 사용함

        path = malloc(sizeof(int *) * size);
        for (int i = 0; i < size; i++) {
            path[i] = malloc(sizeof(int) * size);
            for (int j = 0; j < size; j++) {
                // 비어 있는 공간이라는 표시로 -1을 사용함
                path[i][j] = -1;
            }

            if (current_matrix[0][i] != -1) {  // 1번 노드로부터 해당 정점까지 길이 있는 경우
                // 경로 생성
                path[i][0] = 0;
                path[i][1] = i;
            }
        }

        current_iter = 1;
        for (int i = 0; i < size - 2; i++) {
            w = min(D, size, S, current_iter);  // V-S에 속하는 정점 중 D가 최소인 정점 탐색
            S[current_iter] = w;

            for (int j = 0; j < size; j++) {
                // S에 있는 정점들은 더 이상 업데이트할 필요가 없기 때문에
                // S에 속한 정점들은 스킵해야 함
                if (in(j, S, current_iter + 1)) continue;

                // 경로가 없는 경우에도 스킵해야 함
                // 이때 주어지는 그래프는 연결 그래프로 최소한 하나의 경로가 있음이 보장되어 있기 때문에
                // 별다른 처리 없이 그냥 스킵해도 문제없음
                if (current_matrix[w][j] == -1) continue;

                dist = current_matrix[w][j] + D[w];

                if (D[j] == -1 || D[j] > dist) {
                    // 경로가 처음 생성되는 경우거나 완전히 다른 경로로 업데이트되는 경우
                    // 이 경우 현재 w까지의 경로 + w에서 해당 노드까지의 경로로 바뀜
                    D[j] = dist;
                    update_path(path, w, j, size);
                }
            }
            current_iter++;
        }

        for (int i = 1; i < size; i++) {
            pp = path[i];
            printf("정점 [%d]: %d", i + 1, (*pp++) + 1);
            while (*pp != -1) {
                printf(" - %d", (*pp++) + 1);
            }
            printf(", 길이: %d\n", D[i]);
        }
        printf("============================\n\n");

        for (int i = 0; i < size; i++) {
            free(path[i]);
        }
        free(path);
        free(D);
        free(S);
    }
}

// to를 with로 덮어쓰며 마지막에 to 추가
void update_path(int **path, int with, int to, int pathsize) {
    int *wp = path[with];
    int *tp = path[to];

    memset(tp, -1, sizeof(int) * pathsize);

    while (*wp != -1)
        *tp++ = *wp++;

    *tp = to;
}

int in(int needle, int *haystack, int size) {
    for (int i = 0; i < size; i++) {
        if (haystack[i] == needle)
            return 1;
    }

    return 0;
}

int min(int *target, int size, int *without, int without_size) {
    int min_index = -1;
    int min_element = -1;

    for (int i = 0; i < size; i++) {
        if (target[i] < 0) continue;
        if (in(i, without, without_size)) continue;

        if (min_index == -1 || target[i] < min_element) {
            min_index = i;
            min_element = target[i];
        }
    }
    return min_index;
}

// n * n 크기의 행렬 생성
int **allocate_matrix(int n) {
    int **result = malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++) {
        result[i] = malloc(n * sizeof(int));

        for (int j = 0; j < n; j++) {
            result[i][j] = j == i ? 0 : -1;
        }
    }

    return result;
}

void read_file(const char* filename, biased_matrics *result) {
    int target_node, current_node, bias;
    char *line = malloc(0x200);
    int **adjacent_matrix;
    int total_matrics = 0;
    char *token;
    FILE *fp;
    int n;

    result->biased_adjacent_matrics = calloc(1, sizeof(int **));
    result->matrics_size = calloc(1, sizeof(int));

    fp = fopen(filename, "r");
    while (fgets(line, 0x200, fp) != NULL) {
        // 첫 번째 라인은 정점 개수
        n = atoi(line);
        adjacent_matrix = allocate_matrix(n);

        for (int i = 0; i < n; i++) {
            fgets(line, 0x200, fp);
            token = strtok(line, " ");
            current_node = atoi(token) - 1;  // 파일은 첫 번째 정점을 1로 표현하므로
            token = strtok(NULL, " ");

            // [edge가 있는 정점] [가중치] 형태로 짝을 이룸
            while (token != NULL) {
                target_node = atoi(token) - 1;
                token = strtok(NULL, " ");
                bias = atoi(token);
                token = strtok(NULL, " ");

                adjacent_matrix[current_node][target_node] = bias;
            }
        }

        // 하나 늘려 할당
        result->biased_adjacent_matrics = realloc(result->biased_adjacent_matrics, sizeof(int **) * (total_matrics + 1));
        result->matrics_size = realloc(result->matrics_size, sizeof(int) * (total_matrics + 1));
        result->biased_adjacent_matrics[total_matrics] = adjacent_matrix;
        result->matrics_size[total_matrics] = n;
        total_matrics++;
    }

    // 읽어들인 총 그래프 개수
    result->matrics_count = total_matrics;

    fclose(fp);
    free(line);
}

void free_all(biased_matrics *m) {
    for (int k = 0; k < m->matrics_count; k++) {
        int size = m->matrics_size[k];
        int **mat = m->biased_adjacent_matrics[k];

        for (int i = 0; i < size; i++)
            free(mat[i]);

        free(mat);
    }
    free(m->biased_adjacent_matrics);
    free(m->matrics_size);
}
