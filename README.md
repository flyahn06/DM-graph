# 이산수학 프로젝트
## 1. 그래프 탐색 알고리즘
- 정진영
- 게렐토야

## 2. 다익스트라 알고리즘
- 추민규
- 안동기
---
1. `read_file(const char *filename, result *result)`  
파일을 읽어 가중인접행렬을 만듧니다. `struct result`는 다음과 같습니다.
```c
typedef struct {
    int matrics_count;  // 읽어들인 행렬 개수
    int *matrics_size;  // 읽어들인 행렬의 크기
    int ***biased_adjacent_matrics;  // 실제 가중인접행렬
} result;
```