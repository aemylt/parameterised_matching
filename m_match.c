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
    int n;
    int m = 10;
    char* p = "aaaaabbbbb";
    int* A = (int*)malloc(m * sizeof(int));
    int* correct = (int*)malloc(m * sizeof(int));
    correct[0] = 0; correct[1] = 0; correct[2] = 1; correct[3] = 2; correct[4] = 3;
    correct[5] = 5; correct[6] = 5; correct[7] = 6; correct[8] = 7; correct[9] = 8;
    construct_table(p, m, A);
    assert(compare(A, correct, m));
    assert(!compare_pi_tj(4, p, 5, A));
    assert(compare_pi_tj(4, p, 4, A));
    assert(compare_pi_tj(0, p, 0, A));
    assert(!compare_pi_pj(4, p, 5, A));
    assert(compare_pi_pj(5, p, 5, A));
    assert(compare_pi_pj(0, p, 0, A));

    n = 15; m = 5;
    int correct_matches = 2;
    correct = realloc(correct, correct_matches);
    correct[0] = 0; correct[1] = 10;
    int* output = malloc((n - m) * sizeof(int));
    int matches = mmatch_match("ababbaaaaababaa", n, "ababb", m, output);
    assert(correct_matches == matches && compare(output, correct, matches));

    n = 18; m = 5;
    correct_matches = 3;
    correct = realloc(correct, correct_matches);
    correct[0] = 1; correct[1] = 8; correct[2] = 13;
    output = realloc(output, (n - m) * sizeof(int));
    matches = mmatch_match("ababaabbababbababb", n, "ababb", m, output);
    assert(correct_matches == matches && compare(output, correct, matches));

    n = 18; m = 5;
    correct_matches = 0;
    correct = realloc(correct, correct_matches);
    output = realloc(output, (n - m) * sizeof(int));
    matches = mmatch_match("ababababababababab", n, "ababb", m, output);
    assert(correct_matches == matches);

    free(A);

    printf("All tests succeeded!\n");
    return 0;
}
