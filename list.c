#include <stdio.h>
#include <stdbool.h>

#include "ref.h"
#include "types.h"
#include "list.h"

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

void *fastfold(void *(* fn)(void *, void *), void *init, list *ls) {
    void *result = init;
    while(ls != EMPTY) {
        void *tmp = fn(result, ls->first);
        L(result);
        result = tmp;
        ls = ls->rest;
    }
    return result;
}

list *numeric_range(size_t low, size_t high) {
    list *init = EMPTY;
    do {
        init = _list(_fixed(high, 1), init);
    } while(high-- != low);
    return S(init);
}
