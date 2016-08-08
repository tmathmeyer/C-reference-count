#ifndef _cref_lists_h_
#define _cref_lists_h_

#include "types.h"

#define _for_each(l, e) \
    for(\
            list *__for_each_=(l);\
            ((l)!=EMPTY)?((e)=l->first,1):(0);\
            (l)=(l)->rest)

void print_string_list(list *);
void *fold(void *(*)(void *, void *), void *, list *);
void *fastfold(void *(*)(void *, void *), void *, list *);
list *map(void *(*)(void *), list *);
list *numeric_range(size_t, size_t);

#endif
