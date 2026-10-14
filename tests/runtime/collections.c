#include "sirius_runtime.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct { uint64_t values[24]; } LargeValue;

static void copy_string(void *dst, const void *src) {
	*(S_String **)dst = s_string_retain(*(S_String *const *)src);
}
static void drop_string(void *value) {
	s_string_release(*(S_String **)value);
}

int main(void) {
	S_Array *numbers = s_array_new(sizeof(int64_t), NULL, NULL);
	for (int64_t i = 0; i < 20000; ++i) numbers = s_array_append(numbers, &i);
	assert(s_array_length(numbers) == 20000);
	assert(*(const int64_t *)s_array_at(numbers, 19999) == 19999);

	/* Both operands can be borrowed from the array itself. */
	numbers = s_array_append(numbers, s_array_at(numbers, 0));
	assert(*(const int64_t *)s_array_at(numbers, 20000) == 0);
	S_Array *snapshot = s_array_retain(numbers);
	numbers = s_array_append(numbers, s_array_at(numbers, 19999));
	assert(s_array_length(snapshot) == 20001);
	assert(*(const int64_t *)s_array_at(numbers, 20001) == 19999);
	s_array_set(&numbers, 0, s_array_at(numbers, 19999));
	assert(*(const int64_t *)s_array_at(numbers, 0) == 19999);
	assert(*(const int64_t *)s_array_at(snapshot, 0) == 0);
	s_array_release(snapshot);
	s_array_release(numbers);

	/* Exercise the heap fallback for elements exceeding inline temporary storage. */
	LargeValue big = {{0}};
	for (size_t i = 0; i < 24; ++i) big.values[i] = i + 11;
	S_Array *large = s_array_new(sizeof(big), NULL, NULL);
	large = s_array_append(large, &big);
	large = s_array_append(large, s_array_at(large, 0));
	assert(memcmp(s_array_at(large, 0), s_array_at(large, 1), sizeof(big)) == 0);
	s_array_set(&large, 0, s_array_at(large, 1));
	s_array_release(large);

	/* Aliasing must also work when element copying retains managed values. */
	S_Array *strings = s_array_new(sizeof(S_String *), copy_string, drop_string);
	S_String *hello = s_string_from_utf8("hello");
	strings = s_array_append(strings, &hello);
	s_string_release(hello);
	strings = s_array_append(strings, s_array_at(strings, 0));
	S_Array *old_strings = s_array_retain(strings);
	s_array_set(&strings, 0, s_array_at(strings, 1));
	assert(s_string_length(*(S_String *const *)s_array_at(strings, 0)) == 5);
	assert(s_string_length(*(S_String *const *)s_array_at(old_strings, 0)) == 5);
	s_array_release(old_strings);
	s_array_release(strings);

	S_Dict *values = s_dict_new(sizeof(int64_t), sizeof(int64_t), NULL, NULL,
		NULL, NULL, s_hash_i64_key, s_eq_i64_key);
	for (int64_t i = 0; i < 3000; ++i) values = s_dict_insert(values, &i, &i);
	int64_t key = 100;
	assert(*(const int64_t *)s_dict_get(values, &key) == 100);
	S_Dict *values_snapshot = s_dict_retain(values);
	/* RHS aliases a value stored inside the dictionary being updated. */
	values = s_dict_insert(values, &key, s_dict_get(values, &key));
	int64_t updated = 999;
	values = s_dict_insert(values, &key, &updated);
	assert(*(const int64_t *)s_dict_get(values, &key) == 999);
	assert(*(const int64_t *)s_dict_get(values_snapshot, &key) == 100);
	assert(s_dict_length(values) == 3000);
	s_dict_release(values_snapshot);
	s_dict_release(values);

	S_Dict *managed = s_dict_new(sizeof(S_String *), sizeof(S_String *), copy_string, drop_string,
		copy_string, drop_string, s_hash_string_key, s_eq_string_key);
	S_String *word = s_string_from_utf8("key");
	S_String *payload = s_string_from_utf8("value");
	managed = s_dict_insert(managed, &word, &payload);
	S_Dict *managed_snapshot = s_dict_retain(managed);
	/* Updating with the old value is safe even when it aliases this entry. */
	managed = s_dict_insert(managed, &word, s_dict_get(managed, &word));
	assert(s_string_compare(*(S_String *const *)s_dict_get(managed, &word), payload) == 0);
	assert(s_string_compare(*(S_String *const *)s_dict_get(managed_snapshot, &word), payload) == 0);
	s_dict_release(managed_snapshot);
	s_dict_release(managed);
	s_string_release(word);
	s_string_release(payload);
	puts("PASS runtime collection aliasing, growth, managed values and CoW");
	return 0;
}
