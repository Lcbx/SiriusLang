"""Author golden fixtures. All indented Sirius code is normalized to tabs."""
from pathlib import Path
from textwrap import dedent

ROOT = Path(__file__).resolve().parent.parent

def add(name: str, sirius: str, c: str, output: str):
    source = dedent(sirius).strip() + '\n'
    # Four spaces in the template are one Sirius tab, without changing C source.
    source = '\n'.join(line.replace('    ', '\t', len(line) - len(line.lstrip(' ')) // 4) if False else '\t' * ((len(line) - len(line.lstrip(' '))) // 4) + line.lstrip(' ') for line in source.splitlines()) + '\n'
    (ROOT / 'tests' / 'fixtures' / f'{name}.sirius').write_text(source)
    (ROOT / 'tests' / 'expected_c' / f'{name}.c').write_text('/* Reference lowering, not required byte-for-byte compiler output. */\n' + dedent(c).lstrip())
    (ROOT / 'tests' / 'expected_output' / f'{name}.txt').write_text(output)

add('01_scalars', '''
    def main() -> i64
        let a: i64 = 6
        var b = 2
        b += a * 3
        print(b)
        return 0
''', '''
    #include "sirius_runtime.h"
    int main(void) {
        const int64_t a = 6;
        int64_t b = 2;
        b += a * 3;
        s_print_i64(b);
        return 0;
    }
''', '20\n')

add('02_branches', '''
    def expensive() -> bool
        print(999)
        return true

    def main() -> i64
        let x = 7
        if false and expensive()
            print(111)
        elif x >= 5 and x < 9
            print(222)
        else
            print(333)
        return 0
''', '''
    #include "sirius_runtime.h"
    static bool expensive(void) {
        s_print_i64(999);
        return true;
    }
    int main(void) {
        const int64_t x = 7;
        if (false && expensive()) {
            s_print_i64(111);
        } else if (x >= 5 && x < 9) {
            s_print_i64(222);
        } else {
            s_print_i64(333);
        }
        return 0;
    }
''', '222\n')

add('03_loops', '''
    def main() -> i64
        var total = 0
        for i in 0..8
            if i == 2
                continue
            if i == 6
                break
            total += i
        while total > 10
            total -= 3
        print(total)
        return 0
''', '''
    #include "sirius_runtime.h"
    int main(void) {
        int64_t total = 0;
        for (int64_t i = 0; i < 8; ++i) {
            if (i == 2) continue;
            if (i == 6) break;
            total += i;
        }
        while (total > 10) {
            total -= 3;
        }
        s_print_i64(total);
        return 0;
    }
''', '10\n')

add('04_recursion', '''
    def fib(n: i64) -> i64
        if n < 2
            return n
        return fib(n - 1) + fib(n - 2)

    def factorial(n: i64, acc: i64) -> i64
        if n <= 1
            return acc
        recur factorial(n - 1, acc * n)

    def main() -> i64
        print(fib(7))
        print(factorial(5, 1))
        return 0
''', '''
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
''', '13\n120\n')

add('05_records', '''
    def sum_xy(p)
        return p.x + p.y

    def main() -> i64
        let p = {x = 3, y = 4}
        var q = p
        q.x += 5
        print(sum_xy(p))
        print(sum_xy(q))
        return 0
''', '''
    #include "sirius_runtime.h"
    typedef struct { int64_t x; int64_t y; } S_Record_xy;
    static int64_t sum_xy__xy(S_Record_xy p) { return p.x + p.y; }
    int main(void) {
        const S_Record_xy p = { .x = 3, .y = 4 };
        S_Record_xy q = p; /* value copy for inline records */
        q.x += 5;
        s_print_i64(sum_xy__xy(p));
        s_print_i64(sum_xy__xy(q));
        return 0;
    }
''', '7\n12\n')

add('06_constructor', '''
    def Point(x: f64, y: f64)
        pub x
        pub y
        pub length = sqrt(x*x + y*y)

    def main() -> i64
        let p = Point(3.0, 4.0)
        print(p.length)
        return 0
''', '''
    #include "sirius_runtime.h"
    #include <math.h>
    /* Canonical field order is length, x, y (sorted by name). */
    typedef struct { double length; double x; double y; } S_Record_length_x_y;
    static S_Record_length_x_y Point(double x, double y) {
        return (S_Record_length_x_y){ .x = x, .y = y, .length = sqrt(x*x + y*y) };
    }
    int main(void) {
        const S_Record_length_x_y p = Point(3.0, 4.0);
        s_print_f64(p.length);
        return 0;
    }
''', '5\n')

add('07_record_spread', '''
    def main() -> i64
        let p = {x = 1, y = 2}
        let q = {
            p.*
            del y
            x = 10
            z = 5
        }
        print(q.x + q.z)
        return 0
''', '''
    #include "sirius_runtime.h"
    typedef struct { int64_t x; int64_t y; } S_Record_xy;
    typedef struct { int64_t x; int64_t z; } S_Record_xz;
    int main(void) {
        const S_Record_xy p = { .x = 1, .y = 2 };
        /* Resolve spread/del/overrides statically, in source order. */
        const S_Record_xz q = { .x = 10, .z = 5 };
        (void)p;
        s_print_i64(q.x + q.z);
        return 0;
    }
''', '15\n')

add('08_array_cow', '''
    def main() -> i64
        var a = [10, 20, 30]
        let original = a
        a[1?]! = 99
        print(a[1?] or -1)
        print(original[1?] or -1)
        print(a[9?] or -1)
        return 0
''', '''
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
''', '99\n20\n-1\n')

add('09_strings', '''
    def main() -> i64
        let a = "Sun"
        let b = "rise"
        let c = a + b
        print(c)
        print(a)
        return 0
''', '''
    #include "sirius_runtime.h"
    int main(void) {
        S_String *a = s_string_from_utf8("Sun");
        S_String *b = s_string_from_utf8("rise");
        S_String *c = s_string_concat(a, b);
        s_print_string(c);
        s_print_string(a);
        s_string_release(c);
        s_string_release(b);
        s_string_release(a);
        return 0;
    }
''', 'Sunrise\nSun\n')

add('10_generic', '''
    def identity(x)
        return x

    def main() -> i64
        print(identity(9))
        print(identity(true))
        return 0
''', '''
    #include "sirius_runtime.h"
    /* Two monomorphizations of identity, cached by concrete signature. */
    static int64_t identity__i64(int64_t x) { return x; }
    static bool identity__bool(bool x) { return x; }
    int main(void) {
        s_print_i64(identity__i64(9));
        s_print_bool(identity__bool(true));
        return 0;
    }
''', '9\ntrue\n')

add('11_extension_method', '''
    def move(this, dx: i64, dy: i64)
        return {
            this.*
            x = this.x + dx
            y = this.y + dy
        }

    def main() -> i64
        let p = {x = 2, y = 7}
        let q = p.move(3, 4)
        print(q.x)
        print(q.y)
        return 0
''', '''
    #include "sirius_runtime.h"
    typedef struct { int64_t x; int64_t y; } S_Record_xy;
    /* Extension dispatch is resolved statically. */
    static S_Record_xy move__xy(S_Record_xy this_value, int64_t dx, int64_t dy) {
        return (S_Record_xy) { .x = this_value.x + dx, .y = this_value.y + dy };
    }
    int main(void) {
        const S_Record_xy p = { .x = 2, .y = 7 };
        const S_Record_xy q = move__xy(p, 3, 4);
        s_print_i64(q.x);
        s_print_i64(q.y);
        return 0;
    }
''', '5\n11\n')

add('12_functions_as_values', '''
    def apply(x, operation)
        return operation(x)

    def twice(x: i64) -> i64
        return x * 2

    def main() -> i64
        print(apply(7, twice))
        print(apply(7, fn(x: i64) x + 3))
        return 0
''', '''
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
''', '14\n10\n')

add('13_optional_early_exit', '''
    def positive_at(values: Array[i64, 1], index: i64) -> bool
        let x = values[index?] else return false
        return x > 0

    def main() -> i64
        let a = [-3, 5]
        print(positive_at(a, 1))
        print(positive_at(a, 9))
        return 0
''', '''
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
''', 'true\nfalse\n')

print('Wrote stable Sirius/C fixtures')
