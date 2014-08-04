/*
    shared_funcs.h
    A collection of shared functions between various files.
*/

#ifndef SHARED_FUNCS
#define SHARED_FUNCS

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
    if (left < right) return -1;
    else if (left > right) return 1;
    else return 0;
}

#endif