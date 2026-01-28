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

static void dfs(
    Outcome *outcomes,
    int n,
    int start,
    Parlay *current,
    int min_legs,
    int max_legs)
    {
        if (current->leg_count >= min_legs) {
            evaluate_parlay(current);
            if (current->ev >0) {
                printf("EV=%.4f | Odds=%.2f | Legs=%d\n",
                current->ev, current->odds, current->leg_count);
                for (int i = 0; i < current->leg_count; i++) {
                    printf(" - %s | %s | %s @ %.2f\n",
                    current->legs[i]->event,
                    current->legs[i]->market,
                    current->legs[i]->selection,
                    current->legs[i]->odds);
                }
                printf("\n");
            }
        }
        if (current->leg_count == max_legs) {
            return;
        }
        for (int i = start; i < n; i++) {
            int ok = 1;
            for (int j = 0; j < current->leg_count; j++) {
                if (!compatible(current->legs[j], &outcomes[i])) {
                    ok = 0;
                    break;
                }
            }
            if (!ok) continue;

            current->legs[current->leg_count++] = &outcomes[i];
            dfs(outcomes, n, i + 1, current, min_legs, max_legs);
            current->leg_count--;
        }
    }
void build_parlays(
    Outcome *outcomes,
    int outcome_count,
    int min_legs,
    int max_legs)
    {
        Parlay p;
        p.leg_count = 0;
        dfs(outcomes, outcome_count, 0, &p, min_legs, max_legs);
    }
    
