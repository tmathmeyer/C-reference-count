#ifndef _cref_lists_h_
#define _cref_lists_h_

#include "types.h"

void print_string_list(list *);
void *fold(void *(*)(void *, void *), void *, list *);
void *fastfold(void *(*)(void *, void *), void *, list *);
list *map(void *(*)(void *), list *);
list *numeric_range(size_t, size_t);

#endif
