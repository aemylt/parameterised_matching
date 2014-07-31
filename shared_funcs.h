/*
    shared_funcs.h
    A collection of shared functions between various files.
*/

#ifndef SHARED_FUNCS
#define SHARED_FUNCS

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

/*
    int compare_char(void* leftp, void* rightp)
    Compares two characters. Used for the Red/Black Tree
    Parameters:
        void* leftp  - First character
        void* rightp - Second character
    Returns int:
        -1 if leftp <  rightp
         0 if leftp == rightp
         1 otherwise
*/

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

#endif