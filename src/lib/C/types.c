#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ref.h"
#include "types.h"

string *_string(char *str, bool is_static) {
    string *res = ref_malloc(sizeof(string), 0);
    res->str = str;
    res->hashcode = string_hash;
    res->equals = string_equals;
    res->length = strlen(str);
    res->is_static = is_static;
    res->destructor = string_destructor;
    return res;
}

list *_list(void *elem, list *rest) {
    list *result = ref_malloc(sizeof(list), 2);
    result->destructor = NULL;
    result->hashcode = NULL;
    result->equals = NULL;
    result->first = S(elem);
    result->rest = S(rest);
    return result;
}

fixed *_fixed(size_t num, size_t den) {
    fixed *res = ref_malloc(sizeof(fixed), 0);
    res->num = num;
    res->den = den;
    res->destructor = NULL;
    res->hashcode = NULL;
    res->equals = NULL;
    return res;
}

size_t string_hash(string *string_v) {
    string *s = (string *)string_v;
    char *c = s->str;
    size_t n = 13987123;
    while(*c) {
        n = (n + ((ssize_t)c*97)) * 253;
        c++;
    }
    return n;
}

void string_destructor(string *string_v) {
    string *str = (string *)string_v;
    if (str->is_static) {
        free(str->str);
    }
}

void *strcopy(void *v_str) {
    char *nstr = ((string *)v_str)->str;
    char *dup = strdup(nstr);
    return S(_string(dup, true));
}

void *strappend(void *v_strA, void *v_strB) {
    char *A = ((string *)v_strA)->str;
    char *B = ((string *)v_strB)->str;
    char *C = calloc(strlen(A) + strlen(B) + 1, sizeof(char));
    strcat(C, A);
    strcat(C, B);
    return S(_string(C, true));
}

bool string_equals(string *a, string *b) {
    S(a); S(b);
    if (a->length != b->length) {
        L(a); L(b);
        return false;
    }
    bool result = (0==strncmp(a->str, b->str, a->length));
    L(a); L(b);
    return result;
}
