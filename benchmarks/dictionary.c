/* Build: cc -O2 -std=c11 -I runtime benchmarks/dictionary.c runtime/sirius_runtime.c -o /tmp/sirius-dictionary-bench
   Compare S_Dict with an optimistic contiguous linear dictionary (not the old MIR emitter).
   CSV durations are nanoseconds per operation; results are best interpreted as crossover points. */
#define _POSIX_C_SOURCE 200809L
#include "sirius_runtime.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct { int64_t key, value; } Entry;
static volatile uint64_t sink;
static uint64_t now_ns(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (uint64_t)ts.tv_sec * UINT64_C(1000000000) + (uint64_t)ts.tv_nsec;
}
static uint64_t next(uint64_t *state) {
	*state = *state * UINT64_C(6364136223846793005) + UINT64_C(1442695040888963407);
	return *state >> 32;
}
static int64_t linear_get(const Entry *data, size_t n, int64_t key) {
	for (size_t i = 0; i < n; ++i) if (data[i].key == key) return data[i].value;
	return -1;
}
static void linear_set(Entry *data, size_t n, int64_t key, int64_t value) {
	for (size_t i = 0; i < n; ++i) if (data[i].key == key) { data[i].value = value; return; }
	abort();
}
int main(void) {
	puts("size,operation,linear_ns,hashed_ns,operations");
	for (size_t n = 1; n <= 8192; n *= 2) {
		Entry *data = calloc(n, sizeof(*data));
		if (!data) abort();
		S_Dict *dict = s_dict_new(sizeof(int64_t), sizeof(int64_t), NULL, NULL,
			NULL, NULL, s_hash_i64_key, s_eq_i64_key);
		for (size_t i = 0; i < n; ++i) {
			int64_t key = (int64_t)(i * 7);
			data[i] = (Entry){key, key + 1};
			dict = s_dict_insert(dict, &key, &data[i].value);
		}
		for (int test = 0; test < 3; ++test) {
			const char *name = test == 0 ? "hit" : test == 1 ? "miss" : "overwrite";
			size_t operations = 30000000 / n;
			if (operations > 800000) operations = 800000;
			if (operations < 4000) operations = 4000;
			uint64_t seed = 1234, total = 0, t;
			t = now_ns();
			for (size_t i = 0; i < operations; ++i) {
				int64_t key = (int64_t)((next(&seed) % n) * 7);
				if (test == 1) key += (int64_t)(n * 7);
				if (test == 2) linear_set(data, n, key, (int64_t)i);
				else total += (uint64_t)(linear_get(data, n, key) + 1);
			}
			uint64_t linear_ns = now_ns() - t;
			sink = total;
			seed = 1234; total = 0;
			t = now_ns();
			for (size_t i = 0; i < operations; ++i) {
				int64_t key = (int64_t)((next(&seed) % n) * 7);
				if (test == 1) key += (int64_t)(n * 7);
				if (test == 2) {
					int64_t value = (int64_t)i;
					dict = s_dict_insert(dict, &key, &value);
				} else {
					const int64_t *value = s_dict_get(dict, &key);
					total += (uint64_t)(value ? *value + 1 : 0);
				}
			}
			uint64_t hashed_ns = now_ns() - t;
			sink = total;
			printf("%zu,%s,%.2f,%.2f,%zu\n", n, name, (double)linear_ns/operations, (double)hashed_ns/operations, operations);
		}
		s_dict_release(dict);
		free(data);
	}
	return 0;
}
