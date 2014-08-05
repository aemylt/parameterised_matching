/*
    p_match.h
    Implementation of the parametrised matching algorithm by Amir, Farach and Muthukrishnan.
    More information is available here: http://www.cs.rutgers.edu/~farach/pubs/ParametrizedMatching.pdf
*/

#ifndef P_MATCH
#define P_MATCH

#include "trees/red_black_tree/red_black_tree/rbtree.c"
#include "shared_funcs.h"
#include "kmp.h"
#include "m_match.h"
#include <stdlib.h>

/*
    char build_tree(char* sigma, int s_sigma, rbtree* symbols)
    Builds a search tree for a given alphabet.
    Parameters:
        char* sigma     - Alphabet
        int s_sigma     - Size of alphabet
        rbtree* symbols - tree to write results to
    Returns char:
        max(sigma) + 1
        Value also written to symbols
*/
char build_tree(char* sigma, int s_sigma, rbtree* symbols) {
    char a = 0;
    *symbols = rbtree_create();
    int i;
    for (i = 0; i < s_sigma; i++) {
        rbtree_insert(*symbols, (void*)sigma[i], (void*)1, compare_char);
        if (a < sigma[i]) a = sigma[i];
    }
    a++;
    return a;
}

/*
    void process_string(char* T, int n, rbtree sigma, int a, rbtree pi, int b, char* T_p, char* T_pp)
    Builds strings for variable and non-variable alphabets.
    Parameters:
        char*  T     - String
        int    n     - Length of string
        rbtree sigma - Non-variable alphabet
        char   a     - Character not in non-variable alphabet
        rbtree pi    - Variable alphabet
        char   b     - Character not in variable alphabet
        char*  T_p   - String of characters in sigma or a
        char*  T_pp  - String of characters in pi or a
    Returns void:
        Values returned in T_p and T_pp
*/
void process_string(char* T, int n, rbtree sigma, char a, rbtree pi, char b, char* T_p, char* T_pp) {
    int i;
    for (i = 0; i < n; i++) {
        T_p[i] = (rbtree_lookup(sigma, (void*)T[i], (void*)0, compare_char)) ? T[i] : a;
        T_pp[i] = (rbtree_lookup(pi, (void*)T[i], (void*)0, compare_char)) ? T[i] : b;
    }
}

/*
    int p_match(char* T, int n, char* P, int m, char* sigma, int s_sigma, char* pi, int s_pi, int* output)
    Finds all p-matches of P in T.
    Parameters:
        char* T     - Text
        int n       - Length of text
        char* P     - Pattern
        int m       - Length of pattern
        char* sigma - Alphabet of nonvariables
        int s_sigma - Size of sigma
        char* pi    - Alphabet of variables
        int s_pi    - Size of pi
        int* output - Variable for p-match indexes to be returned in
    Returns int:
        Number of p-matches found
        P-match indexes returned in output parameter
    Notes:
        |T|      == n
        |P|      == m
        |sigma|  == s_sigma
        |pi|     == s_pi
        |output| == n - m + 1 (maximum number of p-matches)
*/
int p_match(char* T, int n, char* P, int m, char* sigma, int s_sigma, char* pi, int s_pi, int* output) {
    char* T_p = malloc(n * sizeof(char));
    char* T_pp = malloc(n * sizeof(char));
    char* P_p = malloc(m * sizeof(char));
    char* P_pp = malloc(m * sizeof(char));
    rbtree s_tree, p_tree;
    char a = build_tree(sigma, s_sigma, &s_tree);
    char b = build_tree(pi, s_pi, &p_tree);
    process_string(T, n, s_tree, a, p_tree, b, T_p, T_pp);
    process_string(P, m, s_tree, a, p_tree, b, P_p, P_pp);

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

/*
    typedef struct pmatch_state_t *pmatch_state
    Internal state of p-matching algorithm.
    Components:
        kmp_state    kmp    - State of KMP algorithm
        mmatch_state mmatch - State of m-match algorithm
        int          j      - Text index
        rbtree       sigma  - Search tree for non-variable alphabet
        char         a      - Character not in non-variable alphabet
        rbtree       pi     - Search tree for variable alphabet
        char         b      - Character not in variable alphabet
*/
typedef struct pmatch_state_t {
    kmp_state kmp;
    mmatch_state mmatch;
    int j;
    rbtree sigma;
    char a;
    rbtree pi;
    char b;
} *pmatch_state;

/*
    pmatch_state pmatch_build(char* P, int m, char* sigma, int s_sigma, char* pi, int s_pi)
    Builds an initial state for p-matching algorithm.
    Parameters:
        char* P - Pattern
        int m - Length of pattern
        char* sigma - Non-variable alphabet
        int s_sigma - Size of non-variable alphabet
        char* pi - Variable alphabet
        int s_pi - Size of variable alphabet
    Returns pmatch_state:
        Initial state for p-matching algorithm
*/
pmatch_state pmatch_build(char* P, int m, char* sigma, int s_sigma, char* pi, int s_pi) {
    char a = 0, b = 0;
    rbtree s_tree, p_tree;
    pmatch_state state = malloc(sizeof(struct pmatch_state_t));
    state->j = 0;
    a = build_tree(sigma, s_sigma, &s_tree);
    state->sigma = s_tree;
    state->a = a;
    b = build_tree(pi, s_pi, &p_tree);
    state->pi = p_tree;
    state->b = b;
    char* P_p = malloc(m * sizeof(char));
    char* P_pp = malloc(m * sizeof(char));
    process_string(P, m, s_tree, a, p_tree, b, P_p, P_pp);
    state->kmp = kmp_build(P_p, m);
    state->mmatch = mmatch_build(P_pp, m);
    return state;
}

/*
    int pmatch_stream(pmatch_state state, char T_j)
    Checks for a p-match at T_j.
    Parameters:
        pmatch_state state - Current state of the algorithm
        char T_j - Next character in text
    Returns int:
        j if P p-matches T[j - m + 1:j]
        -1 otherwise
*/
int pmatch_stream(pmatch_state state, char T_j) {
    int j = state->j, result = -1;
    int kmp_result = kmp_stream(state->kmp, (rbtree_lookup(state->sigma, (void*)T_j, (void*)0, compare_char)) ? T_j : state->a, j);
    int mmatch_result = mmatch_stream(state->mmatch, (rbtree_lookup(state->pi, (void*)T_j, (void*)0, compare_char)) ? T_j : state->b, j);
    if ((kmp_result == j) && (mmatch_result == j)) result = j;
    state->j++;
    return result;
}

#endif
