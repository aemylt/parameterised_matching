#ifndef KMP
#define KMP
#include <stdlib.h>
#include <stdio.h>

void kmp_failure(char* P, int m, int* failure) {
    int i = 0, j;
    failure[0] = 0;
    for (j = 1; j < m - 1; j++) {
        while (i > 0 && P[i] != P[j]) i = failure[i];
        if (P[i] == P[j]) i++;
        failure[j] = i;
    }
}

int kmp_match(char* T, int n, char* P, int m, int* output) {
    int* failure = malloc(m * sizeof(int));
    kmp_failure(P, m, failure);
    int i = 0, j, matches = 0;
    for (j = 0; j < n; j++) {
        while (i > 0 && P[i] != T[j]) {
            i = failure[i - 1];
        }
        if (P[i] == T[j]) i++;
        if (i == m) {
            output[matches++] = j - m + 1;
            i = failure[i - 1];
        }
    }
    output = (int*) realloc(output, matches * sizeof(int));
    return matches;
}

#endif