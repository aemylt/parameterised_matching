#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "p_match.h"

int compare_string(char* A, char* B, int m) {
    int i;
    for (i = 0; i < m; i++) {
        if (A[i] != B[i]) return 0;
    }
    return 1;
}

int main(void) {
    int n = 10, m = 3;
    char* preprocessed = malloc(n * sizeof(char));
    preprocess("abcabcdeab", n, "abc", m, preprocessed);
    assert(compare_string(preprocessed, "abcabcddab", 10));
    printf("All tests succeeded!\n");
    return 0;
}