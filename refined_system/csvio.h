#ifndef CSVIO_H
#define CSVIO_H
#include <stddef.h>
#include "models.h"

int load_quotes_csv(const char *path, QuoteVec *out_quotes); // returns 0 on success

#endif
