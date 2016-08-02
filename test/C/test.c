#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#include "test.h"
#include "cref/ref.h"
#include "cref/types.h"
#include "cref/list.h"

void *add_fixed(void *_a, void *_b) {
    fixed *a = (fixed *)_a;
    fixed *b = (fixed *)_b;
    size_t denom = a->den * b->den;
    size_t numer = (a->num * b->den) + (a->den * b->num);
    return S(_fixed(numer, denom));
}

/*
 test that mallinfo can detect allocated memory values
 if this failes then it is basically a moot point to run other tests
*/
START_TEST(test_mallinfo_works) {
    struct mallinfo init = mallinfo();
    void *c = malloc(867);
    struct mallinfo post = mallinfo();
    free(c);
    ASSERT_REF(init.uordblks, post.uordblks-880, "malloc size invalid");
    ASSERT_SUCCESS();
}

/*
 test that the map function works, and that lists can be freed
*/
START_TEST(map_free_works) {
    struct mallinfo init = mallinfo();
    {
        scoped list *namelist = S(_list(
                _string("ted", false),
                _list(
                    _string("meyer", false),
                    EMPTY)));

        scoped list *names2 = map(strcopy, namelist);
        ASSERT_REF(R(namelist), 1, "refcount(namelist) is not 1");
        ASSERT_REF(R(namelist->first), 1, "namelist->first is not 1");
        ASSERT_REF(R(namelist->rest), 1, "namelist->rest is not 1");
        ASSERT_REF(R(names2), 1, "refcount(names2) is not 1");
    }

    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

/*
 test that a list can be freed manually
*/
START_TEST(num_list_can_be_freed) {
    struct mallinfo init = mallinfo();
    list *nums = S(_list(_fixed(1, 1), _list(_fixed(2, 1), EMPTY)));
    L(nums);
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

/*
 test that things can exist in multiple places witout being freed
*/
START_TEST(multi_reference_dont_free) {
    struct mallinfo init = mallinfo();
    fixed *num = S(_fixed(10, 3));
    list *A = S(_list(num, EMPTY));
    list *B = S(_list(num, EMPTY));

    ASSERT_REF(R(num), 3, "reference in scope, and two lists not 3!");
    L(A);
    ASSERT_REF(R(num), 2, "reference in scope, and one lists not 2!");
    L(B);
    ASSERT_REF(R(num), 1, "reference in scope, and no lists not 1!");
    fixed *bk = num;
    L(num);
    ASSERT_REF(R(bk), 0, "should be freed");
    ASSERT_REF(num, NULL, "should be set to null");

    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

/*
 test that a struct's fields are set to null when it is freed
*/
START_TEST(auto_free_sets_fields_null) {
    struct mallinfo init = mallinfo();
    list *A = S(_list(_fixed(10, 3), EMPTY));
    list *F = A;
    L(F);
    
    ASSERT_REF(F, NULL, "L(x) sets to null");
    ASSERT_REF(A->rest, NULL, "rest in auto-freed list should be null");
    ASSERT_REF(A->first, NULL, "first in auto-freed list should be null");
    
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();

}

/*
 test that the fold function works
*/
START_TEST(fold_internal_numbers) {
    struct mallinfo init = mallinfo();
    list *nums = S(_list(_fixed(2, 1), _list(_fixed(3, 1), EMPTY)));
    fixed *t = fold(add_fixed, S(_fixed(0, 1)), nums);

    ASSERT_REF(t->num, 5, "0 + 2 + 3 should be 5");

    list *nbackup = nums;
    L(nums);
    L(t);
    struct mallinfo post = mallinfo();
    ASSERT_REF(R(nbackup), 0, "refcount not zero!");
    ASSERT_REF(nums, NULL, "ref not set to null");
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();

}

START_TEST(range_function_works) {
    struct mallinfo init = mallinfo();
    list *nums = numeric_range(2, 10);
    fixed *t = fold(add_fixed, S(_fixed(0, 1)), nums);

    ASSERT_REF(t->num, 54, "sum(2..10) should be 54");

    L(nums);
    L(t);
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(range_function_works_fastfold) {
    struct mallinfo init = mallinfo();
    list *nums = numeric_range(2, 10);
    fixed *t = fastfold(add_fixed, S(_fixed(0, 1)), nums);

    ASSERT_REF(t->num, 54, "sum(2..10) should be 54");

    L(nums);
    L(t);
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(range_function_works_big) {
    struct mallinfo init = mallinfo();
    list *nums = numeric_range(1, 10000);
    fixed *t = fold(add_fixed, S(_fixed(0, 1)), nums);

    ASSERT_REF(t->num, 10000*10001/2, "sum(1..10000) should be 10000*10001/2");

    L(nums);
    L(t);
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(string_append_bullshit) {
    struct mallinfo init = mallinfo();
    {
        scoped list *strs =
            S(_list(_string("this ", 0), _list(_string("works.", 0), EMPTY)));
        scoped string *app = fastfold(strappend, S(_string("", 0)), strs);

        ASSERT_STR(app->str, "this works.", "string should fold properly");

        ASSERT_REF(R(app), 1, "app is not 1");
        ASSERT_REF(R(strs), 1, "strs is not 1");
    }

    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

START_TEST(auto_cleanup_attribute) {
    struct mallinfo init = mallinfo();
    {
        scoped list *nums = S(_list(_fixed(1, 1), _list(_fixed(2, 1), EMPTY)));
    }
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

int ez_tester(void) {
    scoped string *str = S(_string("this is a string!", false));
    return str->length;
}

START_TEST(lose_scope_return) {
    struct mallinfo init = mallinfo();
    ASSERT_REF(ez_tester(), 17, "strlen not 17");
    struct mallinfo post = mallinfo();
    ASSERT_REF(init.uordblks, post.uordblks, "memory not freed");
    ASSERT_SUCCESS();
}

int main() { }
