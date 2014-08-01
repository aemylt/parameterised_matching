#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "m_match.h"
#include "trees/red_black_tree/red_black_tree/rbtree.c"
#include "shared_funcs.h"

void test_match(char* T, int n, char* P, int m, int correct_matches, int* correct) {
    int* output = malloc((n - m + 1) * sizeof(int));
    int matches = mmatch_match(T, n, P, m, output);
    assert(correct_matches == matches && compare(output, correct, matches));
    if (matches > 0) free(output);
}

void stream_test(char* T, int n, char* P, int m, int* correct) {
    mmatch_state state = mmatch_build(P, m);
    int j;
    for (j = 0; j < n; j++) {
        assert(correct[j] == mmatch_stream(state, T[j]));
    }
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
    rbtree test = rbtree_create();
    rbtree_insert(test, (void*)'a', (void*)3, compare_char);
    assert(!compare_pi_tj(4, 'b', test, 5, A));
    assert(compare_pi_tj(4, 'a', test, 4, A));
    rbtree_delete(test, (void*)'a', compare_char);
    assert(compare_pi_tj(0, 'a', test, 0, A));
    assert(compare_pi_tj(0, 'b', test, 5, A));
    assert(!compare_pi_pj(4, 5, A));
    assert(compare_pi_pj(5, 5, A));
    assert(compare_pi_pj(0, 0, A));
    assert(compare_pi_pj(4, 4, A));
    assert(compare_pi_pj(0, 5, A));

    m = 3;
    A = realloc(A, m * sizeof(int));
    construct_table("aba", m, A);
    assert(compare_pi_pj(1, 2, A));

    n = 15; m = 5;
    int correct_matches = 2;
    correct = realloc(correct, correct_matches * sizeof(int));
    correct[0] = 0; correct[1] = 10;
    test_match("ababbaaaaababaa", n, "ababb", m, correct_matches, correct);

    n = 15; m = 5;
    correct_matches = 2;
    correct = realloc(correct, correct_matches * sizeof(int));
    correct[0] = 0; correct[1] = 10;
    test_match("cdcddcccccdcdcc", n, "ababb", m, correct_matches, correct);

    n = 18; m = 5;
    correct_matches = 3;
    correct = realloc(correct, correct_matches * sizeof(int));
    correct[0] = 1; correct[1] = 8; correct[2] = 13;
    test_match("ababaabbababbababb", n, "ababb", m, correct_matches, correct);

    n = 18; m = 5;
    correct_matches = 0;
    correct = realloc(correct, correct_matches * sizeof(int));
    test_match("ababababababababab", n, "ababb", m, correct_matches, correct);

    correct = realloc(correct, 15 * sizeof(int));
    correct[0]  = -1; correct[1]  = -1; correct[2]  = -1; correct[3]  = -1; correct[4]  = 4;
    correct[5]  = -1; correct[6]  = -1; correct[7]  = -1; correct[8]  = -1; correct[9]  = -1;
    correct[10] = -1; correct[11] = -1; correct[12] = -1; correct[13] = -1; correct[14] = 14;
    stream_test("ababbaaaaababaa", 15, "ababb", 5, correct);
    correct[0]  = -1; correct[1]  = -1; correct[2]  = -1; correct[3]  = -1; correct[4]  = 4;
    correct[5]  = -1; correct[6]  = -1; correct[7]  = -1; correct[8]  = -1; correct[9]  = -1;
    correct[10] = -1; correct[11] = -1; correct[12] = -1; correct[13] = -1; correct[14] = 14;
    stream_test("cdcddcccccdcdcc", 15, "ababb", 5, correct);
    correct = realloc(correct, 18 * sizeof(int));
    correct[0]  = -1; correct[1]  = -1; correct[2]  = -1; correct[3]  = -1; correct[4]  = -1; correct[5]  = 5;
    correct[6]  = -1; correct[7]  = -1; correct[8]  = -1; correct[9]  = -1; correct[10] = -1; correct[11] = -1;
    correct[12] = 12; correct[13] = -1; correct[14] = -1; correct[15] = -1; correct[16] = -1; correct[17] = 17;
    stream_test("ababaabbababbababb", 18, "ababb", 5, correct);
    correct[0]  = -1; correct[1]  = -1; correct[2]  = -1; correct[3]  = -1; correct[4]  = -1; correct[5]  = -1;
    correct[6]  = -1; correct[7]  = -1; correct[8]  = -1; correct[9]  = -1; correct[10] = -1; correct[11] = -1;
    correct[12] = -1; correct[13] = -1; correct[14] = -1; correct[15] = -1; correct[16] = -1; correct[17] = -1;
    stream_test("ababababababababab", 18, "ababb", 5, correct);

    free(A);

    printf("All tests succeeded!\n");
    return 0;
}
