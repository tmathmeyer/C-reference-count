C-Ref
=====
A reference counting library for C structs

Have you ever had two data structures with a pointer to
a shared piece of memory? How the fuck are you supposed
to know when you can free that memory? Often times, you
end up just putting a reference counter into the shared
memory location and dealing with it manually. With Cref
you no longer have to do this manually! To make an auto
refcounted struct, simply follow these three easy steps

####Step 1:
transform your struct definitions like this:

```C
typedef struct linked_list {
    void *first;
    void *rest;
} linked_list;
```

in this code:

```C
refstruct(linked_list, {
    void *first;
    void *rest;
});
```

####Step 2:
when creating an instance of your struct on the stack,
use the function:

```C
ref_malloc(size_t bytes, size_t subrefs)
```

The C-Ref library requires that you put all reference
counted types in your struct before any other types.

When you've allocated the memory, initialize all your
defined fields to whatever you want. However, the Cref
library adds three other fields to each struct. These
fields are function pointers named:

```C
destructor: (self *) -> void;
hashcode: (self *) -> size_t;
equals: (self *, self *) -> bool;
```

where a ``(self *)`` is a pointer to the type of your
struct. 

####Step 3:
The C-Ref library has a few macros to help you with
counting your references.
 - The ``S(void *)`` macro increases a pointer's
   reference count. Use this when you've allocated
   new space on the stack or when you create a
   variable in a loop.
 - The ``L(void *)`` macro decrememnts a pointer's
   reference count, and if it reaches zero, calls
   the L macro on all fields which are reftypes,
   calls the specified destructor it not null, and
   will set the passed in variable to null. note;
   the parameter to L MUST be an L-Value.
 - the ``R(void *)`` macro gets a reftype pointer's
   refcount value.
 - the ``scoped`` attribute can be used in place
   of calling L manually. If a variable is declared
   as scoped, as soon as it leaves scope, the L macro
   is called on it.

##Examples
See tests.c for examples
