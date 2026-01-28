#ifndef OUTCOME_H
#define OUTCOME_H

#define MAX_NAME 64
#define MAX_BOOK 32

typedef struct {
    int id;
    char event[MAX_NAME];
    char market[MAX_NAME];
    char selection[MAX_NAME];
    char book[MAX_BOOK];
    double odds;
    double fair_prob;
} Outcome;

#endif