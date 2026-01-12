#ifndef MODELS_H
#define MODELS_H
#include <stddef.h>

typedef struct {
    char *book;        // e.g., "BookA"
    char *market_id;   // e.g., "NBA2025-10-31-LAL@BOS-ML"
    char *outcome;     // e.g., "LAL ML" or "U 226.5"
    int american;      // e.g., +150 or -120
} Quote;

typedef struct {
    char *market_id;   // group key
    char **outcomes;   // unique outcomes in this market
    size_t outcomes_n;
} MarketShape;

typedef struct {
    char *market_id;
    char *outcome;
    char *best_book;
    int   best_american;
    double best_decimal;
    double est_true_p;       // fair (no‑vig) probability from cross‑book prices
    double breakeven_dec;    // 1 / est_true_p
    int    breakeven_american; // converted from breakeven_dec
    double single_ev;        // EV of 1‑unit stake at best price
} ConsolidatedOutcome;

// Simple dynamic arrays
typedef struct { Quote *data; size_t n, cap; } QuoteVec;
typedef struct { ConsolidatedOutcome *data; size_t n, cap; } ConOutcomeVec;

void qvec_init(QuoteVec *v); void qvec_push(QuoteVec *v, Quote q); void qvec_free(QuoteVec *v);
void cvec_init(ConOutcomeVec *v); void cvec_push(ConOutcomeVec *v, ConsolidatedOutcome c); void cvec_free(ConOutcomeVec *v);

#endif
