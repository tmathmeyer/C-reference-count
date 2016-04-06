#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ref.h"
#include "types.h"

string *_string(char *str, bool is_static) {
    string *res = ref_malloc(sizeof(string), 0);
    res->str = str;
    res->length = strlen(str);
    res->is_static = is_static;
    res->destructor = string_destructor;
    return res;
}

list *_list(void *elem, list *rest) {
    S(rest);
    S(elem);
    list *result = ref_malloc(sizeof(list), 2);
    result->destructor = NULL;
    result->first = elem;
    result->rest = rest;
    return result;
}

fixed *_fixed(size_t num, size_t den) {
    fixed *res = ref_malloc(sizeof(fixed), 0);
    res->num = num;
    res->den = den;
    res->destructor = NULL;
}

void string_destructor(void *string_v) {
    string *str = (string *)string_v;
    if (str->is_static) {
        free(str->str);
    }
}

void print_string_list(list *list) {
    while(list) {
        S(list);
        char *s = ((string *)(list->first))->str;
        if (s) puts(s);
        L(list);
        list = list->rest;
    }
}

list *map(void *(* fn)(void *), list *ls) {
    if (ls == EMPTY) {
        return EMPTY;
    }
    void *fnres = fn(ls->first);
    list *tail = map(fn, ls->rest);
    list *res = S(_list(fnres, tail));
    L(tail);
    L(fnres);
    return res;
}

void *fold(void *(* fn)(void *, void *), void *init, list *ls) {
    S(init);
    S(ls);
    if (ls == EMPTY) {
        L(ls);
        L(init);
        return init;
    }
    void *foldrec = fold(fn, init, ls->rest);
    void *fnres = fn(ls->first, foldrec);
    L(foldrec);
    L(init);
    L(ls);
    return fnres;
}


void *strcopy(void *v_str) {
    char *nstr = ((string *)v_str)->str;
    return S(_string(strdup(nstr), true));
}

