#ifndef NO_VIG_H
#define NO_VIG_H
#include <stddef.h>

// Convert Americans→implied probs, then proportionally normalize to sum=1.
void no_vig_from_americans(const int *americans, size_t n, double *out_probs);

// Utility: normalize arbitrary positive array to sum=1.
void normalize_probs(double *p, size_t n);

#endif
