// src/line_compare.c
#define _POSIX_C_SOURCE 200809L
#include "line_compare.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <strings.h>
#include "no_vig.h"
#include "odds.h"

static int cmp_market_then_outcome(const void *a, const void *b){
    const Quote *x=a,*y=b; int c=strcmp(x->market_id,y->market_id); if(c) return c; return strcmp(x->outcome,y->outcome);
}

static int cmp_int(const void *a, const void *b){ int x = *(const int*)a, y = *(const int*)b; return (x>y) - (x<y); }

void cvec_init(ConOutcomeVec *v){ v->data=NULL; v->n=0; v->cap=0; }
void cvec_push(ConOutcomeVec *v, ConsolidatedOutcome c){
    if(v->n==v->cap){ size_t nc=v->cap? v->cap*2:64; v->data=realloc(v->data, nc*sizeof(ConsolidatedOutcome)); v->cap=nc; }
    v->data[v->n++]=c;
}
void cvec_free(ConOutcomeVec *v){
    for(size_t i=0;i<v->n;i++){ free(v->data[i].market_id); free(v->data[i].outcome); free(v->data[i].best_book); }
    free(v->data); v->data=NULL; v->n=v->cap=0;
}

int consolidate_markets(const QuoteVec *quotes, ConOutcomeVec *out){
    if(quotes->n==0) return 0;
    // copy & sort quotes by (market_id, outcome)
    Quote *tmp = malloc(quotes->n * sizeof(Quote)); if(!tmp) return -1;
    memcpy(tmp, quotes->data, quotes->n*sizeof(Quote));
    qsort(tmp, quotes->n, sizeof(Quote), cmp_market_then_outcome);

    size_t i=0; while(i<quotes->n){
        size_t j=i; // advance j to end of this market_id
        const char *mid = tmp[i].market_id;
        while(j<quotes->n && strcmp(tmp[j].market_id, mid)==0) j++;
        // within [i,j) we have one market with multiple outcomes & books
        // collect unique outcomes
        size_t k=i; size_t u=0; // count outcomes
        while(k<j){ const char *oc = tmp[k].outcome; u++; while(k<j && strcmp(tmp[k].outcome, oc)==0) k++; }
        // for each outcome, compute best price & representative (median american)
        int *rep_amer = malloc(u*sizeof(int));
        double *best_dec = malloc(u*sizeof(double));
        int *best_amer = malloc(u*sizeof(int));
        char **best_book = malloc(u*sizeof(char*));
        char **outcomes = malloc(u * sizeof(char*));
        if(!rep_amer || !best_dec || !best_amer || !best_book || !outcomes){ free(rep_amer); free(best_dec); free(best_amer); free(best_book); free(outcomes); free(tmp); return -1; }

        /* For each distinct outcome in this market: pick best book (highest decimal),
           and a representative american (median) for no-vig estimation. */
        size_t idx = 0;
        size_t p = i;
        while(p < j){
            const char *oc = tmp[p].outcome;
            size_t q = p; size_t cnt = 0; while(q < j && strcmp(tmp[q].outcome, oc) == 0){ cnt++; q++; }
            int *buffer = malloc(cnt * sizeof(int)); if(!buffer){ free(rep_amer); free(best_dec); free(best_amer); for(size_t z=0;z<idx;z++){ free(best_book[z]); free(outcomes[z]); } free(best_book); free(outcomes); free(tmp); return -1; }

            double bestd = -INFINITY; int besta = 0; char *bestb = NULL;
            size_t t = 0;
            for(size_t r = p; r < q; r++){
                double dec = american_to_decimal(tmp[r].american);
                if(dec > bestd){ bestd = dec; besta = tmp[r].american; free(bestb); bestb = strdup(tmp[r].book); }
                buffer[t++] = tmp[r].american;
            }
            qsort(buffer, cnt, sizeof(int), cmp_int);
            rep_amer[idx] = buffer[cnt/2];
            best_dec[idx] = bestd;
            best_amer[idx] = besta;
            best_book[idx] = bestb ? bestb : strdup("");
            outcomes[idx] = strdup(oc);
            free(buffer);
            idx++;
            p = q;
        }

        double *true_ps = malloc(u * sizeof(double)); if(!true_ps){ for(size_t z=0;z<u;z++){ free(best_book[z]); free(outcomes[z]); } free(best_book); free(outcomes); free(rep_amer); free(best_dec); free(best_amer); free(tmp); return -1; }
        no_vig_from_americans(rep_amer, u, true_ps);

        for(size_t z=0; z<u; z++){
            ConsolidatedOutcome c;
            c.market_id = strdup(mid);
            c.outcome = outcomes[z];
            c.best_book = best_book[z];
            c.best_american = best_amer[z];
            c.best_decimal = best_dec[z];
            c.est_true_p = true_ps[z];
            c.breakeven_dec = (c.est_true_p > 0.0) ? 1.0 / c.est_true_p : 0.0;
            c.breakeven_american = decimal_to_american(c.breakeven_dec);
            c.single_ev = expected_value(c.est_true_p, c.best_decimal, 1.0);
            cvec_push(out, c);
        }

        free(outcomes); free(rep_amer); free(best_dec); free(best_amer); free(true_ps);
        free(best_book);

        i = j; // move to next market
    }
    free(tmp);
    return 0;
}