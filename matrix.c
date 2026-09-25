#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void generate_random_matrix(int rows, int cols, int *matrix) {
    for (int col = 0; col < cols; col++) {
        for (int row = 0; row < rows; row++) {
            matrix[row * cols + col] = rand() % 100; // % 100 for easier STDOUT
        }
    }
}

void multiply_matrices(int rows1, int cols1, int *matrix1,
                       int rows2, int cols2, int *matrix2,
                       int *result) {
    // dot multiply row1 with col1, then row1 with col2, etc...
    for (int row1 = 0; row1 < rows1; row1++) {
    for (int col2 = 0; col2 < cols2; col2++) {
        int sum = 0;
        for (int k = 0; k < cols1; k++) { // cols1 determines # of multiplcations
            sum += matrix1[row1 * cols1 + k] * matrix2[k * cols2 + col2];
             // gets kth item from each matrix and does dot multiplication
        }
        result[row1 * cols2 + col2] = sum;
    }
}
}

void display_matrix(int rows, int cols, int *matrix) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            printf("%d ", matrix[row * cols + col]);
        }
        printf("\n");
    }
}

float do_job(int rows1, int cols1, int cols2, int forever) {
    int rows2 = cols1;
    struct timespec t0, t1;
    int *matrix1 = malloc(rows1 * cols1 * sizeof(int));
    int *matrix2 = malloc(rows2 * cols2 * sizeof(int));
    int *result = malloc(rows1 * cols2 * sizeof(int));
    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        printf("Can't allocate\n");
        free(matrix1);
        free(matrix2);
        free(result);
        return -1.0f; // error, so exit
    }

    do {
        generate_random_matrix(rows1, cols1, matrix1);
        generate_random_matrix(rows2, cols2, matrix2);

        timespec_get(&t0, TIME_UTC);  // timer start, C11 feature
        multiply_matrices(rows1, cols1, matrix1,
                          rows2, cols2, matrix2,
                          result);
        timespec_get(&t1, TIME_UTC);  // timer stop

        // To STDOUT
        printf("Matrix 1:\n");
        display_matrix(rows1, cols1, matrix1);

        printf("\nMatrix 2:\n");
        display_matrix(rows2, cols2, matrix2);

        printf("\nMatrix Multiplication:\n");
        display_matrix(rows1, cols2, result);

    } while (forever);
   
    free(matrix1);
    free(matrix2);
    free(result);

    // nano seconds elapsed converted to fractional seconds
    float dns = (float)(t1.tv_nsec - t0.tv_nsec) / 1000000000;
    // seconds elapsed
    float ds = (float)(t1.tv_sec - t0.tv_sec);

    float total_time = dns+ds;
    return total_time;
}