#ifndef PARLEY_H
#define PARLEY_H

#include "outcome.h"

#define MAX_LEGS 6

typedef struct {
    int leg_count;
    Outcome *legs[MAX_LEGS];
    double probability;
    double odds;
    double ev;
} Parlay;

void build_parlays(
    Outcome *outcomes,
    int outcome_count,
    int min_legs,
    int max_legs,
);

#endif