/* Reference lowering, not required byte-for-byte compiler output. */
#include "sirius_runtime.h"
int main(void) {
    S_Dict_i64_i64 *m = s_dict_i64_i64_new();
    m = s_dict_i64_i64_insert(m, 2, 20); /* consumes m */
    S_Dict_i64_i64 *before = s_dict_i64_i64_retain(m);
    m = s_dict_i64_i64_insert(m, 2, 99); /* copy on write */
    S_Optional_i64 a = s_dict_i64_i64_get(before, 2);
    S_Optional_i64 b = s_dict_i64_i64_get(m, 2);
    S_Optional_i64 c = s_dict_i64_i64_get(m, 8);
    s_print_i64(a.has_value ? a.value : -1);
    s_print_i64(b.has_value ? b.value : -1);
    s_print_i64(c.has_value ? c.value : -1);
    s_dict_i64_i64_release(before);
    s_dict_i64_i64_release(m);
    return 0;
}
