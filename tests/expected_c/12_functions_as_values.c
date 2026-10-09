/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
typedef int64_t (*S_Fn_i64_i64)(int64_t);
static int64_t apply__i64_fn(int64_t x, S_Fn_i64_i64 operation) {
    return operation(x);
}
static int64_t twice(int64_t x) { return x * 2; }
/* Compiler gives the noncapturing lambda an internal name. */
static int64_t lambda__1(int64_t x) { return x + 3; }
int main(void) {
    s_print_i64(apply__i64_fn(7, twice));
    s_print_i64(apply__i64_fn(7, lambda__1));
    return 0;
}
