/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
static S_Array_i64 *change_first(const S_Array_i64 *values) {
    /* Shared immutable input, owned local result. */
    S_Array_i64 *copy = s_array_i64_retain((S_Array_i64 *)values);
    s_array_i64_set(&copy, 0, 42);
    return copy; /* move ownership to caller */
}
int main(void) {
    S_Array_i64 *a = s_array_i64_make(2, (int64_t[]){10, 20});
    S_Array_i64 *b = change_first(a);
    S_Optional_i64 x = s_array_i64_get(a, 0);
    S_Optional_i64 y = s_array_i64_get(b, 0);
    s_print_i64(x.has_value ? x.value : -1);
    s_print_i64(y.has_value ? y.value : -1);
    s_array_i64_release(b);
    s_array_i64_release(a);
    return 0;
}
