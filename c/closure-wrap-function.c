#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// This code can now only work on Linux system.
// It has some problem on OS X. Maybe I'll fix it someday.
#if __WORDSIZE != 64 || !defined(__x86_64__)
#error "This program only works on 64bit machine."
#endif

FILE *log_func_call;

typedef int (*func_t)(int arg);

int foo(int a) {
    return a + 1;
}

struct trampoline_code {
    char mov_target[2];
    uint32_t target;
    char mov_env[2];
    uint64_t env; // value stored in r10
    char jmp[3];
} __attribute__((packed));
typedef struct trampoline_code trampoline_code;

void print_trampline_code(const char *func_msg, trampoline_code *tramp) {
    printf("%s\t", func_msg);
    printf("trampoline_code: target = %x, env = %lx\n",
           tramp->target, tramp->env);
}

void *create_closure(void *f) {
    trampoline_code *tramp = (trampoline_code *)f;
    print_trampline_code("create_closure:tramp", tramp);
    // XXX I guess the value stored in r10 is the address of the variable
    // which resides on the top on the stack in all the variables used but
    // out of nested function's scope.
    // So we can guess which part of the stack env need to be copied
    int env_size = (uint8_t *)tramp - (uint8_t *)tramp->env;
    printf("env_size = %d\n", env_size);
    // XXX You may notice that tramp is changed here if create_closure is used
    // incorrectly
    print_trampline_code("create_closure:tramp", tramp);

    // The closure's "environment"
    uint8_t *env = malloc(env_size);
    assert(env);
    memcpy(env, (uint8_t *)tramp->env, env_size);

    // Copy trampoline code
    trampoline_code *new_tramp = malloc(sizeof(trampoline_code));
    assert(new_tramp);
    new_tramp->mov_target[0] = 0x41;
    new_tramp->mov_target[1] = 0xbb;
    // Get the target address from the trampoline code
    new_tramp->target = tramp->target;
    new_tramp->mov_env[0] = 0x49;
    new_tramp->mov_env[1] = 0xba;
    // Set new_trampironment
    new_tramp->env = (uint64_t) env;
    new_tramp->jmp[0] = 0x49;
    new_tramp->jmp[1] = 0xff;
    new_tramp->jmp[2] = 0xe3;

    print_trampline_code("create_closure:new_tramp", new_tramp);
    return new_tramp;
}

void destory_closure(void *f) {
    trampoline_code *tramp = (trampoline_code *)f;
    if (tramp) {
        free((void *)tramp->env);
        free(tramp);
    }
}

func_t create_wrap_function(func_t f) {
    // Nested function declaration should use "auto".
    auto int wrapped(int arg);
    int wrapped(int arg) {
        // call original function
        int val = f(arg);
        printf("Inside wrapped function\n");
        fprintf(log_func_call, "arg: %d ret: %d\n", arg, val);
        return val;
    }
    print_trampline_code("wrapped function", (trampoline_code *)wrapped);
    return (func_t)create_closure(wrapped);
    // XXX If we return the trampoline code and call create_closure outside this
    // function, the stack containing the trampoline code may be changed.
    // So must call create_closure inside this function.
    //return wrapped;
}

int main(int argc, char* argv[]) {
    assert(log_func_call = fopen("log_func_call", "w"));
    // XXX The next line does not work if create_wrap_function just returns the
    // wrapped function's trampoline code
    //func_t bar = create_closure(create_wrap_function(foo));
    func_t bar = create_wrap_function(foo);
    printf("%d\n", bar(2));
    destory_closure(bar);

    return 0;
}
