// src/odds.c
#include <math.h>
#include <stdio.h>
#include "odds.h"

static double round_nearest(double x){ return floor(x + 0.5); }

double american_to_decimal(int american){
    if (american > 0) return 1.0 + ((double)american)/100.0;
    return 1.0 + 100.0 / (double)(-american);
}

int decimal_to_american(double dec){
    if (dec <= 1.0) return 0; // invalid
    double b = dec - 1.0;
    if (dec >= 2.0) return (int)round_nearest(b * 100.0);
    return (int)round_nearest(-100.0 / b);
}

double implied_prob_from_american(int american){
    double d = american_to_decimal(american);
    return 1.0 / d;
}

double combine_decimal_odds(const double *legs, int n){
    double prod = 1.0; for(int i=0;i<n;i++) prod *= legs[i]; return prod;
}

double expected_value(double true_p, double offered_dec, double stake){
    double payoff = stake * (offered_dec - 1.0);
    return true_p * payoff - (1.0 - true_p) * stake;
}

double kelly_fraction(double true_p, double offered_dec){
    double b = offered_dec - 1.0; if (b <= 0.0) return 0.0;
    double numer = b * true_p - (1.0 - true_p);
    double f = numer / b; return f > 0.0 ? f : 0.0;
}

double parlay_true_probability(const double *leg_true_ps, int n){
    double prod = 1.0; for(int i=0;i<n;i++) prod *= leg_true_ps[i]; return prod;
}

double parlay_ev(const double *leg_true_ps, const double *offered_decimals, int n,
                 double stake, double *out_true_p, double *out_ev){
    double offered_parlay_dec = combine_decimal_odds(offered_decimals, n);
    double tp = parlay_true_probability(leg_true_ps, n);
    double ev = expected_value(tp, offered_parlay_dec, stake);
    if (out_true_p) *out_true_p = tp; if (out_ev) *out_ev = ev; return offered_parlay_dec;
}