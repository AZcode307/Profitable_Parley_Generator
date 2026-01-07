// include/csvio.h
// Assumptions: comma‑separated, no quoted commas, UTF‑8.
#ifndef CSVIO_H
#define CSVIO_H
#include <stddef.h>
#include "models.h"

int load_quotes_csv(const char *path, QuoteVec *out_quotes); // returns 0 on success

#endif

// src/csvio.c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "csvio.h"

static char* trim(char *s){
    if(!s) return s; while(isspace((unsigned char)*s)) s++; if(*s==0) return s;
    char *e = s + strlen(s) - 1; while(e>s && isspace((unsigned char)*e)) *e--='\0';
    return s;
}

static char* str_dup(const char* s){ size_t n=strlen(s)+1; char* p=malloc(n); if(p) memcpy(p,s,n); return p; }

void qvec_init(QuoteVec *v){ v->data=NULL; v->n=0; v->cap=0; }
void qvec_push(QuoteVec *v, Quote q){
    if(v->n==v->cap){ size_t nc = v->cap? v->cap*2:64; v->data=realloc(v->data, nc*sizeof(Quote)); v->cap=nc; }
    v->data[v->n++] = q;
}
void qvec_free(QuoteVec *v){
    for(size_t i=0;i<v->n;i++){ free(v->data[i].book); free(v->data[i].market_id); free(v->data[i].outcome); }
    free(v->data); v->data=NULL; v->n=v->cap=0;
}

int load_quotes_csv(const char *path, QuoteVec *out_quotes){
    FILE *f = fopen(path, "r"); if(!f) return -1; char line[4096];
    // header
    if(!fgets(line, sizeof line, f)){ fclose(f); return -2; }
    while(fgets(line, sizeof line, f)){
        char *p=line; char *book=strtok(p, ",\n"); char *market=strtok(NULL, ",\n");
        char *outcome=strtok(NULL, ",\n"); char *amer=strtok(NULL, ",\n");
        if(!(book&&market&&outcome&&amer)) continue;
        Quote q={0};
        q.book=str_dup(trim(book)); q.market_id=str_dup(trim(market)); q.outcome=str_dup(trim(outcome));
        q.american = atoi(trim(amer));
        qvec_push(out_quotes, q);
    }
    fclose(f); return 0;
}