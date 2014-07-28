#include "kmp.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int* output = malloc(13 * sizeof(int));
    int matches = kmp_match("ababaabbababbababb", 18, "ababb", 5, output);
    int i;
    for (i = 0; i < matches - 1; i++) {
        printf("%d, ", output[i]);
    }
    printf("%d\n", output[matches - 1]);
    return 0;
}