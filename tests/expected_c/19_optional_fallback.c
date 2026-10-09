/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
static int64_t fallback(void) {
    s_print_i64(999);
    return -1;
}
int main(void) {
    S_Array_i64 *a = s_array_i64_make(1, (int64_t[]){7});
    S_Optional_i64 x = s_array_i64_get(a, 0);
    s_print_i64(x.has_value ? x.value : fallback());
    S_Optional_i64 y = s_array_i64_get(a, 9);
    s_print_i64(y.has_value ? y.value : fallback());
    s_array_i64_release(a);
    return 0;
}
