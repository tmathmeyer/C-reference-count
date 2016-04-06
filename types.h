#ifndef _TYPES_H_
#define _TYPES_H_

#include "ref.h"

// simple linked list, list and rest are both refstructs
refstruct(list, {
    struct list *rest;
    void *first;
});


// a string type
refstruct(string, {
    char *str;
    size_t length;
    bool is_static;
});


// a fixed point numeric type
refstruct(fixed, {
    size_t num;
    size_t den;
});






void string_destructor(void *string_v);
string *_string(char *str, bool is_static);
list *_list(void *elem, list *rest);
fixed *_fixed(size_t, size_t);


void print_string_list(list *list);
list *map(void *(* fn)(void *), list *ls);
void *fold(void *(* fn)(void *, void *), void *, list *);
void *strcopy(void *v_str);
#endif
