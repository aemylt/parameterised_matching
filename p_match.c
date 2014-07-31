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

void test_match(char* T, int n, char* P, int m, char* sigma, int s_sigma, char* pi, int s_pi, int correct_matches, int* correct) {
    int* output = malloc((n - m + 1) * sizeof(int));
    int matches = p_match(T, n, P, m, sigma, s_sigma, pi, s_pi, output);
    assert(correct_matches == matches && compare(output, correct, matches));
    if (matches > 0) free(output);
}

int main(void) {
    int n = 10, m = 3;
    char* preprocessed = malloc(n * sizeof(char));
    preprocess("abcabcdeab", n, "abc", m, preprocessed);
    assert(compare_string(preprocessed, "abcabcddab", 10));

    n = 18; m = 5;
    int correct_matches = 1;
    int* correct = malloc(correct_matches * sizeof(int));
    correct[0] = 8;
    test_match("ababaabbababcababd", n, "ababc", m, "cd", 2, "ab", 2, correct_matches, correct);

    n = 18; m = 5;
    correct_matches = 0;
    correct = realloc(correct, correct_matches * sizeof(int));
    test_match("ababaabbababdababd", n, "ababc", m, "cd", 2, "ab", 2, correct_matches, correct);

    printf("All tests succeeded!\n");
    return 0;
}
