#ifndef LINE_COMPARE_H
#define LINE_COMPARE_H
#include "models.h"

int consolidate_markets(const QuoteVec *quotes, ConOutcomeVec *out); // 0 on success

void cvec_init(ConOutcomeVec *v);
void cvec_push(ConOutcomeVec *v, ConsolidatedOutcome c);
void cvec_free(ConOutcomeVec *v);

#endif
