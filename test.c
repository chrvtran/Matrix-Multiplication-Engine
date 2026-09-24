#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "matrix.h"
#include <math.h>


int test_matmul() {
    int mat1[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int mat2[3][3] = {
        {9, 8, 7},
        {6, 5, 4},
        {3, 2, 1}
    };

    int expected[3][3] = {
        {30, 24, 18},
        {84, 69, 54},
        {138, 114, 90}
    };

    int result[3][3] = {0};

    multiply_matrices(
        3, 3, (int*) mat1,
        3, 3, (int*)mat2,
        (int*)result
    );

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (result[i][j] != expected[i][j]) {
                printf("Incorrect MatMul Result at [%d][%d]\n", i, j);
                return 0;
            }
        }
    }

    return 1;
}


int test_matmul2() {
     // 4 x 3
    int mat1[4][3] = {
        {1,  2,  3},
        {4,  5,  6},
        {7,  8,  9},
        {10, 11, 12}
    };

    // 3 x 5
    int mat2[3][5] = {
        {1,  2,  3,  4,  5},
        {6,  7,  8,  9, 10},
        {11, 12, 13, 14, 15}
    };

    // 4 x 5
    int expected[4][5] = {
        {46,  52,  58,  64,  70},
        {100, 115, 130, 145, 160},
        {154, 178, 202, 226, 250},
        {208, 241, 274, 307, 340}
    };

    int result[4][5] = {0};

    multiply_matrices(
        4, 3, (int*)mat1,
        3, 5, (int*)mat2,
        (int*)result
    );

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            if (result[i][j] != expected[i][j]) {
                printf(
                    "Incorrect MatMul Result at [%d][%d]: "
                    "expected %d, got %d\n",
                    i, j, expected[i][j], result[i][j]
                );
                return 0;
            }
        }
    }

    return 1;
}


int test_FIFO() {
    int jobs1[] = {100,100,600,800};
    int jobs2[] = {600,800,100,100};

    float rsp = FIFO(jobs1,4);
    float rsp2 = FIFO(jobs2,4);

    if(rsp >= rsp2) {
        printf("First job should have shorter response time.\n");
        return 0;
    }

    return 1;
}


int test_SJF() {
    int jobs1[] = {300,300,300,600,100};
    int jobs2[] = {100,100,100,600,300};

    float rsp = SJF(jobs1,5);
    float rsp2 = SJF(jobs2,5);

    if(rsp <= rsp2) {
        printf("Second job list should have shorter response time.\n");
        return 0;
    }
    
    return 1;
}

int test_scheduling_metrics() {
    int jobs[] = {300, 200, 100};

    float fifo_response = FIFO(jobs, 3);
    float sjf_response = SJF(jobs, 3);

    if (fifo_response < 0.0f || sjf_response < 0.0f) {
        printf("Scheduling function returned an error.\n");
        return 0;
    }

    if (sjf_response >= fifo_response) {
        printf(
            "SJF should have a shorter average response time "
            "than FIFO.\n"
        );
        return 0;
    }

    return 1;
}

int run_test(char * test_name, int (*test_func)()) {
    int test_res = test_func();
    printf("Test %-25s: %d/1\n", test_name, test_res);
    return test_res;
}

int main(int argc, char **argv){
    if(argc != 2) {
        printf("ERROR: expected format ./test_one <test_num>\n");
        return -1;
    }
    int passed = 0;
    int test_num = atoi(argv[1]);
    
    char *test_list[] = {"Test One MatMul","Test Two MatMul", "Test FIFO", "Test SJF", "TestStats"};
    int (*test_func[])() = {&test_matmul, &test_matmul2, &test_FIFO, &test_SJF, &test_scheduling_metrics};

    passed += run_test(test_list[test_num], test_func[test_num]);
    int tests_ran = 1;

    printf("Total: %d/%d\n", passed, tests_ran);
    return passed;
}