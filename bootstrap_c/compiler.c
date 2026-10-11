#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "sirius_runtime.h"
typedef struct {
	int64_t column;
	int64_t end;
	int64_t kind;
	int64_t line;
	int64_t start;
} s_record_100;
static inline s_record_100 s_record_100_copy(s_record_100 value) {
	return value;
}
static inline void s_record_100_drop(s_record_100 value) {
	(void)value;
}
static inline int64_t s_record_100_get_column_take(s_record_100 owned) {
	int64_t field = owned.column;
	s_record_100_drop(owned);
	return field;
}
static inline int64_t s_record_100_get_end_take(s_record_100 owned) {
	int64_t field = owned.end;
	s_record_100_drop(owned);
	return field;
}
static inline int64_t s_record_100_get_kind_take(s_record_100 owned) {
	int64_t field = owned.kind;
	s_record_100_drop(owned);
	return field;
}
static inline int64_t s_record_100_get_line_take(s_record_100 owned) {
	int64_t field = owned.line;
	s_record_100_drop(owned);
	return field;
}
static inline int64_t s_record_100_get_start_take(s_record_100 owned) {
	int64_t field = owned.start;
	s_record_100_drop(owned);
	return field;
}
typedef struct {
	int64_t count;
	int64_t first;
	int64_t kind;
	int64_t token;
} s_record_101;
static inline s_record_101 s_record_101_copy(s_record_101 value) {
	return value;
}
static inline void s_record_101_drop(s_record_101 value) {
	(void)value;
}
static inline int64_t s_record_101_get_count_take(s_record_101 owned) {
	int64_t field = owned.count;
	s_record_101_drop(owned);
	return field;
}
static inline int64_t s_record_101_get_first_take(s_record_101 owned) {
	int64_t field = owned.first;
	s_record_101_drop(owned);
	return field;
}
static inline int64_t s_record_101_get_kind_take(s_record_101 owned) {
	int64_t field = owned.kind;
	s_record_101_drop(owned);
	return field;
}
static inline int64_t s_record_101_get_token_take(s_record_101 owned) {
	int64_t field = owned.token;
	s_record_101_drop(owned);
	return field;
}
typedef struct {
	S_Array_i64 * edges;
	S_String * error;
	int64_t error_line;
	int64_t index;
	S_Array * nodes;
	S_String * source;
	S_Array * tokens;
} s_record_102;
static inline s_record_102 s_record_102_copy(s_record_102 value) {
	value.edges = s_array_i64_retain(value.edges);
	value.error = s_string_retain(value.error);
	value.nodes = s_array_retain(value.nodes);
	value.source = s_string_retain(value.source);
	value.tokens = s_array_retain(value.tokens);
	return value;
}
static inline void s_record_102_drop(s_record_102 value) {
	s_array_i64_release(value.edges);
	s_string_release(value.error);
	s_array_release(value.nodes);
	s_string_release(value.source);
	s_array_release(value.tokens);
	(void)value;
}
static inline S_Array_i64 * s_record_102_get_edges_take(s_record_102 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.edges);
	s_record_102_drop(owned);
	return field;
}
static inline S_String * s_record_102_get_error_take(s_record_102 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_102_drop(owned);
	return field;
}
static inline int64_t s_record_102_get_error_line_take(s_record_102 owned) {
	int64_t field = owned.error_line;
	s_record_102_drop(owned);
	return field;
}
static inline int64_t s_record_102_get_index_take(s_record_102 owned) {
	int64_t field = owned.index;
	s_record_102_drop(owned);
	return field;
}
static inline S_Array * s_record_102_get_nodes_take(s_record_102 owned) {
	S_Array * field = s_array_retain(owned.nodes);
	s_record_102_drop(owned);
	return field;
}
static inline S_String * s_record_102_get_source_take(s_record_102 owned) {
	S_String * field = s_string_retain(owned.source);
	s_record_102_drop(owned);
	return field;
}
static inline S_Array * s_record_102_get_tokens_take(s_record_102 owned) {
	S_Array * field = s_array_retain(owned.tokens);
	s_record_102_drop(owned);
	return field;
}
typedef struct {
	int64_t node;
	s_record_102 state;
} s_record_103;
static inline s_record_103 s_record_103_copy(s_record_103 value) {
	value.state = s_record_102_copy(value.state);
	return value;
}
static inline void s_record_103_drop(s_record_103 value) {
	s_record_102_drop(value.state);
	(void)value;
}
static inline int64_t s_record_103_get_node_take(s_record_103 owned) {
	int64_t field = owned.node;
	s_record_103_drop(owned);
	return field;
}
static inline s_record_102 s_record_103_get_state_take(s_record_103 owned) {
	s_record_102 field = s_record_102_copy(owned.state);
	s_record_103_drop(owned);
	return field;
}
typedef struct {
	int64_t kind;
	S_String * name;
} s_record_104;
static inline s_record_104 s_record_104_copy(s_record_104 value) {
	value.name = s_string_retain(value.name);
	return value;
}
static inline void s_record_104_drop(s_record_104 value) {
	s_string_release(value.name);
	(void)value;
}
static inline int64_t s_record_104_get_kind_take(s_record_104 owned) {
	int64_t field = owned.kind;
	s_record_104_drop(owned);
	return field;
}
static inline S_String * s_record_104_get_name_take(s_record_104 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_104_drop(owned);
	return field;
}
typedef struct {
	S_Array * fields;
	int64_t first;
	int64_t second;
	int64_t tag;
} s_record_105;
static inline s_record_105 s_record_105_copy(s_record_105 value) {
	value.fields = s_array_retain(value.fields);
	return value;
}
static inline void s_record_105_drop(s_record_105 value) {
	s_array_release(value.fields);
	(void)value;
}
static inline S_Array * s_record_105_get_fields_take(s_record_105 owned) {
	S_Array * field = s_array_retain(owned.fields);
	s_record_105_drop(owned);
	return field;
}
static inline int64_t s_record_105_get_first_take(s_record_105 owned) {
	int64_t field = owned.first;
	s_record_105_drop(owned);
	return field;
}
static inline int64_t s_record_105_get_second_take(s_record_105 owned) {
	int64_t field = owned.second;
	s_record_105_drop(owned);
	return field;
}
static inline int64_t s_record_105_get_tag_take(s_record_105 owned) {
	int64_t field = owned.tag;
	s_record_105_drop(owned);
	return field;
}
typedef struct {
	S_Array * entries;
} s_record_106;
static inline s_record_106 s_record_106_copy(s_record_106 value) {
	value.entries = s_array_retain(value.entries);
	return value;
}
static inline void s_record_106_drop(s_record_106 value) {
	s_array_release(value.entries);
	(void)value;
}
static inline S_Array * s_record_106_get_entries_take(s_record_106 owned) {
	S_Array * field = s_array_retain(owned.entries);
	s_record_106_drop(owned);
	return field;
}
typedef struct {
	int64_t id;
	s_record_106 table;
} s_record_107;
static inline s_record_107 s_record_107_copy(s_record_107 value) {
	value.table = s_record_106_copy(value.table);
	return value;
}
static inline void s_record_107_drop(s_record_107 value) {
	s_record_106_drop(value.table);
	(void)value;
}
static inline int64_t s_record_107_get_id_take(s_record_107 owned) {
	int64_t field = owned.id;
	s_record_107_drop(owned);
	return field;
}
static inline s_record_106 s_record_107_get_table_take(s_record_107 owned) {
	s_record_106 field = s_record_106_copy(owned.table);
	s_record_107_drop(owned);
	return field;
}
typedef struct {
	int64_t field_index;
	int64_t operation;
	int64_t source;
	int64_t type_id;
} s_record_108;
static inline s_record_108 s_record_108_copy(s_record_108 value) {
	return value;
}
static inline void s_record_108_drop(s_record_108 value) {
	(void)value;
}
static inline int64_t s_record_108_get_field_index_take(s_record_108 owned) {
	int64_t field = owned.field_index;
	s_record_108_drop(owned);
	return field;
}
static inline int64_t s_record_108_get_operation_take(s_record_108 owned) {
	int64_t field = owned.operation;
	s_record_108_drop(owned);
	return field;
}
static inline int64_t s_record_108_get_source_take(s_record_108 owned) {
	int64_t field = owned.source;
	s_record_108_drop(owned);
	return field;
}
static inline int64_t s_record_108_get_type_id_take(s_record_108 owned) {
	int64_t field = owned.type_id;
	s_record_108_drop(owned);
	return field;
}
typedef struct {
	S_String * error;
	int64_t field_index;
	int64_t type_id;
} s_record_109;
static inline s_record_109 s_record_109_copy(s_record_109 value) {
	value.error = s_string_retain(value.error);
	return value;
}
static inline void s_record_109_drop(s_record_109 value) {
	s_string_release(value.error);
	(void)value;
}
static inline S_String * s_record_109_get_error_take(s_record_109 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_109_drop(owned);
	return field;
}
static inline int64_t s_record_109_get_field_index_take(s_record_109 owned) {
	int64_t field = owned.field_index;
	s_record_109_drop(owned);
	return field;
}
static inline int64_t s_record_109_get_type_id_take(s_record_109 owned) {
	int64_t field = owned.type_id;
	s_record_109_drop(owned);
	return field;
}
typedef struct {
	int64_t depth;
	int64_t kind;
	bool mutable;
	S_String * name;
	int64_t type_id;
} s_record_110;
static inline s_record_110 s_record_110_copy(s_record_110 value) {
	value.name = s_string_retain(value.name);
	return value;
}
static inline void s_record_110_drop(s_record_110 value) {
	s_string_release(value.name);
	(void)value;
}
static inline int64_t s_record_110_get_depth_take(s_record_110 owned) {
	int64_t field = owned.depth;
	s_record_110_drop(owned);
	return field;
}
static inline int64_t s_record_110_get_kind_take(s_record_110 owned) {
	int64_t field = owned.kind;
	s_record_110_drop(owned);
	return field;
}
static inline bool s_record_110_get_mutable_take(s_record_110 owned) {
	bool field = owned.mutable;
	s_record_110_drop(owned);
	return field;
}
static inline S_String * s_record_110_get_name_take(s_record_110 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_110_drop(owned);
	return field;
}
static inline int64_t s_record_110_get_type_id_take(s_record_110 owned) {
	int64_t field = owned.type_id;
	s_record_110_drop(owned);
	return field;
}
typedef struct {
	S_String * error;
	s_record_106 table;
	int64_t type_id;
} s_record_111;
static inline s_record_111 s_record_111_copy(s_record_111 value) {
	value.error = s_string_retain(value.error);
	value.table = s_record_106_copy(value.table);
	return value;
}
static inline void s_record_111_drop(s_record_111 value) {
	s_string_release(value.error);
	s_record_106_drop(value.table);
	(void)value;
}
static inline S_String * s_record_111_get_error_take(s_record_111 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_111_drop(owned);
	return field;
}
static inline s_record_106 s_record_111_get_table_take(s_record_111 owned) {
	s_record_106 field = s_record_106_copy(owned.table);
	s_record_111_drop(owned);
	return field;
}
static inline int64_t s_record_111_get_type_id_take(s_record_111 owned) {
	int64_t field = owned.type_id;
	s_record_111_drop(owned);
	return field;
}
typedef struct {
	int64_t body;
	bool constructor;
	int64_t kind;
	S_String * name;
	S_Array * params;
	S_String * symbol;
} s_record_112;
static inline s_record_112 s_record_112_copy(s_record_112 value) {
	value.name = s_string_retain(value.name);
	value.params = s_array_retain(value.params);
	value.symbol = s_string_retain(value.symbol);
	return value;
}
static inline void s_record_112_drop(s_record_112 value) {
	s_string_release(value.name);
	s_array_release(value.params);
	s_string_release(value.symbol);
	(void)value;
}
static inline int64_t s_record_112_get_body_take(s_record_112 owned) {
	int64_t field = owned.body;
	s_record_112_drop(owned);
	return field;
}
static inline bool s_record_112_get_constructor_take(s_record_112 owned) {
	bool field = owned.constructor;
	s_record_112_drop(owned);
	return field;
}
static inline int64_t s_record_112_get_kind_take(s_record_112 owned) {
	int64_t field = owned.kind;
	s_record_112_drop(owned);
	return field;
}
static inline S_String * s_record_112_get_name_take(s_record_112 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_112_drop(owned);
	return field;
}
static inline S_Array * s_record_112_get_params_take(s_record_112 owned) {
	S_Array * field = s_array_retain(owned.params);
	s_record_112_drop(owned);
	return field;
}
static inline S_String * s_record_112_get_symbol_take(s_record_112 owned) {
	S_String * field = s_string_retain(owned.symbol);
	s_record_112_drop(owned);
	return field;
}
typedef struct {
	S_String * code;
	int64_t kind;
	S_Array_i64 * kinds;
	S_String * name;
	int64_t status;
	S_String * symbol;
	S_Array_i64 * type_ids;
} s_record_113;
static inline s_record_113 s_record_113_copy(s_record_113 value) {
	value.code = s_string_retain(value.code);
	value.kinds = s_array_i64_retain(value.kinds);
	value.name = s_string_retain(value.name);
	value.symbol = s_string_retain(value.symbol);
	value.type_ids = s_array_i64_retain(value.type_ids);
	return value;
}
static inline void s_record_113_drop(s_record_113 value) {
	s_string_release(value.code);
	s_array_i64_release(value.kinds);
	s_string_release(value.name);
	s_string_release(value.symbol);
	s_array_i64_release(value.type_ids);
	(void)value;
}
static inline S_String * s_record_113_get_code_take(s_record_113 owned) {
	S_String * field = s_string_retain(owned.code);
	s_record_113_drop(owned);
	return field;
}
static inline int64_t s_record_113_get_kind_take(s_record_113 owned) {
	int64_t field = owned.kind;
	s_record_113_drop(owned);
	return field;
}
static inline S_Array_i64 * s_record_113_get_kinds_take(s_record_113 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.kinds);
	s_record_113_drop(owned);
	return field;
}
static inline S_String * s_record_113_get_name_take(s_record_113 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_113_drop(owned);
	return field;
}
static inline int64_t s_record_113_get_status_take(s_record_113 owned) {
	int64_t field = owned.status;
	s_record_113_drop(owned);
	return field;
}
static inline S_String * s_record_113_get_symbol_take(s_record_113 owned) {
	S_String * field = s_string_retain(owned.symbol);
	s_record_113_drop(owned);
	return field;
}
static inline S_Array_i64 * s_record_113_get_type_ids_take(s_record_113 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.type_ids);
	s_record_113_drop(owned);
	return field;
}
typedef struct {
	S_String * name;
	S_String * value;
} s_record_114;
static inline s_record_114 s_record_114_copy(s_record_114 value) {
	value.name = s_string_retain(value.name);
	value.value = s_string_retain(value.value);
	return value;
}
static inline void s_record_114_drop(s_record_114 value) {
	s_string_release(value.name);
	s_string_release(value.value);
	(void)value;
}
static inline S_String * s_record_114_get_name_take(s_record_114 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_114_drop(owned);
	return field;
}
static inline S_String * s_record_114_get_value_take(s_record_114 owned) {
	S_String * field = s_string_retain(owned.value);
	s_record_114_drop(owned);
	return field;
}
typedef struct {
	int64_t canonical;
	S_Array * fields;
} s_record_115;
static inline s_record_115 s_record_115_copy(s_record_115 value) {
	value.fields = s_array_retain(value.fields);
	return value;
}
static inline void s_record_115_drop(s_record_115 value) {
	s_array_release(value.fields);
	(void)value;
}
static inline int64_t s_record_115_get_canonical_take(s_record_115 owned) {
	int64_t field = owned.canonical;
	s_record_115_drop(owned);
	return field;
}
static inline S_Array * s_record_115_get_fields_take(s_record_115 owned) {
	S_Array * field = s_array_retain(owned.fields);
	s_record_115_drop(owned);
	return field;
}
typedef struct {
	S_Array * active_params;
	S_Array * constants;
	int64_t counter;
	S_Array_i64 * dict_kinds;
	S_String * error;
	int64_t error_line;
	S_String * function_name;
	S_Array * functions;
	S_Array * hir;
	int64_t indentation;
	S_Array * instances;
	S_Array * lines;
	int64_t loop_depth;
	int64_t loop_scope;
	S_Array * records;
	int64_t return_kind;
	int64_t scope_depth;
	S_Array * symbols;
	s_record_102 tree;
	s_record_106 types;
} s_record_116;
static inline s_record_116 s_record_116_copy(s_record_116 value) {
	value.active_params = s_array_retain(value.active_params);
	value.constants = s_array_retain(value.constants);
	value.dict_kinds = s_array_i64_retain(value.dict_kinds);
	value.error = s_string_retain(value.error);
	value.function_name = s_string_retain(value.function_name);
	value.functions = s_array_retain(value.functions);
	value.hir = s_array_retain(value.hir);
	value.instances = s_array_retain(value.instances);
	value.lines = s_array_retain(value.lines);
	value.records = s_array_retain(value.records);
	value.symbols = s_array_retain(value.symbols);
	value.tree = s_record_102_copy(value.tree);
	value.types = s_record_106_copy(value.types);
	return value;
}
static inline void s_record_116_drop(s_record_116 value) {
	s_array_release(value.active_params);
	s_array_release(value.constants);
	s_array_i64_release(value.dict_kinds);
	s_string_release(value.error);
	s_string_release(value.function_name);
	s_array_release(value.functions);
	s_array_release(value.hir);
	s_array_release(value.instances);
	s_array_release(value.lines);
	s_array_release(value.records);
	s_array_release(value.symbols);
	s_record_102_drop(value.tree);
	s_record_106_drop(value.types);
	(void)value;
}
static inline S_Array * s_record_116_get_active_params_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.active_params);
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_constants_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.constants);
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_counter_take(s_record_116 owned) {
	int64_t field = owned.counter;
	s_record_116_drop(owned);
	return field;
}
static inline S_Array_i64 * s_record_116_get_dict_kinds_take(s_record_116 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.dict_kinds);
	s_record_116_drop(owned);
	return field;
}
static inline S_String * s_record_116_get_error_take(s_record_116 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_error_line_take(s_record_116 owned) {
	int64_t field = owned.error_line;
	s_record_116_drop(owned);
	return field;
}
static inline S_String * s_record_116_get_function_name_take(s_record_116 owned) {
	S_String * field = s_string_retain(owned.function_name);
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_functions_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.functions);
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_hir_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.hir);
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_indentation_take(s_record_116 owned) {
	int64_t field = owned.indentation;
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_instances_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.instances);
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_lines_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.lines);
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_loop_depth_take(s_record_116 owned) {
	int64_t field = owned.loop_depth;
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_loop_scope_take(s_record_116 owned) {
	int64_t field = owned.loop_scope;
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_records_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.records);
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_return_kind_take(s_record_116 owned) {
	int64_t field = owned.return_kind;
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_scope_depth_take(s_record_116 owned) {
	int64_t field = owned.scope_depth;
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_symbols_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.symbols);
	s_record_116_drop(owned);
	return field;
}
static inline s_record_102 s_record_116_get_tree_take(s_record_116 owned) {
	s_record_102 field = s_record_102_copy(owned.tree);
	s_record_116_drop(owned);
	return field;
}
static inline s_record_106 s_record_116_get_types_take(s_record_116 owned) {
	s_record_106 field = s_record_106_copy(owned.types);
	s_record_116_drop(owned);
	return field;
}
typedef struct {
	int64_t kind;
	s_record_116 state;
	S_String * text;
} s_record_117;
static inline s_record_117 s_record_117_copy(s_record_117 value) {
	value.state = s_record_116_copy(value.state);
	value.text = s_string_retain(value.text);
	return value;
}
static inline void s_record_117_drop(s_record_117 value) {
	s_record_116_drop(value.state);
	s_string_release(value.text);
	(void)value;
}
static inline int64_t s_record_117_get_kind_take(s_record_117 owned) {
	int64_t field = owned.kind;
	s_record_117_drop(owned);
	return field;
}
static inline s_record_116 s_record_117_get_state_take(s_record_117 owned) {
	s_record_116 field = s_record_116_copy(owned.state);
	s_record_117_drop(owned);
	return field;
}
static inline S_String * s_record_117_get_text_take(s_record_117 owned) {
	S_String * field = s_string_retain(owned.text);
	s_record_117_drop(owned);
	return field;
}
typedef struct {
	S_String * code;
	S_String * error;
	int64_t line;
} s_record_118;
static inline s_record_118 s_record_118_copy(s_record_118 value) {
	value.code = s_string_retain(value.code);
	value.error = s_string_retain(value.error);
	return value;
}
static inline void s_record_118_drop(s_record_118 value) {
	s_string_release(value.code);
	s_string_release(value.error);
	(void)value;
}
static inline S_String * s_record_118_get_code_take(s_record_118 owned) {
	S_String * field = s_string_retain(owned.code);
	s_record_118_drop(owned);
	return field;
}
static inline S_String * s_record_118_get_error_take(s_record_118 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_118_drop(owned);
	return field;
}
static inline int64_t s_record_118_get_line_take(s_record_118 owned) {
	int64_t field = owned.line;
	s_record_118_drop(owned);
	return field;
}
typedef struct {
	S_Array_i64 * ids;
	s_record_106 table;
} s_record_119;
static inline s_record_119 s_record_119_copy(s_record_119 value) {
	value.ids = s_array_i64_retain(value.ids);
	value.table = s_record_106_copy(value.table);
	return value;
}
static inline void s_record_119_drop(s_record_119 value) {
	s_array_i64_release(value.ids);
	s_record_106_drop(value.table);
	(void)value;
}
static inline S_Array_i64 * s_record_119_get_ids_take(s_record_119 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.ids);
	s_record_119_drop(owned);
	return field;
}
static inline s_record_106 s_record_119_get_table_take(s_record_119 owned) {
	s_record_106 field = s_record_106_copy(owned.table);
	s_record_119_drop(owned);
	return field;
}
typedef struct {
	S_String * c_name;
	S_String * copy;
	S_String * drop;
	int64_t type_id;
} s_record_120;
static inline s_record_120 s_record_120_copy(s_record_120 value) {
	value.c_name = s_string_retain(value.c_name);
	value.copy = s_string_retain(value.copy);
	value.drop = s_string_retain(value.drop);
	return value;
}
static inline void s_record_120_drop(s_record_120 value) {
	s_string_release(value.c_name);
	s_string_release(value.copy);
	s_string_release(value.drop);
	(void)value;
}
static inline S_String * s_record_120_get_c_name_take(s_record_120 owned) {
	S_String * field = s_string_retain(owned.c_name);
	s_record_120_drop(owned);
	return field;
}
static inline S_String * s_record_120_get_copy_take(s_record_120 owned) {
	S_String * field = s_string_retain(owned.copy);
	s_record_120_drop(owned);
	return field;
}
static inline S_String * s_record_120_get_drop_take(s_record_120 owned) {
	S_String * field = s_string_retain(owned.drop);
	s_record_120_drop(owned);
	return field;
}
static inline int64_t s_record_120_get_type_id_take(s_record_120 owned) {
	int64_t field = owned.type_id;
	s_record_120_drop(owned);
	return field;
}
typedef struct {
	s_record_120 abi;
	S_String * name;
	int64_t type_id;
} s_record_121;
static inline s_record_121 s_record_121_copy(s_record_121 value) {
	value.abi = s_record_120_copy(value.abi);
	value.name = s_string_retain(value.name);
	return value;
}
static inline void s_record_121_drop(s_record_121 value) {
	s_record_120_drop(value.abi);
	s_string_release(value.name);
	(void)value;
}
static inline s_record_120 s_record_121_get_abi_take(s_record_121 owned) {
	s_record_120 field = s_record_120_copy(owned.abi);
	s_record_121_drop(owned);
	return field;
}
static inline S_String * s_record_121_get_name_take(s_record_121 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_121_drop(owned);
	return field;
}
static inline int64_t s_record_121_get_type_id_take(s_record_121 owned) {
	int64_t field = owned.type_id;
	s_record_121_drop(owned);
	return field;
}
typedef S_Optional_string s_optional_4;
static inline s_optional_4 s_optional_4_copy(s_optional_4 owned) {
	if (owned.has_value) owned.value = s_string_retain(owned.value);
	return owned;
}
static inline void s_optional_4_drop(s_optional_4 owned) {
	if (owned.has_value) s_string_release(owned.value);
}
static inline void s_ga_8_copy(void *dst, const void *src) { *(S_String * *)dst = s_string_retain(*(S_String * const *)src); }
static inline void s_ga_8_drop(void *value) { s_string_release(*(S_String * *)value); }
static inline S_Array *s_ga_8_make_take(size_t count, S_String * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_String *), s_ga_8_copy, s_ga_8_drop);
	for (size_t i = 0; i < count; ++i) s_string_release(items[i]);
	return result;
}
static inline S_Array *s_ga_8_append_take(S_Array *owned, S_String * item) {
	S_Array *result = s_array_append(owned, &item);
	s_string_release(item);
	return result;
}
static inline S_String * s_ga_8_get_checked_take(S_Array *owned, int64_t index) {
	S_String * const *p = (S_String * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_String * value = s_string_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_String * s_ga_8_get_borrow(const S_Array *array, int64_t index) {
	S_String * const *p = (S_String * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_string_retain(*p);
}
static inline void s_ga_8_set_take(S_Array **owned, int64_t index, S_String * item) {
	s_array_set(owned, index, &item);
	s_string_release(item);
}
static inline void s_ga_10100_copy(void *dst, const void *src) { *(s_record_100 *)dst = s_record_100_copy(*(s_record_100 const *)src); }
static inline void s_ga_10100_drop(void *value) { s_record_100_drop(*(s_record_100 *)value); }
static inline S_Array *s_ga_10100_make_take(size_t count, s_record_100 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_100), s_ga_10100_copy, s_ga_10100_drop);
	for (size_t i = 0; i < count; ++i) s_record_100_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10100_append_take(S_Array *owned, s_record_100 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_100_drop(item);
	return result;
}
static inline s_record_100 s_ga_10100_get_checked_take(S_Array *owned, int64_t index) {
	s_record_100 const *p = (s_record_100 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_100 value = s_record_100_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_100 s_ga_10100_get_borrow(const S_Array *array, int64_t index) {
	s_record_100 const *p = (s_record_100 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_100_copy(*p);
}
static inline void s_ga_10100_set_take(S_Array **owned, int64_t index, s_record_100 item) {
	s_array_set(owned, index, &item);
	s_record_100_drop(item);
}
static inline void s_ga_10101_copy(void *dst, const void *src) { *(s_record_101 *)dst = s_record_101_copy(*(s_record_101 const *)src); }
static inline void s_ga_10101_drop(void *value) { s_record_101_drop(*(s_record_101 *)value); }
static inline S_Array *s_ga_10101_make_take(size_t count, s_record_101 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_101), s_ga_10101_copy, s_ga_10101_drop);
	for (size_t i = 0; i < count; ++i) s_record_101_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10101_append_take(S_Array *owned, s_record_101 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_101_drop(item);
	return result;
}
static inline s_record_101 s_ga_10101_get_checked_take(S_Array *owned, int64_t index) {
	s_record_101 const *p = (s_record_101 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_101 value = s_record_101_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_101 s_ga_10101_get_borrow(const S_Array *array, int64_t index) {
	s_record_101 const *p = (s_record_101 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_101_copy(*p);
}
static inline void s_ga_10101_set_take(S_Array **owned, int64_t index, s_record_101 item) {
	s_array_set(owned, index, &item);
	s_record_101_drop(item);
}
static inline void s_ga_10102_copy(void *dst, const void *src) { *(s_record_102 *)dst = s_record_102_copy(*(s_record_102 const *)src); }
static inline void s_ga_10102_drop(void *value) { s_record_102_drop(*(s_record_102 *)value); }
static inline S_Array *s_ga_10102_make_take(size_t count, s_record_102 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_102), s_ga_10102_copy, s_ga_10102_drop);
	for (size_t i = 0; i < count; ++i) s_record_102_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10102_append_take(S_Array *owned, s_record_102 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_102_drop(item);
	return result;
}
static inline s_record_102 s_ga_10102_get_checked_take(S_Array *owned, int64_t index) {
	s_record_102 const *p = (s_record_102 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_102 value = s_record_102_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_102 s_ga_10102_get_borrow(const S_Array *array, int64_t index) {
	s_record_102 const *p = (s_record_102 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_102_copy(*p);
}
static inline void s_ga_10102_set_take(S_Array **owned, int64_t index, s_record_102 item) {
	s_array_set(owned, index, &item);
	s_record_102_drop(item);
}
static inline void s_ga_10103_copy(void *dst, const void *src) { *(s_record_103 *)dst = s_record_103_copy(*(s_record_103 const *)src); }
static inline void s_ga_10103_drop(void *value) { s_record_103_drop(*(s_record_103 *)value); }
static inline S_Array *s_ga_10103_make_take(size_t count, s_record_103 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_103), s_ga_10103_copy, s_ga_10103_drop);
	for (size_t i = 0; i < count; ++i) s_record_103_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10103_append_take(S_Array *owned, s_record_103 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_103_drop(item);
	return result;
}
static inline s_record_103 s_ga_10103_get_checked_take(S_Array *owned, int64_t index) {
	s_record_103 const *p = (s_record_103 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_103 value = s_record_103_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_103 s_ga_10103_get_borrow(const S_Array *array, int64_t index) {
	s_record_103 const *p = (s_record_103 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_103_copy(*p);
}
static inline void s_ga_10103_set_take(S_Array **owned, int64_t index, s_record_103 item) {
	s_array_set(owned, index, &item);
	s_record_103_drop(item);
}
static inline void s_ga_10104_copy(void *dst, const void *src) { *(s_record_104 *)dst = s_record_104_copy(*(s_record_104 const *)src); }
static inline void s_ga_10104_drop(void *value) { s_record_104_drop(*(s_record_104 *)value); }
static inline S_Array *s_ga_10104_make_take(size_t count, s_record_104 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_104), s_ga_10104_copy, s_ga_10104_drop);
	for (size_t i = 0; i < count; ++i) s_record_104_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10104_append_take(S_Array *owned, s_record_104 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_104_drop(item);
	return result;
}
static inline s_record_104 s_ga_10104_get_checked_take(S_Array *owned, int64_t index) {
	s_record_104 const *p = (s_record_104 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_104 value = s_record_104_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_104 s_ga_10104_get_borrow(const S_Array *array, int64_t index) {
	s_record_104 const *p = (s_record_104 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_104_copy(*p);
}
static inline void s_ga_10104_set_take(S_Array **owned, int64_t index, s_record_104 item) {
	s_array_set(owned, index, &item);
	s_record_104_drop(item);
}
static inline void s_ga_10105_copy(void *dst, const void *src) { *(s_record_105 *)dst = s_record_105_copy(*(s_record_105 const *)src); }
static inline void s_ga_10105_drop(void *value) { s_record_105_drop(*(s_record_105 *)value); }
static inline S_Array *s_ga_10105_make_take(size_t count, s_record_105 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_105), s_ga_10105_copy, s_ga_10105_drop);
	for (size_t i = 0; i < count; ++i) s_record_105_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10105_append_take(S_Array *owned, s_record_105 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_105_drop(item);
	return result;
}
static inline s_record_105 s_ga_10105_get_checked_take(S_Array *owned, int64_t index) {
	s_record_105 const *p = (s_record_105 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_105 value = s_record_105_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_105 s_ga_10105_get_borrow(const S_Array *array, int64_t index) {
	s_record_105 const *p = (s_record_105 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_105_copy(*p);
}
static inline void s_ga_10105_set_take(S_Array **owned, int64_t index, s_record_105 item) {
	s_array_set(owned, index, &item);
	s_record_105_drop(item);
}
static inline void s_ga_10106_copy(void *dst, const void *src) { *(s_record_106 *)dst = s_record_106_copy(*(s_record_106 const *)src); }
static inline void s_ga_10106_drop(void *value) { s_record_106_drop(*(s_record_106 *)value); }
static inline S_Array *s_ga_10106_make_take(size_t count, s_record_106 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_106), s_ga_10106_copy, s_ga_10106_drop);
	for (size_t i = 0; i < count; ++i) s_record_106_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10106_append_take(S_Array *owned, s_record_106 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_106_drop(item);
	return result;
}
static inline s_record_106 s_ga_10106_get_checked_take(S_Array *owned, int64_t index) {
	s_record_106 const *p = (s_record_106 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_106 value = s_record_106_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_106 s_ga_10106_get_borrow(const S_Array *array, int64_t index) {
	s_record_106 const *p = (s_record_106 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_106_copy(*p);
}
static inline void s_ga_10106_set_take(S_Array **owned, int64_t index, s_record_106 item) {
	s_array_set(owned, index, &item);
	s_record_106_drop(item);
}
static inline void s_ga_10107_copy(void *dst, const void *src) { *(s_record_107 *)dst = s_record_107_copy(*(s_record_107 const *)src); }
static inline void s_ga_10107_drop(void *value) { s_record_107_drop(*(s_record_107 *)value); }
static inline S_Array *s_ga_10107_make_take(size_t count, s_record_107 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_107), s_ga_10107_copy, s_ga_10107_drop);
	for (size_t i = 0; i < count; ++i) s_record_107_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10107_append_take(S_Array *owned, s_record_107 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_107_drop(item);
	return result;
}
static inline s_record_107 s_ga_10107_get_checked_take(S_Array *owned, int64_t index) {
	s_record_107 const *p = (s_record_107 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_107 value = s_record_107_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_107 s_ga_10107_get_borrow(const S_Array *array, int64_t index) {
	s_record_107 const *p = (s_record_107 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_107_copy(*p);
}
static inline void s_ga_10107_set_take(S_Array **owned, int64_t index, s_record_107 item) {
	s_array_set(owned, index, &item);
	s_record_107_drop(item);
}
static inline void s_ga_10108_copy(void *dst, const void *src) { *(s_record_108 *)dst = s_record_108_copy(*(s_record_108 const *)src); }
static inline void s_ga_10108_drop(void *value) { s_record_108_drop(*(s_record_108 *)value); }
static inline S_Array *s_ga_10108_make_take(size_t count, s_record_108 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_108), s_ga_10108_copy, s_ga_10108_drop);
	for (size_t i = 0; i < count; ++i) s_record_108_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10108_append_take(S_Array *owned, s_record_108 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_108_drop(item);
	return result;
}
static inline s_record_108 s_ga_10108_get_checked_take(S_Array *owned, int64_t index) {
	s_record_108 const *p = (s_record_108 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_108 value = s_record_108_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_108 s_ga_10108_get_borrow(const S_Array *array, int64_t index) {
	s_record_108 const *p = (s_record_108 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_108_copy(*p);
}
static inline void s_ga_10108_set_take(S_Array **owned, int64_t index, s_record_108 item) {
	s_array_set(owned, index, &item);
	s_record_108_drop(item);
}
static inline void s_ga_10109_copy(void *dst, const void *src) { *(s_record_109 *)dst = s_record_109_copy(*(s_record_109 const *)src); }
static inline void s_ga_10109_drop(void *value) { s_record_109_drop(*(s_record_109 *)value); }
static inline S_Array *s_ga_10109_make_take(size_t count, s_record_109 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_109), s_ga_10109_copy, s_ga_10109_drop);
	for (size_t i = 0; i < count; ++i) s_record_109_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10109_append_take(S_Array *owned, s_record_109 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_109_drop(item);
	return result;
}
static inline s_record_109 s_ga_10109_get_checked_take(S_Array *owned, int64_t index) {
	s_record_109 const *p = (s_record_109 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_109 value = s_record_109_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_109 s_ga_10109_get_borrow(const S_Array *array, int64_t index) {
	s_record_109 const *p = (s_record_109 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_109_copy(*p);
}
static inline void s_ga_10109_set_take(S_Array **owned, int64_t index, s_record_109 item) {
	s_array_set(owned, index, &item);
	s_record_109_drop(item);
}
static inline void s_ga_10110_copy(void *dst, const void *src) { *(s_record_110 *)dst = s_record_110_copy(*(s_record_110 const *)src); }
static inline void s_ga_10110_drop(void *value) { s_record_110_drop(*(s_record_110 *)value); }
static inline S_Array *s_ga_10110_make_take(size_t count, s_record_110 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_110), s_ga_10110_copy, s_ga_10110_drop);
	for (size_t i = 0; i < count; ++i) s_record_110_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10110_append_take(S_Array *owned, s_record_110 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_110_drop(item);
	return result;
}
static inline s_record_110 s_ga_10110_get_checked_take(S_Array *owned, int64_t index) {
	s_record_110 const *p = (s_record_110 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_110 value = s_record_110_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_110 s_ga_10110_get_borrow(const S_Array *array, int64_t index) {
	s_record_110 const *p = (s_record_110 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_110_copy(*p);
}
static inline void s_ga_10110_set_take(S_Array **owned, int64_t index, s_record_110 item) {
	s_array_set(owned, index, &item);
	s_record_110_drop(item);
}
static inline void s_ga_10111_copy(void *dst, const void *src) { *(s_record_111 *)dst = s_record_111_copy(*(s_record_111 const *)src); }
static inline void s_ga_10111_drop(void *value) { s_record_111_drop(*(s_record_111 *)value); }
static inline S_Array *s_ga_10111_make_take(size_t count, s_record_111 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_111), s_ga_10111_copy, s_ga_10111_drop);
	for (size_t i = 0; i < count; ++i) s_record_111_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10111_append_take(S_Array *owned, s_record_111 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_111_drop(item);
	return result;
}
static inline s_record_111 s_ga_10111_get_checked_take(S_Array *owned, int64_t index) {
	s_record_111 const *p = (s_record_111 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_111 value = s_record_111_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_111 s_ga_10111_get_borrow(const S_Array *array, int64_t index) {
	s_record_111 const *p = (s_record_111 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_111_copy(*p);
}
static inline void s_ga_10111_set_take(S_Array **owned, int64_t index, s_record_111 item) {
	s_array_set(owned, index, &item);
	s_record_111_drop(item);
}
static inline void s_ga_10112_copy(void *dst, const void *src) { *(s_record_112 *)dst = s_record_112_copy(*(s_record_112 const *)src); }
static inline void s_ga_10112_drop(void *value) { s_record_112_drop(*(s_record_112 *)value); }
static inline S_Array *s_ga_10112_make_take(size_t count, s_record_112 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_112), s_ga_10112_copy, s_ga_10112_drop);
	for (size_t i = 0; i < count; ++i) s_record_112_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10112_append_take(S_Array *owned, s_record_112 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_112_drop(item);
	return result;
}
static inline s_record_112 s_ga_10112_get_checked_take(S_Array *owned, int64_t index) {
	s_record_112 const *p = (s_record_112 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_112 value = s_record_112_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_112 s_ga_10112_get_borrow(const S_Array *array, int64_t index) {
	s_record_112 const *p = (s_record_112 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_112_copy(*p);
}
static inline void s_ga_10112_set_take(S_Array **owned, int64_t index, s_record_112 item) {
	s_array_set(owned, index, &item);
	s_record_112_drop(item);
}
static inline void s_ga_10113_copy(void *dst, const void *src) { *(s_record_113 *)dst = s_record_113_copy(*(s_record_113 const *)src); }
static inline void s_ga_10113_drop(void *value) { s_record_113_drop(*(s_record_113 *)value); }
static inline S_Array *s_ga_10113_make_take(size_t count, s_record_113 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_113), s_ga_10113_copy, s_ga_10113_drop);
	for (size_t i = 0; i < count; ++i) s_record_113_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10113_append_take(S_Array *owned, s_record_113 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_113_drop(item);
	return result;
}
static inline s_record_113 s_ga_10113_get_checked_take(S_Array *owned, int64_t index) {
	s_record_113 const *p = (s_record_113 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_113 value = s_record_113_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_113 s_ga_10113_get_borrow(const S_Array *array, int64_t index) {
	s_record_113 const *p = (s_record_113 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_113_copy(*p);
}
static inline void s_ga_10113_set_take(S_Array **owned, int64_t index, s_record_113 item) {
	s_array_set(owned, index, &item);
	s_record_113_drop(item);
}
static inline void s_ga_10114_copy(void *dst, const void *src) { *(s_record_114 *)dst = s_record_114_copy(*(s_record_114 const *)src); }
static inline void s_ga_10114_drop(void *value) { s_record_114_drop(*(s_record_114 *)value); }
static inline S_Array *s_ga_10114_make_take(size_t count, s_record_114 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_114), s_ga_10114_copy, s_ga_10114_drop);
	for (size_t i = 0; i < count; ++i) s_record_114_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10114_append_take(S_Array *owned, s_record_114 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_114_drop(item);
	return result;
}
static inline s_record_114 s_ga_10114_get_checked_take(S_Array *owned, int64_t index) {
	s_record_114 const *p = (s_record_114 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_114 value = s_record_114_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_114 s_ga_10114_get_borrow(const S_Array *array, int64_t index) {
	s_record_114 const *p = (s_record_114 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_114_copy(*p);
}
static inline void s_ga_10114_set_take(S_Array **owned, int64_t index, s_record_114 item) {
	s_array_set(owned, index, &item);
	s_record_114_drop(item);
}
static inline void s_ga_10115_copy(void *dst, const void *src) { *(s_record_115 *)dst = s_record_115_copy(*(s_record_115 const *)src); }
static inline void s_ga_10115_drop(void *value) { s_record_115_drop(*(s_record_115 *)value); }
static inline S_Array *s_ga_10115_make_take(size_t count, s_record_115 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_115), s_ga_10115_copy, s_ga_10115_drop);
	for (size_t i = 0; i < count; ++i) s_record_115_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10115_append_take(S_Array *owned, s_record_115 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_115_drop(item);
	return result;
}
static inline s_record_115 s_ga_10115_get_checked_take(S_Array *owned, int64_t index) {
	s_record_115 const *p = (s_record_115 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_115 value = s_record_115_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_115 s_ga_10115_get_borrow(const S_Array *array, int64_t index) {
	s_record_115 const *p = (s_record_115 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_115_copy(*p);
}
static inline void s_ga_10115_set_take(S_Array **owned, int64_t index, s_record_115 item) {
	s_array_set(owned, index, &item);
	s_record_115_drop(item);
}
static inline void s_ga_10116_copy(void *dst, const void *src) { *(s_record_116 *)dst = s_record_116_copy(*(s_record_116 const *)src); }
static inline void s_ga_10116_drop(void *value) { s_record_116_drop(*(s_record_116 *)value); }
static inline S_Array *s_ga_10116_make_take(size_t count, s_record_116 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_116), s_ga_10116_copy, s_ga_10116_drop);
	for (size_t i = 0; i < count; ++i) s_record_116_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10116_append_take(S_Array *owned, s_record_116 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_116_drop(item);
	return result;
}
static inline s_record_116 s_ga_10116_get_checked_take(S_Array *owned, int64_t index) {
	s_record_116 const *p = (s_record_116 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_116 value = s_record_116_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_116 s_ga_10116_get_borrow(const S_Array *array, int64_t index) {
	s_record_116 const *p = (s_record_116 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_116_copy(*p);
}
static inline void s_ga_10116_set_take(S_Array **owned, int64_t index, s_record_116 item) {
	s_array_set(owned, index, &item);
	s_record_116_drop(item);
}
static inline void s_ga_10117_copy(void *dst, const void *src) { *(s_record_117 *)dst = s_record_117_copy(*(s_record_117 const *)src); }
static inline void s_ga_10117_drop(void *value) { s_record_117_drop(*(s_record_117 *)value); }
static inline S_Array *s_ga_10117_make_take(size_t count, s_record_117 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_117), s_ga_10117_copy, s_ga_10117_drop);
	for (size_t i = 0; i < count; ++i) s_record_117_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10117_append_take(S_Array *owned, s_record_117 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_117_drop(item);
	return result;
}
static inline s_record_117 s_ga_10117_get_checked_take(S_Array *owned, int64_t index) {
	s_record_117 const *p = (s_record_117 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_117 value = s_record_117_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_117 s_ga_10117_get_borrow(const S_Array *array, int64_t index) {
	s_record_117 const *p = (s_record_117 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_117_copy(*p);
}
static inline void s_ga_10117_set_take(S_Array **owned, int64_t index, s_record_117 item) {
	s_array_set(owned, index, &item);
	s_record_117_drop(item);
}
static inline void s_ga_10118_copy(void *dst, const void *src) { *(s_record_118 *)dst = s_record_118_copy(*(s_record_118 const *)src); }
static inline void s_ga_10118_drop(void *value) { s_record_118_drop(*(s_record_118 *)value); }
static inline S_Array *s_ga_10118_make_take(size_t count, s_record_118 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_118), s_ga_10118_copy, s_ga_10118_drop);
	for (size_t i = 0; i < count; ++i) s_record_118_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10118_append_take(S_Array *owned, s_record_118 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_118_drop(item);
	return result;
}
static inline s_record_118 s_ga_10118_get_checked_take(S_Array *owned, int64_t index) {
	s_record_118 const *p = (s_record_118 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_118 value = s_record_118_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_118 s_ga_10118_get_borrow(const S_Array *array, int64_t index) {
	s_record_118 const *p = (s_record_118 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_118_copy(*p);
}
static inline void s_ga_10118_set_take(S_Array **owned, int64_t index, s_record_118 item) {
	s_array_set(owned, index, &item);
	s_record_118_drop(item);
}
static inline void s_ga_10119_copy(void *dst, const void *src) { *(s_record_119 *)dst = s_record_119_copy(*(s_record_119 const *)src); }
static inline void s_ga_10119_drop(void *value) { s_record_119_drop(*(s_record_119 *)value); }
static inline S_Array *s_ga_10119_make_take(size_t count, s_record_119 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_119), s_ga_10119_copy, s_ga_10119_drop);
	for (size_t i = 0; i < count; ++i) s_record_119_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10119_append_take(S_Array *owned, s_record_119 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_119_drop(item);
	return result;
}
static inline s_record_119 s_ga_10119_get_checked_take(S_Array *owned, int64_t index) {
	s_record_119 const *p = (s_record_119 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_119 value = s_record_119_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_119 s_ga_10119_get_borrow(const S_Array *array, int64_t index) {
	s_record_119 const *p = (s_record_119 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_119_copy(*p);
}
static inline void s_ga_10119_set_take(S_Array **owned, int64_t index, s_record_119 item) {
	s_array_set(owned, index, &item);
	s_record_119_drop(item);
}
static inline void s_ga_10120_copy(void *dst, const void *src) { *(s_record_120 *)dst = s_record_120_copy(*(s_record_120 const *)src); }
static inline void s_ga_10120_drop(void *value) { s_record_120_drop(*(s_record_120 *)value); }
static inline S_Array *s_ga_10120_make_take(size_t count, s_record_120 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_120), s_ga_10120_copy, s_ga_10120_drop);
	for (size_t i = 0; i < count; ++i) s_record_120_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10120_append_take(S_Array *owned, s_record_120 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_120_drop(item);
	return result;
}
static inline s_record_120 s_ga_10120_get_checked_take(S_Array *owned, int64_t index) {
	s_record_120 const *p = (s_record_120 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_120 value = s_record_120_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_120 s_ga_10120_get_borrow(const S_Array *array, int64_t index) {
	s_record_120 const *p = (s_record_120 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_120_copy(*p);
}
static inline void s_ga_10120_set_take(S_Array **owned, int64_t index, s_record_120 item) {
	s_array_set(owned, index, &item);
	s_record_120_drop(item);
}
static inline void s_ga_10121_copy(void *dst, const void *src) { *(s_record_121 *)dst = s_record_121_copy(*(s_record_121 const *)src); }
static inline void s_ga_10121_drop(void *value) { s_record_121_drop(*(s_record_121 *)value); }
static inline S_Array *s_ga_10121_make_take(size_t count, s_record_121 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_121), s_ga_10121_copy, s_ga_10121_drop);
	for (size_t i = 0; i < count; ++i) s_record_121_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10121_append_take(S_Array *owned, s_record_121 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_121_drop(item);
	return result;
}
static inline s_record_121 s_ga_10121_get_checked_take(S_Array *owned, int64_t index) {
	s_record_121 const *p = (s_record_121 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_121 value = s_record_121_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_record_121 s_ga_10121_get_borrow(const S_Array *array, int64_t index) {
	s_record_121 const *p = (s_record_121 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_121_copy(*p);
}
static inline void s_ga_10121_set_take(S_Array **owned, int64_t index, s_record_121 item) {
	s_array_set(owned, index, &item);
	s_record_121_drop(item);
}
static inline void s_ga_10007_copy(void *dst, const void *src) { *(S_Array_i64 * *)dst = s_array_i64_retain(*(S_Array_i64 * const *)src); }
static inline void s_ga_10007_drop(void *value) { s_array_i64_release(*(S_Array_i64 * *)value); }
static inline S_Array *s_ga_10007_make_take(size_t count, S_Array_i64 * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array_i64 *), s_ga_10007_copy, s_ga_10007_drop);
	for (size_t i = 0; i < count; ++i) s_array_i64_release(items[i]);
	return result;
}
static inline S_Array *s_ga_10007_append_take(S_Array *owned, S_Array_i64 * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_i64_release(item);
	return result;
}
static inline S_Array_i64 * s_ga_10007_get_checked_take(S_Array *owned, int64_t index) {
	S_Array_i64 * const *p = (S_Array_i64 * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array_i64 * value = s_array_i64_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array_i64 * s_ga_10007_get_borrow(const S_Array *array, int64_t index) {
	S_Array_i64 * const *p = (S_Array_i64 * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_i64_retain(*p);
}
static inline void s_ga_10007_set_take(S_Array **owned, int64_t index, S_Array_i64 * item) {
	s_array_set(owned, index, &item);
	s_array_i64_release(item);
}
static inline void s_ga_10008_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_10008_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_10008_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_10008_copy, s_ga_10008_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_10008_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_10008_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_10008_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_10008_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20100_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20100_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20100_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20100_copy, s_ga_20100_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20100_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20100_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20100_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20100_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20101_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20101_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20101_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20101_copy, s_ga_20101_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20101_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20101_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20101_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20101_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20102_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20102_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20102_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20102_copy, s_ga_20102_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20102_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20102_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20102_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20102_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20103_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20103_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20103_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20103_copy, s_ga_20103_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20103_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20103_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20103_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20103_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20104_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20104_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20104_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20104_copy, s_ga_20104_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20104_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20104_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20104_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20104_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20105_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20105_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20105_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20105_copy, s_ga_20105_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20105_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20105_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20105_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20105_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20106_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20106_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20106_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20106_copy, s_ga_20106_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20106_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20106_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20106_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20106_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20107_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20107_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20107_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20107_copy, s_ga_20107_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20107_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20107_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20107_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20107_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20108_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20108_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20108_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20108_copy, s_ga_20108_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20108_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20108_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20108_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20108_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20109_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20109_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20109_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20109_copy, s_ga_20109_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20109_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20109_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20109_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20109_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20110_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20110_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20110_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20110_copy, s_ga_20110_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20110_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20110_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20110_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20110_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20111_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20111_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20111_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20111_copy, s_ga_20111_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20111_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20111_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20111_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20111_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20112_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20112_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20112_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20112_copy, s_ga_20112_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20112_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20112_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20112_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20112_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20113_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20113_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20113_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20113_copy, s_ga_20113_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20113_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20113_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20113_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20113_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20114_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20114_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20114_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20114_copy, s_ga_20114_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20114_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20114_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20114_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20114_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20115_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20115_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20115_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20115_copy, s_ga_20115_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20115_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20115_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20115_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20115_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20116_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20116_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20116_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20116_copy, s_ga_20116_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20116_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20116_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20116_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20116_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20117_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20117_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20117_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20117_copy, s_ga_20117_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20117_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20117_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20117_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20117_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20118_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20118_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20118_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20118_copy, s_ga_20118_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20118_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20118_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20118_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20118_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20119_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20119_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20119_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20119_copy, s_ga_20119_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20119_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20119_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20119_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20119_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20120_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20120_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20120_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20120_copy, s_ga_20120_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20120_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20120_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20120_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20120_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20121_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20121_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20121_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20121_copy, s_ga_20121_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20121_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20121_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20121_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20121_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20007_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20007_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20007_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20007_copy, s_ga_20007_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20007_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20007_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20007_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20007_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20008_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20008_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20008_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20008_copy, s_ga_20008_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20008_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20008_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_20008_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20008_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30100_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30100_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30100_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30100_copy, s_ga_30100_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30100_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30100_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30100_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30100_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30101_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30101_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30101_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30101_copy, s_ga_30101_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30101_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30101_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30101_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30101_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30102_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30102_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30102_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30102_copy, s_ga_30102_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30102_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30102_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30102_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30102_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30103_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30103_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30103_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30103_copy, s_ga_30103_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30103_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30103_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30103_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30103_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30104_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30104_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30104_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30104_copy, s_ga_30104_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30104_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30104_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30104_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30104_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30105_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30105_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30105_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30105_copy, s_ga_30105_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30105_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30105_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30105_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30105_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30106_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30106_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30106_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30106_copy, s_ga_30106_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30106_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30106_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30106_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30106_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30107_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30107_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30107_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30107_copy, s_ga_30107_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30107_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30107_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30107_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30107_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30108_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30108_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30108_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30108_copy, s_ga_30108_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30108_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30108_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30108_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30108_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30109_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30109_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30109_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30109_copy, s_ga_30109_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30109_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30109_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30109_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30109_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30110_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30110_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30110_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30110_copy, s_ga_30110_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30110_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30110_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30110_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30110_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30111_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30111_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30111_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30111_copy, s_ga_30111_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30111_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30111_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30111_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30111_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30112_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30112_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30112_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30112_copy, s_ga_30112_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30112_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30112_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30112_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30112_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30113_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30113_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30113_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30113_copy, s_ga_30113_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30113_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30113_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30113_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30113_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30114_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30114_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30114_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30114_copy, s_ga_30114_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30114_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30114_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30114_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30114_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30115_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30115_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30115_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30115_copy, s_ga_30115_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30115_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30115_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30115_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30115_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30116_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30116_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30116_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30116_copy, s_ga_30116_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30116_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30116_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30116_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30116_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30117_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30117_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30117_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30117_copy, s_ga_30117_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30117_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30117_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30117_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30117_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30118_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30118_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30118_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30118_copy, s_ga_30118_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30118_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30118_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30118_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30118_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30119_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30119_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30119_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30119_copy, s_ga_30119_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30119_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30119_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30119_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30119_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30120_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30120_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30120_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30120_copy, s_ga_30120_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30120_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30120_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30120_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30120_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30121_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30121_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30121_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30121_copy, s_ga_30121_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30121_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30121_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline S_Array * s_ga_30121_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30121_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline int64_t s_generic_array_length_take(S_Array *owned) { int64_t n = (int64_t)s_array_length(owned); s_array_release(owned); return n; }
static inline int64_t s_dict_length_take(S_Dict *owned) { int64_t n = (int64_t)s_dict_length(owned); s_dict_release(owned); return n; }

s_record_100 s_Token(int64_t, int64_t, int64_t, int64_t, int64_t);
S_String * s_token_text(S_String *, s_record_100);
S_String * s_token_kind_name(int64_t);
int64_t s_peek(S_String *, int64_t);
bool s_is_alpha(int64_t);
bool s_is_digit(int64_t);
bool s_is_ident(int64_t);
int64_t s_scan_identifier(S_String *, int64_t);
int64_t s_scan_number(S_String *, int64_t);
bool s_is_float(S_String *, int64_t, int64_t);
int64_t s_scan_string(S_String *, int64_t);
int64_t s_scan_operator(S_String *, int64_t);
S_Array * s_tokenize(S_String *);
S_String * s_node_kind_name(int64_t);
s_record_101 s_Node(int64_t, int64_t, int64_t, int64_t);
s_record_102 s_ParseState(S_String *, S_Array *);
s_record_103 s_ParseResult(s_record_102, int64_t);
s_record_100 s_current(s_record_102);
S_String * s_word(s_record_102);
bool s_is_token(s_record_102, S_String *);
bool s_kind_is(s_record_102, int64_t);
s_record_102 s_fail(s_record_102, S_String *);
s_record_102 s_expect(s_record_102, S_String *);
s_record_102 s_expect_kind(s_record_102, int64_t, S_String *);
bool s_is_name(s_record_102);
s_record_103 s_append_node(s_record_102, int64_t, int64_t, S_Array_i64 *);
s_record_103 s_leaf(s_record_102, int64_t, int64_t);
s_record_103 s_unary_node(s_record_102, int64_t, int64_t, int64_t);
s_record_103 s_binary_node(s_record_102, int64_t, int64_t, int64_t, int64_t);
int64_t s_precedence(S_String *);
s_record_103 s_parse_type(s_record_102);
s_record_103 s_parse_param(s_record_102);
s_record_103 s_parse_expr(s_record_102, int64_t);
s_record_103 s_parse_block(s_record_102);
s_record_103 s_rec_body(s_record_102, S_Array_i64 *);
s_record_103 s_parse_statement(s_record_102);
s_record_103 s_parse_module(S_String *);
s_record_104 s_RecordField(S_String *, int64_t);
s_record_105 s_TypeDescriptor(int64_t, int64_t, int64_t, S_Array *);
s_record_106 s_TypeTable(S_Array *);
s_record_107 s_InternedType(s_record_106, int64_t);
s_record_106 s_new_type_table();
s_record_107 s_intern_type(s_record_106, int64_t, int64_t, int64_t, S_Array *);
s_record_107 s_intern_array_type(s_record_106, int64_t);
s_record_107 s_intern_dict_type(s_record_106, int64_t, int64_t);
s_record_107 s_intern_optional_type(s_record_106, int64_t);
S_Array * s_ordered_fields(S_Array *);
s_record_107 s_intern_record_type(s_record_106, S_Array *);
s_record_108 s_HirNode(int64_t, int64_t, int64_t, int64_t);
s_record_109 s_HirResolution(int64_t, int64_t, S_String *);
s_record_110 s_SymbolInfo(S_String *, int64_t, int64_t, bool, int64_t);
s_record_109 s_resolve_hir_name(S_Array *, S_String *);
bool s_hir_types_equal(int64_t, int64_t);
s_record_109 s_resolve_hir_field(s_record_106, int64_t, S_String *);
s_record_109 s_resolve_hir_binary(s_record_106, int64_t, int64_t, S_String *);
int64_t s_resolve_hir_literal(int64_t);
s_record_109 s_resolve_hir_unary(s_record_106, int64_t, S_String *);
s_record_111 s_HirCallCheck(s_record_106, int64_t, S_String *);
S_String * s_hir_type_name(s_record_106, int64_t);
s_record_111 s_hir_expected(s_record_106, int64_t, int64_t);
s_record_111 s_hir_call_arguments(s_record_106, S_Array_i64 *, S_Array_i64 *, int64_t, S_String *);
bool s_hir_is_builtin(S_String *);
s_record_111 s_resolve_hir_builtin_call(s_record_106, S_String *, S_Array_i64 *);
bool s_hir_is_string_method(S_String *);
s_record_111 s_resolve_hir_string_call(s_record_106, int64_t, S_String *, S_Array_i64 *);
s_record_111 s_resolve_hir_function_call(s_record_106, S_String *, S_Array_i64 *, S_Array_i64 *, int64_t);
s_record_111 s_resolve_hir_collection_call(s_record_106, int64_t, S_String *, S_Array_i64 *);
bool s_is_optional(int64_t);
int64_t s_optional_value(int64_t);
int64_t s_optional_of(int64_t);
S_String * s_optional_helper(int64_t);
bool s_is_dict(int64_t);
int64_t s_dict_of(int64_t, int64_t);
int64_t s_dict_value(int64_t);
int64_t s_dict_key(int64_t);
S_String * s_dict_helper(int64_t);
int64_t s_array_element(int64_t);
int64_t s_array_of(int64_t);
bool s_managed(int64_t);
bool s_generic_array(int64_t);
S_String * s_array_helper(int64_t);
s_record_104 s_ParameterInfo(S_String *, int64_t);
s_record_112 s_FunctionInfo(S_String *, int64_t, S_Array *, int64_t, bool, S_String *);
s_record_113 s_InstanceInfo(S_String *, S_Array_i64 *, S_Array_i64 *, int64_t, S_String *, S_String *, int64_t);
s_record_114 s_ConstantInfo(S_String *, S_String *);
s_record_115 s_RecordInfo(S_Array *, int64_t);
s_record_116 s_EmitState(s_record_102);
s_record_117 s_ExprCode(s_record_116, S_String *, int64_t);
s_record_118 s_CompileResult(S_String *, S_String *, int64_t);
s_record_101 s_ast_node(s_record_102, int64_t);
int64_t s_ast_child(s_record_102, int64_t, int64_t);
S_String * s_ast_text(s_record_102, int64_t);
int64_t s_ast_line(s_record_102, int64_t);
s_record_116 s_emit_error(s_record_116, int64_t, S_String *);
S_String * s_type_name(int64_t);
S_String * s_c_type(int64_t);
int64_t s_ast_type(s_record_102, int64_t);
int64_t s_resolve_type(s_record_116, int64_t);
int64_t s_array_constructor_kind(s_record_116, int64_t);
int64_t s_dict_constructor_kind(s_record_116, int64_t);
s_record_116 s_register_dict(s_record_116, int64_t);
s_record_116 s_line(s_record_116, S_String *);
int64_t s_find_symbol(s_record_116, S_String *);
int64_t s_find_function(s_record_116, S_String *);
bool s_is_generic(s_record_112);
int64_t s_find_instance(s_record_116, S_String *, S_Array_i64 *);
S_String * s_find_constant(s_record_116, S_String *);
int64_t s_record_field_index(s_record_116, int64_t, S_String *);
s_record_107 s_canonical_kind(s_record_106, S_Array *, int64_t);
s_record_119 s_InternedSignature(s_record_106, S_Array_i64 *);
s_record_119 s_canonical_signature(s_record_116, S_Array_i64 *);
s_record_117 s_intern_record(s_record_116, S_Array *);
s_record_120 s_CTypeABI(int64_t, S_String *, S_String *, S_String *);
s_record_120 s_c_abi(s_record_116, int64_t);
S_String * s_retain_function(int64_t);
S_String * s_release_function(int64_t);
s_record_121 s_CFieldABI(S_String *, int64_t, s_record_120);
S_Array * s_record_c_fields(s_record_116, int64_t);
S_String * s_record_copy(int64_t, S_String *);
S_String * s_record_drop(int64_t, S_String *);
bool s_plain_record(s_record_116, int64_t);
S_String * s_record_declarations(s_record_116);
S_String * s_optional_declarations(s_record_116);
S_String * s_generic_array_declarations(s_record_116);
S_String * s_dict_declarations(s_record_116);
s_record_116 s_append_symbol(s_record_116, int64_t, S_String *, int64_t, bool);
s_record_116 s_check_type(s_record_116, int64_t, int64_t, int64_t);
bool s_has_effect(s_record_116, int64_t, S_Array *);
bool s_uses_optional_fallback(s_record_116, int64_t);
bool s_ambiguous_order(s_record_116, int64_t, int64_t);
s_record_117 s_specialize_call(s_record_116, int64_t, s_record_112, S_Array_i64 *, S_Array *);
s_record_116 s_check_hir_call(s_record_116, int64_t, s_record_111);
s_record_119 s_call_type_ids(s_record_116, S_Array_i64 *);
s_record_117 s_emit_expr(s_record_116, int64_t);
bool s_contains_recur(s_record_102, int64_t);
s_record_116 s_emit_block(s_record_116, int64_t);
s_record_116 s_emit_nested_block(s_record_116, int64_t);
s_record_116 s_emit_statement(s_record_116, int64_t);
s_record_116 s_collect_function(s_record_116, int64_t);
s_record_117 s_evaluate_constant(s_record_116, int64_t);
s_record_116 s_collect_constant(s_record_116, int64_t);
s_record_116 s_emit_function(s_record_116, s_record_112);
s_record_118 s_compile_c(S_String *);
int64_t s_main();

s_record_100 s_Token(int64_t v_kind, int64_t v_start, int64_t v_end, int64_t v_line, int64_t v_column)
{
	s_record_100 s_constructor_result = {0};
	s_constructor_result.kind = v_kind;
	s_constructor_result.start = v_start;
	s_constructor_result.end = v_end;
	s_constructor_result.line = v_line;
	s_constructor_result.column = v_column;
	return s_constructor_result;
}

S_String * s_token_text(S_String * v_source, s_record_100 v_token)
{
	S_String * s_return_0 = s_string_slice_take(s_string_retain(v_source), s_record_100_get_start_take(s_record_100_copy(v_token)), s_record_100_get_end_take(s_record_100_copy(v_token)));
	s_record_100_drop(v_token);
	s_string_release(v_source);
	return s_return_0;
}

S_String * s_token_kind_name(int64_t v_kind)
{
	if ((v_kind == 1))
	{
		return s_string_from_bytes("NAME", sizeof("NAME") - 1);
	}
	if ((v_kind == 2))
	{
		return s_string_from_bytes("INT", sizeof("INT") - 1);
	}
	if ((v_kind == 3))
	{
		return s_string_from_bytes("FLOAT", sizeof("FLOAT") - 1);
	}
	if ((v_kind == 4))
	{
		return s_string_from_bytes("STRING", sizeof("STRING") - 1);
	}
	if ((v_kind == 5))
	{
		return s_string_from_bytes("OP", sizeof("OP") - 1);
	}
	if ((v_kind == 6))
	{
		return s_string_from_bytes("NEWLINE", sizeof("NEWLINE") - 1);
	}
	if ((v_kind == 7))
	{
		return s_string_from_bytes("INDENT", sizeof("INDENT") - 1);
	}
	if ((v_kind == 8))
	{
		return s_string_from_bytes("DEDENT", sizeof("DEDENT") - 1);
	}
	if ((v_kind == 9))
	{
		return s_string_from_bytes("EOF", sizeof("EOF") - 1);
	}
	if ((v_kind == (-1)))
	{
		return s_string_from_bytes("INVALID_CHAR", sizeof("INVALID_CHAR") - 1);
	}
	if ((v_kind == (-2)))
	{
		return s_string_from_bytes("INVALID_INDENT", sizeof("INVALID_INDENT") - 1);
	}
	if ((v_kind == (-3)))
	{
		return s_string_from_bytes("UNCLOSED_STRING", sizeof("UNCLOSED_STRING") - 1);
	}
	if ((v_kind == (-4)))
	{
		return s_string_from_bytes("INVALID_BRACKET", sizeof("INVALID_BRACKET") - 1);
	}
	if ((v_kind == (-5)))
	{
		return s_string_from_bytes("UNCLOSED_BRACKET", sizeof("UNCLOSED_BRACKET") - 1);
	}
	return s_string_from_bytes("UNKNOWN", sizeof("UNKNOWN") - 1);
}

int64_t s_peek(S_String * v_source, int64_t v_i)
{
	S_Optional_i64 s_option_1 = s_string_byte_at_take(s_string_retain(v_source), v_i);
	int64_t s_fallback_1;
	if (s_option_1.has_value) {
		s_fallback_1 = s_option_1.value;
	} else {
		s_fallback_1 = (-1);
	}
	int64_t s_return_2 = s_fallback_1;
	s_string_release(v_source);
	return s_return_2;
}

bool s_is_alpha(int64_t v_c)
{
	return ((((v_c >= 65) && (v_c <= 90)) || ((v_c >= 97) && (v_c <= 122))) || (v_c == 95));
}

bool s_is_digit(int64_t v_c)
{
	return ((v_c >= 48) && (v_c <= 57));
}

bool s_is_ident(int64_t v_c)
{
	return (s_is_alpha(v_c) || s_is_digit(v_c));
}

int64_t s_scan_identifier(S_String * v_source, int64_t v_start)
{
	int64_t v_i = (v_start + 1);
	while (true) {
		if (!(s_is_ident(s_peek(s_string_retain(v_source), v_i)))) break;
		v_i += 1;
	}
	int64_t s_return_3 = v_i;
	s_string_release(v_source);
	return s_return_3;
}

int64_t s_scan_number(S_String * v_source, int64_t v_start)
{
	int64_t v_i = v_start;
	while (true) {
		if (!(s_is_digit(s_peek(s_string_retain(v_source), v_i)))) break;
		v_i += 1;
	}
	if (((s_peek(s_string_retain(v_source), v_i) == 46) && s_is_digit(s_peek(s_string_retain(v_source), (v_i + 1)))))
	{
		v_i += 1;
		while (true) {
			if (!(s_is_digit(s_peek(s_string_retain(v_source), v_i)))) break;
			v_i += 1;
		}
	}
	const int64_t v_c = s_peek(s_string_retain(v_source), v_i);
	if (((v_c == 101) || (v_c == 69)))
	{
		int64_t v_j = (v_i + 1);
		if (((s_peek(s_string_retain(v_source), v_j) == 43) || (s_peek(s_string_retain(v_source), v_j) == 45)))
		{
			v_j += 1;
		}
		if (s_is_digit(s_peek(s_string_retain(v_source), v_j)))
		{
			v_i = (v_j + 1);
			while (true) {
				if (!(s_is_digit(s_peek(s_string_retain(v_source), v_i)))) break;
				v_i += 1;
			}
		}
	}
	int64_t s_return_4 = v_i;
	s_string_release(v_source);
	return s_return_4;
}

bool s_is_float(S_String * v_source, int64_t v_start, int64_t v_end)
{
	for (int64_t v_i = v_start, s_bound_5 = v_end; v_i < s_bound_5; ++v_i)
	{
		const int64_t v_c = s_peek(s_string_retain(v_source), v_i);
		if ((((v_c == 46) || (v_c == 101)) || (v_c == 69)))
		{
			bool s_return_6 = true;
			s_string_release(v_source);
			return s_return_6;
		}
	}
	bool s_return_7 = false;
	s_string_release(v_source);
	return s_return_7;
}

int64_t s_scan_string(S_String * v_source, int64_t v_start)
{
	const int64_t v_quote = s_peek(s_string_retain(v_source), v_start);
	int64_t v_i = (v_start + 1);
	while (true) {
		if (!((v_i < s_string_length_take(s_string_retain(v_source))))) break;
		const int64_t v_c = s_peek(s_string_retain(v_source), v_i);
		if (((v_c == 10) || (v_c == 13)))
		{
			int64_t s_return_8 = (-1);
			s_string_release(v_source);
			return s_return_8;
		}
		if ((v_c == 92))
		{
			const int64_t v_next = s_peek(s_string_retain(v_source), (v_i + 1));
			if ((((v_next == (-1)) || (v_next == 10)) || (v_next == 13)))
			{
				int64_t s_return_9 = (-1);
				s_string_release(v_source);
				return s_return_9;
			}
			v_i += 2;
			continue;
		}
		if ((v_c == v_quote))
		{
			int64_t s_return_10 = (v_i + 1);
			s_string_release(v_source);
			return s_return_10;
		}
		v_i += 1;
	}
	int64_t s_return_11 = (-1);
	s_string_release(v_source);
	return s_return_11;
}

int64_t s_scan_operator(S_String * v_source, int64_t v_start)
{
	const int64_t v_c = s_peek(s_string_retain(v_source), v_start);
	const int64_t v_n = s_peek(s_string_retain(v_source), (v_start + 1));
	const int64_t v_third = s_peek(s_string_retain(v_source), (v_start + 2));
	if (((((((v_c == 42) && (v_n == 42)) || ((v_c == 47) && (v_n == 47))) || ((v_c == 60) && (v_n == 60))) || ((v_c == 62) && (v_n == 62))) && (v_third == 61)))
	{
		int64_t s_return_12 = (v_start + 3);
		s_string_release(v_source);
		return s_return_12;
	}
	if ((((v_c == 46) && (v_n == 46)) || ((v_c == 45) && (v_n == 62))))
	{
		int64_t s_return_13 = (v_start + 2);
		s_string_release(v_source);
		return s_return_13;
	}
	if (((v_n == 61) && ((((((((((((v_c == 61) || (v_c == 33)) || (v_c == 60)) || (v_c == 62)) || (v_c == 43)) || (v_c == 45)) || (v_c == 42)) || (v_c == 47)) || (v_c == 37)) || (v_c == 38)) || (v_c == 124)) || (v_c == 94))))
	{
		int64_t s_return_14 = (v_start + 2);
		s_string_release(v_source);
		return s_return_14;
	}
	if (((v_c == v_n) && ((((((v_c == 42) || (v_c == 38)) || (v_c == 124)) || (v_c == 60)) || (v_c == 62)) || (v_c == 47))))
	{
		int64_t s_return_15 = (v_start + 2);
		s_string_release(v_source);
		return s_return_15;
	}
	if (((((((((((((((v_c >= 40) && (v_c <= 47)) || ((v_c >= 60) && (v_c <= 63))) || (v_c == 33)) || (v_c == 37)) || (v_c == 38)) || (v_c == 58)) || (v_c == 91)) || (v_c == 93)) || (v_c == 94)) || (v_c == 123)) || (v_c == 124)) || (v_c == 125)) || (v_c == 126)))
	{
		int64_t s_return_16 = (v_start + 1);
		s_string_release(v_source);
		return s_return_16;
	}
	int64_t s_return_17 = v_start;
	s_string_release(v_source);
	return s_return_17;
}

S_Array * s_tokenize(S_String * v_source)
{
	S_Array * v_tokens = s_ga_10100_make_take(0, NULL);
	S_Array_i64 * v_brackets = s_array_i64_make(0, NULL);
	int64_t v_depth = 0;
	int64_t v_level = 0;
	int64_t v_pos = 0;
	int64_t v_line = 1;
	int64_t v_column = 1;
	bool v_line_start = true;
	bool v_line_has_code = false;
	const int64_t v_size = s_string_length_take(s_string_retain(v_source));
	while (true) {
		if (!((v_pos < v_size))) break;
		if (v_line_start)
		{
			int64_t v_indent = 0;
			bool v_spaces = false;
			while (true) {
				if (!(((s_peek(s_string_retain(v_source), v_pos) == 9) || (s_peek(s_string_retain(v_source), v_pos) == 32)))) break;
				if ((s_peek(s_string_retain(v_source), v_pos) == 9))
				{
					v_indent += 1;
				}
				else
				{
					v_spaces = true;
				}
				v_pos += 1;
				v_column += 1;
			}
			const int64_t v_first = s_peek(s_string_retain(v_source), v_pos);
			if (((v_depth == 0) && ((((v_first != 10) && (v_first != 13)) && (v_first != 35)) && (v_first != (-1)))))
			{
				if ((v_spaces || (v_indent > (v_level + 1))))
				{
					S_Array * s_return_18 = s_ga_10100_append_take(s_array_retain(v_tokens), s_Token((-2), v_pos, v_pos, v_line, v_column));
					s_array_i64_release(v_brackets);
					s_array_release(v_tokens);
					s_string_release(v_source);
					return s_return_18;
				}
				if ((v_indent > v_level))
				{
					s_record_100 s_item_19 = s_Token(7, v_pos, v_pos, v_line, 1);
					v_tokens = s_ga_10100_append_take(v_tokens, s_item_19);
					v_level += 1;
				}
				while (true) {
					if (!((v_level > v_indent))) break;
					s_record_100 s_item_20 = s_Token(8, v_pos, v_pos, v_line, 1);
					v_tokens = s_ga_10100_append_take(v_tokens, s_item_20);
					v_level -= 1;
				}
			}
			v_line_start = false;
		}
		const int64_t v_c = s_peek(s_string_retain(v_source), v_pos);
		if (((v_c == 10) || (v_c == 13)))
		{
			const int64_t v_start = v_pos;
			if (((v_c == 13) && (s_peek(s_string_retain(v_source), (v_pos + 1)) == 10)))
			{
				v_pos += 2;
			}
			else
			{
				v_pos += 1;
			}
			if (((v_depth == 0) && v_line_has_code))
			{
				s_record_100 s_item_21 = s_Token(6, v_start, v_pos, v_line, v_column);
				v_tokens = s_ga_10100_append_take(v_tokens, s_item_21);
			}
			v_line += 1;
			v_column = 1;
			v_line_start = true;
			v_line_has_code = false;
			continue;
		}
		if (((v_c == 32) || (v_c == 9)))
		{
			v_pos += 1;
			v_column += 1;
			continue;
		}
		if ((v_c == 35))
		{
			while (true) {
				if (!((((v_pos < v_size) && (s_peek(s_string_retain(v_source), v_pos) != 10)) && (s_peek(s_string_retain(v_source), v_pos) != 13)))) break;
				v_pos += 1;
				v_column += 1;
			}
			continue;
		}
		const int64_t v_start = v_pos;
		const int64_t v_start_column = v_column;
		int64_t v_kind = 5;
		int64_t v_end = v_pos;
		if (s_is_alpha(v_c))
		{
			v_kind = 1;
			v_end = s_scan_identifier(s_string_retain(v_source), v_pos);
		}
		else if ((s_is_digit(v_c) || ((v_c == 46) && s_is_digit(s_peek(s_string_retain(v_source), (v_pos + 1))))))
		{
			v_kind = 2;
			v_end = s_scan_number(s_string_retain(v_source), v_pos);
			if (s_is_float(s_string_retain(v_source), v_start, v_end))
			{
				v_kind = 3;
			}
		}
		else if (((v_c == 34) || (v_c == 39)))
		{
			v_kind = 4;
			v_end = s_scan_string(s_string_retain(v_source), v_pos);
			if ((v_end < 0))
			{
				S_Array * s_return_22 = s_ga_10100_append_take(s_array_retain(v_tokens), s_Token((-3), v_pos, (v_pos + 1), v_line, v_column));
				s_array_i64_release(v_brackets);
				s_array_release(v_tokens);
				s_string_release(v_source);
				return s_return_22;
			}
		}
		else
		{
			v_end = s_scan_operator(s_string_retain(v_source), v_pos);
			if ((v_end == v_pos))
			{
				S_Array * s_return_23 = s_ga_10100_append_take(s_array_retain(v_tokens), s_Token((-1), v_pos, (v_pos + 1), v_line, v_column));
				s_array_i64_release(v_brackets);
				s_array_release(v_tokens);
				s_string_release(v_source);
				return s_return_23;
			}
			if ((((v_c == 40) || (v_c == 91)) || (v_c == 123)))
			{
				if ((v_depth == s_array_i64_length_take(s_array_i64_retain(v_brackets))))
				{
					int64_t s_item_24 = v_c;
					v_brackets = s_array_i64_append_take(v_brackets, s_item_24);
				}
				else
				{
					s_array_i64_set(&v_brackets, v_depth, v_c);
				}
				v_depth += 1;
			}
			else if ((((v_c == 41) || (v_c == 93)) || (v_c == 125)))
			{
				if ((v_depth == 0))
				{
					S_Array * s_return_25 = s_ga_10100_append_take(s_array_retain(v_tokens), s_Token((-4), v_pos, v_end, v_line, v_column));
					s_array_i64_release(v_brackets);
					s_array_release(v_tokens);
					s_string_release(v_source);
					return s_return_25;
				}
				const int64_t v_opening = s_array_i64_get_checked_take(s_array_i64_retain(v_brackets), (v_depth - 1));
				if (((((v_c == 41) && (v_opening != 40)) || ((v_c == 93) && (v_opening != 91))) || ((v_c == 125) && (v_opening != 123))))
				{
					S_Array * s_return_26 = s_ga_10100_append_take(s_array_retain(v_tokens), s_Token((-4), v_pos, v_end, v_line, v_column));
					s_array_i64_release(v_brackets);
					s_array_release(v_tokens);
					s_string_release(v_source);
					return s_return_26;
				}
				v_depth -= 1;
			}
		}
		s_record_100 s_item_27 = s_Token(v_kind, v_start, v_end, v_line, v_start_column);
		v_tokens = s_ga_10100_append_take(v_tokens, s_item_27);
		v_column += (v_end - v_pos);
		v_pos = v_end;
		v_line_has_code = true;
	}
	if ((v_depth != 0))
	{
		S_Array * s_return_28 = s_ga_10100_append_take(s_array_retain(v_tokens), s_Token((-5), v_size, v_size, v_line, v_column));
		s_array_i64_release(v_brackets);
		s_array_release(v_tokens);
		s_string_release(v_source);
		return s_return_28;
	}
	if (v_line_has_code)
	{
		s_record_100 s_item_29 = s_Token(6, v_size, v_size, v_line, v_column);
		v_tokens = s_ga_10100_append_take(v_tokens, s_item_29);
	}
	while (true) {
		if (!((v_level > 0))) break;
		s_record_100 s_item_30 = s_Token(8, v_size, v_size, v_line, 1);
		v_tokens = s_ga_10100_append_take(v_tokens, s_item_30);
		v_level -= 1;
	}
	S_Array * s_return_31 = s_ga_10100_append_take(s_array_retain(v_tokens), s_Token(9, v_size, v_size, v_line, v_column));
	s_array_i64_release(v_brackets);
	s_array_release(v_tokens);
	s_string_release(v_source);
	return s_return_31;
	s_array_i64_release(v_brackets);
	s_array_release(v_tokens);
}

S_String * s_node_kind_name(int64_t v_kind)
{
	if ((v_kind == 1))
	{
		return s_string_from_bytes("arm", sizeof("arm") - 1);
	}
	if ((v_kind == 2))
	{
		return s_string_from_bytes("array", sizeof("array") - 1);
	}
	if ((v_kind == 3))
	{
		return s_string_from_bytes("assign", sizeof("assign") - 1);
	}
	if ((v_kind == 4))
	{
		return s_string_from_bytes("attr", sizeof("attr") - 1);
	}
	if ((v_kind == 5))
	{
		return s_string_from_bytes("binary", sizeof("binary") - 1);
	}
	if ((v_kind == 6))
	{
		return s_string_from_bytes("binding", sizeof("binding") - 1);
	}
	if ((v_kind == 7))
	{
		return s_string_from_bytes("block", sizeof("block") - 1);
	}
	if ((v_kind == 8))
	{
		return s_string_from_bytes("bool", sizeof("bool") - 1);
	}
	if ((v_kind == 9))
	{
		return s_string_from_bytes("break", sizeof("break") - 1);
	}
	if ((v_kind == 10))
	{
		return s_string_from_bytes("call", sizeof("call") - 1);
	}
	if ((v_kind == 11))
	{
		return s_string_from_bytes("case", sizeof("case") - 1);
	}
	if ((v_kind == 12))
	{
		return s_string_from_bytes("checked_index", sizeof("checked_index") - 1);
	}
	if ((v_kind == 13))
	{
		return s_string_from_bytes("continue", sizeof("continue") - 1);
	}
	if ((v_kind == 14))
	{
		return s_string_from_bytes("def", sizeof("def") - 1);
	}
	if ((v_kind == 15))
	{
		return s_string_from_bytes("delete", sizeof("delete") - 1);
	}
	if ((v_kind == 16))
	{
		return s_string_from_bytes("else", sizeof("else") - 1);
	}
	if ((v_kind == 17))
	{
		return s_string_from_bytes("else_return", sizeof("else_return") - 1);
	}
	if ((v_kind == 18))
	{
		return s_string_from_bytes("expr", sizeof("expr") - 1);
	}
	if ((v_kind == 19))
	{
		return s_string_from_bytes("field", sizeof("field") - 1);
	}
	if ((v_kind == 20))
	{
		return s_string_from_bytes("float", sizeof("float") - 1);
	}
	if ((v_kind == 21))
	{
		return s_string_from_bytes("for", sizeof("for") - 1);
	}
	if ((v_kind == 22))
	{
		return s_string_from_bytes("if", sizeof("if") - 1);
	}
	if ((v_kind == 23))
	{
		return s_string_from_bytes("index", sizeof("index") - 1);
	}
	if ((v_kind == 24))
	{
		return s_string_from_bytes("integer", sizeof("integer") - 1);
	}
	if ((v_kind == 25))
	{
		return s_string_from_bytes("lambda", sizeof("lambda") - 1);
	}
	if ((v_kind == 26))
	{
		return s_string_from_bytes("let", sizeof("let") - 1);
	}
	if ((v_kind == 27))
	{
		return s_string_from_bytes("match", sizeof("match") - 1);
	}
	if ((v_kind == 28))
	{
		return s_string_from_bytes("module", sizeof("module") - 1);
	}
	if ((v_kind == 29))
	{
		return s_string_from_bytes("name", sizeof("name") - 1);
	}
	if ((v_kind == 30))
	{
		return s_string_from_bytes("optional_index", sizeof("optional_index") - 1);
	}
	if ((v_kind == 31))
	{
		return s_string_from_bytes("param", sizeof("param") - 1);
	}
	if ((v_kind == 32))
	{
		return s_string_from_bytes("pub", sizeof("pub") - 1);
	}
	if ((v_kind == 33))
	{
		return s_string_from_bytes("record", sizeof("record") - 1);
	}
	if ((v_kind == 42))
	{
		return s_string_from_bytes("rec", sizeof("rec") - 1);
	}
	if ((v_kind == 34))
	{
		return s_string_from_bytes("recur", sizeof("recur") - 1);
	}
	if ((v_kind == 35))
	{
		return s_string_from_bytes("return", sizeof("return") - 1);
	}
	if ((v_kind == 36))
	{
		return s_string_from_bytes("spread", sizeof("spread") - 1);
	}
	if ((v_kind == 37))
	{
		return s_string_from_bytes("string", sizeof("string") - 1);
	}
	if ((v_kind == 38))
	{
		return s_string_from_bytes("type", sizeof("type") - 1);
	}
	if ((v_kind == 39))
	{
		return s_string_from_bytes("unary", sizeof("unary") - 1);
	}
	if ((v_kind == 40))
	{
		return s_string_from_bytes("var", sizeof("var") - 1);
	}
	if ((v_kind == 41))
	{
		return s_string_from_bytes("while", sizeof("while") - 1);
	}
	return s_string_from_bytes("unknown", sizeof("unknown") - 1);
}

s_record_101 s_Node(int64_t v_kind, int64_t v_token, int64_t v_first, int64_t v_count)
{
	s_record_101 s_constructor_result = {0};
	s_constructor_result.kind = v_kind;
	s_constructor_result.token = v_token;
	s_constructor_result.first = v_first;
	s_constructor_result.count = v_count;
	return s_constructor_result;
}

s_record_102 s_ParseState(S_String * v_source, S_Array * v_tokens)
{
	s_record_102 s_constructor_result = {0};
	s_constructor_result.source = v_source;
	s_constructor_result.tokens = v_tokens;
	s_constructor_result.nodes = s_ga_10101_make_take(0, NULL);
	s_constructor_result.edges = s_array_i64_make(0, NULL);
	s_constructor_result.index = 0;
	s_constructor_result.error = s_string_from_bytes("", sizeof("") - 1);
	s_constructor_result.error_line = 0;
	return s_constructor_result;
}

s_record_103 s_ParseResult(s_record_102 v_state, int64_t v_node)
{
	s_record_103 s_constructor_result = {0};
	s_constructor_result.state = v_state;
	s_constructor_result.node = v_node;
	return s_constructor_result;
}

s_record_100 s_current(s_record_102 v_p)
{
	s_record_100 s_return_32 = s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_p)), s_record_102_get_index_take(s_record_102_copy(v_p)));
	s_record_102_drop(v_p);
	return s_return_32;
}

S_String * s_word(s_record_102 v_p)
{
	S_String * s_return_33 = s_token_text(s_record_102_get_source_take(s_record_102_copy(v_p)), s_current(s_record_102_copy(v_p)));
	s_record_102_drop(v_p);
	return s_return_33;
}

bool s_is_token(s_record_102 v_p, S_String * v_value)
{
	bool s_return_34 = (s_string_compare_take(s_word(s_record_102_copy(v_p)), s_string_retain(v_value)) == 0);
	s_string_release(v_value);
	s_record_102_drop(v_p);
	return s_return_34;
}

bool s_kind_is(s_record_102 v_p, int64_t v_kind)
{
	bool s_return_35 = (s_record_100_get_kind_take(s_current(s_record_102_copy(v_p))) == v_kind);
	s_record_102_drop(v_p);
	return s_return_35;
}

s_record_102 s_fail(s_record_102 v_p, S_String * v_message)
{
	s_record_102 v_next = s_record_102_copy(v_p);
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		S_String * s_field_36 = s_string_retain(v_message);
		s_string_release(v_next.error);
		v_next.error = s_field_36;
		v_next.error_line = s_record_100_get_line_take(s_current(s_record_102_copy(v_p)));
	}
	s_record_102 s_return_37 = s_record_102_copy(v_next);
	s_record_102_drop(v_next);
	s_string_release(v_message);
	s_record_102_drop(v_p);
	return s_return_37;
	s_record_102_drop(v_next);
}

s_record_102 s_expect(s_record_102 v_p, S_String * v_value)
{
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_102 s_return_38 = s_record_102_copy(v_p);
		s_string_release(v_value);
		s_record_102_drop(v_p);
		return s_return_38;
	}
	if ((!s_is_token(s_record_102_copy(v_p), s_string_retain(v_value))))
	{
		s_record_102 s_return_39 = s_fail(s_record_102_copy(v_p), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected '", sizeof("expected '") - 1), s_string_retain(v_value)), s_string_from_bytes("', found '", sizeof("', found '") - 1)), s_word(s_record_102_copy(v_p))), s_string_from_bytes("'", sizeof("'") - 1)));
		s_string_release(v_value);
		s_record_102_drop(v_p);
		return s_return_39;
	}
	s_record_102 v_next = s_record_102_copy(v_p);
	v_next.index += 1;
	s_record_102 s_return_40 = s_record_102_copy(v_next);
	s_record_102_drop(v_next);
	s_string_release(v_value);
	s_record_102_drop(v_p);
	return s_return_40;
	s_record_102_drop(v_next);
}

s_record_102 s_expect_kind(s_record_102 v_p, int64_t v_kind, S_String * v_description)
{
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_102 s_return_41 = s_record_102_copy(v_p);
		s_string_release(v_description);
		s_record_102_drop(v_p);
		return s_return_41;
	}
	if ((!s_kind_is(s_record_102_copy(v_p), v_kind)))
	{
		s_record_102 s_return_42 = s_fail(s_record_102_copy(v_p), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_string_retain(v_description)), s_string_from_bytes(", found '", sizeof(", found '") - 1)), s_word(s_record_102_copy(v_p))), s_string_from_bytes("'", sizeof("'") - 1)));
		s_string_release(v_description);
		s_record_102_drop(v_p);
		return s_return_42;
	}
	s_record_102 v_next = s_record_102_copy(v_p);
	v_next.index += 1;
	s_record_102 s_return_43 = s_record_102_copy(v_next);
	s_record_102_drop(v_next);
	s_string_release(v_description);
	s_record_102_drop(v_p);
	return s_return_43;
	s_record_102_drop(v_next);
}

bool s_is_name(s_record_102 v_p)
{
	bool s_return_44 = s_kind_is(s_record_102_copy(v_p), 1);
	s_record_102_drop(v_p);
	return s_return_44;
}

s_record_103 s_append_node(s_record_102 v_p, int64_t v_kind, int64_t v_token, S_Array_i64 * v_children)
{
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_103 s_return_45 = s_ParseResult(s_record_102_copy(v_p), (-1));
		s_array_i64_release(v_children);
		s_record_102_drop(v_p);
		return s_return_45;
	}
	s_record_102 v_next = s_record_102_copy(v_p);
	const int64_t v_first = s_array_i64_length_take(s_record_102_get_edges_take(s_record_102_copy(v_next)));
	{
		S_Array_i64 * v_s_iter_46 = s_array_i64_retain(v_children);
		for (int64_t s_index_46 = 0; s_index_46 < (int64_t)v_s_iter_46->length; ++s_index_46)
		{
			int64_t v_child = s_array_i64_get_checked(v_s_iter_46, s_index_46);
			S_Array_i64 * s_field_47 = s_array_i64_append_take(s_record_102_get_edges_take(s_record_102_copy(v_next)), v_child);
			s_array_i64_release(v_next.edges);
			v_next.edges = s_field_47;
		}
		s_array_i64_release(v_s_iter_46);
	}
	const int64_t v_index = s_generic_array_length_take(s_record_102_get_nodes_take(s_record_102_copy(v_next)));
	S_Array * s_field_48 = s_ga_10101_append_take(s_record_102_get_nodes_take(s_record_102_copy(v_next)), s_Node(v_kind, v_token, v_first, s_array_i64_length_take(s_array_i64_retain(v_children))));
	s_array_release(v_next.nodes);
	v_next.nodes = s_field_48;
	s_record_103 s_return_49 = s_ParseResult(s_record_102_copy(v_next), v_index);
	s_record_102_drop(v_next);
	s_array_i64_release(v_children);
	s_record_102_drop(v_p);
	return s_return_49;
	s_record_102_drop(v_next);
}

s_record_103 s_leaf(s_record_102 v_p, int64_t v_kind, int64_t v_token)
{
	s_record_103 s_return_50 = s_append_node(s_record_102_copy(v_p), v_kind, v_token, s_array_i64_make(0, NULL));
	s_record_102_drop(v_p);
	return s_return_50;
}

s_record_103 s_unary_node(s_record_102 v_p, int64_t v_kind, int64_t v_token, int64_t v_a)
{
	S_Array_i64 * v_children = s_array_i64_make(0, NULL);
	int64_t s_item_51 = v_a;
	v_children = s_array_i64_append_take(v_children, s_item_51);
	s_record_103 s_return_52 = s_append_node(s_record_102_copy(v_p), v_kind, v_token, s_array_i64_retain(v_children));
	s_array_i64_release(v_children);
	s_record_102_drop(v_p);
	return s_return_52;
	s_array_i64_release(v_children);
}

s_record_103 s_binary_node(s_record_102 v_p, int64_t v_kind, int64_t v_token, int64_t v_a, int64_t v_b)
{
	S_Array_i64 * v_children = s_array_i64_make(0, NULL);
	int64_t s_item_53 = v_a;
	v_children = s_array_i64_append_take(v_children, s_item_53);
	int64_t s_item_54 = v_b;
	v_children = s_array_i64_append_take(v_children, s_item_54);
	s_record_103 s_return_55 = s_append_node(s_record_102_copy(v_p), v_kind, v_token, s_array_i64_retain(v_children));
	s_array_i64_release(v_children);
	s_record_102_drop(v_p);
	return s_return_55;
	s_array_i64_release(v_children);
}

int64_t s_precedence(S_String * v_op)
{
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("or", sizeof("or") - 1)) == 0))
	{
		int64_t s_return_56 = 1;
		s_string_release(v_op);
		return s_return_56;
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("and", sizeof("and") - 1)) == 0))
	{
		int64_t s_return_57 = 2;
		s_string_release(v_op);
		return s_return_57;
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("|", sizeof("|") - 1)) == 0))
	{
		int64_t s_return_58 = 3;
		s_string_release(v_op);
		return s_return_58;
	}
	if (((((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("==", sizeof("==") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("!=", sizeof("!=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<", sizeof("<") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<=", sizeof("<=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">", sizeof(">") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">=", sizeof(">=") - 1)) == 0)))
	{
		int64_t s_return_59 = 4;
		s_string_release(v_op);
		return s_return_59;
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("..", sizeof("..") - 1)) == 0))
	{
		int64_t s_return_60 = 5;
		s_string_release(v_op);
		return s_return_60;
	}
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("-", sizeof("-") - 1)) == 0)))
	{
		int64_t s_return_61 = 6;
		s_string_release(v_op);
		return s_return_61;
	}
	if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("*", sizeof("*") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("/", sizeof("/") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("%", sizeof("%") - 1)) == 0)))
	{
		int64_t s_return_62 = 7;
		s_string_release(v_op);
		return s_return_62;
	}
	int64_t s_return_63 = (-1);
	s_string_release(v_op);
	return s_return_63;
}

s_record_103 s_parse_type(s_record_102 v_p)
{
	const int64_t v_token = s_record_102_get_index_take(s_record_102_copy(v_p));
	s_record_102 v_next = s_expect_kind(s_record_102_copy(v_p), 1, s_string_from_bytes("type name", sizeof("type name") - 1));
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_103 s_return_64 = s_ParseResult(s_record_102_copy(v_next), (-1));
		s_record_102_drop(v_next);
		s_record_102_drop(v_p);
		return s_return_64;
	}
	S_Array_i64 * v_args = s_array_i64_make(0, NULL);
	if (s_is_token(s_record_102_copy(v_next), s_string_from_bytes("[", sizeof("[") - 1)))
	{
		s_record_102 s_value_65 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("[", sizeof("[") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_65;
		while (true) {
			if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes("]", sizeof("]") - 1)))))) break;
			s_record_103 v_item = s_ParseResult(s_record_102_copy(v_next), (-1));
			if (s_kind_is(s_record_102_copy(v_next), 2))
			{
				const int64_t v_index = s_record_102_get_index_take(s_record_102_copy(v_next));
				v_next.index += 1;
				s_record_103 s_value_66 = s_leaf(s_record_102_copy(v_next), 24, v_index);
				s_record_103_drop(v_item);
				v_item = s_value_66;
			}
			else
			{
				s_record_103 s_value_67 = s_parse_type(s_record_102_copy(v_next));
				s_record_103_drop(v_item);
				v_item = s_value_67;
			}
			s_record_102 s_value_68 = s_record_103_get_state_take(s_record_103_copy(v_item));
			s_record_102_drop(v_next);
			v_next = s_value_68;
			if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_103_drop(v_item);
				break;
			}
			int64_t s_item_69 = s_record_103_get_node_take(s_record_103_copy(v_item));
			v_args = s_array_i64_append_take(v_args, s_item_69);
			if ((!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))
			{
				s_record_103_drop(v_item);
				break;
			}
			s_record_102 s_value_70 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_70;
			s_record_103_drop(v_item);
		}
		s_record_102 s_value_71 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("]", sizeof("]") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_71;
	}
	s_record_103 s_return_72 = s_append_node(s_record_102_copy(v_next), 38, v_token, s_array_i64_retain(v_args));
	s_array_i64_release(v_args);
	s_record_102_drop(v_next);
	s_record_102_drop(v_p);
	return s_return_72;
	s_array_i64_release(v_args);
	s_record_102_drop(v_next);
}

s_record_103 s_parse_param(s_record_102 v_p)
{
	const int64_t v_token = s_record_102_get_index_take(s_record_102_copy(v_p));
	s_record_102 v_next = s_expect_kind(s_record_102_copy(v_p), 1, s_string_from_bytes("parameter name", sizeof("parameter name") - 1));
	S_Array_i64 * v_children = s_array_i64_make(0, NULL);
	if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1))))
	{
		s_record_102 s_value_73 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_73;
		s_record_103 v_annotation = s_parse_type(s_record_102_copy(v_next));
		s_record_102 s_value_74 = s_record_103_get_state_take(s_record_103_copy(v_annotation));
		s_record_102_drop(v_next);
		v_next = s_value_74;
		int64_t s_item_75 = s_record_103_get_node_take(s_record_103_copy(v_annotation));
		v_children = s_array_i64_append_take(v_children, s_item_75);
		s_record_103_drop(v_annotation);
	}
	if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("=", sizeof("=") - 1))))
	{
		s_record_102 s_value_76 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("=", sizeof("=") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_76;
		s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_77 = s_record_103_get_state_take(s_record_103_copy(v_value));
		s_record_102_drop(v_next);
		v_next = s_value_77;
		int64_t s_item_78 = s_record_103_get_node_take(s_record_103_copy(v_value));
		v_children = s_array_i64_append_take(v_children, s_item_78);
		s_record_103_drop(v_value);
	}
	s_record_103 s_return_79 = s_append_node(s_record_102_copy(v_next), 31, v_token, s_array_i64_retain(v_children));
	s_array_i64_release(v_children);
	s_record_102_drop(v_next);
	s_record_102_drop(v_p);
	return s_return_79;
	s_array_i64_release(v_children);
	s_record_102_drop(v_next);
}

s_record_103 s_parse_expr(s_record_102 v_p, int64_t v_min_bp)
{
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_103 s_return_80 = s_ParseResult(s_record_102_copy(v_p), (-1));
		s_record_102_drop(v_p);
		return s_return_80;
	}
	const int64_t v_token = s_record_102_get_index_take(s_record_102_copy(v_p));
	s_record_100 v_t = s_current(s_record_102_copy(v_p));
	S_String * v_text = s_word(s_record_102_copy(v_p));
	s_record_102 v_next = s_record_102_copy(v_p);
	v_next.index += 1;
	int64_t v_lhs = (-1);
	if (((((s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("-", sizeof("-") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("+", sizeof("+") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("not", sizeof("not") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("~", sizeof("~") - 1)) == 0)))
	{
		s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 8);
		s_record_102 s_value_81 = s_record_103_get_state_take(s_record_103_copy(v_value));
		s_record_102_drop(v_next);
		v_next = s_value_81;
		s_record_103 v_node = s_unary_node(s_record_102_copy(v_next), 39, v_token, s_record_103_get_node_take(s_record_103_copy(v_value)));
		s_record_102 s_value_82 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_82;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
		s_record_103_drop(v_value);
	}
	else if ((s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("(", sizeof("(") - 1)) == 0))
	{
		s_record_103 v_inner = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_83 = s_expect(s_record_103_get_state_take(s_record_103_copy(v_inner)), s_string_from_bytes(")", sizeof(")") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_83;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_inner));
		s_record_103_drop(v_inner);
	}
	else if ((s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("[", sizeof("[") - 1)) == 0))
	{
		S_Array_i64 * v_items = s_array_i64_make(0, NULL);
		while (true) {
			if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes("]", sizeof("]") - 1)))))) break;
			s_record_103 v_item = s_parse_expr(s_record_102_copy(v_next), 0);
			s_record_102 s_value_84 = s_record_103_get_state_take(s_record_103_copy(v_item));
			s_record_102_drop(v_next);
			v_next = s_value_84;
			if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_103_drop(v_item);
				break;
			}
			int64_t s_item_85 = s_record_103_get_node_take(s_record_103_copy(v_item));
			v_items = s_array_i64_append_take(v_items, s_item_85);
			if ((!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))
			{
				s_record_103_drop(v_item);
				break;
			}
			s_record_102 s_value_86 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_86;
			s_record_103_drop(v_item);
		}
		s_record_102 s_value_87 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("]", sizeof("]") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_87;
		s_record_103 v_node = s_append_node(s_record_102_copy(v_next), 2, v_token, s_array_i64_retain(v_items));
		s_record_102 s_value_88 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_88;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
		s_array_i64_release(v_items);
	}
	else if ((s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("{", sizeof("{") - 1)) == 0))
	{
		S_Array_i64 * v_fields = s_array_i64_make(0, NULL);
		while (true) {
			if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1)))))) break;
			const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_103 v_item = s_ParseResult(s_record_102_copy(v_next), (-1));
			if (s_is_token(s_record_102_copy(v_next), s_string_from_bytes("del", sizeof("del") - 1)))
			{
				s_record_102 s_value_89 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("del", sizeof("del") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_89;
				const int64_t v_deleted = s_record_102_get_index_take(s_record_102_copy(v_next));
				s_record_102 s_value_90 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("deleted field", sizeof("deleted field") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_90;
				s_record_103 s_value_91 = s_leaf(s_record_102_copy(v_next), 15, v_deleted);
				s_record_103_drop(v_item);
				v_item = s_value_91;
			}
			else
			{
				s_record_102 s_value_92 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("field name", sizeof("field name") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_92;
				if (s_is_token(s_record_102_copy(v_next), s_string_from_bytes(".", sizeof(".") - 1)))
				{
					s_record_102 s_value_93 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(".", sizeof(".") - 1));
					s_record_102_drop(v_next);
					v_next = s_value_93;
					s_record_102 s_value_94 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("*", sizeof("*") - 1));
					s_record_102_drop(v_next);
					v_next = s_value_94;
					s_record_103 s_value_95 = s_leaf(s_record_102_copy(v_next), 36, v_name);
					s_record_103_drop(v_item);
					v_item = s_value_95;
				}
				else
				{
					s_record_102 s_value_96 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("=", sizeof("=") - 1));
					s_record_102_drop(v_next);
					v_next = s_value_96;
					s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
					s_record_103 s_value_97 = s_unary_node(s_record_103_get_state_take(s_record_103_copy(v_value)), 19, v_name, s_record_103_get_node_take(s_record_103_copy(v_value)));
					s_record_103_drop(v_item);
					v_item = s_value_97;
					s_record_103_drop(v_value);
				}
			}
			s_record_102 s_value_98 = s_record_103_get_state_take(s_record_103_copy(v_item));
			s_record_102_drop(v_next);
			v_next = s_value_98;
			if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_103_drop(v_item);
				break;
			}
			int64_t s_item_99 = s_record_103_get_node_take(s_record_103_copy(v_item));
			v_fields = s_array_i64_append_take(v_fields, s_item_99);
			if (s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1)))
			{
				s_record_102 s_value_100 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_100;
			}
			s_record_103_drop(v_item);
		}
		s_record_102 s_value_101 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_101;
		s_record_103 v_node = s_append_node(s_record_102_copy(v_next), 33, v_token, s_array_i64_retain(v_fields));
		s_record_102 s_value_102 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_102;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
		s_array_i64_release(v_fields);
	}
	else if ((s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("fn", sizeof("fn") - 1)) == 0))
	{
		s_record_102 s_value_103 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("(", sizeof("(") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_103;
		S_Array_i64 * v_args = s_array_i64_make(0, NULL);
		while (true) {
			if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1)))))) break;
			s_record_103 v_item = s_parse_param(s_record_102_copy(v_next));
			s_record_102 s_value_104 = s_record_103_get_state_take(s_record_103_copy(v_item));
			s_record_102_drop(v_next);
			v_next = s_value_104;
			if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_103_drop(v_item);
				break;
			}
			int64_t s_item_105 = s_record_103_get_node_take(s_record_103_copy(v_item));
			v_args = s_array_i64_append_take(v_args, s_item_105);
			if ((!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))
			{
				s_record_103_drop(v_item);
				break;
			}
			s_record_102 s_value_106 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_106;
			s_record_103_drop(v_item);
		}
		s_record_102 s_value_107 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_107;
		s_record_103 v_body = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_108 = s_record_103_get_state_take(s_record_103_copy(v_body));
		s_record_102_drop(v_next);
		v_next = s_value_108;
		int64_t s_item_109 = s_record_103_get_node_take(s_record_103_copy(v_body));
		v_args = s_array_i64_append_take(v_args, s_item_109);
		s_record_103 v_node = s_append_node(s_record_102_copy(v_next), 25, v_token, s_array_i64_retain(v_args));
		s_record_102 s_value_110 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_110;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
		s_record_103_drop(v_body);
		s_array_i64_release(v_args);
	}
	else if (((s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("true", sizeof("true") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_text), s_string_from_bytes("false", sizeof("false") - 1)) == 0)))
	{
		s_record_103 v_node = s_leaf(s_record_102_copy(v_next), 8, v_token);
		s_record_102 s_value_111 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_111;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
	}
	else if ((s_record_100_get_kind_take(s_record_100_copy(v_t)) == 1))
	{
		s_record_103 v_node = s_leaf(s_record_102_copy(v_next), 29, v_token);
		s_record_102 s_value_112 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_112;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
	}
	else if ((s_record_100_get_kind_take(s_record_100_copy(v_t)) == 2))
	{
		s_record_103 v_node = s_leaf(s_record_102_copy(v_next), 24, v_token);
		s_record_102 s_value_113 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_113;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
	}
	else if ((s_record_100_get_kind_take(s_record_100_copy(v_t)) == 3))
	{
		s_record_103 v_node = s_leaf(s_record_102_copy(v_next), 20, v_token);
		s_record_102 s_value_114 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_114;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
	}
	else if ((s_record_100_get_kind_take(s_record_100_copy(v_t)) == 4))
	{
		s_record_103 v_node = s_leaf(s_record_102_copy(v_next), 37, v_token);
		s_record_102 s_value_115 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_115;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
	}
	else
	{
		s_record_103 s_return_116 = s_ParseResult(s_fail(s_record_102_copy(v_p), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unexpected expression '", sizeof("unexpected expression '") - 1), s_string_retain(v_text)), s_string_from_bytes("'", sizeof("'") - 1))), (-1));
		s_record_102_drop(v_next);
		s_string_release(v_text);
		s_record_100_drop(v_t);
		s_record_102_drop(v_p);
		return s_return_116;
	}
	while (true) {
		if (!((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))) break;
		S_String * v_op = s_word(s_record_102_copy(v_next));
		if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("(", sizeof("(") - 1)) == 0) && (v_min_bp <= 10)))
		{
			const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_117 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("(", sizeof("(") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_117;
			S_Array_i64 * v_args = s_array_i64_make(0, NULL);
			int64_t s_item_118 = v_lhs;
			v_args = s_array_i64_append_take(v_args, s_item_118);
			while (true) {
				if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1)))))) break;
				s_record_103 v_arg = s_parse_expr(s_record_102_copy(v_next), 0);
				s_record_102 s_value_119 = s_record_103_get_state_take(s_record_103_copy(v_arg));
				s_record_102_drop(v_next);
				v_next = s_value_119;
				if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_103_drop(v_arg);
					break;
				}
				int64_t s_item_120 = s_record_103_get_node_take(s_record_103_copy(v_arg));
				v_args = s_array_i64_append_take(v_args, s_item_120);
				if ((!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))
				{
					s_record_103_drop(v_arg);
					break;
				}
				s_record_102 s_value_121 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_121;
				s_record_103_drop(v_arg);
			}
			s_record_102 s_value_122 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_122;
			s_record_103 v_node = s_append_node(s_record_102_copy(v_next), 10, v_position, s_array_i64_retain(v_args));
			s_record_102 s_value_123 = s_record_103_get_state_take(s_record_103_copy(v_node));
			s_record_102_drop(v_next);
			v_next = s_value_123;
			v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
			s_record_103_drop(v_node);
			s_array_i64_release(v_args);
			s_string_release(v_op);
			continue;
			s_record_103_drop(v_node);
			s_array_i64_release(v_args);
		}
		if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(".", sizeof(".") - 1)) == 0) && (v_min_bp <= 10)))
		{
			s_record_102 s_value_124 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(".", sizeof(".") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_124;
			const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_125 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("member name", sizeof("member name") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_125;
			s_record_103 v_node = s_unary_node(s_record_102_copy(v_next), 4, v_name, v_lhs);
			s_record_102 s_value_126 = s_record_103_get_state_take(s_record_103_copy(v_node));
			s_record_102_drop(v_next);
			v_next = s_value_126;
			v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
			s_record_103_drop(v_node);
			s_string_release(v_op);
			continue;
			s_record_103_drop(v_node);
		}
		if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("[", sizeof("[") - 1)) == 0) && (v_min_bp <= 10)))
		{
			const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_127 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("[", sizeof("[") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_127;
			S_Array_i64 * v_args = s_array_i64_make(0, NULL);
			int64_t s_item_128 = v_lhs;
			v_args = s_array_i64_append_take(v_args, s_item_128);
			s_record_103 v_index = s_parse_expr(s_record_102_copy(v_next), 0);
			s_record_102 s_value_129 = s_record_103_get_state_take(s_record_103_copy(v_index));
			s_record_102_drop(v_next);
			v_next = s_value_129;
			int64_t s_item_130 = s_record_103_get_node_take(s_record_103_copy(v_index));
			v_args = s_array_i64_append_take(v_args, s_item_130);
			while (true) {
				if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))) break;
				s_record_102 s_value_131 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_131;
				s_record_103 v_item = s_parse_expr(s_record_102_copy(v_next), 0);
				s_record_102 s_value_132 = s_record_103_get_state_take(s_record_103_copy(v_item));
				s_record_102_drop(v_next);
				v_next = s_value_132;
				int64_t s_item_133 = s_record_103_get_node_take(s_record_103_copy(v_item));
				v_args = s_array_i64_append_take(v_args, s_item_133);
				s_record_103_drop(v_item);
			}
			int64_t v_kind = 23;
			if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("?", sizeof("?") - 1))))
			{
				s_record_102 s_value_134 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("?", sizeof("?") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_134;
				v_kind = 30;
			}
			s_record_102 s_value_135 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("]", sizeof("]") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_135;
			if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("!", sizeof("!") - 1))))
			{
				s_record_102 s_value_136 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("!", sizeof("!") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_136;
				if ((v_kind != 30))
				{
					s_record_102 s_value_137 = s_fail(s_record_102_copy(v_next), s_string_from_bytes("checked indexing requires '?'", sizeof("checked indexing requires '?'") - 1));
					s_record_102_drop(v_next);
					v_next = s_value_137;
				}
				v_kind = 12;
			}
			s_record_103 v_node = s_append_node(s_record_102_copy(v_next), v_kind, v_position, s_array_i64_retain(v_args));
			s_record_102 s_value_138 = s_record_103_get_state_take(s_record_103_copy(v_node));
			s_record_102_drop(v_next);
			v_next = s_value_138;
			v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
			s_record_103_drop(v_node);
			s_record_103_drop(v_index);
			s_array_i64_release(v_args);
			s_string_release(v_op);
			continue;
			s_record_103_drop(v_node);
			s_record_103_drop(v_index);
			s_array_i64_release(v_args);
		}
		const int64_t v_bp = s_precedence(s_string_retain(v_op));
		if (((v_bp < v_min_bp) || (v_bp < 0)))
		{
			s_string_release(v_op);
			break;
		}
		const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
		v_next.index += 1;
		s_record_103 v_rhs = s_parse_expr(s_record_102_copy(v_next), (v_bp + 1));
		s_record_102 s_value_139 = s_record_103_get_state_take(s_record_103_copy(v_rhs));
		s_record_102_drop(v_next);
		v_next = s_value_139;
		s_record_103 v_node = s_binary_node(s_record_102_copy(v_next), 5, v_position, v_lhs, s_record_103_get_node_take(s_record_103_copy(v_rhs)));
		s_record_102 s_value_140 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_140;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
		s_record_103_drop(v_rhs);
		s_string_release(v_op);
	}
	s_record_103 s_return_141 = s_ParseResult(s_record_102_copy(v_next), v_lhs);
	s_record_102_drop(v_next);
	s_string_release(v_text);
	s_record_100_drop(v_t);
	s_record_102_drop(v_p);
	return s_return_141;
	s_record_102_drop(v_next);
	s_string_release(v_text);
	s_record_100_drop(v_t);
}

s_record_103 s_parse_block(s_record_102 v_p)
{
	const int64_t v_token = s_record_102_get_index_take(s_record_102_copy(v_p));
	s_record_102 v_next = s_expect_kind(s_record_102_copy(v_p), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_103 s_return_142 = s_ParseResult(s_record_102_copy(v_next), (-1));
		s_record_102_drop(v_next);
		s_record_102_drop(v_p);
		return s_return_142;
	}
	s_record_102 s_value_143 = s_expect_kind(s_record_102_copy(v_next), 7, s_string_from_bytes("indented block", sizeof("indented block") - 1));
	s_record_102_drop(v_next);
	v_next = s_value_143;
	S_Array_i64 * v_items = s_array_i64_make(0, NULL);
	while (true) {
		if (!((((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_kind_is(s_record_102_copy(v_next), 8))) && (!s_kind_is(s_record_102_copy(v_next), 9))))) break;
		s_record_103 v_item = s_parse_statement(s_record_102_copy(v_next));
		s_record_102 s_value_144 = s_record_103_get_state_take(s_record_103_copy(v_item));
		s_record_102_drop(v_next);
		v_next = s_value_144;
		if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_103_drop(v_item);
			break;
		}
		int64_t s_item_145 = s_record_103_get_node_take(s_record_103_copy(v_item));
		v_items = s_array_i64_append_take(v_items, s_item_145);
		s_record_103_drop(v_item);
	}
	if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_kind_is(s_record_102_copy(v_next), 9)))
	{
		s_record_102 s_value_146 = s_fail(s_record_102_copy(v_next), s_string_from_bytes("unterminated block", sizeof("unterminated block") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_146;
	}
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		s_record_102 s_value_147 = s_expect_kind(s_record_102_copy(v_next), 8, s_string_from_bytes("dedent", sizeof("dedent") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_147;
	}
	s_record_103 s_return_148 = s_append_node(s_record_102_copy(v_next), 7, v_token, s_array_i64_retain(v_items));
	s_array_i64_release(v_items);
	s_record_102_drop(v_next);
	s_record_102_drop(v_p);
	return s_return_148;
	s_array_i64_release(v_items);
	s_record_102_drop(v_next);
}

s_record_103 s_rec_body(s_record_102 v_p, S_Array_i64 * v_params)
{
	s_record_103 v_body = s_ParseResult(s_record_102_copy(v_p), (-1));
	if (s_kind_is(s_record_102_copy(v_p), 6))
	{
		s_record_102 v_next = s_expect_kind(s_record_102_copy(v_p), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		if (s_kind_is(s_record_102_copy(v_next), 7))
		{
			s_record_103 s_value_149 = s_parse_block(s_record_102_copy(v_p));
			s_record_103_drop(v_body);
			v_body = s_value_149;
		}
		else
		{
			s_record_103 s_value_150 = s_append_node(s_record_102_copy(v_next), 7, s_record_102_get_index_take(s_record_102_copy(v_p)), s_array_i64_make(0, NULL));
			s_record_103_drop(v_body);
			v_body = s_value_150;
		}
		s_record_102_drop(v_next);
	}
	else
	{
		s_record_103 s_return_151 = s_ParseResult(s_fail(s_record_102_copy(v_p), s_string_from_bytes("expected newline after rec parameters", sizeof("expected newline after rec parameters") - 1)), (-1));
		s_record_103_drop(v_body);
		s_array_i64_release(v_params);
		s_record_102_drop(v_p);
		return s_return_151;
	}
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_103_get_state_take(s_record_103_copy(v_body))), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_103 s_return_152 = s_record_103_copy(v_body);
		s_record_103_drop(v_body);
		s_array_i64_release(v_params);
		s_record_102_drop(v_p);
		return s_return_152;
	}
	s_record_102 v_next = s_record_103_get_state_take(s_record_103_copy(v_body));
	S_Array_i64 * v_statements = s_array_i64_make(0, NULL);
	s_record_101 v_block = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_next)), s_record_103_get_node_take(s_record_103_copy(v_body)));
	{
		S_Array_i64 * v_s_iter_153 = s_array_i64_retain(v_params);
		for (int64_t s_index_153 = 0; s_index_153 < (int64_t)v_s_iter_153->length; ++s_index_153)
		{
			int64_t v_param = s_array_i64_get_checked(v_s_iter_153, s_index_153);
			s_record_101 v_entry = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_next)), v_param);
			S_String * v_name = s_token_text(s_record_102_get_source_take(s_record_102_copy(v_next)), s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_next)), s_record_101_get_token_take(s_record_101_copy(v_entry))));
			bool v_explicit = false;
			for (int64_t v_i = 0, s_bound_154 = s_record_101_get_count_take(s_record_101_copy(v_block)); v_i < s_bound_154; ++v_i)
			{
				const int64_t v_child = s_array_i64_get_checked_take(s_record_102_get_edges_take(s_record_102_copy(v_next)), (s_record_101_get_first_take(s_record_101_copy(v_block)) + v_i));
				s_record_101 v_published = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_next)), v_child);
				if (((s_record_101_get_kind_take(s_record_101_copy(v_published)) == 32) && (s_string_compare_take(s_token_text(s_record_102_get_source_take(s_record_102_copy(v_next)), s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_next)), s_record_101_get_token_take(s_record_101_copy(v_published)))), s_string_retain(v_name)) == 0)))
				{
					v_explicit = true;
				}
				s_record_101_drop(v_published);
			}
			if ((!v_explicit))
			{
				s_record_103 v_node = s_leaf(s_record_102_copy(v_next), 32, s_record_101_get_token_take(s_record_101_copy(v_entry)));
				s_record_102 s_value_155 = s_record_103_get_state_take(s_record_103_copy(v_node));
				s_record_102_drop(v_next);
				v_next = s_value_155;
				int64_t s_item_156 = s_record_103_get_node_take(s_record_103_copy(v_node));
				v_statements = s_array_i64_append_take(v_statements, s_item_156);
				s_record_103_drop(v_node);
			}
			s_string_release(v_name);
			s_record_101_drop(v_entry);
		}
		s_array_i64_release(v_s_iter_153);
	}
	for (int64_t v_i = 0, s_bound_157 = s_record_101_get_count_take(s_record_101_copy(v_block)); v_i < s_bound_157; ++v_i)
	{
		int64_t s_item_158 = s_array_i64_get_checked_take(s_record_102_get_edges_take(s_record_102_copy(v_next)), (s_record_101_get_first_take(s_record_101_copy(v_block)) + v_i));
		v_statements = s_array_i64_append_take(v_statements, s_item_158);
	}
	s_record_103 s_return_159 = s_append_node(s_record_102_copy(v_next), 7, s_record_101_get_token_take(s_record_101_copy(v_block)), s_array_i64_retain(v_statements));
	s_record_101_drop(v_block);
	s_array_i64_release(v_statements);
	s_record_102_drop(v_next);
	s_record_103_drop(v_body);
	s_array_i64_release(v_params);
	s_record_102_drop(v_p);
	return s_return_159;
	s_record_101_drop(v_block);
	s_array_i64_release(v_statements);
	s_record_102_drop(v_next);
	s_record_103_drop(v_body);
}

s_record_103 s_parse_statement(s_record_102 v_p)
{
	const int64_t v_token = s_record_102_get_index_take(s_record_102_copy(v_p));
	S_String * v_op = s_word(s_record_102_copy(v_p));
	s_record_102 v_next = s_record_102_copy(v_p);
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("def", sizeof("def") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("rec", sizeof("rec") - 1)) == 0)))
	{
		v_next.index += 1;
		const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
		s_record_102 s_value_160 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("function name", sizeof("function name") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_160;
		s_record_102 s_value_161 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("(", sizeof("(") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_161;
		S_Array_i64 * v_args = s_array_i64_make(0, NULL);
		while (true) {
			if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1)))))) break;
			s_record_103 v_arg = s_parse_param(s_record_102_copy(v_next));
			s_record_102 s_value_162 = s_record_103_get_state_take(s_record_103_copy(v_arg));
			s_record_102_drop(v_next);
			v_next = s_value_162;
			if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_103_drop(v_arg);
				break;
			}
			int64_t s_item_163 = s_record_103_get_node_take(s_record_103_copy(v_arg));
			v_args = s_array_i64_append_take(v_args, s_item_163);
			if ((!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))
			{
				s_record_103_drop(v_arg);
				break;
			}
			s_record_102 s_value_164 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_164;
			s_record_103_drop(v_arg);
		}
		s_record_102 s_value_165 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_165;
		if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("def", sizeof("def") - 1)) == 0) && (s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0)) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("->", sizeof("->") - 1))))
		{
			s_record_102 s_value_166 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("->", sizeof("->") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_166;
			s_record_103 v_typ = s_parse_type(s_record_102_copy(v_next));
			s_record_102 s_value_167 = s_record_103_get_state_take(s_record_103_copy(v_typ));
			s_record_102_drop(v_next);
			v_next = s_value_167;
			int64_t s_item_168 = s_record_103_get_node_take(s_record_103_copy(v_typ));
			v_args = s_array_i64_append_take(v_args, s_item_168);
			s_record_103_drop(v_typ);
		}
		s_record_103 v_body = s_ParseResult(s_record_102_copy(v_next), (-1));
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("rec", sizeof("rec") - 1)) == 0))
		{
			s_record_103 s_value_169 = s_rec_body(s_record_102_copy(v_next), s_array_i64_retain(v_args));
			s_record_103_drop(v_body);
			v_body = s_value_169;
		}
		else
		{
			s_record_103 s_value_170 = s_parse_block(s_record_102_copy(v_next));
			s_record_103_drop(v_body);
			v_body = s_value_170;
		}
		s_record_102 s_value_171 = s_record_103_get_state_take(s_record_103_copy(v_body));
		s_record_102_drop(v_next);
		v_next = s_value_171;
		int64_t s_item_172 = s_record_103_get_node_take(s_record_103_copy(v_body));
		v_args = s_array_i64_append_take(v_args, s_item_172);
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("rec", sizeof("rec") - 1)) == 0))
		{
			s_record_103 s_return_173 = s_append_node(s_record_102_copy(v_next), 42, v_name, s_array_i64_retain(v_args));
			s_record_103_drop(v_body);
			s_array_i64_release(v_args);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_173;
		}
		s_record_103 s_return_174 = s_append_node(s_record_102_copy(v_next), 14, v_name, s_array_i64_retain(v_args));
		s_record_103_drop(v_body);
		s_array_i64_release(v_args);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_174;
		s_record_103_drop(v_body);
		s_array_i64_release(v_args);
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("if", sizeof("if") - 1)) == 0))
	{
		S_Array_i64 * v_arms = s_array_i64_make(0, NULL);
		bool v_first_arm = true;
		while (true) {
			if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && ((v_first_arm && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("if", sizeof("if") - 1))) || ((!v_first_arm) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("elif", sizeof("elif") - 1))))))) break;
			v_first_arm = false;
			const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
			v_next.index += 1;
			s_record_103 v_cond = s_parse_expr(s_record_102_copy(v_next), 0);
			s_record_103 v_body = s_parse_block(s_record_103_get_state_take(s_record_103_copy(v_cond)));
			s_record_103 v_arm = s_binary_node(s_record_103_get_state_take(s_record_103_copy(v_body)), 1, v_position, s_record_103_get_node_take(s_record_103_copy(v_cond)), s_record_103_get_node_take(s_record_103_copy(v_body)));
			s_record_102 s_value_175 = s_record_103_get_state_take(s_record_103_copy(v_arm));
			s_record_102_drop(v_next);
			v_next = s_value_175;
			int64_t s_item_176 = s_record_103_get_node_take(s_record_103_copy(v_arm));
			v_arms = s_array_i64_append_take(v_arms, s_item_176);
			s_record_103_drop(v_arm);
			s_record_103_drop(v_body);
			s_record_103_drop(v_cond);
		}
		if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1))))
		{
			const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_177 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_177;
			s_record_103 v_body = s_parse_block(s_record_102_copy(v_next));
			s_record_103 v_arm = s_unary_node(s_record_103_get_state_take(s_record_103_copy(v_body)), 16, v_position, s_record_103_get_node_take(s_record_103_copy(v_body)));
			s_record_102 s_value_178 = s_record_103_get_state_take(s_record_103_copy(v_arm));
			s_record_102_drop(v_next);
			v_next = s_value_178;
			int64_t s_item_179 = s_record_103_get_node_take(s_record_103_copy(v_arm));
			v_arms = s_array_i64_append_take(v_arms, s_item_179);
			s_record_103_drop(v_arm);
			s_record_103_drop(v_body);
		}
		s_record_103 s_return_180 = s_append_node(s_record_102_copy(v_next), 22, v_token, s_array_i64_retain(v_arms));
		s_array_i64_release(v_arms);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_180;
		s_array_i64_release(v_arms);
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("while", sizeof("while") - 1)) == 0))
	{
		s_record_102 s_value_181 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("while", sizeof("while") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_181;
		s_record_103 v_cond = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_103 v_body = s_parse_block(s_record_103_get_state_take(s_record_103_copy(v_cond)));
		s_record_103 s_return_182 = s_binary_node(s_record_103_get_state_take(s_record_103_copy(v_body)), 41, v_token, s_record_103_get_node_take(s_record_103_copy(v_cond)), s_record_103_get_node_take(s_record_103_copy(v_body)));
		s_record_103_drop(v_body);
		s_record_103_drop(v_cond);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_182;
		s_record_103_drop(v_body);
		s_record_103_drop(v_cond);
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("for", sizeof("for") - 1)) == 0))
	{
		s_record_102 s_value_183 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("for", sizeof("for") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_183;
		const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
		s_record_102 s_value_184 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("loop variable", sizeof("loop variable") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_184;
		S_Array_i64 * v_kids = s_array_i64_make(0, NULL);
		if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1))))
		{
			s_record_102 s_value_185 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_185;
			s_record_103 v_annotation = s_parse_type(s_record_102_copy(v_next));
			s_record_102 s_value_186 = s_record_103_get_state_take(s_record_103_copy(v_annotation));
			s_record_102_drop(v_next);
			v_next = s_value_186;
			int64_t s_item_187 = s_record_103_get_node_take(s_record_103_copy(v_annotation));
			v_kids = s_array_i64_append_take(v_kids, s_item_187);
			s_record_103_drop(v_annotation);
		}
		s_record_102 s_value_188 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("in", sizeof("in") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_188;
		s_record_103 v_iterable = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_103 v_body = s_parse_block(s_record_103_get_state_take(s_record_103_copy(v_iterable)));
		int64_t s_item_189 = s_record_103_get_node_take(s_record_103_copy(v_iterable));
		v_kids = s_array_i64_append_take(v_kids, s_item_189);
		int64_t s_item_190 = s_record_103_get_node_take(s_record_103_copy(v_body));
		v_kids = s_array_i64_append_take(v_kids, s_item_190);
		s_record_103 s_return_191 = s_append_node(s_record_103_get_state_take(s_record_103_copy(v_body)), 21, v_name, s_array_i64_retain(v_kids));
		s_record_103_drop(v_body);
		s_record_103_drop(v_iterable);
		s_array_i64_release(v_kids);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_191;
		s_record_103_drop(v_body);
		s_record_103_drop(v_iterable);
		s_array_i64_release(v_kids);
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("match", sizeof("match") - 1)) == 0))
	{
		s_record_102 s_value_192 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("match", sizeof("match") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_192;
		s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_193 = s_expect_kind(s_record_103_get_state_take(s_record_103_copy(v_value)), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_193;
		s_record_102 s_value_194 = s_expect_kind(s_record_102_copy(v_next), 7, s_string_from_bytes("indented match arms", sizeof("indented match arms") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_194;
		S_Array_i64 * v_arms = s_array_i64_make(0, NULL);
		int64_t s_item_195 = s_record_103_get_node_take(s_record_103_copy(v_value));
		v_arms = s_array_i64_append_take(v_arms, s_item_195);
		while (true) {
			if (!((((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_kind_is(s_record_102_copy(v_next), 8))) && (!s_kind_is(s_record_102_copy(v_next), 9))))) break;
			const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_196 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("variant name", sizeof("variant name") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_196;
			s_record_102 s_value_197 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("(", sizeof("(") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_197;
			S_Array_i64 * v_names = s_array_i64_make(0, NULL);
			while (true) {
				if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1)))))) break;
				const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
				s_record_102 s_value_198 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("bound name", sizeof("bound name") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_198;
				s_record_103 v_leaf_result = s_leaf(s_record_102_copy(v_next), 6, v_name);
				s_record_102 s_value_199 = s_record_103_get_state_take(s_record_103_copy(v_leaf_result));
				s_record_102_drop(v_next);
				v_next = s_value_199;
				int64_t s_item_200 = s_record_103_get_node_take(s_record_103_copy(v_leaf_result));
				v_names = s_array_i64_append_take(v_names, s_item_200);
				if ((!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))
				{
					s_record_103_drop(v_leaf_result);
					break;
				}
				s_record_102 s_value_201 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_201;
				s_record_103_drop(v_leaf_result);
			}
			s_record_102 s_value_202 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_202;
			s_record_103 v_body = s_parse_block(s_record_102_copy(v_next));
			s_record_102 s_value_203 = s_record_103_get_state_take(s_record_103_copy(v_body));
			s_record_102_drop(v_next);
			v_next = s_value_203;
			int64_t s_item_204 = s_record_103_get_node_take(s_record_103_copy(v_body));
			v_names = s_array_i64_append_take(v_names, s_item_204);
			s_record_103 v_arm = s_append_node(s_record_102_copy(v_next), 11, v_position, s_array_i64_retain(v_names));
			s_record_102 s_value_205 = s_record_103_get_state_take(s_record_103_copy(v_arm));
			s_record_102_drop(v_next);
			v_next = s_value_205;
			int64_t s_item_206 = s_record_103_get_node_take(s_record_103_copy(v_arm));
			v_arms = s_array_i64_append_take(v_arms, s_item_206);
			s_record_103_drop(v_arm);
			s_record_103_drop(v_body);
			s_array_i64_release(v_names);
		}
		s_record_102 s_value_207 = s_expect_kind(s_record_102_copy(v_next), 8, s_string_from_bytes("dedent", sizeof("dedent") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_207;
		s_record_103 s_return_208 = s_append_node(s_record_102_copy(v_next), 27, v_token, s_array_i64_retain(v_arms));
		s_array_i64_release(v_arms);
		s_record_103_drop(v_value);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_208;
		s_array_i64_release(v_arms);
		s_record_103_drop(v_value);
	}
	if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("let", sizeof("let") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("var", sizeof("var") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("pub", sizeof("pub") - 1)) == 0)))
	{
		v_next.index += 1;
		const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
		s_record_102 s_value_209 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("binding name", sizeof("binding name") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_209;
		S_Array_i64 * v_kids = s_array_i64_make(0, NULL);
		if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("pub", sizeof("pub") - 1)) == 0) && (s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0)) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(".", sizeof(".") - 1))))
		{
			s_record_102 s_value_210 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(".", sizeof(".") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_210;
			const int64_t v_field = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_211 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("published field name", sizeof("published field name") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_211;
			s_record_103 v_reference = s_leaf(s_record_102_copy(v_next), 29, v_name);
			s_record_103 v_attr = s_unary_node(s_record_103_get_state_take(s_record_103_copy(v_reference)), 4, v_field, s_record_103_get_node_take(s_record_103_copy(v_reference)));
			s_record_102 s_value_212 = s_record_103_get_state_take(s_record_103_copy(v_attr));
			s_record_102_drop(v_next);
			v_next = s_value_212;
			int64_t s_item_213 = s_record_103_get_node_take(s_record_103_copy(v_attr));
			v_kids = s_array_i64_append_take(v_kids, s_item_213);
			s_record_103_drop(v_attr);
			s_record_103_drop(v_reference);
		}
		else
		{
			if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1))))
			{
				s_record_102 s_value_214 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_214;
				s_record_103 v_typ = s_parse_type(s_record_102_copy(v_next));
				s_record_102 s_value_215 = s_record_103_get_state_take(s_record_103_copy(v_typ));
				s_record_102_drop(v_next);
				v_next = s_value_215;
				int64_t s_item_216 = s_record_103_get_node_take(s_record_103_copy(v_typ));
				v_kids = s_array_i64_append_take(v_kids, s_item_216);
				s_record_103_drop(v_typ);
			}
			if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("=", sizeof("=") - 1))))
			{
				s_record_102 s_value_217 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("=", sizeof("=") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_217;
				s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
				s_record_102 s_value_218 = s_record_103_get_state_take(s_record_103_copy(v_value));
				s_record_102_drop(v_next);
				v_next = s_value_218;
				int64_t s_item_219 = s_record_103_get_node_take(s_record_103_copy(v_value));
				v_kids = s_array_i64_append_take(v_kids, s_item_219);
				if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1))))
				{
					s_record_102 s_value_220 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1));
					s_record_102_drop(v_next);
					v_next = s_value_220;
					s_record_102 s_value_221 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("return", sizeof("return") - 1));
					s_record_102_drop(v_next);
					v_next = s_value_221;
					s_record_103 v_fallback = s_parse_expr(s_record_102_copy(v_next), 0);
					s_record_102 s_value_222 = s_record_103_get_state_take(s_record_103_copy(v_fallback));
					s_record_102_drop(v_next);
					v_next = s_value_222;
					s_record_103 v_node = s_unary_node(s_record_102_copy(v_next), 17, v_token, s_record_103_get_node_take(s_record_103_copy(v_fallback)));
					s_record_102 s_value_223 = s_record_103_get_state_take(s_record_103_copy(v_node));
					s_record_102_drop(v_next);
					v_next = s_value_223;
					int64_t s_item_224 = s_record_103_get_node_take(s_record_103_copy(v_node));
					v_kids = s_array_i64_append_take(v_kids, s_item_224);
					s_record_103_drop(v_node);
					s_record_103_drop(v_fallback);
				}
				s_record_103_drop(v_value);
			}
		}
		s_record_102 s_value_225 = s_expect_kind(s_record_102_copy(v_next), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_225;
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("let", sizeof("let") - 1)) == 0))
		{
			s_record_103 s_return_226 = s_append_node(s_record_102_copy(v_next), 26, v_name, s_array_i64_retain(v_kids));
			s_array_i64_release(v_kids);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_226;
		}
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("var", sizeof("var") - 1)) == 0))
		{
			s_record_103 s_return_227 = s_append_node(s_record_102_copy(v_next), 40, v_name, s_array_i64_retain(v_kids));
			s_array_i64_release(v_kids);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_227;
		}
		s_record_103 s_return_228 = s_append_node(s_record_102_copy(v_next), 32, v_name, s_array_i64_retain(v_kids));
		s_array_i64_release(v_kids);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_228;
		s_array_i64_release(v_kids);
	}
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("return", sizeof("return") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("recur", sizeof("recur") - 1)) == 0)))
	{
		v_next.index += 1;
		if (s_kind_is(s_record_102_copy(v_next), 6))
		{
			s_record_102 s_value_229 = s_expect_kind(s_record_102_copy(v_next), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_229;
			if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("return", sizeof("return") - 1)) == 0))
			{
				s_record_103 s_return_230 = s_leaf(s_record_102_copy(v_next), 35, v_token);
				s_record_102_drop(v_next);
				s_string_release(v_op);
				s_record_102_drop(v_p);
				return s_return_230;
			}
			s_record_103 s_return_231 = s_leaf(s_record_102_copy(v_next), 34, v_token);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_231;
		}
		s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_232 = s_expect_kind(s_record_103_get_state_take(s_record_103_copy(v_value)), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_232;
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("return", sizeof("return") - 1)) == 0))
		{
			s_record_103 s_return_233 = s_unary_node(s_record_102_copy(v_next), 35, v_token, s_record_103_get_node_take(s_record_103_copy(v_value)));
			s_record_103_drop(v_value);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_233;
		}
		s_record_103 s_return_234 = s_unary_node(s_record_102_copy(v_next), 34, v_token, s_record_103_get_node_take(s_record_103_copy(v_value)));
		s_record_103_drop(v_value);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_234;
		s_record_103_drop(v_value);
	}
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("break", sizeof("break") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("continue", sizeof("continue") - 1)) == 0)))
	{
		v_next.index += 1;
		s_record_102 s_value_235 = s_expect_kind(s_record_102_copy(v_next), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_235;
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("break", sizeof("break") - 1)) == 0))
		{
			s_record_103 s_return_236 = s_leaf(s_record_102_copy(v_next), 9, v_token);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_236;
		}
		s_record_103 s_return_237 = s_leaf(s_record_102_copy(v_next), 13, v_token);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_237;
	}
	s_record_103 v_lhs = s_parse_expr(s_record_102_copy(v_next), 0);
	s_record_102 s_value_238 = s_record_103_get_state_take(s_record_103_copy(v_lhs));
	s_record_102_drop(v_next);
	v_next = s_value_238;
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_103 s_return_239 = s_ParseResult(s_record_102_copy(v_next), (-1));
		s_record_103_drop(v_lhs);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_239;
	}
	S_String * v_assign = s_word(s_record_102_copy(v_next));
	if ((((((s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("=", sizeof("=") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("+=", sizeof("+=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("-=", sizeof("-=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("*=", sizeof("*=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("/=", sizeof("/=") - 1)) == 0)))
	{
		const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
		v_next.index += 1;
		s_record_103 v_rhs = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_240 = s_expect_kind(s_record_103_get_state_take(s_record_103_copy(v_rhs)), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_240;
		s_record_103 s_return_241 = s_binary_node(s_record_102_copy(v_next), 3, v_position, s_record_103_get_node_take(s_record_103_copy(v_lhs)), s_record_103_get_node_take(s_record_103_copy(v_rhs)));
		s_record_103_drop(v_rhs);
		s_string_release(v_assign);
		s_record_103_drop(v_lhs);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_241;
		s_record_103_drop(v_rhs);
	}
	s_record_102 s_value_242 = s_expect_kind(s_record_102_copy(v_next), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
	s_record_102_drop(v_next);
	v_next = s_value_242;
	s_record_103 s_return_243 = s_unary_node(s_record_102_copy(v_next), 18, v_token, s_record_103_get_node_take(s_record_103_copy(v_lhs)));
	s_string_release(v_assign);
	s_record_103_drop(v_lhs);
	s_record_102_drop(v_next);
	s_string_release(v_op);
	s_record_102_drop(v_p);
	return s_return_243;
	s_string_release(v_assign);
	s_record_103_drop(v_lhs);
	s_record_102_drop(v_next);
	s_string_release(v_op);
}

s_record_103 s_parse_module(S_String * v_source)
{
	S_Array * v_tokens = s_tokenize(s_string_retain(v_source));
	s_record_102 v_p = s_ParseState(s_string_retain(v_source), s_array_retain(v_tokens));
	{
		S_Array * v_s_iter_244 = s_array_retain(v_tokens);
		for (int64_t s_index_244 = 0; s_index_244 < (int64_t)s_array_length(v_s_iter_244); ++s_index_244)
		{
			s_record_100 v_token = s_ga_10100_get_borrow(v_s_iter_244, s_index_244);
			if ((s_record_100_get_kind_take(s_record_100_copy(v_token)) < 0))
			{
				S_String * s_field_245 = s_token_kind_name(s_record_100_get_kind_take(s_record_100_copy(v_token)));
				s_string_release(v_p.error);
				v_p.error = s_field_245;
				v_p.error_line = s_record_100_get_line_take(s_record_100_copy(v_token));
				s_record_103 s_return_246 = s_ParseResult(s_record_102_copy(v_p), (-1));
				s_record_100_drop(v_token);
				s_array_release(v_s_iter_244);
				s_record_102_drop(v_p);
				s_array_release(v_tokens);
				s_string_release(v_source);
				return s_return_246;
			}
			s_record_100_drop(v_token);
		}
		s_array_release(v_s_iter_244);
	}
	S_Array_i64 * v_children = s_array_i64_make(0, NULL);
	while (true) {
		if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_kind_is(s_record_102_copy(v_p), 9))))) break;
		s_record_103 v_item = s_parse_statement(s_record_102_copy(v_p));
		s_record_102 s_value_247 = s_record_103_get_state_take(s_record_103_copy(v_item));
		s_record_102_drop(v_p);
		v_p = s_value_247;
		if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_103_drop(v_item);
			break;
		}
		int64_t s_item_248 = s_record_103_get_node_take(s_record_103_copy(v_item));
		v_children = s_array_i64_append_take(v_children, s_item_248);
		s_record_103_drop(v_item);
	}
	s_record_103 s_return_249 = s_append_node(s_record_102_copy(v_p), 28, 0, s_array_i64_retain(v_children));
	s_array_i64_release(v_children);
	s_record_102_drop(v_p);
	s_array_release(v_tokens);
	s_string_release(v_source);
	return s_return_249;
	s_array_i64_release(v_children);
	s_record_102_drop(v_p);
	s_array_release(v_tokens);
}

s_record_104 s_RecordField(S_String * v_name, int64_t v_kind)
{
	s_record_104 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.kind = v_kind;
	return s_constructor_result;
}

s_record_105 s_TypeDescriptor(int64_t v_tag, int64_t v_first, int64_t v_second, S_Array * v_fields)
{
	s_record_105 s_constructor_result = {0};
	s_constructor_result.tag = v_tag;
	s_constructor_result.first = v_first;
	s_constructor_result.second = v_second;
	s_constructor_result.fields = v_fields;
	return s_constructor_result;
}

s_record_106 s_TypeTable(S_Array * v_entries)
{
	s_record_106 s_constructor_result = {0};
	s_constructor_result.entries = v_entries;
	return s_constructor_result;
}

s_record_107 s_InternedType(s_record_106 v_table, int64_t v_id)
{
	s_record_107 s_constructor_result = {0};
	s_constructor_result.table = v_table;
	s_constructor_result.id = v_id;
	return s_constructor_result;
}

s_record_106 s_new_type_table()
{
	S_Array * v_entries = s_ga_10105_make_take(0, NULL);
	for (int64_t v_tag = 0, s_bound_250 = 6; v_tag < s_bound_250; ++v_tag)
	{
		s_record_105 s_item_251 = s_TypeDescriptor(v_tag, 0, 0, s_ga_10104_make_take(0, NULL));
		v_entries = s_ga_10105_append_take(v_entries, s_item_251);
	}
	s_record_106 s_return_252 = s_TypeTable(s_array_retain(v_entries));
	s_array_release(v_entries);
	return s_return_252;
	s_array_release(v_entries);
}

s_record_107 s_intern_type(s_record_106 v_table, int64_t v_tag, int64_t v_first, int64_t v_second, S_Array * v_fields)
{
	for (int64_t v_i = 0, s_bound_253 = s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_table))); v_i < s_bound_253; ++v_i)
	{
		s_record_105 v_entry = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_table)), v_i);
		int64_t s_left_254 = s_generic_array_length_take(s_record_105_get_fields_take(s_record_105_copy(v_entry)));
		int64_t s_right_255 = s_generic_array_length_take(s_array_retain(v_fields));
		if (((((s_record_105_get_tag_take(s_record_105_copy(v_entry)) != v_tag) || (s_record_105_get_first_take(s_record_105_copy(v_entry)) != v_first)) || (s_record_105_get_second_take(s_record_105_copy(v_entry)) != v_second)) || (s_left_254 != s_right_255)))
		{
			s_record_105_drop(v_entry);
			continue;
		}
		bool v_equal = true;
		for (int64_t v_j = 0, s_bound_256 = s_generic_array_length_take(s_array_retain(v_fields)); v_j < s_bound_256; ++v_j)
		{
			s_record_104 v_a = s_ga_10104_get_checked_take(s_record_105_get_fields_take(s_record_105_copy(v_entry)), v_j);
			s_record_104 v_b = s_ga_10104_get_checked_take(s_array_retain(v_fields), v_j);
			if (((s_string_compare_take(s_record_104_get_name_take(s_record_104_copy(v_a)), s_record_104_get_name_take(s_record_104_copy(v_b))) != 0) || (s_record_104_get_kind_take(s_record_104_copy(v_a)) != s_record_104_get_kind_take(s_record_104_copy(v_b)))))
			{
				v_equal = false;
			}
			s_record_104_drop(v_b);
			s_record_104_drop(v_a);
		}
		if (v_equal)
		{
			s_record_107 s_return_257 = s_InternedType(s_record_106_copy(v_table), v_i);
			s_record_105_drop(v_entry);
			s_array_release(v_fields);
			s_record_106_drop(v_table);
			return s_return_257;
		}
		s_record_105_drop(v_entry);
	}
	s_record_106 v_next = s_record_106_copy(v_table);
	const int64_t v_id = s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_next)));
	S_Array * s_field_258 = s_ga_10105_append_take(s_record_106_get_entries_take(s_record_106_copy(v_next)), s_TypeDescriptor(v_tag, v_first, v_second, s_array_retain(v_fields)));
	s_array_release(v_next.entries);
	v_next.entries = s_field_258;
	s_record_107 s_return_259 = s_InternedType(s_record_106_copy(v_next), v_id);
	s_record_106_drop(v_next);
	s_array_release(v_fields);
	s_record_106_drop(v_table);
	return s_return_259;
	s_record_106_drop(v_next);
}

s_record_107 s_intern_array_type(s_record_106 v_table, int64_t v_element)
{
	s_record_107 s_return_260 = s_intern_type(s_record_106_copy(v_table), 7, v_element, 0, s_ga_10104_make_take(0, NULL));
	s_record_106_drop(v_table);
	return s_return_260;
}

s_record_107 s_intern_dict_type(s_record_106 v_table, int64_t v_key, int64_t v_value)
{
	s_record_107 s_return_261 = s_intern_type(s_record_106_copy(v_table), 8, v_key, v_value, s_ga_10104_make_take(0, NULL));
	s_record_106_drop(v_table);
	return s_return_261;
}

s_record_107 s_intern_optional_type(s_record_106 v_table, int64_t v_value)
{
	s_record_107 s_return_262 = s_intern_type(s_record_106_copy(v_table), 9, v_value, 0, s_ga_10104_make_take(0, NULL));
	s_record_106_drop(v_table);
	return s_return_262;
}

S_Array * s_ordered_fields(S_Array * v_fields)
{
	S_Array * v_result = s_ga_10104_make_take(0, NULL);
	{
		S_Array * v_s_iter_263 = s_array_retain(v_fields);
		for (int64_t s_index_263 = 0; s_index_263 < (int64_t)s_array_length(v_s_iter_263); ++s_index_263)
		{
			s_record_104 v_field = s_ga_10104_get_borrow(v_s_iter_263, s_index_263);
			int64_t v_at = 0;
			while (true) {
				if (!(((v_at < s_generic_array_length_take(s_array_retain(v_result))) && (s_string_compare_take(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_result), v_at)), s_record_104_get_name_take(s_record_104_copy(v_field))) < 0)))) break;
				v_at += 1;
			}
			S_Array * v_sorted = s_ga_10104_make_take(0, NULL);
			for (int64_t v_i = 0, s_bound_264 = v_at; v_i < s_bound_264; ++v_i)
			{
				s_record_104 s_item_265 = s_ga_10104_get_checked_take(s_array_retain(v_result), v_i);
				v_sorted = s_ga_10104_append_take(v_sorted, s_item_265);
			}
			s_record_104 s_item_266 = s_record_104_copy(v_field);
			v_sorted = s_ga_10104_append_take(v_sorted, s_item_266);
			for (int64_t v_i = v_at, s_bound_267 = s_generic_array_length_take(s_array_retain(v_result)); v_i < s_bound_267; ++v_i)
			{
				s_record_104 s_item_268 = s_ga_10104_get_checked_take(s_array_retain(v_result), v_i);
				v_sorted = s_ga_10104_append_take(v_sorted, s_item_268);
			}
			S_Array * s_value_269 = s_array_retain(v_sorted);
			s_array_release(v_result);
			v_result = s_value_269;
			s_array_release(v_sorted);
			s_record_104_drop(v_field);
		}
		s_array_release(v_s_iter_263);
	}
	S_Array * s_return_270 = s_array_retain(v_result);
	s_array_release(v_result);
	s_array_release(v_fields);
	return s_return_270;
	s_array_release(v_result);
}

s_record_107 s_intern_record_type(s_record_106 v_table, S_Array * v_fields)
{
	s_record_107 s_return_271 = s_intern_type(s_record_106_copy(v_table), 6, 0, 0, s_ordered_fields(s_array_retain(v_fields)));
	s_array_release(v_fields);
	s_record_106_drop(v_table);
	return s_return_271;
}

s_record_108 s_HirNode(int64_t v_source, int64_t v_operation, int64_t v_type_id, int64_t v_field_index)
{
	s_record_108 s_constructor_result = {0};
	s_constructor_result.source = v_source;
	s_constructor_result.operation = v_operation;
	s_constructor_result.type_id = v_type_id;
	s_constructor_result.field_index = v_field_index;
	return s_constructor_result;
}

s_record_109 s_HirResolution(int64_t v_type_id, int64_t v_field_index, S_String * v_error)
{
	s_record_109 s_constructor_result = {0};
	s_constructor_result.type_id = v_type_id;
	s_constructor_result.field_index = v_field_index;
	s_constructor_result.error = v_error;
	return s_constructor_result;
}

s_record_110 s_SymbolInfo(S_String * v_name, int64_t v_kind, int64_t v_type_id, bool v_mutable, int64_t v_depth)
{
	s_record_110 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.kind = v_kind;
	s_constructor_result.type_id = v_type_id;
	s_constructor_result.mutable = v_mutable;
	s_constructor_result.depth = v_depth;
	return s_constructor_result;
}

s_record_109 s_resolve_hir_name(S_Array * v_symbols, S_String * v_name)
{
	for (int64_t v_i = 0, s_bound_272 = s_generic_array_length_take(s_array_retain(v_symbols)); v_i < s_bound_272; ++v_i)
	{
		const int64_t v_index = ((s_generic_array_length_take(s_array_retain(v_symbols)) - 1) - v_i);
		s_record_110 v_symbol = s_ga_10110_get_checked_take(s_array_retain(v_symbols), v_index);
		if ((s_string_compare_take(s_record_110_get_name_take(s_record_110_copy(v_symbol)), s_string_retain(v_name)) == 0))
		{
			s_record_109 s_return_273 = s_HirResolution(s_record_110_get_type_id_take(s_record_110_copy(v_symbol)), v_index, s_string_from_bytes("", sizeof("") - 1));
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_array_release(v_symbols);
			return s_return_273;
		}
		s_record_110_drop(v_symbol);
	}
	s_record_109 s_return_274 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown name '", sizeof("unknown name '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
	s_string_release(v_name);
	s_array_release(v_symbols);
	return s_return_274;
}

bool s_hir_types_equal(int64_t v_left, int64_t v_right)
{
	return ((v_left == v_right) && (v_left != 0));
}

s_record_109 s_resolve_hir_field(s_record_106 v_types, int64_t v_owner, S_String * v_name)
{
	if (((v_owner < 0) || (v_owner >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		s_record_109 s_return_275 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown record field '", sizeof("unknown record field '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_275;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_owner);
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 6))
	{
		for (int64_t v_i = 0, s_bound_276 = s_generic_array_length_take(s_record_105_get_fields_take(s_record_105_copy(v_descriptor))); v_i < s_bound_276; ++v_i)
		{
			s_record_104 v_field = s_ga_10104_get_checked_take(s_record_105_get_fields_take(s_record_105_copy(v_descriptor)), v_i);
			if ((s_string_compare_take(s_record_104_get_name_take(s_record_104_copy(v_field)), s_string_retain(v_name)) == 0))
			{
				s_record_109 s_return_277 = s_HirResolution(s_record_104_get_kind_take(s_record_104_copy(v_field)), v_i, s_string_from_bytes("", sizeof("") - 1));
				s_record_104_drop(v_field);
				s_record_105_drop(v_descriptor);
				s_string_release(v_name);
				s_record_106_drop(v_types);
				return s_return_277;
			}
			s_record_104_drop(v_field);
		}
	}
	s_record_109 s_return_278 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown record field '", sizeof("unknown record field '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
	s_record_105_drop(v_descriptor);
	s_string_release(v_name);
	s_record_106_drop(v_types);
	return s_return_278;
	s_record_105_drop(v_descriptor);
}

s_record_109 s_resolve_hir_binary(s_record_106 v_types, int64_t v_left, int64_t v_right, S_String * v_op)
{
	if ((v_left != v_right))
	{
		s_record_109 s_return_279 = s_HirResolution(0, (-1), s_string_from_bytes("binary operand types differ", sizeof("binary operand types differ") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_279;
	}
	if (((v_left < 0) || (v_left >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		s_record_109 s_return_280 = s_HirResolution(0, (-1), s_string_from_bytes("unknown binary operand type", sizeof("unknown binary operand type") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_280;
	}
	const int64_t v_tag = s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_left));
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("and", sizeof("and") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("or", sizeof("or") - 1)) == 0)))
	{
		if ((v_tag != 3))
		{
			s_record_109 s_return_281 = s_HirResolution(0, (-1), s_string_from_bytes("logical operator requires bool", sizeof("logical operator requires bool") - 1));
			s_string_release(v_op);
			s_record_106_drop(v_types);
			return s_return_281;
		}
		s_record_109 s_return_282 = s_HirResolution(3, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_282;
	}
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("==", sizeof("==") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("!=", sizeof("!=") - 1)) == 0)))
	{
		s_record_109 s_return_283 = s_HirResolution(3, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_283;
	}
	if (((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<", sizeof("<") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<=", sizeof("<=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">", sizeof(">") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">=", sizeof(">=") - 1)) == 0)))
	{
		if ((((v_tag != 1) && (v_tag != 2)) && (v_tag != 4)))
		{
			s_record_109 s_return_284 = s_HirResolution(0, (-1), s_string_from_bytes("comparison requires numeric or string operands", sizeof("comparison requires numeric or string operands") - 1));
			s_string_release(v_op);
			s_record_106_drop(v_types);
			return s_return_284;
		}
		s_record_109 s_return_285 = s_HirResolution(3, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_285;
	}
	if ((((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("-", sizeof("-") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("*", sizeof("*") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("/", sizeof("/") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("%", sizeof("%") - 1)) == 0)))
	{
		if ((((v_tag != 1) && (v_tag != 2)) && (!((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0) && (v_tag == 4)))))
		{
			s_record_109 s_return_286 = s_HirResolution(0, (-1), s_string_from_bytes("arithmetic requires numeric operands", sizeof("arithmetic requires numeric operands") - 1));
			s_string_release(v_op);
			s_record_106_drop(v_types);
			return s_return_286;
		}
		if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("%", sizeof("%") - 1)) == 0) && (v_tag != 1)))
		{
			s_record_109 s_return_287 = s_HirResolution(0, (-1), s_string_from_bytes("remainder requires i64", sizeof("remainder requires i64") - 1));
			s_string_release(v_op);
			s_record_106_drop(v_types);
			return s_return_287;
		}
		s_record_109 s_return_288 = s_HirResolution(v_left, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_288;
	}
	s_record_109 s_return_289 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unsupported binary operator '", sizeof("unsupported binary operator '") - 1), s_string_retain(v_op)), s_string_from_bytes("'", sizeof("'") - 1)));
	s_string_release(v_op);
	s_record_106_drop(v_types);
	return s_return_289;
}

int64_t s_resolve_hir_literal(int64_t v_node_kind)
{
	if ((v_node_kind == 24))
	{
		return 1;
	}
	if ((v_node_kind == 20))
	{
		return 2;
	}
	if ((v_node_kind == 8))
	{
		return 3;
	}
	if ((v_node_kind == 37))
	{
		return 4;
	}
	return 0;
}

s_record_109 s_resolve_hir_unary(s_record_106 v_types, int64_t v_operand, S_String * v_op)
{
	if (((v_operand < 0) || (v_operand >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		s_record_109 s_return_290 = s_HirResolution(0, (-1), s_string_from_bytes("invalid unary operand", sizeof("invalid unary operand") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_290;
	}
	const int64_t v_tag = s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_operand));
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("not", sizeof("not") - 1)) == 0) && (v_tag == 3)))
	{
		s_record_109 s_return_291 = s_HirResolution(3, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_291;
	}
	if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("-", sizeof("-") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0)) && ((v_tag == 1) || (v_tag == 2))))
	{
		s_record_109 s_return_292 = s_HirResolution(v_operand, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_292;
	}
	s_record_109 s_return_293 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unsupported unary operator '", sizeof("unsupported unary operator '") - 1), s_string_retain(v_op)), s_string_from_bytes("'", sizeof("'") - 1)));
	s_string_release(v_op);
	s_record_106_drop(v_types);
	return s_return_293;
}

s_record_111 s_HirCallCheck(s_record_106 v_table, int64_t v_type_id, S_String * v_error)
{
	s_record_111 s_constructor_result = {0};
	s_constructor_result.table = v_table;
	s_constructor_result.type_id = v_type_id;
	s_constructor_result.error = v_error;
	return s_constructor_result;
}

S_String * s_hir_type_name(s_record_106 v_types, int64_t v_id)
{
	if (((v_id < 0) || (v_id >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		S_String * s_return_294 = s_string_from_bytes("unknown", sizeof("unknown") - 1);
		s_record_106_drop(v_types);
		return s_return_294;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_id);
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 1))
	{
		S_String * s_return_295 = s_string_from_bytes("i64", sizeof("i64") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_295;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 2))
	{
		S_String * s_return_296 = s_string_from_bytes("f64", sizeof("f64") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_296;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 3))
	{
		S_String * s_return_297 = s_string_from_bytes("bool", sizeof("bool") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_297;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 4))
	{
		S_String * s_return_298 = s_string_from_bytes("string", sizeof("string") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_298;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 5))
	{
		S_String * s_return_299 = s_string_from_bytes("void", sizeof("void") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_299;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 7))
	{
		S_String * s_return_300 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("Array[", sizeof("Array[") - 1), s_hir_type_name(s_record_106_copy(v_types), s_record_105_get_first_take(s_record_105_copy(v_descriptor)))), s_string_from_bytes("]", sizeof("]") - 1));
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_300;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 8))
	{
		S_String * s_left_301 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("Dict[", sizeof("Dict[") - 1), s_hir_type_name(s_record_106_copy(v_types), s_record_105_get_first_take(s_record_105_copy(v_descriptor)))), s_string_from_bytes(",", sizeof(",") - 1));
		S_String * s_right_302 = s_hir_type_name(s_record_106_copy(v_types), s_record_105_get_second_take(s_record_105_copy(v_descriptor)));
		S_String * s_return_303 = s_string_concat_take(s_string_concat_take(s_left_301, s_right_302), s_string_from_bytes("]", sizeof("]") - 1));
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_303;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 9))
	{
		S_String * s_return_304 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("Optional[", sizeof("Optional[") - 1), s_hir_type_name(s_record_106_copy(v_types), s_record_105_get_first_take(s_record_105_copy(v_descriptor)))), s_string_from_bytes("]", sizeof("]") - 1));
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_304;
	}
	S_String * s_return_305 = s_string_from_bytes("record", sizeof("record") - 1);
	s_record_105_drop(v_descriptor);
	s_record_106_drop(v_types);
	return s_return_305;
	s_record_105_drop(v_descriptor);
}

s_record_111 s_hir_expected(s_record_106 v_types, int64_t v_expected, int64_t v_found)
{
	if (s_hir_types_equal(v_expected, v_found))
	{
		s_record_111 s_return_306 = s_HirCallCheck(s_record_106_copy(v_types), v_expected, s_string_from_bytes("", sizeof("") - 1));
		s_record_106_drop(v_types);
		return s_return_306;
	}
	S_String * s_left_307 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_hir_type_name(s_record_106_copy(v_types), v_expected)), s_string_from_bytes(", found ", sizeof(", found ") - 1));
	S_String * s_right_308 = s_hir_type_name(s_record_106_copy(v_types), v_found);
	s_record_111 s_return_309 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_concat_take(s_left_307, s_right_308));
	s_record_106_drop(v_types);
	return s_return_309;
}

s_record_111 s_hir_call_arguments(s_record_106 v_types, S_Array_i64 * v_actual, S_Array_i64 * v_expected, int64_t v_result, S_String * v_arity_error)
{
	int64_t s_left_310 = s_array_i64_length_take(s_array_i64_retain(v_actual));
	int64_t s_right_311 = s_array_i64_length_take(s_array_i64_retain(v_expected));
	if ((s_left_310 != s_right_311))
	{
		s_record_111 s_return_312 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_retain(v_arity_error));
		s_string_release(v_arity_error);
		s_array_i64_release(v_expected);
		s_array_i64_release(v_actual);
		s_record_106_drop(v_types);
		return s_return_312;
	}
	for (int64_t v_i = 0, s_bound_313 = s_array_i64_length_take(s_array_i64_retain(v_actual)); v_i < s_bound_313; ++v_i)
	{
		s_record_111 v_check = s_hir_expected(s_record_106_copy(v_types), s_array_i64_get_checked_take(s_array_i64_retain(v_expected), v_i), s_array_i64_get_checked_take(s_array_i64_retain(v_actual), v_i));
		if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_check)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_111 s_return_314 = s_record_111_copy(v_check);
			s_record_111_drop(v_check);
			s_string_release(v_arity_error);
			s_array_i64_release(v_expected);
			s_array_i64_release(v_actual);
			s_record_106_drop(v_types);
			return s_return_314;
		}
		s_record_111_drop(v_check);
	}
	s_record_111 s_return_315 = s_HirCallCheck(s_record_106_copy(v_types), v_result, s_string_from_bytes("", sizeof("") - 1));
	s_string_release(v_arity_error);
	s_array_i64_release(v_expected);
	s_array_i64_release(v_actual);
	s_record_106_drop(v_types);
	return s_return_315;
}

bool s_hir_is_builtin(S_String * v_name)
{
	bool s_return_316 = (((((((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("arguments", sizeof("arguments") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("read_text", sizeof("read_text") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("write_text", sizeof("write_text") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("len", sizeof("len") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("str", sizeof("str") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("sqrt", sizeof("sqrt") - 1)) == 0));
	s_string_release(v_name);
	return s_return_316;
}

s_record_111 s_resolve_hir_builtin_call(s_record_106 v_types, S_String * v_name, S_Array_i64 * v_args)
{
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("arguments", sizeof("arguments") - 1)) == 0))
	{
		s_record_107 v_array = s_intern_array_type(s_record_106_copy(v_types), 4);
		s_record_111 s_return_317 = s_hir_call_arguments(s_record_107_get_table_take(s_record_107_copy(v_array)), s_array_i64_retain(v_args), s_array_i64_make(0, NULL), s_record_107_get_id_take(s_record_107_copy(v_array)), s_string_from_bytes("arguments expects no arguments", sizeof("arguments expects no arguments") - 1));
		s_record_107_drop(v_array);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_317;
		s_record_107_drop(v_array);
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("read_text", sizeof("read_text") - 1)) == 0))
	{
		s_record_107 v_optional = s_intern_optional_type(s_record_106_copy(v_types), 4);
		s_record_111 s_return_318 = s_hir_call_arguments(s_record_107_get_table_take(s_record_107_copy(v_optional)), s_array_i64_retain(v_args), s_array_i64_make(1, (int64_t[]){4}), s_record_107_get_id_take(s_record_107_copy(v_optional)), s_string_from_bytes("read_text expects one string argument", sizeof("read_text expects one string argument") - 1));
		s_record_107_drop(v_optional);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_318;
		s_record_107_drop(v_optional);
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("write_text", sizeof("write_text") - 1)) == 0))
	{
		s_record_111 s_return_319 = s_hir_call_arguments(s_record_106_copy(v_types), s_array_i64_retain(v_args), s_array_i64_make(2, (int64_t[]){4, 4}), 3, s_string_from_bytes("write_text expects two string arguments", sizeof("write_text expects two string arguments") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_319;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("str", sizeof("str") - 1)) == 0))
	{
		if (((s_array_i64_length_take(s_array_i64_retain(v_args)) != 1) || (s_array_i64_get_checked_take(s_array_i64_retain(v_args), 0) != 1)))
		{
			s_record_111 s_return_320 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("str expects one i64 argument", sizeof("str expects one i64 argument") - 1));
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_106_drop(v_types);
			return s_return_320;
		}
		s_record_111 s_return_321 = s_HirCallCheck(s_record_106_copy(v_types), 4, s_string_from_bytes("", sizeof("") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_321;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("sqrt", sizeof("sqrt") - 1)) == 0))
	{
		s_record_111 s_return_322 = s_hir_call_arguments(s_record_106_copy(v_types), s_array_i64_retain(v_args), s_array_i64_make(1, (int64_t[]){2}), 2, s_string_from_bytes("sqrt expects one argument", sizeof("sqrt expects one argument") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_322;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0))
	{
		if ((s_array_i64_length_take(s_array_i64_retain(v_args)) != 1))
		{
			s_record_111 s_return_323 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("print expects one argument", sizeof("print expects one argument") - 1));
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_106_drop(v_types);
			return s_return_323;
		}
		const int64_t v_id = s_array_i64_get_checked_take(s_array_i64_retain(v_args), 0);
		if (((((v_id == 1) || (v_id == 2)) || (v_id == 3)) || (v_id == 4)))
		{
			s_record_111 s_return_324 = s_HirCallCheck(s_record_106_copy(v_types), 5, s_string_from_bytes("", sizeof("") - 1));
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_106_drop(v_types);
			return s_return_324;
		}
		s_record_111 s_return_325 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("cannot print this type", sizeof("cannot print this type") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_325;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("len", sizeof("len") - 1)) == 0))
	{
		if ((s_array_i64_length_take(s_array_i64_retain(v_args)) != 1))
		{
			s_record_111 s_return_326 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("len expects one argument", sizeof("len expects one argument") - 1));
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_106_drop(v_types);
			return s_return_326;
		}
		const int64_t v_id = s_array_i64_get_checked_take(s_array_i64_retain(v_args), 0);
		if (((v_id >= 0) && (v_id < s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
		{
			const int64_t v_tag = s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_id));
			if ((((v_tag == 4) || (v_tag == 7)) || (v_tag == 8)))
			{
				s_record_111 s_return_327 = s_HirCallCheck(s_record_106_copy(v_types), 1, s_string_from_bytes("", sizeof("") - 1));
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_106_drop(v_types);
				return s_return_327;
			}
		}
		s_record_111 s_return_328 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("len expects a string or array", sizeof("len expects a string or array") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_328;
	}
	s_record_111 s_return_329 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("unknown builtin", sizeof("unknown builtin") - 1));
	s_array_i64_release(v_args);
	s_string_release(v_name);
	s_record_106_drop(v_types);
	return s_return_329;
}

bool s_hir_is_string_method(S_String * v_name)
{
	bool s_return_330 = ((((((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("slice", sizeof("slice") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("find", sizeof("find") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("starts_with", sizeof("starts_with") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("ends_with", sizeof("ends_with") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("join", sizeof("join") - 1)) == 0));
	s_string_release(v_name);
	return s_return_330;
}

s_record_111 s_resolve_hir_string_call(s_record_106 v_types, int64_t v_receiver, S_String * v_method, S_Array_i64 * v_args)
{
	s_record_106 v_current = s_record_106_copy(v_types);
	s_record_111 v_check = s_hir_expected(s_record_106_copy(v_types), 4, v_receiver);
	if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_check)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_111 s_return_331 = s_record_111_copy(v_check);
		s_record_111_drop(v_check);
		s_record_106_drop(v_current);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_331;
	}
	S_Array_i64 * v_expected = s_array_i64_make(0, NULL);
	int64_t v_result = 3;
	if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0))
	{
		S_Array_i64 * s_value_332 = s_array_i64_make(1, (int64_t[]){1});
		s_array_i64_release(v_expected);
		v_expected = s_value_332;
	}
	else if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("slice", sizeof("slice") - 1)) == 0))
	{
		S_Array_i64 * s_value_333 = s_array_i64_make(2, (int64_t[]){1, 1});
		s_array_i64_release(v_expected);
		v_expected = s_value_333;
		v_result = 4;
	}
	else if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("find", sizeof("find") - 1)) == 0))
	{
		S_Array_i64 * s_value_334 = s_array_i64_make(1, (int64_t[]){4});
		s_array_i64_release(v_expected);
		v_expected = s_value_334;
	}
	else if (((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("starts_with", sizeof("starts_with") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("ends_with", sizeof("ends_with") - 1)) == 0)))
	{
		S_Array_i64 * s_value_335 = s_array_i64_make(1, (int64_t[]){4});
		s_array_i64_release(v_expected);
		v_expected = s_value_335;
	}
	else if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("join", sizeof("join") - 1)) == 0))
	{
		s_record_107 v_array = s_intern_array_type(s_record_106_copy(v_types), 4);
		s_record_106 s_value_336 = s_record_107_get_table_take(s_record_107_copy(v_array));
		s_record_106_drop(v_current);
		v_current = s_value_336;
		S_Array_i64 * s_value_337 = s_array_i64_make(1, (int64_t[]){s_record_107_get_id_take(s_record_107_copy(v_array))});
		s_array_i64_release(v_expected);
		v_expected = s_value_337;
		v_result = 4;
		s_record_107_drop(v_array);
	}
	if (((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("find", sizeof("find") - 1)) == 0)))
	{
		s_record_107 v_optional = s_intern_optional_type(s_record_106_copy(v_types), 1);
		s_record_106 s_value_338 = s_record_107_get_table_take(s_record_107_copy(v_optional));
		s_record_106_drop(v_current);
		v_current = s_value_338;
		v_result = s_record_107_get_id_take(s_record_107_copy(v_optional));
		s_record_107_drop(v_optional);
	}
	s_record_111 s_return_339 = s_hir_call_arguments(s_record_106_copy(v_current), s_array_i64_retain(v_args), s_array_i64_retain(v_expected), v_result, s_string_from_bytes("string method argument count mismatch", sizeof("string method argument count mismatch") - 1));
	s_array_i64_release(v_expected);
	s_record_111_drop(v_check);
	s_record_106_drop(v_current);
	s_array_i64_release(v_args);
	s_string_release(v_method);
	s_record_106_drop(v_types);
	return s_return_339;
	s_array_i64_release(v_expected);
	s_record_111_drop(v_check);
	s_record_106_drop(v_current);
}

s_record_111 s_resolve_hir_function_call(s_record_106 v_types, S_String * v_name, S_Array_i64 * v_args, S_Array_i64 * v_params, int64_t v_result)
{
	int64_t s_left_340 = s_array_i64_length_take(s_array_i64_retain(v_args));
	int64_t s_right_341 = s_array_i64_length_take(s_array_i64_retain(v_params));
	if ((s_left_340 != s_right_341))
	{
		s_record_111 s_return_342 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_concat_take(s_string_concat_take(s_string_from_bytes("argument count mismatch for '", sizeof("argument count mismatch for '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
		s_array_i64_release(v_params);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_342;
	}
	for (int64_t v_i = 0, s_bound_343 = s_array_i64_length_take(s_array_i64_retain(v_args)); v_i < s_bound_343; ++v_i)
	{
		if ((s_array_i64_get_checked_take(s_array_i64_retain(v_params), v_i) != 0))
		{
			s_record_111 v_check = s_hir_expected(s_record_106_copy(v_types), s_array_i64_get_checked_take(s_array_i64_retain(v_params), v_i), s_array_i64_get_checked_take(s_array_i64_retain(v_args), v_i));
			if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_check)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_111 s_return_344 = s_record_111_copy(v_check);
				s_record_111_drop(v_check);
				s_array_i64_release(v_params);
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_106_drop(v_types);
				return s_return_344;
			}
			s_record_111_drop(v_check);
		}
	}
	s_record_111 s_return_345 = s_HirCallCheck(s_record_106_copy(v_types), v_result, s_string_from_bytes("", sizeof("") - 1));
	s_array_i64_release(v_params);
	s_array_i64_release(v_args);
	s_string_release(v_name);
	s_record_106_drop(v_types);
	return s_return_345;
}

s_record_111 s_resolve_hir_collection_call(s_record_106 v_types, int64_t v_receiver, S_String * v_method, S_Array_i64 * v_args)
{
	if (((v_receiver < 0) || (v_receiver >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		s_record_111 s_return_346 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("unknown collection receiver type", sizeof("unknown collection receiver type") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_346;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_receiver);
	if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("append", sizeof("append") - 1)) == 0))
	{
		if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) != 7))
		{
			s_record_111 s_return_347 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("append requires an array", sizeof("append requires an array") - 1));
			s_record_105_drop(v_descriptor);
			s_array_i64_release(v_args);
			s_string_release(v_method);
			s_record_106_drop(v_types);
			return s_return_347;
		}
		s_record_111 s_return_348 = s_hir_call_arguments(s_record_106_copy(v_types), s_array_i64_retain(v_args), s_array_i64_make(1, (int64_t[]){s_record_105_get_first_take(s_record_105_copy(v_descriptor))}), v_receiver, s_string_from_bytes("append expects one argument", sizeof("append expects one argument") - 1));
		s_record_105_drop(v_descriptor);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_348;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) != 8))
	{
		s_record_111 s_return_349 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("dictionary method requires Dict", sizeof("dictionary method requires Dict") - 1));
		s_record_105_drop(v_descriptor);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_349;
	}
	if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("insert", sizeof("insert") - 1)) == 0))
	{
		s_record_111 s_return_350 = s_hir_call_arguments(s_record_106_copy(v_types), s_array_i64_retain(v_args), s_array_i64_make(2, (int64_t[]){s_record_105_get_first_take(s_record_105_copy(v_descriptor)), s_record_105_get_second_take(s_record_105_copy(v_descriptor))}), v_receiver, s_string_from_bytes("dictionary method argument count mismatch", sizeof("dictionary method argument count mismatch") - 1));
		s_record_105_drop(v_descriptor);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_350;
	}
	if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("get", sizeof("get") - 1)) == 0))
	{
		s_record_107 v_optional = s_intern_optional_type(s_record_106_copy(v_types), s_record_105_get_second_take(s_record_105_copy(v_descriptor)));
		s_record_111 s_return_351 = s_hir_call_arguments(s_record_107_get_table_take(s_record_107_copy(v_optional)), s_array_i64_retain(v_args), s_array_i64_make(1, (int64_t[]){s_record_105_get_first_take(s_record_105_copy(v_descriptor))}), s_record_107_get_id_take(s_record_107_copy(v_optional)), s_string_from_bytes("dictionary method argument count mismatch", sizeof("dictionary method argument count mismatch") - 1));
		s_record_107_drop(v_optional);
		s_record_105_drop(v_descriptor);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_351;
		s_record_107_drop(v_optional);
	}
	s_record_111 s_return_352 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("unknown collection method", sizeof("unknown collection method") - 1));
	s_record_105_drop(v_descriptor);
	s_array_i64_release(v_args);
	s_string_release(v_method);
	s_record_106_drop(v_types);
	return s_return_352;
	s_record_105_drop(v_descriptor);
}

bool s_is_optional(int64_t v_kind)
{
	return ((v_kind == 6) || ((v_kind >= 2000000) && (v_kind < (2000000 + 2000000))));
}

int64_t s_optional_value(int64_t v_kind)
{
	if ((v_kind == 6))
	{
		return 1;
	}
	if (s_is_optional(v_kind))
	{
		return (v_kind - 2000000);
	}
	return 0;
}

int64_t s_optional_of(int64_t v_kind)
{
	if ((v_kind == 1))
	{
		return 6;
	}
	if ((((v_kind == 0) || (v_kind == 5)) || s_is_optional(v_kind)))
	{
		return 0;
	}
	return (2000000 + v_kind);
}

S_String * s_optional_helper(int64_t v_kind)
{
	return s_string_concat_take(s_string_from_bytes("s_optional_", sizeof("s_optional_") - 1), s_string_from_i64(s_optional_value(v_kind)));
}

bool s_is_dict(int64_t v_kind)
{
	return ((v_kind >= 1000000) && (v_kind < 2000000));
}

int64_t s_dict_of(int64_t v_key, int64_t v_value)
{
	if (((((((v_key != 1) && (v_key != 4)) || (v_value == 0)) || (v_value == 5)) || (v_value == 6)) || (v_value >= 100000)))
	{
		return 0;
	}
	if ((v_key == 4))
	{
		return ((1000000 + (2 * v_value)) + 1);
	}
	return (1000000 + (2 * v_value));
}

int64_t s_dict_value(int64_t v_kind)
{
	if ((!s_is_dict(v_kind)))
	{
		return 0;
	}
	return ((v_kind - 1000000) / 2);
}

int64_t s_dict_key(int64_t v_kind)
{
	if ((!s_is_dict(v_kind)))
	{
		return 0;
	}
	if ((((v_kind - 1000000) % 2) == 1))
	{
		return 4;
	}
	return 1;
}

S_String * s_dict_helper(int64_t v_kind)
{
	return s_string_concat_take(s_string_from_bytes("s_d_", sizeof("s_d_") - 1), s_string_from_i64(v_kind));
}

int64_t s_array_element(int64_t v_kind)
{
	if ((v_kind == 7))
	{
		return 1;
	}
	if ((v_kind == 8))
	{
		return 4;
	}
	if (((v_kind >= 10000) && (v_kind < 1000000)))
	{
		return (v_kind - 10000);
	}
	return 0;
}

int64_t s_array_of(int64_t v_kind)
{
	if ((v_kind == 1))
	{
		return 7;
	}
	if ((v_kind == 4))
	{
		return 8;
	}
	if (((v_kind >= 100) && (v_kind < 10000)))
	{
		return (10000 + v_kind);
	}
	if ((((v_kind == 7) || (v_kind == 8)) || ((v_kind >= 10000) && (v_kind < 1000000))))
	{
		return (10000 + v_kind);
	}
	return 0;
}

bool s_managed(int64_t v_kind)
{
	return (((((v_kind == 4) || s_is_dict(v_kind)) || (s_array_element(v_kind) != 0)) || ((v_kind >= 100) && (v_kind < 10000))) || (s_is_optional(v_kind) && s_managed(s_optional_value(v_kind))));
}

bool s_generic_array(int64_t v_kind)
{
	return ((v_kind == 8) || ((v_kind >= 10000) && (v_kind < 1000000)));
}

S_String * s_array_helper(int64_t v_kind)
{
	return s_string_concat_take(s_string_from_bytes("s_ga_", sizeof("s_ga_") - 1), s_string_from_i64(v_kind));
}

s_record_104 s_ParameterInfo(S_String * v_name, int64_t v_kind)
{
	s_record_104 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.kind = v_kind;
	return s_constructor_result;
}

s_record_112 s_FunctionInfo(S_String * v_name, int64_t v_kind, S_Array * v_params, int64_t v_body, bool v_constructor, S_String * v_symbol)
{
	s_record_112 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.kind = v_kind;
	s_constructor_result.params = v_params;
	s_constructor_result.body = v_body;
	s_constructor_result.constructor = v_constructor;
	s_constructor_result.symbol = v_symbol;
	return s_constructor_result;
}

s_record_113 s_InstanceInfo(S_String * v_name, S_Array_i64 * v_kinds, S_Array_i64 * v_type_ids, int64_t v_kind, S_String * v_symbol, S_String * v_code, int64_t v_status)
{
	s_record_113 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.kinds = v_kinds;
	s_constructor_result.type_ids = v_type_ids;
	s_constructor_result.kind = v_kind;
	s_constructor_result.symbol = v_symbol;
	s_constructor_result.code = v_code;
	s_constructor_result.status = v_status;
	return s_constructor_result;
}

s_record_114 s_ConstantInfo(S_String * v_name, S_String * v_value)
{
	s_record_114 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.value = v_value;
	return s_constructor_result;
}

s_record_115 s_RecordInfo(S_Array * v_fields, int64_t v_canonical)
{
	s_record_115 s_constructor_result = {0};
	s_constructor_result.fields = v_fields;
	s_constructor_result.canonical = v_canonical;
	return s_constructor_result;
}

s_record_116 s_EmitState(s_record_102 v_tree)
{
	s_record_116 s_constructor_result = {0};
	s_constructor_result.tree = v_tree;
	s_constructor_result.functions = s_ga_10112_make_take(0, NULL);
	s_constructor_result.instances = s_ga_10113_make_take(0, NULL);
	s_constructor_result.active_params = s_ga_10104_make_take(0, NULL);
	s_constructor_result.symbols = s_ga_10110_make_take(0, NULL);
	s_constructor_result.constants = s_ga_10114_make_take(0, NULL);
	s_constructor_result.records = s_ga_10115_make_take(0, NULL);
	s_constructor_result.types = s_new_type_table();
	s_constructor_result.hir = s_ga_10108_make_take(0, NULL);
	s_constructor_result.dict_kinds = s_array_i64_make(0, NULL);
	s_constructor_result.lines = s_ga_8_make_take(0, NULL);
	s_constructor_result.indentation = 0;
	s_constructor_result.loop_depth = 0;
	s_constructor_result.loop_scope = 0;
	s_constructor_result.scope_depth = 0;
	s_constructor_result.return_kind = 0;
	s_constructor_result.function_name = s_string_from_bytes("", sizeof("") - 1);
	s_constructor_result.counter = 0;
	s_constructor_result.error = s_string_from_bytes("", sizeof("") - 1);
	s_constructor_result.error_line = 0;
	return s_constructor_result;
}

s_record_117 s_ExprCode(s_record_116 v_state, S_String * v_text, int64_t v_kind)
{
	s_record_117 s_constructor_result = {0};
	s_constructor_result.state = v_state;
	s_constructor_result.text = v_text;
	s_constructor_result.kind = v_kind;
	return s_constructor_result;
}

s_record_118 s_CompileResult(S_String * v_code, S_String * v_error, int64_t v_line)
{
	s_record_118 s_constructor_result = {0};
	s_constructor_result.code = v_code;
	s_constructor_result.error = v_error;
	s_constructor_result.line = v_line;
	return s_constructor_result;
}

s_record_101 s_ast_node(s_record_102 v_tree, int64_t v_id)
{
	s_record_101 s_return_353 = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_id);
	s_record_102_drop(v_tree);
	return s_return_353;
}

int64_t s_ast_child(s_record_102 v_tree, int64_t v_id, int64_t v_offset)
{
	s_record_101 v_entry = s_ast_node(s_record_102_copy(v_tree), v_id);
	int64_t s_return_354 = s_array_i64_get_checked_take(s_record_102_get_edges_take(s_record_102_copy(v_tree)), (s_record_101_get_first_take(s_record_101_copy(v_entry)) + v_offset));
	s_record_101_drop(v_entry);
	s_record_102_drop(v_tree);
	return s_return_354;
	s_record_101_drop(v_entry);
}

S_String * s_ast_text(s_record_102 v_tree, int64_t v_id)
{
	S_String * s_return_355 = s_token_text(s_record_102_get_source_take(s_record_102_copy(v_tree)), s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_tree)), s_record_101_get_token_take(s_ast_node(s_record_102_copy(v_tree), v_id))));
	s_record_102_drop(v_tree);
	return s_return_355;
}

int64_t s_ast_line(s_record_102 v_tree, int64_t v_id)
{
	int64_t s_return_356 = s_record_100_get_line_take(s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_tree)), s_record_101_get_token_take(s_ast_node(s_record_102_copy(v_tree), v_id))));
	s_record_102_drop(v_tree);
	return s_return_356;
}

s_record_116 s_emit_error(s_record_116 v_state, int64_t v_id, S_String * v_message)
{
	s_record_116 v_result = s_record_116_copy(v_state);
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		S_String * s_field_357 = s_string_retain(v_message);
		s_string_release(v_result.error);
		v_result.error = s_field_357;
		v_result.error_line = s_ast_line(s_record_116_get_tree_take(s_record_116_copy(v_result)), v_id);
	}
	s_record_116 s_return_358 = s_record_116_copy(v_result);
	s_record_116_drop(v_result);
	s_string_release(v_message);
	s_record_116_drop(v_state);
	return s_return_358;
	s_record_116_drop(v_result);
}

S_String * s_type_name(int64_t v_kind)
{
	if ((v_kind == 1))
	{
		return s_string_from_bytes("i64", sizeof("i64") - 1);
	}
	if ((v_kind == 2))
	{
		return s_string_from_bytes("f64", sizeof("f64") - 1);
	}
	if ((v_kind == 3))
	{
		return s_string_from_bytes("bool", sizeof("bool") - 1);
	}
	if ((v_kind == 4))
	{
		return s_string_from_bytes("string", sizeof("string") - 1);
	}
	if ((v_kind == 5))
	{
		return s_string_from_bytes("void", sizeof("void") - 1);
	}
	if (s_is_optional(v_kind))
	{
		return s_string_concat_take(s_string_concat_take(s_string_from_bytes("Optional[", sizeof("Optional[") - 1), s_type_name(s_optional_value(v_kind))), s_string_from_bytes("]", sizeof("]") - 1));
	}
	if (s_is_dict(v_kind))
	{
		S_String * s_left_359 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("Dict[", sizeof("Dict[") - 1), s_type_name(s_dict_key(v_kind))), s_string_from_bytes(",", sizeof(",") - 1));
		S_String * s_right_360 = s_type_name(s_dict_value(v_kind));
		return s_string_concat_take(s_string_concat_take(s_left_359, s_right_360), s_string_from_bytes("]", sizeof("]") - 1));
	}
	if ((v_kind == 7))
	{
		return s_string_from_bytes("Array[i64]", sizeof("Array[i64]") - 1);
	}
	if ((v_kind == 8))
	{
		return s_string_from_bytes("Array[string]", sizeof("Array[string]") - 1);
	}
	if ((v_kind >= 10000))
	{
		return s_string_from_bytes("Array[record]", sizeof("Array[record]") - 1);
	}
	if ((v_kind >= 100))
	{
		return s_string_from_bytes("record", sizeof("record") - 1);
	}
	return s_string_from_bytes("unknown", sizeof("unknown") - 1);
}

S_String * s_c_type(int64_t v_kind)
{
	if ((v_kind == 1))
	{
		return s_string_from_bytes("int64_t", sizeof("int64_t") - 1);
	}
	if ((v_kind == 2))
	{
		return s_string_from_bytes("double", sizeof("double") - 1);
	}
	if ((v_kind == 3))
	{
		return s_string_from_bytes("bool", sizeof("bool") - 1);
	}
	if ((v_kind == 4))
	{
		return s_string_from_bytes("S_String *", sizeof("S_String *") - 1);
	}
	if ((v_kind == 5))
	{
		return s_string_from_bytes("void", sizeof("void") - 1);
	}
	if ((v_kind == 6))
	{
		return s_string_from_bytes("S_Optional_i64", sizeof("S_Optional_i64") - 1);
	}
	if ((s_is_optional(v_kind) && (s_optional_value(v_kind) == 4)))
	{
		return s_string_from_bytes("S_Optional_string", sizeof("S_Optional_string") - 1);
	}
	if (s_is_optional(v_kind))
	{
		return s_optional_helper(v_kind);
	}
	if (s_is_dict(v_kind))
	{
		return s_string_from_bytes("S_Dict *", sizeof("S_Dict *") - 1);
	}
	if ((v_kind == 7))
	{
		return s_string_from_bytes("S_Array_i64 *", sizeof("S_Array_i64 *") - 1);
	}
	if (s_generic_array(v_kind))
	{
		return s_string_from_bytes("S_Array *", sizeof("S_Array *") - 1);
	}
	if ((v_kind >= 100))
	{
		return s_string_concat_take(s_string_from_bytes("s_record_", sizeof("s_record_") - 1), s_string_from_i64(v_kind));
	}
	return s_string_from_bytes("INVALID_TYPE", sizeof("INVALID_TYPE") - 1);
}

int64_t s_ast_type(s_record_102 v_tree, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_102_copy(v_tree), v_id);
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 38))
	{
		int64_t s_return_361 = 0;
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_361;
	}
	S_String * v_name = s_ast_text(s_record_102_copy(v_tree), v_id);
	if (((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Dict", sizeof("Dict") - 1)) == 0) && (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2)))
	{
		int64_t s_arg_362 = s_ast_type(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, 0));
		int64_t s_arg_363 = s_ast_type(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, 1));
		int64_t s_return_364 = s_dict_of(s_arg_362, s_arg_363);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_364;
	}
	if (((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Array", sizeof("Array") - 1)) == 0) && ((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 1) || (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2))))
	{
		if (((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2) && (s_string_compare_take(s_ast_text(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, 1)), s_string_from_bytes("1", sizeof("1") - 1)) != 0)))
		{
			int64_t s_return_365 = 0;
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_102_drop(v_tree);
			return s_return_365;
		}
		int64_t s_return_366 = s_array_of(s_ast_type(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, 0)));
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_366;
	}
	if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) != 0))
	{
		int64_t s_return_367 = 0;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_367;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("i64", sizeof("i64") - 1)) == 0))
	{
		int64_t s_return_368 = 1;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_368;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("f64", sizeof("f64") - 1)) == 0))
	{
		int64_t s_return_369 = 2;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_369;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("bool", sizeof("bool") - 1)) == 0))
	{
		int64_t s_return_370 = 3;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_370;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("string", sizeof("string") - 1)) == 0))
	{
		int64_t s_return_371 = 4;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_371;
	}
	int64_t s_return_372 = 0;
	s_string_release(v_name);
	s_record_101_drop(v_entry);
	s_record_102_drop(v_tree);
	return s_return_372;
	s_string_release(v_name);
	s_record_101_drop(v_entry);
}

int64_t s_resolve_type(s_record_116 v_state, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 38))
	{
		int64_t s_return_373 = 0;
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_373;
	}
	if (((s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("Dict", sizeof("Dict") - 1)) == 0) && (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2)))
	{
		int64_t s_arg_374 = s_resolve_type(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		int64_t s_arg_375 = s_resolve_type(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
		int64_t s_return_376 = s_dict_of(s_arg_374, s_arg_375);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_376;
	}
	if (((s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("Array", sizeof("Array") - 1)) == 0) && ((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 1) || (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2))))
	{
		if (((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2) && (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1)), s_string_from_bytes("1", sizeof("1") - 1)) != 0)))
		{
			int64_t s_return_377 = 0;
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_377;
		}
		int64_t s_return_378 = s_array_of(s_resolve_type(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0)));
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_378;
	}
	const int64_t v_primitive = s_ast_type(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if ((v_primitive != 0))
	{
		int64_t s_return_379 = v_primitive;
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_379;
	}
	if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 0))
	{
		const int64_t v_index = s_find_function(s_record_116_copy(v_state), s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id));
		if (((v_index >= 0) && s_record_112_get_constructor_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_index))))
		{
			int64_t s_return_380 = s_record_112_get_kind_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_index));
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_380;
		}
	}
	int64_t s_return_381 = 0;
	s_record_101_drop(v_entry);
	s_record_116_drop(v_state);
	return s_return_381;
	s_record_101_drop(v_entry);
}

int64_t s_array_constructor_kind(s_record_116 v_state, int64_t v_id)
{
	s_record_101 v_index = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_index)) != 23) || (s_record_101_get_count_take(s_record_101_copy(v_index)) != 2)))
	{
		int64_t s_return_382 = 0;
		s_record_101_drop(v_index);
		s_record_116_drop(v_state);
		return s_return_382;
	}
	const int64_t v_root = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
	if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root)) != 29) || (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root), s_string_from_bytes("Array", sizeof("Array") - 1)) != 0)))
	{
		int64_t s_return_383 = 0;
		s_record_101_drop(v_index);
		s_record_116_drop(v_state);
		return s_return_383;
	}
	const int64_t v_type_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1);
	if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_type_id)) == 23))
	{
		int64_t s_return_384 = s_array_of(s_array_constructor_kind(s_record_116_copy(v_state), v_type_id));
		s_record_101_drop(v_index);
		s_record_116_drop(v_state);
		return s_return_384;
	}
	if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_type_id)) != 29))
	{
		int64_t s_return_385 = 0;
		s_record_101_drop(v_index);
		s_record_116_drop(v_state);
		return s_return_385;
	}
	S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_type_id);
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("string", sizeof("string") - 1)) == 0))
	{
		int64_t s_return_386 = 8;
		s_string_release(v_name);
		s_record_101_drop(v_index);
		s_record_116_drop(v_state);
		return s_return_386;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("i64", sizeof("i64") - 1)) == 0))
	{
		int64_t s_return_387 = 7;
		s_string_release(v_name);
		s_record_101_drop(v_index);
		s_record_116_drop(v_state);
		return s_return_387;
	}
	const int64_t v_function = s_find_function(s_record_116_copy(v_state), s_string_retain(v_name));
	if (((v_function >= 0) && s_record_112_get_constructor_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_function))))
	{
		int64_t s_return_388 = s_array_of(s_record_112_get_kind_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_function)));
		s_string_release(v_name);
		s_record_101_drop(v_index);
		s_record_116_drop(v_state);
		return s_return_388;
	}
	int64_t s_return_389 = 0;
	s_string_release(v_name);
	s_record_101_drop(v_index);
	s_record_116_drop(v_state);
	return s_return_389;
	s_string_release(v_name);
	s_record_101_drop(v_index);
}

int64_t s_dict_constructor_kind(s_record_116 v_state, int64_t v_id)
{
	s_record_101 v_node = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_node)) != 23) || (s_record_101_get_count_take(s_record_101_copy(v_node)) != 3)))
	{
		int64_t s_return_390 = 0;
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_390;
	}
	const int64_t v_root = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
	if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root)) != 29) || (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root), s_string_from_bytes("Dict", sizeof("Dict") - 1)) != 0)))
	{
		int64_t s_return_391 = 0;
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_391;
	}
	S_String * v_key = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
	const int64_t v_value_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 2);
	S_String * v_value = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_value_id);
	int64_t v_key_kind = 0;
	int64_t v_value_kind = 0;
	if ((s_string_compare_take(s_string_retain(v_key), s_string_from_bytes("i64", sizeof("i64") - 1)) == 0))
	{
		v_key_kind = 1;
	}
	if ((s_string_compare_take(s_string_retain(v_key), s_string_from_bytes("string", sizeof("string") - 1)) == 0))
	{
		v_key_kind = 4;
	}
	if ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("i64", sizeof("i64") - 1)) == 0))
	{
		v_value_kind = 1;
	}
	if ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("string", sizeof("string") - 1)) == 0))
	{
		v_value_kind = 4;
	}
	if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_value_id)) == 23))
	{
		v_value_kind = s_array_constructor_kind(s_record_116_copy(v_state), v_value_id);
	}
	const int64_t v_function = s_find_function(s_record_116_copy(v_state), s_string_retain(v_value));
	if (((v_function >= 0) && s_record_112_get_constructor_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_function))))
	{
		v_value_kind = s_record_112_get_kind_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_function));
	}
	int64_t s_return_392 = s_dict_of(v_key_kind, v_value_kind);
	s_string_release(v_value);
	s_string_release(v_key);
	s_record_101_drop(v_node);
	s_record_116_drop(v_state);
	return s_return_392;
	s_string_release(v_value);
	s_string_release(v_key);
	s_record_101_drop(v_node);
}

s_record_116 s_register_dict(s_record_116 v_state, int64_t v_kind)
{
	s_record_116 v_next = s_record_116_copy(v_state);
	{
		S_Array_i64 * v_s_iter_393 = s_record_116_get_dict_kinds_take(s_record_116_copy(v_next));
		for (int64_t s_index_393 = 0; s_index_393 < (int64_t)v_s_iter_393->length; ++s_index_393)
		{
			int64_t v_existing = s_array_i64_get_checked(v_s_iter_393, s_index_393);
			if ((v_existing == v_kind))
			{
				s_record_116 s_return_394 = s_record_116_copy(v_next);
				s_array_i64_release(v_s_iter_393);
				s_record_116_drop(v_next);
				s_record_116_drop(v_state);
				return s_return_394;
			}
		}
		s_array_i64_release(v_s_iter_393);
	}
	S_Array_i64 * s_field_395 = s_array_i64_append_take(s_record_116_get_dict_kinds_take(s_record_116_copy(v_next)), v_kind);
	s_array_i64_release(v_next.dict_kinds);
	v_next.dict_kinds = s_field_395;
	s_record_116 s_return_396 = s_record_116_copy(v_next);
	s_record_116_drop(v_next);
	s_record_116_drop(v_state);
	return s_return_396;
	s_record_116_drop(v_next);
}

s_record_116 s_line(s_record_116 v_state, S_String * v_text)
{
	s_record_116 v_result = s_record_116_copy(v_state);
	S_String * v_prefix = s_string_from_bytes("", sizeof("") - 1);
	for (int64_t v_i = 0, s_bound_397 = s_record_116_get_indentation_take(s_record_116_copy(v_state)); v_i < s_bound_397; ++v_i)
	{
		S_String *s_value_398 = s_string_concat_take(s_string_retain(v_prefix), s_string_from_bytes("\t", sizeof("\t") - 1));
		s_string_release(v_prefix);
		v_prefix = s_value_398;
	}
	S_Array * s_field_399 = s_ga_8_append_take(s_record_116_get_lines_take(s_record_116_copy(v_result)), s_string_concat_take(s_string_retain(v_prefix), s_string_retain(v_text)));
	s_array_release(v_result.lines);
	v_result.lines = s_field_399;
	s_record_116 s_return_400 = s_record_116_copy(v_result);
	s_string_release(v_prefix);
	s_record_116_drop(v_result);
	s_string_release(v_text);
	s_record_116_drop(v_state);
	return s_return_400;
	s_string_release(v_prefix);
	s_record_116_drop(v_result);
}

int64_t s_find_symbol(s_record_116 v_state, S_String * v_name)
{
	int64_t s_return_401 = s_record_109_get_field_index_take(s_resolve_hir_name(s_record_116_get_symbols_take(s_record_116_copy(v_state)), s_string_retain(v_name)));
	s_string_release(v_name);
	s_record_116_drop(v_state);
	return s_return_401;
}

int64_t s_find_function(s_record_116 v_state, S_String * v_name)
{
	for (int64_t v_i = 0, s_bound_402 = s_generic_array_length_take(s_record_116_get_functions_take(s_record_116_copy(v_state))); v_i < s_bound_402; ++v_i)
	{
		if ((s_string_compare_take(s_record_112_get_name_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_i)), s_string_retain(v_name)) == 0))
		{
			int64_t s_return_403 = v_i;
			s_string_release(v_name);
			s_record_116_drop(v_state);
			return s_return_403;
		}
	}
	int64_t s_return_404 = (-1);
	s_string_release(v_name);
	s_record_116_drop(v_state);
	return s_return_404;
}

bool s_is_generic(s_record_112 v_info)
{
	if ((s_record_112_get_kind_take(s_record_112_copy(v_info)) == 0))
	{
		bool s_return_405 = true;
		s_record_112_drop(v_info);
		return s_return_405;
	}
	{
		S_Array * v_s_iter_406 = s_record_112_get_params_take(s_record_112_copy(v_info));
		for (int64_t s_index_406 = 0; s_index_406 < (int64_t)s_array_length(v_s_iter_406); ++s_index_406)
		{
			s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_406, s_index_406);
			if ((s_record_104_get_kind_take(s_record_104_copy(v_param)) == 0))
			{
				bool s_return_407 = true;
				s_record_104_drop(v_param);
				s_array_release(v_s_iter_406);
				s_record_112_drop(v_info);
				return s_return_407;
			}
			s_record_104_drop(v_param);
		}
		s_array_release(v_s_iter_406);
	}
	bool s_return_408 = false;
	s_record_112_drop(v_info);
	return s_return_408;
}

int64_t s_find_instance(s_record_116 v_state, S_String * v_name, S_Array_i64 * v_type_ids)
{
	for (int64_t v_i = 0, s_bound_409 = s_generic_array_length_take(s_record_116_get_instances_take(s_record_116_copy(v_state))); v_i < s_bound_409; ++v_i)
	{
		s_record_113 v_instance = s_ga_10113_get_checked_take(s_record_116_get_instances_take(s_record_116_copy(v_state)), v_i);
		int64_t s_left_410 = s_array_i64_length_take(s_record_113_get_type_ids_take(s_record_113_copy(v_instance)));
		int64_t s_right_411 = s_array_i64_length_take(s_array_i64_retain(v_type_ids));
		if (((s_string_compare_take(s_record_113_get_name_take(s_record_113_copy(v_instance)), s_string_retain(v_name)) == 0) && (s_left_410 == s_right_411)))
		{
			bool v_equal = true;
			for (int64_t v_j = 0, s_bound_412 = s_array_i64_length_take(s_array_i64_retain(v_type_ids)); v_j < s_bound_412; ++v_j)
			{
				if ((s_array_i64_get_checked_take(s_record_113_get_type_ids_take(s_record_113_copy(v_instance)), v_j) != s_array_i64_get_checked_take(s_array_i64_retain(v_type_ids), v_j)))
				{
					v_equal = false;
				}
			}
			if (v_equal)
			{
				int64_t s_return_413 = v_i;
				s_record_113_drop(v_instance);
				s_array_i64_release(v_type_ids);
				s_string_release(v_name);
				s_record_116_drop(v_state);
				return s_return_413;
			}
		}
		s_record_113_drop(v_instance);
	}
	int64_t s_return_414 = (-1);
	s_array_i64_release(v_type_ids);
	s_string_release(v_name);
	s_record_116_drop(v_state);
	return s_return_414;
}

S_String * s_find_constant(s_record_116 v_state, S_String * v_name)
{
	{
		S_Array * v_s_iter_415 = s_record_116_get_constants_take(s_record_116_copy(v_state));
		for (int64_t s_index_415 = 0; s_index_415 < (int64_t)s_array_length(v_s_iter_415); ++s_index_415)
		{
			s_record_114 v_item = s_ga_10114_get_borrow(v_s_iter_415, s_index_415);
			if ((s_string_compare_take(s_record_114_get_name_take(s_record_114_copy(v_item)), s_string_retain(v_name)) == 0))
			{
				S_String * s_return_416 = s_record_114_get_value_take(s_record_114_copy(v_item));
				s_record_114_drop(v_item);
				s_array_release(v_s_iter_415);
				s_string_release(v_name);
				s_record_116_drop(v_state);
				return s_return_416;
			}
			s_record_114_drop(v_item);
		}
		s_array_release(v_s_iter_415);
	}
	S_String * s_return_417 = s_string_from_bytes("", sizeof("") - 1);
	s_string_release(v_name);
	s_record_116_drop(v_state);
	return s_return_417;
}

int64_t s_record_field_index(s_record_116 v_state, int64_t v_kind, S_String * v_name)
{
	if ((((v_kind < 100) || (v_kind >= 10000)) || ((v_kind - 100) >= s_generic_array_length_take(s_record_116_get_records_take(s_record_116_copy(v_state))))))
	{
		int64_t s_return_418 = (-1);
		s_string_release(v_name);
		s_record_116_drop(v_state);
		return s_return_418;
	}
	const int64_t v_canonical = s_record_115_get_canonical_take(s_ga_10115_get_checked_take(s_record_116_get_records_take(s_record_116_copy(v_state)), (v_kind - 100)));
	S_Array * v_fields = s_record_105_get_fields_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_state))), v_canonical));
	for (int64_t v_i = 0, s_bound_419 = s_generic_array_length_take(s_array_retain(v_fields)); v_i < s_bound_419; ++v_i)
	{
		if ((s_string_compare_take(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_fields), v_i)), s_string_retain(v_name)) == 0))
		{
			int64_t s_return_420 = v_i;
			s_array_release(v_fields);
			s_string_release(v_name);
			s_record_116_drop(v_state);
			return s_return_420;
		}
	}
	int64_t s_return_421 = (-1);
	s_array_release(v_fields);
	s_string_release(v_name);
	s_record_116_drop(v_state);
	return s_return_421;
	s_array_release(v_fields);
}

s_record_107 s_canonical_kind(s_record_106 v_table, S_Array * v_records, int64_t v_kind)
{
	if (((v_kind >= 1) && (v_kind <= 5)))
	{
		s_record_107 s_return_422 = s_InternedType(s_record_106_copy(v_table), v_kind);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_422;
	}
	if (((v_kind >= 100) && (v_kind < 10000)))
	{
		const int64_t v_index = (v_kind - 100);
		if ((v_index < s_generic_array_length_take(s_array_retain(v_records))))
		{
			s_record_107 s_return_423 = s_InternedType(s_record_106_copy(v_table), s_record_115_get_canonical_take(s_ga_10115_get_checked_take(s_array_retain(v_records), v_index)));
			s_array_release(v_records);
			s_record_106_drop(v_table);
			return s_return_423;
		}
		s_record_107 s_return_424 = s_InternedType(s_record_106_copy(v_table), 0);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_424;
	}
	if (s_is_optional(v_kind))
	{
		s_record_107 v_payload = s_canonical_kind(s_record_106_copy(v_table), s_array_retain(v_records), s_optional_value(v_kind));
		s_record_107 s_return_425 = s_intern_optional_type(s_record_107_get_table_take(s_record_107_copy(v_payload)), s_record_107_get_id_take(s_record_107_copy(v_payload)));
		s_record_107_drop(v_payload);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_425;
		s_record_107_drop(v_payload);
	}
	if (s_is_dict(v_kind))
	{
		s_record_107 v_key = s_canonical_kind(s_record_106_copy(v_table), s_array_retain(v_records), s_dict_key(v_kind));
		s_record_107 v_value = s_canonical_kind(s_record_107_get_table_take(s_record_107_copy(v_key)), s_array_retain(v_records), s_dict_value(v_kind));
		s_record_107 s_return_426 = s_intern_dict_type(s_record_107_get_table_take(s_record_107_copy(v_value)), s_record_107_get_id_take(s_record_107_copy(v_key)), s_record_107_get_id_take(s_record_107_copy(v_value)));
		s_record_107_drop(v_value);
		s_record_107_drop(v_key);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_426;
		s_record_107_drop(v_value);
		s_record_107_drop(v_key);
	}
	const int64_t v_element = s_array_element(v_kind);
	if ((v_element != 0))
	{
		s_record_107 v_inner = s_canonical_kind(s_record_106_copy(v_table), s_array_retain(v_records), v_element);
		s_record_107 s_return_427 = s_intern_array_type(s_record_107_get_table_take(s_record_107_copy(v_inner)), s_record_107_get_id_take(s_record_107_copy(v_inner)));
		s_record_107_drop(v_inner);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_427;
		s_record_107_drop(v_inner);
	}
	s_record_107 s_return_428 = s_InternedType(s_record_106_copy(v_table), 0);
	s_array_release(v_records);
	s_record_106_drop(v_table);
	return s_return_428;
}

s_record_119 s_InternedSignature(s_record_106 v_table, S_Array_i64 * v_ids)
{
	s_record_119 s_constructor_result = {0};
	s_constructor_result.table = v_table;
	s_constructor_result.ids = v_ids;
	return s_constructor_result;
}

s_record_119 s_canonical_signature(s_record_116 v_state, S_Array_i64 * v_kinds)
{
	s_record_106 v_table = s_record_116_get_types_take(s_record_116_copy(v_state));
	S_Array_i64 * v_ids = s_array_i64_make(0, NULL);
	{
		S_Array_i64 * v_s_iter_429 = s_array_i64_retain(v_kinds);
		for (int64_t s_index_429 = 0; s_index_429 < (int64_t)v_s_iter_429->length; ++s_index_429)
		{
			int64_t v_kind = s_array_i64_get_checked(v_s_iter_429, s_index_429);
			s_record_107 v_entry = s_canonical_kind(s_record_106_copy(v_table), s_record_116_get_records_take(s_record_116_copy(v_state)), v_kind);
			s_record_106 s_value_430 = s_record_107_get_table_take(s_record_107_copy(v_entry));
			s_record_106_drop(v_table);
			v_table = s_value_430;
			int64_t s_item_431 = s_record_107_get_id_take(s_record_107_copy(v_entry));
			v_ids = s_array_i64_append_take(v_ids, s_item_431);
			s_record_107_drop(v_entry);
		}
		s_array_i64_release(v_s_iter_429);
	}
	s_record_119 s_return_432 = s_InternedSignature(s_record_106_copy(v_table), s_array_i64_retain(v_ids));
	s_array_i64_release(v_ids);
	s_record_106_drop(v_table);
	s_array_i64_release(v_kinds);
	s_record_116_drop(v_state);
	return s_return_432;
	s_array_i64_release(v_ids);
	s_record_106_drop(v_table);
}

s_record_117 s_intern_record(s_record_116 v_state, S_Array * v_fields)
{
	S_Array * v_ordered = s_ordered_fields(s_array_retain(v_fields));
	s_record_116 v_next = s_record_116_copy(v_state);
	S_Array * v_canonical_fields = s_ga_10104_make_take(0, NULL);
	{
		S_Array * v_s_iter_433 = s_array_retain(v_ordered);
		for (int64_t s_index_433 = 0; s_index_433 < (int64_t)s_array_length(v_s_iter_433); ++s_index_433)
		{
			s_record_104 v_field = s_ga_10104_get_borrow(v_s_iter_433, s_index_433);
			s_record_107 v_lookup = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_next)), s_record_116_get_records_take(s_record_116_copy(v_next)), s_record_104_get_kind_take(s_record_104_copy(v_field)));
			if ((s_record_107_get_id_take(s_record_107_copy(v_lookup)) == 0))
			{
				s_record_117 s_return_434 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_107_drop(v_lookup);
				s_record_104_drop(v_field);
				s_array_release(v_s_iter_433);
				s_array_release(v_canonical_fields);
				s_record_116_drop(v_next);
				s_array_release(v_ordered);
				s_array_release(v_fields);
				s_record_116_drop(v_state);
				return s_return_434;
			}
			s_record_106 s_field_435 = s_record_107_get_table_take(s_record_107_copy(v_lookup));
			s_record_106_drop(v_next.types);
			v_next.types = s_field_435;
			s_record_104 s_item_436 = s_RecordField(s_record_104_get_name_take(s_record_104_copy(v_field)), s_record_107_get_id_take(s_record_107_copy(v_lookup)));
			v_canonical_fields = s_ga_10104_append_take(v_canonical_fields, s_item_436);
			s_record_107_drop(v_lookup);
			s_record_104_drop(v_field);
		}
		s_array_release(v_s_iter_433);
	}
	s_record_107 v_entry = s_intern_record_type(s_record_116_get_types_take(s_record_116_copy(v_next)), s_array_retain(v_canonical_fields));
	s_record_106 s_field_437 = s_record_107_get_table_take(s_record_107_copy(v_entry));
	s_record_106_drop(v_next.types);
	v_next.types = s_field_437;
	for (int64_t v_i = 0, s_bound_438 = s_generic_array_length_take(s_record_116_get_records_take(s_record_116_copy(v_next))); v_i < s_bound_438; ++v_i)
	{
		if ((s_record_115_get_canonical_take(s_ga_10115_get_checked_take(s_record_116_get_records_take(s_record_116_copy(v_next)), v_i)) == s_record_107_get_id_take(s_record_107_copy(v_entry))))
		{
			s_record_117 s_return_439 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), (100 + v_i));
			s_record_107_drop(v_entry);
			s_array_release(v_canonical_fields);
			s_record_116_drop(v_next);
			s_array_release(v_ordered);
			s_array_release(v_fields);
			s_record_116_drop(v_state);
			return s_return_439;
		}
	}
	const int64_t v_kind = (100 + s_generic_array_length_take(s_record_116_get_records_take(s_record_116_copy(v_next))));
	S_Array * s_field_440 = s_ga_10115_append_take(s_record_116_get_records_take(s_record_116_copy(v_next)), s_RecordInfo(s_array_retain(v_ordered), s_record_107_get_id_take(s_record_107_copy(v_entry))));
	s_array_release(v_next.records);
	v_next.records = s_field_440;
	s_record_117 s_return_441 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), v_kind);
	s_record_107_drop(v_entry);
	s_array_release(v_canonical_fields);
	s_record_116_drop(v_next);
	s_array_release(v_ordered);
	s_array_release(v_fields);
	s_record_116_drop(v_state);
	return s_return_441;
	s_record_107_drop(v_entry);
	s_array_release(v_canonical_fields);
	s_record_116_drop(v_next);
	s_array_release(v_ordered);
}

s_record_120 s_CTypeABI(int64_t v_type_id, S_String * v_c_name, S_String * v_copy, S_String * v_drop)
{
	s_record_120 s_constructor_result = {0};
	s_constructor_result.type_id = v_type_id;
	s_constructor_result.c_name = v_c_name;
	s_constructor_result.copy = v_copy;
	s_constructor_result.drop = v_drop;
	return s_constructor_result;
}

s_record_120 s_c_abi(s_record_116 v_state, int64_t v_kind)
{
	s_record_107 v_interned = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_state)), s_record_116_get_records_take(s_record_116_copy(v_state)), v_kind);
	const int64_t v_id = s_record_107_get_id_take(s_record_107_copy(v_interned));
	if ((v_id == 0))
	{
		s_record_120 s_return_442 = s_CTypeABI(v_id, s_string_from_bytes("INVALID_TYPE", sizeof("INVALID_TYPE") - 1), s_string_from_bytes("", sizeof("") - 1), s_string_from_bytes("", sizeof("") - 1));
		s_record_107_drop(v_interned);
		s_record_116_drop(v_state);
		return s_return_442;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_107_get_table_take(s_record_107_copy(v_interned))), v_id);
	S_String * v_name = s_c_type(v_kind);
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 4))
	{
		s_record_120 s_return_443 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("s_string_retain", sizeof("s_string_retain") - 1), s_string_from_bytes("s_string_release", sizeof("s_string_release") - 1));
		s_string_release(v_name);
		s_record_105_drop(v_descriptor);
		s_record_107_drop(v_interned);
		s_record_116_drop(v_state);
		return s_return_443;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 6))
	{
		s_record_120 s_return_444 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_concat_take(s_string_retain(v_name), s_string_from_bytes("_copy", sizeof("_copy") - 1)), s_string_concat_take(s_string_retain(v_name), s_string_from_bytes("_drop", sizeof("_drop") - 1)));
		s_string_release(v_name);
		s_record_105_drop(v_descriptor);
		s_record_107_drop(v_interned);
		s_record_116_drop(v_state);
		return s_return_444;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 7))
	{
		if ((s_record_105_get_first_take(s_record_105_copy(v_descriptor)) == 1))
		{
			s_record_120 s_return_445 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("s_array_i64_retain", sizeof("s_array_i64_retain") - 1), s_string_from_bytes("s_array_i64_release", sizeof("s_array_i64_release") - 1));
			s_string_release(v_name);
			s_record_105_drop(v_descriptor);
			s_record_107_drop(v_interned);
			s_record_116_drop(v_state);
			return s_return_445;
		}
		s_record_120 s_return_446 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("s_array_retain", sizeof("s_array_retain") - 1), s_string_from_bytes("s_array_release", sizeof("s_array_release") - 1));
		s_string_release(v_name);
		s_record_105_drop(v_descriptor);
		s_record_107_drop(v_interned);
		s_record_116_drop(v_state);
		return s_return_446;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 8))
	{
		s_record_120 s_return_447 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("s_dict_retain", sizeof("s_dict_retain") - 1), s_string_from_bytes("s_dict_release", sizeof("s_dict_release") - 1));
		s_string_release(v_name);
		s_record_105_drop(v_descriptor);
		s_record_107_drop(v_interned);
		s_record_116_drop(v_state);
		return s_return_447;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 9))
	{
		s_record_105 v_payload = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_107_get_table_take(s_record_107_copy(v_interned))), s_record_105_get_first_take(s_record_105_copy(v_descriptor)));
		if ((((((s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 4) || (s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 6)) || (s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 7)) || (s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 8)) || (s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 9)))
		{
			S_String * v_helper = s_optional_helper(v_kind);
			s_record_120 s_return_448 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_concat_take(s_string_retain(v_helper), s_string_from_bytes("_copy", sizeof("_copy") - 1)), s_string_concat_take(s_string_retain(v_helper), s_string_from_bytes("_drop", sizeof("_drop") - 1)));
			s_string_release(v_helper);
			s_record_105_drop(v_payload);
			s_string_release(v_name);
			s_record_105_drop(v_descriptor);
			s_record_107_drop(v_interned);
			s_record_116_drop(v_state);
			return s_return_448;
			s_string_release(v_helper);
		}
		s_record_105_drop(v_payload);
	}
	s_record_120 s_return_449 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("", sizeof("") - 1), s_string_from_bytes("", sizeof("") - 1));
	s_string_release(v_name);
	s_record_105_drop(v_descriptor);
	s_record_107_drop(v_interned);
	s_record_116_drop(v_state);
	return s_return_449;
	s_string_release(v_name);
	s_record_105_drop(v_descriptor);
	s_record_107_drop(v_interned);
}

S_String * s_retain_function(int64_t v_kind)
{
	if ((v_kind == 4))
	{
		return s_string_from_bytes("s_string_retain", sizeof("s_string_retain") - 1);
	}
	if ((v_kind == 7))
	{
		return s_string_from_bytes("s_array_i64_retain", sizeof("s_array_i64_retain") - 1);
	}
	if (s_generic_array(v_kind))
	{
		return s_string_from_bytes("s_array_retain", sizeof("s_array_retain") - 1);
	}
	if (s_is_dict(v_kind))
	{
		return s_string_from_bytes("s_dict_retain", sizeof("s_dict_retain") - 1);
	}
	if ((s_is_optional(v_kind) && s_managed(s_optional_value(v_kind))))
	{
		return s_string_concat_take(s_optional_helper(v_kind), s_string_from_bytes("_copy", sizeof("_copy") - 1));
	}
	if (((v_kind >= 100) && (v_kind < 10000)))
	{
		return s_string_concat_take(s_c_type(v_kind), s_string_from_bytes("_copy", sizeof("_copy") - 1));
	}
	return s_string_from_bytes("", sizeof("") - 1);
}

S_String * s_release_function(int64_t v_kind)
{
	if ((v_kind == 4))
	{
		return s_string_from_bytes("s_string_release", sizeof("s_string_release") - 1);
	}
	if ((v_kind == 7))
	{
		return s_string_from_bytes("s_array_i64_release", sizeof("s_array_i64_release") - 1);
	}
	if (s_generic_array(v_kind))
	{
		return s_string_from_bytes("s_array_release", sizeof("s_array_release") - 1);
	}
	if (s_is_dict(v_kind))
	{
		return s_string_from_bytes("s_dict_release", sizeof("s_dict_release") - 1);
	}
	if ((s_is_optional(v_kind) && s_managed(s_optional_value(v_kind))))
	{
		return s_string_concat_take(s_optional_helper(v_kind), s_string_from_bytes("_drop", sizeof("_drop") - 1));
	}
	if (((v_kind >= 100) && (v_kind < 10000)))
	{
		return s_string_concat_take(s_c_type(v_kind), s_string_from_bytes("_drop", sizeof("_drop") - 1));
	}
	return s_string_from_bytes("", sizeof("") - 1);
}

s_record_121 s_CFieldABI(S_String * v_name, int64_t v_type_id, s_record_120 v_abi)
{
	s_record_121 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.type_id = v_type_id;
	s_constructor_result.abi = v_abi;
	return s_constructor_result;
}

S_Array * s_record_c_fields(s_record_116 v_state, int64_t v_record_index)
{
	s_record_115 v_record = s_ga_10115_get_checked_take(s_record_116_get_records_take(s_record_116_copy(v_state)), v_record_index);
	S_Array * v_canonical = s_record_105_get_fields_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_state))), s_record_115_get_canonical_take(s_record_115_copy(v_record))));
	S_Array * v_result = s_ga_10121_make_take(0, NULL);
	for (int64_t v_i = 0, s_bound_450 = s_generic_array_length_take(s_array_retain(v_canonical)); v_i < s_bound_450; ++v_i)
	{
		s_record_104 v_field = s_ga_10104_get_checked_take(s_array_retain(v_canonical), v_i);
		s_record_104 v_legacy = s_ga_10104_get_checked_take(s_record_115_get_fields_take(s_record_115_copy(v_record)), v_i);
		s_record_121 s_item_451 = s_CFieldABI(s_record_104_get_name_take(s_record_104_copy(v_field)), s_record_104_get_kind_take(s_record_104_copy(v_field)), s_c_abi(s_record_116_copy(v_state), s_record_104_get_kind_take(s_record_104_copy(v_legacy))));
		v_result = s_ga_10121_append_take(v_result, s_item_451);
		s_record_104_drop(v_legacy);
		s_record_104_drop(v_field);
	}
	S_Array * s_return_452 = s_array_retain(v_result);
	s_array_release(v_result);
	s_array_release(v_canonical);
	s_record_115_drop(v_record);
	s_record_116_drop(v_state);
	return s_return_452;
	s_array_release(v_result);
	s_array_release(v_canonical);
	s_record_115_drop(v_record);
}

S_String * s_record_copy(int64_t v_kind, S_String * v_expr)
{
	S_String * v_function = s_retain_function(v_kind);
	if ((s_string_compare_take(s_string_retain(v_function), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		S_String * s_return_453 = s_string_retain(v_expr);
		s_string_release(v_function);
		s_string_release(v_expr);
		return s_return_453;
	}
	S_String * s_return_454 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_function), s_string_from_bytes("(", sizeof("(") - 1)), s_string_retain(v_expr)), s_string_from_bytes(")", sizeof(")") - 1));
	s_string_release(v_function);
	s_string_release(v_expr);
	return s_return_454;
	s_string_release(v_function);
}

S_String * s_record_drop(int64_t v_kind, S_String * v_expr)
{
	S_String * v_function = s_release_function(v_kind);
	if ((s_string_compare_take(s_string_retain(v_function), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		S_String * s_return_455 = s_string_from_bytes("", sizeof("") - 1);
		s_string_release(v_function);
		s_string_release(v_expr);
		return s_return_455;
	}
	S_String * s_return_456 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_function), s_string_from_bytes("(", sizeof("(") - 1)), s_string_retain(v_expr)), s_string_from_bytes(");", sizeof(");") - 1));
	s_string_release(v_function);
	s_string_release(v_expr);
	return s_return_456;
	s_string_release(v_function);
}

bool s_plain_record(s_record_116 v_state, int64_t v_kind)
{
	if (((v_kind < 100) || (v_kind >= 10000)))
	{
		bool s_return_457 = (!s_managed(v_kind));
		s_record_116_drop(v_state);
		return s_return_457;
	}
	{
		S_Array * v_s_iter_458 = s_record_115_get_fields_take(s_ga_10115_get_checked_take(s_record_116_get_records_take(s_record_116_copy(v_state)), (v_kind - 100)));
		for (int64_t s_index_458 = 0; s_index_458 < (int64_t)s_array_length(v_s_iter_458); ++s_index_458)
		{
			s_record_104 v_field = s_ga_10104_get_borrow(v_s_iter_458, s_index_458);
			if ((!s_plain_record(s_record_116_copy(v_state), s_record_104_get_kind_take(s_record_104_copy(v_field)))))
			{
				bool s_return_459 = false;
				s_record_104_drop(v_field);
				s_array_release(v_s_iter_458);
				s_record_116_drop(v_state);
				return s_return_459;
			}
			s_record_104_drop(v_field);
		}
		s_array_release(v_s_iter_458);
	}
	bool s_return_460 = true;
	s_record_116_drop(v_state);
	return s_return_460;
}

S_String * s_record_declarations(s_record_116 v_state)
{
	S_String * v_result = s_string_from_bytes("", sizeof("") - 1);
	for (int64_t v_i = 0, s_bound_461 = s_generic_array_length_take(s_record_116_get_records_take(s_record_116_copy(v_state))); v_i < s_bound_461; ++v_i)
	{
		const int64_t v_kind = (100 + v_i);
		S_String * v_type = s_record_120_get_c_name_take(s_c_abi(s_record_116_copy(v_state), v_kind));
		S_Array * v_fields = s_record_c_fields(s_record_116_copy(v_state), v_i);
		S_String *s_value_462 = s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("typedef struct {\n", sizeof("typedef struct {\n") - 1));
		s_string_release(v_result);
		v_result = s_value_462;
		{
			S_Array * v_s_iter_463 = s_array_retain(v_fields);
			for (int64_t s_index_463 = 0; s_index_463 < (int64_t)s_array_length(v_s_iter_463); ++s_index_463)
			{
				s_record_121 v_field = s_ga_10121_get_borrow(v_s_iter_463, s_index_463);
				S_String *s_value_464 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_120_get_c_name_take(s_record_121_get_abi_take(s_record_121_copy(v_field)))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_121_get_name_take(s_record_121_copy(v_field))), s_string_from_bytes(";\n", sizeof(";\n") - 1));
				s_string_release(v_result);
				v_result = s_value_464;
				s_record_121_drop(v_field);
			}
			s_array_release(v_s_iter_463);
		}
		S_String *s_value_465 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("} ", sizeof("} ") - 1)), s_string_retain(v_type)), s_string_from_bytes(";\n", sizeof(";\n") - 1));
		s_string_release(v_result);
		v_result = s_value_465;
		S_String *s_value_466 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_type)), s_string_from_bytes("_copy(", sizeof("_copy(") - 1)), s_string_retain(v_type)), s_string_from_bytes(" value) {\n", sizeof(" value) {\n") - 1));
		s_string_release(v_result);
		v_result = s_value_466;
		{
			S_Array * v_s_iter_467 = s_array_retain(v_fields);
			for (int64_t s_index_467 = 0; s_index_467 < (int64_t)s_array_length(v_s_iter_467); ++s_index_467)
			{
				s_record_121 v_field = s_ga_10121_get_borrow(v_s_iter_467, s_index_467);
				if ((s_string_compare_take(s_record_120_get_copy_take(s_record_121_get_abi_take(s_record_121_copy(v_field))), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					S_String *s_value_468 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\tvalue.", sizeof("\tvalue.") - 1)), s_record_121_get_name_take(s_record_121_copy(v_field))), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_120_get_copy_take(s_record_121_get_abi_take(s_record_121_copy(v_field)))), s_string_from_bytes("(value.", sizeof("(value.") - 1)), s_record_121_get_name_take(s_record_121_copy(v_field))), s_string_from_bytes(");\n", sizeof(");\n") - 1));
					s_string_release(v_result);
					v_result = s_value_468;
				}
				s_record_121_drop(v_field);
			}
			s_array_release(v_s_iter_467);
		}
		S_String *s_value_469 = s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\treturn value;\n}\n", sizeof("\treturn value;\n}\n") - 1));
		s_string_release(v_result);
		v_result = s_value_469;
		S_String *s_value_470 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_type)), s_string_from_bytes("_drop(", sizeof("_drop(") - 1)), s_string_retain(v_type)), s_string_from_bytes(" value) {\n", sizeof(" value) {\n") - 1));
		s_string_release(v_result);
		v_result = s_value_470;
		{
			S_Array * v_s_iter_471 = s_array_retain(v_fields);
			for (int64_t s_index_471 = 0; s_index_471 < (int64_t)s_array_length(v_s_iter_471); ++s_index_471)
			{
				s_record_121 v_field = s_ga_10121_get_borrow(v_s_iter_471, s_index_471);
				if ((s_string_compare_take(s_record_120_get_drop_take(s_record_121_get_abi_take(s_record_121_copy(v_field))), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					S_String *s_value_472 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_120_get_drop_take(s_record_121_get_abi_take(s_record_121_copy(v_field)))), s_string_from_bytes("(value.", sizeof("(value.") - 1)), s_record_121_get_name_take(s_record_121_copy(v_field))), s_string_from_bytes(");\n", sizeof(");\n") - 1));
					s_string_release(v_result);
					v_result = s_value_472;
				}
				s_record_121_drop(v_field);
			}
			s_array_release(v_s_iter_471);
		}
		S_String *s_value_473 = s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t(void)value;\n}\n", sizeof("\t(void)value;\n}\n") - 1));
		s_string_release(v_result);
		v_result = s_value_473;
		{
			S_Array * v_s_iter_474 = s_array_retain(v_fields);
			for (int64_t s_index_474 = 0; s_index_474 < (int64_t)s_array_length(v_s_iter_474); ++s_index_474)
			{
				s_record_121 v_field = s_ga_10121_get_borrow(v_s_iter_474, s_index_474);
				S_String *s_value_475 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_record_120_get_c_name_take(s_record_121_get_abi_take(s_record_121_copy(v_field)))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_type)), s_string_from_bytes("_get_", sizeof("_get_") - 1)), s_record_121_get_name_take(s_record_121_copy(v_field))), s_string_from_bytes("_take(", sizeof("_take(") - 1)), s_string_retain(v_type)), s_string_from_bytes(" owned) {\n", sizeof(" owned) {\n") - 1));
				s_string_release(v_result);
				v_result = s_value_475;
				S_String * v_copied = s_string_concat_take(s_string_from_bytes("owned.", sizeof("owned.") - 1), s_record_121_get_name_take(s_record_121_copy(v_field)));
				if ((s_string_compare_take(s_record_120_get_copy_take(s_record_121_get_abi_take(s_record_121_copy(v_field))), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					S_String *s_value_476 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_record_120_get_copy_take(s_record_121_get_abi_take(s_record_121_copy(v_field))), s_string_from_bytes("(", sizeof("(") - 1)), s_string_retain(v_copied)), s_string_from_bytes(")", sizeof(")") - 1));
					s_string_release(v_copied);
					v_copied = s_value_476;
				}
				S_String *s_value_477 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_120_get_c_name_take(s_record_121_get_abi_take(s_record_121_copy(v_field)))), s_string_from_bytes(" field = ", sizeof(" field = ") - 1)), s_string_retain(v_copied)), s_string_from_bytes(";\n", sizeof(";\n") - 1));
				s_string_release(v_result);
				v_result = s_value_477;
				S_String *s_value_478 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes("_drop(owned);\n\treturn field;\n}\n", sizeof("_drop(owned);\n\treturn field;\n}\n") - 1));
				s_string_release(v_result);
				v_result = s_value_478;
				s_string_release(v_copied);
				s_record_121_drop(v_field);
			}
			s_array_release(v_s_iter_474);
		}
		s_array_release(v_fields);
		s_string_release(v_type);
	}
	S_String * s_return_479 = s_string_retain(v_result);
	s_string_release(v_result);
	s_record_116_drop(v_state);
	return s_return_479;
	s_string_release(v_result);
}

S_String * s_optional_declarations(s_record_116 v_state)
{
	S_String * v_output = s_string_from_bytes("", sizeof("") - 1);
	S_Array_i64 * v_seen = s_array_i64_make(0, NULL);
	S_Array_i64 * v_values = s_array_i64_make(0, NULL);
	int64_t s_item_480 = 4;
	v_values = s_array_i64_append_take(v_values, s_item_480);
	{
		S_Array_i64 * v_s_iter_481 = s_record_116_get_dict_kinds_take(s_record_116_copy(v_state));
		for (int64_t s_index_481 = 0; s_index_481 < (int64_t)v_s_iter_481->length; ++s_index_481)
		{
			int64_t v_dict_kind = s_array_i64_get_checked(v_s_iter_481, s_index_481);
			int64_t s_item_482 = s_dict_value(v_dict_kind);
			v_values = s_array_i64_append_take(v_values, s_item_482);
		}
		s_array_i64_release(v_s_iter_481);
	}
	{
		S_Array_i64 * v_s_iter_483 = s_array_i64_retain(v_values);
		for (int64_t s_index_483 = 0; s_index_483 < (int64_t)v_s_iter_483->length; ++s_index_483)
		{
			int64_t v_value = s_array_i64_get_checked(v_s_iter_483, s_index_483);
			if ((v_value == 1))
			{
				continue;
			}
			bool v_duplicate = false;
			{
				S_Array_i64 * v_s_iter_484 = s_array_i64_retain(v_seen);
				for (int64_t s_index_484 = 0; s_index_484 < (int64_t)v_s_iter_484->length; ++s_index_484)
				{
					int64_t v_previous = s_array_i64_get_checked(v_s_iter_484, s_index_484);
					if ((v_previous == v_value))
					{
						v_duplicate = true;
					}
				}
				s_array_i64_release(v_s_iter_484);
			}
			if (v_duplicate)
			{
				continue;
			}
			int64_t s_item_485 = v_value;
			v_seen = s_array_i64_append_take(v_seen, s_item_485);
			const int64_t v_kind = s_optional_of(v_value);
			S_String * v_name = s_optional_helper(v_kind);
			if ((v_value == 4))
			{
				S_String *s_value_486 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("typedef S_Optional_string ", sizeof("typedef S_Optional_string ") - 1)), s_string_retain(v_name)), s_string_from_bytes(";\n", sizeof(";\n") - 1));
				s_string_release(v_output);
				v_output = s_value_486;
			}
			else
			{
				S_String *s_value_487 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("typedef struct { bool has_value; ", sizeof("typedef struct { bool has_value; ") - 1)), s_c_type(v_value)), s_string_from_bytes(" value; } ", sizeof(" value; } ") - 1)), s_string_retain(v_name)), s_string_from_bytes(";\n", sizeof(";\n") - 1));
				s_string_release(v_output);
				v_output = s_value_487;
			}
			s_record_120 v_abi = s_c_abi(s_record_116_copy(v_state), v_value);
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_488 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_name)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_name)), s_string_from_bytes("_copy(", sizeof("_copy(") - 1)), s_string_retain(v_name)), s_string_from_bytes(" owned) {\n", sizeof(" owned) {\n") - 1));
				s_string_release(v_output);
				v_output = s_value_488;
				S_String *s_value_489 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (owned.has_value) owned.value = ", sizeof("\tif (owned.has_value) owned.value = ") - 1)), s_record_copy(v_value, s_string_from_bytes("owned.value", sizeof("owned.value") - 1))), s_string_from_bytes(";\n\treturn owned;\n}\n", sizeof(";\n\treturn owned;\n}\n") - 1));
				s_string_release(v_output);
				v_output = s_value_489;
				S_String *s_value_490 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_name)), s_string_from_bytes("_drop(", sizeof("_drop(") - 1)), s_string_retain(v_name)), s_string_from_bytes(" owned) {\n", sizeof(" owned) {\n") - 1));
				s_string_release(v_output);
				v_output = s_value_490;
				S_String *s_value_491 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (owned.has_value) ", sizeof("\tif (owned.has_value) ") - 1)), s_record_drop(v_value, s_string_from_bytes("owned.value", sizeof("owned.value") - 1))), s_string_from_bytes("\n}\n", sizeof("\n}\n") - 1));
				s_string_release(v_output);
				v_output = s_value_491;
			}
			s_record_120_drop(v_abi);
			s_string_release(v_name);
		}
		s_array_i64_release(v_s_iter_483);
	}
	S_String * s_return_492 = s_string_retain(v_output);
	s_array_i64_release(v_values);
	s_array_i64_release(v_seen);
	s_string_release(v_output);
	s_record_116_drop(v_state);
	return s_return_492;
	s_array_i64_release(v_values);
	s_array_i64_release(v_seen);
	s_string_release(v_output);
}

S_String * s_generic_array_declarations(s_record_116 v_state)
{
	S_String * v_output = s_string_from_bytes("", sizeof("") - 1);
	S_Array_i64 * v_elements = s_array_i64_make(0, NULL);
	int64_t s_item_493 = 4;
	v_elements = s_array_i64_append_take(v_elements, s_item_493);
	for (int64_t v_i = 0, s_bound_494 = s_generic_array_length_take(s_record_116_get_records_take(s_record_116_copy(v_state))); v_i < s_bound_494; ++v_i)
	{
		int64_t s_item_495 = (100 + v_i);
		v_elements = s_array_i64_append_take(v_elements, s_item_495);
	}
	S_Array_i64 * v_frontier = s_array_i64_make(0, NULL);
	int64_t s_item_496 = 7;
	v_frontier = s_array_i64_append_take(v_frontier, s_item_496);
	int64_t s_item_497 = 8;
	v_frontier = s_array_i64_append_take(v_frontier, s_item_497);
	for (int64_t v_i = 0, s_bound_498 = s_generic_array_length_take(s_record_116_get_records_take(s_record_116_copy(v_state))); v_i < s_bound_498; ++v_i)
	{
		int64_t s_item_499 = s_array_of((100 + v_i));
		v_frontier = s_array_i64_append_take(v_frontier, s_item_499);
	}
	for (int64_t v_depth = 0, s_bound_500 = 2; v_depth < s_bound_500; ++v_depth)
	{
		S_Array_i64 * v_next_frontier = s_array_i64_make(0, NULL);
		{
			S_Array_i64 * v_s_iter_501 = s_array_i64_retain(v_frontier);
			for (int64_t s_index_501 = 0; s_index_501 < (int64_t)v_s_iter_501->length; ++s_index_501)
			{
				int64_t v_item = s_array_i64_get_checked(v_s_iter_501, s_index_501);
				int64_t s_item_502 = v_item;
				v_elements = s_array_i64_append_take(v_elements, s_item_502);
				int64_t s_item_503 = s_array_of(v_item);
				v_next_frontier = s_array_i64_append_take(v_next_frontier, s_item_503);
			}
			s_array_i64_release(v_s_iter_501);
		}
		S_Array_i64 * s_value_504 = s_array_i64_retain(v_next_frontier);
		s_array_i64_release(v_frontier);
		v_frontier = s_value_504;
		s_array_i64_release(v_next_frontier);
	}
	{
		S_Array_i64 * v_s_iter_505 = s_array_i64_retain(v_elements);
		for (int64_t s_index_505 = 0; s_index_505 < (int64_t)v_s_iter_505->length; ++s_index_505)
		{
			int64_t v_element = s_array_i64_get_checked(v_s_iter_505, s_index_505);
			const int64_t v_kind = s_array_of(v_element);
			s_record_120 v_abi = s_c_abi(s_record_116_copy(v_state), v_element);
			S_String * v_type = s_record_120_get_c_name_take(s_record_120_copy(v_abi));
			S_String * v_prefix = s_array_helper(v_kind);
			S_String * v_copy = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_drop = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_506 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_copy(void *dst, const void *src) { *(", sizeof("_copy(void *dst, const void *src) { *(") - 1)), s_string_retain(v_type)), s_string_from_bytes(" *)dst = ", sizeof(" *)dst = ") - 1)), s_record_copy(v_element, s_string_concat_take(s_string_concat_take(s_string_from_bytes("*(", sizeof("*(") - 1), s_string_retain(v_type)), s_string_from_bytes(" const *)src", sizeof(" const *)src") - 1)))), s_string_from_bytes("; }\n", sizeof("; }\n") - 1));
				s_string_release(v_output);
				v_output = s_value_506;
				S_String *s_value_507 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_drop(void *value) { ", sizeof("_drop(void *value) { ") - 1)), s_record_drop(v_element, s_string_concat_take(s_string_concat_take(s_string_from_bytes("*(", sizeof("*(") - 1), s_string_retain(v_type)), s_string_from_bytes(" *)value", sizeof(" *)value") - 1)))), s_string_from_bytes(" }\n", sizeof(" }\n") - 1));
				s_string_release(v_output);
				v_output = s_value_507;
				S_String *s_value_508 = s_string_concat_take(s_string_retain(v_prefix), s_string_from_bytes("_copy", sizeof("_copy") - 1));
				s_string_release(v_copy);
				v_copy = s_value_508;
				S_String *s_value_509 = s_string_concat_take(s_string_retain(v_prefix), s_string_from_bytes("_drop", sizeof("_drop") - 1));
				s_string_release(v_drop);
				v_drop = s_value_509;
			}
			S_String *s_value_510 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Array *", sizeof("static inline S_Array *") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_make_take(size_t count, ", sizeof("_make_take(size_t count, ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" *items) {\n", sizeof(" *items) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_510;
			S_String *s_value_511 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tS_Array *result = s_array_make(count, items, sizeof(", sizeof("\tS_Array *result = s_array_make(count, items, sizeof(") - 1)), s_string_retain(v_type)), s_string_from_bytes("), ", sizeof("), ") - 1)), s_string_retain(v_copy)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_drop)), s_string_from_bytes(");\n", sizeof(");\n") - 1));
			s_string_release(v_output);
			v_output = s_value_511;
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_512 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tfor (size_t i = 0; i < count; ++i) ", sizeof("\tfor (size_t i = 0; i < count; ++i) ") - 1)), s_record_drop(v_element, s_string_from_bytes("items[i]", sizeof("items[i]") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_512;
			}
			S_String *s_value_513 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn result;\n}\n", sizeof("\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_513;
			S_String *s_value_514 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Array *", sizeof("static inline S_Array *") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_append_take(S_Array *owned, ", sizeof("_append_take(S_Array *owned, ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" item) {\n", sizeof(" item) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_514;
			S_String *s_value_515 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tS_Array *result = s_array_append(owned, &item);\n", sizeof("\tS_Array *result = s_array_append(owned, &item);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_515;
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_516 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_element, s_string_from_bytes("item", sizeof("item") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_516;
			}
			S_String *s_value_517 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn result;\n}\n", sizeof("\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_517;
			S_String *s_value_518 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_get_checked_take(S_Array *owned, int64_t index) {\n", sizeof("_get_checked_take(S_Array *owned, int64_t index) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_518;
			S_String *s_value_519 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *p = (", sizeof(" const *p = (") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *)s_array_at(owned, index);\n", sizeof(" const *)s_array_at(owned, index);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_519;
			S_String *s_value_520 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (!p) s_panic(\"index out of bounds\");\n", sizeof("\tif (!p) s_panic(\"index out of bounds\");\n") - 1));
			s_string_release(v_output);
			v_output = s_value_520;
			S_String *s_value_521 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes(" value = ", sizeof(" value = ") - 1)), s_record_copy(v_element, s_string_from_bytes("*p", sizeof("*p") - 1))), s_string_from_bytes(";\n", sizeof(";\n") - 1));
			s_string_release(v_output);
			v_output = s_value_521;
			S_String *s_value_522 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\ts_array_release(owned);\n\treturn value;\n}\n", sizeof("\ts_array_release(owned);\n\treturn value;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_522;
			S_String *s_value_523 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_get_borrow(const S_Array *array, int64_t index) {\n", sizeof("_get_borrow(const S_Array *array, int64_t index) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_523;
			S_String *s_value_524 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *p = (", sizeof(" const *p = (") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *)s_array_at(array, index);\n", sizeof(" const *)s_array_at(array, index);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_524;
			S_String *s_value_525 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (!p) s_panic(\"index out of bounds\");\n", sizeof("\tif (!p) s_panic(\"index out of bounds\");\n") - 1));
			s_string_release(v_output);
			v_output = s_value_525;
			S_String *s_value_526 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn ", sizeof("\treturn ") - 1)), s_record_copy(v_element, s_string_from_bytes("*p", sizeof("*p") - 1))), s_string_from_bytes(";\n}\n", sizeof(";\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_526;
			S_String *s_value_527 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_set_take(S_Array **owned, int64_t index, ", sizeof("_set_take(S_Array **owned, int64_t index, ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" item) {\n", sizeof(" item) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_527;
			S_String *s_value_528 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\ts_array_set(owned, index, &item);\n", sizeof("\ts_array_set(owned, index, &item);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_528;
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_529 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_element, s_string_from_bytes("item", sizeof("item") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_529;
			}
			S_String *s_value_530 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("}\n", sizeof("}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_530;
			s_string_release(v_drop);
			s_string_release(v_copy);
			s_string_release(v_prefix);
			s_string_release(v_type);
			s_record_120_drop(v_abi);
		}
		s_array_i64_release(v_s_iter_505);
	}
	S_String *s_value_531 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline int64_t s_generic_array_length_take(S_Array *owned) { int64_t n = (int64_t)s_array_length(owned); s_array_release(owned); return n; }\n", sizeof("static inline int64_t s_generic_array_length_take(S_Array *owned) { int64_t n = (int64_t)s_array_length(owned); s_array_release(owned); return n; }\n") - 1));
	s_string_release(v_output);
	v_output = s_value_531;
	S_String * s_return_532 = s_string_retain(v_output);
	s_array_i64_release(v_frontier);
	s_array_i64_release(v_elements);
	s_string_release(v_output);
	s_record_116_drop(v_state);
	return s_return_532;
	s_array_i64_release(v_frontier);
	s_array_i64_release(v_elements);
	s_string_release(v_output);
}

S_String * s_dict_declarations(s_record_116 v_state)
{
	S_String * v_output = s_string_from_bytes("", sizeof("") - 1);
	{
		S_Array_i64 * v_s_iter_533 = s_record_116_get_dict_kinds_take(s_record_116_copy(v_state));
		for (int64_t s_index_533 = 0; s_index_533 < (int64_t)v_s_iter_533->length; ++s_index_533)
		{
			int64_t v_kind = s_array_i64_get_checked(v_s_iter_533, s_index_533);
			const int64_t v_key = s_dict_key(v_kind);
			const int64_t v_value = s_dict_value(v_kind);
			S_String * v_prefix = s_dict_helper(v_kind);
			s_record_120 v_key_abi = s_c_abi(s_record_116_copy(v_state), v_key);
			s_record_120 v_value_abi = s_c_abi(s_record_116_copy(v_state), v_value);
			S_String * v_key_type = s_record_120_get_c_name_take(s_record_120_copy(v_key_abi));
			S_String * v_val_type = s_record_120_get_c_name_take(s_record_120_copy(v_value_abi));
			S_String * v_key_copy = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_key_drop = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_val_copy = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_val_drop = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_hash = s_string_from_bytes("s_hash_i64_key", sizeof("s_hash_i64_key") - 1);
			S_String * v_eq = s_string_from_bytes("s_eq_i64_key", sizeof("s_eq_i64_key") - 1);
			if ((v_key == 4))
			{
				S_String *s_value_534 = s_string_concat_take(s_array_helper(8), s_string_from_bytes("_copy", sizeof("_copy") - 1));
				s_string_release(v_key_copy);
				v_key_copy = s_value_534;
				S_String *s_value_535 = s_string_concat_take(s_array_helper(8), s_string_from_bytes("_drop", sizeof("_drop") - 1));
				s_string_release(v_key_drop);
				v_key_drop = s_value_535;
				S_String *s_value_536 = s_string_from_bytes("s_hash_string_key", sizeof("s_hash_string_key") - 1);
				s_string_release(v_hash);
				v_hash = s_value_536;
				S_String *s_value_537 = s_string_from_bytes("s_eq_string_key", sizeof("s_eq_string_key") - 1);
				s_string_release(v_eq);
				v_eq = s_value_537;
			}
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_value_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_538 = s_string_concat_take(s_array_helper(s_array_of(v_value)), s_string_from_bytes("_copy", sizeof("_copy") - 1));
				s_string_release(v_val_copy);
				v_val_copy = s_value_538;
				S_String *s_value_539 = s_string_concat_take(s_array_helper(s_array_of(v_value)), s_string_from_bytes("_drop", sizeof("_drop") - 1));
				s_string_release(v_val_drop);
				v_val_drop = s_value_539;
			}
			S_String *s_value_540 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Dict *", sizeof("static inline S_Dict *") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_make(void) { return s_dict_new(sizeof(", sizeof("_make(void) { return s_dict_new(sizeof(") - 1)), s_string_retain(v_key_type)), s_string_from_bytes("), sizeof(", sizeof("), sizeof(") - 1)), s_string_retain(v_val_type)), s_string_from_bytes("), ", sizeof("), ") - 1)), s_string_retain(v_key_copy)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_key_drop)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_val_copy)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_val_drop)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_hash)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_eq)), s_string_from_bytes("); }\n", sizeof("); }\n") - 1));
			s_string_release(v_output);
			v_output = s_value_540;
			S_String *s_value_541 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Dict *", sizeof("static inline S_Dict *") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_insert_take(S_Dict *owned, ", sizeof("_insert_take(S_Dict *owned, ") - 1)), s_string_retain(v_key_type)), s_string_from_bytes(" key, ", sizeof(" key, ") - 1)), s_string_retain(v_val_type)), s_string_from_bytes(" value) {\n", sizeof(" value) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_541;
			S_String *s_value_542 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tS_Dict *result = s_dict_insert(owned, &key, &value);\n", sizeof("\tS_Dict *result = s_dict_insert(owned, &key, &value);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_542;
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_key_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_543 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_key, s_string_from_bytes("key", sizeof("key") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_543;
			}
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_value_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_544 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_value, s_string_from_bytes("value", sizeof("value") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_544;
			}
			S_String *s_value_545 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn result;\n}\n", sizeof("\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_545;
			S_String * v_option_type = s_c_type(s_optional_of(v_value));
			S_String *s_value_546 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_option_type)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_get_take(S_Dict *owned, ", sizeof("_get_take(S_Dict *owned, ") - 1)), s_string_retain(v_key_type)), s_string_from_bytes(" key) {\n", sizeof(" key) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_546;
			S_String *s_value_547 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_val_type)), s_string_from_bytes(" const *item = (", sizeof(" const *item = (") - 1)), s_string_retain(v_val_type)), s_string_from_bytes(" const *)s_dict_get(owned, &key);\n", sizeof(" const *)s_dict_get(owned, &key);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_547;
			S_String *s_value_548 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_option_type)), s_string_from_bytes(" result = {0};\n", sizeof(" result = {0};\n") - 1));
			s_string_release(v_output);
			v_output = s_value_548;
			S_String *s_value_549 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (item) { result.has_value = true; result.value = ", sizeof("\tif (item) { result.has_value = true; result.value = ") - 1)), s_record_copy(v_value, s_string_from_bytes("*item", sizeof("*item") - 1))), s_string_from_bytes("; }\n", sizeof("; }\n") - 1));
			s_string_release(v_output);
			v_output = s_value_549;
			S_String *s_value_550 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\ts_dict_release(owned);\n", sizeof("\ts_dict_release(owned);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_550;
			if ((s_string_compare_take(s_record_120_get_copy_take(s_record_120_copy(v_key_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_551 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_key, s_string_from_bytes("key", sizeof("key") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_551;
			}
			S_String *s_value_552 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn result;\n}\n", sizeof("\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_552;
			s_string_release(v_option_type);
			s_string_release(v_eq);
			s_string_release(v_hash);
			s_string_release(v_val_drop);
			s_string_release(v_val_copy);
			s_string_release(v_key_drop);
			s_string_release(v_key_copy);
			s_string_release(v_val_type);
			s_string_release(v_key_type);
			s_record_120_drop(v_value_abi);
			s_record_120_drop(v_key_abi);
			s_string_release(v_prefix);
		}
		s_array_i64_release(v_s_iter_533);
	}
	S_String *s_value_553 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline int64_t s_dict_length_take(S_Dict *owned) { int64_t n = (int64_t)s_dict_length(owned); s_dict_release(owned); return n; }\n", sizeof("static inline int64_t s_dict_length_take(S_Dict *owned) { int64_t n = (int64_t)s_dict_length(owned); s_dict_release(owned); return n; }\n") - 1));
	s_string_release(v_output);
	v_output = s_value_553;
	S_String * s_return_554 = s_string_retain(v_output);
	s_string_release(v_output);
	s_record_116_drop(v_state);
	return s_return_554;
	s_string_release(v_output);
}

s_record_116 s_append_symbol(s_record_116 v_state, int64_t v_id, S_String * v_name, int64_t v_kind, bool v_mutable)
{
	s_record_116 v_result = s_record_116_copy(v_state);
	if (((v_kind == 0) || (v_kind == 5)))
	{
		s_record_116 s_return_555 = s_emit_error(s_record_116_copy(v_result), v_id, s_string_from_bytes("unsupported binding type", sizeof("unsupported binding type") - 1));
		s_record_116_drop(v_result);
		s_string_release(v_name);
		s_record_116_drop(v_state);
		return s_return_555;
	}
	s_record_107 v_canonical = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_records_take(s_record_116_copy(v_result)), v_kind);
	if ((s_record_107_get_id_take(s_record_107_copy(v_canonical)) == 0))
	{
		s_record_116 s_return_556 = s_emit_error(s_record_116_copy(v_result), v_id, s_string_from_bytes("unsupported binding type", sizeof("unsupported binding type") - 1));
		s_record_107_drop(v_canonical);
		s_record_116_drop(v_result);
		s_string_release(v_name);
		s_record_116_drop(v_state);
		return s_return_556;
	}
	s_record_106 s_field_557 = s_record_107_get_table_take(s_record_107_copy(v_canonical));
	s_record_106_drop(v_result.types);
	v_result.types = s_field_557;
	S_Array * s_field_558 = s_ga_10110_append_take(s_record_116_get_symbols_take(s_record_116_copy(v_result)), s_SymbolInfo(s_string_retain(v_name), v_kind, s_record_107_get_id_take(s_record_107_copy(v_canonical)), v_mutable, s_record_116_get_scope_depth_take(s_record_116_copy(v_result))));
	s_array_release(v_result.symbols);
	v_result.symbols = s_field_558;
	s_record_116 s_return_559 = s_record_116_copy(v_result);
	s_record_107_drop(v_canonical);
	s_record_116_drop(v_result);
	s_string_release(v_name);
	s_record_116_drop(v_state);
	return s_return_559;
	s_record_107_drop(v_canonical);
	s_record_116_drop(v_result);
}

s_record_116 s_check_type(s_record_116 v_state, int64_t v_id, int64_t v_found, int64_t v_expected)
{
	if (((v_found == v_expected) && (v_found != 0)))
	{
		s_record_116 s_return_560 = s_record_116_copy(v_state);
		s_record_116_drop(v_state);
		return s_return_560;
	}
	s_record_107 v_left = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_state)), s_record_116_get_records_take(s_record_116_copy(v_state)), v_found);
	s_record_107 v_right = s_canonical_kind(s_record_107_get_table_take(s_record_107_copy(v_left)), s_record_116_get_records_take(s_record_116_copy(v_state)), v_expected);
	s_record_116 v_next = s_record_116_copy(v_state);
	s_record_106 s_field_561 = s_record_107_get_table_take(s_record_107_copy(v_right));
	s_record_106_drop(v_next.types);
	v_next.types = s_field_561;
	if (s_hir_types_equal(s_record_107_get_id_take(s_record_107_copy(v_left)), s_record_107_get_id_take(s_record_107_copy(v_right))))
	{
		s_record_116 s_return_562 = s_record_116_copy(v_next);
		s_record_116_drop(v_next);
		s_record_107_drop(v_right);
		s_record_107_drop(v_left);
		s_record_116_drop(v_state);
		return s_return_562;
	}
	S_String * s_left_563 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_type_name(v_expected)), s_string_from_bytes(", found ", sizeof(", found ") - 1));
	S_String * s_right_564 = s_type_name(v_found);
	s_record_116 s_return_565 = s_emit_error(s_record_116_copy(v_next), v_id, s_string_concat_take(s_left_563, s_right_564));
	s_record_116_drop(v_next);
	s_record_107_drop(v_right);
	s_record_107_drop(v_left);
	s_record_116_drop(v_state);
	return s_return_565;
	s_record_116_drop(v_next);
	s_record_107_drop(v_right);
	s_record_107_drop(v_left);
}

bool s_has_effect(s_record_116 v_state, int64_t v_id, S_Array * v_visited)
{
	s_record_101 v_entry = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if ((((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 41) || (s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 21)) || (s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 34)))
	{
		bool s_return_566 = true;
		s_record_101_drop(v_entry);
		s_array_release(v_visited);
		s_record_116_drop(v_state);
		return s_return_566;
	}
	if (((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 5) && ((s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("/", sizeof("/") - 1)) == 0) || (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("%", sizeof("%") - 1)) == 0))))
	{
		bool s_return_567 = true;
		s_record_101_drop(v_entry);
		s_array_release(v_visited);
		s_record_116_drop(v_state);
		return s_return_567;
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 10))
	{
		const int64_t v_callee = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee)) != 29))
		{
			bool s_return_568 = true;
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_116_drop(v_state);
			return s_return_568;
		}
		S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee);
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0))
		{
			bool s_return_569 = true;
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_116_drop(v_state);
			return s_return_569;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("sqrt", sizeof("sqrt") - 1)) == 0))
		{
			bool s_return_570 = false;
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_116_drop(v_state);
			return s_return_570;
		}
		const int64_t v_index = s_find_function(s_record_116_copy(v_state), s_string_retain(v_name));
		if ((v_index < 0))
		{
			bool s_return_571 = true;
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_116_drop(v_state);
			return s_return_571;
		}
		bool v_seen = false;
		{
			S_Array * v_s_iter_572 = s_array_retain(v_visited);
			for (int64_t s_index_572 = 0; s_index_572 < (int64_t)s_array_length(v_s_iter_572); ++s_index_572)
			{
				S_String * v_value = s_ga_8_get_borrow(v_s_iter_572, s_index_572);
				if ((s_string_compare_take(s_string_retain(v_value), s_string_retain(v_name)) == 0))
				{
					v_seen = true;
				}
				s_string_release(v_value);
			}
			s_array_release(v_s_iter_572);
		}
		if ((!v_seen))
		{
			s_record_112 v_info = s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_index);
			if (s_has_effect(s_record_116_copy(v_state), s_record_112_get_body_take(s_record_112_copy(v_info)), s_ga_8_append_take(s_array_retain(v_visited), s_string_retain(v_name))))
			{
				bool s_return_573 = true;
				s_record_112_drop(v_info);
				s_string_release(v_name);
				s_record_101_drop(v_entry);
				s_array_release(v_visited);
				s_record_116_drop(v_state);
				return s_return_573;
			}
			s_record_112_drop(v_info);
		}
		s_string_release(v_name);
	}
	for (int64_t v_i = 0, s_bound_574 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_574; ++v_i)
	{
		if (s_has_effect(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i), s_array_retain(v_visited)))
		{
			bool s_return_575 = true;
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_116_drop(v_state);
			return s_return_575;
		}
	}
	bool s_return_576 = false;
	s_record_101_drop(v_entry);
	s_array_release(v_visited);
	s_record_116_drop(v_state);
	return s_return_576;
	s_record_101_drop(v_entry);
}

bool s_uses_optional_fallback(s_record_116 v_state, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 5) && (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("or", sizeof("or") - 1)) == 0)))
	{
		const int64_t v_left = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		s_record_101 v_leaf = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_left);
		if ((s_record_101_get_kind_take(s_record_101_copy(v_leaf)) == 29))
		{
			const int64_t v_index = s_find_symbol(s_record_116_copy(v_state), s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_left));
			if (((v_index >= 0) && s_is_optional(s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_index)))))
			{
				bool s_return_577 = true;
				s_record_101_drop(v_leaf);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_577;
			}
		}
		if ((s_record_101_get_kind_take(s_record_101_copy(v_leaf)) == 10))
		{
			const int64_t v_call = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_left, 0);
			if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_call)) == 4) && (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_call), s_string_from_bytes("get", sizeof("get") - 1)) == 0)))
			{
				bool s_return_578 = true;
				s_record_101_drop(v_leaf);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_578;
			}
		}
		s_record_101_drop(v_leaf);
	}
	for (int64_t v_i = 0, s_bound_579 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_579; ++v_i)
	{
		if (s_uses_optional_fallback(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i)))
		{
			bool s_return_580 = true;
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_580;
		}
	}
	bool s_return_581 = false;
	s_record_101_drop(v_entry);
	s_record_116_drop(v_state);
	return s_return_581;
	s_record_101_drop(v_entry);
}

bool s_ambiguous_order(s_record_116 v_state, int64_t v_a, int64_t v_b)
{
	bool s_return_582 = (s_has_effect(s_record_116_copy(v_state), v_a, s_ga_8_make_take(0, NULL)) && s_has_effect(s_record_116_copy(v_state), v_b, s_ga_8_make_take(0, NULL)));
	s_record_116_drop(v_state);
	return s_return_582;
}

s_record_117 s_specialize_call(s_record_116 v_state, int64_t v_id, s_record_112 v_info, S_Array_i64 * v_kinds, S_Array * v_arguments)
{
	s_record_119 v_signature = s_canonical_signature(s_record_116_copy(v_state), s_array_i64_retain(v_kinds));
	s_record_116 v_next = s_record_116_copy(v_state);
	s_record_106 s_field_583 = s_record_119_get_table_take(s_record_119_copy(v_signature));
	s_record_106_drop(v_next.types);
	v_next.types = s_field_583;
	const int64_t v_existing = s_find_instance(s_record_116_copy(v_next), s_record_112_get_name_take(s_record_112_copy(v_info)), s_record_119_get_ids_take(s_record_119_copy(v_signature)));
	if ((v_existing >= 0))
	{
		s_record_113 v_instance = s_ga_10113_get_checked_take(s_record_116_get_instances_take(s_record_116_copy(v_next)), v_existing);
		if ((s_record_113_get_kind_take(s_record_113_copy(v_instance)) == 0))
		{
			s_record_117 s_return_584 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("recursive generic call requires an explicit return type", sizeof("recursive generic call requires an explicit return type") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_113_drop(v_instance);
			s_record_116_drop(v_next);
			s_record_119_drop(v_signature);
			s_array_release(v_arguments);
			s_array_i64_release(v_kinds);
			s_record_112_drop(v_info);
			s_record_116_drop(v_state);
			return s_return_584;
		}
		s_record_117 s_return_585 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_record_113_get_symbol_take(s_record_113_copy(v_instance)), s_string_from_bytes("(", sizeof("(") - 1)), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_113_get_kind_take(s_record_113_copy(v_instance)));
		s_record_113_drop(v_instance);
		s_record_116_drop(v_next);
		s_record_119_drop(v_signature);
		s_array_release(v_arguments);
		s_array_i64_release(v_kinds);
		s_record_112_drop(v_info);
		s_record_116_drop(v_state);
		return s_return_585;
		s_record_113_drop(v_instance);
	}
	const int64_t v_index = s_generic_array_length_take(s_record_116_get_instances_take(s_record_116_copy(v_next)));
	S_String * v_symbol = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_", sizeof("s_") - 1), s_record_112_get_name_take(s_record_112_copy(v_info))), s_string_from_bytes("__", sizeof("__") - 1)), s_string_from_i64(v_index));
	S_Array * v_params = s_ga_10104_make_take(0, NULL);
	for (int64_t v_i = 0, s_bound_586 = s_array_i64_length_take(s_array_i64_retain(v_kinds)); v_i < s_bound_586; ++v_i)
	{
		s_record_104 s_item_587 = s_ParameterInfo(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_record_112_get_params_take(s_record_112_copy(v_info)), v_i)), s_array_i64_get_checked_take(s_array_i64_retain(v_kinds), v_i));
		v_params = s_ga_10104_append_take(v_params, s_item_587);
	}
	S_Array * s_field_588 = s_ga_10113_append_take(s_record_116_get_instances_take(s_record_116_copy(v_next)), s_InstanceInfo(s_record_112_get_name_take(s_record_112_copy(v_info)), s_array_i64_retain(v_kinds), s_record_119_get_ids_take(s_record_119_copy(v_signature)), s_record_112_get_kind_take(s_record_112_copy(v_info)), s_string_retain(v_symbol), s_string_from_bytes("", sizeof("") - 1), 1));
	s_array_release(v_next.instances);
	v_next.instances = s_field_588;
	S_Array * v_previous_lines = s_record_116_get_lines_take(s_record_116_copy(v_next));
	S_Array * v_previous_symbols = s_record_116_get_symbols_take(s_record_116_copy(v_next));
	const int64_t v_previous_indent = s_record_116_get_indentation_take(s_record_116_copy(v_next));
	const int64_t v_previous_loop = s_record_116_get_loop_depth_take(s_record_116_copy(v_next));
	const int64_t v_previous_loop_scope = s_record_116_get_loop_scope_take(s_record_116_copy(v_next));
	const int64_t v_previous_scope = s_record_116_get_scope_depth_take(s_record_116_copy(v_next));
	const int64_t v_previous_return = s_record_116_get_return_kind_take(s_record_116_copy(v_next));
	S_String * v_previous_name = s_record_116_get_function_name_take(s_record_116_copy(v_next));
	S_Array * v_previous_params = s_record_116_get_active_params_take(s_record_116_copy(v_next));
	S_Array * s_field_589 = s_ga_8_make_take(0, NULL);
	s_array_release(v_next.lines);
	v_next.lines = s_field_589;
	v_next.indentation = 0;
	v_next.loop_depth = 0;
	v_next.loop_scope = 0;
	v_next.scope_depth = 0;
	s_record_116 s_value_590 = s_emit_function(s_record_116_copy(v_next), s_FunctionInfo(s_record_112_get_name_take(s_record_112_copy(v_info)), s_record_112_get_kind_take(s_record_112_copy(v_info)), s_array_retain(v_params), s_record_112_get_body_take(s_record_112_copy(v_info)), false, s_string_retain(v_symbol)));
	s_record_116_drop(v_next);
	v_next = s_value_590;
	S_String * v_body = s_string_from_bytes("", sizeof("") - 1);
	{
		S_Array * v_s_iter_591 = s_record_116_get_lines_take(s_record_116_copy(v_next));
		for (int64_t s_index_591 = 0; s_index_591 < (int64_t)s_array_length(v_s_iter_591); ++s_index_591)
		{
			S_String * v_row = s_ga_8_get_borrow(v_s_iter_591, s_index_591);
			S_String *s_value_592 = s_string_concat_take(s_string_concat_take(s_string_retain(v_body), s_string_retain(v_row)), s_string_from_bytes("\n", sizeof("\n") - 1));
			s_string_release(v_body);
			v_body = s_value_592;
			s_string_release(v_row);
		}
		s_array_release(v_s_iter_591);
	}
	const int64_t v_result = s_record_116_get_return_kind_take(s_record_116_copy(v_next));
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		s_ga_10113_set_take(&v_next.instances, v_index, s_InstanceInfo(s_record_112_get_name_take(s_record_112_copy(v_info)), s_array_i64_retain(v_kinds), s_record_119_get_ids_take(s_record_119_copy(v_signature)), v_result, s_string_retain(v_symbol), s_string_retain(v_body), 2));
	}
	S_Array * s_field_593 = s_array_retain(v_previous_lines);
	s_array_release(v_next.lines);
	v_next.lines = s_field_593;
	S_Array * s_field_594 = s_array_retain(v_previous_symbols);
	s_array_release(v_next.symbols);
	v_next.symbols = s_field_594;
	v_next.indentation = v_previous_indent;
	v_next.loop_depth = v_previous_loop;
	v_next.loop_scope = v_previous_loop_scope;
	v_next.scope_depth = v_previous_scope;
	v_next.return_kind = v_previous_return;
	S_String * s_field_595 = s_string_retain(v_previous_name);
	s_string_release(v_next.function_name);
	v_next.function_name = s_field_595;
	S_Array * s_field_596 = s_array_retain(v_previous_params);
	s_array_release(v_next.active_params);
	v_next.active_params = s_field_596;
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_117 s_return_597 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
		s_string_release(v_body);
		s_array_release(v_previous_params);
		s_string_release(v_previous_name);
		s_array_release(v_previous_symbols);
		s_array_release(v_previous_lines);
		s_array_release(v_params);
		s_string_release(v_symbol);
		s_record_116_drop(v_next);
		s_record_119_drop(v_signature);
		s_array_release(v_arguments);
		s_array_i64_release(v_kinds);
		s_record_112_drop(v_info);
		s_record_116_drop(v_state);
		return s_return_597;
	}
	s_record_117 s_return_598 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_symbol), s_string_from_bytes("(", sizeof("(") - 1)), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments))), s_string_from_bytes(")", sizeof(")") - 1)), v_result);
	s_string_release(v_body);
	s_array_release(v_previous_params);
	s_string_release(v_previous_name);
	s_array_release(v_previous_symbols);
	s_array_release(v_previous_lines);
	s_array_release(v_params);
	s_string_release(v_symbol);
	s_record_116_drop(v_next);
	s_record_119_drop(v_signature);
	s_array_release(v_arguments);
	s_array_i64_release(v_kinds);
	s_record_112_drop(v_info);
	s_record_116_drop(v_state);
	return s_return_598;
	s_string_release(v_body);
	s_array_release(v_previous_params);
	s_string_release(v_previous_name);
	s_array_release(v_previous_symbols);
	s_array_release(v_previous_lines);
	s_array_release(v_params);
	s_string_release(v_symbol);
	s_record_116_drop(v_next);
	s_record_119_drop(v_signature);
}

s_record_116 s_check_hir_call(s_record_116 v_state, int64_t v_id, s_record_111 v_result)
{
	s_record_116 v_next = s_record_116_copy(v_state);
	s_record_106 s_field_599 = s_record_111_get_table_take(s_record_111_copy(v_result));
	s_record_106_drop(v_next.types);
	v_next.types = s_field_599;
	if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_116 s_return_600 = s_emit_error(s_record_116_copy(v_next), v_id, s_record_111_get_error_take(s_record_111_copy(v_result)));
		s_record_116_drop(v_next);
		s_record_111_drop(v_result);
		s_record_116_drop(v_state);
		return s_return_600;
	}
	if ((s_record_111_get_type_id_take(s_record_111_copy(v_result)) != 0))
	{
		S_Array * s_field_601 = s_ga_10108_append_take(s_record_116_get_hir_take(s_record_116_copy(v_next)), s_HirNode(v_id, 6, s_record_111_get_type_id_take(s_record_111_copy(v_result)), (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_601;
	}
	s_record_116 s_return_602 = s_record_116_copy(v_next);
	s_record_116_drop(v_next);
	s_record_111_drop(v_result);
	s_record_116_drop(v_state);
	return s_return_602;
	s_record_116_drop(v_next);
}

s_record_119 s_call_type_ids(s_record_116 v_state, S_Array_i64 * v_kinds)
{
	s_record_119 s_return_603 = s_canonical_signature(s_record_116_copy(v_state), s_array_i64_retain(v_kinds));
	s_array_i64_release(v_kinds);
	s_record_116_drop(v_state);
	return s_return_603;
}

s_record_117 s_emit_expr(s_record_116 v_state, int64_t v_id)
{
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_117 s_return_604 = s_ExprCode(s_record_116_copy(v_state), s_string_from_bytes("", sizeof("") - 1), 0);
		s_record_116_drop(v_state);
		return s_return_604;
	}
	s_record_101 v_entry = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	S_String * v_value = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	const int64_t v_literal = s_resolve_hir_literal(s_record_101_get_kind_take(s_record_101_copy(v_entry)));
	if ((v_literal != 0))
	{
		s_record_116 v_next = s_record_116_copy(v_state);
		S_Array * s_field_605 = s_ga_10108_append_take(s_record_116_get_hir_take(s_record_116_copy(v_next)), s_HirNode(v_id, 4, v_literal, (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_605;
		if ((v_literal == 4))
		{
			s_record_117 s_return_606 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_from_bytes(", sizeof("s_string_from_bytes(") - 1), s_string_retain(v_value)), s_string_from_bytes(", sizeof(", sizeof(", sizeof(") - 1)), s_string_retain(v_value)), s_string_from_bytes(") - 1)", sizeof(") - 1)") - 1)), 4);
			s_record_116_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_606;
		}
		s_record_117 s_return_607 = s_ExprCode(s_record_116_copy(v_next), s_string_retain(v_value), v_literal);
		s_record_116_drop(v_next);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_607;
		s_record_116_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 2))
	{
		if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 0))
		{
			s_record_117 s_return_608 = s_ExprCode(s_record_116_copy(v_state), s_string_from_bytes("s_array_i64_make(0, NULL)", sizeof("s_array_i64_make(0, NULL)") - 1), 7);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_608;
		}
		s_record_116 v_next = s_record_116_copy(v_state);
		S_Array * v_items = s_ga_8_make_take(0, NULL);
		int64_t v_effects = 0;
		int64_t v_element_kind = 0;
		for (int64_t v_i = 0, s_bound_609 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_609; ++v_i)
		{
			if (s_has_effect(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i), s_ga_8_make_take(0, NULL)))
			{
				v_effects += 1;
			}
			s_record_117 v_element = s_emit_expr(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
			s_record_116 s_value_610 = s_record_117_get_state_take(s_record_117_copy(v_element));
			s_record_116_drop(v_next);
			v_next = s_value_610;
			if ((v_i == 0))
			{
				v_element_kind = s_record_117_get_kind_take(s_record_117_copy(v_element));
			}
			else
			{
				s_record_116 s_value_611 = s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_element)), v_element_kind);
				s_record_116_drop(v_next);
				v_next = s_value_611;
			}
			S_String * s_item_612 = s_record_117_get_text_take(s_record_117_copy(v_element));
			v_items = s_ga_8_append_take(v_items, s_item_612);
			s_record_117_drop(v_element);
		}
		if ((v_effects > 1))
		{
			s_record_117 s_return_613 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("array initializer requires ordered evaluation", sizeof("array initializer requires ordered evaluation") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_release(v_items);
			s_record_116_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_613;
		}
		const int64_t v_kind = s_array_of(v_element_kind);
		if ((v_kind == 0))
		{
			s_record_117 s_return_614 = s_ExprCode(s_check_type(s_record_116_copy(v_next), v_id, v_element_kind, 1), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_release(v_items);
			s_record_116_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_614;
		}
		if ((v_kind == 7))
		{
			S_String * s_left_615 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_make(", sizeof("s_array_i64_make(") - 1), s_string_from_i64(s_record_101_get_count_take(s_record_101_copy(v_entry)))), s_string_from_bytes(", (int64_t[]){", sizeof(", (int64_t[]){") - 1));
			S_String * s_right_616 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_items));
			s_record_117 s_return_617 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_left_615, s_right_616), s_string_from_bytes("})", sizeof("})") - 1)), v_kind);
			s_array_release(v_items);
			s_record_116_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_617;
		}
		S_String * s_left_618 = s_string_concat_take(s_array_helper(v_kind), s_string_from_bytes("_make_take(", sizeof("_make_take(") - 1));
		S_String * s_right_619 = s_string_from_i64(s_record_101_get_count_take(s_record_101_copy(v_entry)));
		S_String * s_left_620 = s_string_concat_take(s_string_concat_take(s_left_618, s_right_619), s_string_from_bytes(", (", sizeof(", (") - 1));
		S_String * s_right_621 = s_c_type(v_element_kind);
		S_String * s_left_622 = s_string_concat_take(s_string_concat_take(s_left_620, s_right_621), s_string_from_bytes("[]){", sizeof("[]){") - 1));
		S_String * s_right_623 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_items));
		s_record_117 s_return_624 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_left_622, s_right_623), s_string_from_bytes("})", sizeof("})") - 1)), v_kind);
		s_array_release(v_items);
		s_record_116_drop(v_next);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_624;
		s_array_release(v_items);
		s_record_116_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 12))
	{
		if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) != 2))
		{
			s_record_117 s_return_625 = s_ExprCode(s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("only rank-1 indexing is supported", sizeof("only rank-1 indexing is supported") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_625;
		}
		s_record_117 v_array = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_117 v_index = s_emit_expr(s_record_117_get_state_take(s_record_117_copy(v_array)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
		s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_index));
		const int64_t v_element = s_array_element(s_record_117_get_kind_take(s_record_117_copy(v_array)));
		if ((v_element == 0))
		{
			s_record_117 s_return_626 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("checked indexing requires an array", sizeof("checked indexing requires an array") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_116_drop(v_next);
			s_record_117_drop(v_index);
			s_record_117_drop(v_array);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_626;
		}
		s_record_116 s_value_627 = s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_index)), 1);
		s_record_116_drop(v_next);
		v_next = s_value_627;
		if ((s_record_117_get_kind_take(s_record_117_copy(v_array)) == 7))
		{
			s_record_117 s_return_628 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_get_checked_take(", sizeof("s_array_i64_get_checked_take(") - 1), s_record_117_get_text_take(s_record_117_copy(v_array))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_index))), s_string_from_bytes(")", sizeof(")") - 1)), 1);
			s_record_116_drop(v_next);
			s_record_117_drop(v_index);
			s_record_117_drop(v_array);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_628;
		}
		s_record_117 s_return_629 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(s_record_117_get_kind_take(s_record_117_copy(v_array))), s_string_from_bytes("_get_checked_take(", sizeof("_get_checked_take(") - 1)), s_record_117_get_text_take(s_record_117_copy(v_array))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_index))), s_string_from_bytes(")", sizeof(")") - 1)), v_element);
		s_record_116_drop(v_next);
		s_record_117_drop(v_index);
		s_record_117_drop(v_array);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_629;
		s_record_116_drop(v_next);
		s_record_117_drop(v_index);
		s_record_117_drop(v_array);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 33))
	{
		s_record_116 v_next = s_record_116_copy(v_state);
		S_Array * v_fields = s_ga_10104_make_take(0, NULL);
		S_Array * v_initializers = s_ga_8_make_take(0, NULL);
		int64_t v_effects = 0;
		S_String * v_touches = s_string_from_bytes("", sizeof("") - 1);
		for (int64_t v_i = 0, s_bound_630 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_630; ++v_i)
		{
			const int64_t v_field_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i);
			s_record_101 v_field = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_field_id);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 36))
			{
				S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_field_id);
				const int64_t v_symbol = s_find_symbol(s_record_116_copy(v_next), s_string_retain(v_name));
				if ((v_symbol < 0))
				{
					s_record_117 s_return_631 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_field_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown spread source '", sizeof("unknown spread source '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_string_release(v_touches);
					s_array_release(v_initializers);
					s_array_release(v_fields);
					s_record_116_drop(v_next);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_631;
				}
				S_String *s_value_632 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_touches), s_string_from_bytes("(void)v_", sizeof("(void)v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1));
				s_string_release(v_touches);
				v_touches = s_value_632;
				const int64_t v_kind = s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)), v_symbol));
				if (((v_kind < 100) || (v_kind >= 10000)))
				{
					s_record_117 s_return_633 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_field_id, s_string_from_bytes("record spread expects a record", sizeof("record spread expects a record") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_string_release(v_touches);
					s_array_release(v_initializers);
					s_array_release(v_fields);
					s_record_116_drop(v_next);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_633;
				}
				{
					S_Array * v_s_iter_634 = s_record_115_get_fields_take(s_ga_10115_get_checked_take(s_record_116_get_records_take(s_record_116_copy(v_next)), (v_kind - 100)));
					for (int64_t s_index_634 = 0; s_index_634 < (int64_t)s_array_length(v_s_iter_634); ++s_index_634)
					{
						s_record_104 v_member = s_ga_10104_get_borrow(v_s_iter_634, s_index_634);
						int64_t v_found = (-1);
						for (int64_t v_j = 0, s_bound_635 = s_generic_array_length_take(s_array_retain(v_fields)); v_j < s_bound_635; ++v_j)
						{
							if ((s_string_compare_take(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_fields), v_j)), s_record_104_get_name_take(s_record_104_copy(v_member))) == 0))
							{
								v_found = v_j;
							}
						}
						S_String * v_code = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes(".", sizeof(".") - 1), s_record_104_get_name_take(s_record_104_copy(v_member))), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_copy(s_record_104_get_kind_take(s_record_104_copy(v_member)), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(".", sizeof(".") - 1)), s_record_104_get_name_take(s_record_104_copy(v_member)))));
						if ((v_found >= 0))
						{
							s_ga_10104_set_take(&v_fields, v_found, s_record_104_copy(v_member));
							s_ga_8_set_take(&v_initializers, v_found, s_string_retain(v_code));
						}
						else
						{
							s_record_104 s_item_636 = s_record_104_copy(v_member);
							v_fields = s_ga_10104_append_take(v_fields, s_item_636);
							S_String * s_item_637 = s_string_retain(v_code);
							v_initializers = s_ga_8_append_take(v_initializers, s_item_637);
						}
						s_string_release(v_code);
						s_record_104_drop(v_member);
					}
					s_array_release(v_s_iter_634);
				}
				s_string_release(v_name);
			}
			else if ((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 15))
			{
				S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_field_id);
				bool v_found = false;
				S_Array * v_kept = s_ga_10104_make_take(0, NULL);
				S_Array * v_values = s_ga_8_make_take(0, NULL);
				for (int64_t v_j = 0, s_bound_638 = s_generic_array_length_take(s_array_retain(v_fields)); v_j < s_bound_638; ++v_j)
				{
					if ((s_string_compare_take(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_fields), v_j)), s_string_retain(v_name)) == 0))
					{
						v_found = true;
					}
					else
					{
						s_record_104 s_item_639 = s_ga_10104_get_checked_take(s_array_retain(v_fields), v_j);
						v_kept = s_ga_10104_append_take(v_kept, s_item_639);
						S_String * s_item_640 = s_ga_8_get_checked_take(s_array_retain(v_initializers), v_j);
						v_values = s_ga_8_append_take(v_values, s_item_640);
					}
				}
				if ((!v_found))
				{
					s_record_117 s_return_641 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_field_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown deleted field '", sizeof("unknown deleted field '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
					s_array_release(v_values);
					s_array_release(v_kept);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_string_release(v_touches);
					s_array_release(v_initializers);
					s_array_release(v_fields);
					s_record_116_drop(v_next);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_641;
				}
				S_Array * s_value_642 = s_array_retain(v_kept);
				s_array_release(v_fields);
				v_fields = s_value_642;
				S_Array * s_value_643 = s_array_retain(v_values);
				s_array_release(v_initializers);
				v_initializers = s_value_643;
				s_array_release(v_values);
				s_array_release(v_kept);
				s_string_release(v_name);
			}
			else if ((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 19))
			{
				S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_field_id);
				const int64_t v_value_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_field_id, 0);
				if (s_has_effect(s_record_116_copy(v_next), v_value_id, s_ga_8_make_take(0, NULL)))
				{
					v_effects += 1;
				}
				s_record_117 v_result = s_emit_expr(s_record_116_copy(v_next), v_value_id);
				s_record_116 s_value_644 = s_record_117_get_state_take(s_record_117_copy(v_result));
				s_record_116_drop(v_next);
				v_next = s_value_644;
				if (((s_record_117_get_kind_take(s_record_117_copy(v_result)) == 0) || (s_record_117_get_kind_take(s_record_117_copy(v_result)) == 5)))
				{
					s_record_117 s_return_645 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_field_id, s_string_from_bytes("invalid record field type", sizeof("invalid record field type") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
					s_record_117_drop(v_result);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_string_release(v_touches);
					s_array_release(v_initializers);
					s_array_release(v_fields);
					s_record_116_drop(v_next);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_645;
				}
				int64_t v_found = (-1);
				for (int64_t v_j = 0, s_bound_646 = s_generic_array_length_take(s_array_retain(v_fields)); v_j < s_bound_646; ++v_j)
				{
					if ((s_string_compare_take(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_fields), v_j)), s_string_retain(v_name)) == 0))
					{
						v_found = v_j;
					}
				}
				s_record_104 v_member = s_RecordField(s_string_retain(v_name), s_record_117_get_kind_take(s_record_117_copy(v_result)));
				S_String * v_code = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes(".", sizeof(".") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result)));
				if ((v_found >= 0))
				{
					s_ga_10104_set_take(&v_fields, v_found, s_record_104_copy(v_member));
					s_ga_8_set_take(&v_initializers, v_found, s_string_retain(v_code));
				}
				else
				{
					s_record_104 s_item_647 = s_record_104_copy(v_member);
					v_fields = s_ga_10104_append_take(v_fields, s_item_647);
					S_String * s_item_648 = s_string_retain(v_code);
					v_initializers = s_ga_8_append_take(v_initializers, s_item_648);
				}
				s_string_release(v_code);
				s_record_104_drop(v_member);
				s_record_117_drop(v_result);
				s_string_release(v_name);
			}
			else
			{
				s_record_117 s_return_649 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_field_id, s_string_from_bytes("invalid record field", sizeof("invalid record field") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_101_drop(v_field);
				s_string_release(v_touches);
				s_array_release(v_initializers);
				s_array_release(v_fields);
				s_record_116_drop(v_next);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_649;
			}
			s_record_101_drop(v_field);
		}
		if (((v_effects > 0) && (s_record_101_get_count_take(s_record_101_copy(v_entry)) > 1)))
		{
			s_record_117 s_return_650 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("record initializer requires ordered evaluation", sizeof("record initializer requires ordered evaluation") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_touches);
			s_array_release(v_initializers);
			s_array_release(v_fields);
			s_record_116_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_650;
		}
		if ((s_generic_array_length_take(s_array_retain(v_fields)) == 0))
		{
			s_record_117 s_return_651 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("empty records are not supported yet", sizeof("empty records are not supported yet") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_touches);
			s_array_release(v_initializers);
			s_array_release(v_fields);
			s_record_116_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_651;
		}
		s_record_117 v_record = s_intern_record(s_record_116_copy(v_next), s_array_retain(v_fields));
		S_String * s_left_652 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_string_retain(v_touches)), s_string_from_bytes("((", sizeof("((") - 1)), s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_record)))), s_string_from_bytes("){ ", sizeof("){ ") - 1));
		S_String * s_right_653 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_initializers));
		s_record_117 s_return_654 = s_ExprCode(s_record_117_get_state_take(s_record_117_copy(v_record)), s_string_concat_take(s_string_concat_take(s_left_652, s_right_653), s_string_from_bytes(" }))", sizeof(" }))") - 1)), s_record_117_get_kind_take(s_record_117_copy(v_record)));
		s_record_117_drop(v_record);
		s_string_release(v_touches);
		s_array_release(v_initializers);
		s_array_release(v_fields);
		s_record_116_drop(v_next);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_654;
		s_record_117_drop(v_record);
		s_string_release(v_touches);
		s_array_release(v_initializers);
		s_array_release(v_fields);
		s_record_116_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 4))
	{
		s_record_117 v_receiver = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_107 v_owner = s_canonical_kind(s_record_116_get_types_take(s_record_117_get_state_take(s_record_117_copy(v_receiver))), s_record_116_get_records_take(s_record_117_get_state_take(s_record_117_copy(v_receiver))), s_record_117_get_kind_take(s_record_117_copy(v_receiver)));
		s_record_109 v_resolved = s_resolve_hir_field(s_record_107_get_table_take(s_record_107_copy(v_owner)), s_record_107_get_id_take(s_record_107_copy(v_owner)), s_string_retain(v_value));
		s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_receiver));
		s_record_106 s_field_655 = s_record_107_get_table_take(s_record_107_copy(v_owner));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_655;
		if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_117 s_return_656 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_record_109_get_error_take(s_record_109_copy(v_resolved))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_116_drop(v_next);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_owner);
			s_record_117_drop(v_receiver);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_656;
		}
		s_record_104 v_field = s_ga_10104_get_checked_take(s_record_115_get_fields_take(s_ga_10115_get_checked_take(s_record_116_get_records_take(s_record_116_copy(v_next)), (s_record_117_get_kind_take(s_record_117_copy(v_receiver)) - 100))), s_record_109_get_field_index_take(s_record_109_copy(v_resolved)));
		S_Array * s_field_657 = s_ga_10108_append_take(s_record_116_get_hir_take(s_record_116_copy(v_next)), s_HirNode(v_id, 1, s_record_109_get_type_id_take(s_record_109_copy(v_resolved)), s_record_109_get_field_index_take(s_record_109_copy(v_resolved))));
		s_array_release(v_next.hir);
		v_next.hir = s_field_657;
		s_record_117 s_return_658 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_receiver))), s_string_from_bytes("_get_", sizeof("_get_") - 1)), s_record_104_get_name_take(s_record_104_copy(v_field))), s_string_from_bytes("_take(", sizeof("_take(") - 1)), s_record_117_get_text_take(s_record_117_copy(v_receiver))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_104_get_kind_take(s_record_104_copy(v_field)));
		s_record_104_drop(v_field);
		s_record_116_drop(v_next);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_owner);
		s_record_117_drop(v_receiver);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_658;
		s_record_104_drop(v_field);
		s_record_116_drop(v_next);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_owner);
		s_record_117_drop(v_receiver);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 29))
	{
		s_record_109 v_binding = s_resolve_hir_name(s_record_116_get_symbols_take(s_record_116_copy(v_state)), s_string_retain(v_value));
		if ((s_record_109_get_field_index_take(s_record_109_copy(v_binding)) >= 0))
		{
			s_record_116 v_next = s_record_116_copy(v_state);
			S_Array * s_field_659 = s_ga_10108_append_take(s_record_116_get_hir_take(s_record_116_copy(v_next)), s_HirNode(v_id, 3, s_record_109_get_type_id_take(s_record_109_copy(v_binding)), s_record_109_get_field_index_take(s_record_109_copy(v_binding))));
			s_array_release(v_next.hir);
			v_next.hir = s_field_659;
			const int64_t v_kind = s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)), s_record_109_get_field_index_take(s_record_109_copy(v_binding))));
			S_String * v_copied = s_record_copy(v_kind, s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_value)));
			s_record_117 s_return_660 = s_ExprCode(s_record_116_copy(v_next), s_string_retain(v_copied), v_kind);
			s_string_release(v_copied);
			s_record_116_drop(v_next);
			s_record_109_drop(v_binding);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_660;
			s_string_release(v_copied);
			s_record_116_drop(v_next);
		}
		S_String * v_constant = s_find_constant(s_record_116_copy(v_state), s_string_retain(v_value));
		if ((s_string_compare_take(s_string_retain(v_constant), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_117 s_return_661 = s_ExprCode(s_record_116_copy(v_state), s_string_retain(v_constant), 1);
			s_string_release(v_constant);
			s_record_109_drop(v_binding);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_661;
		}
		s_record_117 s_return_662 = s_ExprCode(s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown name '", sizeof("unknown name '") - 1), s_string_retain(v_value)), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
		s_string_release(v_constant);
		s_record_109_drop(v_binding);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_662;
		s_string_release(v_constant);
		s_record_109_drop(v_binding);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 39))
	{
		s_record_117 v_arg = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_107 v_type_id = s_canonical_kind(s_record_116_get_types_take(s_record_117_get_state_take(s_record_117_copy(v_arg))), s_record_116_get_records_take(s_record_117_get_state_take(s_record_117_copy(v_arg))), s_record_117_get_kind_take(s_record_117_copy(v_arg)));
		s_record_109 v_resolved = s_resolve_hir_unary(s_record_107_get_table_take(s_record_107_copy(v_type_id)), s_record_107_get_id_take(s_record_107_copy(v_type_id)), s_string_retain(v_value));
		s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_arg));
		s_record_106 s_field_663 = s_record_107_get_table_take(s_record_107_copy(v_type_id));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_663;
		if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			if ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("not", sizeof("not") - 1)) == 0))
			{
				s_record_117 s_return_664 = s_ExprCode(s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_arg)), 3), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_116_drop(v_next);
				s_record_109_drop(v_resolved);
				s_record_107_drop(v_type_id);
				s_record_117_drop(v_arg);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_664;
			}
			s_record_117 s_return_665 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_record_109_get_error_take(s_record_109_copy(v_resolved))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_116_drop(v_next);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_type_id);
			s_record_117_drop(v_arg);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_665;
		}
		S_Array * s_field_666 = s_ga_10108_append_take(s_record_116_get_hir_take(s_record_116_copy(v_next)), s_HirNode(v_id, 5, s_record_109_get_type_id_take(s_record_109_copy(v_resolved)), (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_666;
		if ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("not", sizeof("not") - 1)) == 0))
		{
			s_record_117 s_return_667 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("(!", sizeof("(!") - 1), s_record_117_get_text_take(s_record_117_copy(v_arg))), s_string_from_bytes(")", sizeof(")") - 1)), 3);
			s_record_116_drop(v_next);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_type_id);
			s_record_117_drop(v_arg);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_667;
		}
		s_record_117 s_return_668 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_string_retain(v_value)), s_record_117_get_text_take(s_record_117_copy(v_arg))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_117_get_kind_take(s_record_117_copy(v_arg)));
		s_record_116_drop(v_next);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_type_id);
		s_record_117_drop(v_arg);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_668;
		s_record_116_drop(v_next);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_type_id);
		s_record_117_drop(v_arg);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 5))
	{
		const int64_t v_left_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		const int64_t v_right_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1);
		const bool v_needs_order = (((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("and", sizeof("and") - 1)) != 0) && (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("or", sizeof("or") - 1)) != 0)) && s_ambiguous_order(s_record_116_copy(v_state), v_left_id, v_right_id));
		s_record_117 v_left = s_emit_expr(s_record_116_copy(v_state), v_left_id);
		if (v_needs_order)
		{
			s_record_116 v_ordered = s_record_117_get_state_take(s_record_117_copy(v_left));
			S_String * v_name = s_string_concat_take(s_string_from_bytes("s_left_", sizeof("s_left_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_ordered))));
			v_ordered.counter += 1;
			s_record_116 s_value_669 = s_line(s_record_116_copy(v_ordered), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_left))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_left))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_ordered);
			v_ordered = s_value_669;
			s_record_117 s_value_670 = s_ExprCode(s_record_116_copy(v_ordered), s_string_retain(v_name), s_record_117_get_kind_take(s_record_117_copy(v_left)));
			s_record_117_drop(v_left);
			v_left = s_value_670;
			s_string_release(v_name);
			s_record_116_drop(v_ordered);
		}
		if (((((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("and", sizeof("and") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("or", sizeof("or") - 1)) == 0)) && (s_record_117_get_kind_take(s_record_117_copy(v_left)) == 3)) && s_uses_optional_fallback(s_record_116_copy(v_state), v_right_id)))
		{
			s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_left));
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_bool_", sizeof("s_bool_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_next))));
			v_next.counter += 1;
			s_record_116 s_value_671 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("bool ", sizeof("bool ") - 1), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_left))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_671;
			S_String * v_test = s_string_retain(v_temp);
			if ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("or", sizeof("or") - 1)) == 0))
			{
				S_String *s_value_672 = s_string_concat_take(s_string_from_bytes("!", sizeof("!") - 1), s_string_retain(v_temp));
				s_string_release(v_test);
				v_test = s_value_672;
			}
			s_record_116 s_value_673 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (", sizeof("if (") - 1), s_string_retain(v_test)), s_string_from_bytes(") {", sizeof(") {") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_673;
			v_next.indentation += 1;
			s_record_117 v_right = s_emit_expr(s_record_116_copy(v_next), v_right_id);
			s_record_116 s_value_674 = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_right)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_right)), 3);
			s_record_116_drop(v_next);
			v_next = s_value_674;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_117 s_return_675 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_117_drop(v_right);
				s_string_release(v_test);
				s_string_release(v_temp);
				s_record_116_drop(v_next);
				s_record_117_drop(v_left);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_675;
			}
			s_record_116 s_value_676 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_temp), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_right))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_676;
			v_next.indentation -= 1;
			s_record_116 s_value_677 = s_line(s_record_116_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_116_drop(v_next);
			v_next = s_value_677;
			s_record_117 s_return_678 = s_ExprCode(s_record_116_copy(v_next), s_string_retain(v_temp), 3);
			s_record_117_drop(v_right);
			s_string_release(v_test);
			s_string_release(v_temp);
			s_record_116_drop(v_next);
			s_record_117_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_678;
			s_record_117_drop(v_right);
			s_string_release(v_test);
			s_string_release(v_temp);
			s_record_116_drop(v_next);
		}
		if (((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("or", sizeof("or") - 1)) == 0) && s_is_optional(s_record_117_get_kind_take(s_record_117_copy(v_left)))))
		{
			const int64_t v_kind = s_optional_value(s_record_117_get_kind_take(s_record_117_copy(v_left)));
			s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_left));
			const int64_t v_number = s_record_116_get_counter_take(s_record_116_copy(v_next));
			v_next.counter += 1;
			S_String * v_option = s_string_concat_take(s_string_from_bytes("s_option_", sizeof("s_option_") - 1), s_string_from_i64(v_number));
			S_String * v_result = s_string_concat_take(s_string_from_bytes("s_fallback_", sizeof("s_fallback_") - 1), s_string_from_i64(v_number));
			s_record_116 s_value_679 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_left))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_option)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_left))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_679;
			s_record_116 s_value_680 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(v_kind), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_result)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_680;
			s_record_116 s_value_681 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (", sizeof("if (") - 1), s_string_retain(v_option)), s_string_from_bytes(".has_value) {", sizeof(".has_value) {") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_681;
			v_next.indentation += 1;
			s_record_116 s_value_682 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_option)), s_string_from_bytes(".value;", sizeof(".value;") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_682;
			v_next.indentation -= 1;
			s_record_116 s_value_683 = s_line(s_record_116_copy(v_next), s_string_from_bytes("} else {", sizeof("} else {") - 1));
			s_record_116_drop(v_next);
			v_next = s_value_683;
			v_next.indentation += 1;
			s_record_117 v_right = s_emit_expr(s_record_116_copy(v_next), v_right_id);
			s_record_116 s_value_684 = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_right)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_right)), v_kind);
			s_record_116_drop(v_next);
			v_next = s_value_684;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_117 s_return_685 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_117_drop(v_right);
				s_string_release(v_result);
				s_string_release(v_option);
				s_record_116_drop(v_next);
				s_record_117_drop(v_left);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_685;
			}
			s_record_116 s_value_686 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_right))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_686;
			v_next.indentation -= 1;
			s_record_116 s_value_687 = s_line(s_record_116_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_116_drop(v_next);
			v_next = s_value_687;
			s_record_117 s_return_688 = s_ExprCode(s_record_116_copy(v_next), s_string_retain(v_result), v_kind);
			s_record_117_drop(v_right);
			s_string_release(v_result);
			s_string_release(v_option);
			s_record_116_drop(v_next);
			s_record_117_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_688;
			s_record_117_drop(v_right);
			s_string_release(v_result);
			s_string_release(v_option);
			s_record_116_drop(v_next);
		}
		s_record_117 v_right = s_emit_expr(s_record_117_get_state_take(s_record_117_copy(v_left)), v_right_id);
		if (v_needs_order)
		{
			s_record_116 v_ordered = s_record_117_get_state_take(s_record_117_copy(v_right));
			S_String * v_name = s_string_concat_take(s_string_from_bytes("s_right_", sizeof("s_right_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_ordered))));
			v_ordered.counter += 1;
			s_record_116 s_value_689 = s_line(s_record_116_copy(v_ordered), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_right))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_right))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_ordered);
			v_ordered = s_value_689;
			s_record_117 s_value_690 = s_ExprCode(s_record_116_copy(v_ordered), s_string_retain(v_name), s_record_117_get_kind_take(s_record_117_copy(v_right)));
			s_record_117_drop(v_right);
			v_right = s_value_690;
			s_string_release(v_name);
			s_record_116_drop(v_ordered);
		}
		s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_right));
		if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_117 s_return_691 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_116_drop(v_next);
			s_record_117_drop(v_right);
			s_record_117_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_691;
		}
		s_record_107 v_left_type = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_next)), s_record_116_get_records_take(s_record_116_copy(v_next)), s_record_117_get_kind_take(s_record_117_copy(v_left)));
		s_record_107 v_right_type = s_canonical_kind(s_record_107_get_table_take(s_record_107_copy(v_left_type)), s_record_116_get_records_take(s_record_116_copy(v_next)), s_record_117_get_kind_take(s_record_117_copy(v_right)));
		s_record_109 v_resolved = s_resolve_hir_binary(s_record_107_get_table_take(s_record_107_copy(v_right_type)), s_record_107_get_id_take(s_record_107_copy(v_left_type)), s_record_107_get_id_take(s_record_107_copy(v_right_type)), s_string_retain(v_value));
		s_record_106 s_field_692 = s_record_107_get_table_take(s_record_107_copy(v_right_type));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_692;
		if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_117 s_return_693 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_record_109_get_error_take(s_record_109_copy(v_resolved))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_right_type);
			s_record_107_drop(v_left_type);
			s_record_116_drop(v_next);
			s_record_117_drop(v_right);
			s_record_117_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_693;
		}
		S_Array * s_field_694 = s_ga_10108_append_take(s_record_116_get_hir_take(s_record_116_copy(v_next)), s_HirNode(v_id, 2, s_record_109_get_type_id_take(s_record_109_copy(v_resolved)), (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_694;
		S_String * v_op = s_string_retain(v_value);
		int64_t v_kind = s_record_117_get_kind_take(s_record_117_copy(v_left));
		if ((s_record_109_get_type_id_take(s_record_109_copy(v_resolved)) == 3))
		{
			v_kind = 3;
		}
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("and", sizeof("and") - 1)) == 0))
		{
			S_String *s_value_695 = s_string_from_bytes("&&", sizeof("&&") - 1);
			s_string_release(v_op);
			v_op = s_value_695;
		}
		else if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("or", sizeof("or") - 1)) == 0))
		{
			S_String *s_value_696 = s_string_from_bytes("||", sizeof("||") - 1);
			s_string_release(v_op);
			v_op = s_value_696;
		}
		if (((s_record_117_get_kind_take(s_record_117_copy(v_left)) == 4) && (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0)))
		{
			s_record_117 s_return_697 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_concat_take(", sizeof("s_string_concat_take(") - 1), s_record_117_get_text_take(s_record_117_copy(v_left))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_right))), s_string_from_bytes(")", sizeof(")") - 1)), 4);
			s_string_release(v_op);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_right_type);
			s_record_107_drop(v_left_type);
			s_record_116_drop(v_next);
			s_record_117_drop(v_right);
			s_record_117_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_697;
		}
		if (((s_record_117_get_kind_take(s_record_117_copy(v_left)) == 4) && ((((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("==", sizeof("==") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("!=", sizeof("!=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<", sizeof("<") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<=", sizeof("<=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">", sizeof(">") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">=", sizeof(">=") - 1)) == 0))))
		{
			s_record_117 s_return_698 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(s_string_compare_take(", sizeof("(s_string_compare_take(") - 1), s_record_117_get_text_take(s_record_117_copy(v_left))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_right))), s_string_from_bytes(") ", sizeof(") ") - 1)), s_string_retain(v_op)), s_string_from_bytes(" 0)", sizeof(" 0)") - 1)), 3);
			s_string_release(v_op);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_right_type);
			s_record_107_drop(v_left_type);
			s_record_116_drop(v_next);
			s_record_117_drop(v_right);
			s_record_117_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_698;
		}
		s_record_117 s_return_699 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_record_117_get_text_take(s_record_117_copy(v_left))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_op)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_right))), s_string_from_bytes(")", sizeof(")") - 1)), v_kind);
		s_string_release(v_op);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_right_type);
		s_record_107_drop(v_left_type);
		s_record_116_drop(v_next);
		s_record_117_drop(v_right);
		s_record_117_drop(v_left);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_699;
		s_string_release(v_op);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_right_type);
		s_record_107_drop(v_left_type);
		s_record_116_drop(v_next);
		s_record_117_drop(v_right);
		s_record_117_drop(v_left);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 10))
	{
		const int64_t v_callee = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee)) == 23))
		{
			const int64_t v_dict_kind = s_dict_constructor_kind(s_record_116_copy(v_state), v_callee);
			if ((v_dict_kind != 0))
			{
				if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) != 1))
				{
					s_record_117 s_return_700 = s_ExprCode(s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("dictionary constructor takes no arguments", sizeof("dictionary constructor takes no arguments") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_700;
				}
				s_record_116 s_arg_701 = s_register_dict(s_record_116_copy(v_state), v_dict_kind);
				S_String * s_arg_702 = s_string_concat_take(s_dict_helper(v_dict_kind), s_string_from_bytes("_make()", sizeof("_make()") - 1));
				int64_t s_arg_703 = v_dict_kind;
				s_record_117 s_return_704 = s_ExprCode(s_arg_701, s_arg_702, s_arg_703);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_704;
			}
			const int64_t v_kind = s_array_constructor_kind(s_record_116_copy(v_state), v_callee);
			if (((v_kind == 0) || (s_record_101_get_count_take(s_record_101_copy(v_entry)) != 1)))
			{
				s_record_117 s_return_705 = s_ExprCode(s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("unsupported array constructor", sizeof("unsupported array constructor") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_705;
			}
			if ((v_kind == 7))
			{
				s_record_117 s_return_706 = s_ExprCode(s_record_116_copy(v_state), s_string_from_bytes("s_array_i64_make(0, NULL)", sizeof("s_array_i64_make(0, NULL)") - 1), v_kind);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_706;
			}
			s_record_117 s_return_707 = s_ExprCode(s_record_116_copy(v_state), s_string_concat_take(s_array_helper(v_kind), s_string_from_bytes("_make_take(0, NULL)", sizeof("_make_take(0, NULL)") - 1)), v_kind);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_707;
		}
		if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee)) == 4))
		{
			S_String * v_method = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee);
			if (((((((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("slice", sizeof("slice") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("find", sizeof("find") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("starts_with", sizeof("starts_with") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("ends_with", sizeof("ends_with") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("join", sizeof("join") - 1)) == 0)))
			{
				const int64_t v_owner_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee, 0);
				int64_t v_effects = 0;
				if (s_has_effect(s_record_116_copy(v_state), v_owner_id, s_ga_8_make_take(0, NULL)))
				{
					v_effects += 1;
				}
				for (int64_t v_i = 1, s_bound_708 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_708; ++v_i)
				{
					if (s_has_effect(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i), s_ga_8_make_take(0, NULL)))
					{
						v_effects += 1;
					}
				}
				if ((v_effects > 1))
				{
					s_record_117 s_return_709 = s_ExprCode(s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("string method requires ordered argument evaluation", sizeof("string method requires ordered argument evaluation") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
					s_string_release(v_method);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_709;
				}
				s_record_117 v_receiver = s_emit_expr(s_record_116_copy(v_state), v_owner_id);
				s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_receiver));
				S_Array * v_args = s_ga_8_make_take(0, NULL);
				S_Array_i64 * v_kinds = s_array_i64_make(0, NULL);
				for (int64_t v_i = 1, s_bound_710 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_710; ++v_i)
				{
					s_record_117 v_item = s_emit_expr(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
					s_record_116 s_value_711 = s_record_117_get_state_take(s_record_117_copy(v_item));
					s_record_116_drop(v_next);
					v_next = s_value_711;
					S_String * s_item_712 = s_record_117_get_text_take(s_record_117_copy(v_item));
					v_args = s_ga_8_append_take(v_args, s_item_712);
					int64_t s_item_713 = s_record_117_get_kind_take(s_record_117_copy(v_item));
					v_kinds = s_array_i64_append_take(v_kinds, s_item_713);
					s_record_117_drop(v_item);
				}
				s_record_119 v_signature = s_call_type_ids(s_record_116_copy(v_next), s_array_i64_retain(v_kinds));
				s_record_106 s_field_714 = s_record_119_get_table_take(s_record_119_copy(v_signature));
				s_record_106_drop(v_next.types);
				v_next.types = s_field_714;
				s_record_107 v_owner = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_next)), s_record_116_get_records_take(s_record_116_copy(v_next)), s_record_117_get_kind_take(s_record_117_copy(v_receiver)));
				s_record_116 s_value_715 = s_check_hir_call(s_record_116_copy(v_next), v_id, s_resolve_hir_string_call(s_record_107_get_table_take(s_record_107_copy(v_owner)), s_record_107_get_id_take(s_record_107_copy(v_owner)), s_string_retain(v_method), s_record_119_get_ids_take(s_record_119_copy(v_signature))));
				s_record_116_drop(v_next);
				v_next = s_value_715;
				if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_117 s_return_716 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
					s_record_107_drop(v_owner);
					s_record_119_drop(v_signature);
					s_array_i64_release(v_kinds);
					s_array_release(v_args);
					s_record_116_drop(v_next);
					s_record_117_drop(v_receiver);
					s_string_release(v_method);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_716;
				}
				int64_t v_result_kind = 3;
				if (((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("find", sizeof("find") - 1)) == 0)))
				{
					v_result_kind = 6;
				}
				else if (((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("slice", sizeof("slice") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("join", sizeof("join") - 1)) == 0)))
				{
					v_result_kind = 4;
				}
				s_record_117 s_return_717 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_", sizeof("s_string_") - 1), s_string_retain(v_method)), s_string_from_bytes("_take(", sizeof("_take(") - 1)), s_record_117_get_text_take(s_record_117_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_args))), s_string_from_bytes(")", sizeof(")") - 1)), v_result_kind);
				s_record_107_drop(v_owner);
				s_record_119_drop(v_signature);
				s_array_i64_release(v_kinds);
				s_array_release(v_args);
				s_record_116_drop(v_next);
				s_record_117_drop(v_receiver);
				s_string_release(v_method);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_717;
				s_record_107_drop(v_owner);
				s_record_119_drop(v_signature);
				s_array_i64_release(v_kinds);
				s_array_release(v_args);
				s_record_116_drop(v_next);
				s_record_117_drop(v_receiver);
			}
			s_string_release(v_method);
		}
		if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee)) == 4) && ((s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee), s_string_from_bytes("insert", sizeof("insert") - 1)) == 0) || (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee), s_string_from_bytes("get", sizeof("get") - 1)) == 0))))
		{
			S_String * v_method = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee);
			s_record_117 v_receiver = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee, 0));
			s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_receiver));
			S_Array * v_arguments = s_ga_8_make_take(0, NULL);
			S_Array_i64 * v_kinds = s_array_i64_make(0, NULL);
			for (int64_t v_i = 1, s_bound_718 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_718; ++v_i)
			{
				s_record_117 v_argument = s_emit_expr(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
				s_record_116 s_value_719 = s_record_117_get_state_take(s_record_117_copy(v_argument));
				s_record_116_drop(v_next);
				v_next = s_value_719;
				S_String * s_item_720 = s_record_117_get_text_take(s_record_117_copy(v_argument));
				v_arguments = s_ga_8_append_take(v_arguments, s_item_720);
				int64_t s_item_721 = s_record_117_get_kind_take(s_record_117_copy(v_argument));
				v_kinds = s_array_i64_append_take(v_kinds, s_item_721);
				s_record_117_drop(v_argument);
			}
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_117 s_return_722 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_array_i64_release(v_kinds);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_record_117_drop(v_receiver);
				s_string_release(v_method);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_722;
			}
			s_record_119 v_signature = s_call_type_ids(s_record_116_copy(v_next), s_array_i64_retain(v_kinds));
			s_record_107 v_owner = s_canonical_kind(s_record_119_get_table_take(s_record_119_copy(v_signature)), s_record_116_get_records_take(s_record_116_copy(v_next)), s_record_117_get_kind_take(s_record_117_copy(v_receiver)));
			s_record_116 s_value_723 = s_check_hir_call(s_record_116_copy(v_next), v_id, s_resolve_hir_collection_call(s_record_107_get_table_take(s_record_107_copy(v_owner)), s_record_107_get_id_take(s_record_107_copy(v_owner)), s_string_retain(v_method), s_record_119_get_ids_take(s_record_119_copy(v_signature))));
			s_record_116_drop(v_next);
			v_next = s_value_723;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_117 s_return_724 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_107_drop(v_owner);
				s_record_119_drop(v_signature);
				s_array_i64_release(v_kinds);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_record_117_drop(v_receiver);
				s_string_release(v_method);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_724;
			}
			s_record_116 s_value_725 = s_register_dict(s_record_116_copy(v_next), s_record_117_get_kind_take(s_record_117_copy(v_receiver)));
			s_record_116_drop(v_next);
			v_next = s_value_725;
			if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("get", sizeof("get") - 1)) == 0))
			{
				s_record_116 s_arg_726 = s_record_116_copy(v_next);
				S_String * s_arg_727 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_dict_helper(s_record_117_get_kind_take(s_record_117_copy(v_receiver))), s_string_from_bytes("_get_take(", sizeof("_get_take(") - 1)), s_record_117_get_text_take(s_record_117_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1));
				int64_t s_arg_728 = s_optional_of(s_dict_value(s_record_117_get_kind_take(s_record_117_copy(v_receiver))));
				s_record_117 s_return_729 = s_ExprCode(s_arg_726, s_arg_727, s_arg_728);
				s_record_107_drop(v_owner);
				s_record_119_drop(v_signature);
				s_array_i64_release(v_kinds);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_record_117_drop(v_receiver);
				s_string_release(v_method);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_729;
			}
			S_String * s_left_730 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_dict_helper(s_record_117_get_kind_take(s_record_117_copy(v_receiver))), s_string_from_bytes("_insert_take(", sizeof("_insert_take(") - 1)), s_record_117_get_text_take(s_record_117_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1));
			S_String * s_right_731 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments));
			s_record_117 s_return_732 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_left_730, s_right_731), s_string_from_bytes(")", sizeof(")") - 1)), s_record_117_get_kind_take(s_record_117_copy(v_receiver)));
			s_record_107_drop(v_owner);
			s_record_119_drop(v_signature);
			s_array_i64_release(v_kinds);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_record_117_drop(v_receiver);
			s_string_release(v_method);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_732;
			s_record_107_drop(v_owner);
			s_record_119_drop(v_signature);
			s_array_i64_release(v_kinds);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_record_117_drop(v_receiver);
			s_string_release(v_method);
		}
		if ((((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee)) == 4) && (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee), s_string_from_bytes("append", sizeof("append") - 1)) == 0)) && (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2)))
		{
			s_record_117 v_receiver = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee, 0));
			s_record_117 v_item = s_emit_expr(s_record_117_get_state_take(s_record_117_copy(v_receiver)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
			s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_item));
			s_record_119 v_signature = s_call_type_ids(s_record_116_copy(v_next), s_array_i64_make(1, (int64_t[]){s_record_117_get_kind_take(s_record_117_copy(v_item))}));
			s_record_107 v_owner = s_canonical_kind(s_record_119_get_table_take(s_record_119_copy(v_signature)), s_record_116_get_records_take(s_record_116_copy(v_next)), s_record_117_get_kind_take(s_record_117_copy(v_receiver)));
			s_record_116 s_value_733 = s_check_hir_call(s_record_116_copy(v_next), v_id, s_resolve_hir_collection_call(s_record_107_get_table_take(s_record_107_copy(v_owner)), s_record_107_get_id_take(s_record_107_copy(v_owner)), s_string_from_bytes("append", sizeof("append") - 1), s_record_119_get_ids_take(s_record_119_copy(v_signature))));
			s_record_116_drop(v_next);
			v_next = s_value_733;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_117 s_return_734 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_107_drop(v_owner);
				s_record_119_drop(v_signature);
				s_record_116_drop(v_next);
				s_record_117_drop(v_item);
				s_record_117_drop(v_receiver);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_734;
			}
			if ((s_record_117_get_kind_take(s_record_117_copy(v_receiver)) == 7))
			{
				s_record_117 s_return_735 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_append_take(", sizeof("s_array_i64_append_take(") - 1), s_record_117_get_text_take(s_record_117_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_item))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_117_get_kind_take(s_record_117_copy(v_receiver)));
				s_record_107_drop(v_owner);
				s_record_119_drop(v_signature);
				s_record_116_drop(v_next);
				s_record_117_drop(v_item);
				s_record_117_drop(v_receiver);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_735;
			}
			s_record_117 s_return_736 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(s_record_117_get_kind_take(s_record_117_copy(v_receiver))), s_string_from_bytes("_append_take(", sizeof("_append_take(") - 1)), s_record_117_get_text_take(s_record_117_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_item))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_117_get_kind_take(s_record_117_copy(v_receiver)));
			s_record_107_drop(v_owner);
			s_record_119_drop(v_signature);
			s_record_116_drop(v_next);
			s_record_117_drop(v_item);
			s_record_117_drop(v_receiver);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_736;
			s_record_107_drop(v_owner);
			s_record_119_drop(v_signature);
			s_record_116_drop(v_next);
			s_record_117_drop(v_item);
			s_record_117_drop(v_receiver);
		}
		if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee)) != 29))
		{
			s_record_117 s_return_737 = s_ExprCode(s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("only named calls are supported", sizeof("only named calls are supported") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_737;
		}
		S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee);
		int64_t v_calls = 0;
		for (int64_t v_i = 1, s_bound_738 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_738; ++v_i)
		{
			if (s_has_effect(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i), s_ga_8_make_take(0, NULL)))
			{
				v_calls += 1;
			}
		}
		s_record_116 v_next = s_record_116_copy(v_state);
		S_Array * v_arguments = s_ga_8_make_take(0, NULL);
		S_Array_i64 * v_arg_types = s_array_i64_make(0, NULL);
		for (int64_t v_i = 1, s_bound_739 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_739; ++v_i)
		{
			s_record_117 v_arg = s_emit_expr(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
			s_record_116 s_value_740 = s_record_117_get_state_take(s_record_117_copy(v_arg));
			s_record_116_drop(v_next);
			v_next = s_value_740;
			S_String * v_expression = s_record_117_get_text_take(s_record_117_copy(v_arg));
			if ((((v_calls > 1) && (s_record_117_get_kind_take(s_record_117_copy(v_arg)) != 0)) && (s_record_117_get_kind_take(s_record_117_copy(v_arg)) != 5)))
			{
				S_String * v_temporary = s_string_concat_take(s_string_from_bytes("s_arg_", sizeof("s_arg_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_next))));
				v_next.counter += 1;
				s_record_116 s_value_741 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_arg))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temporary)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_arg))), s_string_from_bytes(";", sizeof(";") - 1)));
				s_record_116_drop(v_next);
				v_next = s_value_741;
				S_String *s_value_742 = s_string_retain(v_temporary);
				s_string_release(v_expression);
				v_expression = s_value_742;
				s_string_release(v_temporary);
			}
			S_String * s_item_743 = s_string_retain(v_expression);
			v_arguments = s_ga_8_append_take(v_arguments, s_item_743);
			int64_t s_item_744 = s_record_117_get_kind_take(s_record_117_copy(v_arg));
			v_arg_types = s_array_i64_append_take(v_arg_types, s_item_744);
			s_string_release(v_expression);
			s_record_117_drop(v_arg);
		}
		if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_117 s_return_745 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_745;
		}
		if (s_hir_is_builtin(s_string_retain(v_name)))
		{
			s_record_119 v_signature = s_call_type_ids(s_record_116_copy(v_next), s_array_i64_retain(v_arg_types));
			s_record_116 s_value_746 = s_check_hir_call(s_record_116_copy(v_next), v_id, s_resolve_hir_builtin_call(s_record_119_get_table_take(s_record_119_copy(v_signature)), s_string_retain(v_name), s_record_119_get_ids_take(s_record_119_copy(v_signature))));
			s_record_116_drop(v_next);
			v_next = s_value_746;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_117 s_return_747 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_119_drop(v_signature);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_747;
			}
			s_record_119_drop(v_signature);
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("arguments", sizeof("arguments") - 1)) == 0))
		{
			s_record_117 s_return_748 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("s_arguments()", sizeof("s_arguments()") - 1), 8);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_748;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("read_text", sizeof("read_text") - 1)) == 0))
		{
			s_record_117 s_return_749 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_read_text_take(", sizeof("s_read_text_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), s_optional_of(4));
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_749;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("write_text", sizeof("write_text") - 1)) == 0))
		{
			s_record_117 s_return_750 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_write_text_take(", sizeof("s_write_text_take(") - 1), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments))), s_string_from_bytes(")", sizeof(")") - 1)), 3);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_750;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0))
		{
			S_String * v_printed_arg = s_ga_8_get_checked_take(s_array_retain(v_arguments), 0);
			const int64_t v_kind = s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0);
			if ((v_kind == 1))
			{
				s_record_117 s_return_751 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("printf(\"%lld\\n\", (long long)(", sizeof("printf(\"%lld\\n\", (long long)(") - 1), s_string_retain(v_printed_arg)), s_string_from_bytes("))", sizeof("))") - 1)), 5);
				s_string_release(v_printed_arg);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_751;
			}
			if ((v_kind == 2))
			{
				s_record_117 s_return_752 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("printf(\"%.17g\\n\", ", sizeof("printf(\"%.17g\\n\", ") - 1), s_string_retain(v_printed_arg)), s_string_from_bytes(")", sizeof(")") - 1)), 5);
				s_string_release(v_printed_arg);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_752;
			}
			if ((v_kind == 3))
			{
				s_record_117 s_return_753 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("printf(\"%s\\n\", ", sizeof("printf(\"%s\\n\", ") - 1), s_string_retain(v_printed_arg)), s_string_from_bytes(" ? \"true\" : \"false\")", sizeof(" ? \"true\" : \"false\")") - 1)), 5);
				s_string_release(v_printed_arg);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_753;
			}
			if ((v_kind == 4))
			{
				s_record_117 s_return_754 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_print_string_take(", sizeof("s_print_string_take(") - 1), s_string_retain(v_printed_arg)), s_string_from_bytes(")", sizeof(")") - 1)), 5);
				s_string_release(v_printed_arg);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_754;
			}
			s_record_117 s_return_755 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("cannot print this type", sizeof("cannot print this type") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_printed_arg);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_755;
			s_string_release(v_printed_arg);
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("len", sizeof("len") - 1)) == 0))
		{
			if ((s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0) == 4))
			{
				s_record_117 s_return_756 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_length_take(", sizeof("s_string_length_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 1);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_756;
			}
			if ((s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0) == 7))
			{
				s_record_117 s_return_757 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_length_take(", sizeof("s_array_i64_length_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 1);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_757;
			}
			if (s_generic_array(s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0)))
			{
				s_record_117 s_return_758 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_generic_array_length_take(", sizeof("s_generic_array_length_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 1);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_758;
			}
			if (s_is_dict(s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0)))
			{
				s_record_117 s_return_759 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_dict_length_take(", sizeof("s_dict_length_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 1);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_116_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_759;
			}
			s_record_117 s_return_760 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("len expects a string or array", sizeof("len expects a string or array") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_760;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("str", sizeof("str") - 1)) == 0))
		{
			s_record_117 s_return_761 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_from_i64(", sizeof("s_string_from_i64(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 4);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_761;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("sqrt", sizeof("sqrt") - 1)) == 0))
		{
			s_record_117 s_return_762 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("sqrt(", sizeof("sqrt(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 2);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_762;
		}
		const int64_t v_index = s_find_function(s_record_116_copy(v_next), s_string_retain(v_name));
		if ((v_index < 0))
		{
			s_record_117 s_return_763 = s_ExprCode(s_emit_error(s_record_116_copy(v_next), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown function '", sizeof("unknown function '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_763;
		}
		s_record_112 v_function = s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_next)), v_index);
		s_record_119 v_signature = s_call_type_ids(s_record_116_copy(v_next), s_array_i64_retain(v_arg_types));
		s_record_106 s_field_764 = s_record_119_get_table_take(s_record_119_copy(v_signature));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_764;
		S_Array_i64 * v_parameter_ids = s_array_i64_make(0, NULL);
		{
			S_Array * v_s_iter_765 = s_record_112_get_params_take(s_record_112_copy(v_function));
			for (int64_t s_index_765 = 0; s_index_765 < (int64_t)s_array_length(v_s_iter_765); ++s_index_765)
			{
				s_record_104 v_parameter = s_ga_10104_get_borrow(v_s_iter_765, s_index_765);
				s_record_107 v_canonical = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_next)), s_record_116_get_records_take(s_record_116_copy(v_next)), s_record_104_get_kind_take(s_record_104_copy(v_parameter)));
				s_record_106 s_field_766 = s_record_107_get_table_take(s_record_107_copy(v_canonical));
				s_record_106_drop(v_next.types);
				v_next.types = s_field_766;
				int64_t s_item_767 = s_record_107_get_id_take(s_record_107_copy(v_canonical));
				v_parameter_ids = s_array_i64_append_take(v_parameter_ids, s_item_767);
				s_record_107_drop(v_canonical);
				s_record_104_drop(v_parameter);
			}
			s_array_release(v_s_iter_765);
		}
		s_record_107 v_ret = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_next)), s_record_116_get_records_take(s_record_116_copy(v_next)), s_record_112_get_kind_take(s_record_112_copy(v_function)));
		s_record_116 s_value_768 = s_check_hir_call(s_record_116_copy(v_next), v_id, s_resolve_hir_function_call(s_record_107_get_table_take(s_record_107_copy(v_ret)), s_string_retain(v_name), s_record_119_get_ids_take(s_record_119_copy(v_signature)), s_array_i64_retain(v_parameter_ids), s_record_107_get_id_take(s_record_107_copy(v_ret))));
		s_record_116_drop(v_next);
		v_next = s_value_768;
		if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_117 s_return_769 = s_ExprCode(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_107_drop(v_ret);
			s_array_i64_release(v_parameter_ids);
			s_record_119_drop(v_signature);
			s_record_112_drop(v_function);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_769;
		}
		if (s_is_generic(s_record_112_copy(v_function)))
		{
			s_record_117 v_specialized = s_specialize_call(s_record_116_copy(v_next), v_id, s_record_112_copy(v_function), s_array_i64_retain(v_arg_types), s_array_retain(v_arguments));
			s_record_116 v_result = s_record_117_get_state_take(s_record_117_copy(v_specialized));
			if (((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (s_record_117_get_kind_take(s_record_117_copy(v_specialized)) != 0)))
			{
				s_record_107 v_resolved = s_canonical_kind(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_records_take(s_record_116_copy(v_result)), s_record_117_get_kind_take(s_record_117_copy(v_specialized)));
				s_record_106 s_field_770 = s_record_107_get_table_take(s_record_107_copy(v_resolved));
				s_record_106_drop(v_result.types);
				v_result.types = s_field_770;
				S_Array * s_field_771 = s_ga_10108_append_take(s_record_116_get_hir_take(s_record_116_copy(v_result)), s_HirNode(v_id, 6, s_record_107_get_id_take(s_record_107_copy(v_resolved)), (-1)));
				s_array_release(v_result.hir);
				v_result.hir = s_field_771;
				s_record_107_drop(v_resolved);
			}
			s_record_117 s_return_772 = s_ExprCode(s_record_116_copy(v_result), s_record_117_get_text_take(s_record_117_copy(v_specialized)), s_record_117_get_kind_take(s_record_117_copy(v_specialized)));
			s_record_116_drop(v_result);
			s_record_117_drop(v_specialized);
			s_record_107_drop(v_ret);
			s_array_i64_release(v_parameter_ids);
			s_record_119_drop(v_signature);
			s_record_112_drop(v_function);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_116_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_772;
			s_record_116_drop(v_result);
			s_record_117_drop(v_specialized);
		}
		s_record_117 s_return_773 = s_ExprCode(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_record_112_get_symbol_take(s_record_112_copy(v_function)), s_string_from_bytes("(", sizeof("(") - 1)), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_112_get_kind_take(s_record_112_copy(v_function)));
		s_record_107_drop(v_ret);
		s_array_i64_release(v_parameter_ids);
		s_record_119_drop(v_signature);
		s_record_112_drop(v_function);
		s_array_i64_release(v_arg_types);
		s_array_release(v_arguments);
		s_record_116_drop(v_next);
		s_string_release(v_name);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_773;
		s_record_107_drop(v_ret);
		s_array_i64_release(v_parameter_ids);
		s_record_119_drop(v_signature);
		s_record_112_drop(v_function);
		s_array_i64_release(v_arg_types);
		s_array_release(v_arguments);
		s_record_116_drop(v_next);
		s_string_release(v_name);
	}
	s_record_117 s_return_774 = s_ExprCode(s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unsupported expression '", sizeof("unsupported expression '") - 1), s_node_kind_name(s_record_101_get_kind_take(s_record_101_copy(v_entry)))), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
	s_string_release(v_value);
	s_record_101_drop(v_entry);
	s_record_116_drop(v_state);
	return s_return_774;
	s_string_release(v_value);
	s_record_101_drop(v_entry);
}

bool s_contains_recur(s_record_102 v_tree, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_102_copy(v_tree), v_id);
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 34))
	{
		bool s_return_775 = true;
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_775;
	}
	for (int64_t v_i = 0, s_bound_776 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_776; ++v_i)
	{
		if (s_contains_recur(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, v_i)))
		{
			bool s_return_777 = true;
			s_record_101_drop(v_entry);
			s_record_102_drop(v_tree);
			return s_return_777;
		}
	}
	bool s_return_778 = false;
	s_record_101_drop(v_entry);
	s_record_102_drop(v_tree);
	return s_return_778;
	s_record_101_drop(v_entry);
}

s_record_116 s_emit_block(s_record_116 v_state, int64_t v_id)
{
	const int64_t v_count = s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)));
	s_record_116 v_next = s_record_116_copy(v_state);
	s_record_101 v_block = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	for (int64_t v_i = 0, s_bound_779 = s_record_101_get_count_take(s_record_101_copy(v_block)); v_i < s_bound_779; ++v_i)
	{
		s_record_116 s_value_780 = s_emit_statement(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_id, v_i));
		s_record_116_drop(v_next);
		v_next = s_value_780;
		if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_116 s_return_781 = s_record_116_copy(v_next);
			s_record_101_drop(v_block);
			s_record_116_drop(v_next);
			s_record_116_drop(v_state);
			return s_return_781;
		}
	}
	for (int64_t v_i = v_count, s_bound_782 = s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_next))); v_i < s_bound_782; ++v_i)
	{
		const int64_t v_index = (((s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_next))) + v_count) - v_i) - 1);
		s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)), v_index);
		if ((s_is_optional(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) && s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))))
		{
			s_record_116 s_value_783 = s_line(s_record_116_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
			s_record_116_drop(v_next);
			v_next = s_value_783;
		}
		if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 4))
		{
			s_record_116 s_value_784 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_release(v_", sizeof("s_string_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_784;
		}
		if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 7))
		{
			s_record_116 s_value_785 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_release(v_", sizeof("s_array_i64_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_785;
		}
		if (s_generic_array(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
		{
			s_record_116 s_value_786 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_release(v_", sizeof("s_array_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_786;
		}
		if (s_is_dict(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
		{
			s_record_116 s_value_787 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_dict_release(v_", sizeof("s_dict_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_787;
		}
		if (((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) >= 100) && (s_record_110_get_kind_take(s_record_110_copy(v_symbol)) < 10000)))
		{
			s_record_116 s_value_788 = s_line(s_record_116_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
			s_record_116_drop(v_next);
			v_next = s_value_788;
		}
		s_record_110_drop(v_symbol);
	}
	S_Array * v_restored = s_ga_10110_make_take(0, NULL);
	for (int64_t v_i = 0, s_bound_789 = v_count; v_i < s_bound_789; ++v_i)
	{
		s_record_110 s_item_790 = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)), v_i);
		v_restored = s_ga_10110_append_take(v_restored, s_item_790);
	}
	S_Array * s_field_791 = s_array_retain(v_restored);
	s_array_release(v_next.symbols);
	v_next.symbols = s_field_791;
	s_record_116 s_return_792 = s_record_116_copy(v_next);
	s_array_release(v_restored);
	s_record_101_drop(v_block);
	s_record_116_drop(v_next);
	s_record_116_drop(v_state);
	return s_return_792;
	s_array_release(v_restored);
	s_record_101_drop(v_block);
	s_record_116_drop(v_next);
}

s_record_116 s_emit_nested_block(s_record_116 v_state, int64_t v_id)
{
	s_record_116 v_next = s_line(s_record_116_copy(v_state), s_string_from_bytes("{", sizeof("{") - 1));
	v_next.indentation += 1;
	v_next.scope_depth += 1;
	s_record_116 s_value_793 = s_emit_block(s_record_116_copy(v_next), v_id);
	s_record_116_drop(v_next);
	v_next = s_value_793;
	v_next.scope_depth -= 1;
	v_next.indentation -= 1;
	s_record_116 s_return_794 = s_line(s_record_116_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
	s_record_116_drop(v_next);
	s_record_116_drop(v_state);
	return s_return_794;
	s_record_116_drop(v_next);
}

s_record_116 s_emit_statement(s_record_116 v_state, int64_t v_id)
{
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_116 s_return_795 = s_record_116_copy(v_state);
		s_record_116_drop(v_state);
		return s_return_795;
	}
	s_record_101 v_item = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 26) || (s_record_101_get_kind_take(s_record_101_copy(v_item)) == 40)))
	{
		S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
		const int64_t v_previous = s_find_symbol(s_record_116_copy(v_state), s_string_retain(v_name));
		if (((v_previous >= 0) && (s_record_110_get_depth_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_previous)) == s_record_116_get_scope_depth_take(s_record_116_copy(v_state)))))
		{
			s_record_116 s_return_796 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("duplicate binding '", sizeof("duplicate binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_796;
		}
		int64_t v_kind = 0;
		int64_t v_expr_id = (-1);
		for (int64_t v_i = 0, s_bound_797 = s_record_101_get_count_take(s_record_101_copy(v_item)); v_i < s_bound_797; ++v_i)
		{
			const int64_t v_child = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i);
			if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_child)) == 38))
			{
				v_kind = s_resolve_type(s_record_116_copy(v_state), v_child);
			}
			else
			{
				v_expr_id = v_child;
			}
		}
		if ((v_expr_id < 0))
		{
			s_record_116 s_return_798 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("binding requires initializer", sizeof("binding requires initializer") - 1));
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_798;
		}
		s_record_117 v_result = s_emit_expr(s_record_116_copy(v_state), v_expr_id);
		s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_result));
		if ((v_kind == 0))
		{
			v_kind = s_record_117_get_kind_take(s_record_117_copy(v_result));
		}
		s_record_116 s_value_799 = s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_result)), v_kind);
		s_record_116_drop(v_next);
		v_next = s_value_799;
		if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_116 s_return_800 = s_record_116_copy(v_next);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_800;
		}
		S_String * v_declaration = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(v_kind), s_string_from_bytes(" v_", sizeof(" v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1));
		if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 26) && (!s_managed(v_kind))))
		{
			S_String *s_value_801 = s_string_concat_take(s_string_from_bytes("const ", sizeof("const ") - 1), s_string_retain(v_declaration));
			s_string_release(v_declaration);
			v_declaration = s_value_801;
		}
		s_record_116 s_value_802 = s_line(s_record_116_copy(v_next), s_string_retain(v_declaration));
		s_record_116_drop(v_next);
		v_next = s_value_802;
		s_record_116 s_return_803 = s_append_symbol(s_record_116_copy(v_next), v_id, s_string_retain(v_name), v_kind, (s_record_101_get_kind_take(s_record_101_copy(v_item)) == 40));
		s_string_release(v_declaration);
		s_record_116_drop(v_next);
		s_record_117_drop(v_result);
		s_string_release(v_name);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_803;
		s_string_release(v_declaration);
		s_record_116_drop(v_next);
		s_record_117_drop(v_result);
		s_string_release(v_name);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 3))
	{
		const int64_t v_target = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_target)) == 12))
		{
			if (((s_record_101_get_count_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_target)) != 2) || (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0)))
			{
				s_record_116 s_return_804 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("unsupported indexed assignment", sizeof("unsupported indexed assignment") - 1));
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_804;
			}
			const int64_t v_root = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_target, 0);
			if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root)) == 4))
			{
				const int64_t v_owner_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root, 0);
				if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_owner_id)) != 29))
				{
					s_record_116 s_return_805 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
					s_record_101_drop(v_item);
					s_record_116_drop(v_state);
					return s_return_805;
				}
				S_String * v_owner = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_owner_id);
				const int64_t v_owner_index = s_find_symbol(s_record_116_copy(v_state), s_string_retain(v_owner));
				if (((v_owner_index < 0) || (!s_record_110_get_mutable_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_owner_index)))))
				{
					s_record_116 s_return_806 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
					s_string_release(v_owner);
					s_record_101_drop(v_item);
					s_record_116_drop(v_state);
					return s_return_806;
				}
				S_String * v_field_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root);
				const int64_t v_field_index = s_record_field_index(s_record_116_copy(v_state), s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_owner_index)), s_string_retain(v_field_name));
				if ((v_field_index < 0))
				{
					s_record_116 s_return_807 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("indexed update requires an array field", sizeof("indexed update requires an array field") - 1));
					s_string_release(v_field_name);
					s_string_release(v_owner);
					s_record_101_drop(v_item);
					s_record_116_drop(v_state);
					return s_return_807;
				}
				const int64_t v_array_kind = s_record_104_get_kind_take(s_ga_10104_get_checked_take(s_record_115_get_fields_take(s_ga_10115_get_checked_take(s_record_116_get_records_take(s_record_116_copy(v_state)), (s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_owner_index)) - 100))), v_field_index));
				const int64_t v_element_kind = s_array_element(v_array_kind);
				if ((v_element_kind == 0))
				{
					s_record_116 s_return_808 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("indexed update requires an array", sizeof("indexed update requires an array") - 1));
					s_string_release(v_field_name);
					s_string_release(v_owner);
					s_record_101_drop(v_item);
					s_record_116_drop(v_state);
					return s_return_808;
				}
				s_record_117 v_index = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_target, 1));
				s_record_117 v_item_value = s_emit_expr(s_record_117_get_state_take(s_record_117_copy(v_index)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
				s_record_116 v_next = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_item_value)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_index)), 1);
				s_record_116 s_value_809 = s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_item_value)), v_element_kind);
				s_record_116_drop(v_next);
				v_next = s_value_809;
				S_String * v_pointer = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("&v_", sizeof("&v_") - 1), s_string_retain(v_owner)), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_field_name));
				if ((v_array_kind == 7))
				{
					s_record_116 s_return_810 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_set(", sizeof("s_array_i64_set(") - 1), s_string_retain(v_pointer)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_index))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_item_value))), s_string_from_bytes(");", sizeof(");") - 1)));
					s_string_release(v_pointer);
					s_record_116_drop(v_next);
					s_record_117_drop(v_item_value);
					s_record_117_drop(v_index);
					s_string_release(v_field_name);
					s_string_release(v_owner);
					s_record_101_drop(v_item);
					s_record_116_drop(v_state);
					return s_return_810;
				}
				s_record_116 s_return_811 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(v_array_kind), s_string_from_bytes("_set_take(", sizeof("_set_take(") - 1)), s_string_retain(v_pointer)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_index))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_item_value))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_string_release(v_pointer);
				s_record_116_drop(v_next);
				s_record_117_drop(v_item_value);
				s_record_117_drop(v_index);
				s_string_release(v_field_name);
				s_string_release(v_owner);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_811;
				s_string_release(v_pointer);
				s_record_116_drop(v_next);
				s_record_117_drop(v_item_value);
				s_record_117_drop(v_index);
				s_string_release(v_field_name);
				s_string_release(v_owner);
			}
			if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root)) != 29))
			{
				s_record_116 s_return_812 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_812;
			}
			S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root);
			const int64_t v_symbol = s_find_symbol(s_record_116_copy(v_state), s_string_retain(v_name));
			if (((v_symbol < 0) || (!s_record_110_get_mutable_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_symbol)))))
			{
				s_record_116 s_return_813 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_813;
			}
			const int64_t v_array_kind = s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_symbol));
			const int64_t v_element_kind = s_array_element(v_array_kind);
			if ((v_element_kind == 0))
			{
				s_record_116 s_return_814 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("indexed update requires an array", sizeof("indexed update requires an array") - 1));
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_814;
			}
			s_record_117 v_index = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_target, 1));
			s_record_117 v_item_value = s_emit_expr(s_record_117_get_state_take(s_record_117_copy(v_index)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
			s_record_116 v_next = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_item_value)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_index)), 1);
			s_record_116 s_value_815 = s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_item_value)), v_element_kind);
			s_record_116_drop(v_next);
			v_next = s_value_815;
			if ((v_array_kind == 7))
			{
				s_record_116 s_return_816 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_set(&v_", sizeof("s_array_i64_set(&v_") - 1), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_index))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_item_value))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_116_drop(v_next);
				s_record_117_drop(v_item_value);
				s_record_117_drop(v_index);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_816;
			}
			s_record_116 s_return_817 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(v_array_kind), s_string_from_bytes("_set_take(&v_", sizeof("_set_take(&v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_index))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_item_value))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_116_drop(v_next);
			s_record_117_drop(v_item_value);
			s_record_117_drop(v_index);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_817;
			s_record_116_drop(v_next);
			s_record_117_drop(v_item_value);
			s_record_117_drop(v_index);
			s_string_release(v_name);
		}
		if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_target)) == 4))
		{
			const int64_t v_root = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_target, 0);
			if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root)) != 29))
			{
				s_record_116 s_return_818 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("record update requires a local root binding", sizeof("record update requires a local root binding") - 1));
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_818;
			}
			S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root);
			const int64_t v_index = s_find_symbol(s_record_116_copy(v_state), s_string_retain(v_name));
			if ((v_index < 0))
			{
				s_record_116 s_return_819 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown binding '", sizeof("unknown binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_819;
			}
			s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_index);
			if ((!s_record_110_get_mutable_take(s_record_110_copy(v_symbol))))
			{
				s_record_116 s_return_820 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("cannot assign immutable binding '", sizeof("cannot assign immutable binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_820;
			}
			S_String * v_field = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_target);
			const int64_t v_member = s_record_field_index(s_record_116_copy(v_state), s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_retain(v_field));
			if ((v_member < 0))
			{
				s_record_116 s_return_821 = s_emit_error(s_record_116_copy(v_state), v_target, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown record field '", sizeof("unknown record field '") - 1), s_string_retain(v_field)), s_string_from_bytes("'", sizeof("'") - 1)));
				s_string_release(v_field);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_821;
			}
			const int64_t v_kind = s_record_104_get_kind_take(s_ga_10104_get_checked_take(s_record_115_get_fields_take(s_ga_10115_get_checked_take(s_record_116_get_records_take(s_record_116_copy(v_state)), (s_record_110_get_kind_take(s_record_110_copy(v_symbol)) - 100))), v_member));
			s_record_117 v_result = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
			s_record_116 v_next = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_result)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_result)), v_kind);
			S_String * v_op = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
			if (s_managed(v_kind))
			{
				if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
				{
					s_record_116 s_return_822 = s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("managed record field update requires =", sizeof("managed record field update requires =") - 1));
					s_string_release(v_op);
					s_record_116_drop(v_next);
					s_record_117_drop(v_result);
					s_string_release(v_field);
					s_record_110_drop(v_symbol);
					s_string_release(v_name);
					s_record_101_drop(v_item);
					s_record_116_drop(v_state);
					return s_return_822;
				}
				S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_field_", sizeof("s_field_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_next))));
				v_next.counter += 1;
				s_record_116 s_value_823 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(v_kind), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
				s_record_116_drop(v_next);
				v_next = s_value_823;
				s_record_116 s_value_824 = s_line(s_record_116_copy(v_next), s_record_drop(v_kind, s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_field))));
				s_record_116_drop(v_next);
				v_next = s_value_824;
				s_record_116 s_return_825 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_field)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
				s_string_release(v_temp);
				s_string_release(v_op);
				s_record_116_drop(v_next);
				s_record_117_drop(v_result);
				s_string_release(v_field);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_825;
				s_string_release(v_temp);
			}
			s_record_116 s_return_826 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_field)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_op)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_op);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_string_release(v_field);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_826;
			s_string_release(v_op);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_string_release(v_field);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
		}
		const int64_t v_lhs = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		const int64_t v_rhs = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1);
		if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_lhs)) != 29))
		{
			s_record_116 s_return_827 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("only local-variable assignment is supported", sizeof("only local-variable assignment is supported") - 1));
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_827;
		}
		S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_lhs);
		const int64_t v_index = s_find_symbol(s_record_116_copy(v_state), s_string_retain(v_name));
		if ((v_index < 0))
		{
			s_record_116 s_return_828 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown binding '", sizeof("unknown binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_828;
		}
		s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_state)), v_index);
		if ((!s_record_110_get_mutable_take(s_record_110_copy(v_symbol))))
		{
			s_record_116 s_return_829 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("cannot assign immutable binding '", sizeof("cannot assign immutable binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_829;
		}
		if (((s_array_element(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) != 0) && (s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs)) == 10)))
		{
			s_record_101 v_call = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs);
			if ((s_record_101_get_count_take(s_record_101_copy(v_call)) == 2))
			{
				const int64_t v_method = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs, 0);
				if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_method)) == 4) && (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_method), s_string_from_bytes("append", sizeof("append") - 1)) == 0)))
				{
					const int64_t v_receiver = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_method, 0);
					if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_receiver)) == 29) && (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_receiver), s_string_retain(v_name)) == 0)))
					{
						s_record_117 v_element = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs, 1));
						s_record_116 v_next = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_element)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_element)), s_array_element(s_record_110_get_kind_take(s_record_110_copy(v_symbol))));
						if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
						{
							s_record_116 s_return_830 = s_record_116_copy(v_next);
							s_record_116_drop(v_next);
							s_record_117_drop(v_element);
							s_record_101_drop(v_call);
							s_record_110_drop(v_symbol);
							s_string_release(v_name);
							s_record_101_drop(v_item);
							s_record_116_drop(v_state);
							return s_return_830;
						}
						const int64_t v_serial = s_record_116_get_counter_take(s_record_116_copy(v_next));
						v_next.counter += 1;
						S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_item_", sizeof("s_item_") - 1), s_string_from_i64(v_serial));
						s_record_116 s_value_831 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_element))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_element))), s_string_from_bytes(";", sizeof(";") - 1)));
						s_record_116_drop(v_next);
						v_next = s_value_831;
						if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 7))
						{
							s_record_116 s_return_832 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = s_array_i64_append_take(v_", sizeof(" = s_array_i64_append_take(v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(");", sizeof(");") - 1)));
							s_string_release(v_temp);
							s_record_116_drop(v_next);
							s_record_117_drop(v_element);
							s_record_101_drop(v_call);
							s_record_110_drop(v_symbol);
							s_string_release(v_name);
							s_record_101_drop(v_item);
							s_record_116_drop(v_state);
							return s_return_832;
						}
						s_record_116 s_return_833 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_array_helper(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))), s_string_from_bytes("_append_take(v_", sizeof("_append_take(v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(");", sizeof(");") - 1)));
						s_string_release(v_temp);
						s_record_116_drop(v_next);
						s_record_117_drop(v_element);
						s_record_101_drop(v_call);
						s_record_110_drop(v_symbol);
						s_string_release(v_name);
						s_record_101_drop(v_item);
						s_record_116_drop(v_state);
						return s_return_833;
						s_string_release(v_temp);
						s_record_116_drop(v_next);
						s_record_117_drop(v_element);
					}
				}
			}
			s_record_101_drop(v_call);
		}
		if ((s_is_dict(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) && (s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs)) == 10)))
		{
			s_record_101 v_call = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs);
			if ((s_record_101_get_count_take(s_record_101_copy(v_call)) == 3))
			{
				const int64_t v_method = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs, 0);
				if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_method)) == 4) && (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_method), s_string_from_bytes("insert", sizeof("insert") - 1)) == 0)))
				{
					const int64_t v_receiver = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_method, 0);
					if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_receiver)) == 29) && (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_receiver), s_string_retain(v_name)) == 0)))
					{
						s_record_117 v_key = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs, 1));
						s_record_117 v_value = s_emit_expr(s_record_117_get_state_take(s_record_117_copy(v_key)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_rhs, 2));
						s_record_116 v_next = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_value)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_key)), s_dict_key(s_record_110_get_kind_take(s_record_110_copy(v_symbol))));
						s_record_116 s_value_834 = s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_value)), s_dict_value(s_record_110_get_kind_take(s_record_110_copy(v_symbol))));
						s_record_116_drop(v_next);
						v_next = s_value_834;
						if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
						{
							s_record_116 s_return_835 = s_record_116_copy(v_next);
							s_record_116_drop(v_next);
							s_record_117_drop(v_value);
							s_record_117_drop(v_key);
							s_record_101_drop(v_call);
							s_record_110_drop(v_symbol);
							s_string_release(v_name);
							s_record_101_drop(v_item);
							s_record_116_drop(v_state);
							return s_return_835;
						}
						const int64_t v_serial = s_record_116_get_counter_take(s_record_116_copy(v_next));
						v_next.counter += 1;
						S_String * v_k = s_string_concat_take(s_string_from_bytes("s_key_", sizeof("s_key_") - 1), s_string_from_i64(v_serial));
						S_String * v_v = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(v_serial));
						s_record_116 s_value_836 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_key))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_k)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_key))), s_string_from_bytes(";", sizeof(";") - 1)));
						s_record_116_drop(v_next);
						v_next = s_value_836;
						s_record_116 s_value_837 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_value))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_v)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_value))), s_string_from_bytes(";", sizeof(";") - 1)));
						s_record_116_drop(v_next);
						v_next = s_value_837;
						s_record_116 s_return_838 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_dict_helper(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))), s_string_from_bytes("_insert_take(v_", sizeof("_insert_take(v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_k)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_v)), s_string_from_bytes(");", sizeof(");") - 1)));
						s_string_release(v_v);
						s_string_release(v_k);
						s_record_116_drop(v_next);
						s_record_117_drop(v_value);
						s_record_117_drop(v_key);
						s_record_101_drop(v_call);
						s_record_110_drop(v_symbol);
						s_string_release(v_name);
						s_record_101_drop(v_item);
						s_record_116_drop(v_state);
						return s_return_838;
						s_string_release(v_v);
						s_string_release(v_k);
						s_record_116_drop(v_next);
						s_record_117_drop(v_value);
						s_record_117_drop(v_key);
					}
				}
			}
			s_record_101_drop(v_call);
		}
		s_record_117 v_result = s_emit_expr(s_record_116_copy(v_state), v_rhs);
		s_record_116 v_next = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_result)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_result)), s_record_110_get_kind_take(s_record_110_copy(v_symbol)));
		if ((s_array_element(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) != 0))
		{
			if ((s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
			{
				s_record_116 s_return_839 = s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("unsupported array assignment", sizeof("unsupported array assignment") - 1));
				s_record_116_drop(v_next);
				s_record_117_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_839;
			}
			const int64_t v_serial = s_record_116_get_counter_take(s_record_116_copy(v_next));
			v_next.counter += 1;
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(v_serial));
			s_record_116 s_value_840 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_110_get_kind_take(s_record_110_copy(v_symbol))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_840;
			if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 7))
			{
				s_record_116 s_value_841 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_release(v_", sizeof("s_array_i64_release(v_") - 1), s_string_retain(v_name)), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_116_drop(v_next);
				v_next = s_value_841;
			}
			else
			{
				s_record_116 s_value_842 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_release(v_", sizeof("s_array_release(v_") - 1), s_string_retain(v_name)), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_116_drop(v_next);
				v_next = s_value_842;
			}
			s_record_116 s_return_843 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_843;
			s_string_release(v_temp);
		}
		if (s_is_dict(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
		{
			if ((s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
			{
				s_record_116 s_return_844 = s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("unsupported dictionary assignment", sizeof("unsupported dictionary assignment") - 1));
				s_record_116_drop(v_next);
				s_record_117_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_844;
			}
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_next))));
			v_next.counter += 1;
			s_record_116 s_value_845 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_110_get_kind_take(s_record_110_copy(v_symbol))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_845;
			s_record_116 s_value_846 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_dict_release(v_", sizeof("s_dict_release(v_") - 1), s_string_retain(v_name)), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_846;
			s_record_116 s_return_847 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_847;
			s_string_release(v_temp);
		}
		if (((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) >= 100) && (s_record_110_get_kind_take(s_record_110_copy(v_symbol)) < 10000)))
		{
			if ((s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
			{
				s_record_116 s_return_848 = s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("unsupported record assignment", sizeof("unsupported record assignment") - 1));
				s_record_116_drop(v_next);
				s_record_117_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_848;
			}
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_next))));
			v_next.counter += 1;
			s_record_116 s_value_849 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_110_get_kind_take(s_record_110_copy(v_symbol))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_849;
			s_record_116 s_value_850 = s_line(s_record_116_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name))));
			s_record_116_drop(v_next);
			v_next = s_value_850;
			s_record_116 s_return_851 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_851;
			s_string_release(v_temp);
		}
		if (s_is_optional(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
		{
			if ((s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
			{
				s_record_116 s_return_852 = s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("unsupported optional assignment", sizeof("unsupported optional assignment") - 1));
				s_record_116_drop(v_next);
				s_record_117_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_852;
			}
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_next))));
			v_next.counter += 1;
			s_record_116 s_value_853 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_110_get_kind_take(s_record_110_copy(v_symbol))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_853;
			if (s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
			{
				s_record_116 s_value_854 = s_line(s_record_116_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name))));
				s_record_116_drop(v_next);
				v_next = s_value_854;
			}
			s_record_116 s_return_855 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_855;
			s_string_release(v_temp);
		}
		if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 4))
		{
			const int64_t v_serial = s_record_116_get_counter_take(s_record_116_copy(v_next));
			v_next.counter += 1;
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(v_serial));
			S_String * v_operator = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
			if ((s_string_compare_take(s_string_retain(v_operator), s_string_from_bytes("+=", sizeof("+=") - 1)) == 0))
			{
				s_record_116 s_value_856 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("S_String *", sizeof("S_String *") - 1), s_string_retain(v_temp)), s_string_from_bytes(" = s_string_concat_take(s_string_retain(v_", sizeof(" = s_string_concat_take(s_string_retain(v_") - 1)), s_string_retain(v_name)), s_string_from_bytes("), ", sizeof("), ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_116_drop(v_next);
				v_next = s_value_856;
			}
			else if ((s_string_compare_take(s_string_retain(v_operator), s_string_from_bytes("=", sizeof("=") - 1)) == 0))
			{
				s_record_116 s_value_857 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("S_String *", sizeof("S_String *") - 1), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
				s_record_116_drop(v_next);
				v_next = s_value_857;
			}
			else
			{
				s_record_116 s_return_858 = s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("unsupported string assignment", sizeof("unsupported string assignment") - 1));
				s_string_release(v_operator);
				s_string_release(v_temp);
				s_record_116_drop(v_next);
				s_record_117_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_858;
			}
			s_record_116 s_value_859 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_release(v_", sizeof("s_string_release(v_") - 1), s_string_retain(v_name)), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_859;
			s_record_116 s_return_860 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_operator);
			s_string_release(v_temp);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_860;
			s_string_release(v_operator);
			s_string_release(v_temp);
		}
		s_record_116 s_return_861 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_116_drop(v_next);
		s_record_117_drop(v_result);
		s_record_110_drop(v_symbol);
		s_string_release(v_name);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_861;
		s_record_116_drop(v_next);
		s_record_117_drop(v_result);
		s_record_110_drop(v_symbol);
		s_string_release(v_name);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 35))
	{
		if ((s_record_101_get_count_take(s_record_101_copy(v_item)) == 0))
		{
			if ((s_record_116_get_return_kind_take(s_record_116_copy(v_state)) != 5))
			{
				s_record_116 s_return_862 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("non-void function must return a value", sizeof("non-void function must return a value") - 1));
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_862;
			}
			s_record_116 s_return_863 = s_line(s_record_116_copy(v_state), s_string_from_bytes("return;", sizeof("return;") - 1));
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_863;
		}
		s_record_117 v_result = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_result));
		if ((s_record_116_get_return_kind_take(s_record_116_copy(v_next)) == 0))
		{
			v_next.return_kind = s_record_117_get_kind_take(s_record_117_copy(v_result));
		}
		else
		{
			s_record_116 s_value_864 = s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_result)), s_record_116_get_return_kind_take(s_record_116_copy(v_next)));
			s_record_116_drop(v_next);
			v_next = s_value_864;
		}
		bool v_needs_cleanup = false;
		{
			S_Array * v_s_iter_865 = s_record_116_get_symbols_take(s_record_116_copy(v_next));
			for (int64_t s_index_865 = 0; s_index_865 < (int64_t)s_array_length(v_s_iter_865); ++s_index_865)
			{
				s_record_110 v_symbol = s_ga_10110_get_borrow(v_s_iter_865, s_index_865);
				if (s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
				{
					v_needs_cleanup = true;
				}
				s_record_110_drop(v_symbol);
			}
			s_array_release(v_s_iter_865);
		}
		if (v_needs_cleanup)
		{
			const int64_t v_serial = s_record_116_get_counter_take(s_record_116_copy(v_next));
			v_next.counter += 1;
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_return_", sizeof("s_return_") - 1), s_string_from_i64(v_serial));
			s_record_116 s_value_866 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_result))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_866;
			for (int64_t v_i = 0, s_bound_867 = s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_next))); v_i < s_bound_867; ++v_i)
			{
				const int64_t v_index = ((s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_next))) - v_i) - 1);
				s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)), v_index);
				if ((s_is_optional(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) && s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))))
				{
					s_record_116 s_value_868 = s_line(s_record_116_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
					s_record_116_drop(v_next);
					v_next = s_value_868;
				}
				if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 4))
				{
					s_record_116 s_value_869 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_release(v_", sizeof("s_string_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
					s_record_116_drop(v_next);
					v_next = s_value_869;
				}
				if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 7))
				{
					s_record_116 s_value_870 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_release(v_", sizeof("s_array_i64_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
					s_record_116_drop(v_next);
					v_next = s_value_870;
				}
				if (s_generic_array(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
				{
					s_record_116 s_value_871 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_release(v_", sizeof("s_array_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
					s_record_116_drop(v_next);
					v_next = s_value_871;
				}
				if (s_is_dict(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
				{
					s_record_116 s_value_872 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_dict_release(v_", sizeof("s_dict_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
					s_record_116_drop(v_next);
					v_next = s_value_872;
				}
				if (((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) >= 100) && (s_record_110_get_kind_take(s_record_110_copy(v_symbol)) < 10000)))
				{
					s_record_116 s_value_873 = s_line(s_record_116_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
					s_record_116_drop(v_next);
					v_next = s_value_873;
				}
				s_record_110_drop(v_symbol);
			}
			s_record_116 s_return_874 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("return ", sizeof("return ") - 1), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_116_drop(v_next);
			s_record_117_drop(v_result);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_874;
			s_string_release(v_temp);
		}
		s_record_116 s_return_875 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("return ", sizeof("return ") - 1), s_record_117_get_text_take(s_record_117_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_116_drop(v_next);
		s_record_117_drop(v_result);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_875;
		s_record_116_drop(v_next);
		s_record_117_drop(v_result);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 34))
	{
		{
			S_Array * v_s_iter_876 = s_record_116_get_symbols_take(s_record_116_copy(v_state));
			for (int64_t s_index_876 = 0; s_index_876 < (int64_t)s_array_length(v_s_iter_876); ++s_index_876)
			{
				s_record_110 v_symbol = s_ga_10110_get_borrow(v_s_iter_876, s_index_876);
				if (s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
				{
					s_record_116 s_return_877 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("managed recur requires ownership lowering", sizeof("managed recur requires ownership lowering") - 1));
					s_record_110_drop(v_symbol);
					s_array_release(v_s_iter_876);
					s_record_101_drop(v_item);
					s_record_116_drop(v_state);
					return s_return_877;
				}
				s_record_110_drop(v_symbol);
			}
			s_array_release(v_s_iter_876);
		}
		if ((s_record_101_get_count_take(s_record_101_copy(v_item)) != 1))
		{
			s_record_116 s_return_878 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("recur expects a direct self-call", sizeof("recur expects a direct self-call") - 1));
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_878;
		}
		const int64_t v_call = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		s_record_101 v_entry = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_call);
		if (((s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 10) || (s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_call, 0))) != 29)))
		{
			s_record_116 s_return_879 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("recur expects a direct self-call", sizeof("recur expects a direct self-call") - 1));
			s_record_101_drop(v_entry);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_879;
		}
		S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_call, 0));
		if ((s_string_compare_take(s_string_retain(v_name), s_record_116_get_function_name_take(s_record_116_copy(v_state))) != 0))
		{
			s_record_116 s_return_880 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("recur must call the enclosing function", sizeof("recur must call the enclosing function") - 1));
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_880;
		}
		S_Array * v_params = s_record_116_get_active_params_take(s_record_116_copy(v_state));
		if (((s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1) != s_generic_array_length_take(s_array_retain(v_params))))
		{
			s_record_116 s_return_881 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("recur argument count mismatch", sizeof("recur argument count mismatch") - 1));
			s_array_release(v_params);
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_881;
		}
		s_record_116 v_next = s_record_116_copy(v_state);
		const int64_t v_serial = s_record_116_get_counter_take(s_record_116_copy(v_next));
		v_next.counter += 1;
		for (int64_t v_i = 0, s_bound_882 = s_generic_array_length_take(s_array_retain(v_params)); v_i < s_bound_882; ++v_i)
		{
			s_record_117 v_arg = s_emit_expr(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_call, (v_i + 1)));
			s_record_116 s_value_883 = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_arg)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_arg)), s_record_104_get_kind_take(s_ga_10104_get_checked_take(s_array_retain(v_params), v_i)));
			s_record_116_drop(v_next);
			v_next = s_value_883;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_884 = s_record_116_copy(v_next);
				s_record_117_drop(v_arg);
				s_record_116_drop(v_next);
				s_array_release(v_params);
				s_string_release(v_name);
				s_record_101_drop(v_entry);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_884;
			}
			S_String * s_left_885 = s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_arg))), s_string_from_bytes(" s_tail_", sizeof(" s_tail_") - 1));
			S_String * s_right_886 = s_string_from_i64(v_serial);
			S_String * s_left_887 = s_string_concat_take(s_string_concat_take(s_left_885, s_right_886), s_string_from_bytes("_", sizeof("_") - 1));
			S_String * s_right_888 = s_string_from_i64(v_i);
			s_record_116 s_value_889 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_left_887, s_right_888), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_arg))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_889;
			s_record_117_drop(v_arg);
		}
		for (int64_t v_i = 0, s_bound_890 = s_generic_array_length_take(s_array_retain(v_params)); v_i < s_bound_890; ++v_i)
		{
			S_String * s_left_891 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_params), v_i))), s_string_from_bytes(" = s_tail_", sizeof(" = s_tail_") - 1)), s_string_from_i64(v_serial)), s_string_from_bytes("_", sizeof("_") - 1));
			S_String * s_right_892 = s_string_from_i64(v_i);
			s_record_116 s_value_893 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_left_891, s_right_892), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_893;
		}
		s_record_116 s_return_894 = s_line(s_record_116_copy(v_next), s_string_from_bytes("goto s_recur;", sizeof("goto s_recur;") - 1));
		s_record_116_drop(v_next);
		s_array_release(v_params);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_894;
		s_record_116_drop(v_next);
		s_array_release(v_params);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 18))
	{
		s_record_117 v_result = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		if ((s_record_117_get_kind_take(s_record_117_copy(v_result)) != 5))
		{
			s_record_116 s_return_895 = s_emit_error(s_record_117_get_state_take(s_record_117_copy(v_result)), v_id, s_string_from_bytes("expression statement must be effectful", sizeof("expression statement must be effectful") - 1));
			s_record_117_drop(v_result);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_895;
		}
		s_record_116 s_return_896 = s_line(s_record_117_get_state_take(s_record_117_copy(v_result)), s_string_concat_take(s_record_117_get_text_take(s_record_117_copy(v_result)), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_117_drop(v_result);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_896;
		s_record_117_drop(v_result);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 22))
	{
		s_record_116 v_next = s_record_116_copy(v_state);
		for (int64_t v_i = 0, s_bound_897 = s_record_101_get_count_take(s_record_101_copy(v_item)); v_i < s_bound_897; ++v_i)
		{
			const int64_t v_arm = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i);
			s_record_101 v_branch = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_arm);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_branch)) == 16))
			{
				s_record_116 s_value_898 = s_line(s_record_116_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1));
				s_record_116_drop(v_next);
				v_next = s_value_898;
				s_record_116 s_value_899 = s_emit_nested_block(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_arm, 0));
				s_record_116_drop(v_next);
				v_next = s_value_899;
			}
			else
			{
				if (((v_i > 0) && s_uses_optional_fallback(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_arm, 0))))
				{
					s_record_116 s_return_900 = s_emit_error(s_record_116_copy(v_next), v_arm, s_string_from_bytes("optional fallback in elif requires CFG lowering", sizeof("optional fallback in elif requires CFG lowering") - 1));
					s_record_101_drop(v_branch);
					s_record_116_drop(v_next);
					s_record_101_drop(v_item);
					s_record_116_drop(v_state);
					return s_return_900;
				}
				s_record_117 v_cond = s_emit_expr(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_arm, 0));
				s_record_116 s_value_901 = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_cond)), v_arm, s_record_117_get_kind_take(s_record_117_copy(v_cond)), 3);
				s_record_116_drop(v_next);
				v_next = s_value_901;
				S_String * v_header = s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (", sizeof("if (") - 1), s_record_117_get_text_take(s_record_117_copy(v_cond))), s_string_from_bytes(")", sizeof(")") - 1));
				if ((v_i > 0))
				{
					S_String *s_value_902 = s_string_concat_take(s_string_from_bytes("else ", sizeof("else ") - 1), s_string_retain(v_header));
					s_string_release(v_header);
					v_header = s_value_902;
				}
				s_record_116 s_value_903 = s_line(s_record_116_copy(v_next), s_string_retain(v_header));
				s_record_116_drop(v_next);
				v_next = s_value_903;
				s_record_116 s_value_904 = s_emit_nested_block(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_arm, 1));
				s_record_116_drop(v_next);
				v_next = s_value_904;
				s_string_release(v_header);
				s_record_117_drop(v_cond);
			}
			s_record_101_drop(v_branch);
		}
		s_record_116 s_return_905 = s_record_116_copy(v_next);
		s_record_116_drop(v_next);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_905;
		s_record_116_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 41))
	{
		s_record_116 v_next = s_line(s_record_116_copy(v_state), s_string_from_bytes("while (true) {", sizeof("while (true) {") - 1));
		v_next.indentation += 1;
		v_next.scope_depth += 1;
		v_next.loop_depth += 1;
		const int64_t v_previous_loop_scope = s_record_116_get_loop_scope_take(s_record_116_copy(v_next));
		v_next.loop_scope = s_record_116_get_scope_depth_take(s_record_116_copy(v_next));
		s_record_117 v_cond = s_emit_expr(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_id, 0));
		s_record_116 s_value_906 = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_cond)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_cond)), 3);
		s_record_116_drop(v_next);
		v_next = s_value_906;
		s_record_116 s_value_907 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (!(", sizeof("if (!(") - 1), s_record_117_get_text_take(s_record_117_copy(v_cond))), s_string_from_bytes(")) break;", sizeof(")) break;") - 1)));
		s_record_116_drop(v_next);
		v_next = s_value_907;
		s_record_116 s_value_908 = s_emit_block(s_record_116_copy(v_next), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_id, 1));
		s_record_116_drop(v_next);
		v_next = s_value_908;
		v_next.loop_depth -= 1;
		v_next.loop_scope = v_previous_loop_scope;
		v_next.scope_depth -= 1;
		v_next.indentation -= 1;
		s_record_116 s_return_909 = s_line(s_record_116_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
		s_record_117_drop(v_cond);
		s_record_116_drop(v_next);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_909;
		s_record_117_drop(v_cond);
		s_record_116_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 21))
	{
		const int64_t v_iterable = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_item)) - 2));
		const int64_t v_body = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_item)) - 1));
		if (((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_iterable)) != 5) || (s_string_compare_take(s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_iterable), s_string_from_bytes("..", sizeof("..") - 1)) != 0)))
		{
			s_record_117 v_source = s_emit_expr(s_record_116_copy(v_state), v_iterable);
			s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_source));
			const int64_t v_element_kind = s_array_element(s_record_117_get_kind_take(s_record_117_copy(v_source)));
			if ((v_element_kind == 0))
			{
				s_record_116 s_return_910 = s_emit_error(s_record_116_copy(v_next), v_id, s_string_from_bytes("for expects an array or range", sizeof("for expects an array or range") - 1));
				s_record_116_drop(v_next);
				s_record_117_drop(v_source);
				s_record_101_drop(v_item);
				s_record_116_drop(v_state);
				return s_return_910;
			}
			const int64_t v_original = s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)));
			const int64_t v_serial = s_record_116_get_counter_take(s_record_116_copy(v_next));
			v_next.counter += 1;
			S_String * v_iter = s_string_concat_take(s_string_from_bytes("s_iter_", sizeof("s_iter_") - 1), s_string_from_i64(v_serial));
			S_String * v_index = s_string_concat_take(s_string_from_bytes("s_index_", sizeof("s_index_") - 1), s_string_from_i64(v_serial));
			S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_id);
			s_record_116 s_value_911 = s_line(s_record_116_copy(v_next), s_string_from_bytes("{", sizeof("{") - 1));
			s_record_116_drop(v_next);
			v_next = s_value_911;
			v_next.indentation += 1;
			v_next.scope_depth += 1;
			s_record_116 s_value_912 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_117_get_kind_take(s_record_117_copy(v_source))), s_string_from_bytes(" v_", sizeof(" v_") - 1)), s_string_retain(v_iter)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_source))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_912;
			s_record_116 s_value_913 = s_append_symbol(s_record_116_copy(v_next), v_id, s_string_retain(v_iter), s_record_117_get_kind_take(s_record_117_copy(v_source)), false);
			s_record_116_drop(v_next);
			v_next = s_value_913;
			S_String * v_length = s_string_concat_take(s_string_concat_take(s_string_from_bytes("(int64_t)v_", sizeof("(int64_t)v_") - 1), s_string_retain(v_iter)), s_string_from_bytes("->length", sizeof("->length") - 1));
			if (s_generic_array(s_record_117_get_kind_take(s_record_117_copy(v_source))))
			{
				S_String *s_value_914 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("(int64_t)s_array_length(v_", sizeof("(int64_t)s_array_length(v_") - 1), s_string_retain(v_iter)), s_string_from_bytes(")", sizeof(")") - 1));
				s_string_release(v_length);
				v_length = s_value_914;
			}
			s_record_116 s_value_915 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("for (int64_t ", sizeof("for (int64_t ") - 1), s_string_retain(v_index)), s_string_from_bytes(" = 0; ", sizeof(" = 0; ") - 1)), s_string_retain(v_index)), s_string_from_bytes(" < ", sizeof(" < ") - 1)), s_string_retain(v_length)), s_string_from_bytes("; ++", sizeof("; ++") - 1)), s_string_retain(v_index)), s_string_from_bytes(")", sizeof(")") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_915;
			s_record_116 s_value_916 = s_line(s_record_116_copy(v_next), s_string_from_bytes("{", sizeof("{") - 1));
			s_record_116_drop(v_next);
			v_next = s_value_916;
			v_next.indentation += 1;
			v_next.scope_depth += 1;
			v_next.loop_depth += 1;
			const int64_t v_previous_loop_scope = s_record_116_get_loop_scope_take(s_record_116_copy(v_next));
			v_next.loop_scope = s_record_116_get_scope_depth_take(s_record_116_copy(v_next));
			S_String * v_getter = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_get_checked(v_", sizeof("s_array_i64_get_checked(v_") - 1), s_string_retain(v_iter)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_index)), s_string_from_bytes(")", sizeof(")") - 1));
			if (s_generic_array(s_record_117_get_kind_take(s_record_117_copy(v_source))))
			{
				S_String *s_value_917 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(s_record_117_get_kind_take(s_record_117_copy(v_source))), s_string_from_bytes("_get_borrow(v_", sizeof("_get_borrow(v_") - 1)), s_string_retain(v_iter)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_index)), s_string_from_bytes(")", sizeof(")") - 1));
				s_string_release(v_getter);
				v_getter = s_value_917;
			}
			s_record_116 s_value_918 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(v_element_kind), s_string_from_bytes(" v_", sizeof(" v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_getter)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_918;
			s_record_116 s_value_919 = s_append_symbol(s_record_116_copy(v_next), v_id, s_string_retain(v_name), v_element_kind, false);
			s_record_116_drop(v_next);
			v_next = s_value_919;
			s_record_116 s_value_920 = s_emit_block(s_record_116_copy(v_next), v_body);
			s_record_116_drop(v_next);
			v_next = s_value_920;
			if (s_managed(v_element_kind))
			{
				s_record_116 s_value_921 = s_line(s_record_116_copy(v_next), s_record_drop(v_element_kind, s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name))));
				s_record_116_drop(v_next);
				v_next = s_value_921;
			}
			v_next.loop_depth -= 1;
			v_next.loop_scope = v_previous_loop_scope;
			v_next.scope_depth -= 1;
			v_next.indentation -= 1;
			s_record_116 s_value_922 = s_line(s_record_116_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_116_drop(v_next);
			v_next = s_value_922;
			if ((s_record_117_get_kind_take(s_record_117_copy(v_source)) == 7))
			{
				s_record_116 s_value_923 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_release(v_", sizeof("s_array_i64_release(v_") - 1), s_string_retain(v_iter)), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_116_drop(v_next);
				v_next = s_value_923;
			}
			else
			{
				s_record_116 s_value_924 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_release(v_", sizeof("s_array_release(v_") - 1), s_string_retain(v_iter)), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_116_drop(v_next);
				v_next = s_value_924;
			}
			v_next.scope_depth -= 1;
			v_next.indentation -= 1;
			s_record_116 s_value_925 = s_line(s_record_116_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_116_drop(v_next);
			v_next = s_value_925;
			S_Array * v_symbols = s_ga_10110_make_take(0, NULL);
			for (int64_t v_i = 0, s_bound_926 = v_original; v_i < s_bound_926; ++v_i)
			{
				s_record_110 s_item_927 = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)), v_i);
				v_symbols = s_ga_10110_append_take(v_symbols, s_item_927);
			}
			S_Array * s_field_928 = s_array_retain(v_symbols);
			s_array_release(v_next.symbols);
			v_next.symbols = s_field_928;
			s_record_116 s_return_929 = s_record_116_copy(v_next);
			s_array_release(v_symbols);
			s_string_release(v_getter);
			s_string_release(v_length);
			s_string_release(v_name);
			s_string_release(v_index);
			s_string_release(v_iter);
			s_record_116_drop(v_next);
			s_record_117_drop(v_source);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_929;
			s_array_release(v_symbols);
			s_string_release(v_getter);
			s_string_release(v_length);
			s_string_release(v_name);
			s_string_release(v_index);
			s_string_release(v_iter);
			s_record_116_drop(v_next);
			s_record_117_drop(v_source);
		}
		s_record_117 v_from = s_emit_expr(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_iterable, 0));
		s_record_117 v_to = s_emit_expr(s_record_117_get_state_take(s_record_117_copy(v_from)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_iterable, 1));
		s_record_116 v_next = s_check_type(s_record_117_get_state_take(s_record_117_copy(v_to)), v_id, s_record_117_get_kind_take(s_record_117_copy(v_from)), 1);
		s_record_116 s_value_930 = s_check_type(s_record_116_copy(v_next), v_id, s_record_117_get_kind_take(s_record_117_copy(v_to)), 1);
		s_record_116_drop(v_next);
		v_next = s_value_930;
		S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_id);
		S_String * v_bound = s_string_concat_take(s_string_from_bytes("s_bound_", sizeof("s_bound_") - 1), s_string_from_i64(s_record_116_get_counter_take(s_record_116_copy(v_next))));
		v_next.counter += 1;
		s_record_116 s_value_931 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("for (int64_t v_", sizeof("for (int64_t v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_from))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_bound)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_to))), s_string_from_bytes("; v_", sizeof("; v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(" < ", sizeof(" < ") - 1)), s_string_retain(v_bound)), s_string_from_bytes("; ++v_", sizeof("; ++v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(")", sizeof(")") - 1)));
		s_record_116_drop(v_next);
		v_next = s_value_931;
		v_next.scope_depth += 1;
		s_record_116 s_value_932 = s_append_symbol(s_record_116_copy(v_next), v_id, s_string_retain(v_name), 1, false);
		s_record_116_drop(v_next);
		v_next = s_value_932;
		v_next.loop_depth += 1;
		const int64_t v_previous_loop_scope = s_record_116_get_loop_scope_take(s_record_116_copy(v_next));
		v_next.loop_scope = (s_record_116_get_scope_depth_take(s_record_116_copy(v_next)) + 1);
		s_record_116 s_value_933 = s_emit_nested_block(s_record_116_copy(v_next), v_body);
		s_record_116_drop(v_next);
		v_next = s_value_933;
		v_next.loop_scope = v_previous_loop_scope;
		v_next.loop_depth -= 1;
		v_next.scope_depth -= 1;
		S_Array * v_restored = s_ga_10110_make_take(0, NULL);
		for (int64_t v_i = 0, s_bound_934 = s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_state))); v_i < s_bound_934; ++v_i)
		{
			s_record_110 s_item_935 = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)), v_i);
			v_restored = s_ga_10110_append_take(v_restored, s_item_935);
		}
		S_Array * s_field_936 = s_array_retain(v_restored);
		s_array_release(v_next.symbols);
		v_next.symbols = s_field_936;
		s_record_116 s_return_937 = s_record_116_copy(v_next);
		s_array_release(v_restored);
		s_string_release(v_bound);
		s_string_release(v_name);
		s_record_116_drop(v_next);
		s_record_117_drop(v_to);
		s_record_117_drop(v_from);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_937;
		s_array_release(v_restored);
		s_string_release(v_bound);
		s_string_release(v_name);
		s_record_116_drop(v_next);
		s_record_117_drop(v_to);
		s_record_117_drop(v_from);
	}
	if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 9) || (s_record_101_get_kind_take(s_record_101_copy(v_item)) == 13)))
	{
		if ((s_record_116_get_loop_depth_take(s_record_116_copy(v_state)) == 0))
		{
			s_record_116 s_return_938 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("loop control outside loop", sizeof("loop control outside loop") - 1));
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_938;
		}
		s_record_116 v_next = s_record_116_copy(v_state);
		for (int64_t v_i = 0, s_bound_939 = s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_next))); v_i < s_bound_939; ++v_i)
		{
			s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_next)), ((s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_next))) - 1) - v_i));
			if (((s_record_110_get_depth_take(s_record_110_copy(v_symbol)) >= s_record_116_get_loop_scope_take(s_record_116_copy(v_next))) && s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))))
			{
				s_record_116 s_value_940 = s_line(s_record_116_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
				s_record_116_drop(v_next);
				v_next = s_value_940;
			}
			s_record_110_drop(v_symbol);
		}
		if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 9))
		{
			s_record_116 s_return_941 = s_line(s_record_116_copy(v_next), s_string_from_bytes("break;", sizeof("break;") - 1));
			s_record_116_drop(v_next);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_941;
		}
		s_record_116 s_return_942 = s_line(s_record_116_copy(v_next), s_string_from_bytes("continue;", sizeof("continue;") - 1));
		s_record_116_drop(v_next);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_942;
		s_record_116_drop(v_next);
	}
	s_record_116 s_return_943 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unsupported statement '", sizeof("unsupported statement '") - 1), s_node_kind_name(s_record_101_get_kind_take(s_record_101_copy(v_item)))), s_string_from_bytes("'", sizeof("'") - 1)));
	s_record_101_drop(v_item);
	s_record_116_drop(v_state);
	return s_return_943;
	s_record_101_drop(v_item);
}

s_record_116 s_collect_function(s_record_116 v_state, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if ((((s_find_function(s_record_116_copy(v_state), s_string_retain(v_name)) >= 0) || (s_string_compare_take(s_find_constant(s_record_116_copy(v_state), s_string_retain(v_name)), s_string_from_bytes("", sizeof("") - 1)) != 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0)))
	{
		s_record_116 s_return_944 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("duplicate function '", sizeof("duplicate function '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_944;
	}
	S_Array * v_params = s_ga_10104_make_take(0, NULL);
	int64_t v_result = 0;
	for (int64_t v_i = 0, s_bound_945 = (s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1); v_i < s_bound_945; ++v_i)
	{
		const int64_t v_part = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i);
		s_record_101 v_info = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_part);
		if ((s_record_101_get_kind_take(s_record_101_copy(v_info)) == 31))
		{
			int64_t v_kind = 0;
			if ((s_record_101_get_count_take(s_record_101_copy(v_info)) > 0))
			{
				if (((s_record_101_get_count_take(s_record_101_copy(v_info)) != 1) || (s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_part, 0))) != 38)))
				{
					s_record_116 s_return_946 = s_emit_error(s_record_116_copy(v_state), v_part, s_string_from_bytes("parameter defaults are not supported yet", sizeof("parameter defaults are not supported yet") - 1));
					s_record_101_drop(v_info);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_946;
				}
				v_kind = s_resolve_type(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_part, 0));
				if (((v_kind == 0) || (v_kind == 5)))
				{
					s_record_116 s_return_947 = s_emit_error(s_record_116_copy(v_state), v_part, s_string_from_bytes("unsupported parameter type", sizeof("unsupported parameter type") - 1));
					s_record_101_drop(v_info);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_947;
				}
			}
			S_String * v_parameter_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_part);
			{
				S_Array * v_s_iter_948 = s_array_retain(v_params);
				for (int64_t s_index_948 = 0; s_index_948 < (int64_t)s_array_length(v_s_iter_948); ++s_index_948)
				{
					s_record_104 v_previous = s_ga_10104_get_borrow(v_s_iter_948, s_index_948);
					if ((s_string_compare_take(s_record_104_get_name_take(s_record_104_copy(v_previous)), s_string_retain(v_parameter_name)) == 0))
					{
						s_record_116 s_return_949 = s_emit_error(s_record_116_copy(v_state), v_part, s_string_concat_take(s_string_concat_take(s_string_from_bytes("duplicate parameter '", sizeof("duplicate parameter '") - 1), s_string_retain(v_parameter_name)), s_string_from_bytes("'", sizeof("'") - 1)));
						s_record_104_drop(v_previous);
						s_array_release(v_s_iter_948);
						s_string_release(v_parameter_name);
						s_record_101_drop(v_info);
						s_array_release(v_params);
						s_string_release(v_name);
						s_record_101_drop(v_entry);
						s_record_116_drop(v_state);
						return s_return_949;
					}
					s_record_104_drop(v_previous);
				}
				s_array_release(v_s_iter_948);
			}
			s_record_104 s_item_950 = s_ParameterInfo(s_string_retain(v_parameter_name), v_kind);
			v_params = s_ga_10104_append_take(v_params, s_item_950);
			s_string_release(v_parameter_name);
		}
		else if ((s_record_101_get_kind_take(s_record_101_copy(v_info)) == 38))
		{
			v_result = s_resolve_type(s_record_116_copy(v_state), v_part);
		}
		s_record_101_drop(v_info);
	}
	s_record_116 v_next = s_record_116_copy(v_state);
	bool v_constructor = (s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 42);
	const int64_t v_body_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1));
	s_record_101 v_body = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_body_id);
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 14))
	{
		for (int64_t v_i = 0, s_bound_951 = s_record_101_get_count_take(s_record_101_copy(v_body)); v_i < s_bound_951; ++v_i)
		{
			if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_body_id, v_i))) == 32))
			{
				s_record_116 s_return_952 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("def constructors are no longer supported; use rec", sizeof("def constructors are no longer supported; use rec") - 1));
				s_record_101_drop(v_body);
				s_record_116_drop(v_next);
				s_array_release(v_params);
				s_string_release(v_name);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_952;
			}
		}
	}
	if (v_constructor)
	{
		{
			S_Array * v_s_iter_953 = s_array_retain(v_params);
			for (int64_t s_index_953 = 0; s_index_953 < (int64_t)s_array_length(v_s_iter_953); ++s_index_953)
			{
				s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_953, s_index_953);
				if ((s_record_104_get_kind_take(s_record_104_copy(v_param)) == 0))
				{
					s_record_116 s_return_954 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("generic constructors are not supported yet", sizeof("generic constructors are not supported yet") - 1));
					s_record_104_drop(v_param);
					s_array_release(v_s_iter_953);
					s_record_101_drop(v_body);
					s_record_116_drop(v_next);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_954;
				}
				s_record_104_drop(v_param);
			}
			s_array_release(v_s_iter_953);
		}
		const int64_t v_body_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1));
		s_record_101 v_body = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_body_id);
		s_record_116 v_probe = s_record_116_copy(v_state);
		S_Array * s_field_955 = s_ga_10110_make_take(0, NULL);
		s_array_release(v_probe.symbols);
		v_probe.symbols = s_field_955;
		S_Array * v_fields = s_ga_10104_make_take(0, NULL);
		{
			S_Array * v_s_iter_956 = s_array_retain(v_params);
			for (int64_t s_index_956 = 0; s_index_956 < (int64_t)s_array_length(v_s_iter_956); ++s_index_956)
			{
				s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_956, s_index_956);
				s_record_116 s_value_957 = s_append_symbol(s_record_116_copy(v_probe), v_id, s_record_104_get_name_take(s_record_104_copy(v_param)), s_record_104_get_kind_take(s_record_104_copy(v_param)), false);
				s_record_116_drop(v_probe);
				v_probe = s_value_957;
				s_record_104_drop(v_param);
			}
			s_array_release(v_s_iter_956);
		}
		for (int64_t v_i = 0, s_bound_958 = s_record_101_get_count_take(s_record_101_copy(v_body)); v_i < s_bound_958; ++v_i)
		{
			const int64_t v_pub_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_body_id, v_i);
			s_record_101 v_pub = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_pub_id);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_pub)) != 32))
			{
				s_record_116 s_return_959 = s_emit_error(s_record_116_copy(v_state), v_pub_id, s_string_from_bytes("constructors currently require direct pub statements", sizeof("constructors currently require direct pub statements") - 1));
				s_record_101_drop(v_pub);
				s_array_release(v_fields);
				s_record_116_drop(v_probe);
				s_record_101_drop(v_body);
				s_record_101_drop(v_body);
				s_record_116_drop(v_next);
				s_array_release(v_params);
				s_string_release(v_name);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_959;
			}
			S_String * v_field_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_pub_id);
			int64_t v_kind = 0;
			if ((s_record_101_get_count_take(s_record_101_copy(v_pub)) == 0))
			{
				const int64_t v_symbol = s_find_symbol(s_record_116_copy(v_probe), s_string_retain(v_field_name));
				if ((v_symbol < 0))
				{
					s_record_116 s_return_960 = s_emit_error(s_record_116_copy(v_state), v_pub_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown published binding '", sizeof("unknown published binding '") - 1), s_string_retain(v_field_name)), s_string_from_bytes("'", sizeof("'") - 1)));
					s_string_release(v_field_name);
					s_record_101_drop(v_pub);
					s_array_release(v_fields);
					s_record_116_drop(v_probe);
					s_record_101_drop(v_body);
					s_record_101_drop(v_body);
					s_record_116_drop(v_next);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_960;
				}
				v_kind = s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_probe)), v_symbol));
			}
			else
			{
				const int64_t v_value_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_pub_id, 0);
				if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_value_id)) == 4))
				{
					S_String *s_value_961 = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_value_id);
					s_string_release(v_field_name);
					v_field_name = s_value_961;
				}
				s_record_117 v_value = s_emit_expr(s_record_116_copy(v_probe), v_value_id);
				s_record_116 s_value_962 = s_record_117_get_state_take(s_record_117_copy(v_value));
				s_record_116_drop(v_probe);
				v_probe = s_value_962;
				if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_probe)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_116 s_return_963 = s_record_116_copy(v_probe);
					s_record_117_drop(v_value);
					s_string_release(v_field_name);
					s_record_101_drop(v_pub);
					s_array_release(v_fields);
					s_record_116_drop(v_probe);
					s_record_101_drop(v_body);
					s_record_101_drop(v_body);
					s_record_116_drop(v_next);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_101_drop(v_entry);
					s_record_116_drop(v_state);
					return s_return_963;
				}
				v_kind = s_record_117_get_kind_take(s_record_117_copy(v_value));
				s_record_117_drop(v_value);
			}
			if (((v_kind == 0) || (v_kind == 5)))
			{
				s_record_116 s_return_964 = s_emit_error(s_record_116_copy(v_state), v_pub_id, s_string_from_bytes("constructor fields currently require unmanaged values", sizeof("constructor fields currently require unmanaged values") - 1));
				s_string_release(v_field_name);
				s_record_101_drop(v_pub);
				s_array_release(v_fields);
				s_record_116_drop(v_probe);
				s_record_101_drop(v_body);
				s_record_101_drop(v_body);
				s_record_116_drop(v_next);
				s_array_release(v_params);
				s_string_release(v_name);
				s_record_101_drop(v_entry);
				s_record_116_drop(v_state);
				return s_return_964;
			}
			{
				S_Array * v_s_iter_965 = s_array_retain(v_fields);
				for (int64_t s_index_965 = 0; s_index_965 < (int64_t)s_array_length(v_s_iter_965); ++s_index_965)
				{
					s_record_104 v_previous = s_ga_10104_get_borrow(v_s_iter_965, s_index_965);
					if ((s_string_compare_take(s_record_104_get_name_take(s_record_104_copy(v_previous)), s_string_retain(v_field_name)) == 0))
					{
						s_record_116 s_return_966 = s_emit_error(s_record_116_copy(v_state), v_pub_id, s_string_from_bytes("constructor field overrides are not supported yet", sizeof("constructor field overrides are not supported yet") - 1));
						s_record_104_drop(v_previous);
						s_array_release(v_s_iter_965);
						s_string_release(v_field_name);
						s_record_101_drop(v_pub);
						s_array_release(v_fields);
						s_record_116_drop(v_probe);
						s_record_101_drop(v_body);
						s_record_101_drop(v_body);
						s_record_116_drop(v_next);
						s_array_release(v_params);
						s_string_release(v_name);
						s_record_101_drop(v_entry);
						s_record_116_drop(v_state);
						return s_return_966;
					}
					s_record_104_drop(v_previous);
				}
				s_array_release(v_s_iter_965);
			}
			s_record_104 s_item_967 = s_RecordField(s_string_retain(v_field_name), v_kind);
			v_fields = s_ga_10104_append_take(v_fields, s_item_967);
			s_string_release(v_field_name);
			s_record_101_drop(v_pub);
		}
		if ((s_generic_array_length_take(s_array_retain(v_fields)) == 0))
		{
			s_record_116 s_return_968 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("constructor must publish at least one field", sizeof("constructor must publish at least one field") - 1));
			s_array_release(v_fields);
			s_record_116_drop(v_probe);
			s_record_101_drop(v_body);
			s_record_101_drop(v_body);
			s_record_116_drop(v_next);
			s_array_release(v_params);
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_968;
		}
		s_record_117 v_record = s_intern_record(s_record_116_copy(v_probe), s_array_retain(v_fields));
		s_record_116 s_value_969 = s_record_117_get_state_take(s_record_117_copy(v_record));
		s_record_116_drop(v_next);
		v_next = s_value_969;
		v_result = s_record_117_get_kind_take(s_record_117_copy(v_record));
		s_record_117_drop(v_record);
		s_array_release(v_fields);
		s_record_116_drop(v_probe);
		s_record_101_drop(v_body);
	}
	S_Array * s_field_970 = s_ga_10112_append_take(s_record_116_get_functions_take(s_record_116_copy(v_next)), s_FunctionInfo(s_string_retain(v_name), v_result, s_array_retain(v_params), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1)), v_constructor, s_string_concat_take(s_string_from_bytes("s_", sizeof("s_") - 1), s_string_retain(v_name))));
	s_array_release(v_next.functions);
	v_next.functions = s_field_970;
	s_record_116 s_return_971 = s_record_116_copy(v_next);
	s_record_101_drop(v_body);
	s_record_116_drop(v_next);
	s_array_release(v_params);
	s_string_release(v_name);
	s_record_101_drop(v_entry);
	s_record_116_drop(v_state);
	return s_return_971;
	s_record_101_drop(v_body);
	s_record_116_drop(v_next);
	s_array_release(v_params);
	s_string_release(v_name);
	s_record_101_drop(v_entry);
}

s_record_117 s_evaluate_constant(s_record_116 v_state, int64_t v_id)
{
	s_record_101 v_item = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	S_String * v_value = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 24))
	{
		s_record_117 s_return_972 = s_ExprCode(s_record_116_copy(v_state), s_string_retain(v_value), 1);
		s_string_release(v_value);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_972;
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 29))
	{
		S_String * v_found = s_find_constant(s_record_116_copy(v_state), s_string_retain(v_value));
		if ((s_string_compare_take(s_string_retain(v_found), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_117 s_return_973 = s_ExprCode(s_record_116_copy(v_state), s_string_retain(v_found), 1);
			s_string_release(v_found);
			s_string_release(v_value);
			s_record_101_drop(v_item);
			s_record_116_drop(v_state);
			return s_return_973;
		}
		s_string_release(v_found);
	}
	if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 39) && ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("-", sizeof("-") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("+", sizeof("+") - 1)) == 0))))
	{
		s_record_117 v_inner = s_evaluate_constant(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_117 s_return_974 = s_ExprCode(s_record_117_get_state_take(s_record_117_copy(v_inner)), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_string_retain(v_value)), s_record_117_get_text_take(s_record_117_copy(v_inner))), s_string_from_bytes(")", sizeof(")") - 1)), 1);
		s_record_117_drop(v_inner);
		s_string_release(v_value);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_974;
		s_record_117_drop(v_inner);
	}
	if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 5) && (((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("+", sizeof("+") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("-", sizeof("-") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("*", sizeof("*") - 1)) == 0))))
	{
		s_record_117 v_left = s_evaluate_constant(s_record_116_copy(v_state), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_117 v_right = s_evaluate_constant(s_record_117_get_state_take(s_record_117_copy(v_left)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
		s_record_117 s_return_975 = s_ExprCode(s_record_117_get_state_take(s_record_117_copy(v_right)), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_record_117_get_text_take(s_record_117_copy(v_left))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_value)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_117_get_text_take(s_record_117_copy(v_right))), s_string_from_bytes(")", sizeof(")") - 1)), 1);
		s_record_117_drop(v_right);
		s_record_117_drop(v_left);
		s_string_release(v_value);
		s_record_101_drop(v_item);
		s_record_116_drop(v_state);
		return s_return_975;
		s_record_117_drop(v_right);
		s_record_117_drop(v_left);
	}
	s_record_117 s_return_976 = s_ExprCode(s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("unsupported module constant initializer", sizeof("unsupported module constant initializer") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
	s_string_release(v_value);
	s_record_101_drop(v_item);
	s_record_116_drop(v_state);
	return s_return_976;
	s_string_release(v_value);
	s_record_101_drop(v_item);
}

s_record_116 s_collect_constant(s_record_116 v_state, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 26) || (s_record_101_get_count_take(s_record_101_copy(v_entry)) < 1)))
	{
		s_record_116 s_return_977 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("unsupported module statement", sizeof("unsupported module statement") - 1));
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_977;
	}
	if (((s_string_compare_take(s_find_constant(s_record_116_copy(v_state), s_string_retain(v_name)), s_string_from_bytes("", sizeof("") - 1)) != 0) || (s_find_function(s_record_116_copy(v_state), s_string_retain(v_name)) >= 0)))
	{
		s_record_116 s_return_978 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("duplicate module name", sizeof("duplicate module name") - 1));
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_978;
	}
	if (((s_record_101_get_count_take(s_record_101_copy(v_entry)) > 1) && (s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0))) == 38)))
	{
		if ((s_ast_type(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0)) != 1))
		{
			s_record_116 s_return_979 = s_emit_error(s_record_116_copy(v_state), v_id, s_string_from_bytes("module constants must have type i64", sizeof("module constants must have type i64") - 1));
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_116_drop(v_state);
			return s_return_979;
		}
	}
	const int64_t v_initializer = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1));
	s_record_117 v_result = s_evaluate_constant(s_record_116_copy(v_state), v_initializer);
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_117_get_state_take(s_record_117_copy(v_result))), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_116 s_return_980 = s_record_117_get_state_take(s_record_117_copy(v_result));
		s_record_117_drop(v_result);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_116_drop(v_state);
		return s_return_980;
	}
	s_record_116 v_next = s_record_117_get_state_take(s_record_117_copy(v_result));
	S_Array * s_field_981 = s_ga_10114_append_take(s_record_116_get_constants_take(s_record_116_copy(v_next)), s_ConstantInfo(s_string_retain(v_name), s_record_117_get_text_take(s_record_117_copy(v_result))));
	s_array_release(v_next.constants);
	v_next.constants = s_field_981;
	s_record_116 s_return_982 = s_record_116_copy(v_next);
	s_record_116_drop(v_next);
	s_record_117_drop(v_result);
	s_string_release(v_name);
	s_record_101_drop(v_entry);
	s_record_116_drop(v_state);
	return s_return_982;
	s_record_116_drop(v_next);
	s_record_117_drop(v_result);
	s_string_release(v_name);
	s_record_101_drop(v_entry);
}

s_record_116 s_emit_function(s_record_116 v_state, s_record_112 v_info)
{
	s_record_116 v_next = s_record_116_copy(v_state);
	S_Array * s_field_983 = s_ga_10110_make_take(0, NULL);
	s_array_release(v_next.symbols);
	v_next.symbols = s_field_983;
	v_next.return_kind = s_record_112_get_kind_take(s_record_112_copy(v_info));
	S_String * s_field_984 = s_record_112_get_name_take(s_record_112_copy(v_info));
	s_string_release(v_next.function_name);
	v_next.function_name = s_field_984;
	S_Array * s_field_985 = s_record_112_get_params_take(s_record_112_copy(v_info));
	s_array_release(v_next.active_params);
	v_next.active_params = s_field_985;
	S_Array * v_args = s_ga_8_make_take(0, NULL);
	{
		S_Array * v_s_iter_986 = s_record_112_get_params_take(s_record_112_copy(v_info));
		for (int64_t s_index_986 = 0; s_index_986 < (int64_t)s_array_length(v_s_iter_986); ++s_index_986)
		{
			s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_986, s_index_986);
			S_String * s_item_987 = s_string_concat_take(s_string_concat_take(s_c_type(s_record_104_get_kind_take(s_record_104_copy(v_param))), s_string_from_bytes(" v_", sizeof(" v_") - 1)), s_record_104_get_name_take(s_record_104_copy(v_param)));
			v_args = s_ga_8_append_take(v_args, s_item_987);
			s_record_116 s_value_988 = s_append_symbol(s_record_116_copy(v_next), s_record_112_get_body_take(s_record_112_copy(v_info)), s_record_104_get_name_take(s_record_104_copy(v_param)), s_record_104_get_kind_take(s_record_104_copy(v_param)), false);
			s_record_116_drop(v_next);
			v_next = s_value_988;
			s_record_104_drop(v_param);
		}
		s_array_release(v_s_iter_986);
	}
	const int64_t v_header = s_generic_array_length_take(s_record_116_get_lines_take(s_record_116_copy(v_next)));
	s_record_116 s_value_989 = s_line(s_record_116_copy(v_next), s_string_from_bytes("", sizeof("") - 1));
	s_record_116_drop(v_next);
	v_next = s_value_989;
	s_record_116 s_value_990 = s_line(s_record_116_copy(v_next), s_string_from_bytes("{", sizeof("{") - 1));
	s_record_116_drop(v_next);
	v_next = s_value_990;
	v_next.indentation += 1;
	if (s_contains_recur(s_record_116_get_tree_take(s_record_116_copy(v_next)), s_record_112_get_body_take(s_record_112_copy(v_info))))
	{
		s_record_116 s_value_991 = s_line(s_record_116_copy(v_next), s_string_from_bytes("s_recur: ;", sizeof("s_recur: ;") - 1));
		s_record_116_drop(v_next);
		v_next = s_value_991;
	}
	v_next.scope_depth += 1;
	if (s_record_112_get_constructor_take(s_record_112_copy(v_info)))
	{
		s_record_101 v_body = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_next)), s_record_112_get_body_take(s_record_112_copy(v_info)));
		S_String * v_output = s_string_from_bytes("s_constructor_result", sizeof("s_constructor_result") - 1);
		s_record_116 s_value_992 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_112_get_kind_take(s_record_112_copy(v_info))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_output)), s_string_from_bytes(" = {0};", sizeof(" = {0};") - 1)));
		s_record_116_drop(v_next);
		v_next = s_value_992;
		S_Array * v_transferred = s_ga_8_make_take(0, NULL);
		for (int64_t v_i = 0, s_bound_993 = s_record_101_get_count_take(s_record_101_copy(v_body)); v_i < s_bound_993; ++v_i)
		{
			const int64_t v_pub_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), s_record_112_get_body_take(s_record_112_copy(v_info)), v_i);
			s_record_101 v_pub = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_pub_id);
			S_String * v_name = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_pub_id);
			S_String * v_expression = s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name));
			if ((s_record_101_get_count_take(s_record_101_copy(v_pub)) == 1))
			{
				const int64_t v_value_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_pub_id, 0);
				if ((s_record_101_get_kind_take(s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_value_id)) == 4))
				{
					S_String *s_value_994 = s_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_next)), v_value_id);
					s_string_release(v_name);
					v_name = s_value_994;
				}
				s_record_117 v_value = s_emit_expr(s_record_116_copy(v_next), v_value_id);
				s_record_116 s_value_995 = s_record_117_get_state_take(s_record_117_copy(v_value));
				s_record_116_drop(v_next);
				v_next = s_value_995;
				S_String *s_value_996 = s_record_117_get_text_take(s_record_117_copy(v_value));
				s_string_release(v_expression);
				v_expression = s_value_996;
				s_record_117_drop(v_value);
			}
			else
			{
				S_String * s_item_997 = s_string_retain(v_name);
				v_transferred = s_ga_8_append_take(v_transferred, s_item_997);
			}
			s_record_116 s_value_998 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_expression)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_116_drop(v_next);
			v_next = s_value_998;
			s_string_release(v_expression);
			s_string_release(v_name);
			s_record_101_drop(v_pub);
		}
		{
			S_Array * v_s_iter_999 = s_record_112_get_params_take(s_record_112_copy(v_info));
			for (int64_t s_index_999 = 0; s_index_999 < (int64_t)s_array_length(v_s_iter_999); ++s_index_999)
			{
				s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_999, s_index_999);
				bool v_kept = false;
				{
					S_Array * v_s_iter_1000 = s_array_retain(v_transferred);
					for (int64_t s_index_1000 = 0; s_index_1000 < (int64_t)s_array_length(v_s_iter_1000); ++s_index_1000)
					{
						S_String * v_name = s_ga_8_get_borrow(v_s_iter_1000, s_index_1000);
						if ((s_string_compare_take(s_string_retain(v_name), s_record_104_get_name_take(s_record_104_copy(v_param))) == 0))
						{
							v_kept = true;
						}
						s_string_release(v_name);
					}
					s_array_release(v_s_iter_1000);
				}
				if (((!v_kept) && s_managed(s_record_104_get_kind_take(s_record_104_copy(v_param)))))
				{
					s_record_116 s_value_1001 = s_line(s_record_116_copy(v_next), s_record_drop(s_record_104_get_kind_take(s_record_104_copy(v_param)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_104_get_name_take(s_record_104_copy(v_param)))));
					s_record_116_drop(v_next);
					v_next = s_value_1001;
				}
				s_record_104_drop(v_param);
			}
			s_array_release(v_s_iter_999);
		}
		s_record_116 s_value_1002 = s_line(s_record_116_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("return ", sizeof("return ") - 1), s_string_retain(v_output)), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_116_drop(v_next);
		v_next = s_value_1002;
		s_array_release(v_transferred);
		s_string_release(v_output);
		s_record_101_drop(v_body);
	}
	else
	{
		s_record_116 s_value_1003 = s_emit_block(s_record_116_copy(v_next), s_record_112_get_body_take(s_record_112_copy(v_info)));
		s_record_116_drop(v_next);
		v_next = s_value_1003;
	}
	v_next.scope_depth -= 1;
	v_next.indentation -= 1;
	if ((s_record_116_get_return_kind_take(s_record_116_copy(v_next)) == 0))
	{
		s_record_116 s_return_1004 = s_emit_error(s_record_116_copy(v_next), s_record_112_get_body_take(s_record_112_copy(v_info)), s_string_from_bytes("cannot infer function return type", sizeof("cannot infer function return type") - 1));
		s_array_release(v_args);
		s_record_116_drop(v_next);
		s_record_112_drop(v_info);
		s_record_116_drop(v_state);
		return s_return_1004;
	}
	S_String * s_left_1005 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_116_get_return_kind_take(s_record_116_copy(v_next))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_112_get_symbol_take(s_record_112_copy(v_info))), s_string_from_bytes("(", sizeof("(") - 1));
	S_String * s_right_1006 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_args));
	s_ga_8_set_take(&v_next.lines, v_header, s_string_concat_take(s_string_concat_take(s_left_1005, s_right_1006), s_string_from_bytes(")", sizeof(")") - 1)));
	s_record_116 s_return_1007 = s_line(s_record_116_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
	s_array_release(v_args);
	s_record_116_drop(v_next);
	s_record_112_drop(v_info);
	s_record_116_drop(v_state);
	return s_return_1007;
	s_array_release(v_args);
	s_record_116_drop(v_next);
}

s_record_118 s_compile_c(S_String * v_source)
{
	s_record_103 v_parsed = s_parse_module(s_string_retain(v_source));
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_103_get_state_take(s_record_103_copy(v_parsed))), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_118 s_return_1008 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_record_102_get_error_take(s_record_103_get_state_take(s_record_103_copy(v_parsed))), s_record_102_get_error_line_take(s_record_103_get_state_take(s_record_103_copy(v_parsed))));
		s_record_103_drop(v_parsed);
		s_string_release(v_source);
		return s_return_1008;
	}
	s_record_116 v_state = s_EmitState(s_record_103_get_state_take(s_record_103_copy(v_parsed)));
	s_record_101 v_root = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_record_103_get_node_take(s_record_103_copy(v_parsed)));
	for (int64_t v_i = 0, s_bound_1009 = s_record_101_get_count_take(s_record_101_copy(v_root)); v_i < s_bound_1009; ++v_i)
	{
		const int64_t v_id = s_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_record_103_get_node_take(s_record_103_copy(v_parsed)), v_i);
		s_record_101 v_item = s_ast_node(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
		if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 14) || (s_record_101_get_kind_take(s_record_101_copy(v_item)) == 42)))
		{
			s_record_116 s_value_1010 = s_collect_function(s_record_116_copy(v_state), v_id);
			s_record_116_drop(v_state);
			v_state = s_value_1010;
		}
		else
		{
			s_record_116 s_value_1011 = s_collect_constant(s_record_116_copy(v_state), v_id);
			s_record_116_drop(v_state);
			v_state = s_value_1011;
		}
		if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_118 s_return_1012 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_record_116_get_error_take(s_record_116_copy(v_state)), s_record_116_get_error_line_take(s_record_116_copy(v_state)));
			s_record_101_drop(v_item);
			s_record_101_drop(v_root);
			s_record_116_drop(v_state);
			s_record_103_drop(v_parsed);
			s_string_release(v_source);
			return s_return_1012;
		}
		s_record_101_drop(v_item);
	}
	const int64_t v_main_index = s_find_function(s_record_116_copy(v_state), s_string_from_bytes("main", sizeof("main") - 1));
	if ((((v_main_index < 0) || (s_record_112_get_kind_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_main_index)) != 1)) || (s_generic_array_length_take(s_record_112_get_params_take(s_ga_10112_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_state)), v_main_index))) != 0)))
	{
		s_record_118 s_return_1013 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_string_from_bytes("expected def main() -> i64", sizeof("expected def main() -> i64") - 1), 1);
		s_record_101_drop(v_root);
		s_record_116_drop(v_state);
		s_record_103_drop(v_parsed);
		s_string_release(v_source);
		return s_return_1013;
	}
	s_record_116 s_value_1014 = s_line(s_record_116_copy(v_state), s_string_from_bytes("#include <stdint.h>", sizeof("#include <stdint.h>") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1014;
	s_record_116 s_value_1015 = s_line(s_record_116_copy(v_state), s_string_from_bytes("#include <stdbool.h>", sizeof("#include <stdbool.h>") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1015;
	s_record_116 s_value_1016 = s_line(s_record_116_copy(v_state), s_string_from_bytes("#include <stdio.h>", sizeof("#include <stdio.h>") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1016;
	s_record_116 s_value_1017 = s_line(s_record_116_copy(v_state), s_string_from_bytes("#include <string.h>", sizeof("#include <string.h>") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1017;
	s_record_116 s_value_1018 = s_line(s_record_116_copy(v_state), s_string_from_bytes("#include <math.h>", sizeof("#include <math.h>") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1018;
	s_record_116 s_value_1019 = s_line(s_record_116_copy(v_state), s_string_from_bytes("#include \"sirius_runtime.h\"", sizeof("#include \"sirius_runtime.h\"") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1019;
	s_record_116 s_value_1020 = s_line(s_record_116_copy(v_state), s_string_from_bytes("", sizeof("") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1020;
	{
		S_Array * v_s_iter_1021 = s_record_116_get_functions_take(s_record_116_copy(v_state));
		for (int64_t s_index_1021 = 0; s_index_1021 < (int64_t)s_array_length(v_s_iter_1021); ++s_index_1021)
		{
			s_record_112 v_info = s_ga_10112_get_borrow(v_s_iter_1021, s_index_1021);
			if ((!s_is_generic(s_record_112_copy(v_info))))
			{
				S_Array * v_args = s_ga_8_make_take(0, NULL);
				{
					S_Array * v_s_iter_1022 = s_record_112_get_params_take(s_record_112_copy(v_info));
					for (int64_t s_index_1022 = 0; s_index_1022 < (int64_t)s_array_length(v_s_iter_1022); ++s_index_1022)
					{
						s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_1022, s_index_1022);
						S_String * s_item_1023 = s_c_type(s_record_104_get_kind_take(s_record_104_copy(v_param)));
						v_args = s_ga_8_append_take(v_args, s_item_1023);
						s_record_104_drop(v_param);
					}
					s_array_release(v_s_iter_1022);
				}
				S_String * s_left_1024 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_112_get_kind_take(s_record_112_copy(v_info))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_112_get_symbol_take(s_record_112_copy(v_info))), s_string_from_bytes("(", sizeof("(") - 1));
				S_String * s_right_1025 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_args));
				s_record_116 s_value_1026 = s_line(s_record_116_copy(v_state), s_string_concat_take(s_string_concat_take(s_left_1024, s_right_1025), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_116_drop(v_state);
				v_state = s_value_1026;
				s_array_release(v_args);
			}
			s_record_112_drop(v_info);
		}
		s_array_release(v_s_iter_1021);
	}
	s_record_116 s_value_1027 = s_line(s_record_116_copy(v_state), s_string_from_bytes("", sizeof("") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1027;
	{
		S_Array * v_s_iter_1028 = s_record_116_get_functions_take(s_record_116_copy(v_state));
		for (int64_t s_index_1028 = 0; s_index_1028 < (int64_t)s_array_length(v_s_iter_1028); ++s_index_1028)
		{
			s_record_112 v_info = s_ga_10112_get_borrow(v_s_iter_1028, s_index_1028);
			if ((!s_is_generic(s_record_112_copy(v_info))))
			{
				s_record_116 s_value_1029 = s_emit_function(s_record_116_copy(v_state), s_record_112_copy(v_info));
				s_record_116_drop(v_state);
				v_state = s_value_1029;
				if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_118 s_return_1030 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_record_116_get_error_take(s_record_116_copy(v_state)), s_record_116_get_error_line_take(s_record_116_copy(v_state)));
					s_record_112_drop(v_info);
					s_array_release(v_s_iter_1028);
					s_record_101_drop(v_root);
					s_record_116_drop(v_state);
					s_record_103_drop(v_parsed);
					s_string_release(v_source);
					return s_return_1030;
				}
				s_record_116 s_value_1031 = s_line(s_record_116_copy(v_state), s_string_from_bytes("", sizeof("") - 1));
				s_record_116_drop(v_state);
				v_state = s_value_1031;
			}
			s_record_112_drop(v_info);
		}
		s_array_release(v_s_iter_1028);
	}
	s_record_116 s_value_1032 = s_line(s_record_116_copy(v_state), s_string_from_bytes("int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_main(); }", sizeof("int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_main(); }") - 1));
	s_record_116_drop(v_state);
	v_state = s_value_1032;
	S_String * v_declarations = s_string_from_bytes("", sizeof("") - 1);
	S_String * v_definitions = s_string_from_bytes("", sizeof("") - 1);
	{
		S_Array * v_s_iter_1033 = s_record_116_get_instances_take(s_record_116_copy(v_state));
		for (int64_t s_index_1033 = 0; s_index_1033 < (int64_t)s_array_length(v_s_iter_1033); ++s_index_1033)
		{
			s_record_113 v_instance = s_ga_10113_get_borrow(v_s_iter_1033, s_index_1033);
			S_Array * v_args = s_ga_8_make_take(0, NULL);
			{
				S_Array_i64 * v_s_iter_1034 = s_record_113_get_kinds_take(s_record_113_copy(v_instance));
				for (int64_t s_index_1034 = 0; s_index_1034 < (int64_t)v_s_iter_1034->length; ++s_index_1034)
				{
					int64_t v_kind = s_array_i64_get_checked(v_s_iter_1034, s_index_1034);
					S_String * s_item_1035 = s_c_type(v_kind);
					v_args = s_ga_8_append_take(v_args, s_item_1035);
				}
				s_array_i64_release(v_s_iter_1034);
			}
			S_String * s_left_1036 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_declarations), s_c_type(s_record_113_get_kind_take(s_record_113_copy(v_instance)))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_113_get_symbol_take(s_record_113_copy(v_instance))), s_string_from_bytes("(", sizeof("(") - 1));
			S_String * s_right_1037 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_args));
			S_String *s_value_1038 = s_string_concat_take(s_string_concat_take(s_left_1036, s_right_1037), s_string_from_bytes(");\n", sizeof(");\n") - 1));
			s_string_release(v_declarations);
			v_declarations = s_value_1038;
			S_String *s_value_1039 = s_string_concat_take(s_string_concat_take(s_string_retain(v_definitions), s_record_113_get_code_take(s_record_113_copy(v_instance))), s_string_from_bytes("\n", sizeof("\n") - 1));
			s_string_release(v_definitions);
			v_definitions = s_value_1039;
			s_array_release(v_args);
			s_record_113_drop(v_instance);
		}
		s_array_release(v_s_iter_1033);
	}
	S_String * v_output = s_string_from_bytes("", sizeof("") - 1);
	for (int64_t v_i = 0, s_bound_1040 = s_generic_array_length_take(s_record_116_get_lines_take(s_record_116_copy(v_state))); v_i < s_bound_1040; ++v_i)
	{
		S_String *s_value_1041 = s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_ga_8_get_checked_take(s_record_116_get_lines_take(s_record_116_copy(v_state)), v_i)), s_string_from_bytes("\n", sizeof("\n") - 1));
		s_string_release(v_output);
		v_output = s_value_1041;
		if ((v_i == 5))
		{
			S_String *s_value_1042 = s_string_concat_take(s_string_retain(v_output), s_record_declarations(s_record_116_copy(v_state)));
			s_string_release(v_output);
			v_output = s_value_1042;
			S_String *s_value_1043 = s_string_concat_take(s_string_retain(v_output), s_optional_declarations(s_record_116_copy(v_state)));
			s_string_release(v_output);
			v_output = s_value_1043;
			S_String *s_value_1044 = s_string_concat_take(s_string_retain(v_output), s_generic_array_declarations(s_record_116_copy(v_state)));
			s_string_release(v_output);
			v_output = s_value_1044;
			S_String *s_value_1045 = s_string_concat_take(s_string_retain(v_output), s_dict_declarations(s_record_116_copy(v_state)));
			s_string_release(v_output);
			v_output = s_value_1045;
			S_String *s_value_1046 = s_string_concat_take(s_string_retain(v_output), s_string_retain(v_declarations));
			s_string_release(v_output);
			v_output = s_value_1046;
		}
	}
	S_String *s_value_1047 = s_string_concat_take(s_string_retain(v_output), s_string_retain(v_definitions));
	s_string_release(v_output);
	v_output = s_value_1047;
	s_record_118 s_return_1048 = s_CompileResult(s_string_retain(v_output), s_string_from_bytes("", sizeof("") - 1), 0);
	s_string_release(v_output);
	s_string_release(v_definitions);
	s_string_release(v_declarations);
	s_record_101_drop(v_root);
	s_record_116_drop(v_state);
	s_record_103_drop(v_parsed);
	s_string_release(v_source);
	return s_return_1048;
	s_string_release(v_output);
	s_string_release(v_definitions);
	s_string_release(v_declarations);
	s_record_101_drop(v_root);
	s_record_116_drop(v_state);
	s_record_103_drop(v_parsed);
}

int64_t s_main()
{
	S_Array * v_args = s_arguments();
	if ((s_generic_array_length_take(s_array_retain(v_args)) != 3))
	{
		s_print_string_take(s_string_from_bytes("usage: emit_c input.sirius output.c", sizeof("usage: emit_c input.sirius output.c") - 1));
		int64_t s_return_1049 = 2;
		s_array_release(v_args);
		return s_return_1049;
	}
	S_Optional_string s_option_1050 = s_read_text_take(s_ga_8_get_checked_take(s_array_retain(v_args), 1));
	S_String * s_fallback_1050;
	if (s_option_1050.has_value) {
		s_fallback_1050 = s_option_1050.value;
	} else {
		s_fallback_1050 = s_string_from_bytes("", sizeof("") - 1);
	}
	S_String * v_source = s_fallback_1050;
	s_record_118 v_result = s_compile_c(s_string_retain(v_source));
	if ((s_string_compare_take(s_record_118_get_error_take(s_record_118_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_print_string_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("ERROR ", sizeof("ERROR ") - 1), s_string_from_i64(s_record_118_get_line_take(s_record_118_copy(v_result)))), s_string_from_bytes(": ", sizeof(": ") - 1)), s_record_118_get_error_take(s_record_118_copy(v_result))));
		int64_t s_return_1051 = 1;
		s_record_118_drop(v_result);
		s_string_release(v_source);
		s_array_release(v_args);
		return s_return_1051;
	}
	if ((!s_write_text_take(s_ga_8_get_checked_take(s_array_retain(v_args), 2), s_record_118_get_code_take(s_record_118_copy(v_result)))))
	{
		s_print_string_take(s_string_from_bytes("cannot write output", sizeof("cannot write output") - 1));
		int64_t s_return_1052 = 2;
		s_record_118_drop(v_result);
		s_string_release(v_source);
		s_array_release(v_args);
		return s_return_1052;
	}
	int64_t s_return_1053 = 0;
	s_record_118_drop(v_result);
	s_string_release(v_source);
	s_array_release(v_args);
	return s_return_1053;
	s_record_118_drop(v_result);
	s_string_release(v_source);
	s_array_release(v_args);
}

int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_main(); }
