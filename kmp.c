#include "kmp.h"
#include "shared_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void stream_test(char* T, int n, char* P, int m, int* correct) {
    kmp_state state = kmp_build(P, m);
    int j;
    for (j = 0; j < n; j++) {
        assert(correct[j] == kmp_stream(state, T[j]));
    }
}

int main(void) {
    int n = 18, m = 5;
    int correct_matches = 2;
    int* correct = malloc(correct_matches * sizeof(int));
    correct[0] = 8; correct[1] = 13;
    int* output = malloc((n - m + 1) * sizeof(int));
    int matches = kmp_match("ababaabbababbababb", n, "ababb", m, output);
    assert(correct_matches == matches && compare(output, correct, matches));
    correct = realloc(correct, n * sizeof(int));
    correct[0]  = -1; correct[1]  = -1; correct[2]  = -1; correct[3]  = -1; correct[4]  = -1; correct[5]  = -1;
    correct[6]  = -1; correct[7]  = -1; correct[8]  = -1; correct[9]  = -1; correct[10] = -1; correct[11] = -1;
    correct[12] = 12; correct[13] = -1; correct[14] = -1; correct[15] = -1; correct[16] = -1; correct[17] = 17;
    stream_test("ababaabbababbababb", n, "ababb", m, correct);
    free(correct);
    free(output);
    printf("All tests succeeded!\n");
    return 0;
}