#!/usr/bin/env python
"""Additional valid fixtures and explicitly provisional syntax for unfinished design."""
from pathlib import Path
from textwrap import dedent
ROOT = Path(__file__).resolve().parent.parent

def add(name, src, c, expected):
    src = dedent(src).strip() + '\n'
    src = '\n'.join('\t' * ((len(line) - len(line.lstrip(' '))) // 4) + line.lstrip(' ') for line in src.splitlines()) + '\n'
    (ROOT / 'tests' / 'fixtures' / (name + '.sirius')).write_text(src)
    (ROOT / 'tests' / 'reference_c' / (name + '.c')).write_text('/* Reference lowering, not required byte-for-byte compiler output. */\n' + dedent(c).lstrip())
    (ROOT / 'tests' / 'expected_output' / (name + '.txt')).write_text(expected)

add('14_nested_cow', '''
    def main() -> i64
        var state = {values = [1, 2]}
        let before = state
        state.values[0?]! = 7
        print(state.values[0?] or -1)
        print(before.values[0?] or -1)
        return 0
''', '''
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
''', '7\n1\n')

add('15_array_return', '''
    def change_first(values: Array[i64, 1]) -> Array[i64, 1]
        var copy = values
        copy[0?]! = 42
        return copy

    def main() -> i64
        let a = [10, 20]
        let b = change_first(a)
        print(a[0?] or -1)
        print(b[0?] or -1)
        return 0
''', '''
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
''', '10\n42\n')

# Agreed union syntax: constructors publish structurally distinct records.
add('16_tagged_union', '''
    def Success(value: i64)
        pub value

    def Failure(code: i64)
        pub code

    let Result = Success | Failure

    def unwrap(result: Result) -> i64
        match result
            Success(value)
                return value
            Failure(code)
                return -code

    def main() -> i64
        print(unwrap(Success(7)))
        print(unwrap(Failure(2)))
        return 0
''', '''
    #include "sirius_runtime.h"
    /* Record types are canonical by their fields, not constructor names. */
    typedef struct { int64_t value; } S_Record_value;
    typedef struct { int64_t code; } S_Record_code;
    static S_Record_value Success(int64_t value) {
        return (S_Record_value){.value = value};
    }
    static S_Record_code Failure(int64_t code) {
        return (S_Record_code){.code = code};
    }
    typedef enum { S_Success, S_Failure } S_Result_Tag;
    typedef struct {
        S_Result_Tag tag;
        union { S_Record_value success; S_Record_code failure; } data;
    } S_Result;
    /* Implicit widening from a constructor result to the Result union. */
    static S_Result result_from_success(S_Record_value payload) {
        return (S_Result){.tag=S_Success, .data.success=payload};
    }
    static S_Result result_from_failure(S_Record_code payload) {
        return (S_Result){.tag=S_Failure, .data.failure=payload};
    }
    static int64_t unwrap(S_Result result) {
        switch (result.tag) {
        case S_Success: {
            const S_Record_value payload = result.data.success;
            return payload.value;
        }
        case S_Failure: {
            const S_Record_code payload = result.data.failure;
            return -payload.code;
        }
        }
        s_panic("invalid union tag");
    }
    int main(void) {
        s_print_i64(unwrap(result_from_success(Success(7))));
        s_print_i64(unwrap(result_from_failure(Failure(2))));
        return 0;
    }
''', '7\n-2\n')

add('17_dict', '''
    # Dict constructor, .insert returns a new Dict, .get returns Optional
    def main() -> i64
        var m = Dict[i64, i64]()
        m = m.insert(2, 20)
        let before = m
        m = m.insert(2, 99)
        print(before.get(2) or -1)
        print(m.get(2) or -1)
        print(m.get(8) or -1)
        return 0
''', '''
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
''', '20\n99\n-1\n')

add('18_file_io_PROVISIONAL', '''
    # PROVISIONAL API: write_text/read_text, returning bool/Optional[string]
    def main() -> i64
        let path = "example.txt"
        print(write_text(path, "Hello, Sirius!"))
        let content = read_text(path) or "missing"
        print(content)
        return 0
''', '''
    #include "sirius_runtime.h"
    int main(void) {
        S_String *path = s_string_from_utf8("example.txt");
        S_String *message = s_string_from_utf8("Hello, Sirius!");
        s_print_bool(s_write_text(path, message));
        S_Optional_string maybe = s_read_text(path);
        S_String *fallback = NULL;
        S_String *content;
        if (maybe.has_value) content = maybe.value;
        else { fallback = s_string_from_utf8("missing"); content = fallback; }
        s_print_string(content);
        s_string_release(content); /* owns read result OR fallback, never both */
        s_string_release(message);
        s_string_release(path);
        return 0;
    }
''', 'true\nHello, Sirius!\n')

print('Wrote extended fixtures')

add('19_optional_fallback', '''
    def fallback() -> i64
        print(999)
        return -1

    def main() -> i64
        let a = [7]
        print(a[0?] or fallback())
        print(a[9?] or fallback())
        return 0
''', '''
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
''', '7\n999\n-1\n')
