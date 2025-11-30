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
void print_matrics(biased_matrics*);
int min(int *, int, int*, int);
int in(int, int*, int);
void dijkstra(biased_matrics *);

int main() {
    biased_matrics matrics;

    setbuf(stdin, 0);
    setbuf(stdout, 0);
    read_file("input2_1.txt", &matrics);
    print_matrics(&matrics);
    dijkstra(&matrics);

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

    if (matrics->matrics_count == 0) return;

    for (int i = 0; i < matrics->matrics_count; i++) {
        size = matrics->matrics_size[i];
        current_matrix = matrics->biased_adjacent_matrics[i];
        D = calloc(sizeof(int), size - 1);
        S = calloc(sizeof(int), size);
        w = 0;

        for (int i = 1; i < size; i++) {
            D[i] = current_matrix[w][i];
        }

        current_iter = 1;
        for (int i = 0; i < size - 2; i++) {
            w = min(D, size, S, current_iter);
            S[current_iter] = w;

            // compare
            for (int j = 0; j < size; j++) {
                if (in(j, S, current_iter + 1)) continue;
                if (current_matrix[w][j] == -1) continue;
                dist = current_matrix[w][j] + D[w];

                if (D[j] == -1)
                    D[j] = dist;
                else
                    D[j] = D[j] > dist ? dist : D[j];
            }
            current_iter++;
        }

        for (int i = 1; i < size; i++) {
            printf("%d\t", D[i]);
        }
        printf("\n");

        free(D);
        free(S);
    }
}

int in(int needle, int *haystack, int size) {
    for (int i = 0; i < size; i++) {
        if (haystack[i] == needle)
            return 1;
    }

    return 0;
}

int min(int *target, int size, int* without, int without_size) {
    int min_index;
    int min_element;

    if (size < 1) return -1;

    min_index = 0;
    min_element = target[0];

    for (int i = 0; i < size; i++) {
        if (in(i, without, without_size)) continue;
        if (target[i] == -1) continue;

        if (min_element == 0 || min_element > target[i]) {
            min_index = i;
            min_element = target[i];
        }
    }

    return min_index;
}

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
