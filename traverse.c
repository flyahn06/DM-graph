#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 101 //정점개수가 100개로 제한되어있어서 
//여유공간으로0~ 100로 설정했습니다.
//그리고 100이 아닌 101인 이유는 visisted할때 100번 정점이 방문했다도 표시하려면 0~100인 101개의 
//공간이 필요하기때문입니다+가시성     
#define MAX_QUEUE_SIZE 101

// 전역 변수 선언
int adjMatrix[MAX_VERTICES][MAX_VERTICES]; // 인접 행렬:저 값이 0이면 연결 정점2개가 연결x
  //저값이 1이면 두 정점이 연결되어있음.
int visited[MAX_VERTICES];                 // 방문 여부 체크 배열: 0이면 방문x 1이면 방문o
int numVertices;                           // 현재 그래프의 정점 개수

// 큐(Queue) 구조체 및 함수 (BFS용)
typedef struct {
    int items[MAX_QUEUE_SIZE];//데이터 저장할배열
    //인접할 정점들을 찾기위한 대기표같은느낌입니다.
    int front;//큐의 맨앞의 인덱스를 가르키는 변수(출구)
    int rear;//큐의 맨뒤의 인덱스를 가르키느 변수(입구)
} Queue;

void initQueue(Queue* q) {//맨처음 초기화해주는 함수입니당
    //포인터를 사용하는 이유는 함수같은거 전달할때 바뀐것을 반영하기위해
    //passing by reference했습니다.
    q->front = -1;//(그 매개변수로 받은 q의 front 를 -1로 초기화해줌)->비어있다는걸로 인식하기위해
    q->rear = -1;
}

int isEmpty(Queue* q) {
    return q->front == -1;//큐가 비어있는지 체크해주는 함수입니당
    //q의 front가 -1이라는 소리는 비어있다는 소리입니다.
}

void enqueue(Queue* q, int value) {//q와 대기표에 무슨 정점 들어갈지넣어주는것
    if (q->rear == MAX_QUEUE_SIZE - 1) {
        return; //만약 큐 가득차있을때는 넣으면안되니깐 return해줍니다.(정점100개가 최대란ㄴ 가정하에)
    }
    if (q->front == -1) {//만약 완전 처음일때는 front 0번째에로해줘 이제 데이터가 있습니다.라는것을 알려줍니다
        q->front = 0;
    }
    q->rear++;//한칸 뒤로옮겨서 이제 빈 새로운공간에 방문할 정점을 넣습니다.
    q->items[q->rear] = value;//그 아이템(대기표같은)에 rear의 값의 인덱스에 방문할 예정인 정점을 넣어줍니다.
}

int dequeue(Queue* q) {
    if (isEmpty(q)) {
        return -1;//비어있다면 비정상이라고알립니다(비어있는데 큐의 내용을 지울려고하면 안되니깐)
    }
    int item = q->items[q->front];//선입선출이라 front안에있는것을 item에넣어줍니다(나중에 리턴해줄려고)
    q->front++;//그리고 이제 한칸뒤로 땡겨줍니다
    if (q->front > q->rear) { // 큐의 front가 rear를 앞지르게되면 큐에 남은 데이커가 없다는 소리니깐
        q->front = -1;//만약 input의 그래프가 여러개일때 재사용하게하려고
        q->rear = -1;//둘다 인덱스를 -1로해줘큐를 재사용하게 해줍니다.
    }
    return item;//dequeue할떄 결국 이제 방문한거니깐 출력해야해서 item반환
}
//깊이 우선 탐색 (DFS) 재귀 함수 부분
void dfs_recursive(int node) {
    visited[node] = 1; // 현재 노드를 반문했다고 표시 ( 1 = 방문함)
    printf("%d", node); //현재 방문한 정점을 출력
    //현재 노드와 연결된 정점들을 1부터 차례대로 확인
    for (int i = 1; i <= numVertices; i++) {
		// adjMatrix[node][i] == 1 : 연겨됨
		// visited[i] == 0 : 아직 방문 x
        if (adjMartx[nodde][i] == 1 && visited[i] == 0) {
            dfs_recursivre(i); // 재귀 DFS
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
    dfs_recursive(startNode);
    // 3. 줄바꿈 (출력 마무리)
    printf("\n");
}

// 정진영-너비 우선 탐색 (BFS) 함수
void bfs(int startNode) {//시작노드를 매개변수로줍니다 사실 그냥 1임
    Queue q;//q는 스택메모리에 생성되고(스택,큐할때 스택아님)
    initQueue(&q);//함수들은 다 반영되어야하니깐 포인터 연산자로 넘겨주기
    //q초기화해주기 새로시작할거니깐
    // 
    // 1. 방문 배열 초기화//아까말했듯이 0이면 아직 방분안한겨
    for (int i = 1;//0이아니라 1인이유 맨위에서 말했듯이 점점=인덱스해서 방문한정점 보게할려고
        i <= numVertices; i++) {
        visited[i] = 0;//그리고 이전그래프에서 1로 막 되어있으니깐 0으로 다시 다 초기화
    }

    // 2. 시작 정점 처리
    
    enqueue(&q, startNode);//1 넣음으로써 시작
    visited[startNode] = 1;//1이면 방문한것
    printf("%d", startNode); // 첫 정점 출력 

    // 3. 큐가 빌 때까지 반복
    while (!isEmpty(&q)) {
        int current = dequeue(&q);//맨앞에있는걸 빼고 그게 지금 현재정점으로 넣어줌

        // 현재 정점(current)과 연결된 모든 정점(i)을 확인
        // 인접 행렬을 1부터 N까지 순회하므로 자동으로 오름차순 방문이 됨
        for (int i = 1; i <= numVertices; i++) {
            // 현재 정점과연결되어 있고(adjMatrix==1), 아직 방문하지 않았다면(visted==0)
            if (adjMatrix[current][i] == 1 && visited[i] == 0) {
                visited[i] = 1;       // 방문 처리
                printf(" - %d", i);   // 출력 형식 맞춰 프린트
                enqueue(&q, i);       // 큐에 삽입(나중에 이정점가서 이정점이랑 연결된것도 찾아야하니깐 )
            }
        }
    }
    printf("\n"); // 줄바꿈
}

int main() {
    FILE* fp = fopen("input1_2.txt", "r");//읽기모드로 파일읽어오기
    //여기서
    //input1_1하면 그래프가 한개인경우로 테스트할수있고
    //input1_2로하면 그래프가 여러개인경우로 테스트할수있습니다.



    if (fp == NULL) {//정상종료를ㄹ위해
        printf("파일을 열 수 없습니다: input1.txt\n");
        return 1;
    }

    int graphCount = 1;//출력할때 []안의 수
    char line[1024];//파일 한줄씩읽어올 버퍼

    // 파일에서 정점 개수를 읽어옴 (더 이상 읽을 게 없으면 종료)
    while (fscanf(fp, "%d", &numVertices) != EOF) {
        
        // 인접 행렬 초기화
        for (int i = 0; i < MAX_VERTICES; i++) {
            for (int j = 0; j < MAX_VERTICES; j++) {
                adjMatrix[i][j] = 0;
            }
        }

        // 줄바꿈 문자 제거 (fscanf 후 남은 \n 처리)
        fgets(line, sizeof(line), fp);

        // 정점의 개수만큼 줄을 읽어서 인접 행렬 구성
        for (int i = 0; i < numVertices; i++) {
            if (fgets(line, sizeof(line), fp) == NULL) {//fgets는 한줄일겅오는..
                break;//파일끝이라서 EOF라면 특수포인터값인 NULL이 반환될거라서 그때는 반복문 종료
            }

            // 한 줄 파싱 (예: "1 2 3" -> 정점 1은 2, 3과 연결됨)
            //데이터 쪼개는parsing말하는거입니다.
            char* ptr = strtok(line, " \t\n");//구분자를 " ","\t","\n"으로해서 line중에 저것들로 구분자만남ㄴ 쪼개기
            //그러고나면 line은[1널2 3\n널]이되어버림
            // ptr은  저line[0]의 주소를 들고있는상태인"1"의 주소를 들고있어       
            int u = atoi(ptr); // 첫 번째 숫자는 기준 정점(문자열이니깐 정수로바꿔주기)
            //atoi:ascii to integer 
            while ((ptr = strtok(NULL, " \t\n")) != NULL) {
                //strtok는 첫번째 부를때는 (짜를배열,구분자들)였다면
                //그후에(Null,구분자들)하면 아까 짜르던데서부터 이어서 짜르라는뜻입니다.
                int v = atoi(ptr); // 그 뒤는 인접 정점들
                
                adjMatrix[u][v] = 1;
            }
        }

        // 출력 호출
        printf("그래프 [%d]\n", graphCount++);
        printf("--------------------------------\n");

        // DFS 
        printf("깊이 우선 탐색\n");
        dfs(1); 

        printf("\n");

        // BFS 호출
        printf("너비 우선 탐색\n");
        bfs(1); // 1번 정점부터 시작

        printf("================================\n\n");
    }

    fclose(fp);
    return 0;
}
