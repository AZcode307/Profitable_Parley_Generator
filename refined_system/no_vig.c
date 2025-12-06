// include/no_vig.h
#ifndef NO_VIG_H
#define NO_VIG_H
#include <stddef.h>

// Convert Americans→implied probs, then proportionally normalize to sum=1.
void no_vig_from_americans(const int *americans, size_t n, double *out_probs);

// Utility: normalize arbitrary positive array to sum=1.
void normalize_probs(double *p, size_t n);

#endif

// src/no_vig.c
#include "no_vig.h"
#include "odds.h"

void normalize_probs(double *p, size_t n){
    double s=0.0; for(size_t i=0;i<n;i++) s+=p[i];
    if (s<=0.0) return; for(size_t i=0;i<n;i++) p[i]/=s;
}

void no_vig_from_americans(const int *americans, size_t n, double *out_probs){
    for(size_t i=0;i<n;i++) out_probs[i] = implied_prob_from_american(americans[i]);
    normalize_probs(out_probs, n);
}