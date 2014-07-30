#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "m_match.h"

int compare(int* A, int* B, int m) {
    int i;
    for (i = 0; i < m; i++) {
        if (A[i] != B[i]) return 0;
    }
    return 1;
}

int main(void) {
    int m = 10;
    int i;
    char* p = "aaaaabbbbb";
    int* A = (int*)malloc(m * sizeof(int));
    int* correct = (int*)malloc(m * sizeof(int));
    correct[0] = 0; correct[1] = 0; correct[2] = 1; correct[3] = 2; correct[4] = 3;
    correct[5] = 5; correct[6] = 5; correct[7] = 6; correct[8] = 7; correct[9] = 8;
    construct_table(p, m, A);
    assert(compare(A, correct, m));
    return 0;
}