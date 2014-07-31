#include "trees/red_black_tree/red_black_tree/rbtree.c"
#include "shared_funcs.h"
#include "kmp.h"
#include "m_match.h"
#include <stdio.h>
#include <stdlib.h>

void preprocess(char* T, int n, char* sigma, int s_sigma, char* T_p) {
    char a = 0;
    int i;
    rbtree symbols = rbtree_create();
    for (i = 0; i < s_sigma; i++) {
        rbtree_insert(symbols, (void*)sigma[i], (void*)1, compare_char);
        if (a < sigma[i]) a = sigma[i];
    }
    a++;
    for (i = 0; i < n; i++) {
        T_p[i] = (rbtree_lookup(symbols, (void*)T[i], (void*)0, compare_char) == 0) ? a : T[i];
    }
}

int p_match(char* T, int n, char* P, int m, char* sigma, int s_sigma, char* pi, int s_pi, int* output) {
    char* T_p = malloc(n * sizeof(char));
    char* T_pp = malloc(n * sizeof(char));
    char* P_p = malloc(m * sizeof(char));
    char* P_pp = malloc(m * sizeof(char));
    preprocess(T, n, sigma, s_sigma, T_p);
    preprocess(P, m, sigma, s_sigma, P_p);
    preprocess(T, n, pi, s_pi, T_pp);
    preprocess(P, m, pi, s_pi, P_pp);

    int* static_match = malloc((n - m + 1) * sizeof(int));
    int* variable_match = malloc((n - m + 1) * sizeof(int));

    int static_cnt = kmp_match(T_p, n, P_p, m, static_match);
    int variable_cnt = mmatch_match(T_pp, n, P_pp, m, variable_match);

    int i = 0, j = 0;
    int matches = 0;
    while ((i < static_cnt) && (j < variable_cnt)) {
        if (static_match[i] == variable_match[j]) {
            output[matches++] = static_match[i];
            i++;
            j++;
        } else if (static_match[i] > variable_match[j]) i++;
        else j++;
    }
    output = realloc(output, matches * sizeof(int));
    free(T_p);
    free(T_pp);
    free(P_p);
    free(P_pp);
    free(static_match);
    free(variable_match);
    return matches;
}
