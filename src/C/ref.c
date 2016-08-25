#include <stdlib.h>
#include <execinfo.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ref.h"
#include "types.h"

struct alloc_tracer {
    char valid;
    void *address;
    char *backtrace_type;
    char *backtrace_origin;
    char *backtrace_origin_opt;
};

static struct alloc_tracer *memtester = NULL;

void init_mem_tester() {
    memtester = calloc(sizeof(struct alloc_tracer), 4096);
}

#define BT_BUF_SIZE 100
void ins_ptr(void *pointer) {
    if (memtester == NULL) {
        return;
    }

    for(int i=0; i<4096; i++) {
        if (memtester[i].valid == 0) {
            memtester[i].valid = 1;
            memtester[i].address = pointer;

            void *buffer[BT_BUF_SIZE];
            int nptrs = backtrace(buffer, BT_BUF_SIZE);
            char **strings = backtrace_symbols(buffer, nptrs);
            memtester[i].backtrace_type = strdup(strings[2]);
            memtester[i].backtrace_origin = strdup(strings[3]);
            if (nptrs >= 4) {
                memtester[i].backtrace_origin_opt = strdup(strings[4]);
            } else {
                memtester[i].backtrace_origin_opt = NULL;
            }
            free(strings);
            return;
        }
    }
}

void rem_ptr(void *ptr) {
    if (memtester == NULL) {
        return;
    }

    for(int i=0; i<4096; i++) {
        if (memtester[i].valid && memtester[i].address==ptr) {
            memtester[i].valid = 0;
            free(memtester[i].backtrace_origin);
            free(memtester[i].backtrace_type);
            if (memtester[i].backtrace_origin_opt) {
                free(memtester[i].backtrace_origin_opt);
            }
        }
    }
}

void print_allocated_addresses() {
    if (memtester == NULL) {
        return;
    }
    for(int i=0; i<4096; i++) {
        if (memtester[i].valid) {
            printf("unfreed reference: [%p]\n", memtester[i].address);
            printf("    type:   %s\n", memtester[i].backtrace_type);
            printf("    origin: %s\n", memtester[i].backtrace_origin);
            if (memtester[i].backtrace_origin_opt) {
                printf("    origin: %s\n", memtester[i].backtrace_origin_opt);
            }
        }
    }
}


void *ref_malloc(size_t num_bytes, size_t subrefs) {
    void *memblock = malloc(num_bytes + sizeof(ptr_refcount));
    ins_ptr(memblock);
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
            lose_scope(location[i+SELF_FUNCTIONS]);
            location[i+SELF_FUNCTIONS] = NULL;
        }
        self_fn destructor = ((self_fn *)reftype)[0];
        if (destructor) {
            destructor(reftype);
        }
        rem_ptr(refd);
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
    if (*lose != 0 && R(*lose) != 0) {
        lose_scope(*lose);
    }
}
