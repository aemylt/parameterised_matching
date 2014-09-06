#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "m_match.h"
#include "trees/red_black_tree/red_black_tree/rbtree.c"
#include "shared_funcs.h"
#include "test_funcs.h"

void test_match(char* T, int n, char* P, int m, int correct_matches, int* correct) {
    int* output = malloc((n - m + 1) * sizeof(int));
    int matches = mmatch_match(T, n, P, m, output);
    assert(correct_matches == matches && compare(output, correct, matches));
    free(output);
}

void stream_test(char* T, int n, char* P, int m, int* correct, int num_correct) {
    mmatch_state state = mmatch_build(P, m);
    int j, counter = 0;
    for (j = 0; j < n; j++) {
        if ((counter < num_correct) && (j == correct[counter] + m - 1)) assert(mmatch_stream(state, T[j], j) == correct[counter++] + m - 1);
        else assert(mmatch_stream(state, T[j], j) == -1);
    }
    mmatch_free(state);
}

int main(void) {
    char* p = "aaaaabbbbb";
    int* A = (int*)malloc(10 * sizeof(int));
    int* correct = (int*)malloc(100 * sizeof(int));
    correct[0] = 0; correct[1] = 0; correct[2] = 1; correct[3] = 2; correct[4] = 3;
    correct[5] = 5; correct[6] = 5; correct[7] = 6; correct[8] = 7; correct[9] = 8;
    construct_table(p, 10, A);
    assert(compare(A, correct, 10));
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
    rbtree_destroy(test);

    A = realloc(A, 3 * sizeof(int));
    construct_table("aba", 3, A);
    assert(compare_pi_pj(1, 2, A));
    free(A);

    correct[0] = 0; correct[1] = 10;
    test_match("ababbaaaaababaa", 15, "ababb", 5, 2, correct);
    stream_test("ababbaaaaababaa", 15, "ababb", 5, correct, 2);

    correct[0] = 0; correct[1] = 10;
    test_match("cdcddcccccdcdcc", 15, "ababb", 5, 2, correct);
    stream_test("cdcddcccccdcdcc", 15, "ababb", 5, correct, 2);

    correct[0] = 1; correct[1] = 8; correct[2] = 13;
    test_match("ababaabbababbababb", 18, "ababb", 5, 3, correct);
    stream_test("ababaabbababbababb", 18, "ababb", 5, correct, 3);

    test_match("ababababababababab", 18, "ababb", 5, 0, correct);
    stream_test("ababababababababab", 18, "ababb", 5, correct, 0);

    correct[0] = 0; correct[1] = 100;
    test_match("aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 100, "aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaa", 65, 1, correct);
    stream_test("aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 100, "aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaa", 65, correct, 1);

    test_match("aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 200, "cccccaaaaacccccbbbbbcccccaaaaacccccbbbbbcccccaaaaacccccbbbbbccccc", 65, 2, correct);
    stream_test("aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 200, "cccccaaaaacccccbbbbbcccccaaaaacccccbbbbbcccccaaaaacccccbbbbbccccc", 65, correct, 2);

    test_match("aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaabbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaabbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 200, "aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaabb", 65, 2, correct);
    stream_test("aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaabbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaabbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 200, "aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaabb", 65, correct, 2);

    test_match("aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaabbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaabbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 200, "aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaabbb", 65, 2, correct);
    stream_test("aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaabbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaabbbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 200, "aaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaaaaabbbbbaaaaacccccaabbb", 65, correct, 2);

    test_match("aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaaaaaaaaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaaaaaaaaaaaabbbbbaaaaabbbbb", 200, "aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaaaaaaa", 80, 2, correct);
    stream_test("aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaaaaaaaaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaaaaaaaaaaaabbbbbaaaaabbbbb", 200, "aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaaaaaaa", 80, correct, 2);

    test_match("aaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbb", 200, "aaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbaaaaabbbbb", 80, 2, correct);
    stream_test("aaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbb", 200, "aaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbaaaaabbbbb", 80, correct, 2);

    test_match("aaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaaabbbbbbbbbbbbbbaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaaabbbbbbbbbbbbbb", 200, "aaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaaabbbb", 90, 2, correct);
    stream_test("aaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaaabbbbbbbbbbbbbbaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaaabbbbbbbbbbbbbb", 200, "aaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaabbbbbbbbbbaaaaaaaaaaabbbb", 90, correct, 2);

    int i;
    for (i = 0; i < 25; i++) correct[i] = 5 * i;
    test_match("aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbb", 200, "aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbb", 80, 25, correct);
    stream_test("aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbb", 200, "aaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbbaaaaabbbbb", 80, correct, 25);

    free(correct);

    printf("All tests succeeded!\n");
    return 0;
}
