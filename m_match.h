#ifndef M_MATCH
#define M_MATCH

#include "trees/red_black_tree/red_black_tree/rbtree.c"

int compare_char(void* leftp, void* rightp) {
    char left = (char)leftp;
    char right = (char)rightp;
    if (left < right) 
        return -1;
    else if (left > right)
        return 1;
    else {
        return 0;
    }
}

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

int compare_pi_tj(int i, char t_j, rbtree text, int j, int* A) {
    int result;
    int lookup = (int)rbtree_lookup(text, (void*)t_j, (void*)j - i - 1, compare_char);
    if ((A[i] != i) && (lookup == j - i + A[i])) result = 1;
    else if ((A[i] == i) && (lookup < j - i)) result = 1;
    else result = 0;
    return result;
}

int compare_pi_pj(int i, char* p, int j, int* A) {
    if ((j - A[j] <= i - 1) && (A[i] == i)) return 1;
    if ((j - A[j] >= i - 1) && (p[j] == p[j - i + A[i]])) return 1;
    return 0;
}

void mmatch_failure(char* P, int m, int* failure, int* A) {
    int i = 0, j;
    failure[0] = 0;
    for (j = 1; j < m - 1; j++) {
        while (i > 0 && !compare_pi_pj(i, P, j, A)) i = failure[i - 1];
        if (compare_pi_pj(i, P, j, A)) i++;
        failure[j] = i;
    }
}

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
