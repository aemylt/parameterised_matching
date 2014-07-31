#ifndef SHARED_FUNCS
#define SHARED_FUNCS

int compare(int* A, int* B, int m) {
    int i;
    for (i = 0; i < m; i++) {
        if (A[i] != B[i]) return 0;
    }
    return 1;
}

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