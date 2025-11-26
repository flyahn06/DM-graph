#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct {
    int matrics_count;
    int *matrics_size;
    int ***biased_adjacent_matrics;
} result;

int **allocate_matrix(int);
void read_file(const char *, result *);
void print_matrics(result*);

int main() {
    result matrics;

    setbuf(stdin, 0);
    setbuf(stdout, 0);
    read_file("input2_2.txt", &matrics);
    print_matrics(&matrics);

    return 0;
}

void print_matrics(result *result) {
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
    int **result = calloc(n, sizeof(int *));

    for (int i = 0; i < n; i++) {
        result[i] = calloc(n, sizeof(int));
    }

    return result;
}

void read_file(const char* filename, result *result) {
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
