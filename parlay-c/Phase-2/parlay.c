// for construction, math and pruning of parlay trees
#include <stdio.h>
#include <stdlib.h>
#include "parlay.h"

static void evaluate_parlay(Parlay *p) {
    p->probability = 1.0;
    p ->odds = 1.0;

    for (int i = 0; i < p->leg_count; i++) {
        p->probability *= p->legs[i]->fair_prob;
        p->odds *= p->legs[i]->odds;
    }

    p->ev = p->probability * p->odds - 1.0;
}

static int compatible(Outcome *a, Outcome *b) {
    // phase 2 rule: no identical market within same event
    if (strcmp(a->event, b->event) == 0 &&
        strcmp(a->market, b->market) == 0) {
        return 0;
    }
    return 1;
}
    
