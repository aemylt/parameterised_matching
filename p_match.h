#include "trees/red_black_tree/red_black_tree/rbtree.c"
#include "shared_funcs.h"

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