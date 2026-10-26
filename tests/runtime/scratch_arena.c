/* Experimental, scope-local scratch storage. NOT part of the Sirius runtime.
   Only POD buffers whose lifetime ends at reset are permitted. In particular,
   do not store ARC-managed values or return pointers from an arena scope. */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHECK(condition) do { if (!(condition)) { \
	fprintf(stderr, "scratch test failed: %s (%d)\n", #condition, __LINE__); \
	exit(1); } } while (0)

typedef struct ScratchBlock {
	struct ScratchBlock *next;
	size_t capacity, used;
	max_align_t align;
	unsigned char data[];
} ScratchBlock;

typedef struct {
	ScratchBlock *head, *tail, *current;
	size_t system_allocations, capacity_bytes;
} Scratch;

_Static_assert(offsetof(ScratchBlock, data) % _Alignof(max_align_t) == 0,
	"scratch payload must be maximally aligned");

static void *scratch_alloc(Scratch *arena, size_t size) {
	const size_t alignment = _Alignof(max_align_t);
	if (size > SIZE_MAX - (alignment - 1)) return NULL;
	size_t need = ((size ? size : 1) + alignment - 1) / alignment * alignment;
	ScratchBlock *block = arena->current ? arena->current : arena->head;
	while (block && block->capacity - block->used < need) block = block->next;
	if (!block) {
		size_t capacity = arena->tail ? arena->tail->capacity : 4096;
		if (capacity < need) {
			while (capacity < need && capacity <= SIZE_MAX / 2) capacity *= 2;
			if (capacity < need) capacity = need;
		}
		if (capacity > SIZE_MAX - offsetof(ScratchBlock, data)) return NULL;
		if (capacity > SIZE_MAX - arena->capacity_bytes) return NULL;
		block = malloc(offsetof(ScratchBlock, data) + capacity);
		if (!block) return NULL;
		block->next = NULL;
		block->capacity = capacity;
		block->used = 0;
		if (arena->tail) arena->tail->next = block;
		else arena->head = block;
		arena->tail = block;
		arena->system_allocations++;
		arena->capacity_bytes += capacity;
	}
	arena->current = block;
	void *result = block->data + block->used;
	block->used += need;
	return result;
}

static void scratch_reset(Scratch *arena) {
	for (ScratchBlock *block = arena->head; block; block = block->next)
		block->used = 0;
	arena->current = arena->head;
}

static void scratch_dispose(Scratch *arena) {
	ScratchBlock *block = arena->head;
	while (block) {
		ScratchBlock *next = block->next;
		free(block);
		block = next;
	}
	memset(arena, 0, sizeof(*arena));
}

static void test_scratch(void) {
	Scratch arena = {0};
	void *first = scratch_alloc(&arena, 1);
	CHECK(first && (uintptr_t)first % _Alignof(max_align_t) == 0);
	CHECK(scratch_alloc(&arena, 0) != NULL);
	CHECK(scratch_alloc(&arena, SIZE_MAX) == NULL);
	CHECK(scratch_alloc(&arena, SIZE_MAX - 7) == NULL);
	for (size_t i = 1; i <= 400; ++i) {
		unsigned char *p = scratch_alloc(&arena, i);
		CHECK(p && (uintptr_t)p % _Alignof(max_align_t) == 0);
		memset(p, (int)(i & 255), i);
		CHECK(p[0] == (i & 255) && p[i - 1] == (i & 255));
	}
	CHECK(arena.system_allocations > 1);
	size_t allocations = arena.system_allocations;
	size_t capacity = arena.capacity_bytes;
	scratch_reset(&arena);
	CHECK(scratch_alloc(&arena, 1) == first);
	CHECK(scratch_alloc(&arena, 0) != NULL);
	for (size_t i = 1; i <= 400; ++i) {
		unsigned char *p = scratch_alloc(&arena, i);
		CHECK(p && (uintptr_t)p % _Alignof(max_align_t) == 0);
		memset(p, (int)((i + 1) & 255), i);
		CHECK(p[i - 1] == ((i + 1) & 255));
	}
	CHECK(arena.system_allocations == allocations);
	CHECK(arena.capacity_bytes == capacity);
	scratch_dispose(&arena);
	CHECK(arena.head == NULL && arena.capacity_bytes == 0);
}

/* A pass-local POD workload: a set of temporary nodes used together, then
   discarded at a phase boundary. Same writes/reads for both allocation paths. */
static uint64_t workload(int use_arena, size_t phases, size_t count,
		size_t bytes, size_t *system_allocations, size_t *capacity_bytes) {
	Scratch arena = {0};
	void **nodes = malloc(count * sizeof(*nodes));
	CHECK(nodes);
	uint64_t digest = 0;
	for (size_t phase = 0; phase < phases; ++phase) {
		for (size_t i = 0; i < count; ++i) {
			nodes[i] = use_arena ? scratch_alloc(&arena, bytes) : malloc(bytes);
			CHECK(nodes[i]);
			uint64_t value = (uint64_t)phase * 1315423911u + i * 2654435761u;
			memset(nodes[i], (int)(value & 255), bytes);
			memcpy(nodes[i], &value, sizeof(value));
		}
		for (size_t i = 0; i < count; ++i) {
			uint64_t value;
			memcpy(&value, nodes[i], sizeof(value));
			unsigned char last = ((unsigned char *)nodes[i])[bytes - 1];
			digest = (digest << 5 | digest >> 59) ^ value ^ last;
		}
		if (use_arena) scratch_reset(&arena);
		else for (size_t i = 0; i < count; ++i) free(nodes[i]);
	}
	*system_allocations = use_arena ? arena.system_allocations : phases * count;
	*capacity_bytes = arena.capacity_bytes;
	scratch_dispose(&arena);
	free(nodes);
	return digest;
}

static void run_workload(const char *name, size_t phases, size_t count, size_t bytes,
		int benchmark) {
	size_t heap_allocations, arena_allocations, capacity;
	clock_t start = clock();
	uint64_t heap = workload(0, phases, count, bytes, &heap_allocations, &capacity);
	clock_t split = clock();
	uint64_t scratch = workload(1, phases, count, bytes, &arena_allocations, &capacity);
	clock_t finish = clock();
	CHECK(heap == scratch);
	CHECK(arena_allocations < heap_allocations);
	if (benchmark) {
		printf("%s: heap=%.3fms scratch=%.3fms heap_allocs=%zu arena_allocs=%zu "
			"retained_capacity=%zu digest=%llu\n", name,
			1000.0 * (double)(split - start) / CLOCKS_PER_SEC,
			1000.0 * (double)(finish - split) / CLOCKS_PER_SEC,
			heap_allocations, arena_allocations, capacity,
			(unsigned long long)scratch);
	}
}

int main(int argc, char **argv) {
	int benchmark = argc == 2 && strcmp(argv[1], "--benchmark") == 0;
	if (argc != 1 && !benchmark) return 2;
	test_scratch();
	run_workload("small_nodes", benchmark ? 600 : 15, 512, 64, benchmark);
	run_workload("large_buffers", benchmark ? 200 : 10, 64, 4096, benchmark);
	printf("PASS local scratch: alignment, overflow, reuse, POD parity\n");
	return 0;
}
