/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
/* Read-only parameter is borrowed; no array ownership transfer. */
static bool positive_at(const S_Array_i64 *values, int64_t index) {
    S_Optional_i64 x = s_array_i64_get(values, index);
    if (!x.has_value) return false;
    return x.value > 0;
}
int main(void) {
    S_Array_i64 *a = s_array_i64_make(2, (int64_t[]){-3, 5});
    s_print_bool(positive_at(a, 1));
    s_print_bool(positive_at(a, 9));
    s_array_i64_release(a);
    return 0;
}
