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
void construct_table(char* p, int m, int* A) {
    int i;
    rbtree symbols = rbtree_create();
    A[0] = 0;
    rbtree_insert(symbols, (void*)p[0], (void*)0, compare_char);
    for (i = 1; i < m; i++) {
        A[i] = (int)rbtree_lookup(symbols, (void*)p[i], (void*)i, compare_char);
        rbtree_insert(symbols, (void*)p[i], (void*)i, compare_char);
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
    int result;
    int lookup = (int)rbtree_lookup(text, (void*)t_j, (void*)j - i - 1, compare_char);
    if ((A[i] != i) && (lookup == j - i + A[i])) result = 1;
    else if ((A[i] == i) && (lookup < j - i)) result = 1;
    else result = 0;
    return result;
}

/*
    int compare_pi_pj(int i, char* p, int j, int* A)
    Implements the Compare(p_i, p_j) function by Amir et al.
    Parameters:
        int   i - Index i of pattern
        char* p - Pattern
        int   j - Index j of pattern
        int*  A - Predecessor table
    Returns int:
        1 if p_i \cong p_j
        0 otherwise
*/
int compare_pi_pj(int i, char* p, int j, int* A) {
    if ((j - A[j] <= i - 1) && (A[i] == i)) return 1;
    if ((j - A[j] >= i - 1) && (p[j] == p[j - i + A[i]])) return 1;
    return 0;
}

/*
    void mmatch_failure(char* P, int m, int* failure, int* A)
    Constructs the failure table for pattern P.
    Parameters:
        char* P       - Pattern
        int   m       - Length of pattern
        int*  failure - Variable for failure table to be returned in
        int*  A       - Predecessor table
    Returns void:
        Value returned in failure parameter
        failure[i] = longest prefix of P[0:i] that is also a suffix of P[0:i]
    Notes:
        |P| == |A| == |failure| == m
*/
void mmatch_failure(char* P, int m, int* failure, int* A) {
    int i = 0, j;
    failure[0] = 0;
    for (j = 1; j < m - 1; j++) {
        while (i > 0 && !compare_pi_pj(i, P, j, A)) i = failure[i - 1];
        if (compare_pi_pj(i, P, j, A)) i++;
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
    mmatch_failure(P, m, failure, A);
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
    output = (int*) realloc(output, matches * sizeof(int));
    return matches;
}

#endif
