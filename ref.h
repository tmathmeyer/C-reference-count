/**
 * @file ref.h
 * @author Ted Meyer
 * @date 6 April, 2016
 * @brief File containing macros and structs for creating auto-refcount types
 */

#ifndef _REF_H_
#define _REF_H_

typedef 
struct {
    void *inner;
    size_t refcount;
    size_t sub_references;
} ptr_refcount;

#define S(s) scope(s)
#define L(s) lose_scope(s)
#define R(s) get_refcount(s)
#define REF_STR destructor destructor;
#define refstruct(name, fields) typedef struct name { REF_STR struct fields; } name

typedef void (* destructor)(void *);

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
size_t get_refcount(void *reftype);

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
#endif
