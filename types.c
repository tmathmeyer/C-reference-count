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
    list *result = ref_malloc(sizeof(list), 2);
    result->destructor = NULL;
    result->first = S(elem);
    result->rest = S(rest);
    return result;
}

fixed *_fixed(size_t num, size_t den) {
    fixed *res = ref_malloc(sizeof(fixed), 0);
    res->num = num;
    res->den = den;
    res->destructor = NULL;
    return res;
}

void string_destructor(void *string_v) {
    string *str = (string *)string_v;
    if (str->is_static) {
        free(str->str);
    }
}

void *strcopy(void *v_str) {
    char *nstr = ((string *)v_str)->str;
    return S(_string(strdup(nstr), true));
}

void *strappend(void *v_strA, void *v_strB) {
    char *A = ((string *)v_strA)->str;
    char *B = ((string *)v_strB)->str;
    char *C = calloc(strlen(A) + strlen(B) + 1, sizeof(char));
    strcat(C, A);
    strcat(C, B);
    return S(_string(C, true));
}
