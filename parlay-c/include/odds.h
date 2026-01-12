#ifndef ODDS_H
#define ODDS_H

// Conversions and utilities for American/decimal odds and parlay math

double american_to_decimal(int american);
int decimal_to_american(double dec);
double implied_prob_from_american(int american);
double combine_decimal_odds(const double *legs, int n);
double expected_value(double true_p, double offered_dec, double stake);
double kelly_fraction(double true_p, double offered_dec);
double parlay_true_probability(const double *leg_true_ps, int n);

double parlay_ev(const double *leg_true_ps, const double *offered_decimals, int n,
                 double stake, double *out_true_p, double *out_ev);

#endif
