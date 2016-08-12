/**
 * @file ref.h
 * @author Ted Meyer
 * @date 6 April, 2016
 * @brief File containing macros and structs for creating auto-refcount types
 */

#include <stddef.h>
#include <sys/types.h>
#include <stdbool.h>

#ifndef _REF_H_
#define _REF_H_

typedef 
struct {
    void *inner;
    ssize_t refcount;
    size_t sub_references;
} ptr_refcount;

/* number of fields in generic_type  */
#define SELF_FUNCTIONS 3
typedef
struct generic_type {
    void (*destructor)(struct generic_type *);
    size_t (*hashcode)(struct generic_type *);
    bool (*equals)(struct generic_type *, struct generic_type *);
}
generic_type;

#define S(s) scope(s)
#define L(s) ((s) = lose_scope(s))
#define R(s) get_refcount(s)

#define refstruct(name, fields) typedef struct name { \
    void (*destructor)(struct name *); \
    size_t (*hashcode)(struct name *); \
    bool (*equals)(struct name *, struct name *); \
    struct fields; \
} name
#define scoped __attribute__((cleanup(auto_cleanup_ref)))

typedef void (* self_fn)(void *);
typedef size_t (* self_fn_size_t)(void *);

/**
 * Allocate a refcounted struct.
 * DO NOT USE FOR ARRAYS
 * @param num_bytes the number of bytes in a struct
 * @param subrefs the number of void* elements in this struct which should be refcounted
 */
void *ref_malloc(size_t num_bytes, size_t subrefs);

/**
 * get the reference count for a struct
 * covered by the macro R(pointer)
 * @param reftype the pointer to memory returned from red_malloc
 */
ssize_t get_refcount(void *reftype);

/**
 * decrement the scope counter from a reftype
 * covered by the macro L(pointer)
 * @param reftype a pointer to memory returned from ref_malloc
 */
void *lose_scope(void *reftype);

/**
 * incriment the scope counter from a reftype
 * covered by the macro S(pointer)
 * @param reftype a pointer to memory returned from ref_malloc
 */
void *scope(void *reftype);

void init_mem_tester();
void print_allocated_addresses();
void auto_cleanup_ref(void *ref);
#endif
