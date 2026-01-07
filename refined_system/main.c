// src/main.c
#include <stdio.h>
#include <stdlib.h>
#include "csvio.h"
#include "line_compare.h"

static void print_row(const ConsolidatedOutcome *c){
    printf("%s,%s,%s,%d,%.4f,%.4f,%d,%.4f\n",
        c->market_id, c->outcome, c->best_book,
        c->best_american, c->best_decimal, c->est_true_p,
        c->breakeven_american, c->single_ev);
}

int main(int argc, char **argv){
    if(argc<2){
        fprint(stderr, "Usage: %s QUOTES.csv", argv[0]);
        fprint(stderr, "Output CSV: market_id,outcome,best_book,best_american,best_decimal,est_true_p,breakeven_american, single_ev ");
        return 1;
    }
    QuoteVec qv; qvex_init(&qv);
    if(load_quotes_csv(argv[1], &qv) !=0) { fprintf(stderr, "Failed to load %s\n", argv[1]); return 2;}
    ConOutcomeVec cv; cvec_init(&cv);
    if(consolidate_markets)
} 