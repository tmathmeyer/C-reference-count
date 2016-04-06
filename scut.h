#ifndef _scut_h_
#define _scut_h_
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#define NO_FORK 1

#define START_TEST(name) \
    char *__scut__##name(); \
    void __scut_fork__##name() { \
        if (NO_FORK) { \
            printf("====" #name "====\n"); \
            char *__scut__call = __scut__##name(); \
            if (__scut__call) { \
                printf("\x1B[31m%s\x1B[0m\n", __scut__call); \
            } else { \
                printf("\x1B[32m   test passed\x1B[0m\n"); \
            } \
            printf("====" #name "====\n\n"); \
            return; \
        } else { \
            if (!fork()) { \
                return; \
            } else { \
                char *__scut__call = __scut__##name(); \
                    if (__scut__call) { \
                        printf("===="#name"====\n\x1B[31m%s\x1B[0m\n===="#name"====\n\n", \
                                __scut__call); \
                    } else { \
                        printf("====" #name "====\n  test passed\n====" #name "===="); \
                    } \
                exit(0); \
            } \
        } \
    } \
    char *__scut__##name()

#define END_TEST ;;

#define ASSERT_REF(a, b, fail) do{if(a!=b){return fail;}}while(0)
#define ASSERT(test, fail) do{if(!test){return fail}}while(0)
#define ASSERT_SUCCESS() do{return NULL;}while(0)

void RUN_TESTS();

#endif
