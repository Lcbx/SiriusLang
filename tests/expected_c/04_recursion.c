/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
static int64_t fib(int64_t n) {
    if (n < 2) return n;
    return fib(n - 1) + fib(n - 2);
}
static int64_t factorial(int64_t n, int64_t acc) {
    for (;;) {
        if (n <= 1) return acc;
        /* recur simultaneously replaces all argument bindings */
        int64_t next_n = n - 1;
        int64_t next_acc = acc * n;
        n = next_n;
        acc = next_acc;
    }
}
int main(void) {
    s_print_i64(fib(7));
    s_print_i64(factorial(5, 1));
    return 0;
}
