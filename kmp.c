#include "kmp.h"
#include "shared_funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
    int n = 18, m = 5;
    int correct_matches = 2;
    int* correct = malloc(correct_matches * sizeof(int));
    correct[0] = 8; correct[1] = 13;
    int* output = malloc((n - m + 1) * sizeof(int));
    int matches = kmp_match("ababaabbababbababb", 18, "ababb", 5, output);
    assert(correct_matches == matches && compare(output, correct, matches));
    free(correct);
    free(output);
    printf("All tests succeeded!\n");
    return 0;
}