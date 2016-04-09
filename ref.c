#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ref.h"
#include "types.h"

void *ref_malloc(size_t num_bytes, size_t subrefs) {
    void *memblock = malloc(num_bytes + sizeof(ptr_refcount));
    ptr_refcount *result = memblock;
    result->refcount = 0;
    result->inner = (void *)(((ptr_refcount *)memblock)+1);
    result->sub_references = subrefs;
    return (void *)result->inner;
}

void *lose_scope(void *reftype) {
    if (reftype == NULL) {
        return NULL;
    }
    ptr_refcount *memblock = (ptr_refcount *)(reftype);
    ptr_refcount *refd = memblock-1;
    refd->refcount--;
    if (refd->refcount <= 0) {
        for(size_t i=0; i<refd->sub_references; i++) {
            void **location = (void **)reftype;
            lose_scope(location[i+1]);
            location[i+1] = NULL;
        }
        destructor d = ((destructor *)reftype)[0];
        if (d) {
            d(reftype);
        }
        free(refd);
        return NULL;
    }
    return reftype;
}

void *scope(void *reftype) {
    if (reftype == NULL) {
        return NULL;
    }
    ptr_refcount *memblock = (ptr_refcount *)(reftype);
    ptr_refcount *refd = memblock-1;
    refd->refcount++;
    return reftype;
}

ssize_t get_refcount(void *reftype) {
    if (reftype == NULL) {
        return ~0x0;
    }
    ptr_refcount *memblock = (ptr_refcount *)(reftype);
    return memblock[-1].refcount;
}

void auto_cleanup_ref(void *ref) {
    void **lose = (void **)ref;
    lose_scope(*lose);
}
