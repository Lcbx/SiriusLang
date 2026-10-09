/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
typedef struct { S_Array_i64 *values; } S_Record_values;
int main(void) {
    S_Record_values state = {s_array_i64_make(2, (int64_t[]){1, 2})};
    const S_Record_values before = {s_array_i64_retain(state.values)};
    s_array_i64_set(&state.values, 0, 7);
    S_Optional_i64 x = s_array_i64_get(state.values, 0);
    S_Optional_i64 y = s_array_i64_get(before.values, 0);
    s_print_i64(x.has_value ? x.value : -1);
    s_print_i64(y.has_value ? y.value : -1);
    s_array_i64_release(before.values);
    s_array_i64_release(state.values);
    return 0;
}
