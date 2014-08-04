/*
    test_funcs.h
    A collection of shared functions for testing.
*/

#ifndef TEST_FUNCS
#define TEST_FUNCS

/*
    int compare(int* A, int* B, int m)
    Compares two lists of integers. Used for testing only.
    Parameters:
        int* A - First list of integers
        int* B - Second list of integers
        int  m - Length of lists A and B
    Returns int:
        1 if for 0 <= i < m A[i] == B[i]
        0 otherwise
    Notes:
        |A| == |B| == m
*/
int compare(int* A, int* B, int m) {
    int i;
    for (i = 0; i < m; i++) {
        if (A[i] != B[i]) return 0;
    }
    return 1;
}

#endif