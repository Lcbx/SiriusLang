/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
int main(void) {
    S_Array_i64 *a = s_array_i64_make(3, (int64_t[]){10, 20, 30});
    S_Array_i64 *original = s_array_i64_retain(a);
    s_array_i64_set(&a, 1, 99); /* CoW: original remains unchanged */
    S_Optional_i64 x = s_array_i64_get(a, 1);
    S_Optional_i64 y = s_array_i64_get(original, 1);
    S_Optional_i64 z = s_array_i64_get(a, 9);
    s_print_i64(x.has_value ? x.value : -1);
    s_print_i64(y.has_value ? y.value : -1);
    s_print_i64(z.has_value ? z.value : -1);
    s_array_i64_release(original);
    s_array_i64_release(a);
    return 0;
}
