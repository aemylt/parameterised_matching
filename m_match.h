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