#ifndef _REF_H_
#define _REF_H_


typedef 
struct {
    void *inner;
    size_t refcount;
    size_t sub_references;
} ptr_refcount;

#define EMPTY NULL
#define S(s) scope(s)
#define L(s) lose_scope(s)
#define R(s) get_refcount(s)
#define REF_STR destructor destructor;
#define refstruct(name, fields) typedef struct name { REF_STR struct fields; } name

typedef void (* destructor)(void *);

void *ref_malloc(size_t num_bytes, size_t subrefs);
size_t get_refcount(void *reftype);
void *lose_scope(void *reftype);
void *scope(void *reftype);
#endif
