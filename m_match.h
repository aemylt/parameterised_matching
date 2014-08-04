/*
    m_match.h
    Implementation of the mapped matching algorithm by Amir, Farach and Muthukrishnan.
    More information is available here: http://www.cs.rutgers.edu/~farach/pubs/ParametrizedMatching.pdf
    This variant of the KMP algorithm is from lecture slides courtesy of Dr Raphael Clifford: (private link) https://www.cs.bris.ac.uk/Teaching/Resources/COMS21103/material-dima/string_matching.pdf
*/

#ifndef M_MATCH
#define M_MATCH

#include "trees/red_black_tree/red_black_tree/rbtree.c"
#include "shared_funcs.h"

/*
    void construct_table(char* p, int m, int* A)
    Constructs the predecessor table.
    Parameters:
        char* p - Pattern
        int   m - Length of pattern
        int*  A - Variable for predecessor table to be returned in
    Returns void:
        Value returned in A parameter
        A[i] = j where 0 <= j <= i, A[i] == A[j] and \forall k such that j < k < i, A[k] != A[i]
    Notes:
        |p| == |A| == m
*/
void construct_table(char* P, int m, int* A) {
    int i;
    rbtree symbols = rbtree_create();
    A[0] = 0;
    rbtree_insert(symbols, (void*)P[0], (void*)0, compare_char);
    for (i = 1; i < m; i++) {
        A[i] = (int)rbtree_lookup(symbols, (void*)P[i], (void*)i, compare_char);
        rbtree_insert(symbols, (void*)P[i], (void*)i, compare_char);
    }
}

/*
    int compare_pi_tj(int i, char t_j, rbtree text, int j, int* A)
    Implements the Compare(p_i, t_j) function by Amir et al.
    Parameters:
        int    i    - Index of pattern
        char   t_j  - Current character in text
        rbtree text - Red/Black tree of when each character in the alphabet last occured in the text
        int    j    - Index of text
        int*   A    - Predecessor table
    Returns int:
        1 if p_i \cong t_j
        0 otherwise
*/
int compare_pi_tj(int i, char t_j, rbtree text, int j, int* A) {
    int lookup = (int)rbtree_lookup(text, (void*)t_j, (void*)j, compare_char);
    return ((i - A[i] == j - lookup) || ((A[i] == i) && (j - lookup > i)));
}

/*
    int compare_pi_pj(int i, char* p, int j, int* A)
    Implements the Compare(p_i, p_j) function by Amir et al.
    Parameters:
        int   i - Index i of pattern
        int   j - Index j of pattern
        int*  A - Predecessor table
    Returns int:
        1 if p_i \cong p_j
        0 otherwise
*/
int compare_pi_pj(int i, int j, int* A) {
    return ((i - A[i] == j - A[j]) || ((A[i] == i) && (j - A[j] > i)));
}

/*
    void mmatch_failure(char* P, int m, int* failure, int* A)
    Constructs the failure table for pattern P.
    Parameters:
        int   m       - Length of pattern
        int*  failure - Variable for failure table to be returned in
        int*  A       - Predecessor table
    Returns void:
        Value returned in failure parameter
        failure[i] = longest prefix of P[0:i] that is also a suffix of P[0:i]
    Notes:
        |A| == |failure| == m
*/
void mmatch_failure(int m, int* failure, int* A) {
    int i = 0, j;
    failure[0] = 0;
    for (j = 1; j < m - 1; j++) {
        while (i > 0 && !compare_pi_pj(i, j, A)) i = failure[i - 1];
        if (compare_pi_pj(i, j, A)) i++;
        failure[j] = i;
    }
}

/*
    int mmatch_match(char* T, int n, char* P, int m, int* output)
    Finds all m-matches of P in T via the algorithm by Amir et al.
    Parameters:
        char* T      - Text to search
        int   n      - Length of text
        char* P      - Pattern to search for
        int   m      - Length of pattern
        int*  output - Variable for m-match indexes to be returned in
    Returns int:
        Number of m-matches found
        M-match indexes returned in output parameter
    Notes:
        |T| == n
        |P| == m
        |output| == n - m + 1 (maximum number of m-matches)
*/
int mmatch_match(char* T, int n, char* P, int m, int* output) {
    int* A = malloc(m * sizeof(int));
    construct_table(P, m, A);
    int* failure = malloc(m * sizeof(int));
    mmatch_failure(m, failure, A);
    rbtree text = rbtree_create();
    int i = 0, j, matches = 0;
    for (j = 0; j < n; j++) {
        while (i > 0 && !compare_pi_tj(i, T[j], text, j, A)) i = failure[i - 1];
        if (compare_pi_tj(i, T[j], text, j, A)) i++;
        if (i == m) {
            output[matches++] = j - m + 1;
            i = failure[i - 1];
        }
        rbtree_insert(text, (void*)T[j], (void*)j, compare_char);
    }
    output = realloc(output, matches * sizeof(int));
    free(A);
    free(failure);
    return matches;
}

/*
    typedef struct mmatch_state_t *mmatch_state
    Structure to hold current state of algorithm.
    Components:
        int*   A       - Predecessor table for pattern
        int    m       - Length of pattern
        int    i       - Current index of pattern
        int*   failure - Failure table for pattern
        rbtree text    - Predecessor tree for text
*/
typedef struct mmatch_state_t {
    int* A;
    int m;
    int i;
    int* failure;
    rbtree text;
} *mmatch_state;

/*
    mmatch_state mmatch_build(char* P, int m)
    Creates an initial state for m-match algorithm.
    Parameters:
        char* P - Pattern
        int   m - Length of pattern
    Returns mmatch_state:
        Initial state for algorithm
*/
mmatch_state mmatch_build(char* P, int m) {
    mmatch_state state = malloc(sizeof(struct mmatch_state_t));
    state->A = malloc(m * sizeof(int));
    construct_table(P, m, state->A);
    state->failure = malloc(m * sizeof(int));
    mmatch_failure(m, state->failure, state->A);
    state->text = rbtree_create();
    state->m = m;
    state->i = 0;
    return state;
}

/*
    int mmatch_stream(mmatch_state state, char T_j)
    Returns whether an m-match occurs for character T_j.
    Parameters:
        mmatch_state state - The current state of the algorithm
        char         T_j   - The next character in the text
        int          j     - The current index of the text
    Returns int:
        j  if P m-matches T[j - m + 1:j]
        -1 otherwise
*/
int mmatch_stream(mmatch_state state, char T_j, int j) {
    int* A = state->A;
    int* failure = state->failure;
    int i = state->i, result = -1;
    rbtree text = state->text;
    while (i > 0 && !compare_pi_tj(i, T_j, text, j, A)) i = failure[i - 1];
    if (compare_pi_tj(i, T_j, text, j, A)) i++;
    if (i == state->m) {
        result = j;
        i = failure[i - 1];
    }
    rbtree_insert(text, (void*)T_j, (void*)j, compare_char);
    state->i = i;
    return result;
}

#endif
