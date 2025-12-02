#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    int matrics_count;
    int *matrics_size;
    int ***biased_adjacent_matrics;
} biased_matrics;

int **allocate_matrix(int);
void read_file(const char *, biased_matrics *);
int min(int *, int, int*, int);
int in(int, int*, int);
void dijkstra(biased_matrics *);
void update_path(int **, int, int, int);
void free_all(biased_matrics *);
#ifdef DEBUG
void print_matrics(biased_matrics*);
#endif

int main() {
    biased_matrics matrics;

    setbuf(stdin, 0);
    setbuf(stdout, 0);
    read_file("test/input2_2.txt", &matrics);
#ifdef DEBUG
    print_matrics(&matrics);
#endif
    dijkstra(&matrics);
    free_all(&matrics);

    return 0;
}

void dijkstra(biased_matrics *matrics) {
    int *D;
    int *S;
    int w;
    int size;
    int **current_matrix;
    int current_iter;
    int dist;
    int **path;
    int *pp;

    if (matrics->matrics_count == 0) return;

    printf("\n2. 최단 경로 구하기 수행 결과\n\n");

    for (int k = 0; k < matrics->matrics_count; k++) {
        printf("그래프 [%d]\n", k + 1);
        printf("----------------------------\n");
        printf("시작점: 1\n");

        size = matrics->matrics_size[k];
        current_matrix = matrics->biased_adjacent_matrics[k];
        D = malloc(sizeof(int) * size);
        for (int i = 0; i < size; i++)
            D[i] = -1;
            
        S = malloc(sizeof(int) * size);
        for (int i = 0; i < size; i++) 
            S[i] = -1;   

        w = 0;

        path = malloc(sizeof(int *) * size);
        for (int i = 0; i < size; i++) {
            path[i] = malloc(sizeof(int) * size);
            for (int j = 0; j < size; j++) {
                path[i][j] = -1;
            }

            if (current_matrix[0][i] != -1) {
                path[i][0] = 0;
                path[i][1] = i;
            }
        }

        for (int i = 1; i < size; i++) {
            D[i] = current_matrix[w][i];
        }

        S[0] = 0;
        current_iter = 1;
        for (int i = 0; i < size - 2; i++) {
            w = min(D, size, S, current_iter);
            S[current_iter] = w;

            // compare
            for (int j = 0; j < size; j++) {
                if (in(j, S, current_iter + 1)) continue;
                if (current_matrix[w][j] == -1) continue;
                dist = current_matrix[w][j] + D[w];

                if (D[j] == -1 || D[j] > dist) {
                    // 경로가 처음 생성되는 경우거나 완전히 다른 경로로 업데이트되는 경우
                    // 이 경우 현재 w까지의 경로 + w에서 해당 노드까지의 경로로 바뀜
                    D[j] = dist;
#ifdef DEBUG
                    printf("update %d with %d\n", j, w);
#endif
                    update_path(path, w, j, size);

#ifdef DEBUG
                    for (int a = 1; a < size; a++) {
                        printf("%d\t", D[a]);
                    }
                    printf("\n");

                    for (int a = 0; a < size; a++) {
                        for (int b = 0; b < size; b++) {
                            printf("%d\t", path[a][b]);
                        }
                        printf("\n");
                    }
                    printf("\n");
#endif

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

# ifdef DEBUG
void print_matrics(biased_matrics *result) {
    for (int i = 0; i < result->matrics_count; i++) {
        for (int j = 0; j < result->matrics_size[i]; j++) {
            for (int k = 0; k < result->matrics_size[i]; k++) {
                printf("%d ", result->biased_adjacent_matrics[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
# endif

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
    FILE *fp;
    char *token;
    char *line = malloc(0x1000);
    int **adjacent_matrix;
    int total_matrics = 0;
    int target_node, current_node, bias;
    int n;

    result->biased_adjacent_matrics = calloc(1, sizeof(int **));
    result->matrics_size = calloc(1, sizeof(int));

    fp = fopen(filename, "r");
    while (fgets(line, 0x1000, fp) != NULL) {
        n = atoi(line);

        adjacent_matrix = allocate_matrix(n);

        for (int i = 0; i < n; i++) {
            fgets(line, 0x1000, fp);
            token = strtok(line, " ");
            current_node = atoi(token) - 1;
            token = strtok(NULL, " ");

            while (token != NULL) {
                target_node = atoi(token) - 1;
                token = strtok(NULL, " ");
                bias = atoi(token);
                token = strtok(NULL, " ");

                adjacent_matrix[current_node][target_node] = bias;
            }
        }

        result->biased_adjacent_matrics = realloc(result->biased_adjacent_matrics, sizeof(int **) * (total_matrics + 1));
        result->matrics_size = realloc(result->matrics_size, sizeof(int) * (total_matrics + 1));
        result->biased_adjacent_matrics[total_matrics] = adjacent_matrix;
        result->matrics_size[total_matrics] = n;
        total_matrics++;
    }

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
