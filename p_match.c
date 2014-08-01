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

void stream_test(char* T, int n, char* P, int m, char* sigma, int s_sigma, char* pi, int s_pi, int* correct) {
    pmatch_state state = pmatch_build(P, m, sigma, s_sigma, pi, s_pi);
    int j;
    for (j = 0; j < n; j++) {
        assert(correct[j] == pmatch_stream(state, T[j]));
    }
}

int main(void) {
    int n = 18, m = 5;
    int correct_matches = 1;
    int* correct = malloc(correct_matches * sizeof(int));
    correct[0] = 8;
    test_match("ababaabbababcababd", n, "ababc", m, "cd", 2, "ab", 2, correct_matches, correct);

    n = 5; m = 5;
    correct_matches = 1;
    correct = realloc(correct, correct_matches * sizeof(int));
    correct[0] = 0;
    test_match("ababc", n, "ababc", m, "cd", 2, "ab", 2, correct_matches, correct);

    n = 18; m = 5;
    correct_matches = 0;
    correct = realloc(correct, correct_matches * sizeof(int));
    test_match("ababaabbababdababd", n, "ababc", m, "cd", 2, "ab", 2, correct_matches, correct);

    correct = realloc(correct, 18 * sizeof(int));
    correct[0]  = -1; correct[1]  = -1; correct[2]  = -1; correct[3]  = -1; correct[4]  = -1; correct[5]  = -1;
    correct[6]  = -1; correct[7]  = -1; correct[8]  = -1; correct[9]  = -1; correct[10] = -1; correct[11] = -1;
    correct[12] = 12; correct[13] = -1; correct[14] = -1; correct[15] = -1; correct[16] = -1; correct[17] = -1;
    stream_test("ababaabbababcababd", 18, "ababc", 5, "cd", 2, "ab", 2, correct);

    correct = realloc(correct, 5 * sizeof(int));
    correct[0]  = -1; correct[1]  = -1; correct[2]  = -1; correct[3]  = -1; correct[4]  = 4;
    stream_test("ababc", 5, "ababc", 5, "cd", 2, "ab", 2, correct);

    correct = realloc(correct, 18 * sizeof(int));
    correct[0]  = -1; correct[1]  = -1; correct[2]  = -1; correct[3]  = -1; correct[4]  = -1; correct[5]  = -1;
    correct[6]  = -1; correct[7]  = -1; correct[8]  = -1; correct[9]  = -1; correct[10] = -1; correct[11] = -1;
    correct[12] = -1; correct[13] = -1; correct[14] = -1; correct[15] = -1; correct[16] = -1; correct[17] = -1;
    stream_test("ababaabbababdababd", 18, "ababc", 5, "cd", 2, "ab", 2, correct);

    printf("All tests succeeded!\n");
    return 0;
}
