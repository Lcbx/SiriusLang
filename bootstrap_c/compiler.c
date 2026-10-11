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
	S_String * error;
	S_Array * fields;
	int64_t index;
} s_record_112;
static inline s_record_112 s_record_112_copy(s_record_112 value) {
	value.error = s_string_retain(value.error);
	value.fields = s_array_retain(value.fields);
	return value;
}
static inline void s_record_112_drop(s_record_112 value) {
	s_string_release(value.error);
	s_array_release(value.fields);
	(void)value;
}
static inline S_String * s_record_112_get_error_take(s_record_112 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_112_drop(owned);
	return field;
}
static inline S_Array * s_record_112_get_fields_take(s_record_112 owned) {
	S_Array * field = s_array_retain(owned.fields);
	s_record_112_drop(owned);
	return field;
}
static inline int64_t s_record_112_get_index_take(s_record_112 owned) {
	int64_t field = owned.index;
	s_record_112_drop(owned);
	return field;
}
typedef struct {
	S_String * name;
	int64_t source;
	int64_t type_node;
} s_record_113;
static inline s_record_113 s_record_113_copy(s_record_113 value) {
	value.name = s_string_retain(value.name);
	return value;
}
static inline void s_record_113_drop(s_record_113 value) {
	s_string_release(value.name);
	(void)value;
}
static inline S_String * s_record_113_get_name_take(s_record_113 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_113_drop(owned);
	return field;
}
static inline int64_t s_record_113_get_source_take(s_record_113 owned) {
	int64_t field = owned.source;
	s_record_113_drop(owned);
	return field;
}
static inline int64_t s_record_113_get_type_node_take(s_record_113 owned) {
	int64_t field = owned.type_node;
	s_record_113_drop(owned);
	return field;
}
typedef struct {
	int64_t body;
	bool constructor;
	S_String * name;
	S_Array * parameters;
	int64_t result_type_node;
	int64_t source;
} s_record_114;
static inline s_record_114 s_record_114_copy(s_record_114 value) {
	value.name = s_string_retain(value.name);
	value.parameters = s_array_retain(value.parameters);
	return value;
}
static inline void s_record_114_drop(s_record_114 value) {
	s_string_release(value.name);
	s_array_release(value.parameters);
	(void)value;
}
static inline int64_t s_record_114_get_body_take(s_record_114 owned) {
	int64_t field = owned.body;
	s_record_114_drop(owned);
	return field;
}
static inline bool s_record_114_get_constructor_take(s_record_114 owned) {
	bool field = owned.constructor;
	s_record_114_drop(owned);
	return field;
}
static inline S_String * s_record_114_get_name_take(s_record_114 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_114_drop(owned);
	return field;
}
static inline S_Array * s_record_114_get_parameters_take(s_record_114 owned) {
	S_Array * field = s_array_retain(owned.parameters);
	s_record_114_drop(owned);
	return field;
}
static inline int64_t s_record_114_get_result_type_node_take(s_record_114 owned) {
	int64_t field = owned.result_type_node;
	s_record_114_drop(owned);
	return field;
}
static inline int64_t s_record_114_get_source_take(s_record_114 owned) {
	int64_t field = owned.source;
	s_record_114_drop(owned);
	return field;
}
typedef struct {
	S_String * error;
	int64_t error_line;
	S_Array * functions;
} s_record_115;
static inline s_record_115 s_record_115_copy(s_record_115 value) {
	value.error = s_string_retain(value.error);
	value.functions = s_array_retain(value.functions);
	return value;
}
static inline void s_record_115_drop(s_record_115 value) {
	s_string_release(value.error);
	s_array_release(value.functions);
	(void)value;
}
static inline S_String * s_record_115_get_error_take(s_record_115 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_115_drop(owned);
	return field;
}
static inline int64_t s_record_115_get_error_line_take(s_record_115 owned) {
	int64_t field = owned.error_line;
	s_record_115_drop(owned);
	return field;
}
static inline S_Array * s_record_115_get_functions_take(s_record_115 owned) {
	S_Array * field = s_array_retain(owned.functions);
	s_record_115_drop(owned);
	return field;
}
typedef struct {
	S_Array * active_functions;
	S_Array * constants;
	int64_t depth;
	S_String * error;
	int64_t error_line;
	S_Array * functions;
	int64_t inferred_return;
	int64_t last_type;
	S_Array * names;
	S_Array * nodes;
	S_Array * symbols;
	s_record_102 tree;
	s_record_106 types;
} s_record_116;
static inline s_record_116 s_record_116_copy(s_record_116 value) {
	value.active_functions = s_array_retain(value.active_functions);
	value.constants = s_array_retain(value.constants);
	value.error = s_string_retain(value.error);
	value.functions = s_array_retain(value.functions);
	value.names = s_array_retain(value.names);
	value.nodes = s_array_retain(value.nodes);
	value.symbols = s_array_retain(value.symbols);
	value.tree = s_record_102_copy(value.tree);
	value.types = s_record_106_copy(value.types);
	return value;
}
static inline void s_record_116_drop(s_record_116 value) {
	s_array_release(value.active_functions);
	s_array_release(value.constants);
	s_string_release(value.error);
	s_array_release(value.functions);
	s_array_release(value.names);
	s_array_release(value.nodes);
	s_array_release(value.symbols);
	s_record_102_drop(value.tree);
	s_record_106_drop(value.types);
	(void)value;
}
static inline S_Array * s_record_116_get_active_functions_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.active_functions);
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_constants_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.constants);
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_depth_take(s_record_116 owned) {
	int64_t field = owned.depth;
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
static inline S_Array * s_record_116_get_functions_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.functions);
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_inferred_return_take(s_record_116 owned) {
	int64_t field = owned.inferred_return;
	s_record_116_drop(owned);
	return field;
}
static inline int64_t s_record_116_get_last_type_take(s_record_116 owned) {
	int64_t field = owned.last_type;
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_names_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.names);
	s_record_116_drop(owned);
	return field;
}
static inline S_Array * s_record_116_get_nodes_take(s_record_116 owned) {
	S_Array * field = s_array_retain(owned.nodes);
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
	S_String * error;
	int64_t error_line;
	S_Array * nodes;
} s_record_117;
static inline s_record_117 s_record_117_copy(s_record_117 value) {
	value.error = s_string_retain(value.error);
	value.nodes = s_array_retain(value.nodes);
	return value;
}
static inline void s_record_117_drop(s_record_117 value) {
	s_string_release(value.error);
	s_array_release(value.nodes);
	(void)value;
}
static inline S_String * s_record_117_get_error_take(s_record_117 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_117_drop(owned);
	return field;
}
static inline int64_t s_record_117_get_error_line_take(s_record_117 owned) {
	int64_t field = owned.error_line;
	s_record_117_drop(owned);
	return field;
}
static inline S_Array * s_record_117_get_nodes_take(s_record_117 owned) {
	S_Array * field = s_array_retain(owned.nodes);
	s_record_117_drop(owned);
	return field;
}
typedef struct {
	int64_t body;
	bool constructor;
	int64_t kind;
	S_String * name;
	S_Array * params;
	S_String * symbol;
} s_record_118;
static inline s_record_118 s_record_118_copy(s_record_118 value) {
	value.name = s_string_retain(value.name);
	value.params = s_array_retain(value.params);
	value.symbol = s_string_retain(value.symbol);
	return value;
}
static inline void s_record_118_drop(s_record_118 value) {
	s_string_release(value.name);
	s_array_release(value.params);
	s_string_release(value.symbol);
	(void)value;
}
static inline int64_t s_record_118_get_body_take(s_record_118 owned) {
	int64_t field = owned.body;
	s_record_118_drop(owned);
	return field;
}
static inline bool s_record_118_get_constructor_take(s_record_118 owned) {
	bool field = owned.constructor;
	s_record_118_drop(owned);
	return field;
}
static inline int64_t s_record_118_get_kind_take(s_record_118 owned) {
	int64_t field = owned.kind;
	s_record_118_drop(owned);
	return field;
}
static inline S_String * s_record_118_get_name_take(s_record_118 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_118_drop(owned);
	return field;
}
static inline S_Array * s_record_118_get_params_take(s_record_118 owned) {
	S_Array * field = s_array_retain(owned.params);
	s_record_118_drop(owned);
	return field;
}
static inline S_String * s_record_118_get_symbol_take(s_record_118 owned) {
	S_String * field = s_string_retain(owned.symbol);
	s_record_118_drop(owned);
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
} s_record_119;
static inline s_record_119 s_record_119_copy(s_record_119 value) {
	value.code = s_string_retain(value.code);
	value.kinds = s_array_i64_retain(value.kinds);
	value.name = s_string_retain(value.name);
	value.symbol = s_string_retain(value.symbol);
	value.type_ids = s_array_i64_retain(value.type_ids);
	return value;
}
static inline void s_record_119_drop(s_record_119 value) {
	s_string_release(value.code);
	s_array_i64_release(value.kinds);
	s_string_release(value.name);
	s_string_release(value.symbol);
	s_array_i64_release(value.type_ids);
	(void)value;
}
static inline S_String * s_record_119_get_code_take(s_record_119 owned) {
	S_String * field = s_string_retain(owned.code);
	s_record_119_drop(owned);
	return field;
}
static inline int64_t s_record_119_get_kind_take(s_record_119 owned) {
	int64_t field = owned.kind;
	s_record_119_drop(owned);
	return field;
}
static inline S_Array_i64 * s_record_119_get_kinds_take(s_record_119 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.kinds);
	s_record_119_drop(owned);
	return field;
}
static inline S_String * s_record_119_get_name_take(s_record_119 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_119_drop(owned);
	return field;
}
static inline int64_t s_record_119_get_status_take(s_record_119 owned) {
	int64_t field = owned.status;
	s_record_119_drop(owned);
	return field;
}
static inline S_String * s_record_119_get_symbol_take(s_record_119 owned) {
	S_String * field = s_string_retain(owned.symbol);
	s_record_119_drop(owned);
	return field;
}
static inline S_Array_i64 * s_record_119_get_type_ids_take(s_record_119 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.type_ids);
	s_record_119_drop(owned);
	return field;
}
typedef struct {
	S_String * name;
	S_String * value;
} s_record_120;
static inline s_record_120 s_record_120_copy(s_record_120 value) {
	value.name = s_string_retain(value.name);
	value.value = s_string_retain(value.value);
	return value;
}
static inline void s_record_120_drop(s_record_120 value) {
	s_string_release(value.name);
	s_string_release(value.value);
	(void)value;
}
static inline S_String * s_record_120_get_name_take(s_record_120 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_120_drop(owned);
	return field;
}
static inline S_String * s_record_120_get_value_take(s_record_120 owned) {
	S_String * field = s_string_retain(owned.value);
	s_record_120_drop(owned);
	return field;
}
typedef struct {
	int64_t canonical;
	S_Array * fields;
} s_record_121;
static inline s_record_121 s_record_121_copy(s_record_121 value) {
	value.fields = s_array_retain(value.fields);
	return value;
}
static inline void s_record_121_drop(s_record_121 value) {
	s_array_release(value.fields);
	(void)value;
}
static inline int64_t s_record_121_get_canonical_take(s_record_121 owned) {
	int64_t field = owned.canonical;
	s_record_121_drop(owned);
	return field;
}
static inline S_Array * s_record_121_get_fields_take(s_record_121 owned) {
	S_Array * field = s_array_retain(owned.fields);
	s_record_121_drop(owned);
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
} s_record_122;
static inline s_record_122 s_record_122_copy(s_record_122 value) {
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
static inline void s_record_122_drop(s_record_122 value) {
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
static inline S_Array * s_record_122_get_active_params_take(s_record_122 owned) {
	S_Array * field = s_array_retain(owned.active_params);
	s_record_122_drop(owned);
	return field;
}
static inline S_Array * s_record_122_get_constants_take(s_record_122 owned) {
	S_Array * field = s_array_retain(owned.constants);
	s_record_122_drop(owned);
	return field;
}
static inline int64_t s_record_122_get_counter_take(s_record_122 owned) {
	int64_t field = owned.counter;
	s_record_122_drop(owned);
	return field;
}
static inline S_Array_i64 * s_record_122_get_dict_kinds_take(s_record_122 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.dict_kinds);
	s_record_122_drop(owned);
	return field;
}
static inline S_String * s_record_122_get_error_take(s_record_122 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_122_drop(owned);
	return field;
}
static inline int64_t s_record_122_get_error_line_take(s_record_122 owned) {
	int64_t field = owned.error_line;
	s_record_122_drop(owned);
	return field;
}
static inline S_String * s_record_122_get_function_name_take(s_record_122 owned) {
	S_String * field = s_string_retain(owned.function_name);
	s_record_122_drop(owned);
	return field;
}
static inline S_Array * s_record_122_get_functions_take(s_record_122 owned) {
	S_Array * field = s_array_retain(owned.functions);
	s_record_122_drop(owned);
	return field;
}
static inline S_Array * s_record_122_get_hir_take(s_record_122 owned) {
	S_Array * field = s_array_retain(owned.hir);
	s_record_122_drop(owned);
	return field;
}
static inline int64_t s_record_122_get_indentation_take(s_record_122 owned) {
	int64_t field = owned.indentation;
	s_record_122_drop(owned);
	return field;
}
static inline S_Array * s_record_122_get_instances_take(s_record_122 owned) {
	S_Array * field = s_array_retain(owned.instances);
	s_record_122_drop(owned);
	return field;
}
static inline S_Array * s_record_122_get_lines_take(s_record_122 owned) {
	S_Array * field = s_array_retain(owned.lines);
	s_record_122_drop(owned);
	return field;
}
static inline int64_t s_record_122_get_loop_depth_take(s_record_122 owned) {
	int64_t field = owned.loop_depth;
	s_record_122_drop(owned);
	return field;
}
static inline int64_t s_record_122_get_loop_scope_take(s_record_122 owned) {
	int64_t field = owned.loop_scope;
	s_record_122_drop(owned);
	return field;
}
static inline S_Array * s_record_122_get_records_take(s_record_122 owned) {
	S_Array * field = s_array_retain(owned.records);
	s_record_122_drop(owned);
	return field;
}
static inline int64_t s_record_122_get_return_kind_take(s_record_122 owned) {
	int64_t field = owned.return_kind;
	s_record_122_drop(owned);
	return field;
}
static inline int64_t s_record_122_get_scope_depth_take(s_record_122 owned) {
	int64_t field = owned.scope_depth;
	s_record_122_drop(owned);
	return field;
}
static inline S_Array * s_record_122_get_symbols_take(s_record_122 owned) {
	S_Array * field = s_array_retain(owned.symbols);
	s_record_122_drop(owned);
	return field;
}
static inline s_record_102 s_record_122_get_tree_take(s_record_122 owned) {
	s_record_102 field = s_record_102_copy(owned.tree);
	s_record_122_drop(owned);
	return field;
}
static inline s_record_106 s_record_122_get_types_take(s_record_122 owned) {
	s_record_106 field = s_record_106_copy(owned.types);
	s_record_122_drop(owned);
	return field;
}
typedef struct {
	int64_t kind;
	s_record_122 state;
	S_String * text;
} s_record_123;
static inline s_record_123 s_record_123_copy(s_record_123 value) {
	value.state = s_record_122_copy(value.state);
	value.text = s_string_retain(value.text);
	return value;
}
static inline void s_record_123_drop(s_record_123 value) {
	s_record_122_drop(value.state);
	s_string_release(value.text);
	(void)value;
}
static inline int64_t s_record_123_get_kind_take(s_record_123 owned) {
	int64_t field = owned.kind;
	s_record_123_drop(owned);
	return field;
}
static inline s_record_122 s_record_123_get_state_take(s_record_123 owned) {
	s_record_122 field = s_record_122_copy(owned.state);
	s_record_123_drop(owned);
	return field;
}
static inline S_String * s_record_123_get_text_take(s_record_123 owned) {
	S_String * field = s_string_retain(owned.text);
	s_record_123_drop(owned);
	return field;
}
typedef struct {
	S_String * code;
	S_String * error;
	int64_t line;
} s_record_124;
static inline s_record_124 s_record_124_copy(s_record_124 value) {
	value.code = s_string_retain(value.code);
	value.error = s_string_retain(value.error);
	return value;
}
static inline void s_record_124_drop(s_record_124 value) {
	s_string_release(value.code);
	s_string_release(value.error);
	(void)value;
}
static inline S_String * s_record_124_get_code_take(s_record_124 owned) {
	S_String * field = s_string_retain(owned.code);
	s_record_124_drop(owned);
	return field;
}
static inline S_String * s_record_124_get_error_take(s_record_124 owned) {
	S_String * field = s_string_retain(owned.error);
	s_record_124_drop(owned);
	return field;
}
static inline int64_t s_record_124_get_line_take(s_record_124 owned) {
	int64_t field = owned.line;
	s_record_124_drop(owned);
	return field;
}
typedef struct {
	S_Array_i64 * ids;
	s_record_106 table;
} s_record_125;
static inline s_record_125 s_record_125_copy(s_record_125 value) {
	value.ids = s_array_i64_retain(value.ids);
	value.table = s_record_106_copy(value.table);
	return value;
}
static inline void s_record_125_drop(s_record_125 value) {
	s_array_i64_release(value.ids);
	s_record_106_drop(value.table);
	(void)value;
}
static inline S_Array_i64 * s_record_125_get_ids_take(s_record_125 owned) {
	S_Array_i64 * field = s_array_i64_retain(owned.ids);
	s_record_125_drop(owned);
	return field;
}
static inline s_record_106 s_record_125_get_table_take(s_record_125 owned) {
	s_record_106 field = s_record_106_copy(owned.table);
	s_record_125_drop(owned);
	return field;
}
typedef struct {
	S_String * c_name;
	S_String * copy;
	S_String * drop;
	int64_t type_id;
} s_record_126;
static inline s_record_126 s_record_126_copy(s_record_126 value) {
	value.c_name = s_string_retain(value.c_name);
	value.copy = s_string_retain(value.copy);
	value.drop = s_string_retain(value.drop);
	return value;
}
static inline void s_record_126_drop(s_record_126 value) {
	s_string_release(value.c_name);
	s_string_release(value.copy);
	s_string_release(value.drop);
	(void)value;
}
static inline S_String * s_record_126_get_c_name_take(s_record_126 owned) {
	S_String * field = s_string_retain(owned.c_name);
	s_record_126_drop(owned);
	return field;
}
static inline S_String * s_record_126_get_copy_take(s_record_126 owned) {
	S_String * field = s_string_retain(owned.copy);
	s_record_126_drop(owned);
	return field;
}
static inline S_String * s_record_126_get_drop_take(s_record_126 owned) {
	S_String * field = s_string_retain(owned.drop);
	s_record_126_drop(owned);
	return field;
}
static inline int64_t s_record_126_get_type_id_take(s_record_126 owned) {
	int64_t field = owned.type_id;
	s_record_126_drop(owned);
	return field;
}
typedef struct {
	s_record_126 abi;
	S_String * name;
	int64_t type_id;
} s_record_127;
static inline s_record_127 s_record_127_copy(s_record_127 value) {
	value.abi = s_record_126_copy(value.abi);
	value.name = s_string_retain(value.name);
	return value;
}
static inline void s_record_127_drop(s_record_127 value) {
	s_record_126_drop(value.abi);
	s_string_release(value.name);
	(void)value;
}
static inline s_record_126 s_record_127_get_abi_take(s_record_127 owned) {
	s_record_126 field = s_record_126_copy(owned.abi);
	s_record_127_drop(owned);
	return field;
}
static inline S_String * s_record_127_get_name_take(s_record_127 owned) {
	S_String * field = s_string_retain(owned.name);
	s_record_127_drop(owned);
	return field;
}
static inline int64_t s_record_127_get_type_id_take(s_record_127 owned) {
	int64_t field = owned.type_id;
	s_record_127_drop(owned);
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
typedef struct { bool has_value; s_record_100 value; } s_optional_100;
static inline s_optional_100 s_optional_100_copy(s_optional_100 owned) {
	if (owned.has_value) owned.value = s_record_100_copy(owned.value);
	return owned;
}
static inline void s_optional_100_drop(s_optional_100 owned) {
	if (owned.has_value) s_record_100_drop(owned.value);
}
typedef struct { bool has_value; s_record_101 value; } s_optional_101;
static inline s_optional_101 s_optional_101_copy(s_optional_101 owned) {
	if (owned.has_value) owned.value = s_record_101_copy(owned.value);
	return owned;
}
static inline void s_optional_101_drop(s_optional_101 owned) {
	if (owned.has_value) s_record_101_drop(owned.value);
}
typedef struct { bool has_value; s_record_102 value; } s_optional_102;
static inline s_optional_102 s_optional_102_copy(s_optional_102 owned) {
	if (owned.has_value) owned.value = s_record_102_copy(owned.value);
	return owned;
}
static inline void s_optional_102_drop(s_optional_102 owned) {
	if (owned.has_value) s_record_102_drop(owned.value);
}
typedef struct { bool has_value; s_record_103 value; } s_optional_103;
static inline s_optional_103 s_optional_103_copy(s_optional_103 owned) {
	if (owned.has_value) owned.value = s_record_103_copy(owned.value);
	return owned;
}
static inline void s_optional_103_drop(s_optional_103 owned) {
	if (owned.has_value) s_record_103_drop(owned.value);
}
typedef struct { bool has_value; s_record_104 value; } s_optional_104;
static inline s_optional_104 s_optional_104_copy(s_optional_104 owned) {
	if (owned.has_value) owned.value = s_record_104_copy(owned.value);
	return owned;
}
static inline void s_optional_104_drop(s_optional_104 owned) {
	if (owned.has_value) s_record_104_drop(owned.value);
}
typedef struct { bool has_value; s_record_105 value; } s_optional_105;
static inline s_optional_105 s_optional_105_copy(s_optional_105 owned) {
	if (owned.has_value) owned.value = s_record_105_copy(owned.value);
	return owned;
}
static inline void s_optional_105_drop(s_optional_105 owned) {
	if (owned.has_value) s_record_105_drop(owned.value);
}
typedef struct { bool has_value; s_record_106 value; } s_optional_106;
static inline s_optional_106 s_optional_106_copy(s_optional_106 owned) {
	if (owned.has_value) owned.value = s_record_106_copy(owned.value);
	return owned;
}
static inline void s_optional_106_drop(s_optional_106 owned) {
	if (owned.has_value) s_record_106_drop(owned.value);
}
typedef struct { bool has_value; s_record_107 value; } s_optional_107;
static inline s_optional_107 s_optional_107_copy(s_optional_107 owned) {
	if (owned.has_value) owned.value = s_record_107_copy(owned.value);
	return owned;
}
static inline void s_optional_107_drop(s_optional_107 owned) {
	if (owned.has_value) s_record_107_drop(owned.value);
}
typedef struct { bool has_value; s_record_108 value; } s_optional_108;
static inline s_optional_108 s_optional_108_copy(s_optional_108 owned) {
	if (owned.has_value) owned.value = s_record_108_copy(owned.value);
	return owned;
}
static inline void s_optional_108_drop(s_optional_108 owned) {
	if (owned.has_value) s_record_108_drop(owned.value);
}
typedef struct { bool has_value; s_record_109 value; } s_optional_109;
static inline s_optional_109 s_optional_109_copy(s_optional_109 owned) {
	if (owned.has_value) owned.value = s_record_109_copy(owned.value);
	return owned;
}
static inline void s_optional_109_drop(s_optional_109 owned) {
	if (owned.has_value) s_record_109_drop(owned.value);
}
typedef struct { bool has_value; s_record_110 value; } s_optional_110;
static inline s_optional_110 s_optional_110_copy(s_optional_110 owned) {
	if (owned.has_value) owned.value = s_record_110_copy(owned.value);
	return owned;
}
static inline void s_optional_110_drop(s_optional_110 owned) {
	if (owned.has_value) s_record_110_drop(owned.value);
}
typedef struct { bool has_value; s_record_111 value; } s_optional_111;
static inline s_optional_111 s_optional_111_copy(s_optional_111 owned) {
	if (owned.has_value) owned.value = s_record_111_copy(owned.value);
	return owned;
}
static inline void s_optional_111_drop(s_optional_111 owned) {
	if (owned.has_value) s_record_111_drop(owned.value);
}
typedef struct { bool has_value; s_record_112 value; } s_optional_112;
static inline s_optional_112 s_optional_112_copy(s_optional_112 owned) {
	if (owned.has_value) owned.value = s_record_112_copy(owned.value);
	return owned;
}
static inline void s_optional_112_drop(s_optional_112 owned) {
	if (owned.has_value) s_record_112_drop(owned.value);
}
typedef struct { bool has_value; s_record_113 value; } s_optional_113;
static inline s_optional_113 s_optional_113_copy(s_optional_113 owned) {
	if (owned.has_value) owned.value = s_record_113_copy(owned.value);
	return owned;
}
static inline void s_optional_113_drop(s_optional_113 owned) {
	if (owned.has_value) s_record_113_drop(owned.value);
}
typedef struct { bool has_value; s_record_114 value; } s_optional_114;
static inline s_optional_114 s_optional_114_copy(s_optional_114 owned) {
	if (owned.has_value) owned.value = s_record_114_copy(owned.value);
	return owned;
}
static inline void s_optional_114_drop(s_optional_114 owned) {
	if (owned.has_value) s_record_114_drop(owned.value);
}
typedef struct { bool has_value; s_record_115 value; } s_optional_115;
static inline s_optional_115 s_optional_115_copy(s_optional_115 owned) {
	if (owned.has_value) owned.value = s_record_115_copy(owned.value);
	return owned;
}
static inline void s_optional_115_drop(s_optional_115 owned) {
	if (owned.has_value) s_record_115_drop(owned.value);
}
typedef struct { bool has_value; s_record_116 value; } s_optional_116;
static inline s_optional_116 s_optional_116_copy(s_optional_116 owned) {
	if (owned.has_value) owned.value = s_record_116_copy(owned.value);
	return owned;
}
static inline void s_optional_116_drop(s_optional_116 owned) {
	if (owned.has_value) s_record_116_drop(owned.value);
}
typedef struct { bool has_value; s_record_117 value; } s_optional_117;
static inline s_optional_117 s_optional_117_copy(s_optional_117 owned) {
	if (owned.has_value) owned.value = s_record_117_copy(owned.value);
	return owned;
}
static inline void s_optional_117_drop(s_optional_117 owned) {
	if (owned.has_value) s_record_117_drop(owned.value);
}
typedef struct { bool has_value; s_record_118 value; } s_optional_118;
static inline s_optional_118 s_optional_118_copy(s_optional_118 owned) {
	if (owned.has_value) owned.value = s_record_118_copy(owned.value);
	return owned;
}
static inline void s_optional_118_drop(s_optional_118 owned) {
	if (owned.has_value) s_record_118_drop(owned.value);
}
typedef struct { bool has_value; s_record_119 value; } s_optional_119;
static inline s_optional_119 s_optional_119_copy(s_optional_119 owned) {
	if (owned.has_value) owned.value = s_record_119_copy(owned.value);
	return owned;
}
static inline void s_optional_119_drop(s_optional_119 owned) {
	if (owned.has_value) s_record_119_drop(owned.value);
}
typedef struct { bool has_value; s_record_120 value; } s_optional_120;
static inline s_optional_120 s_optional_120_copy(s_optional_120 owned) {
	if (owned.has_value) owned.value = s_record_120_copy(owned.value);
	return owned;
}
static inline void s_optional_120_drop(s_optional_120 owned) {
	if (owned.has_value) s_record_120_drop(owned.value);
}
typedef struct { bool has_value; s_record_121 value; } s_optional_121;
static inline s_optional_121 s_optional_121_copy(s_optional_121 owned) {
	if (owned.has_value) owned.value = s_record_121_copy(owned.value);
	return owned;
}
static inline void s_optional_121_drop(s_optional_121 owned) {
	if (owned.has_value) s_record_121_drop(owned.value);
}
typedef struct { bool has_value; s_record_122 value; } s_optional_122;
static inline s_optional_122 s_optional_122_copy(s_optional_122 owned) {
	if (owned.has_value) owned.value = s_record_122_copy(owned.value);
	return owned;
}
static inline void s_optional_122_drop(s_optional_122 owned) {
	if (owned.has_value) s_record_122_drop(owned.value);
}
typedef struct { bool has_value; s_record_123 value; } s_optional_123;
static inline s_optional_123 s_optional_123_copy(s_optional_123 owned) {
	if (owned.has_value) owned.value = s_record_123_copy(owned.value);
	return owned;
}
static inline void s_optional_123_drop(s_optional_123 owned) {
	if (owned.has_value) s_record_123_drop(owned.value);
}
typedef struct { bool has_value; s_record_124 value; } s_optional_124;
static inline s_optional_124 s_optional_124_copy(s_optional_124 owned) {
	if (owned.has_value) owned.value = s_record_124_copy(owned.value);
	return owned;
}
static inline void s_optional_124_drop(s_optional_124 owned) {
	if (owned.has_value) s_record_124_drop(owned.value);
}
typedef struct { bool has_value; s_record_125 value; } s_optional_125;
static inline s_optional_125 s_optional_125_copy(s_optional_125 owned) {
	if (owned.has_value) owned.value = s_record_125_copy(owned.value);
	return owned;
}
static inline void s_optional_125_drop(s_optional_125 owned) {
	if (owned.has_value) s_record_125_drop(owned.value);
}
typedef struct { bool has_value; s_record_126 value; } s_optional_126;
static inline s_optional_126 s_optional_126_copy(s_optional_126 owned) {
	if (owned.has_value) owned.value = s_record_126_copy(owned.value);
	return owned;
}
static inline void s_optional_126_drop(s_optional_126 owned) {
	if (owned.has_value) s_record_126_drop(owned.value);
}
typedef struct { bool has_value; s_record_127 value; } s_optional_127;
static inline s_optional_127 s_optional_127_copy(s_optional_127 owned) {
	if (owned.has_value) owned.value = s_record_127_copy(owned.value);
	return owned;
}
static inline void s_optional_127_drop(s_optional_127 owned) {
	if (owned.has_value) s_record_127_drop(owned.value);
}
typedef struct { bool has_value; S_Array_i64 * value; } s_optional_7;
static inline s_optional_7 s_optional_7_copy(s_optional_7 owned) {
	if (owned.has_value) owned.value = s_array_i64_retain(owned.value);
	return owned;
}
static inline void s_optional_7_drop(s_optional_7 owned) {
	if (owned.has_value) s_array_i64_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_8;
static inline s_optional_8 s_optional_8_copy(s_optional_8 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_8_drop(s_optional_8 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10100;
static inline s_optional_10100 s_optional_10100_copy(s_optional_10100 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10100_drop(s_optional_10100 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10101;
static inline s_optional_10101 s_optional_10101_copy(s_optional_10101 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10101_drop(s_optional_10101 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10102;
static inline s_optional_10102 s_optional_10102_copy(s_optional_10102 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10102_drop(s_optional_10102 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10103;
static inline s_optional_10103 s_optional_10103_copy(s_optional_10103 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10103_drop(s_optional_10103 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10104;
static inline s_optional_10104 s_optional_10104_copy(s_optional_10104 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10104_drop(s_optional_10104 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10105;
static inline s_optional_10105 s_optional_10105_copy(s_optional_10105 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10105_drop(s_optional_10105 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10106;
static inline s_optional_10106 s_optional_10106_copy(s_optional_10106 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10106_drop(s_optional_10106 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10107;
static inline s_optional_10107 s_optional_10107_copy(s_optional_10107 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10107_drop(s_optional_10107 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10108;
static inline s_optional_10108 s_optional_10108_copy(s_optional_10108 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10108_drop(s_optional_10108 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10109;
static inline s_optional_10109 s_optional_10109_copy(s_optional_10109 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10109_drop(s_optional_10109 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10110;
static inline s_optional_10110 s_optional_10110_copy(s_optional_10110 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10110_drop(s_optional_10110 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10111;
static inline s_optional_10111 s_optional_10111_copy(s_optional_10111 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10111_drop(s_optional_10111 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10112;
static inline s_optional_10112 s_optional_10112_copy(s_optional_10112 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10112_drop(s_optional_10112 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10113;
static inline s_optional_10113 s_optional_10113_copy(s_optional_10113 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10113_drop(s_optional_10113 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10114;
static inline s_optional_10114 s_optional_10114_copy(s_optional_10114 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10114_drop(s_optional_10114 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10115;
static inline s_optional_10115 s_optional_10115_copy(s_optional_10115 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10115_drop(s_optional_10115 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10116;
static inline s_optional_10116 s_optional_10116_copy(s_optional_10116 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10116_drop(s_optional_10116 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10117;
static inline s_optional_10117 s_optional_10117_copy(s_optional_10117 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10117_drop(s_optional_10117 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10118;
static inline s_optional_10118 s_optional_10118_copy(s_optional_10118 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10118_drop(s_optional_10118 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10119;
static inline s_optional_10119 s_optional_10119_copy(s_optional_10119 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10119_drop(s_optional_10119 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10120;
static inline s_optional_10120 s_optional_10120_copy(s_optional_10120 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10120_drop(s_optional_10120 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10121;
static inline s_optional_10121 s_optional_10121_copy(s_optional_10121 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10121_drop(s_optional_10121 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10122;
static inline s_optional_10122 s_optional_10122_copy(s_optional_10122 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10122_drop(s_optional_10122 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10123;
static inline s_optional_10123 s_optional_10123_copy(s_optional_10123 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10123_drop(s_optional_10123 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10124;
static inline s_optional_10124 s_optional_10124_copy(s_optional_10124 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10124_drop(s_optional_10124 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10125;
static inline s_optional_10125 s_optional_10125_copy(s_optional_10125 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10125_drop(s_optional_10125 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10126;
static inline s_optional_10126 s_optional_10126_copy(s_optional_10126 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10126_drop(s_optional_10126 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10127;
static inline s_optional_10127 s_optional_10127_copy(s_optional_10127 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10127_drop(s_optional_10127 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10007;
static inline s_optional_10007 s_optional_10007_copy(s_optional_10007 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10007_drop(s_optional_10007 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_10008;
static inline s_optional_10008 s_optional_10008_copy(s_optional_10008 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_10008_drop(s_optional_10008 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20100;
static inline s_optional_20100 s_optional_20100_copy(s_optional_20100 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20100_drop(s_optional_20100 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20101;
static inline s_optional_20101 s_optional_20101_copy(s_optional_20101 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20101_drop(s_optional_20101 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20102;
static inline s_optional_20102 s_optional_20102_copy(s_optional_20102 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20102_drop(s_optional_20102 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20103;
static inline s_optional_20103 s_optional_20103_copy(s_optional_20103 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20103_drop(s_optional_20103 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20104;
static inline s_optional_20104 s_optional_20104_copy(s_optional_20104 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20104_drop(s_optional_20104 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20105;
static inline s_optional_20105 s_optional_20105_copy(s_optional_20105 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20105_drop(s_optional_20105 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20106;
static inline s_optional_20106 s_optional_20106_copy(s_optional_20106 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20106_drop(s_optional_20106 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20107;
static inline s_optional_20107 s_optional_20107_copy(s_optional_20107 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20107_drop(s_optional_20107 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20108;
static inline s_optional_20108 s_optional_20108_copy(s_optional_20108 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20108_drop(s_optional_20108 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20109;
static inline s_optional_20109 s_optional_20109_copy(s_optional_20109 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20109_drop(s_optional_20109 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20110;
static inline s_optional_20110 s_optional_20110_copy(s_optional_20110 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20110_drop(s_optional_20110 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20111;
static inline s_optional_20111 s_optional_20111_copy(s_optional_20111 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20111_drop(s_optional_20111 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20112;
static inline s_optional_20112 s_optional_20112_copy(s_optional_20112 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20112_drop(s_optional_20112 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20113;
static inline s_optional_20113 s_optional_20113_copy(s_optional_20113 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20113_drop(s_optional_20113 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20114;
static inline s_optional_20114 s_optional_20114_copy(s_optional_20114 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20114_drop(s_optional_20114 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20115;
static inline s_optional_20115 s_optional_20115_copy(s_optional_20115 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20115_drop(s_optional_20115 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20116;
static inline s_optional_20116 s_optional_20116_copy(s_optional_20116 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20116_drop(s_optional_20116 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20117;
static inline s_optional_20117 s_optional_20117_copy(s_optional_20117 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20117_drop(s_optional_20117 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20118;
static inline s_optional_20118 s_optional_20118_copy(s_optional_20118 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20118_drop(s_optional_20118 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20119;
static inline s_optional_20119 s_optional_20119_copy(s_optional_20119 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20119_drop(s_optional_20119 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20120;
static inline s_optional_20120 s_optional_20120_copy(s_optional_20120 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20120_drop(s_optional_20120 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20121;
static inline s_optional_20121 s_optional_20121_copy(s_optional_20121 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20121_drop(s_optional_20121 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20122;
static inline s_optional_20122 s_optional_20122_copy(s_optional_20122 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20122_drop(s_optional_20122 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20123;
static inline s_optional_20123 s_optional_20123_copy(s_optional_20123 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20123_drop(s_optional_20123 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20124;
static inline s_optional_20124 s_optional_20124_copy(s_optional_20124 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20124_drop(s_optional_20124 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20125;
static inline s_optional_20125 s_optional_20125_copy(s_optional_20125 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20125_drop(s_optional_20125 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20126;
static inline s_optional_20126 s_optional_20126_copy(s_optional_20126 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20126_drop(s_optional_20126 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
typedef struct { bool has_value; S_Array * value; } s_optional_20127;
static inline s_optional_20127 s_optional_20127_copy(s_optional_20127 owned) {
	if (owned.has_value) owned.value = s_array_retain(owned.value);
	return owned;
}
static inline void s_optional_20127_drop(s_optional_20127 owned) {
	if (owned.has_value) s_array_release(owned.value);
}
static inline S_Optional_i64 s_array_i64_get_optional_take(S_Array_i64 *owned, int64_t index) {
	S_Optional_i64 result = s_array_i64_get(owned, index);
	s_array_i64_release(owned);
	return result;
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
static inline S_Optional_string s_ga_8_get_optional_take(S_Array *owned, int64_t index) {
	S_Optional_string result = {0};
	S_String * const *p = (S_String * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_string_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_100 s_ga_10100_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_100 result = {0};
	s_record_100 const *p = (s_record_100 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_100_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_101 s_ga_10101_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_101 result = {0};
	s_record_101 const *p = (s_record_101 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_101_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_102 s_ga_10102_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_102 result = {0};
	s_record_102 const *p = (s_record_102 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_102_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_103 s_ga_10103_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_103 result = {0};
	s_record_103 const *p = (s_record_103 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_103_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_104 s_ga_10104_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_104 result = {0};
	s_record_104 const *p = (s_record_104 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_104_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_105 s_ga_10105_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_105 result = {0};
	s_record_105 const *p = (s_record_105 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_105_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_106 s_ga_10106_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_106 result = {0};
	s_record_106 const *p = (s_record_106 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_106_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_107 s_ga_10107_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_107 result = {0};
	s_record_107 const *p = (s_record_107 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_107_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_108 s_ga_10108_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_108 result = {0};
	s_record_108 const *p = (s_record_108 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_108_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_109 s_ga_10109_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_109 result = {0};
	s_record_109 const *p = (s_record_109 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_109_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_110 s_ga_10110_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_110 result = {0};
	s_record_110 const *p = (s_record_110 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_110_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_111 s_ga_10111_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_111 result = {0};
	s_record_111 const *p = (s_record_111 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_111_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_112 s_ga_10112_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_112 result = {0};
	s_record_112 const *p = (s_record_112 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_112_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_113 s_ga_10113_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_113 result = {0};
	s_record_113 const *p = (s_record_113 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_113_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_114 s_ga_10114_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_114 result = {0};
	s_record_114 const *p = (s_record_114 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_114_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_115 s_ga_10115_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_115 result = {0};
	s_record_115 const *p = (s_record_115 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_115_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_116 s_ga_10116_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_116 result = {0};
	s_record_116 const *p = (s_record_116 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_116_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_117 s_ga_10117_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_117 result = {0};
	s_record_117 const *p = (s_record_117 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_117_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_118 s_ga_10118_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_118 result = {0};
	s_record_118 const *p = (s_record_118 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_118_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_119 s_ga_10119_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_119 result = {0};
	s_record_119 const *p = (s_record_119 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_119_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_120 s_ga_10120_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_120 result = {0};
	s_record_120 const *p = (s_record_120 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_120_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_121 s_ga_10121_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_121 result = {0};
	s_record_121 const *p = (s_record_121 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_121_copy(*p); }
	s_array_release(owned);
	return result;
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
static inline void s_ga_10122_copy(void *dst, const void *src) { *(s_record_122 *)dst = s_record_122_copy(*(s_record_122 const *)src); }
static inline void s_ga_10122_drop(void *value) { s_record_122_drop(*(s_record_122 *)value); }
static inline S_Array *s_ga_10122_make_take(size_t count, s_record_122 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_122), s_ga_10122_copy, s_ga_10122_drop);
	for (size_t i = 0; i < count; ++i) s_record_122_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10122_append_take(S_Array *owned, s_record_122 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_122_drop(item);
	return result;
}
static inline s_record_122 s_ga_10122_get_checked_take(S_Array *owned, int64_t index) {
	s_record_122 const *p = (s_record_122 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_122 value = s_record_122_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_122 s_ga_10122_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_122 result = {0};
	s_record_122 const *p = (s_record_122 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_122_copy(*p); }
	s_array_release(owned);
	return result;
}
static inline s_record_122 s_ga_10122_get_borrow(const S_Array *array, int64_t index) {
	s_record_122 const *p = (s_record_122 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_122_copy(*p);
}
static inline void s_ga_10122_set_take(S_Array **owned, int64_t index, s_record_122 item) {
	s_array_set(owned, index, &item);
	s_record_122_drop(item);
}
static inline void s_ga_10123_copy(void *dst, const void *src) { *(s_record_123 *)dst = s_record_123_copy(*(s_record_123 const *)src); }
static inline void s_ga_10123_drop(void *value) { s_record_123_drop(*(s_record_123 *)value); }
static inline S_Array *s_ga_10123_make_take(size_t count, s_record_123 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_123), s_ga_10123_copy, s_ga_10123_drop);
	for (size_t i = 0; i < count; ++i) s_record_123_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10123_append_take(S_Array *owned, s_record_123 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_123_drop(item);
	return result;
}
static inline s_record_123 s_ga_10123_get_checked_take(S_Array *owned, int64_t index) {
	s_record_123 const *p = (s_record_123 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_123 value = s_record_123_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_123 s_ga_10123_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_123 result = {0};
	s_record_123 const *p = (s_record_123 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_123_copy(*p); }
	s_array_release(owned);
	return result;
}
static inline s_record_123 s_ga_10123_get_borrow(const S_Array *array, int64_t index) {
	s_record_123 const *p = (s_record_123 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_123_copy(*p);
}
static inline void s_ga_10123_set_take(S_Array **owned, int64_t index, s_record_123 item) {
	s_array_set(owned, index, &item);
	s_record_123_drop(item);
}
static inline void s_ga_10124_copy(void *dst, const void *src) { *(s_record_124 *)dst = s_record_124_copy(*(s_record_124 const *)src); }
static inline void s_ga_10124_drop(void *value) { s_record_124_drop(*(s_record_124 *)value); }
static inline S_Array *s_ga_10124_make_take(size_t count, s_record_124 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_124), s_ga_10124_copy, s_ga_10124_drop);
	for (size_t i = 0; i < count; ++i) s_record_124_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10124_append_take(S_Array *owned, s_record_124 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_124_drop(item);
	return result;
}
static inline s_record_124 s_ga_10124_get_checked_take(S_Array *owned, int64_t index) {
	s_record_124 const *p = (s_record_124 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_124 value = s_record_124_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_124 s_ga_10124_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_124 result = {0};
	s_record_124 const *p = (s_record_124 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_124_copy(*p); }
	s_array_release(owned);
	return result;
}
static inline s_record_124 s_ga_10124_get_borrow(const S_Array *array, int64_t index) {
	s_record_124 const *p = (s_record_124 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_124_copy(*p);
}
static inline void s_ga_10124_set_take(S_Array **owned, int64_t index, s_record_124 item) {
	s_array_set(owned, index, &item);
	s_record_124_drop(item);
}
static inline void s_ga_10125_copy(void *dst, const void *src) { *(s_record_125 *)dst = s_record_125_copy(*(s_record_125 const *)src); }
static inline void s_ga_10125_drop(void *value) { s_record_125_drop(*(s_record_125 *)value); }
static inline S_Array *s_ga_10125_make_take(size_t count, s_record_125 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_125), s_ga_10125_copy, s_ga_10125_drop);
	for (size_t i = 0; i < count; ++i) s_record_125_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10125_append_take(S_Array *owned, s_record_125 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_125_drop(item);
	return result;
}
static inline s_record_125 s_ga_10125_get_checked_take(S_Array *owned, int64_t index) {
	s_record_125 const *p = (s_record_125 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_125 value = s_record_125_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_125 s_ga_10125_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_125 result = {0};
	s_record_125 const *p = (s_record_125 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_125_copy(*p); }
	s_array_release(owned);
	return result;
}
static inline s_record_125 s_ga_10125_get_borrow(const S_Array *array, int64_t index) {
	s_record_125 const *p = (s_record_125 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_125_copy(*p);
}
static inline void s_ga_10125_set_take(S_Array **owned, int64_t index, s_record_125 item) {
	s_array_set(owned, index, &item);
	s_record_125_drop(item);
}
static inline void s_ga_10126_copy(void *dst, const void *src) { *(s_record_126 *)dst = s_record_126_copy(*(s_record_126 const *)src); }
static inline void s_ga_10126_drop(void *value) { s_record_126_drop(*(s_record_126 *)value); }
static inline S_Array *s_ga_10126_make_take(size_t count, s_record_126 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_126), s_ga_10126_copy, s_ga_10126_drop);
	for (size_t i = 0; i < count; ++i) s_record_126_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10126_append_take(S_Array *owned, s_record_126 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_126_drop(item);
	return result;
}
static inline s_record_126 s_ga_10126_get_checked_take(S_Array *owned, int64_t index) {
	s_record_126 const *p = (s_record_126 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_126 value = s_record_126_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_126 s_ga_10126_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_126 result = {0};
	s_record_126 const *p = (s_record_126 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_126_copy(*p); }
	s_array_release(owned);
	return result;
}
static inline s_record_126 s_ga_10126_get_borrow(const S_Array *array, int64_t index) {
	s_record_126 const *p = (s_record_126 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_126_copy(*p);
}
static inline void s_ga_10126_set_take(S_Array **owned, int64_t index, s_record_126 item) {
	s_array_set(owned, index, &item);
	s_record_126_drop(item);
}
static inline void s_ga_10127_copy(void *dst, const void *src) { *(s_record_127 *)dst = s_record_127_copy(*(s_record_127 const *)src); }
static inline void s_ga_10127_drop(void *value) { s_record_127_drop(*(s_record_127 *)value); }
static inline S_Array *s_ga_10127_make_take(size_t count, s_record_127 *items) {
	S_Array *result = s_array_make(count, items, sizeof(s_record_127), s_ga_10127_copy, s_ga_10127_drop);
	for (size_t i = 0; i < count; ++i) s_record_127_drop(items[i]);
	return result;
}
static inline S_Array *s_ga_10127_append_take(S_Array *owned, s_record_127 item) {
	S_Array *result = s_array_append(owned, &item);
	s_record_127_drop(item);
	return result;
}
static inline s_record_127 s_ga_10127_get_checked_take(S_Array *owned, int64_t index) {
	s_record_127 const *p = (s_record_127 const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	s_record_127 value = s_record_127_copy(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_127 s_ga_10127_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_127 result = {0};
	s_record_127 const *p = (s_record_127 const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_record_127_copy(*p); }
	s_array_release(owned);
	return result;
}
static inline s_record_127 s_ga_10127_get_borrow(const S_Array *array, int64_t index) {
	s_record_127 const *p = (s_record_127 const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_record_127_copy(*p);
}
static inline void s_ga_10127_set_take(S_Array **owned, int64_t index, s_record_127 item) {
	s_array_set(owned, index, &item);
	s_record_127_drop(item);
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
static inline s_optional_7 s_ga_10007_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_7 result = {0};
	S_Array_i64 * const *p = (S_Array_i64 * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_i64_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_8 s_ga_10008_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_8 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10100 s_ga_20100_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10100 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10101 s_ga_20101_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10101 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10102 s_ga_20102_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10102 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10103 s_ga_20103_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10103 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10104 s_ga_20104_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10104 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10105 s_ga_20105_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10105 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10106 s_ga_20106_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10106 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10107 s_ga_20107_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10107 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10108 s_ga_20108_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10108 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10109 s_ga_20109_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10109 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10110 s_ga_20110_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10110 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10111 s_ga_20111_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10111 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10112 s_ga_20112_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10112 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10113 s_ga_20113_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10113 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10114 s_ga_20114_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10114 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10115 s_ga_20115_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10115 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10116 s_ga_20116_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10116 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10117 s_ga_20117_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10117 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10118 s_ga_20118_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10118 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10119 s_ga_20119_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10119 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10120 s_ga_20120_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10120 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10121 s_ga_20121_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10121 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline void s_ga_20122_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20122_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20122_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20122_copy, s_ga_20122_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20122_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20122_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_10122 s_ga_20122_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10122 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_20122_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20122_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20123_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20123_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20123_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20123_copy, s_ga_20123_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20123_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20123_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_10123 s_ga_20123_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10123 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_20123_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20123_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20124_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20124_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20124_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20124_copy, s_ga_20124_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20124_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20124_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_10124 s_ga_20124_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10124 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_20124_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20124_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20125_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20125_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20125_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20125_copy, s_ga_20125_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20125_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20125_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_10125 s_ga_20125_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10125 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_20125_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20125_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20126_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20126_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20126_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20126_copy, s_ga_20126_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20126_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20126_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_10126 s_ga_20126_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10126 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_20126_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20126_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_20127_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_20127_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_20127_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_20127_copy, s_ga_20127_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_20127_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_20127_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_10127 s_ga_20127_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10127 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_20127_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_20127_set_take(S_Array **owned, int64_t index, S_Array * item) {
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
static inline s_optional_10007 s_ga_20007_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10007 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_10008 s_ga_20008_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_10008 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20100 s_ga_30100_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20100 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20101 s_ga_30101_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20101 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20102 s_ga_30102_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20102 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20103 s_ga_30103_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20103 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20104 s_ga_30104_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20104 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20105 s_ga_30105_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20105 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20106 s_ga_30106_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20106 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20107 s_ga_30107_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20107 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20108 s_ga_30108_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20108 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20109 s_ga_30109_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20109 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20110 s_ga_30110_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20110 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20111 s_ga_30111_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20111 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20112 s_ga_30112_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20112 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20113 s_ga_30113_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20113 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20114 s_ga_30114_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20114 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20115 s_ga_30115_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20115 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20116 s_ga_30116_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20116 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20117 s_ga_30117_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20117 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20118 s_ga_30118_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20118 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20119 s_ga_30119_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20119 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20120 s_ga_30120_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20120 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline s_optional_20121 s_ga_30121_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20121 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
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
static inline void s_ga_30122_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30122_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30122_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30122_copy, s_ga_30122_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30122_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30122_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_20122 s_ga_30122_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20122 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_30122_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30122_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30123_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30123_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30123_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30123_copy, s_ga_30123_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30123_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30123_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_20123 s_ga_30123_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20123 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_30123_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30123_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30124_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30124_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30124_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30124_copy, s_ga_30124_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30124_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30124_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_20124 s_ga_30124_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20124 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_30124_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30124_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30125_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30125_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30125_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30125_copy, s_ga_30125_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30125_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30125_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_20125 s_ga_30125_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20125 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_30125_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30125_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30126_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30126_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30126_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30126_copy, s_ga_30126_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30126_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30126_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_20126 s_ga_30126_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20126 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_30126_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30126_set_take(S_Array **owned, int64_t index, S_Array * item) {
	s_array_set(owned, index, &item);
	s_array_release(item);
}
static inline void s_ga_30127_copy(void *dst, const void *src) { *(S_Array * *)dst = s_array_retain(*(S_Array * const *)src); }
static inline void s_ga_30127_drop(void *value) { s_array_release(*(S_Array * *)value); }
static inline S_Array *s_ga_30127_make_take(size_t count, S_Array * *items) {
	S_Array *result = s_array_make(count, items, sizeof(S_Array *), s_ga_30127_copy, s_ga_30127_drop);
	for (size_t i = 0; i < count; ++i) s_array_release(items[i]);
	return result;
}
static inline S_Array *s_ga_30127_append_take(S_Array *owned, S_Array * item) {
	S_Array *result = s_array_append(owned, &item);
	s_array_release(item);
	return result;
}
static inline S_Array * s_ga_30127_get_checked_take(S_Array *owned, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (!p) s_panic("index out of bounds");
	S_Array * value = s_array_retain(*p);
	s_array_release(owned);
	return value;
}
static inline s_optional_20127 s_ga_30127_get_optional_take(S_Array *owned, int64_t index) {
	s_optional_20127 result = {0};
	S_Array * const *p = (S_Array * const *)s_array_at(owned, index);
	if (p) { result.has_value = true; result.value = s_array_retain(*p); }
	s_array_release(owned);
	return result;
}
static inline S_Array * s_ga_30127_get_borrow(const S_Array *array, int64_t index) {
	S_Array * const *p = (S_Array * const *)s_array_at(array, index);
	if (!p) s_panic("index out of bounds");
	return s_array_retain(*p);
}
static inline void s_ga_30127_set_take(S_Array **owned, int64_t index, S_Array * item) {
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
s_record_112 s_HirRecordEdit(S_Array *, int64_t, S_String *);
int64_t s_hir_record_field(S_Array *, S_String *);
s_record_112 s_hir_record_put(S_Array *, S_String *, int64_t);
s_record_112 s_hir_record_delete(S_Array *, S_String *);
s_record_112 s_hir_record_spread(s_record_106, S_Array *, int64_t);
s_record_111 s_resolve_hir_record(s_record_106, S_Array *);
s_record_112 s_hir_constructor_publish(S_Array *, S_String *, int64_t);
s_record_113 s_HirParameterDecl(S_String *, int64_t, int64_t);
s_record_114 s_HirFunctionDecl(S_String *, int64_t, int64_t, bool, int64_t, S_Array *);
s_record_115 s_HirModuleDecl(S_Array *, S_String *, int64_t);
int64_t s_hir_ast_child(s_record_102, int64_t, int64_t);
S_String * s_hir_ast_text(s_record_102, int64_t);
int64_t s_hir_ast_line(s_record_102, int64_t);
s_record_115 s_hir_declaration_error(S_Array *, S_String *, int64_t);
s_record_115 s_hir_module_declarations(s_record_102, int64_t);
s_record_116 s_HirBodyState(s_record_102, S_Array *, S_Array *);
s_record_117 s_HirBodyAnalysis(S_Array *, S_String *, int64_t);
s_record_116 s_hir_body_error(s_record_116, int64_t, S_String *);
s_record_111 s_hir_body_type_reference(s_record_106, s_record_102, int64_t);
s_record_116 s_hir_body_binding(s_record_116, int64_t, S_String *, int64_t, bool);
s_record_116 s_hir_body_user_call(s_record_116, int64_t, S_String *, S_Array_i64 *);
s_record_116 s_hir_body_expression(s_record_116, int64_t);
s_record_116 s_hir_body_block(s_record_116, int64_t);
s_record_116 s_hir_body_statement(s_record_116, int64_t);
s_record_117 s_hir_analyze_function_body(s_record_102, s_record_114, S_Array *, S_Array *);
s_record_117 s_hir_analyze_module_body(s_record_102, s_record_114, S_Array *, S_Array *);
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
s_record_118 s_FunctionInfo(S_String *, int64_t, S_Array *, int64_t, bool, S_String *);
s_record_119 s_InstanceInfo(S_String *, S_Array_i64 *, S_Array_i64 *, int64_t, S_String *, S_String *, int64_t);
s_record_120 s_ConstantInfo(S_String *, S_String *);
s_record_121 s_RecordInfo(S_Array *, int64_t);
s_record_122 s_EmitState(s_record_102);
s_record_123 s_ExprCode(s_record_122, S_String *, int64_t);
s_record_124 s_CompileResult(S_String *, S_String *, int64_t);
s_record_101 s_ast_node(s_record_102, int64_t);
int64_t s_ast_child(s_record_102, int64_t, int64_t);
S_String * s_ast_text(s_record_102, int64_t);
int64_t s_ast_line(s_record_102, int64_t);
s_record_122 s_emit_error(s_record_122, int64_t, S_String *);
S_String * s_type_name(int64_t);
S_String * s_c_type(int64_t);
int64_t s_ast_type(s_record_102, int64_t);
int64_t s_resolve_type(s_record_122, int64_t);
int64_t s_array_constructor_kind(s_record_122, int64_t);
int64_t s_dict_constructor_kind(s_record_122, int64_t);
s_record_122 s_register_dict(s_record_122, int64_t);
s_record_122 s_line(s_record_122, S_String *);
int64_t s_find_symbol(s_record_122, S_String *);
int64_t s_find_function(s_record_122, S_String *);
bool s_is_generic(s_record_118);
int64_t s_find_instance(s_record_122, S_String *, S_Array_i64 *);
S_String * s_find_constant(s_record_122, S_String *);
int64_t s_record_field_index(s_record_122, int64_t, S_String *);
s_record_107 s_canonical_kind(s_record_106, S_Array *, int64_t);
s_record_125 s_InternedSignature(s_record_106, S_Array_i64 *);
s_record_125 s_canonical_signature(s_record_122, S_Array_i64 *);
s_record_123 s_intern_record(s_record_122, S_Array *);
s_record_126 s_CTypeABI(int64_t, S_String *, S_String *, S_String *);
s_record_126 s_c_abi(s_record_122, int64_t);
S_String * s_retain_function(int64_t);
S_String * s_release_function(int64_t);
s_record_127 s_CFieldABI(S_String *, int64_t, s_record_126);
S_Array * s_record_c_fields(s_record_122, int64_t);
S_String * s_record_copy(int64_t, S_String *);
S_String * s_record_drop(int64_t, S_String *);
bool s_plain_record(s_record_122, int64_t);
S_String * s_record_declarations(s_record_122);
S_String * s_optional_declarations(s_record_122);
S_Array_i64 * s_generic_array_element_types(s_record_122);
S_String * s_generic_array_declarations(s_record_122);
S_String * s_dict_declarations(s_record_122);
s_record_122 s_append_symbol(s_record_122, int64_t, S_String *, int64_t, bool);
s_record_122 s_check_type(s_record_122, int64_t, int64_t, int64_t);
bool s_has_effect(s_record_122, int64_t, S_Array *);
bool s_uses_optional_fallback(s_record_122, int64_t);
bool s_ambiguous_order(s_record_122, int64_t, int64_t);
s_record_123 s_specialize_call(s_record_122, int64_t, s_record_118, S_Array_i64 *, S_Array *);
s_record_122 s_check_hir_call(s_record_122, int64_t, s_record_111);
s_record_125 s_call_type_ids(s_record_122, S_Array_i64 *);
s_record_123 s_emit_expr(s_record_122, int64_t);
bool s_contains_recur(s_record_102, int64_t);
s_record_122 s_emit_block(s_record_122, int64_t);
s_record_122 s_emit_nested_block(s_record_122, int64_t);
s_record_122 s_emit_return_value(s_record_122, int64_t, int64_t);
s_record_122 s_emit_statement(s_record_122, int64_t);
s_record_122 s_collect_function(s_record_122, s_record_114);
s_record_123 s_evaluate_constant(s_record_122, int64_t);
s_record_122 s_collect_constant(s_record_122, int64_t);
s_record_122 s_emit_function(s_record_122, s_record_118);
s_record_124 s_compile_c(S_String *);
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
	if ((v_kind == 43))
	{
		return s_string_from_bytes("guard", sizeof("guard") - 1);
	}
	if ((v_kind == 44))
	{
		return s_string_from_bytes("unwrap", sizeof("unwrap") - 1);
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
	if (!((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
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
	if (!((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
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
			int64_t v_kind = 23;
			if (s_is_token(s_record_102_copy(v_next), s_string_from_bytes("?", sizeof("?") - 1)))
			{
				s_record_102 s_value_128 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("?", sizeof("?") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_128;
				v_kind = 30;
			}
			else if (s_is_token(s_record_102_copy(v_next), s_string_from_bytes("!", sizeof("!") - 1)))
			{
				s_record_102 s_value_129 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("!", sizeof("!") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_129;
				v_kind = 12;
			}
			S_Array_i64 * v_args = s_array_i64_make(0, NULL);
			int64_t s_item_130 = v_lhs;
			v_args = s_array_i64_append_take(v_args, s_item_130);
			s_record_103 v_index = s_parse_expr(s_record_102_copy(v_next), 0);
			s_record_102 s_value_131 = s_record_103_get_state_take(s_record_103_copy(v_index));
			s_record_102_drop(v_next);
			v_next = s_value_131;
			int64_t s_item_132 = s_record_103_get_node_take(s_record_103_copy(v_index));
			v_args = s_array_i64_append_take(v_args, s_item_132);
			while (true) {
				if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))) break;
				s_record_102 s_value_133 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_133;
				s_record_103 v_item = s_parse_expr(s_record_102_copy(v_next), 0);
				s_record_102 s_value_134 = s_record_103_get_state_take(s_record_103_copy(v_item));
				s_record_102_drop(v_next);
				v_next = s_value_134;
				int64_t s_item_135 = s_record_103_get_node_take(s_record_103_copy(v_item));
				v_args = s_array_i64_append_take(v_args, s_item_135);
				s_record_103_drop(v_item);
			}
			s_record_102 s_value_136 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("]", sizeof("]") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_136;
			s_record_103 v_node = s_append_node(s_record_102_copy(v_next), v_kind, v_position, s_array_i64_retain(v_args));
			s_record_102 s_value_137 = s_record_103_get_state_take(s_record_103_copy(v_node));
			s_record_102_drop(v_next);
			v_next = s_value_137;
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
		if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("!", sizeof("!") - 1)) == 0) && (v_min_bp <= 10)))
		{
			const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_138 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("!", sizeof("!") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_138;
			s_record_103 v_node = s_unary_node(s_record_102_copy(v_next), 44, v_position, v_lhs);
			s_record_102 s_value_139 = s_record_103_get_state_take(s_record_103_copy(v_node));
			s_record_102_drop(v_next);
			v_next = s_value_139;
			v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
			s_record_103_drop(v_node);
			s_string_release(v_op);
			continue;
			s_record_103_drop(v_node);
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
		s_record_102 s_value_140 = s_record_103_get_state_take(s_record_103_copy(v_rhs));
		s_record_102_drop(v_next);
		v_next = s_value_140;
		s_record_103 v_node = s_binary_node(s_record_102_copy(v_next), 5, v_position, v_lhs, s_record_103_get_node_take(s_record_103_copy(v_rhs)));
		s_record_102 s_value_141 = s_record_103_get_state_take(s_record_103_copy(v_node));
		s_record_102_drop(v_next);
		v_next = s_value_141;
		v_lhs = s_record_103_get_node_take(s_record_103_copy(v_node));
		s_record_103_drop(v_node);
		s_record_103_drop(v_rhs);
		s_string_release(v_op);
	}
	s_record_103 s_return_142 = s_ParseResult(s_record_102_copy(v_next), v_lhs);
	s_record_102_drop(v_next);
	s_string_release(v_text);
	s_record_100_drop(v_t);
	s_record_102_drop(v_p);
	return s_return_142;
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
		s_record_103 s_return_143 = s_ParseResult(s_record_102_copy(v_next), (-1));
		s_record_102_drop(v_next);
		s_record_102_drop(v_p);
		return s_return_143;
	}
	s_record_102 s_value_144 = s_expect_kind(s_record_102_copy(v_next), 7, s_string_from_bytes("indented block", sizeof("indented block") - 1));
	s_record_102_drop(v_next);
	v_next = s_value_144;
	S_Array_i64 * v_items = s_array_i64_make(0, NULL);
	while (true) {
		if (!((((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_kind_is(s_record_102_copy(v_next), 8))) && (!s_kind_is(s_record_102_copy(v_next), 9))))) break;
		s_record_103 v_item = s_parse_statement(s_record_102_copy(v_next));
		s_record_102 s_value_145 = s_record_103_get_state_take(s_record_103_copy(v_item));
		s_record_102_drop(v_next);
		v_next = s_value_145;
		if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_103_drop(v_item);
			break;
		}
		int64_t s_item_146 = s_record_103_get_node_take(s_record_103_copy(v_item));
		v_items = s_array_i64_append_take(v_items, s_item_146);
		s_record_103_drop(v_item);
	}
	if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_kind_is(s_record_102_copy(v_next), 9)))
	{
		s_record_102 s_value_147 = s_fail(s_record_102_copy(v_next), s_string_from_bytes("unterminated block", sizeof("unterminated block") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_147;
	}
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		s_record_102 s_value_148 = s_expect_kind(s_record_102_copy(v_next), 8, s_string_from_bytes("dedent", sizeof("dedent") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_148;
	}
	s_record_103 s_return_149 = s_append_node(s_record_102_copy(v_next), 7, v_token, s_array_i64_retain(v_items));
	s_array_i64_release(v_items);
	s_record_102_drop(v_next);
	s_record_102_drop(v_p);
	return s_return_149;
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
			s_record_103 s_value_150 = s_parse_block(s_record_102_copy(v_p));
			s_record_103_drop(v_body);
			v_body = s_value_150;
		}
		else
		{
			s_record_103 s_value_151 = s_append_node(s_record_102_copy(v_next), 7, s_record_102_get_index_take(s_record_102_copy(v_p)), s_array_i64_make(0, NULL));
			s_record_103_drop(v_body);
			v_body = s_value_151;
		}
		s_record_102_drop(v_next);
	}
	else
	{
		s_record_103 s_return_152 = s_ParseResult(s_fail(s_record_102_copy(v_p), s_string_from_bytes("expected newline after rec parameters", sizeof("expected newline after rec parameters") - 1)), (-1));
		s_record_103_drop(v_body);
		s_array_i64_release(v_params);
		s_record_102_drop(v_p);
		return s_return_152;
	}
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_103_get_state_take(s_record_103_copy(v_body))), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_103 s_return_153 = s_record_103_copy(v_body);
		s_record_103_drop(v_body);
		s_array_i64_release(v_params);
		s_record_102_drop(v_p);
		return s_return_153;
	}
	s_record_102 v_next = s_record_103_get_state_take(s_record_103_copy(v_body));
	S_Array_i64 * v_statements = s_array_i64_make(0, NULL);
	s_record_101 v_block = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_next)), s_record_103_get_node_take(s_record_103_copy(v_body)));
	{
		S_Array_i64 * v_s_iter_154 = s_array_i64_retain(v_params);
		for (int64_t s_index_154 = 0; s_index_154 < (int64_t)v_s_iter_154->length; ++s_index_154)
		{
			int64_t v_param = s_array_i64_get_checked(v_s_iter_154, s_index_154);
			s_record_101 v_entry = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_next)), v_param);
			S_String * v_name = s_token_text(s_record_102_get_source_take(s_record_102_copy(v_next)), s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_next)), s_record_101_get_token_take(s_record_101_copy(v_entry))));
			bool v_explicit = false;
			for (int64_t v_i = 0, s_bound_155 = s_record_101_get_count_take(s_record_101_copy(v_block)); v_i < s_bound_155; ++v_i)
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
				s_record_102 s_value_156 = s_record_103_get_state_take(s_record_103_copy(v_node));
				s_record_102_drop(v_next);
				v_next = s_value_156;
				int64_t s_item_157 = s_record_103_get_node_take(s_record_103_copy(v_node));
				v_statements = s_array_i64_append_take(v_statements, s_item_157);
				s_record_103_drop(v_node);
			}
			s_string_release(v_name);
			s_record_101_drop(v_entry);
		}
		s_array_i64_release(v_s_iter_154);
	}
	for (int64_t v_i = 0, s_bound_158 = s_record_101_get_count_take(s_record_101_copy(v_block)); v_i < s_bound_158; ++v_i)
	{
		int64_t s_item_159 = s_array_i64_get_checked_take(s_record_102_get_edges_take(s_record_102_copy(v_next)), (s_record_101_get_first_take(s_record_101_copy(v_block)) + v_i));
		v_statements = s_array_i64_append_take(v_statements, s_item_159);
	}
	s_record_103 s_return_160 = s_append_node(s_record_102_copy(v_next), 7, s_record_101_get_token_take(s_record_101_copy(v_block)), s_array_i64_retain(v_statements));
	s_record_101_drop(v_block);
	s_array_i64_release(v_statements);
	s_record_102_drop(v_next);
	s_record_103_drop(v_body);
	s_array_i64_release(v_params);
	s_record_102_drop(v_p);
	return s_return_160;
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
		s_record_102 s_value_161 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("function name", sizeof("function name") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_161;
		s_record_102 s_value_162 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("(", sizeof("(") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_162;
		S_Array_i64 * v_args = s_array_i64_make(0, NULL);
		while (true) {
			if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1)))))) break;
			s_record_103 v_arg = s_parse_param(s_record_102_copy(v_next));
			s_record_102 s_value_163 = s_record_103_get_state_take(s_record_103_copy(v_arg));
			s_record_102_drop(v_next);
			v_next = s_value_163;
			if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_103_drop(v_arg);
				break;
			}
			int64_t s_item_164 = s_record_103_get_node_take(s_record_103_copy(v_arg));
			v_args = s_array_i64_append_take(v_args, s_item_164);
			if ((!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))
			{
				s_record_103_drop(v_arg);
				break;
			}
			s_record_102 s_value_165 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_165;
			s_record_103_drop(v_arg);
		}
		s_record_102 s_value_166 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_166;
		if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("def", sizeof("def") - 1)) == 0) && (s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0)) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("->", sizeof("->") - 1))))
		{
			s_record_102 s_value_167 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("->", sizeof("->") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_167;
			s_record_103 v_typ = s_parse_type(s_record_102_copy(v_next));
			s_record_102 s_value_168 = s_record_103_get_state_take(s_record_103_copy(v_typ));
			s_record_102_drop(v_next);
			v_next = s_value_168;
			int64_t s_item_169 = s_record_103_get_node_take(s_record_103_copy(v_typ));
			v_args = s_array_i64_append_take(v_args, s_item_169);
			s_record_103_drop(v_typ);
		}
		s_record_103 v_body = s_ParseResult(s_record_102_copy(v_next), (-1));
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("rec", sizeof("rec") - 1)) == 0))
		{
			s_record_103 s_value_170 = s_rec_body(s_record_102_copy(v_next), s_array_i64_retain(v_args));
			s_record_103_drop(v_body);
			v_body = s_value_170;
		}
		else
		{
			s_record_103 s_value_171 = s_parse_block(s_record_102_copy(v_next));
			s_record_103_drop(v_body);
			v_body = s_value_171;
		}
		s_record_102 s_value_172 = s_record_103_get_state_take(s_record_103_copy(v_body));
		s_record_102_drop(v_next);
		v_next = s_value_172;
		int64_t s_item_173 = s_record_103_get_node_take(s_record_103_copy(v_body));
		v_args = s_array_i64_append_take(v_args, s_item_173);
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("rec", sizeof("rec") - 1)) == 0))
		{
			s_record_103 s_return_174 = s_append_node(s_record_102_copy(v_next), 42, v_name, s_array_i64_retain(v_args));
			s_record_103_drop(v_body);
			s_array_i64_release(v_args);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_174;
		}
		s_record_103 s_return_175 = s_append_node(s_record_102_copy(v_next), 14, v_name, s_array_i64_retain(v_args));
		s_record_103_drop(v_body);
		s_array_i64_release(v_args);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_175;
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
			s_record_102 s_value_176 = s_record_103_get_state_take(s_record_103_copy(v_arm));
			s_record_102_drop(v_next);
			v_next = s_value_176;
			int64_t s_item_177 = s_record_103_get_node_take(s_record_103_copy(v_arm));
			v_arms = s_array_i64_append_take(v_arms, s_item_177);
			s_record_103_drop(v_arm);
			s_record_103_drop(v_body);
			s_record_103_drop(v_cond);
		}
		if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1))))
		{
			const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_178 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_178;
			s_record_103 v_body = s_parse_block(s_record_102_copy(v_next));
			s_record_103 v_arm = s_unary_node(s_record_103_get_state_take(s_record_103_copy(v_body)), 16, v_position, s_record_103_get_node_take(s_record_103_copy(v_body)));
			s_record_102 s_value_179 = s_record_103_get_state_take(s_record_103_copy(v_arm));
			s_record_102_drop(v_next);
			v_next = s_value_179;
			int64_t s_item_180 = s_record_103_get_node_take(s_record_103_copy(v_arm));
			v_arms = s_array_i64_append_take(v_arms, s_item_180);
			s_record_103_drop(v_arm);
			s_record_103_drop(v_body);
		}
		s_record_103 s_return_181 = s_append_node(s_record_102_copy(v_next), 22, v_token, s_array_i64_retain(v_arms));
		s_array_i64_release(v_arms);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_181;
		s_array_i64_release(v_arms);
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("while", sizeof("while") - 1)) == 0))
	{
		s_record_102 s_value_182 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("while", sizeof("while") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_182;
		s_record_103 v_cond = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_103 v_body = s_parse_block(s_record_103_get_state_take(s_record_103_copy(v_cond)));
		s_record_103 s_return_183 = s_binary_node(s_record_103_get_state_take(s_record_103_copy(v_body)), 41, v_token, s_record_103_get_node_take(s_record_103_copy(v_cond)), s_record_103_get_node_take(s_record_103_copy(v_body)));
		s_record_103_drop(v_body);
		s_record_103_drop(v_cond);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_183;
		s_record_103_drop(v_body);
		s_record_103_drop(v_cond);
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("for", sizeof("for") - 1)) == 0))
	{
		s_record_102 s_value_184 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("for", sizeof("for") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_184;
		const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
		s_record_102 s_value_185 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("loop variable", sizeof("loop variable") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_185;
		S_Array_i64 * v_kids = s_array_i64_make(0, NULL);
		if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1))))
		{
			s_record_102 s_value_186 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_186;
			s_record_103 v_annotation = s_parse_type(s_record_102_copy(v_next));
			s_record_102 s_value_187 = s_record_103_get_state_take(s_record_103_copy(v_annotation));
			s_record_102_drop(v_next);
			v_next = s_value_187;
			int64_t s_item_188 = s_record_103_get_node_take(s_record_103_copy(v_annotation));
			v_kids = s_array_i64_append_take(v_kids, s_item_188);
			s_record_103_drop(v_annotation);
		}
		s_record_102 s_value_189 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("in", sizeof("in") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_189;
		s_record_103 v_iterable = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_103 v_body = s_parse_block(s_record_103_get_state_take(s_record_103_copy(v_iterable)));
		int64_t s_item_190 = s_record_103_get_node_take(s_record_103_copy(v_iterable));
		v_kids = s_array_i64_append_take(v_kids, s_item_190);
		int64_t s_item_191 = s_record_103_get_node_take(s_record_103_copy(v_body));
		v_kids = s_array_i64_append_take(v_kids, s_item_191);
		s_record_103 s_return_192 = s_append_node(s_record_103_get_state_take(s_record_103_copy(v_body)), 21, v_name, s_array_i64_retain(v_kids));
		s_record_103_drop(v_body);
		s_record_103_drop(v_iterable);
		s_array_i64_release(v_kids);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_192;
		s_record_103_drop(v_body);
		s_record_103_drop(v_iterable);
		s_array_i64_release(v_kids);
	}
	if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("match", sizeof("match") - 1)) == 0))
	{
		s_record_102 s_value_193 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("match", sizeof("match") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_193;
		s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_194 = s_expect_kind(s_record_103_get_state_take(s_record_103_copy(v_value)), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_194;
		s_record_102 s_value_195 = s_expect_kind(s_record_102_copy(v_next), 7, s_string_from_bytes("indented match arms", sizeof("indented match arms") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_195;
		S_Array_i64 * v_arms = s_array_i64_make(0, NULL);
		int64_t s_item_196 = s_record_103_get_node_take(s_record_103_copy(v_value));
		v_arms = s_array_i64_append_take(v_arms, s_item_196);
		while (true) {
			if (!((((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_kind_is(s_record_102_copy(v_next), 8))) && (!s_kind_is(s_record_102_copy(v_next), 9))))) break;
			const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_197 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("variant name", sizeof("variant name") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_197;
			s_record_102 s_value_198 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("(", sizeof("(") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_198;
			S_Array_i64 * v_names = s_array_i64_make(0, NULL);
			while (true) {
				if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1)))))) break;
				const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
				s_record_102 s_value_199 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("bound name", sizeof("bound name") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_199;
				s_record_103 v_leaf_result = s_leaf(s_record_102_copy(v_next), 6, v_name);
				s_record_102 s_value_200 = s_record_103_get_state_take(s_record_103_copy(v_leaf_result));
				s_record_102_drop(v_next);
				v_next = s_value_200;
				int64_t s_item_201 = s_record_103_get_node_take(s_record_103_copy(v_leaf_result));
				v_names = s_array_i64_append_take(v_names, s_item_201);
				if ((!s_is_token(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1))))
				{
					s_record_103_drop(v_leaf_result);
					break;
				}
				s_record_102 s_value_202 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(",", sizeof(",") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_202;
				s_record_103_drop(v_leaf_result);
			}
			s_record_102 s_value_203 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(")", sizeof(")") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_203;
			s_record_103 v_body = s_parse_block(s_record_102_copy(v_next));
			s_record_102 s_value_204 = s_record_103_get_state_take(s_record_103_copy(v_body));
			s_record_102_drop(v_next);
			v_next = s_value_204;
			int64_t s_item_205 = s_record_103_get_node_take(s_record_103_copy(v_body));
			v_names = s_array_i64_append_take(v_names, s_item_205);
			s_record_103 v_arm = s_append_node(s_record_102_copy(v_next), 11, v_position, s_array_i64_retain(v_names));
			s_record_102 s_value_206 = s_record_103_get_state_take(s_record_103_copy(v_arm));
			s_record_102_drop(v_next);
			v_next = s_value_206;
			int64_t s_item_207 = s_record_103_get_node_take(s_record_103_copy(v_arm));
			v_arms = s_array_i64_append_take(v_arms, s_item_207);
			s_record_103_drop(v_arm);
			s_record_103_drop(v_body);
			s_array_i64_release(v_names);
		}
		s_record_102 s_value_208 = s_expect_kind(s_record_102_copy(v_next), 8, s_string_from_bytes("dedent", sizeof("dedent") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_208;
		s_record_103 s_return_209 = s_append_node(s_record_102_copy(v_next), 27, v_token, s_array_i64_retain(v_arms));
		s_array_i64_release(v_arms);
		s_record_103_drop(v_value);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_209;
		s_array_i64_release(v_arms);
		s_record_103_drop(v_value);
	}
	if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("let", sizeof("let") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("var", sizeof("var") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("pub", sizeof("pub") - 1)) == 0)))
	{
		v_next.index += 1;
		const int64_t v_name = s_record_102_get_index_take(s_record_102_copy(v_next));
		s_record_102 s_value_210 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("binding name", sizeof("binding name") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_210;
		S_Array_i64 * v_kids = s_array_i64_make(0, NULL);
		if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("pub", sizeof("pub") - 1)) == 0) && (s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0)) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(".", sizeof(".") - 1))))
		{
			s_record_102 s_value_211 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(".", sizeof(".") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_211;
			const int64_t v_field = s_record_102_get_index_take(s_record_102_copy(v_next));
			s_record_102 s_value_212 = s_expect_kind(s_record_102_copy(v_next), 1, s_string_from_bytes("published field name", sizeof("published field name") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_212;
			s_record_103 v_reference = s_leaf(s_record_102_copy(v_next), 29, v_name);
			s_record_103 v_attr = s_unary_node(s_record_103_get_state_take(s_record_103_copy(v_reference)), 4, v_field, s_record_103_get_node_take(s_record_103_copy(v_reference)));
			s_record_102 s_value_213 = s_record_103_get_state_take(s_record_103_copy(v_attr));
			s_record_102_drop(v_next);
			v_next = s_value_213;
			int64_t s_item_214 = s_record_103_get_node_take(s_record_103_copy(v_attr));
			v_kids = s_array_i64_append_take(v_kids, s_item_214);
			s_record_103_drop(v_attr);
			s_record_103_drop(v_reference);
		}
		else
		{
			if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1))))
			{
				s_record_102 s_value_215 = s_expect(s_record_102_copy(v_next), s_string_from_bytes(":", sizeof(":") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_215;
				s_record_103 v_typ = s_parse_type(s_record_102_copy(v_next));
				s_record_102 s_value_216 = s_record_103_get_state_take(s_record_103_copy(v_typ));
				s_record_102_drop(v_next);
				v_next = s_value_216;
				int64_t s_item_217 = s_record_103_get_node_take(s_record_103_copy(v_typ));
				v_kids = s_array_i64_append_take(v_kids, s_item_217);
				s_record_103_drop(v_typ);
			}
			if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("=", sizeof("=") - 1))))
			{
				s_record_102 s_value_218 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("=", sizeof("=") - 1));
				s_record_102_drop(v_next);
				v_next = s_value_218;
				s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
				s_record_102 s_value_219 = s_record_103_get_state_take(s_record_103_copy(v_value));
				s_record_102_drop(v_next);
				v_next = s_value_219;
				int64_t s_item_220 = s_record_103_get_node_take(s_record_103_copy(v_value));
				v_kids = s_array_i64_append_take(v_kids, s_item_220);
				if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1))))
				{
					s_record_102 s_value_221 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1));
					s_record_102_drop(v_next);
					v_next = s_value_221;
					s_record_102 s_value_222 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("return", sizeof("return") - 1));
					s_record_102_drop(v_next);
					v_next = s_value_222;
					s_record_103 v_fallback = s_parse_expr(s_record_102_copy(v_next), 0);
					s_record_102 s_value_223 = s_record_103_get_state_take(s_record_103_copy(v_fallback));
					s_record_102_drop(v_next);
					v_next = s_value_223;
					s_record_103 v_node = s_unary_node(s_record_102_copy(v_next), 17, v_token, s_record_103_get_node_take(s_record_103_copy(v_fallback)));
					s_record_102 s_value_224 = s_record_103_get_state_take(s_record_103_copy(v_node));
					s_record_102_drop(v_next);
					v_next = s_value_224;
					int64_t s_item_225 = s_record_103_get_node_take(s_record_103_copy(v_node));
					v_kids = s_array_i64_append_take(v_kids, s_item_225);
					s_record_103_drop(v_node);
					s_record_103_drop(v_fallback);
				}
				s_record_103_drop(v_value);
			}
		}
		s_record_102 s_value_226 = s_expect_kind(s_record_102_copy(v_next), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_226;
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("let", sizeof("let") - 1)) == 0))
		{
			s_record_103 s_return_227 = s_append_node(s_record_102_copy(v_next), 26, v_name, s_array_i64_retain(v_kids));
			s_array_i64_release(v_kids);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_227;
		}
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("var", sizeof("var") - 1)) == 0))
		{
			s_record_103 s_return_228 = s_append_node(s_record_102_copy(v_next), 40, v_name, s_array_i64_retain(v_kids));
			s_array_i64_release(v_kids);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_228;
		}
		s_record_103 s_return_229 = s_append_node(s_record_102_copy(v_next), 32, v_name, s_array_i64_retain(v_kids));
		s_array_i64_release(v_kids);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_229;
		s_array_i64_release(v_kids);
	}
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("return", sizeof("return") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("recur", sizeof("recur") - 1)) == 0)))
	{
		v_next.index += 1;
		if (s_kind_is(s_record_102_copy(v_next), 6))
		{
			s_record_102 s_value_230 = s_expect_kind(s_record_102_copy(v_next), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_230;
			if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("return", sizeof("return") - 1)) == 0))
			{
				s_record_103 s_return_231 = s_leaf(s_record_102_copy(v_next), 35, v_token);
				s_record_102_drop(v_next);
				s_string_release(v_op);
				s_record_102_drop(v_p);
				return s_return_231;
			}
			s_record_103 s_return_232 = s_leaf(s_record_102_copy(v_next), 34, v_token);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_232;
		}
		s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_233 = s_expect_kind(s_record_103_get_state_take(s_record_103_copy(v_value)), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_233;
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("return", sizeof("return") - 1)) == 0))
		{
			s_record_103 s_return_234 = s_unary_node(s_record_102_copy(v_next), 35, v_token, s_record_103_get_node_take(s_record_103_copy(v_value)));
			s_record_103_drop(v_value);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_234;
		}
		s_record_103 s_return_235 = s_unary_node(s_record_102_copy(v_next), 34, v_token, s_record_103_get_node_take(s_record_103_copy(v_value)));
		s_record_103_drop(v_value);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_235;
		s_record_103_drop(v_value);
	}
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("break", sizeof("break") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("continue", sizeof("continue") - 1)) == 0)))
	{
		v_next.index += 1;
		s_record_102 s_value_236 = s_expect_kind(s_record_102_copy(v_next), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_236;
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("break", sizeof("break") - 1)) == 0))
		{
			s_record_103 s_return_237 = s_leaf(s_record_102_copy(v_next), 9, v_token);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_237;
		}
		s_record_103 s_return_238 = s_leaf(s_record_102_copy(v_next), 13, v_token);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_238;
	}
	s_record_103 v_lhs = s_parse_expr(s_record_102_copy(v_next), 0);
	s_record_102 s_value_239 = s_record_103_get_state_take(s_record_103_copy(v_lhs));
	s_record_102_drop(v_next);
	v_next = s_value_239;
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_103 s_return_240 = s_ParseResult(s_record_102_copy(v_next), (-1));
		s_record_103_drop(v_lhs);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_240;
	}
	S_String * v_assign = s_word(s_record_102_copy(v_next));
	if ((((((s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("=", sizeof("=") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("+=", sizeof("+=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("-=", sizeof("-=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("*=", sizeof("*=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_assign), s_string_from_bytes("/=", sizeof("/=") - 1)) == 0)))
	{
		const int64_t v_position = s_record_102_get_index_take(s_record_102_copy(v_next));
		v_next.index += 1;
		s_record_103 v_rhs = s_parse_expr(s_record_102_copy(v_next), 0);
		s_record_102 s_value_241 = s_expect_kind(s_record_103_get_state_take(s_record_103_copy(v_rhs)), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_241;
		s_record_103 s_return_242 = s_binary_node(s_record_102_copy(v_next), 3, v_position, s_record_103_get_node_take(s_record_103_copy(v_lhs)), s_record_103_get_node_take(s_record_103_copy(v_rhs)));
		s_record_103_drop(v_rhs);
		s_string_release(v_assign);
		s_record_103_drop(v_lhs);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_242;
		s_record_103_drop(v_rhs);
	}
	if (((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0) && s_is_token(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1))))
	{
		s_record_102 s_value_243 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_243;
		const int64_t v_exit_token = s_record_102_get_index_take(s_record_102_copy(v_next));
		s_record_103 v_exit_node = s_ParseResult(s_record_102_copy(v_next), (-1));
		if (s_is_token(s_record_102_copy(v_next), s_string_from_bytes("return", sizeof("return") - 1)))
		{
			s_record_102 s_value_244 = s_expect(s_record_102_copy(v_next), s_string_from_bytes("return", sizeof("return") - 1));
			s_record_102_drop(v_next);
			v_next = s_value_244;
			if (s_kind_is(s_record_102_copy(v_next), 6))
			{
				s_record_103 s_value_245 = s_leaf(s_record_102_copy(v_next), 35, v_exit_token);
				s_record_103_drop(v_exit_node);
				v_exit_node = s_value_245;
			}
			else
			{
				s_record_103 v_value = s_parse_expr(s_record_102_copy(v_next), 0);
				s_record_103 s_value_246 = s_unary_node(s_record_103_get_state_take(s_record_103_copy(v_value)), 35, v_exit_token, s_record_103_get_node_take(s_record_103_copy(v_value)));
				s_record_103_drop(v_exit_node);
				v_exit_node = s_value_246;
				s_record_103_drop(v_value);
			}
		}
		else if ((s_is_token(s_record_102_copy(v_next), s_string_from_bytes("break", sizeof("break") - 1)) || s_is_token(s_record_102_copy(v_next), s_string_from_bytes("continue", sizeof("continue") - 1))))
		{
			const bool v_is_break = s_is_token(s_record_102_copy(v_next), s_string_from_bytes("break", sizeof("break") - 1));
			v_next.index += 1;
			if (v_is_break)
			{
				s_record_103 s_value_247 = s_leaf(s_record_102_copy(v_next), 9, v_exit_token);
				s_record_103_drop(v_exit_node);
				v_exit_node = s_value_247;
			}
			else
			{
				s_record_103 s_value_248 = s_leaf(s_record_102_copy(v_next), 13, v_exit_token);
				s_record_103_drop(v_exit_node);
				v_exit_node = s_value_248;
			}
		}
		else
		{
			s_record_103 s_return_249 = s_ParseResult(s_fail(s_record_102_copy(v_next), s_string_from_bytes("guard else requires return, break, or continue", sizeof("guard else requires return, break, or continue") - 1)), (-1));
			s_record_103_drop(v_exit_node);
			s_string_release(v_assign);
			s_record_103_drop(v_lhs);
			s_record_102_drop(v_next);
			s_string_release(v_op);
			s_record_102_drop(v_p);
			return s_return_249;
		}
		s_record_102 s_value_250 = s_expect_kind(s_record_103_get_state_take(s_record_103_copy(v_exit_node)), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
		s_record_102_drop(v_next);
		v_next = s_value_250;
		s_record_103 s_return_251 = s_binary_node(s_record_102_copy(v_next), 43, v_token, s_record_103_get_node_take(s_record_103_copy(v_lhs)), s_record_103_get_node_take(s_record_103_copy(v_exit_node)));
		s_record_103_drop(v_exit_node);
		s_string_release(v_assign);
		s_record_103_drop(v_lhs);
		s_record_102_drop(v_next);
		s_string_release(v_op);
		s_record_102_drop(v_p);
		return s_return_251;
		s_record_103_drop(v_exit_node);
	}
	s_record_102 s_value_252 = s_expect_kind(s_record_102_copy(v_next), 6, s_string_from_bytes("newline", sizeof("newline") - 1));
	s_record_102_drop(v_next);
	v_next = s_value_252;
	s_record_103 s_return_253 = s_unary_node(s_record_102_copy(v_next), 18, v_token, s_record_103_get_node_take(s_record_103_copy(v_lhs)));
	s_string_release(v_assign);
	s_record_103_drop(v_lhs);
	s_record_102_drop(v_next);
	s_string_release(v_op);
	s_record_102_drop(v_p);
	return s_return_253;
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
		S_Array * v_s_iter_254 = s_array_retain(v_tokens);
		for (int64_t s_index_254 = 0; s_index_254 < (int64_t)s_array_length(v_s_iter_254); ++s_index_254)
		{
			s_record_100 v_token = s_ga_10100_get_borrow(v_s_iter_254, s_index_254);
			if ((s_record_100_get_kind_take(s_record_100_copy(v_token)) < 0))
			{
				S_String * s_field_255 = s_token_kind_name(s_record_100_get_kind_take(s_record_100_copy(v_token)));
				s_string_release(v_p.error);
				v_p.error = s_field_255;
				v_p.error_line = s_record_100_get_line_take(s_record_100_copy(v_token));
				s_record_103 s_return_256 = s_ParseResult(s_record_102_copy(v_p), (-1));
				s_record_100_drop(v_token);
				s_array_release(v_s_iter_254);
				s_record_102_drop(v_p);
				s_array_release(v_tokens);
				s_string_release(v_source);
				return s_return_256;
			}
			s_record_100_drop(v_token);
		}
		s_array_release(v_s_iter_254);
	}
	S_Array_i64 * v_children = s_array_i64_make(0, NULL);
	while (true) {
		if (!(((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (!s_kind_is(s_record_102_copy(v_p), 9))))) break;
		s_record_103 v_item = s_parse_statement(s_record_102_copy(v_p));
		s_record_102 s_value_257 = s_record_103_get_state_take(s_record_103_copy(v_item));
		s_record_102_drop(v_p);
		v_p = s_value_257;
		if ((s_string_compare_take(s_record_102_get_error_take(s_record_102_copy(v_p)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_103_drop(v_item);
			break;
		}
		int64_t s_item_258 = s_record_103_get_node_take(s_record_103_copy(v_item));
		v_children = s_array_i64_append_take(v_children, s_item_258);
		s_record_103_drop(v_item);
	}
	s_record_103 s_return_259 = s_append_node(s_record_102_copy(v_p), 28, 0, s_array_i64_retain(v_children));
	s_array_i64_release(v_children);
	s_record_102_drop(v_p);
	s_array_release(v_tokens);
	s_string_release(v_source);
	return s_return_259;
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
	for (int64_t v_tag = 0, s_bound_260 = 6; v_tag < s_bound_260; ++v_tag)
	{
		s_record_105 s_item_261 = s_TypeDescriptor(v_tag, 0, 0, s_ga_10104_make_take(0, NULL));
		v_entries = s_ga_10105_append_take(v_entries, s_item_261);
	}
	s_record_106 s_return_262 = s_TypeTable(s_array_retain(v_entries));
	s_array_release(v_entries);
	return s_return_262;
	s_array_release(v_entries);
}

s_record_107 s_intern_type(s_record_106 v_table, int64_t v_tag, int64_t v_first, int64_t v_second, S_Array * v_fields)
{
	for (int64_t v_i = 0, s_bound_263 = s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_table))); v_i < s_bound_263; ++v_i)
	{
		s_record_105 v_entry = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_table)), v_i);
		int64_t s_left_264 = s_generic_array_length_take(s_record_105_get_fields_take(s_record_105_copy(v_entry)));
		int64_t s_right_265 = s_generic_array_length_take(s_array_retain(v_fields));
		if (((((s_record_105_get_tag_take(s_record_105_copy(v_entry)) != v_tag) || (s_record_105_get_first_take(s_record_105_copy(v_entry)) != v_first)) || (s_record_105_get_second_take(s_record_105_copy(v_entry)) != v_second)) || (s_left_264 != s_right_265)))
		{
			s_record_105_drop(v_entry);
			continue;
		}
		bool v_equal = true;
		for (int64_t v_j = 0, s_bound_266 = s_generic_array_length_take(s_array_retain(v_fields)); v_j < s_bound_266; ++v_j)
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
			s_record_107 s_return_267 = s_InternedType(s_record_106_copy(v_table), v_i);
			s_record_105_drop(v_entry);
			s_array_release(v_fields);
			s_record_106_drop(v_table);
			return s_return_267;
		}
		s_record_105_drop(v_entry);
	}
	s_record_106 v_next = s_record_106_copy(v_table);
	const int64_t v_id = s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_next)));
	S_Array * s_field_268 = s_ga_10105_append_take(s_record_106_get_entries_take(s_record_106_copy(v_next)), s_TypeDescriptor(v_tag, v_first, v_second, s_array_retain(v_fields)));
	s_array_release(v_next.entries);
	v_next.entries = s_field_268;
	s_record_107 s_return_269 = s_InternedType(s_record_106_copy(v_next), v_id);
	s_record_106_drop(v_next);
	s_array_release(v_fields);
	s_record_106_drop(v_table);
	return s_return_269;
	s_record_106_drop(v_next);
}

s_record_107 s_intern_array_type(s_record_106 v_table, int64_t v_element)
{
	s_record_107 s_return_270 = s_intern_type(s_record_106_copy(v_table), 7, v_element, 0, s_ga_10104_make_take(0, NULL));
	s_record_106_drop(v_table);
	return s_return_270;
}

s_record_107 s_intern_dict_type(s_record_106 v_table, int64_t v_key, int64_t v_value)
{
	s_record_107 s_return_271 = s_intern_type(s_record_106_copy(v_table), 8, v_key, v_value, s_ga_10104_make_take(0, NULL));
	s_record_106_drop(v_table);
	return s_return_271;
}

s_record_107 s_intern_optional_type(s_record_106 v_table, int64_t v_value)
{
	s_record_107 s_return_272 = s_intern_type(s_record_106_copy(v_table), 9, v_value, 0, s_ga_10104_make_take(0, NULL));
	s_record_106_drop(v_table);
	return s_return_272;
}

S_Array * s_ordered_fields(S_Array * v_fields)
{
	S_Array * v_result = s_ga_10104_make_take(0, NULL);
	{
		S_Array * v_s_iter_273 = s_array_retain(v_fields);
		for (int64_t s_index_273 = 0; s_index_273 < (int64_t)s_array_length(v_s_iter_273); ++s_index_273)
		{
			s_record_104 v_field = s_ga_10104_get_borrow(v_s_iter_273, s_index_273);
			int64_t v_at = 0;
			while (true) {
				if (!(((v_at < s_generic_array_length_take(s_array_retain(v_result))) && (s_string_compare_take(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_result), v_at)), s_record_104_get_name_take(s_record_104_copy(v_field))) < 0)))) break;
				v_at += 1;
			}
			S_Array * v_sorted = s_ga_10104_make_take(0, NULL);
			for (int64_t v_i = 0, s_bound_274 = v_at; v_i < s_bound_274; ++v_i)
			{
				s_record_104 s_item_275 = s_ga_10104_get_checked_take(s_array_retain(v_result), v_i);
				v_sorted = s_ga_10104_append_take(v_sorted, s_item_275);
			}
			s_record_104 s_item_276 = s_record_104_copy(v_field);
			v_sorted = s_ga_10104_append_take(v_sorted, s_item_276);
			for (int64_t v_i = v_at, s_bound_277 = s_generic_array_length_take(s_array_retain(v_result)); v_i < s_bound_277; ++v_i)
			{
				s_record_104 s_item_278 = s_ga_10104_get_checked_take(s_array_retain(v_result), v_i);
				v_sorted = s_ga_10104_append_take(v_sorted, s_item_278);
			}
			S_Array * s_value_279 = s_array_retain(v_sorted);
			s_array_release(v_result);
			v_result = s_value_279;
			s_array_release(v_sorted);
			s_record_104_drop(v_field);
		}
		s_array_release(v_s_iter_273);
	}
	S_Array * s_return_280 = s_array_retain(v_result);
	s_array_release(v_result);
	s_array_release(v_fields);
	return s_return_280;
	s_array_release(v_result);
}

s_record_107 s_intern_record_type(s_record_106 v_table, S_Array * v_fields)
{
	s_record_107 s_return_281 = s_intern_type(s_record_106_copy(v_table), 6, 0, 0, s_ordered_fields(s_array_retain(v_fields)));
	s_array_release(v_fields);
	s_record_106_drop(v_table);
	return s_return_281;
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
	for (int64_t v_i = 0, s_bound_282 = s_generic_array_length_take(s_array_retain(v_symbols)); v_i < s_bound_282; ++v_i)
	{
		const int64_t v_index = ((s_generic_array_length_take(s_array_retain(v_symbols)) - 1) - v_i);
		s_record_110 v_symbol = s_ga_10110_get_checked_take(s_array_retain(v_symbols), v_index);
		if ((s_string_compare_take(s_record_110_get_name_take(s_record_110_copy(v_symbol)), s_string_retain(v_name)) == 0))
		{
			s_record_109 s_return_283 = s_HirResolution(s_record_110_get_type_id_take(s_record_110_copy(v_symbol)), v_index, s_string_from_bytes("", sizeof("") - 1));
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_array_release(v_symbols);
			return s_return_283;
		}
		s_record_110_drop(v_symbol);
	}
	s_record_109 s_return_284 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown name '", sizeof("unknown name '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
	s_string_release(v_name);
	s_array_release(v_symbols);
	return s_return_284;
}

bool s_hir_types_equal(int64_t v_left, int64_t v_right)
{
	return ((v_left == v_right) && (v_left != 0));
}

s_record_109 s_resolve_hir_field(s_record_106 v_types, int64_t v_owner, S_String * v_name)
{
	if (((v_owner < 0) || (v_owner >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		s_record_109 s_return_285 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown record field '", sizeof("unknown record field '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_285;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_owner);
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 6))
	{
		for (int64_t v_i = 0, s_bound_286 = s_generic_array_length_take(s_record_105_get_fields_take(s_record_105_copy(v_descriptor))); v_i < s_bound_286; ++v_i)
		{
			s_record_104 v_field = s_ga_10104_get_checked_take(s_record_105_get_fields_take(s_record_105_copy(v_descriptor)), v_i);
			if ((s_string_compare_take(s_record_104_get_name_take(s_record_104_copy(v_field)), s_string_retain(v_name)) == 0))
			{
				s_record_109 s_return_287 = s_HirResolution(s_record_104_get_kind_take(s_record_104_copy(v_field)), v_i, s_string_from_bytes("", sizeof("") - 1));
				s_record_104_drop(v_field);
				s_record_105_drop(v_descriptor);
				s_string_release(v_name);
				s_record_106_drop(v_types);
				return s_return_287;
			}
			s_record_104_drop(v_field);
		}
	}
	s_record_109 s_return_288 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown record field '", sizeof("unknown record field '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
	s_record_105_drop(v_descriptor);
	s_string_release(v_name);
	s_record_106_drop(v_types);
	return s_return_288;
	s_record_105_drop(v_descriptor);
}

s_record_109 s_resolve_hir_binary(s_record_106 v_types, int64_t v_left, int64_t v_right, S_String * v_op)
{
	if ((v_left != v_right))
	{
		s_record_109 s_return_289 = s_HirResolution(0, (-1), s_string_from_bytes("binary operand types differ", sizeof("binary operand types differ") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_289;
	}
	if (((v_left < 0) || (v_left >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		s_record_109 s_return_290 = s_HirResolution(0, (-1), s_string_from_bytes("unknown binary operand type", sizeof("unknown binary operand type") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_290;
	}
	const int64_t v_tag = s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_left));
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("and", sizeof("and") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("or", sizeof("or") - 1)) == 0)))
	{
		if ((v_tag != 3))
		{
			s_record_109 s_return_291 = s_HirResolution(0, (-1), s_string_from_bytes("logical operator requires bool", sizeof("logical operator requires bool") - 1));
			s_string_release(v_op);
			s_record_106_drop(v_types);
			return s_return_291;
		}
		s_record_109 s_return_292 = s_HirResolution(3, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_292;
	}
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("==", sizeof("==") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("!=", sizeof("!=") - 1)) == 0)))
	{
		s_record_109 s_return_293 = s_HirResolution(3, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_293;
	}
	if (((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<", sizeof("<") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<=", sizeof("<=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">", sizeof(">") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">=", sizeof(">=") - 1)) == 0)))
	{
		if ((((v_tag != 1) && (v_tag != 2)) && (v_tag != 4)))
		{
			s_record_109 s_return_294 = s_HirResolution(0, (-1), s_string_from_bytes("comparison requires numeric or string operands", sizeof("comparison requires numeric or string operands") - 1));
			s_string_release(v_op);
			s_record_106_drop(v_types);
			return s_return_294;
		}
		s_record_109 s_return_295 = s_HirResolution(3, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_295;
	}
	if ((((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("-", sizeof("-") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("*", sizeof("*") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("/", sizeof("/") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("%", sizeof("%") - 1)) == 0)))
	{
		if ((((v_tag != 1) && (v_tag != 2)) && (!((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0) && (v_tag == 4)))))
		{
			s_record_109 s_return_296 = s_HirResolution(0, (-1), s_string_from_bytes("arithmetic requires numeric operands", sizeof("arithmetic requires numeric operands") - 1));
			s_string_release(v_op);
			s_record_106_drop(v_types);
			return s_return_296;
		}
		if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("%", sizeof("%") - 1)) == 0) && (v_tag != 1)))
		{
			s_record_109 s_return_297 = s_HirResolution(0, (-1), s_string_from_bytes("remainder requires i64", sizeof("remainder requires i64") - 1));
			s_string_release(v_op);
			s_record_106_drop(v_types);
			return s_return_297;
		}
		s_record_109 s_return_298 = s_HirResolution(v_left, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_298;
	}
	s_record_109 s_return_299 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unsupported binary operator '", sizeof("unsupported binary operator '") - 1), s_string_retain(v_op)), s_string_from_bytes("'", sizeof("'") - 1)));
	s_string_release(v_op);
	s_record_106_drop(v_types);
	return s_return_299;
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
		s_record_109 s_return_300 = s_HirResolution(0, (-1), s_string_from_bytes("invalid unary operand", sizeof("invalid unary operand") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_300;
	}
	const int64_t v_tag = s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_operand));
	if (((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("not", sizeof("not") - 1)) == 0) && (v_tag == 3)))
	{
		s_record_109 s_return_301 = s_HirResolution(3, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_301;
	}
	if ((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("-", sizeof("-") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0)) && ((v_tag == 1) || (v_tag == 2))))
	{
		s_record_109 s_return_302 = s_HirResolution(v_operand, (-1), s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_op);
		s_record_106_drop(v_types);
		return s_return_302;
	}
	s_record_109 s_return_303 = s_HirResolution(0, (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unsupported unary operator '", sizeof("unsupported unary operator '") - 1), s_string_retain(v_op)), s_string_from_bytes("'", sizeof("'") - 1)));
	s_string_release(v_op);
	s_record_106_drop(v_types);
	return s_return_303;
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
		S_String * s_return_304 = s_string_from_bytes("unknown", sizeof("unknown") - 1);
		s_record_106_drop(v_types);
		return s_return_304;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_id);
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 1))
	{
		S_String * s_return_305 = s_string_from_bytes("i64", sizeof("i64") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_305;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 2))
	{
		S_String * s_return_306 = s_string_from_bytes("f64", sizeof("f64") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_306;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 3))
	{
		S_String * s_return_307 = s_string_from_bytes("bool", sizeof("bool") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_307;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 4))
	{
		S_String * s_return_308 = s_string_from_bytes("string", sizeof("string") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_308;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 5))
	{
		S_String * s_return_309 = s_string_from_bytes("void", sizeof("void") - 1);
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_309;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 7))
	{
		S_String * s_return_310 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("Array[", sizeof("Array[") - 1), s_hir_type_name(s_record_106_copy(v_types), s_record_105_get_first_take(s_record_105_copy(v_descriptor)))), s_string_from_bytes("]", sizeof("]") - 1));
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_310;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 8))
	{
		S_String * s_left_311 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("Dict[", sizeof("Dict[") - 1), s_hir_type_name(s_record_106_copy(v_types), s_record_105_get_first_take(s_record_105_copy(v_descriptor)))), s_string_from_bytes(",", sizeof(",") - 1));
		S_String * s_right_312 = s_hir_type_name(s_record_106_copy(v_types), s_record_105_get_second_take(s_record_105_copy(v_descriptor)));
		S_String * s_return_313 = s_string_concat_take(s_string_concat_take(s_left_311, s_right_312), s_string_from_bytes("]", sizeof("]") - 1));
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_313;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 9))
	{
		S_String * s_return_314 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("Optional[", sizeof("Optional[") - 1), s_hir_type_name(s_record_106_copy(v_types), s_record_105_get_first_take(s_record_105_copy(v_descriptor)))), s_string_from_bytes("]", sizeof("]") - 1));
		s_record_105_drop(v_descriptor);
		s_record_106_drop(v_types);
		return s_return_314;
	}
	S_String * s_return_315 = s_string_from_bytes("record", sizeof("record") - 1);
	s_record_105_drop(v_descriptor);
	s_record_106_drop(v_types);
	return s_return_315;
	s_record_105_drop(v_descriptor);
}

s_record_111 s_hir_expected(s_record_106 v_types, int64_t v_expected, int64_t v_found)
{
	if (s_hir_types_equal(v_expected, v_found))
	{
		s_record_111 s_return_316 = s_HirCallCheck(s_record_106_copy(v_types), v_expected, s_string_from_bytes("", sizeof("") - 1));
		s_record_106_drop(v_types);
		return s_return_316;
	}
	S_String * s_left_317 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_hir_type_name(s_record_106_copy(v_types), v_expected)), s_string_from_bytes(", found ", sizeof(", found ") - 1));
	S_String * s_right_318 = s_hir_type_name(s_record_106_copy(v_types), v_found);
	s_record_111 s_return_319 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_concat_take(s_left_317, s_right_318));
	s_record_106_drop(v_types);
	return s_return_319;
}

s_record_111 s_hir_call_arguments(s_record_106 v_types, S_Array_i64 * v_actual, S_Array_i64 * v_expected, int64_t v_result, S_String * v_arity_error)
{
	int64_t s_left_320 = s_array_i64_length_take(s_array_i64_retain(v_actual));
	int64_t s_right_321 = s_array_i64_length_take(s_array_i64_retain(v_expected));
	if ((s_left_320 != s_right_321))
	{
		s_record_111 s_return_322 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_retain(v_arity_error));
		s_string_release(v_arity_error);
		s_array_i64_release(v_expected);
		s_array_i64_release(v_actual);
		s_record_106_drop(v_types);
		return s_return_322;
	}
	for (int64_t v_i = 0, s_bound_323 = s_array_i64_length_take(s_array_i64_retain(v_actual)); v_i < s_bound_323; ++v_i)
	{
		s_record_111 v_check = s_hir_expected(s_record_106_copy(v_types), s_array_i64_get_checked_take(s_array_i64_retain(v_expected), v_i), s_array_i64_get_checked_take(s_array_i64_retain(v_actual), v_i));
		if (!((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_check)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
			s_record_111 s_return_324 = s_record_111_copy(v_check);
			s_record_111_drop(v_check);
			s_string_release(v_arity_error);
			s_array_i64_release(v_expected);
			s_array_i64_release(v_actual);
			s_record_106_drop(v_types);
			return s_return_324;
		}
		s_record_111_drop(v_check);
	}
	s_record_111 s_return_325 = s_HirCallCheck(s_record_106_copy(v_types), v_result, s_string_from_bytes("", sizeof("") - 1));
	s_string_release(v_arity_error);
	s_array_i64_release(v_expected);
	s_array_i64_release(v_actual);
	s_record_106_drop(v_types);
	return s_return_325;
}

bool s_hir_is_builtin(S_String * v_name)
{
	bool s_return_326 = (((((((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("arguments", sizeof("arguments") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("read_text", sizeof("read_text") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("write_text", sizeof("write_text") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("len", sizeof("len") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("str", sizeof("str") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("sqrt", sizeof("sqrt") - 1)) == 0));
	s_string_release(v_name);
	return s_return_326;
}

s_record_111 s_resolve_hir_builtin_call(s_record_106 v_types, S_String * v_name, S_Array_i64 * v_args)
{
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("arguments", sizeof("arguments") - 1)) == 0))
	{
		s_record_107 v_array = s_intern_array_type(s_record_106_copy(v_types), 4);
		s_record_111 s_return_327 = s_hir_call_arguments(s_record_107_get_table_take(s_record_107_copy(v_array)), s_array_i64_retain(v_args), s_array_i64_make(0, NULL), s_record_107_get_id_take(s_record_107_copy(v_array)), s_string_from_bytes("arguments expects no arguments", sizeof("arguments expects no arguments") - 1));
		s_record_107_drop(v_array);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_327;
		s_record_107_drop(v_array);
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("read_text", sizeof("read_text") - 1)) == 0))
	{
		s_record_107 v_optional = s_intern_optional_type(s_record_106_copy(v_types), 4);
		s_record_111 s_return_328 = s_hir_call_arguments(s_record_107_get_table_take(s_record_107_copy(v_optional)), s_array_i64_retain(v_args), s_array_i64_make(1, (int64_t[]){4}), s_record_107_get_id_take(s_record_107_copy(v_optional)), s_string_from_bytes("read_text expects one string argument", sizeof("read_text expects one string argument") - 1));
		s_record_107_drop(v_optional);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_328;
		s_record_107_drop(v_optional);
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("write_text", sizeof("write_text") - 1)) == 0))
	{
		s_record_111 s_return_329 = s_hir_call_arguments(s_record_106_copy(v_types), s_array_i64_retain(v_args), s_array_i64_make(2, (int64_t[]){4, 4}), 3, s_string_from_bytes("write_text expects two string arguments", sizeof("write_text expects two string arguments") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_329;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("str", sizeof("str") - 1)) == 0))
	{
		if (((s_array_i64_length_take(s_array_i64_retain(v_args)) != 1) || (s_array_i64_get_checked_take(s_array_i64_retain(v_args), 0) != 1)))
		{
			s_record_111 s_return_330 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("str expects one i64 argument", sizeof("str expects one i64 argument") - 1));
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_106_drop(v_types);
			return s_return_330;
		}
		s_record_111 s_return_331 = s_HirCallCheck(s_record_106_copy(v_types), 4, s_string_from_bytes("", sizeof("") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_331;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("sqrt", sizeof("sqrt") - 1)) == 0))
	{
		s_record_111 s_return_332 = s_hir_call_arguments(s_record_106_copy(v_types), s_array_i64_retain(v_args), s_array_i64_make(1, (int64_t[]){2}), 2, s_string_from_bytes("sqrt expects one argument", sizeof("sqrt expects one argument") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_332;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0))
	{
		if ((s_array_i64_length_take(s_array_i64_retain(v_args)) != 1))
		{
			s_record_111 s_return_333 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("print expects one argument", sizeof("print expects one argument") - 1));
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_106_drop(v_types);
			return s_return_333;
		}
		const int64_t v_id = s_array_i64_get_checked_take(s_array_i64_retain(v_args), 0);
		if (((((v_id == 1) || (v_id == 2)) || (v_id == 3)) || (v_id == 4)))
		{
			s_record_111 s_return_334 = s_HirCallCheck(s_record_106_copy(v_types), 5, s_string_from_bytes("", sizeof("") - 1));
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_106_drop(v_types);
			return s_return_334;
		}
		s_record_111 s_return_335 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("cannot print this type", sizeof("cannot print this type") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_335;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("len", sizeof("len") - 1)) == 0))
	{
		if ((s_array_i64_length_take(s_array_i64_retain(v_args)) != 1))
		{
			s_record_111 s_return_336 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("len expects one argument", sizeof("len expects one argument") - 1));
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_106_drop(v_types);
			return s_return_336;
		}
		const int64_t v_id = s_array_i64_get_checked_take(s_array_i64_retain(v_args), 0);
		if (((v_id >= 0) && (v_id < s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
		{
			const int64_t v_tag = s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_id));
			if ((((v_tag == 4) || (v_tag == 7)) || (v_tag == 8)))
			{
				s_record_111 s_return_337 = s_HirCallCheck(s_record_106_copy(v_types), 1, s_string_from_bytes("", sizeof("") - 1));
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_106_drop(v_types);
				return s_return_337;
			}
		}
		s_record_111 s_return_338 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("len expects a string or array", sizeof("len expects a string or array") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_338;
	}
	s_record_111 s_return_339 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("unknown builtin", sizeof("unknown builtin") - 1));
	s_array_i64_release(v_args);
	s_string_release(v_name);
	s_record_106_drop(v_types);
	return s_return_339;
}

bool s_hir_is_string_method(S_String * v_name)
{
	bool s_return_340 = ((((((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("slice", sizeof("slice") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("find", sizeof("find") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("starts_with", sizeof("starts_with") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("ends_with", sizeof("ends_with") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("join", sizeof("join") - 1)) == 0));
	s_string_release(v_name);
	return s_return_340;
}

s_record_111 s_resolve_hir_string_call(s_record_106 v_types, int64_t v_receiver, S_String * v_method, S_Array_i64 * v_args)
{
	s_record_106 v_current = s_record_106_copy(v_types);
	s_record_111 v_check = s_hir_expected(s_record_106_copy(v_types), 4, v_receiver);
	if (!((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_check)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
		s_record_111 s_return_341 = s_record_111_copy(v_check);
		s_record_111_drop(v_check);
		s_record_106_drop(v_current);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_341;
	}
	S_Array_i64 * v_expected = s_array_i64_make(0, NULL);
	int64_t v_result = 3;
	if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0))
	{
		S_Array_i64 * s_value_342 = s_array_i64_make(1, (int64_t[]){1});
		s_array_i64_release(v_expected);
		v_expected = s_value_342;
	}
	else if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("slice", sizeof("slice") - 1)) == 0))
	{
		S_Array_i64 * s_value_343 = s_array_i64_make(2, (int64_t[]){1, 1});
		s_array_i64_release(v_expected);
		v_expected = s_value_343;
		v_result = 4;
	}
	else if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("find", sizeof("find") - 1)) == 0))
	{
		S_Array_i64 * s_value_344 = s_array_i64_make(1, (int64_t[]){4});
		s_array_i64_release(v_expected);
		v_expected = s_value_344;
	}
	else if (((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("starts_with", sizeof("starts_with") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("ends_with", sizeof("ends_with") - 1)) == 0)))
	{
		S_Array_i64 * s_value_345 = s_array_i64_make(1, (int64_t[]){4});
		s_array_i64_release(v_expected);
		v_expected = s_value_345;
	}
	else if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("join", sizeof("join") - 1)) == 0))
	{
		s_record_107 v_array = s_intern_array_type(s_record_106_copy(v_types), 4);
		s_record_106 s_value_346 = s_record_107_get_table_take(s_record_107_copy(v_array));
		s_record_106_drop(v_current);
		v_current = s_value_346;
		S_Array_i64 * s_value_347 = s_array_i64_make(1, (int64_t[]){s_record_107_get_id_take(s_record_107_copy(v_array))});
		s_array_i64_release(v_expected);
		v_expected = s_value_347;
		v_result = 4;
		s_record_107_drop(v_array);
	}
	if (((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("find", sizeof("find") - 1)) == 0)))
	{
		s_record_107 v_optional = s_intern_optional_type(s_record_106_copy(v_types), 1);
		s_record_106 s_value_348 = s_record_107_get_table_take(s_record_107_copy(v_optional));
		s_record_106_drop(v_current);
		v_current = s_value_348;
		v_result = s_record_107_get_id_take(s_record_107_copy(v_optional));
		s_record_107_drop(v_optional);
	}
	s_record_111 s_return_349 = s_hir_call_arguments(s_record_106_copy(v_current), s_array_i64_retain(v_args), s_array_i64_retain(v_expected), v_result, s_string_from_bytes("string method argument count mismatch", sizeof("string method argument count mismatch") - 1));
	s_array_i64_release(v_expected);
	s_record_111_drop(v_check);
	s_record_106_drop(v_current);
	s_array_i64_release(v_args);
	s_string_release(v_method);
	s_record_106_drop(v_types);
	return s_return_349;
	s_array_i64_release(v_expected);
	s_record_111_drop(v_check);
	s_record_106_drop(v_current);
}

s_record_111 s_resolve_hir_function_call(s_record_106 v_types, S_String * v_name, S_Array_i64 * v_args, S_Array_i64 * v_params, int64_t v_result)
{
	int64_t s_left_350 = s_array_i64_length_take(s_array_i64_retain(v_args));
	int64_t s_right_351 = s_array_i64_length_take(s_array_i64_retain(v_params));
	if ((s_left_350 != s_right_351))
	{
		s_record_111 s_return_352 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_concat_take(s_string_concat_take(s_string_from_bytes("argument count mismatch for '", sizeof("argument count mismatch for '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
		s_array_i64_release(v_params);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_106_drop(v_types);
		return s_return_352;
	}
	for (int64_t v_i = 0, s_bound_353 = s_array_i64_length_take(s_array_i64_retain(v_args)); v_i < s_bound_353; ++v_i)
	{
		if ((s_array_i64_get_checked_take(s_array_i64_retain(v_params), v_i) != 0))
		{
			s_record_111 v_check = s_hir_expected(s_record_106_copy(v_types), s_array_i64_get_checked_take(s_array_i64_retain(v_params), v_i), s_array_i64_get_checked_take(s_array_i64_retain(v_args), v_i));
			if (!((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_check)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
				s_record_111 s_return_354 = s_record_111_copy(v_check);
				s_record_111_drop(v_check);
				s_array_i64_release(v_params);
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_106_drop(v_types);
				return s_return_354;
			}
			s_record_111_drop(v_check);
		}
	}
	s_record_111 s_return_355 = s_HirCallCheck(s_record_106_copy(v_types), v_result, s_string_from_bytes("", sizeof("") - 1));
	s_array_i64_release(v_params);
	s_array_i64_release(v_args);
	s_string_release(v_name);
	s_record_106_drop(v_types);
	return s_return_355;
}

s_record_111 s_resolve_hir_collection_call(s_record_106 v_types, int64_t v_receiver, S_String * v_method, S_Array_i64 * v_args)
{
	if (((v_receiver < 0) || (v_receiver >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		s_record_111 s_return_356 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("unknown collection receiver type", sizeof("unknown collection receiver type") - 1));
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_356;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_receiver);
	if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("append", sizeof("append") - 1)) == 0))
	{
		if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) != 7))
		{
			s_record_111 s_return_357 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("append requires an array", sizeof("append requires an array") - 1));
			s_record_105_drop(v_descriptor);
			s_array_i64_release(v_args);
			s_string_release(v_method);
			s_record_106_drop(v_types);
			return s_return_357;
		}
		s_record_111 s_return_358 = s_hir_call_arguments(s_record_106_copy(v_types), s_array_i64_retain(v_args), s_array_i64_make(1, (int64_t[]){s_record_105_get_first_take(s_record_105_copy(v_descriptor))}), v_receiver, s_string_from_bytes("append expects one argument", sizeof("append expects one argument") - 1));
		s_record_105_drop(v_descriptor);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_358;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) != 8))
	{
		s_record_111 s_return_359 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("dictionary method requires Dict", sizeof("dictionary method requires Dict") - 1));
		s_record_105_drop(v_descriptor);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_359;
	}
	if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("insert", sizeof("insert") - 1)) == 0))
	{
		s_record_111 s_return_360 = s_hir_call_arguments(s_record_106_copy(v_types), s_array_i64_retain(v_args), s_array_i64_make(2, (int64_t[]){s_record_105_get_first_take(s_record_105_copy(v_descriptor)), s_record_105_get_second_take(s_record_105_copy(v_descriptor))}), v_receiver, s_string_from_bytes("dictionary method argument count mismatch", sizeof("dictionary method argument count mismatch") - 1));
		s_record_105_drop(v_descriptor);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_360;
	}
	if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("get", sizeof("get") - 1)) == 0))
	{
		s_record_107 v_optional = s_intern_optional_type(s_record_106_copy(v_types), s_record_105_get_second_take(s_record_105_copy(v_descriptor)));
		s_record_111 s_return_361 = s_hir_call_arguments(s_record_107_get_table_take(s_record_107_copy(v_optional)), s_array_i64_retain(v_args), s_array_i64_make(1, (int64_t[]){s_record_105_get_first_take(s_record_105_copy(v_descriptor))}), s_record_107_get_id_take(s_record_107_copy(v_optional)), s_string_from_bytes("dictionary method argument count mismatch", sizeof("dictionary method argument count mismatch") - 1));
		s_record_107_drop(v_optional);
		s_record_105_drop(v_descriptor);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_106_drop(v_types);
		return s_return_361;
		s_record_107_drop(v_optional);
	}
	s_record_111 s_return_362 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("unknown collection method", sizeof("unknown collection method") - 1));
	s_record_105_drop(v_descriptor);
	s_array_i64_release(v_args);
	s_string_release(v_method);
	s_record_106_drop(v_types);
	return s_return_362;
	s_record_105_drop(v_descriptor);
}

s_record_112 s_HirRecordEdit(S_Array * v_fields, int64_t v_index, S_String * v_error)
{
	s_record_112 s_constructor_result = {0};
	s_constructor_result.fields = v_fields;
	s_constructor_result.index = v_index;
	s_constructor_result.error = v_error;
	return s_constructor_result;
}

int64_t s_hir_record_field(S_Array * v_fields, S_String * v_name)
{
	for (int64_t v_i = 0, s_bound_363 = s_generic_array_length_take(s_array_retain(v_fields)); v_i < s_bound_363; ++v_i)
	{
		if ((s_string_compare_take(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_fields), v_i)), s_string_retain(v_name)) == 0))
		{
			int64_t s_return_364 = v_i;
			s_string_release(v_name);
			s_array_release(v_fields);
			return s_return_364;
		}
	}
	int64_t s_return_365 = (-1);
	s_string_release(v_name);
	s_array_release(v_fields);
	return s_return_365;
}

s_record_112 s_hir_record_put(S_Array * v_fields, S_String * v_name, int64_t v_type_id)
{
	if (((v_type_id == 0) || (v_type_id == 5)))
	{
		s_record_112 s_return_366 = s_HirRecordEdit(s_array_retain(v_fields), (-1), s_string_from_bytes("invalid record field type", sizeof("invalid record field type") - 1));
		s_string_release(v_name);
		s_array_release(v_fields);
		return s_return_366;
	}
	const int64_t v_index = s_hir_record_field(s_array_retain(v_fields), s_string_retain(v_name));
	S_Array * v_result = s_array_retain(v_fields);
	if ((v_index >= 0))
	{
		s_ga_10104_set_take(&v_result, v_index, s_RecordField(s_string_retain(v_name), v_type_id));
	}
	else
	{
		s_record_104 s_item_367 = s_RecordField(s_string_retain(v_name), v_type_id);
		v_result = s_ga_10104_append_take(v_result, s_item_367);
	}
	s_record_112 s_return_368 = s_HirRecordEdit(s_array_retain(v_result), v_index, s_string_from_bytes("", sizeof("") - 1));
	s_array_release(v_result);
	s_string_release(v_name);
	s_array_release(v_fields);
	return s_return_368;
	s_array_release(v_result);
}

s_record_112 s_hir_record_delete(S_Array * v_fields, S_String * v_name)
{
	const int64_t v_index = s_hir_record_field(s_array_retain(v_fields), s_string_retain(v_name));
	if ((v_index < 0))
	{
		s_record_112 s_return_369 = s_HirRecordEdit(s_array_retain(v_fields), (-1), s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown deleted field '", sizeof("unknown deleted field '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
		s_string_release(v_name);
		s_array_release(v_fields);
		return s_return_369;
	}
	S_Array * v_result = s_ga_10104_make_take(0, NULL);
	for (int64_t v_i = 0, s_bound_370 = s_generic_array_length_take(s_array_retain(v_fields)); v_i < s_bound_370; ++v_i)
	{
		if ((v_i != v_index))
		{
			s_record_104 s_item_371 = s_ga_10104_get_checked_take(s_array_retain(v_fields), v_i);
			v_result = s_ga_10104_append_take(v_result, s_item_371);
		}
	}
	s_record_112 s_return_372 = s_HirRecordEdit(s_array_retain(v_result), v_index, s_string_from_bytes("", sizeof("") - 1));
	s_array_release(v_result);
	s_string_release(v_name);
	s_array_release(v_fields);
	return s_return_372;
	s_array_release(v_result);
}

s_record_112 s_hir_record_spread(s_record_106 v_types, S_Array * v_fields, int64_t v_owner)
{
	if (((v_owner < 0) || (v_owner >= s_generic_array_length_take(s_record_106_get_entries_take(s_record_106_copy(v_types))))))
	{
		s_record_112 s_return_373 = s_HirRecordEdit(s_array_retain(v_fields), (-1), s_string_from_bytes("record spread expects a record", sizeof("record spread expects a record") - 1));
		s_array_release(v_fields);
		s_record_106_drop(v_types);
		return s_return_373;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_106_copy(v_types)), v_owner);
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) != 6))
	{
		s_record_112 s_return_374 = s_HirRecordEdit(s_array_retain(v_fields), (-1), s_string_from_bytes("record spread expects a record", sizeof("record spread expects a record") - 1));
		s_record_105_drop(v_descriptor);
		s_array_release(v_fields);
		s_record_106_drop(v_types);
		return s_return_374;
	}
	S_Array * v_result = s_array_retain(v_fields);
	{
		S_Array * v_s_iter_375 = s_record_105_get_fields_take(s_record_105_copy(v_descriptor));
		for (int64_t s_index_375 = 0; s_index_375 < (int64_t)s_array_length(v_s_iter_375); ++s_index_375)
		{
			s_record_104 v_field = s_ga_10104_get_borrow(v_s_iter_375, s_index_375);
			S_Array * s_value_376 = s_record_112_get_fields_take(s_hir_record_put(s_array_retain(v_result), s_record_104_get_name_take(s_record_104_copy(v_field)), s_record_104_get_kind_take(s_record_104_copy(v_field))));
			s_array_release(v_result);
			v_result = s_value_376;
			s_record_104_drop(v_field);
		}
		s_array_release(v_s_iter_375);
	}
	s_record_112 s_return_377 = s_HirRecordEdit(s_array_retain(v_result), (-1), s_string_from_bytes("", sizeof("") - 1));
	s_array_release(v_result);
	s_record_105_drop(v_descriptor);
	s_array_release(v_fields);
	s_record_106_drop(v_types);
	return s_return_377;
	s_array_release(v_result);
	s_record_105_drop(v_descriptor);
}

s_record_111 s_resolve_hir_record(s_record_106 v_types, S_Array * v_fields)
{
	if ((s_generic_array_length_take(s_array_retain(v_fields)) == 0))
	{
		s_record_111 s_return_378 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("empty records are not supported yet", sizeof("empty records are not supported yet") - 1));
		s_array_release(v_fields);
		s_record_106_drop(v_types);
		return s_return_378;
	}
	s_record_107 v_interned = s_intern_record_type(s_record_106_copy(v_types), s_array_retain(v_fields));
	s_record_111 s_return_379 = s_HirCallCheck(s_record_107_get_table_take(s_record_107_copy(v_interned)), s_record_107_get_id_take(s_record_107_copy(v_interned)), s_string_from_bytes("", sizeof("") - 1));
	s_record_107_drop(v_interned);
	s_array_release(v_fields);
	s_record_106_drop(v_types);
	return s_return_379;
	s_record_107_drop(v_interned);
}

s_record_112 s_hir_constructor_publish(S_Array * v_fields, S_String * v_name, int64_t v_type_id)
{
	if ((s_hir_record_field(s_array_retain(v_fields), s_string_retain(v_name)) >= 0))
	{
		s_record_112 s_return_380 = s_HirRecordEdit(s_array_retain(v_fields), (-1), s_string_from_bytes("constructor field overrides are not supported yet", sizeof("constructor field overrides are not supported yet") - 1));
		s_string_release(v_name);
		s_array_release(v_fields);
		return s_return_380;
	}
	s_record_112 s_return_381 = s_hir_record_put(s_array_retain(v_fields), s_string_retain(v_name), v_type_id);
	s_string_release(v_name);
	s_array_release(v_fields);
	return s_return_381;
}

s_record_113 s_HirParameterDecl(S_String * v_name, int64_t v_source, int64_t v_type_node)
{
	s_record_113 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.source = v_source;
	s_constructor_result.type_node = v_type_node;
	return s_constructor_result;
}

s_record_114 s_HirFunctionDecl(S_String * v_name, int64_t v_source, int64_t v_body, bool v_constructor, int64_t v_result_type_node, S_Array * v_parameters)
{
	s_record_114 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.source = v_source;
	s_constructor_result.body = v_body;
	s_constructor_result.constructor = v_constructor;
	s_constructor_result.result_type_node = v_result_type_node;
	s_constructor_result.parameters = v_parameters;
	return s_constructor_result;
}

s_record_115 s_HirModuleDecl(S_Array * v_functions, S_String * v_error, int64_t v_error_line)
{
	s_record_115 s_constructor_result = {0};
	s_constructor_result.functions = v_functions;
	s_constructor_result.error = v_error;
	s_constructor_result.error_line = v_error_line;
	return s_constructor_result;
}

int64_t s_hir_ast_child(s_record_102 v_tree, int64_t v_id, int64_t v_offset)
{
	s_record_101 v_node = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_id);
	int64_t s_return_382 = s_array_i64_get_checked_take(s_record_102_get_edges_take(s_record_102_copy(v_tree)), (s_record_101_get_first_take(s_record_101_copy(v_node)) + v_offset));
	s_record_101_drop(v_node);
	s_record_102_drop(v_tree);
	return s_return_382;
	s_record_101_drop(v_node);
}

S_String * s_hir_ast_text(s_record_102 v_tree, int64_t v_id)
{
	S_String * s_return_383 = s_token_text(s_record_102_get_source_take(s_record_102_copy(v_tree)), s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_tree)), s_record_101_get_token_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_id))));
	s_record_102_drop(v_tree);
	return s_return_383;
}

int64_t s_hir_ast_line(s_record_102 v_tree, int64_t v_id)
{
	int64_t s_return_384 = s_record_100_get_line_take(s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_tree)), s_record_101_get_token_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_id))));
	s_record_102_drop(v_tree);
	return s_return_384;
}

s_record_115 s_hir_declaration_error(S_Array * v_functions, S_String * v_message, int64_t v_line)
{
	s_record_115 s_return_385 = s_HirModuleDecl(s_array_retain(v_functions), s_string_retain(v_message), v_line);
	s_string_release(v_message);
	s_array_release(v_functions);
	return s_return_385;
}

s_record_115 s_hir_module_declarations(s_record_102 v_tree, int64_t v_root)
{
	S_Array * v_functions = s_ga_10114_make_take(0, NULL);
	S_Array * v_names = s_ga_8_make_take(0, NULL);
	s_record_101 v_module = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_root);
	for (int64_t v_i = 0, s_bound_386 = s_record_101_get_count_take(s_record_101_copy(v_module)); v_i < s_bound_386; ++v_i)
	{
		const int64_t v_id = s_hir_ast_child(s_record_102_copy(v_tree), v_root, v_i);
		s_record_101 v_entry = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_id);
		S_String * v_name = s_hir_ast_text(s_record_102_copy(v_tree), v_id);
		if ((((s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 14) && (s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 42)) && (s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 26)))
		{
			s_record_115 s_return_387 = s_hir_declaration_error(s_array_retain(v_functions), s_string_from_bytes("unsupported module statement", sizeof("unsupported module statement") - 1), s_hir_ast_line(s_record_102_copy(v_tree), v_id));
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_101_drop(v_module);
			s_array_release(v_names);
			s_array_release(v_functions);
			s_record_102_drop(v_tree);
			return s_return_387;
		}
		if (((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0) && ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 14) || (s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 42))))
		{
			s_record_115 s_return_388 = s_hir_declaration_error(s_array_retain(v_functions), s_string_concat_take(s_string_concat_take(s_string_from_bytes("duplicate function '", sizeof("duplicate function '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)), s_hir_ast_line(s_record_102_copy(v_tree), v_id));
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_101_drop(v_module);
			s_array_release(v_names);
			s_array_release(v_functions);
			s_record_102_drop(v_tree);
			return s_return_388;
		}
		{
			S_Array * v_s_iter_389 = s_array_retain(v_names);
			for (int64_t s_index_389 = 0; s_index_389 < (int64_t)s_array_length(v_s_iter_389); ++s_index_389)
			{
				S_String * v_seen = s_ga_8_get_borrow(v_s_iter_389, s_index_389);
				if ((s_string_compare_take(s_string_retain(v_seen), s_string_retain(v_name)) == 0))
				{
					if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 26))
					{
						s_record_115 s_return_390 = s_hir_declaration_error(s_array_retain(v_functions), s_string_from_bytes("duplicate module name", sizeof("duplicate module name") - 1), s_hir_ast_line(s_record_102_copy(v_tree), v_id));
						s_string_release(v_seen);
						s_array_release(v_s_iter_389);
						s_string_release(v_name);
						s_record_101_drop(v_entry);
						s_record_101_drop(v_module);
						s_array_release(v_names);
						s_array_release(v_functions);
						s_record_102_drop(v_tree);
						return s_return_390;
					}
					s_record_115 s_return_391 = s_hir_declaration_error(s_array_retain(v_functions), s_string_concat_take(s_string_concat_take(s_string_from_bytes("duplicate function '", sizeof("duplicate function '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)), s_hir_ast_line(s_record_102_copy(v_tree), v_id));
					s_string_release(v_seen);
					s_array_release(v_s_iter_389);
					s_string_release(v_name);
					s_record_101_drop(v_entry);
					s_record_101_drop(v_module);
					s_array_release(v_names);
					s_array_release(v_functions);
					s_record_102_drop(v_tree);
					return s_return_391;
				}
				s_string_release(v_seen);
			}
			s_array_release(v_s_iter_389);
		}
		S_String * s_item_392 = s_string_retain(v_name);
		v_names = s_ga_8_append_take(v_names, s_item_392);
		if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 26))
		{
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			continue;
		}
		S_Array * v_parameters = s_ga_10113_make_take(0, NULL);
		int64_t v_return_type = (-1);
		for (int64_t v_j = 0, s_bound_393 = (s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1); v_j < s_bound_393; ++v_j)
		{
			const int64_t v_part = s_hir_ast_child(s_record_102_copy(v_tree), v_id, v_j);
			s_record_101 v_info = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_part);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_info)) == 31))
			{
				int64_t v_annotation = (-1);
				if ((s_record_101_get_count_take(s_record_101_copy(v_info)) > 0))
				{
					v_annotation = s_hir_ast_child(s_record_102_copy(v_tree), v_part, 0);
					if (((s_record_101_get_count_take(s_record_101_copy(v_info)) != 1) || (s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_annotation)) != 38)))
					{
						s_record_115 s_return_394 = s_hir_declaration_error(s_array_retain(v_functions), s_string_from_bytes("parameter defaults are not supported yet", sizeof("parameter defaults are not supported yet") - 1), s_hir_ast_line(s_record_102_copy(v_tree), v_part));
						s_record_101_drop(v_info);
						s_array_release(v_parameters);
						s_string_release(v_name);
						s_record_101_drop(v_entry);
						s_record_101_drop(v_module);
						s_array_release(v_names);
						s_array_release(v_functions);
						s_record_102_drop(v_tree);
						return s_return_394;
					}
				}
				S_String * v_pname = s_hir_ast_text(s_record_102_copy(v_tree), v_part);
				{
					S_Array * v_s_iter_395 = s_array_retain(v_parameters);
					for (int64_t s_index_395 = 0; s_index_395 < (int64_t)s_array_length(v_s_iter_395); ++s_index_395)
					{
						s_record_113 v_previous = s_ga_10113_get_borrow(v_s_iter_395, s_index_395);
						if ((s_string_compare_take(s_record_113_get_name_take(s_record_113_copy(v_previous)), s_string_retain(v_pname)) == 0))
						{
							s_record_115 s_return_396 = s_hir_declaration_error(s_array_retain(v_functions), s_string_concat_take(s_string_concat_take(s_string_from_bytes("duplicate parameter '", sizeof("duplicate parameter '") - 1), s_string_retain(v_pname)), s_string_from_bytes("'", sizeof("'") - 1)), s_hir_ast_line(s_record_102_copy(v_tree), v_part));
							s_record_113_drop(v_previous);
							s_array_release(v_s_iter_395);
							s_string_release(v_pname);
							s_record_101_drop(v_info);
							s_array_release(v_parameters);
							s_string_release(v_name);
							s_record_101_drop(v_entry);
							s_record_101_drop(v_module);
							s_array_release(v_names);
							s_array_release(v_functions);
							s_record_102_drop(v_tree);
							return s_return_396;
						}
						s_record_113_drop(v_previous);
					}
					s_array_release(v_s_iter_395);
				}
				s_record_113 s_item_397 = s_HirParameterDecl(s_string_retain(v_pname), v_part, v_annotation);
				v_parameters = s_ga_10113_append_take(v_parameters, s_item_397);
				s_string_release(v_pname);
			}
			else if ((s_record_101_get_kind_take(s_record_101_copy(v_info)) == 38))
			{
				v_return_type = v_part;
			}
			s_record_101_drop(v_info);
		}
		const int64_t v_body_id = s_hir_ast_child(s_record_102_copy(v_tree), v_id, (s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1));
		if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 14))
		{
			s_record_101 v_body = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_body_id);
			for (int64_t v_j = 0, s_bound_398 = s_record_101_get_count_take(s_record_101_copy(v_body)); v_j < s_bound_398; ++v_j)
			{
				if ((s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), s_hir_ast_child(s_record_102_copy(v_tree), v_body_id, v_j))) == 32))
				{
					s_record_115 s_return_399 = s_hir_declaration_error(s_array_retain(v_functions), s_string_from_bytes("def constructors are no longer supported; use rec", sizeof("def constructors are no longer supported; use rec") - 1), s_hir_ast_line(s_record_102_copy(v_tree), v_id));
					s_record_101_drop(v_body);
					s_array_release(v_parameters);
					s_string_release(v_name);
					s_record_101_drop(v_entry);
					s_record_101_drop(v_module);
					s_array_release(v_names);
					s_array_release(v_functions);
					s_record_102_drop(v_tree);
					return s_return_399;
				}
			}
			s_record_101_drop(v_body);
		}
		s_record_114 s_item_400 = s_HirFunctionDecl(s_string_retain(v_name), v_id, v_body_id, (s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 42), v_return_type, s_array_retain(v_parameters));
		v_functions = s_ga_10114_append_take(v_functions, s_item_400);
		s_array_release(v_parameters);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
	}
	s_record_115 s_return_401 = s_HirModuleDecl(s_array_retain(v_functions), s_string_from_bytes("", sizeof("") - 1), 0);
	s_record_101_drop(v_module);
	s_array_release(v_names);
	s_array_release(v_functions);
	s_record_102_drop(v_tree);
	return s_return_401;
	s_record_101_drop(v_module);
	s_array_release(v_names);
	s_array_release(v_functions);
}

s_record_116 s_HirBodyState(s_record_102 v_tree, S_Array * v_names, S_Array * v_constants)
{
	s_record_116 s_constructor_result = {0};
	s_constructor_result.tree = v_tree;
	s_constructor_result.names = v_names;
	s_constructor_result.constants = v_constants;
	s_constructor_result.symbols = s_ga_10110_make_take(0, NULL);
	s_constructor_result.nodes = s_ga_10108_make_take(0, NULL);
	s_constructor_result.types = s_new_type_table();
	s_constructor_result.depth = 0;
	s_constructor_result.last_type = 0;
	s_constructor_result.error = s_string_from_bytes("", sizeof("") - 1);
	s_constructor_result.error_line = 0;
	s_constructor_result.functions = s_ga_10114_make_take(0, NULL);
	s_constructor_result.active_functions = s_ga_8_make_take(0, NULL);
	s_constructor_result.inferred_return = 0;
	return s_constructor_result;
}

s_record_117 s_HirBodyAnalysis(S_Array * v_nodes, S_String * v_error, int64_t v_error_line)
{
	s_record_117 s_constructor_result = {0};
	s_constructor_result.nodes = v_nodes;
	s_constructor_result.error = v_error;
	s_constructor_result.error_line = v_error_line;
	return s_constructor_result;
}

s_record_116 s_hir_body_error(s_record_116 v_state, int64_t v_id, S_String * v_message)
{
	s_record_116 v_result = s_record_116_copy(v_state);
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		S_String * s_field_402 = s_string_retain(v_message);
		s_string_release(v_result.error);
		v_result.error = s_field_402;
		v_result.error_line = s_hir_ast_line(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
	}
	s_record_116 s_return_403 = s_record_116_copy(v_result);
	s_record_116_drop(v_result);
	s_string_release(v_message);
	s_record_116_drop(v_state);
	return s_return_403;
	s_record_116_drop(v_result);
}

s_record_111 s_hir_body_type_reference(s_record_106 v_types, s_record_102 v_tree, int64_t v_id)
{
	s_record_101 v_node = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 29) || (s_record_101_get_kind_take(s_record_101_copy(v_node)) == 38)))
	{
		S_String * v_name = s_hir_ast_text(s_record_102_copy(v_tree), v_id);
		if ((s_record_101_get_count_take(s_record_101_copy(v_node)) == 0))
		{
			if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("i64", sizeof("i64") - 1)) == 0))
			{
				s_record_111 s_return_404 = s_HirCallCheck(s_record_106_copy(v_types), 1, s_string_from_bytes("", sizeof("") - 1));
				s_string_release(v_name);
				s_record_101_drop(v_node);
				s_record_102_drop(v_tree);
				s_record_106_drop(v_types);
				return s_return_404;
			}
			if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("f64", sizeof("f64") - 1)) == 0))
			{
				s_record_111 s_return_405 = s_HirCallCheck(s_record_106_copy(v_types), 2, s_string_from_bytes("", sizeof("") - 1));
				s_string_release(v_name);
				s_record_101_drop(v_node);
				s_record_102_drop(v_tree);
				s_record_106_drop(v_types);
				return s_return_405;
			}
			if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("bool", sizeof("bool") - 1)) == 0))
			{
				s_record_111 s_return_406 = s_HirCallCheck(s_record_106_copy(v_types), 3, s_string_from_bytes("", sizeof("") - 1));
				s_string_release(v_name);
				s_record_101_drop(v_node);
				s_record_102_drop(v_tree);
				s_record_106_drop(v_types);
				return s_return_406;
			}
			if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("string", sizeof("string") - 1)) == 0))
			{
				s_record_111 s_return_407 = s_HirCallCheck(s_record_106_copy(v_types), 4, s_string_from_bytes("", sizeof("") - 1));
				s_string_release(v_name);
				s_record_101_drop(v_node);
				s_record_102_drop(v_tree);
				s_record_106_drop(v_types);
				return s_return_407;
			}
			s_record_111 s_return_408 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("", sizeof("") - 1));
			s_string_release(v_name);
			s_record_101_drop(v_node);
			s_record_102_drop(v_tree);
			s_record_106_drop(v_types);
			return s_return_408;
		}
		s_string_release(v_name);
	}
	else
	{
		if (((s_record_101_get_kind_take(s_record_101_copy(v_node)) != 23) || (s_record_101_get_count_take(s_record_101_copy(v_node)) < 2)))
		{
			s_record_111 s_return_409 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("", sizeof("") - 1));
			s_record_101_drop(v_node);
			s_record_102_drop(v_tree);
			s_record_106_drop(v_types);
			return s_return_409;
		}
	}
	S_String * v_name = s_hir_ast_text(s_record_102_copy(v_tree), v_id);
	int64_t v_first = 0;
	if ((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 23))
	{
		S_String *s_value_410 = s_hir_ast_text(s_record_102_copy(v_tree), s_hir_ast_child(s_record_102_copy(v_tree), v_id, 0));
		s_string_release(v_name);
		v_name = s_value_410;
		v_first = 1;
	}
	if ((((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Array", sizeof("Array") - 1)) != 0) && (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Dict", sizeof("Dict") - 1)) != 0)) && (s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Optional", sizeof("Optional") - 1)) != 0)))
	{
		s_record_111 s_return_411 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_name);
		s_record_101_drop(v_node);
		s_record_102_drop(v_tree);
		s_record_106_drop(v_types);
		return s_return_411;
	}
	if ((((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Dict", sizeof("Dict") - 1)) == 0) && (s_record_101_get_count_take(s_record_101_copy(v_node)) != (v_first + 2))) || ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Dict", sizeof("Dict") - 1)) != 0) && (s_record_101_get_count_take(s_record_101_copy(v_node)) != (v_first + 1)))))
	{
		s_record_111 s_return_412 = s_HirCallCheck(s_record_106_copy(v_types), 0, s_string_from_bytes("", sizeof("") - 1));
		s_string_release(v_name);
		s_record_101_drop(v_node);
		s_record_102_drop(v_tree);
		s_record_106_drop(v_types);
		return s_return_412;
	}
	s_record_111 v_element = s_hir_body_type_reference(s_record_106_copy(v_types), s_record_102_copy(v_tree), s_hir_ast_child(s_record_102_copy(v_tree), v_id, v_first));
	if ((s_record_111_get_type_id_take(s_record_111_copy(v_element)) == 0))
	{
		s_record_111 s_return_413 = s_record_111_copy(v_element);
		s_record_111_drop(v_element);
		s_string_release(v_name);
		s_record_101_drop(v_node);
		s_record_102_drop(v_tree);
		s_record_106_drop(v_types);
		return s_return_413;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Array", sizeof("Array") - 1)) == 0))
	{
		s_record_107 v_array = s_intern_array_type(s_record_111_get_table_take(s_record_111_copy(v_element)), s_record_111_get_type_id_take(s_record_111_copy(v_element)));
		s_record_111 s_return_414 = s_HirCallCheck(s_record_107_get_table_take(s_record_107_copy(v_array)), s_record_107_get_id_take(s_record_107_copy(v_array)), s_string_from_bytes("", sizeof("") - 1));
		s_record_107_drop(v_array);
		s_record_111_drop(v_element);
		s_string_release(v_name);
		s_record_101_drop(v_node);
		s_record_102_drop(v_tree);
		s_record_106_drop(v_types);
		return s_return_414;
		s_record_107_drop(v_array);
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Optional", sizeof("Optional") - 1)) == 0))
	{
		s_record_107 v_optional = s_intern_optional_type(s_record_111_get_table_take(s_record_111_copy(v_element)), s_record_111_get_type_id_take(s_record_111_copy(v_element)));
		s_record_111 s_return_415 = s_HirCallCheck(s_record_107_get_table_take(s_record_107_copy(v_optional)), s_record_107_get_id_take(s_record_107_copy(v_optional)), s_string_from_bytes("", sizeof("") - 1));
		s_record_107_drop(v_optional);
		s_record_111_drop(v_element);
		s_string_release(v_name);
		s_record_101_drop(v_node);
		s_record_102_drop(v_tree);
		s_record_106_drop(v_types);
		return s_return_415;
		s_record_107_drop(v_optional);
	}
	s_record_111 v_value = s_hir_body_type_reference(s_record_111_get_table_take(s_record_111_copy(v_element)), s_record_102_copy(v_tree), s_hir_ast_child(s_record_102_copy(v_tree), v_id, (v_first + 1)));
	if ((s_record_111_get_type_id_take(s_record_111_copy(v_value)) == 0))
	{
		s_record_111 s_return_416 = s_record_111_copy(v_value);
		s_record_111_drop(v_value);
		s_record_111_drop(v_element);
		s_string_release(v_name);
		s_record_101_drop(v_node);
		s_record_102_drop(v_tree);
		s_record_106_drop(v_types);
		return s_return_416;
	}
	s_record_107 v_dict = s_intern_dict_type(s_record_111_get_table_take(s_record_111_copy(v_value)), s_record_111_get_type_id_take(s_record_111_copy(v_element)), s_record_111_get_type_id_take(s_record_111_copy(v_value)));
	s_record_111 s_return_417 = s_HirCallCheck(s_record_107_get_table_take(s_record_107_copy(v_dict)), s_record_107_get_id_take(s_record_107_copy(v_dict)), s_string_from_bytes("", sizeof("") - 1));
	s_record_107_drop(v_dict);
	s_record_111_drop(v_value);
	s_record_111_drop(v_element);
	s_string_release(v_name);
	s_record_101_drop(v_node);
	s_record_102_drop(v_tree);
	s_record_106_drop(v_types);
	return s_return_417;
	s_record_107_drop(v_dict);
	s_record_111_drop(v_value);
	s_record_111_drop(v_element);
	s_string_release(v_name);
	s_record_101_drop(v_node);
}

s_record_116 s_hir_body_binding(s_record_116 v_state, int64_t v_id, S_String * v_name, int64_t v_type_id, bool v_mutable)
{
	s_record_116 v_result = s_record_116_copy(v_state);
	{
		S_Array * v_s_iter_418 = s_record_116_get_symbols_take(s_record_116_copy(v_result));
		for (int64_t s_index_418 = 0; s_index_418 < (int64_t)s_array_length(v_s_iter_418); ++s_index_418)
		{
			s_record_110 v_symbol = s_ga_10110_get_borrow(v_s_iter_418, s_index_418);
			if (((s_string_compare_take(s_record_110_get_name_take(s_record_110_copy(v_symbol)), s_string_retain(v_name)) == 0) && (s_record_110_get_depth_take(s_record_110_copy(v_symbol)) == s_record_116_get_depth_take(s_record_116_copy(v_result)))))
			{
				s_record_116 s_return_419 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("duplicate binding '", sizeof("duplicate binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
				s_record_110_drop(v_symbol);
				s_array_release(v_s_iter_418);
				s_record_116_drop(v_result);
				s_string_release(v_name);
				s_record_116_drop(v_state);
				return s_return_419;
			}
			s_record_110_drop(v_symbol);
		}
		s_array_release(v_s_iter_418);
	}
	S_Array * s_field_420 = s_ga_10110_append_take(s_record_116_get_symbols_take(s_record_116_copy(v_result)), s_SymbolInfo(s_string_retain(v_name), v_type_id, v_type_id, v_mutable, s_record_116_get_depth_take(s_record_116_copy(v_result))));
	s_array_release(v_result.symbols);
	v_result.symbols = s_field_420;
	S_Array * s_field_421 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 9, v_type_id, (s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_result))) - 1)));
	s_array_release(v_result.nodes);
	v_result.nodes = s_field_421;
	s_record_116 s_return_422 = s_record_116_copy(v_result);
	s_record_116_drop(v_result);
	s_string_release(v_name);
	s_record_116_drop(v_state);
	return s_return_422;
	s_record_116_drop(v_result);
}

s_record_116 s_hir_body_user_call(s_record_116 v_state, int64_t v_id, S_String * v_name, S_Array_i64 * v_args)
{
	s_record_116 v_result = s_record_116_copy(v_state);
	bool v_found = false;
	s_record_114 v_function = s_HirFunctionDecl(s_string_from_bytes("", sizeof("") - 1), (-1), (-1), false, (-1), s_ga_10113_make_take(0, NULL));
	{
		S_Array * v_s_iter_423 = s_record_116_get_functions_take(s_record_116_copy(v_state));
		for (int64_t s_index_423 = 0; s_index_423 < (int64_t)s_array_length(v_s_iter_423); ++s_index_423)
		{
			s_record_114 v_candidate = s_ga_10114_get_borrow(v_s_iter_423, s_index_423);
			if ((s_string_compare_take(s_record_114_get_name_take(s_record_114_copy(v_candidate)), s_string_retain(v_name)) == 0))
			{
				s_record_114 s_value_424 = s_record_114_copy(v_candidate);
				s_record_114_drop(v_function);
				v_function = s_value_424;
				v_found = true;
			}
			s_record_114_drop(v_candidate);
		}
		s_array_release(v_s_iter_423);
	}
	if ((!v_found))
	{
		s_record_116 s_return_425 = s_record_116_copy(v_result);
		s_record_114_drop(v_function);
		s_record_116_drop(v_result);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_116_drop(v_state);
		return s_return_425;
	}
	int64_t s_left_426 = s_array_i64_length_take(s_array_i64_retain(v_args));
	int64_t s_right_427 = s_generic_array_length_take(s_record_114_get_parameters_take(s_record_114_copy(v_function)));
	if ((s_left_426 != s_right_427))
	{
		s_record_116 s_return_428 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("argument count mismatch for '", sizeof("argument count mismatch for '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
		s_record_114_drop(v_function);
		s_record_116_drop(v_result);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_116_drop(v_state);
		return s_return_428;
	}
	for (int64_t v_i = 0, s_bound_429 = s_array_i64_length_take(s_array_i64_retain(v_args)); v_i < s_bound_429; ++v_i)
	{
		s_record_113 v_parameter = s_ga_10113_get_checked_take(s_record_114_get_parameters_take(s_record_114_copy(v_function)), v_i);
		if ((s_record_113_get_type_node_take(s_record_113_copy(v_parameter)) >= 0))
		{
			s_record_111 v_annotation = s_hir_body_type_reference(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_tree_take(s_record_116_copy(v_result)), s_record_113_get_type_node_take(s_record_113_copy(v_parameter)));
			s_record_106 s_field_430 = s_record_111_get_table_take(s_record_111_copy(v_annotation));
			s_record_106_drop(v_result.types);
			v_result.types = s_field_430;
			if (((s_record_111_get_type_id_take(s_record_111_copy(v_annotation)) != 0) && (s_record_111_get_type_id_take(s_record_111_copy(v_annotation)) != s_array_i64_get_checked_take(s_array_i64_retain(v_args), v_i))))
			{
				S_String * s_left_431 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_111_get_type_id_take(s_record_111_copy(v_annotation)))), s_string_from_bytes(", found ", sizeof(", found ") - 1));
				S_String * s_right_432 = s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), s_array_i64_get_checked_take(s_array_i64_retain(v_args), v_i));
				s_record_116 s_return_433 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_left_431, s_right_432));
				s_record_111_drop(v_annotation);
				s_record_113_drop(v_parameter);
				s_record_114_drop(v_function);
				s_record_116_drop(v_result);
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_116_drop(v_state);
				return s_return_433;
			}
			s_record_111_drop(v_annotation);
		}
		s_record_113_drop(v_parameter);
	}
	if (s_record_114_get_constructor_take(s_record_114_copy(v_function)))
	{
		s_record_116 v_nested = s_HirBodyState(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_record_116_get_names_take(s_record_116_copy(v_state)), s_record_116_get_constants_take(s_record_116_copy(v_state)));
		s_record_106 s_field_434 = s_record_116_get_types_take(s_record_116_copy(v_result));
		s_record_106_drop(v_nested.types);
		v_nested.types = s_field_434;
		S_Array * s_field_435 = s_record_116_get_functions_take(s_record_116_copy(v_state));
		s_array_release(v_nested.functions);
		v_nested.functions = s_field_435;
		S_Array * s_field_436 = s_record_116_get_active_functions_take(s_record_116_copy(v_state));
		s_array_release(v_nested.active_functions);
		v_nested.active_functions = s_field_436;
		for (int64_t v_i = 0, s_bound_437 = s_array_i64_length_take(s_array_i64_retain(v_args)); v_i < s_bound_437; ++v_i)
		{
			s_record_113 v_param = s_ga_10113_get_checked_take(s_record_114_get_parameters_take(s_record_114_copy(v_function)), v_i);
			s_record_116 s_value_438 = s_hir_body_binding(s_record_116_copy(v_nested), s_record_113_get_source_take(s_record_113_copy(v_param)), s_record_113_get_name_take(s_record_113_copy(v_param)), s_array_i64_get_checked_take(s_array_i64_retain(v_args), v_i), false);
			s_record_116_drop(v_nested);
			v_nested = s_value_438;
			s_record_113_drop(v_param);
		}
		S_Array * v_fields = s_ga_10104_make_take(0, NULL);
		s_record_101 v_block = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), s_record_114_get_body_take(s_record_114_copy(v_function)));
		for (int64_t v_i = 0, s_bound_439 = s_record_101_get_count_take(s_record_101_copy(v_block)); v_i < s_bound_439; ++v_i)
		{
			const int64_t v_pub_id = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_record_114_get_body_take(s_record_114_copy(v_function)), v_i);
			s_record_101 v_pub = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_pub_id);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_pub)) != 32))
			{
				s_record_116 s_return_440 = s_record_116_copy(v_result);
				s_record_101_drop(v_pub);
				s_record_101_drop(v_block);
				s_array_release(v_fields);
				s_record_116_drop(v_nested);
				s_record_114_drop(v_function);
				s_record_116_drop(v_result);
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_116_drop(v_state);
				return s_return_440;
			}
			S_String * v_field_name = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_pub_id);
			int64_t v_field_type = 0;
			if ((s_record_101_get_count_take(s_record_101_copy(v_pub)) == 0))
			{
				s_record_109 v_symbol = s_resolve_hir_name(s_record_116_get_symbols_take(s_record_116_copy(v_nested)), s_string_retain(v_field_name));
				v_field_type = s_record_109_get_type_id_take(s_record_109_copy(v_symbol));
				s_record_109_drop(v_symbol);
			}
			else
			{
				const int64_t v_expression = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_pub_id, 0);
				if ((s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_expression)) == 4))
				{
					S_String *s_value_441 = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_expression);
					s_string_release(v_field_name);
					v_field_name = s_value_441;
				}
				s_record_116 s_value_442 = s_hir_body_expression(s_record_116_copy(v_nested), v_expression);
				s_record_116_drop(v_nested);
				v_nested = s_value_442;
				v_field_type = s_record_116_get_last_type_take(s_record_116_copy(v_nested));
			}
			if (((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_nested)), s_string_from_bytes("", sizeof("") - 1)) != 0) || (v_field_type == 0)))
			{
				s_record_116 s_return_443 = s_record_116_copy(v_result);
				s_string_release(v_field_name);
				s_record_101_drop(v_pub);
				s_record_101_drop(v_block);
				s_array_release(v_fields);
				s_record_116_drop(v_nested);
				s_record_114_drop(v_function);
				s_record_116_drop(v_result);
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_116_drop(v_state);
				return s_return_443;
			}
			s_record_112 v_edit = s_hir_constructor_publish(s_array_retain(v_fields), s_string_retain(v_field_name), v_field_type);
			if ((s_string_compare_take(s_record_112_get_error_take(s_record_112_copy(v_edit)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_444 = s_hir_body_error(s_record_116_copy(v_result), v_pub_id, s_record_112_get_error_take(s_record_112_copy(v_edit)));
				s_record_112_drop(v_edit);
				s_string_release(v_field_name);
				s_record_101_drop(v_pub);
				s_record_101_drop(v_block);
				s_array_release(v_fields);
				s_record_116_drop(v_nested);
				s_record_114_drop(v_function);
				s_record_116_drop(v_result);
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_116_drop(v_state);
				return s_return_444;
			}
			S_Array * s_value_445 = s_record_112_get_fields_take(s_record_112_copy(v_edit));
			s_array_release(v_fields);
			v_fields = s_value_445;
			s_record_112_drop(v_edit);
			s_string_release(v_field_name);
			s_record_101_drop(v_pub);
		}
		if ((s_generic_array_length_take(s_array_retain(v_fields)) > 0))
		{
			s_record_107 v_record = s_intern_record_type(s_record_116_get_types_take(s_record_116_copy(v_nested)), s_array_retain(v_fields));
			s_record_106 s_field_446 = s_record_107_get_table_take(s_record_107_copy(v_record));
			s_record_106_drop(v_result.types);
			v_result.types = s_field_446;
			v_result.last_type = s_record_107_get_id_take(s_record_107_copy(v_record));
			s_record_107_drop(v_record);
		}
		s_record_116 s_return_447 = s_record_116_copy(v_result);
		s_record_101_drop(v_block);
		s_array_release(v_fields);
		s_record_116_drop(v_nested);
		s_record_114_drop(v_function);
		s_record_116_drop(v_result);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_116_drop(v_state);
		return s_return_447;
		s_record_101_drop(v_block);
		s_array_release(v_fields);
		s_record_116_drop(v_nested);
	}
	if ((s_record_114_get_result_type_node_take(s_record_114_copy(v_function)) >= 0))
	{
		s_record_111 v_declared = s_hir_body_type_reference(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_tree_take(s_record_116_copy(v_result)), s_record_114_get_result_type_node_take(s_record_114_copy(v_function)));
		s_record_106 s_field_448 = s_record_111_get_table_take(s_record_111_copy(v_declared));
		s_record_106_drop(v_result.types);
		v_result.types = s_field_448;
		v_result.last_type = s_record_111_get_type_id_take(s_record_111_copy(v_declared));
		if ((s_record_111_get_type_id_take(s_record_111_copy(v_declared)) != 0))
		{
			s_record_116 s_return_449 = s_record_116_copy(v_result);
			s_record_111_drop(v_declared);
			s_record_114_drop(v_function);
			s_record_116_drop(v_result);
			s_array_i64_release(v_args);
			s_string_release(v_name);
			s_record_116_drop(v_state);
			return s_return_449;
		}
		s_record_111_drop(v_declared);
	}
	{
		S_Array * v_s_iter_450 = s_record_116_get_active_functions_take(s_record_116_copy(v_state));
		for (int64_t s_index_450 = 0; s_index_450 < (int64_t)s_array_length(v_s_iter_450); ++s_index_450)
		{
			S_String * v_active = s_ga_8_get_borrow(v_s_iter_450, s_index_450);
			if ((s_string_compare_take(s_string_retain(v_active), s_string_retain(v_name)) == 0))
			{
				s_record_116 s_return_451 = s_record_116_copy(v_result);
				s_string_release(v_active);
				s_array_release(v_s_iter_450);
				s_record_114_drop(v_function);
				s_record_116_drop(v_result);
				s_array_i64_release(v_args);
				s_string_release(v_name);
				s_record_116_drop(v_state);
				return s_return_451;
			}
			s_string_release(v_active);
		}
		s_array_release(v_s_iter_450);
	}
	s_record_116 v_nested = s_HirBodyState(s_record_116_get_tree_take(s_record_116_copy(v_state)), s_record_116_get_names_take(s_record_116_copy(v_state)), s_record_116_get_constants_take(s_record_116_copy(v_state)));
	s_record_106 s_field_452 = s_record_116_get_types_take(s_record_116_copy(v_result));
	s_record_106_drop(v_nested.types);
	v_nested.types = s_field_452;
	S_Array * s_field_453 = s_record_116_get_functions_take(s_record_116_copy(v_state));
	s_array_release(v_nested.functions);
	v_nested.functions = s_field_453;
	S_Array * s_field_454 = s_ga_8_append_take(s_record_116_get_active_functions_take(s_record_116_copy(v_state)), s_string_retain(v_name));
	s_array_release(v_nested.active_functions);
	v_nested.active_functions = s_field_454;
	for (int64_t v_i = 0, s_bound_455 = s_array_i64_length_take(s_array_i64_retain(v_args)); v_i < s_bound_455; ++v_i)
	{
		s_record_113 v_parameter = s_ga_10113_get_checked_take(s_record_114_get_parameters_take(s_record_114_copy(v_function)), v_i);
		s_record_116 s_value_456 = s_hir_body_binding(s_record_116_copy(v_nested), s_record_113_get_source_take(s_record_113_copy(v_parameter)), s_record_113_get_name_take(s_record_113_copy(v_parameter)), s_array_i64_get_checked_take(s_array_i64_retain(v_args), v_i), false);
		s_record_116_drop(v_nested);
		v_nested = s_value_456;
		s_record_113_drop(v_parameter);
	}
	s_record_116 s_value_457 = s_hir_body_block(s_record_116_copy(v_nested), s_record_114_get_body_take(s_record_114_copy(v_function)));
	s_record_116_drop(v_nested);
	v_nested = s_value_457;
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_nested)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		S_String * s_field_458 = s_record_116_get_error_take(s_record_116_copy(v_nested));
		s_string_release(v_result.error);
		v_result.error = s_field_458;
		v_result.error_line = s_record_116_get_error_line_take(s_record_116_copy(v_nested));
		s_record_116 s_return_459 = s_record_116_copy(v_result);
		s_record_116_drop(v_nested);
		s_record_114_drop(v_function);
		s_record_116_drop(v_result);
		s_array_i64_release(v_args);
		s_string_release(v_name);
		s_record_116_drop(v_state);
		return s_return_459;
	}
	s_record_106 s_field_460 = s_record_116_get_types_take(s_record_116_copy(v_nested));
	s_record_106_drop(v_result.types);
	v_result.types = s_field_460;
	v_result.last_type = s_record_116_get_inferred_return_take(s_record_116_copy(v_nested));
	s_record_116 s_return_461 = s_record_116_copy(v_result);
	s_record_116_drop(v_nested);
	s_record_114_drop(v_function);
	s_record_116_drop(v_result);
	s_array_i64_release(v_args);
	s_string_release(v_name);
	s_record_116_drop(v_state);
	return s_return_461;
	s_record_116_drop(v_nested);
	s_record_114_drop(v_function);
	s_record_116_drop(v_result);
}

s_record_116 s_hir_body_expression(s_record_116 v_state, int64_t v_id)
{
	if (!((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
		s_record_116 s_return_462 = s_record_116_copy(v_state);
		s_record_116_drop(v_state);
		return s_return_462;
	}
	s_record_101 v_node = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_id);
	const int64_t v_kind = s_record_101_get_kind_take(s_record_101_copy(v_node));
	s_record_116 v_result = s_record_116_copy(v_state);
	v_result.last_type = 0;
	const int64_t v_literal = s_resolve_hir_literal(v_kind);
	if ((v_literal != 0))
	{
		v_result.last_type = v_literal;
		S_Array * s_field_463 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 4, v_literal, (-1)));
		s_array_release(v_result.nodes);
		v_result.nodes = s_field_463;
		s_record_116 s_return_464 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_464;
	}
	if ((v_kind == 29))
	{
		S_String * v_name = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
		s_record_109 v_resolved = s_resolve_hir_name(s_record_116_get_symbols_take(s_record_116_copy(v_result)), s_string_retain(v_name));
		if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) == 0))
		{
			v_result.last_type = s_record_109_get_type_id_take(s_record_109_copy(v_resolved));
			S_Array * s_field_465 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 3, s_record_109_get_type_id_take(s_record_109_copy(v_resolved)), s_record_109_get_field_index_take(s_record_109_copy(v_resolved))));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_465;
			s_record_116 s_return_466 = s_record_116_copy(v_result);
			s_record_109_drop(v_resolved);
			s_string_release(v_name);
			s_record_116_drop(v_result);
			s_record_101_drop(v_node);
			s_record_116_drop(v_state);
			return s_return_466;
		}
		{
			S_Array * v_s_iter_467 = s_record_116_get_constants_take(s_record_116_copy(v_result));
			for (int64_t s_index_467 = 0; s_index_467 < (int64_t)s_array_length(v_s_iter_467); ++s_index_467)
			{
				S_String * v_constant = s_ga_8_get_borrow(v_s_iter_467, s_index_467);
				if ((s_string_compare_take(s_string_retain(v_constant), s_string_retain(v_name)) == 0))
				{
					v_result.last_type = 1;
					S_Array * s_field_468 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 3, 1, (-1)));
					s_array_release(v_result.nodes);
					v_result.nodes = s_field_468;
					s_record_116 s_return_469 = s_record_116_copy(v_result);
					s_string_release(v_constant);
					s_array_release(v_s_iter_467);
					s_record_109_drop(v_resolved);
					s_string_release(v_name);
					s_record_116_drop(v_result);
					s_record_101_drop(v_node);
					s_record_116_drop(v_state);
					return s_return_469;
				}
				s_string_release(v_constant);
			}
			s_array_release(v_s_iter_467);
		}
		{
			S_Array * v_s_iter_470 = s_record_116_get_names_take(s_record_116_copy(v_result));
			for (int64_t s_index_470 = 0; s_index_470 < (int64_t)s_array_length(v_s_iter_470); ++s_index_470)
			{
				S_String * v_function = s_ga_8_get_borrow(v_s_iter_470, s_index_470);
				if ((s_string_compare_take(s_string_retain(v_function), s_string_retain(v_name)) == 0))
				{
					s_record_116 s_return_471 = s_record_116_copy(v_result);
					s_string_release(v_function);
					s_array_release(v_s_iter_470);
					s_record_109_drop(v_resolved);
					s_string_release(v_name);
					s_record_116_drop(v_result);
					s_record_101_drop(v_node);
					s_record_116_drop(v_state);
					return s_return_471;
				}
				s_string_release(v_function);
			}
			s_array_release(v_s_iter_470);
		}
		s_record_116 s_return_472 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown name '", sizeof("unknown name '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
		s_record_109_drop(v_resolved);
		s_string_release(v_name);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_472;
		s_record_109_drop(v_resolved);
		s_string_release(v_name);
	}
	if ((v_kind == 10))
	{
		const int64_t v_callee = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		const int64_t v_callee_kind = s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_callee));
		int64_t v_receiver = 0;
		S_String * v_method = s_string_from_bytes("", sizeof("") - 1);
		int64_t v_resolved_function = (-1);
		if ((v_callee_kind == 4))
		{
			S_String *s_value_473 = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee);
			s_string_release(v_method);
			v_method = s_value_473;
			s_record_116 s_value_474 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee, 0));
			s_record_116_drop(v_result);
			v_result = s_value_474;
			v_receiver = s_record_116_get_last_type_take(s_record_116_copy(v_result));
		}
		else if (((v_callee_kind != 29) && (v_callee_kind != 23)))
		{
			s_record_116 s_value_475 = s_hir_body_expression(s_record_116_copy(v_result), v_callee);
			s_record_116_drop(v_result);
			v_result = s_value_475;
		}
		S_Array_i64 * v_args = s_array_i64_make(0, NULL);
		bool v_complete = true;
		for (int64_t v_i = 1, s_bound_476 = s_record_101_get_count_take(s_record_101_copy(v_node)); v_i < s_bound_476; ++v_i)
		{
			s_record_116 s_value_477 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
			s_record_116_drop(v_result);
			v_result = s_value_477;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_478 = s_record_116_copy(v_result);
				s_array_i64_release(v_args);
				s_string_release(v_method);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_478;
			}
			if ((s_record_116_get_last_type_take(s_record_116_copy(v_result)) == 0))
			{
				v_complete = false;
			}
			int64_t s_item_479 = s_record_116_get_last_type_take(s_record_116_copy(v_result));
			v_args = s_array_i64_append_take(v_args, s_item_479);
		}
		v_result.last_type = 0;
		if (((v_callee_kind == 23) && (s_record_101_get_count_take(s_record_101_copy(v_node)) == 1)))
		{
			s_record_111 v_typed = s_hir_body_type_reference(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_tree_take(s_record_116_copy(v_result)), v_callee);
			if ((s_record_111_get_type_id_take(s_record_111_copy(v_typed)) != 0))
			{
				s_record_106 s_field_480 = s_record_111_get_table_take(s_record_111_copy(v_typed));
				s_record_106_drop(v_result.types);
				v_result.types = s_field_480;
				v_result.last_type = s_record_111_get_type_id_take(s_record_111_copy(v_typed));
			}
			s_record_111_drop(v_typed);
		}
		else if ((v_complete && (v_callee_kind == 29)))
		{
			S_String * v_name = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_callee);
			if (s_hir_is_builtin(s_string_retain(v_name)))
			{
				s_record_111 v_checked = s_resolve_hir_builtin_call(s_record_116_get_types_take(s_record_116_copy(v_result)), s_string_retain(v_name), s_array_i64_retain(v_args));
				if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_checked)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_116 s_return_481 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_record_111_get_error_take(s_record_111_copy(v_checked)));
					s_record_111_drop(v_checked);
					s_string_release(v_name);
					s_array_i64_release(v_args);
					s_string_release(v_method);
					s_record_116_drop(v_result);
					s_record_101_drop(v_node);
					s_record_116_drop(v_state);
					return s_return_481;
				}
				s_record_106 s_field_482 = s_record_111_get_table_take(s_record_111_copy(v_checked));
				s_record_106_drop(v_result.types);
				v_result.types = s_field_482;
				v_result.last_type = s_record_111_get_type_id_take(s_record_111_copy(v_checked));
				s_record_111_drop(v_checked);
			}
			else
			{
				for (int64_t v_j = 0, s_bound_483 = s_generic_array_length_take(s_record_116_get_functions_take(s_record_116_copy(v_result))); v_j < s_bound_483; ++v_j)
				{
					if ((s_string_compare_take(s_record_114_get_name_take(s_ga_10114_get_checked_take(s_record_116_get_functions_take(s_record_116_copy(v_result)), v_j)), s_string_retain(v_name)) == 0))
					{
						v_resolved_function = v_j;
					}
				}
				s_record_116 s_value_484 = s_hir_body_user_call(s_record_116_copy(v_result), v_id, s_string_retain(v_name), s_array_i64_retain(v_args));
				s_record_116_drop(v_result);
				v_result = s_value_484;
			}
			s_string_release(v_name);
		}
		else if ((v_complete && (v_receiver != 0)))
		{
			const int64_t v_tag = s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_result))), v_receiver));
			s_record_111 v_checked = s_HirCallCheck(s_record_116_get_types_take(s_record_116_copy(v_result)), 0, s_string_from_bytes("", sizeof("") - 1));
			if (((v_tag == 4) && s_hir_is_string_method(s_string_retain(v_method))))
			{
				s_record_111 s_value_485 = s_resolve_hir_string_call(s_record_116_get_types_take(s_record_116_copy(v_result)), v_receiver, s_string_retain(v_method), s_array_i64_retain(v_args));
				s_record_111_drop(v_checked);
				v_checked = s_value_485;
			}
			else if ((((v_tag == 7) && (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("append", sizeof("append") - 1)) == 0)) || ((v_tag == 8) && ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("get", sizeof("get") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("insert", sizeof("insert") - 1)) == 0)))))
			{
				s_record_111 s_value_486 = s_resolve_hir_collection_call(s_record_116_get_types_take(s_record_116_copy(v_result)), v_receiver, s_string_retain(v_method), s_array_i64_retain(v_args));
				s_record_111_drop(v_checked);
				v_checked = s_value_486;
			}
			if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_checked)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_487 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_record_111_get_error_take(s_record_111_copy(v_checked)));
				s_record_111_drop(v_checked);
				s_array_i64_release(v_args);
				s_string_release(v_method);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_487;
			}
			s_record_106 s_field_488 = s_record_111_get_table_take(s_record_111_copy(v_checked));
			s_record_106_drop(v_result.types);
			v_result.types = s_field_488;
			v_result.last_type = s_record_111_get_type_id_take(s_record_111_copy(v_checked));
			s_record_111_drop(v_checked);
		}
		if ((s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 0))
		{
			S_Array * s_field_489 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 6, s_record_116_get_last_type_take(s_record_116_copy(v_result)), v_resolved_function));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_489;
		}
		s_record_116 s_return_490 = s_record_116_copy(v_result);
		s_array_i64_release(v_args);
		s_string_release(v_method);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_490;
		s_array_i64_release(v_args);
		s_string_release(v_method);
	}
	if ((v_kind == 39))
	{
		s_record_116 s_value_491 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_116_drop(v_result);
		v_result = s_value_491;
		if ((s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 0))
		{
			s_record_109 v_checked = s_resolve_hir_unary(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_last_type_take(s_record_116_copy(v_result)), s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id));
			if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_checked)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_492 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_record_109_get_error_take(s_record_109_copy(v_checked)));
				s_record_109_drop(v_checked);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_492;
			}
			v_result.last_type = s_record_109_get_type_id_take(s_record_109_copy(v_checked));
			S_Array * s_field_493 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 5, s_record_109_get_type_id_take(s_record_109_copy(v_checked)), (-1)));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_493;
			s_record_109_drop(v_checked);
		}
		s_record_116 s_return_494 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_494;
	}
	if ((v_kind == 5))
	{
		s_record_116 v_lhs = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		const int64_t v_left = s_record_116_get_last_type_take(s_record_116_copy(v_lhs));
		s_record_116 s_value_495 = s_hir_body_expression(s_record_116_copy(v_lhs), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
		s_record_116_drop(v_result);
		v_result = s_value_495;
		const int64_t v_right = s_record_116_get_last_type_take(s_record_116_copy(v_result));
		S_String * v_op = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id);
		if (((((v_left != 0) && (v_right != 0)) && (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("or", sizeof("or") - 1)) == 0)) && (s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_result))), v_left)) == 9)))
		{
			const int64_t v_value_type = s_record_105_get_first_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_result))), v_left));
			if ((v_right != v_value_type))
			{
				S_String * s_left_496 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), v_value_type)), s_string_from_bytes(", found ", sizeof(", found ") - 1));
				S_String * s_right_497 = s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), v_right);
				s_record_116 s_return_498 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_left_496, s_right_497));
				s_string_release(v_op);
				s_record_116_drop(v_lhs);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_498;
			}
			v_result.last_type = v_value_type;
			S_Array * s_field_499 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 2, v_value_type, (-1)));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_499;
		}
		else if ((((v_left != 0) && (v_right != 0)) && (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("..", sizeof("..") - 1)) != 0)))
		{
			s_record_109 v_checked = s_resolve_hir_binary(s_record_116_get_types_take(s_record_116_copy(v_result)), v_left, v_right, s_string_retain(v_op));
			if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_checked)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_500 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_record_109_get_error_take(s_record_109_copy(v_checked)));
				s_record_109_drop(v_checked);
				s_string_release(v_op);
				s_record_116_drop(v_lhs);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_500;
			}
			v_result.last_type = s_record_109_get_type_id_take(s_record_109_copy(v_checked));
			S_Array * s_field_501 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 2, s_record_109_get_type_id_take(s_record_109_copy(v_checked)), (-1)));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_501;
			s_record_109_drop(v_checked);
		}
		else
		{
			v_result.last_type = 0;
		}
		s_record_116 s_return_502 = s_record_116_copy(v_result);
		s_string_release(v_op);
		s_record_116_drop(v_lhs);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_502;
		s_string_release(v_op);
		s_record_116_drop(v_lhs);
	}
	if ((v_kind == 4))
	{
		s_record_116 s_value_503 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_116_drop(v_result);
		v_result = s_value_503;
		const int64_t v_owner = s_record_116_get_last_type_take(s_record_116_copy(v_result));
		if (((v_owner != 0) && (s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_result))), v_owner)) == 6)))
		{
			s_record_109 v_field = s_resolve_hir_field(s_record_116_get_types_take(s_record_116_copy(v_result)), v_owner, s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id));
			if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_field)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_504 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_record_109_get_error_take(s_record_109_copy(v_field)));
				s_record_109_drop(v_field);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_504;
			}
			v_result.last_type = s_record_109_get_type_id_take(s_record_109_copy(v_field));
			S_Array * s_field_505 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 1, s_record_109_get_type_id_take(s_record_109_copy(v_field)), s_record_109_get_field_index_take(s_record_109_copy(v_field))));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_505;
			s_record_109_drop(v_field);
		}
		else
		{
			v_result.last_type = 0;
		}
		s_record_116 s_return_506 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_506;
	}
	if ((v_kind == 25))
	{
		s_record_116 s_return_507 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_507;
	}
	if ((v_kind == 33))
	{
		S_Array * v_fields = s_ga_10104_make_take(0, NULL);
		bool v_complete = true;
		for (int64_t v_i = 0, s_bound_508 = s_record_101_get_count_take(s_record_101_copy(v_node)); v_i < s_bound_508; ++v_i)
		{
			const int64_t v_field_id = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i);
			s_record_101 v_field = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_field_id);
			S_String * v_name = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_field_id);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 19))
			{
				s_record_116 s_value_509 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_field_id, 0));
				s_record_116_drop(v_result);
				v_result = s_value_509;
				if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_116 s_return_510 = s_record_116_copy(v_result);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_array_release(v_fields);
					s_record_116_drop(v_result);
					s_record_101_drop(v_node);
					s_record_116_drop(v_state);
					return s_return_510;
				}
				if ((s_record_116_get_last_type_take(s_record_116_copy(v_result)) == 0))
				{
					v_complete = false;
				}
				else if (v_complete)
				{
					s_record_112 v_edit = s_hir_record_put(s_array_retain(v_fields), s_string_retain(v_name), s_record_116_get_last_type_take(s_record_116_copy(v_result)));
					if ((s_string_compare_take(s_record_112_get_error_take(s_record_112_copy(v_edit)), s_string_from_bytes("", sizeof("") - 1)) != 0))
					{
						s_record_116 s_return_511 = s_hir_body_error(s_record_116_copy(v_result), v_field_id, s_record_112_get_error_take(s_record_112_copy(v_edit)));
						s_record_112_drop(v_edit);
						s_string_release(v_name);
						s_record_101_drop(v_field);
						s_array_release(v_fields);
						s_record_116_drop(v_result);
						s_record_101_drop(v_node);
						s_record_116_drop(v_state);
						return s_return_511;
					}
					S_Array * s_value_512 = s_record_112_get_fields_take(s_record_112_copy(v_edit));
					s_array_release(v_fields);
					v_fields = s_value_512;
					s_record_112_drop(v_edit);
				}
			}
			else if ((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 36))
			{
				s_record_109 v_symbol = s_resolve_hir_name(s_record_116_get_symbols_take(s_record_116_copy(v_result)), s_string_retain(v_name));
				if ((s_record_109_get_type_id_take(s_record_109_copy(v_symbol)) == 0))
				{
					v_complete = false;
				}
				else if (v_complete)
				{
					s_record_112 v_edit = s_hir_record_spread(s_record_116_get_types_take(s_record_116_copy(v_result)), s_array_retain(v_fields), s_record_109_get_type_id_take(s_record_109_copy(v_symbol)));
					if ((s_string_compare_take(s_record_112_get_error_take(s_record_112_copy(v_edit)), s_string_from_bytes("", sizeof("") - 1)) != 0))
					{
						s_record_116 s_return_513 = s_hir_body_error(s_record_116_copy(v_result), v_field_id, s_record_112_get_error_take(s_record_112_copy(v_edit)));
						s_record_112_drop(v_edit);
						s_record_109_drop(v_symbol);
						s_string_release(v_name);
						s_record_101_drop(v_field);
						s_array_release(v_fields);
						s_record_116_drop(v_result);
						s_record_101_drop(v_node);
						s_record_116_drop(v_state);
						return s_return_513;
					}
					S_Array * s_value_514 = s_record_112_get_fields_take(s_record_112_copy(v_edit));
					s_array_release(v_fields);
					v_fields = s_value_514;
					s_record_112_drop(v_edit);
				}
				s_record_109_drop(v_symbol);
			}
			else if (((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 15) && v_complete))
			{
				s_record_112 v_edit = s_hir_record_delete(s_array_retain(v_fields), s_string_retain(v_name));
				if ((s_string_compare_take(s_record_112_get_error_take(s_record_112_copy(v_edit)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_116 s_return_515 = s_hir_body_error(s_record_116_copy(v_result), v_field_id, s_record_112_get_error_take(s_record_112_copy(v_edit)));
					s_record_112_drop(v_edit);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_array_release(v_fields);
					s_record_116_drop(v_result);
					s_record_101_drop(v_node);
					s_record_116_drop(v_state);
					return s_return_515;
				}
				S_Array * s_value_516 = s_record_112_get_fields_take(s_record_112_copy(v_edit));
				s_array_release(v_fields);
				v_fields = s_value_516;
				s_record_112_drop(v_edit);
			}
			s_string_release(v_name);
			s_record_101_drop(v_field);
		}
		v_result.last_type = 0;
		if (v_complete)
		{
			s_record_111 v_typed = s_resolve_hir_record(s_record_116_get_types_take(s_record_116_copy(v_result)), s_array_retain(v_fields));
			if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_typed)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_517 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_record_111_get_error_take(s_record_111_copy(v_typed)));
				s_record_111_drop(v_typed);
				s_array_release(v_fields);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_517;
			}
			s_record_106 s_field_518 = s_record_111_get_table_take(s_record_111_copy(v_typed));
			s_record_106_drop(v_result.types);
			v_result.types = s_field_518;
			v_result.last_type = s_record_111_get_type_id_take(s_record_111_copy(v_typed));
			S_Array * s_field_519 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 7, s_record_111_get_type_id_take(s_record_111_copy(v_typed)), (-1)));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_519;
			s_record_111_drop(v_typed);
		}
		s_record_116 s_return_520 = s_record_116_copy(v_result);
		s_array_release(v_fields);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_520;
		s_array_release(v_fields);
	}
	if ((v_kind == 2))
	{
		int64_t v_element_type = 0;
		bool v_complete = true;
		for (int64_t v_i = 0, s_bound_521 = s_record_101_get_count_take(s_record_101_copy(v_node)); v_i < s_bound_521; ++v_i)
		{
			s_record_116 s_value_522 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
			s_record_116_drop(v_result);
			v_result = s_value_522;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_523 = s_record_116_copy(v_result);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_523;
			}
			const int64_t v_current = s_record_116_get_last_type_take(s_record_116_copy(v_result));
			if ((v_current == 0))
			{
				v_complete = false;
			}
			else if ((v_element_type == 0))
			{
				v_element_type = v_current;
			}
			else if (((v_element_type != v_current) && v_complete))
			{
				S_String * s_left_524 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), v_element_type)), s_string_from_bytes(", found ", sizeof(", found ") - 1));
				S_String * s_right_525 = s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), v_current);
				s_record_116 s_return_526 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_left_524, s_right_525));
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_526;
			}
		}
		v_result.last_type = 0;
		if (v_complete)
		{
			if ((v_element_type == 0))
			{
				v_element_type = 1;
			}
			s_record_107 v_typed = s_intern_array_type(s_record_116_get_types_take(s_record_116_copy(v_result)), v_element_type);
			s_record_106 s_field_527 = s_record_107_get_table_take(s_record_107_copy(v_typed));
			s_record_106_drop(v_result.types);
			v_result.types = s_field_527;
			v_result.last_type = s_record_107_get_id_take(s_record_107_copy(v_typed));
			S_Array * s_field_528 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 11, s_record_107_get_id_take(s_record_107_copy(v_typed)), (-1)));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_528;
			s_record_107_drop(v_typed);
		}
		s_record_116 s_return_529 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_529;
	}
	if ((((v_kind == 23) || (v_kind == 30)) || (v_kind == 12)))
	{
		if (((v_kind == 23) && (s_record_101_get_count_take(s_record_101_copy(v_node)) > 0)))
		{
			const int64_t v_base = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
			if ((s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_base)) == 29))
			{
				S_String * v_type_name = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_base);
				if ((((s_string_compare_take(s_string_retain(v_type_name), s_string_from_bytes("Array", sizeof("Array") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_type_name), s_string_from_bytes("Dict", sizeof("Dict") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_type_name), s_string_from_bytes("Optional", sizeof("Optional") - 1)) == 0)))
				{
					s_record_116 s_return_530 = s_record_116_copy(v_result);
					s_string_release(v_type_name);
					s_record_116_drop(v_result);
					s_record_101_drop(v_node);
					s_record_116_drop(v_state);
					return s_return_530;
				}
				s_string_release(v_type_name);
			}
		}
		s_record_116 v_owner_state = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		const int64_t v_owner = s_record_116_get_last_type_take(s_record_116_copy(v_owner_state));
		s_record_116 s_value_531 = s_record_116_copy(v_owner_state);
		s_record_116_drop(v_result);
		v_result = s_value_531;
		for (int64_t v_i = 1, s_bound_532 = s_record_101_get_count_take(s_record_101_copy(v_node)); v_i < s_bound_532; ++v_i)
		{
			s_record_116 s_value_533 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
			s_record_116_drop(v_result);
			v_result = s_value_533;
			if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_116 s_return_534 = s_record_116_copy(v_result);
				s_record_116_drop(v_owner_state);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_534;
			}
			if (((s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 0) && (s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 1)))
			{
				s_record_116 s_return_535 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_string_from_bytes("expected i64, found ", sizeof("expected i64, found ") - 1), s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_last_type_take(s_record_116_copy(v_result)))));
				s_record_116_drop(v_owner_state);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_535;
			}
		}
		const bool v_valid_index = ((s_record_116_get_last_type_take(s_record_116_copy(v_result)) == 1) && (s_record_101_get_count_take(s_record_101_copy(v_node)) == 2));
		v_result.last_type = 0;
		if (((v_valid_index && (v_owner != 0)) && (s_record_105_get_tag_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_result))), v_owner)) == 7)))
		{
			const int64_t v_element = s_record_105_get_first_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_result))), v_owner));
			if ((v_kind == 30))
			{
				s_record_107 v_optional = s_intern_optional_type(s_record_116_get_types_take(s_record_116_copy(v_result)), v_element);
				s_record_106 s_field_536 = s_record_107_get_table_take(s_record_107_copy(v_optional));
				s_record_106_drop(v_result.types);
				v_result.types = s_field_536;
				v_result.last_type = s_record_107_get_id_take(s_record_107_copy(v_optional));
				s_record_107_drop(v_optional);
			}
			else
			{
				v_result.last_type = v_element;
			}
			S_Array * s_field_537 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 12, s_record_116_get_last_type_take(s_record_116_copy(v_result)), (-1)));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_537;
		}
		s_record_116 s_return_538 = s_record_116_copy(v_result);
		s_record_116_drop(v_owner_state);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_538;
		s_record_116_drop(v_owner_state);
	}
	if ((v_kind == 44))
	{
		s_record_116 s_value_539 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_116_drop(v_result);
		v_result = s_value_539;
		const int64_t v_wrapped = s_record_116_get_last_type_take(s_record_116_copy(v_result));
		if ((v_wrapped != 0))
		{
			s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_result))), v_wrapped);
			if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) != 9))
			{
				s_record_116 s_return_540 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_from_bytes("unboxing requires an Optional", sizeof("unboxing requires an Optional") - 1));
				s_record_105_drop(v_descriptor);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_540;
			}
			v_result.last_type = s_record_105_get_first_take(s_record_105_copy(v_descriptor));
			S_Array * s_field_541 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 13, s_record_105_get_first_take(s_record_105_copy(v_descriptor)), (-1)));
			s_array_release(v_result.nodes);
			v_result.nodes = s_field_541;
			s_record_105_drop(v_descriptor);
		}
		s_record_116 s_return_542 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_542;
	}
	if ((v_kind == 17))
	{
		for (int64_t v_i = 0, s_bound_543 = s_record_101_get_count_take(s_record_101_copy(v_node)); v_i < s_bound_543; ++v_i)
		{
			s_record_116 s_value_544 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
			s_record_116_drop(v_result);
			v_result = s_value_544;
		}
		s_record_116 s_return_545 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_545;
	}
	s_record_116 s_return_546 = s_record_116_copy(v_result);
	s_record_116_drop(v_result);
	s_record_101_drop(v_node);
	s_record_116_drop(v_state);
	return s_return_546;
	s_record_116_drop(v_result);
	s_record_101_drop(v_node);
}

s_record_116 s_hir_body_block(s_record_116 v_state, int64_t v_id)
{
	s_record_116 v_result = s_record_116_copy(v_state);
	const int64_t v_initial = s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_result)));
	v_result.depth += 1;
	s_record_101 v_block = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_result))), v_id);
	for (int64_t v_i = 0, s_bound_547 = s_record_101_get_count_take(s_record_101_copy(v_block)); v_i < s_bound_547; ++v_i)
	{
		s_record_116 s_value_548 = s_hir_body_statement(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_result)), v_id, v_i));
		s_record_116_drop(v_result);
		v_result = s_value_548;
		if (!((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
			s_record_116 s_return_549 = s_record_116_copy(v_result);
			s_record_101_drop(v_block);
			s_record_116_drop(v_result);
			s_record_116_drop(v_state);
			return s_return_549;
		}
	}
	S_Array * v_symbols = s_ga_10110_make_take(0, NULL);
	for (int64_t v_i = 0, s_bound_550 = v_initial; v_i < s_bound_550; ++v_i)
	{
		s_record_110 s_item_551 = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_result)), v_i);
		v_symbols = s_ga_10110_append_take(v_symbols, s_item_551);
	}
	S_Array * s_field_552 = s_array_retain(v_symbols);
	s_array_release(v_result.symbols);
	v_result.symbols = s_field_552;
	v_result.depth -= 1;
	s_record_116 s_return_553 = s_record_116_copy(v_result);
	s_array_release(v_symbols);
	s_record_101_drop(v_block);
	s_record_116_drop(v_result);
	s_record_116_drop(v_state);
	return s_return_553;
	s_array_release(v_symbols);
	s_record_101_drop(v_block);
	s_record_116_drop(v_result);
}

s_record_116 s_hir_body_statement(s_record_116 v_state, int64_t v_id)
{
	if (!((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
		s_record_116 s_return_554 = s_record_116_copy(v_state);
		s_record_116_drop(v_state);
		return s_return_554;
	}
	s_record_101 v_node = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_id);
	s_record_116 v_result = s_record_116_copy(v_state);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 26) || (s_record_101_get_kind_take(s_record_101_copy(v_node)) == 40)))
	{
		int64_t v_declared = 0;
		int64_t v_initializer = (-1);
		int64_t v_fallback = (-1);
		for (int64_t v_i = 0, s_bound_555 = s_record_101_get_count_take(s_record_101_copy(v_node)); v_i < s_bound_555; ++v_i)
		{
			const int64_t v_child = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i);
			const int64_t v_child_kind = s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_child));
			if ((v_child_kind == 38))
			{
				s_record_111 v_annotated = s_hir_body_type_reference(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_tree_take(s_record_116_copy(v_result)), v_child);
				s_record_106 s_field_556 = s_record_111_get_table_take(s_record_111_copy(v_annotated));
				s_record_106_drop(v_result.types);
				v_result.types = s_field_556;
				v_declared = s_record_111_get_type_id_take(s_record_111_copy(v_annotated));
				s_record_111_drop(v_annotated);
			}
			else if ((v_child_kind == 17))
			{
				v_fallback = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_child, 0);
			}
			else
			{
				v_initializer = v_child;
			}
		}
		if ((v_initializer >= 0))
		{
			s_record_116 s_value_557 = s_hir_body_expression(s_record_116_copy(v_result), v_initializer);
			s_record_116_drop(v_result);
			v_result = s_value_557;
		}
		if (!((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
			s_record_116 s_return_558 = s_record_116_copy(v_result);
			s_record_116_drop(v_result);
			s_record_101_drop(v_node);
			s_record_116_drop(v_state);
			return s_return_558;
		}
		if ((v_fallback >= 0))
		{
			const int64_t v_wrapped = s_record_116_get_last_type_take(s_record_116_copy(v_result));
			s_record_116 s_value_559 = s_hir_body_expression(s_record_116_copy(v_result), v_fallback);
			s_record_116_drop(v_result);
			v_result = s_value_559;
			if (!((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
				s_record_116 s_return_560 = s_record_116_copy(v_result);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_560;
			}
			v_result.last_type = 0;
			if ((v_wrapped != 0))
			{
				s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_116_get_types_take(s_record_116_copy(v_result))), v_wrapped);
				if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) != 9))
				{
					s_record_116 s_return_561 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_from_bytes("else return requires an Optional", sizeof("else return requires an Optional") - 1));
					s_record_105_drop(v_descriptor);
					s_record_116_drop(v_result);
					s_record_101_drop(v_node);
					s_record_116_drop(v_state);
					return s_return_561;
				}
				v_result.last_type = s_record_105_get_first_take(s_record_105_copy(v_descriptor));
				s_record_105_drop(v_descriptor);
			}
		}
		if ((((v_declared != 0) && (s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 0)) && (v_declared != s_record_116_get_last_type_take(s_record_116_copy(v_result)))))
		{
			S_String * s_left_562 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), v_declared)), s_string_from_bytes(", found ", sizeof(", found ") - 1));
			S_String * s_right_563 = s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_last_type_take(s_record_116_copy(v_result)));
			s_record_116 s_return_564 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_left_562, s_right_563));
			s_record_116_drop(v_result);
			s_record_101_drop(v_node);
			s_record_116_drop(v_state);
			return s_return_564;
		}
		if ((v_declared == 0))
		{
			v_declared = s_record_116_get_last_type_take(s_record_116_copy(v_result));
		}
		s_record_116 s_return_565 = s_hir_body_binding(s_record_116_copy(v_result), v_id, s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), v_declared, (s_record_101_get_kind_take(s_record_101_copy(v_node)) == 40));
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_565;
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 3))
	{
		const int64_t v_lhs = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0);
		const int64_t v_rhs = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1);
		s_record_101 v_target = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_lhs);
		int64_t v_root = v_lhs;
		while (true) {
			if (!(((s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_root)) == 4) || (s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_root)) == 12)))) break;
			v_root = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root, 0);
		}
		if ((s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_root)) == 29))
		{
			S_String * v_name = s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_root);
			s_record_109 v_resolved = s_resolve_hir_name(s_record_116_get_symbols_take(s_record_116_copy(v_result)), s_string_retain(v_name));
			if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) == 0))
			{
				if ((!s_record_110_get_mutable_take(s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_result)), s_record_109_get_field_index_take(s_record_109_copy(v_resolved))))))
				{
					if ((s_record_101_get_kind_take(s_record_101_copy(v_target)) == 12))
					{
						s_record_116 s_return_566 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
						s_record_109_drop(v_resolved);
						s_string_release(v_name);
						s_record_101_drop(v_target);
						s_record_116_drop(v_result);
						s_record_101_drop(v_node);
						s_record_116_drop(v_state);
						return s_return_566;
					}
					s_record_116 s_return_567 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("cannot assign immutable binding '", sizeof("cannot assign immutable binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
					s_record_109_drop(v_resolved);
					s_string_release(v_name);
					s_record_101_drop(v_target);
					s_record_116_drop(v_result);
					s_record_101_drop(v_node);
					s_record_116_drop(v_state);
					return s_return_567;
				}
			}
			s_record_109_drop(v_resolved);
			s_string_release(v_name);
		}
		s_record_116 s_value_568 = s_hir_body_expression(s_record_116_copy(v_result), v_rhs);
		s_record_116_drop(v_result);
		v_result = s_value_568;
		if ((s_record_101_get_kind_take(s_record_101_copy(v_target)) == 29))
		{
			s_record_109 v_resolved = s_resolve_hir_name(s_record_116_get_symbols_take(s_record_116_copy(v_result)), s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_lhs));
			if (((((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 0)) && (s_record_109_get_type_id_take(s_record_109_copy(v_resolved)) != 0)) && (s_record_109_get_type_id_take(s_record_109_copy(v_resolved)) != s_record_116_get_last_type_take(s_record_116_copy(v_result)))))
			{
				S_String * s_left_569 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_109_get_type_id_take(s_record_109_copy(v_resolved)))), s_string_from_bytes(", found ", sizeof(", found ") - 1));
				S_String * s_right_570 = s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_last_type_take(s_record_116_copy(v_result)));
				s_record_116 s_return_571 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_left_569, s_right_570));
				s_record_109_drop(v_resolved);
				s_record_101_drop(v_target);
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_571;
			}
			s_record_109_drop(v_resolved);
		}
		S_Array * s_field_572 = s_ga_10108_append_take(s_record_116_get_nodes_take(s_record_116_copy(v_result)), s_HirNode(v_id, 10, s_record_116_get_last_type_take(s_record_116_copy(v_result)), (-1)));
		s_array_release(v_result.nodes);
		v_result.nodes = s_field_572;
		s_record_116 s_return_573 = s_record_116_copy(v_result);
		s_record_101_drop(v_target);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_573;
		s_record_101_drop(v_target);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 22))
	{
		for (int64_t v_i = 0, s_bound_574 = s_record_101_get_count_take(s_record_101_copy(v_node)); v_i < s_bound_574; ++v_i)
		{
			const int64_t v_arm = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i);
			s_record_101 v_branch = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_state))), v_arm);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_branch)) == 16))
			{
				s_record_116 s_value_575 = s_hir_body_block(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_arm, 0));
				s_record_116_drop(v_result);
				v_result = s_value_575;
			}
			else
			{
				s_record_116 s_value_576 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_arm, 0));
				s_record_116_drop(v_result);
				v_result = s_value_576;
				s_record_116 s_value_577 = s_hir_body_block(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_arm, 1));
				s_record_116_drop(v_result);
				v_result = s_value_577;
			}
			s_record_101_drop(v_branch);
		}
		s_record_116 s_return_578 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_578;
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 41))
	{
		s_record_116 s_value_579 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_116_drop(v_result);
		v_result = s_value_579;
		s_record_116 s_return_580 = s_hir_body_block(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1));
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_580;
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 21))
	{
		const int64_t v_iterable = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_node)) - 2));
		s_record_116 s_value_581 = s_hir_body_expression(s_record_116_copy(v_result), v_iterable);
		s_record_116_drop(v_result);
		v_result = s_value_581;
		const int64_t v_initial = s_generic_array_length_take(s_record_116_get_symbols_take(s_record_116_copy(v_result)));
		v_result.depth += 1;
		int64_t v_item_type = 0;
		if (((s_record_101_get_kind_take(s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_116_get_tree_take(s_record_116_copy(v_result))), v_iterable)) == 5) && (s_string_compare_take(s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_result)), v_iterable), s_string_from_bytes("..", sizeof("..") - 1)) == 0)))
		{
			v_item_type = 1;
		}
		s_record_116 s_value_582 = s_hir_body_binding(s_record_116_copy(v_result), v_id, s_hir_ast_text(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id), v_item_type, false);
		s_record_116_drop(v_result);
		v_result = s_value_582;
		s_record_116 s_value_583 = s_hir_body_block(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_node)) - 1)));
		s_record_116_drop(v_result);
		v_result = s_value_583;
		S_Array * v_symbols = s_ga_10110_make_take(0, NULL);
		for (int64_t v_i = 0, s_bound_584 = v_initial; v_i < s_bound_584; ++v_i)
		{
			s_record_110 s_item_585 = s_ga_10110_get_checked_take(s_record_116_get_symbols_take(s_record_116_copy(v_result)), v_i);
			v_symbols = s_ga_10110_append_take(v_symbols, s_item_585);
		}
		S_Array * s_field_586 = s_array_retain(v_symbols);
		s_array_release(v_result.symbols);
		v_result.symbols = s_field_586;
		v_result.depth -= 1;
		s_record_116 s_return_587 = s_record_116_copy(v_result);
		s_array_release(v_symbols);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_587;
		s_array_release(v_symbols);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 43))
	{
		s_record_116 s_value_588 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 0));
		s_record_116_drop(v_result);
		v_result = s_value_588;
		if (((s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 0) && (s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 3)))
		{
			s_record_116 s_return_589 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_concat_take(s_string_from_bytes("expected bool, found ", sizeof("expected bool, found ") - 1), s_hir_type_name(s_record_116_get_types_take(s_record_116_copy(v_result)), s_record_116_get_last_type_take(s_record_116_copy(v_result)))));
			s_record_116_drop(v_result);
			s_record_101_drop(v_node);
			s_record_116_drop(v_state);
			return s_return_589;
		}
		const int64_t v_exit_node = s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, 1);
		s_record_116 s_return_590 = s_hir_body_statement(s_record_116_copy(v_result), v_exit_node);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_590;
	}
	if ((((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 35) || (s_record_101_get_kind_take(s_record_101_copy(v_node)) == 18)) || (s_record_101_get_kind_take(s_record_101_copy(v_node)) == 34)))
	{
		for (int64_t v_i = 0, s_bound_591 = s_record_101_get_count_take(s_record_101_copy(v_node)); v_i < s_bound_591; ++v_i)
		{
			s_record_116 s_value_592 = s_hir_body_expression(s_record_116_copy(v_result), s_hir_ast_child(s_record_116_get_tree_take(s_record_116_copy(v_state)), v_id, v_i));
			s_record_116_drop(v_result);
			v_result = s_value_592;
		}
		if (((s_record_101_get_kind_take(s_record_101_copy(v_node)) == 35) && (s_record_116_get_last_type_take(s_record_116_copy(v_result)) != 0)))
		{
			if (((s_record_116_get_inferred_return_take(s_record_116_copy(v_result)) != 0) && (s_record_116_get_inferred_return_take(s_record_116_copy(v_result)) != s_record_116_get_last_type_take(s_record_116_copy(v_result)))))
			{
				s_record_116 s_return_593 = s_hir_body_error(s_record_116_copy(v_result), v_id, s_string_from_bytes("inconsistent return types", sizeof("inconsistent return types") - 1));
				s_record_116_drop(v_result);
				s_record_101_drop(v_node);
				s_record_116_drop(v_state);
				return s_return_593;
			}
			v_result.inferred_return = s_record_116_get_last_type_take(s_record_116_copy(v_result));
		}
		s_record_116 s_return_594 = s_record_116_copy(v_result);
		s_record_116_drop(v_result);
		s_record_101_drop(v_node);
		s_record_116_drop(v_state);
		return s_return_594;
	}
	s_record_116 s_return_595 = s_record_116_copy(v_result);
	s_record_116_drop(v_result);
	s_record_101_drop(v_node);
	s_record_116_drop(v_state);
	return s_return_595;
	s_record_116_drop(v_result);
	s_record_101_drop(v_node);
}

s_record_117 s_hir_analyze_function_body(s_record_102 v_tree, s_record_114 v_header, S_Array * v_names, S_Array * v_constants)
{
	s_record_116 v_state = s_HirBodyState(s_record_102_copy(v_tree), s_array_retain(v_names), s_array_retain(v_constants));
	{
		S_Array * v_s_iter_596 = s_record_114_get_parameters_take(s_record_114_copy(v_header));
		for (int64_t s_index_596 = 0; s_index_596 < (int64_t)s_array_length(v_s_iter_596); ++s_index_596)
		{
			s_record_113 v_parameter = s_ga_10113_get_borrow(v_s_iter_596, s_index_596);
			int64_t v_type_id = 0;
			if ((s_record_113_get_type_node_take(s_record_113_copy(v_parameter)) >= 0))
			{
				s_record_111 v_annotated = s_hir_body_type_reference(s_record_116_get_types_take(s_record_116_copy(v_state)), s_record_102_copy(v_tree), s_record_113_get_type_node_take(s_record_113_copy(v_parameter)));
				s_record_106 s_field_597 = s_record_111_get_table_take(s_record_111_copy(v_annotated));
				s_record_106_drop(v_state.types);
				v_state.types = s_field_597;
				v_type_id = s_record_111_get_type_id_take(s_record_111_copy(v_annotated));
				s_record_111_drop(v_annotated);
			}
			s_record_116 s_value_598 = s_hir_body_binding(s_record_116_copy(v_state), s_record_113_get_source_take(s_record_113_copy(v_parameter)), s_record_113_get_name_take(s_record_113_copy(v_parameter)), v_type_id, false);
			s_record_116_drop(v_state);
			v_state = s_value_598;
			s_record_113_drop(v_parameter);
		}
		s_array_release(v_s_iter_596);
	}
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		s_record_116 s_value_599 = s_hir_body_block(s_record_116_copy(v_state), s_record_114_get_body_take(s_record_114_copy(v_header)));
		s_record_116_drop(v_state);
		v_state = s_value_599;
	}
	s_record_117 s_return_600 = s_HirBodyAnalysis(s_record_116_get_nodes_take(s_record_116_copy(v_state)), s_record_116_get_error_take(s_record_116_copy(v_state)), s_record_116_get_error_line_take(s_record_116_copy(v_state)));
	s_record_116_drop(v_state);
	s_array_release(v_constants);
	s_array_release(v_names);
	s_record_114_drop(v_header);
	s_record_102_drop(v_tree);
	return s_return_600;
	s_record_116_drop(v_state);
}

s_record_117 s_hir_analyze_module_body(s_record_102 v_tree, s_record_114 v_header, S_Array * v_functions, S_Array * v_constants)
{
	S_Array * v_names = s_ga_8_make_take(0, NULL);
	{
		S_Array * v_s_iter_601 = s_array_retain(v_functions);
		for (int64_t s_index_601 = 0; s_index_601 < (int64_t)s_array_length(v_s_iter_601); ++s_index_601)
		{
			s_record_114 v_function = s_ga_10114_get_borrow(v_s_iter_601, s_index_601);
			S_String * s_item_602 = s_record_114_get_name_take(s_record_114_copy(v_function));
			v_names = s_ga_8_append_take(v_names, s_item_602);
			s_record_114_drop(v_function);
		}
		s_array_release(v_s_iter_601);
	}
	s_record_116 v_state = s_HirBodyState(s_record_102_copy(v_tree), s_array_retain(v_names), s_array_retain(v_constants));
	S_Array * s_field_603 = s_array_retain(v_functions);
	s_array_release(v_state.functions);
	v_state.functions = s_field_603;
	S_Array * s_field_604 = s_ga_8_make_take(1, (S_String *[]){s_record_114_get_name_take(s_record_114_copy(v_header))});
	s_array_release(v_state.active_functions);
	v_state.active_functions = s_field_604;
	{
		S_Array * v_s_iter_605 = s_record_114_get_parameters_take(s_record_114_copy(v_header));
		for (int64_t s_index_605 = 0; s_index_605 < (int64_t)s_array_length(v_s_iter_605); ++s_index_605)
		{
			s_record_113 v_parameter = s_ga_10113_get_borrow(v_s_iter_605, s_index_605);
			int64_t v_type_id = 0;
			if ((s_record_113_get_type_node_take(s_record_113_copy(v_parameter)) >= 0))
			{
				s_record_111 v_annotated = s_hir_body_type_reference(s_record_116_get_types_take(s_record_116_copy(v_state)), s_record_102_copy(v_tree), s_record_113_get_type_node_take(s_record_113_copy(v_parameter)));
				s_record_106 s_field_606 = s_record_111_get_table_take(s_record_111_copy(v_annotated));
				s_record_106_drop(v_state.types);
				v_state.types = s_field_606;
				v_type_id = s_record_111_get_type_id_take(s_record_111_copy(v_annotated));
				s_record_111_drop(v_annotated);
			}
			s_record_116 s_value_607 = s_hir_body_binding(s_record_116_copy(v_state), s_record_113_get_source_take(s_record_113_copy(v_parameter)), s_record_113_get_name_take(s_record_113_copy(v_parameter)), v_type_id, false);
			s_record_116_drop(v_state);
			v_state = s_value_607;
			s_record_113_drop(v_parameter);
		}
		s_array_release(v_s_iter_605);
	}
	if ((s_string_compare_take(s_record_116_get_error_take(s_record_116_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		s_record_116 s_value_608 = s_hir_body_block(s_record_116_copy(v_state), s_record_114_get_body_take(s_record_114_copy(v_header)));
		s_record_116_drop(v_state);
		v_state = s_value_608;
	}
	s_record_117 s_return_609 = s_HirBodyAnalysis(s_record_116_get_nodes_take(s_record_116_copy(v_state)), s_record_116_get_error_take(s_record_116_copy(v_state)), s_record_116_get_error_line_take(s_record_116_copy(v_state)));
	s_record_116_drop(v_state);
	s_array_release(v_names);
	s_array_release(v_constants);
	s_array_release(v_functions);
	s_record_114_drop(v_header);
	s_record_102_drop(v_tree);
	return s_return_609;
	s_record_116_drop(v_state);
	s_array_release(v_names);
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

s_record_118 s_FunctionInfo(S_String * v_name, int64_t v_kind, S_Array * v_params, int64_t v_body, bool v_constructor, S_String * v_symbol)
{
	s_record_118 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.kind = v_kind;
	s_constructor_result.params = v_params;
	s_constructor_result.body = v_body;
	s_constructor_result.constructor = v_constructor;
	s_constructor_result.symbol = v_symbol;
	return s_constructor_result;
}

s_record_119 s_InstanceInfo(S_String * v_name, S_Array_i64 * v_kinds, S_Array_i64 * v_type_ids, int64_t v_kind, S_String * v_symbol, S_String * v_code, int64_t v_status)
{
	s_record_119 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.kinds = v_kinds;
	s_constructor_result.type_ids = v_type_ids;
	s_constructor_result.kind = v_kind;
	s_constructor_result.symbol = v_symbol;
	s_constructor_result.code = v_code;
	s_constructor_result.status = v_status;
	return s_constructor_result;
}

s_record_120 s_ConstantInfo(S_String * v_name, S_String * v_value)
{
	s_record_120 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.value = v_value;
	return s_constructor_result;
}

s_record_121 s_RecordInfo(S_Array * v_fields, int64_t v_canonical)
{
	s_record_121 s_constructor_result = {0};
	s_constructor_result.fields = v_fields;
	s_constructor_result.canonical = v_canonical;
	return s_constructor_result;
}

s_record_122 s_EmitState(s_record_102 v_tree)
{
	s_record_122 s_constructor_result = {0};
	s_constructor_result.tree = v_tree;
	s_constructor_result.functions = s_ga_10118_make_take(0, NULL);
	s_constructor_result.instances = s_ga_10119_make_take(0, NULL);
	s_constructor_result.active_params = s_ga_10104_make_take(0, NULL);
	s_constructor_result.symbols = s_ga_10110_make_take(0, NULL);
	s_constructor_result.constants = s_ga_10120_make_take(0, NULL);
	s_constructor_result.records = s_ga_10121_make_take(0, NULL);
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

s_record_123 s_ExprCode(s_record_122 v_state, S_String * v_text, int64_t v_kind)
{
	s_record_123 s_constructor_result = {0};
	s_constructor_result.state = v_state;
	s_constructor_result.text = v_text;
	s_constructor_result.kind = v_kind;
	return s_constructor_result;
}

s_record_124 s_CompileResult(S_String * v_code, S_String * v_error, int64_t v_line)
{
	s_record_124 s_constructor_result = {0};
	s_constructor_result.code = v_code;
	s_constructor_result.error = v_error;
	s_constructor_result.line = v_line;
	return s_constructor_result;
}

s_record_101 s_ast_node(s_record_102 v_tree, int64_t v_id)
{
	s_record_101 s_return_610 = s_ga_10101_get_checked_take(s_record_102_get_nodes_take(s_record_102_copy(v_tree)), v_id);
	s_record_102_drop(v_tree);
	return s_return_610;
}

int64_t s_ast_child(s_record_102 v_tree, int64_t v_id, int64_t v_offset)
{
	s_record_101 v_entry = s_ast_node(s_record_102_copy(v_tree), v_id);
	int64_t s_return_611 = s_array_i64_get_checked_take(s_record_102_get_edges_take(s_record_102_copy(v_tree)), (s_record_101_get_first_take(s_record_101_copy(v_entry)) + v_offset));
	s_record_101_drop(v_entry);
	s_record_102_drop(v_tree);
	return s_return_611;
	s_record_101_drop(v_entry);
}

S_String * s_ast_text(s_record_102 v_tree, int64_t v_id)
{
	S_String * s_return_612 = s_token_text(s_record_102_get_source_take(s_record_102_copy(v_tree)), s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_tree)), s_record_101_get_token_take(s_ast_node(s_record_102_copy(v_tree), v_id))));
	s_record_102_drop(v_tree);
	return s_return_612;
}

int64_t s_ast_line(s_record_102 v_tree, int64_t v_id)
{
	int64_t s_return_613 = s_record_100_get_line_take(s_ga_10100_get_checked_take(s_record_102_get_tokens_take(s_record_102_copy(v_tree)), s_record_101_get_token_take(s_ast_node(s_record_102_copy(v_tree), v_id))));
	s_record_102_drop(v_tree);
	return s_return_613;
}

s_record_122 s_emit_error(s_record_122 v_state, int64_t v_id, S_String * v_message)
{
	s_record_122 v_result = s_record_122_copy(v_state);
	if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		S_String * s_field_614 = s_string_retain(v_message);
		s_string_release(v_result.error);
		v_result.error = s_field_614;
		v_result.error_line = s_ast_line(s_record_122_get_tree_take(s_record_122_copy(v_result)), v_id);
	}
	s_record_122 s_return_615 = s_record_122_copy(v_result);
	s_record_122_drop(v_result);
	s_string_release(v_message);
	s_record_122_drop(v_state);
	return s_return_615;
	s_record_122_drop(v_result);
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
		S_String * s_left_616 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("Dict[", sizeof("Dict[") - 1), s_type_name(s_dict_key(v_kind))), s_string_from_bytes(",", sizeof(",") - 1));
		S_String * s_right_617 = s_type_name(s_dict_value(v_kind));
		return s_string_concat_take(s_string_concat_take(s_left_616, s_right_617), s_string_from_bytes("]", sizeof("]") - 1));
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
		int64_t s_return_618 = 0;
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_618;
	}
	S_String * v_name = s_ast_text(s_record_102_copy(v_tree), v_id);
	if (((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Dict", sizeof("Dict") - 1)) == 0) && (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2)))
	{
		int64_t s_arg_619 = s_ast_type(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, 0));
		int64_t s_arg_620 = s_ast_type(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, 1));
		int64_t s_return_621 = s_dict_of(s_arg_619, s_arg_620);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_621;
	}
	if (((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("Array", sizeof("Array") - 1)) == 0) && ((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 1) || (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2))))
	{
		if (((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2) && (s_string_compare_take(s_ast_text(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, 1)), s_string_from_bytes("1", sizeof("1") - 1)) != 0)))
		{
			int64_t s_return_622 = 0;
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_102_drop(v_tree);
			return s_return_622;
		}
		int64_t s_return_623 = s_array_of(s_ast_type(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, 0)));
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_623;
	}
	if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) != 0))
	{
		int64_t s_return_624 = 0;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_624;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("i64", sizeof("i64") - 1)) == 0))
	{
		int64_t s_return_625 = 1;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_625;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("f64", sizeof("f64") - 1)) == 0))
	{
		int64_t s_return_626 = 2;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_626;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("bool", sizeof("bool") - 1)) == 0))
	{
		int64_t s_return_627 = 3;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_627;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("string", sizeof("string") - 1)) == 0))
	{
		int64_t s_return_628 = 4;
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_628;
	}
	int64_t s_return_629 = 0;
	s_string_release(v_name);
	s_record_101_drop(v_entry);
	s_record_102_drop(v_tree);
	return s_return_629;
	s_string_release(v_name);
	s_record_101_drop(v_entry);
}

int64_t s_resolve_type(s_record_122 v_state, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 38))
	{
		int64_t s_return_630 = 0;
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_630;
	}
	if (((s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("Dict", sizeof("Dict") - 1)) == 0) && (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2)))
	{
		int64_t s_arg_631 = s_resolve_type(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		int64_t s_arg_632 = s_resolve_type(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
		int64_t s_return_633 = s_dict_of(s_arg_631, s_arg_632);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_633;
	}
	if (((s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("Array", sizeof("Array") - 1)) == 0) && ((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 1) || (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2))))
	{
		if (((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2) && (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1)), s_string_from_bytes("1", sizeof("1") - 1)) != 0)))
		{
			int64_t s_return_634 = 0;
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_634;
		}
		int64_t s_return_635 = s_array_of(s_resolve_type(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0)));
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_635;
	}
	const int64_t v_primitive = s_ast_type(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if ((v_primitive != 0))
	{
		int64_t s_return_636 = v_primitive;
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_636;
	}
	if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 0))
	{
		const int64_t v_index = s_find_function(s_record_122_copy(v_state), s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id));
		if (((v_index >= 0) && s_record_118_get_constructor_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_index))))
		{
			int64_t s_return_637 = s_record_118_get_kind_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_index));
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_637;
		}
	}
	int64_t s_return_638 = 0;
	s_record_101_drop(v_entry);
	s_record_122_drop(v_state);
	return s_return_638;
	s_record_101_drop(v_entry);
}

int64_t s_array_constructor_kind(s_record_122 v_state, int64_t v_id)
{
	s_record_101 v_index = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_index)) != 23) || (s_record_101_get_count_take(s_record_101_copy(v_index)) != 2)))
	{
		int64_t s_return_639 = 0;
		s_record_101_drop(v_index);
		s_record_122_drop(v_state);
		return s_return_639;
	}
	const int64_t v_root = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
	if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root)) != 29) || (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root), s_string_from_bytes("Array", sizeof("Array") - 1)) != 0)))
	{
		int64_t s_return_640 = 0;
		s_record_101_drop(v_index);
		s_record_122_drop(v_state);
		return s_return_640;
	}
	const int64_t v_type_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1);
	if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_type_id)) == 23))
	{
		int64_t s_return_641 = s_array_of(s_array_constructor_kind(s_record_122_copy(v_state), v_type_id));
		s_record_101_drop(v_index);
		s_record_122_drop(v_state);
		return s_return_641;
	}
	if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_type_id)) != 29))
	{
		int64_t s_return_642 = 0;
		s_record_101_drop(v_index);
		s_record_122_drop(v_state);
		return s_return_642;
	}
	S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_type_id);
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("string", sizeof("string") - 1)) == 0))
	{
		int64_t s_return_643 = 8;
		s_string_release(v_name);
		s_record_101_drop(v_index);
		s_record_122_drop(v_state);
		return s_return_643;
	}
	if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("i64", sizeof("i64") - 1)) == 0))
	{
		int64_t s_return_644 = 7;
		s_string_release(v_name);
		s_record_101_drop(v_index);
		s_record_122_drop(v_state);
		return s_return_644;
	}
	const int64_t v_function = s_find_function(s_record_122_copy(v_state), s_string_retain(v_name));
	if (((v_function >= 0) && s_record_118_get_constructor_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_function))))
	{
		int64_t s_return_645 = s_array_of(s_record_118_get_kind_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_function)));
		s_string_release(v_name);
		s_record_101_drop(v_index);
		s_record_122_drop(v_state);
		return s_return_645;
	}
	int64_t s_return_646 = 0;
	s_string_release(v_name);
	s_record_101_drop(v_index);
	s_record_122_drop(v_state);
	return s_return_646;
	s_string_release(v_name);
	s_record_101_drop(v_index);
}

int64_t s_dict_constructor_kind(s_record_122 v_state, int64_t v_id)
{
	s_record_101 v_node = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_node)) != 23) || (s_record_101_get_count_take(s_record_101_copy(v_node)) != 3)))
	{
		int64_t s_return_647 = 0;
		s_record_101_drop(v_node);
		s_record_122_drop(v_state);
		return s_return_647;
	}
	const int64_t v_root = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
	if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root)) != 29) || (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root), s_string_from_bytes("Dict", sizeof("Dict") - 1)) != 0)))
	{
		int64_t s_return_648 = 0;
		s_record_101_drop(v_node);
		s_record_122_drop(v_state);
		return s_return_648;
	}
	S_String * v_key = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
	const int64_t v_value_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 2);
	S_String * v_value = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_value_id);
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
	if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_value_id)) == 23))
	{
		v_value_kind = s_array_constructor_kind(s_record_122_copy(v_state), v_value_id);
	}
	const int64_t v_function = s_find_function(s_record_122_copy(v_state), s_string_retain(v_value));
	if (((v_function >= 0) && s_record_118_get_constructor_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_function))))
	{
		v_value_kind = s_record_118_get_kind_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_function));
	}
	int64_t s_return_649 = s_dict_of(v_key_kind, v_value_kind);
	s_string_release(v_value);
	s_string_release(v_key);
	s_record_101_drop(v_node);
	s_record_122_drop(v_state);
	return s_return_649;
	s_string_release(v_value);
	s_string_release(v_key);
	s_record_101_drop(v_node);
}

s_record_122 s_register_dict(s_record_122 v_state, int64_t v_kind)
{
	s_record_122 v_next = s_record_122_copy(v_state);
	{
		S_Array_i64 * v_s_iter_650 = s_record_122_get_dict_kinds_take(s_record_122_copy(v_next));
		for (int64_t s_index_650 = 0; s_index_650 < (int64_t)v_s_iter_650->length; ++s_index_650)
		{
			int64_t v_existing = s_array_i64_get_checked(v_s_iter_650, s_index_650);
			if ((v_existing == v_kind))
			{
				s_record_122 s_return_651 = s_record_122_copy(v_next);
				s_array_i64_release(v_s_iter_650);
				s_record_122_drop(v_next);
				s_record_122_drop(v_state);
				return s_return_651;
			}
		}
		s_array_i64_release(v_s_iter_650);
	}
	S_Array_i64 * s_field_652 = s_array_i64_append_take(s_record_122_get_dict_kinds_take(s_record_122_copy(v_next)), v_kind);
	s_array_i64_release(v_next.dict_kinds);
	v_next.dict_kinds = s_field_652;
	s_record_122 s_return_653 = s_record_122_copy(v_next);
	s_record_122_drop(v_next);
	s_record_122_drop(v_state);
	return s_return_653;
	s_record_122_drop(v_next);
}

s_record_122 s_line(s_record_122 v_state, S_String * v_text)
{
	s_record_122 v_result = s_record_122_copy(v_state);
	S_String * v_prefix = s_string_from_bytes("", sizeof("") - 1);
	for (int64_t v_i = 0, s_bound_654 = s_record_122_get_indentation_take(s_record_122_copy(v_state)); v_i < s_bound_654; ++v_i)
	{
		S_String *s_value_655 = s_string_concat_take(s_string_retain(v_prefix), s_string_from_bytes("\t", sizeof("\t") - 1));
		s_string_release(v_prefix);
		v_prefix = s_value_655;
	}
	S_Array * s_field_656 = s_ga_8_append_take(s_record_122_get_lines_take(s_record_122_copy(v_result)), s_string_concat_take(s_string_retain(v_prefix), s_string_retain(v_text)));
	s_array_release(v_result.lines);
	v_result.lines = s_field_656;
	s_record_122 s_return_657 = s_record_122_copy(v_result);
	s_string_release(v_prefix);
	s_record_122_drop(v_result);
	s_string_release(v_text);
	s_record_122_drop(v_state);
	return s_return_657;
	s_string_release(v_prefix);
	s_record_122_drop(v_result);
}

int64_t s_find_symbol(s_record_122 v_state, S_String * v_name)
{
	int64_t s_return_658 = s_record_109_get_field_index_take(s_resolve_hir_name(s_record_122_get_symbols_take(s_record_122_copy(v_state)), s_string_retain(v_name)));
	s_string_release(v_name);
	s_record_122_drop(v_state);
	return s_return_658;
}

int64_t s_find_function(s_record_122 v_state, S_String * v_name)
{
	for (int64_t v_i = 0, s_bound_659 = s_generic_array_length_take(s_record_122_get_functions_take(s_record_122_copy(v_state))); v_i < s_bound_659; ++v_i)
	{
		if ((s_string_compare_take(s_record_118_get_name_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_i)), s_string_retain(v_name)) == 0))
		{
			int64_t s_return_660 = v_i;
			s_string_release(v_name);
			s_record_122_drop(v_state);
			return s_return_660;
		}
	}
	int64_t s_return_661 = (-1);
	s_string_release(v_name);
	s_record_122_drop(v_state);
	return s_return_661;
}

bool s_is_generic(s_record_118 v_info)
{
	if ((s_record_118_get_kind_take(s_record_118_copy(v_info)) == 0))
	{
		bool s_return_662 = true;
		s_record_118_drop(v_info);
		return s_return_662;
	}
	{
		S_Array * v_s_iter_663 = s_record_118_get_params_take(s_record_118_copy(v_info));
		for (int64_t s_index_663 = 0; s_index_663 < (int64_t)s_array_length(v_s_iter_663); ++s_index_663)
		{
			s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_663, s_index_663);
			if ((s_record_104_get_kind_take(s_record_104_copy(v_param)) == 0))
			{
				bool s_return_664 = true;
				s_record_104_drop(v_param);
				s_array_release(v_s_iter_663);
				s_record_118_drop(v_info);
				return s_return_664;
			}
			s_record_104_drop(v_param);
		}
		s_array_release(v_s_iter_663);
	}
	bool s_return_665 = false;
	s_record_118_drop(v_info);
	return s_return_665;
}

int64_t s_find_instance(s_record_122 v_state, S_String * v_name, S_Array_i64 * v_type_ids)
{
	for (int64_t v_i = 0, s_bound_666 = s_generic_array_length_take(s_record_122_get_instances_take(s_record_122_copy(v_state))); v_i < s_bound_666; ++v_i)
	{
		s_record_119 v_instance = s_ga_10119_get_checked_take(s_record_122_get_instances_take(s_record_122_copy(v_state)), v_i);
		int64_t s_left_667 = s_array_i64_length_take(s_record_119_get_type_ids_take(s_record_119_copy(v_instance)));
		int64_t s_right_668 = s_array_i64_length_take(s_array_i64_retain(v_type_ids));
		if (((s_string_compare_take(s_record_119_get_name_take(s_record_119_copy(v_instance)), s_string_retain(v_name)) == 0) && (s_left_667 == s_right_668)))
		{
			bool v_equal = true;
			for (int64_t v_j = 0, s_bound_669 = s_array_i64_length_take(s_array_i64_retain(v_type_ids)); v_j < s_bound_669; ++v_j)
			{
				if ((s_array_i64_get_checked_take(s_record_119_get_type_ids_take(s_record_119_copy(v_instance)), v_j) != s_array_i64_get_checked_take(s_array_i64_retain(v_type_ids), v_j)))
				{
					v_equal = false;
				}
			}
			if (v_equal)
			{
				int64_t s_return_670 = v_i;
				s_record_119_drop(v_instance);
				s_array_i64_release(v_type_ids);
				s_string_release(v_name);
				s_record_122_drop(v_state);
				return s_return_670;
			}
		}
		s_record_119_drop(v_instance);
	}
	int64_t s_return_671 = (-1);
	s_array_i64_release(v_type_ids);
	s_string_release(v_name);
	s_record_122_drop(v_state);
	return s_return_671;
}

S_String * s_find_constant(s_record_122 v_state, S_String * v_name)
{
	{
		S_Array * v_s_iter_672 = s_record_122_get_constants_take(s_record_122_copy(v_state));
		for (int64_t s_index_672 = 0; s_index_672 < (int64_t)s_array_length(v_s_iter_672); ++s_index_672)
		{
			s_record_120 v_item = s_ga_10120_get_borrow(v_s_iter_672, s_index_672);
			if ((s_string_compare_take(s_record_120_get_name_take(s_record_120_copy(v_item)), s_string_retain(v_name)) == 0))
			{
				S_String * s_return_673 = s_record_120_get_value_take(s_record_120_copy(v_item));
				s_record_120_drop(v_item);
				s_array_release(v_s_iter_672);
				s_string_release(v_name);
				s_record_122_drop(v_state);
				return s_return_673;
			}
			s_record_120_drop(v_item);
		}
		s_array_release(v_s_iter_672);
	}
	S_String * s_return_674 = s_string_from_bytes("", sizeof("") - 1);
	s_string_release(v_name);
	s_record_122_drop(v_state);
	return s_return_674;
}

int64_t s_record_field_index(s_record_122 v_state, int64_t v_kind, S_String * v_name)
{
	if ((((v_kind < 100) || (v_kind >= 10000)) || ((v_kind - 100) >= s_generic_array_length_take(s_record_122_get_records_take(s_record_122_copy(v_state))))))
	{
		int64_t s_return_675 = (-1);
		s_string_release(v_name);
		s_record_122_drop(v_state);
		return s_return_675;
	}
	const int64_t v_canonical = s_record_121_get_canonical_take(s_ga_10121_get_checked_take(s_record_122_get_records_take(s_record_122_copy(v_state)), (v_kind - 100)));
	S_Array * v_fields = s_record_105_get_fields_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_122_get_types_take(s_record_122_copy(v_state))), v_canonical));
	for (int64_t v_i = 0, s_bound_676 = s_generic_array_length_take(s_array_retain(v_fields)); v_i < s_bound_676; ++v_i)
	{
		if ((s_string_compare_take(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_fields), v_i)), s_string_retain(v_name)) == 0))
		{
			int64_t s_return_677 = v_i;
			s_array_release(v_fields);
			s_string_release(v_name);
			s_record_122_drop(v_state);
			return s_return_677;
		}
	}
	int64_t s_return_678 = (-1);
	s_array_release(v_fields);
	s_string_release(v_name);
	s_record_122_drop(v_state);
	return s_return_678;
	s_array_release(v_fields);
}

s_record_107 s_canonical_kind(s_record_106 v_table, S_Array * v_records, int64_t v_kind)
{
	if (((v_kind >= 1) && (v_kind <= 5)))
	{
		s_record_107 s_return_679 = s_InternedType(s_record_106_copy(v_table), v_kind);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_679;
	}
	if (((v_kind >= 100) && (v_kind < 10000)))
	{
		const int64_t v_index = (v_kind - 100);
		if ((v_index < s_generic_array_length_take(s_array_retain(v_records))))
		{
			s_record_107 s_return_680 = s_InternedType(s_record_106_copy(v_table), s_record_121_get_canonical_take(s_ga_10121_get_checked_take(s_array_retain(v_records), v_index)));
			s_array_release(v_records);
			s_record_106_drop(v_table);
			return s_return_680;
		}
		s_record_107 s_return_681 = s_InternedType(s_record_106_copy(v_table), 0);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_681;
	}
	if (s_is_optional(v_kind))
	{
		s_record_107 v_payload = s_canonical_kind(s_record_106_copy(v_table), s_array_retain(v_records), s_optional_value(v_kind));
		s_record_107 s_return_682 = s_intern_optional_type(s_record_107_get_table_take(s_record_107_copy(v_payload)), s_record_107_get_id_take(s_record_107_copy(v_payload)));
		s_record_107_drop(v_payload);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_682;
		s_record_107_drop(v_payload);
	}
	if (s_is_dict(v_kind))
	{
		s_record_107 v_key = s_canonical_kind(s_record_106_copy(v_table), s_array_retain(v_records), s_dict_key(v_kind));
		s_record_107 v_value = s_canonical_kind(s_record_107_get_table_take(s_record_107_copy(v_key)), s_array_retain(v_records), s_dict_value(v_kind));
		s_record_107 s_return_683 = s_intern_dict_type(s_record_107_get_table_take(s_record_107_copy(v_value)), s_record_107_get_id_take(s_record_107_copy(v_key)), s_record_107_get_id_take(s_record_107_copy(v_value)));
		s_record_107_drop(v_value);
		s_record_107_drop(v_key);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_683;
		s_record_107_drop(v_value);
		s_record_107_drop(v_key);
	}
	const int64_t v_element = s_array_element(v_kind);
	if ((v_element != 0))
	{
		s_record_107 v_inner = s_canonical_kind(s_record_106_copy(v_table), s_array_retain(v_records), v_element);
		s_record_107 s_return_684 = s_intern_array_type(s_record_107_get_table_take(s_record_107_copy(v_inner)), s_record_107_get_id_take(s_record_107_copy(v_inner)));
		s_record_107_drop(v_inner);
		s_array_release(v_records);
		s_record_106_drop(v_table);
		return s_return_684;
		s_record_107_drop(v_inner);
	}
	s_record_107 s_return_685 = s_InternedType(s_record_106_copy(v_table), 0);
	s_array_release(v_records);
	s_record_106_drop(v_table);
	return s_return_685;
}

s_record_125 s_InternedSignature(s_record_106 v_table, S_Array_i64 * v_ids)
{
	s_record_125 s_constructor_result = {0};
	s_constructor_result.table = v_table;
	s_constructor_result.ids = v_ids;
	return s_constructor_result;
}

s_record_125 s_canonical_signature(s_record_122 v_state, S_Array_i64 * v_kinds)
{
	s_record_106 v_table = s_record_122_get_types_take(s_record_122_copy(v_state));
	S_Array_i64 * v_ids = s_array_i64_make(0, NULL);
	{
		S_Array_i64 * v_s_iter_686 = s_array_i64_retain(v_kinds);
		for (int64_t s_index_686 = 0; s_index_686 < (int64_t)v_s_iter_686->length; ++s_index_686)
		{
			int64_t v_kind = s_array_i64_get_checked(v_s_iter_686, s_index_686);
			s_record_107 v_entry = s_canonical_kind(s_record_106_copy(v_table), s_record_122_get_records_take(s_record_122_copy(v_state)), v_kind);
			s_record_106 s_value_687 = s_record_107_get_table_take(s_record_107_copy(v_entry));
			s_record_106_drop(v_table);
			v_table = s_value_687;
			int64_t s_item_688 = s_record_107_get_id_take(s_record_107_copy(v_entry));
			v_ids = s_array_i64_append_take(v_ids, s_item_688);
			s_record_107_drop(v_entry);
		}
		s_array_i64_release(v_s_iter_686);
	}
	s_record_125 s_return_689 = s_InternedSignature(s_record_106_copy(v_table), s_array_i64_retain(v_ids));
	s_array_i64_release(v_ids);
	s_record_106_drop(v_table);
	s_array_i64_release(v_kinds);
	s_record_122_drop(v_state);
	return s_return_689;
	s_array_i64_release(v_ids);
	s_record_106_drop(v_table);
}

s_record_123 s_intern_record(s_record_122 v_state, S_Array * v_fields)
{
	S_Array * v_ordered = s_ordered_fields(s_array_retain(v_fields));
	s_record_122 v_next = s_record_122_copy(v_state);
	S_Array * v_canonical_fields = s_ga_10104_make_take(0, NULL);
	{
		S_Array * v_s_iter_690 = s_array_retain(v_ordered);
		for (int64_t s_index_690 = 0; s_index_690 < (int64_t)s_array_length(v_s_iter_690); ++s_index_690)
		{
			s_record_104 v_field = s_ga_10104_get_borrow(v_s_iter_690, s_index_690);
			s_record_107 v_lookup = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_next)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_104_get_kind_take(s_record_104_copy(v_field)));
			if ((s_record_107_get_id_take(s_record_107_copy(v_lookup)) == 0))
			{
				s_record_123 s_return_691 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_107_drop(v_lookup);
				s_record_104_drop(v_field);
				s_array_release(v_s_iter_690);
				s_array_release(v_canonical_fields);
				s_record_122_drop(v_next);
				s_array_release(v_ordered);
				s_array_release(v_fields);
				s_record_122_drop(v_state);
				return s_return_691;
			}
			s_record_106 s_field_692 = s_record_107_get_table_take(s_record_107_copy(v_lookup));
			s_record_106_drop(v_next.types);
			v_next.types = s_field_692;
			s_record_104 s_item_693 = s_RecordField(s_record_104_get_name_take(s_record_104_copy(v_field)), s_record_107_get_id_take(s_record_107_copy(v_lookup)));
			v_canonical_fields = s_ga_10104_append_take(v_canonical_fields, s_item_693);
			s_record_107_drop(v_lookup);
			s_record_104_drop(v_field);
		}
		s_array_release(v_s_iter_690);
	}
	s_record_107 v_entry = s_intern_record_type(s_record_122_get_types_take(s_record_122_copy(v_next)), s_array_retain(v_canonical_fields));
	s_record_106 s_field_694 = s_record_107_get_table_take(s_record_107_copy(v_entry));
	s_record_106_drop(v_next.types);
	v_next.types = s_field_694;
	for (int64_t v_i = 0, s_bound_695 = s_generic_array_length_take(s_record_122_get_records_take(s_record_122_copy(v_next))); v_i < s_bound_695; ++v_i)
	{
		if ((s_record_121_get_canonical_take(s_ga_10121_get_checked_take(s_record_122_get_records_take(s_record_122_copy(v_next)), v_i)) == s_record_107_get_id_take(s_record_107_copy(v_entry))))
		{
			s_record_123 s_return_696 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), (100 + v_i));
			s_record_107_drop(v_entry);
			s_array_release(v_canonical_fields);
			s_record_122_drop(v_next);
			s_array_release(v_ordered);
			s_array_release(v_fields);
			s_record_122_drop(v_state);
			return s_return_696;
		}
	}
	const int64_t v_kind = (100 + s_generic_array_length_take(s_record_122_get_records_take(s_record_122_copy(v_next))));
	S_Array * s_field_697 = s_ga_10121_append_take(s_record_122_get_records_take(s_record_122_copy(v_next)), s_RecordInfo(s_array_retain(v_ordered), s_record_107_get_id_take(s_record_107_copy(v_entry))));
	s_array_release(v_next.records);
	v_next.records = s_field_697;
	s_record_123 s_return_698 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), v_kind);
	s_record_107_drop(v_entry);
	s_array_release(v_canonical_fields);
	s_record_122_drop(v_next);
	s_array_release(v_ordered);
	s_array_release(v_fields);
	s_record_122_drop(v_state);
	return s_return_698;
	s_record_107_drop(v_entry);
	s_array_release(v_canonical_fields);
	s_record_122_drop(v_next);
	s_array_release(v_ordered);
}

s_record_126 s_CTypeABI(int64_t v_type_id, S_String * v_c_name, S_String * v_copy, S_String * v_drop)
{
	s_record_126 s_constructor_result = {0};
	s_constructor_result.type_id = v_type_id;
	s_constructor_result.c_name = v_c_name;
	s_constructor_result.copy = v_copy;
	s_constructor_result.drop = v_drop;
	return s_constructor_result;
}

s_record_126 s_c_abi(s_record_122 v_state, int64_t v_kind)
{
	s_record_107 v_interned = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_state)), s_record_122_get_records_take(s_record_122_copy(v_state)), v_kind);
	const int64_t v_id = s_record_107_get_id_take(s_record_107_copy(v_interned));
	if ((v_id == 0))
	{
		s_record_126 s_return_699 = s_CTypeABI(v_id, s_string_from_bytes("INVALID_TYPE", sizeof("INVALID_TYPE") - 1), s_string_from_bytes("", sizeof("") - 1), s_string_from_bytes("", sizeof("") - 1));
		s_record_107_drop(v_interned);
		s_record_122_drop(v_state);
		return s_return_699;
	}
	s_record_105 v_descriptor = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_107_get_table_take(s_record_107_copy(v_interned))), v_id);
	S_String * v_name = s_c_type(v_kind);
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 4))
	{
		s_record_126 s_return_700 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("s_string_retain", sizeof("s_string_retain") - 1), s_string_from_bytes("s_string_release", sizeof("s_string_release") - 1));
		s_string_release(v_name);
		s_record_105_drop(v_descriptor);
		s_record_107_drop(v_interned);
		s_record_122_drop(v_state);
		return s_return_700;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 6))
	{
		s_record_126 s_return_701 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_concat_take(s_string_retain(v_name), s_string_from_bytes("_copy", sizeof("_copy") - 1)), s_string_concat_take(s_string_retain(v_name), s_string_from_bytes("_drop", sizeof("_drop") - 1)));
		s_string_release(v_name);
		s_record_105_drop(v_descriptor);
		s_record_107_drop(v_interned);
		s_record_122_drop(v_state);
		return s_return_701;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 7))
	{
		if ((s_record_105_get_first_take(s_record_105_copy(v_descriptor)) == 1))
		{
			s_record_126 s_return_702 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("s_array_i64_retain", sizeof("s_array_i64_retain") - 1), s_string_from_bytes("s_array_i64_release", sizeof("s_array_i64_release") - 1));
			s_string_release(v_name);
			s_record_105_drop(v_descriptor);
			s_record_107_drop(v_interned);
			s_record_122_drop(v_state);
			return s_return_702;
		}
		s_record_126 s_return_703 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("s_array_retain", sizeof("s_array_retain") - 1), s_string_from_bytes("s_array_release", sizeof("s_array_release") - 1));
		s_string_release(v_name);
		s_record_105_drop(v_descriptor);
		s_record_107_drop(v_interned);
		s_record_122_drop(v_state);
		return s_return_703;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 8))
	{
		s_record_126 s_return_704 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("s_dict_retain", sizeof("s_dict_retain") - 1), s_string_from_bytes("s_dict_release", sizeof("s_dict_release") - 1));
		s_string_release(v_name);
		s_record_105_drop(v_descriptor);
		s_record_107_drop(v_interned);
		s_record_122_drop(v_state);
		return s_return_704;
	}
	if ((s_record_105_get_tag_take(s_record_105_copy(v_descriptor)) == 9))
	{
		s_record_105 v_payload = s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_107_get_table_take(s_record_107_copy(v_interned))), s_record_105_get_first_take(s_record_105_copy(v_descriptor)));
		if ((((((s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 4) || (s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 6)) || (s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 7)) || (s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 8)) || (s_record_105_get_tag_take(s_record_105_copy(v_payload)) == 9)))
		{
			S_String * v_helper = s_optional_helper(v_kind);
			s_record_126 s_return_705 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_concat_take(s_string_retain(v_helper), s_string_from_bytes("_copy", sizeof("_copy") - 1)), s_string_concat_take(s_string_retain(v_helper), s_string_from_bytes("_drop", sizeof("_drop") - 1)));
			s_string_release(v_helper);
			s_record_105_drop(v_payload);
			s_string_release(v_name);
			s_record_105_drop(v_descriptor);
			s_record_107_drop(v_interned);
			s_record_122_drop(v_state);
			return s_return_705;
			s_string_release(v_helper);
		}
		s_record_105_drop(v_payload);
	}
	s_record_126 s_return_706 = s_CTypeABI(v_id, s_string_retain(v_name), s_string_from_bytes("", sizeof("") - 1), s_string_from_bytes("", sizeof("") - 1));
	s_string_release(v_name);
	s_record_105_drop(v_descriptor);
	s_record_107_drop(v_interned);
	s_record_122_drop(v_state);
	return s_return_706;
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

s_record_127 s_CFieldABI(S_String * v_name, int64_t v_type_id, s_record_126 v_abi)
{
	s_record_127 s_constructor_result = {0};
	s_constructor_result.name = v_name;
	s_constructor_result.type_id = v_type_id;
	s_constructor_result.abi = v_abi;
	return s_constructor_result;
}

S_Array * s_record_c_fields(s_record_122 v_state, int64_t v_record_index)
{
	s_record_121 v_record = s_ga_10121_get_checked_take(s_record_122_get_records_take(s_record_122_copy(v_state)), v_record_index);
	S_Array * v_canonical = s_record_105_get_fields_take(s_ga_10105_get_checked_take(s_record_106_get_entries_take(s_record_122_get_types_take(s_record_122_copy(v_state))), s_record_121_get_canonical_take(s_record_121_copy(v_record))));
	S_Array * v_result = s_ga_10127_make_take(0, NULL);
	for (int64_t v_i = 0, s_bound_707 = s_generic_array_length_take(s_array_retain(v_canonical)); v_i < s_bound_707; ++v_i)
	{
		s_record_104 v_field = s_ga_10104_get_checked_take(s_array_retain(v_canonical), v_i);
		s_record_104 v_legacy = s_ga_10104_get_checked_take(s_record_121_get_fields_take(s_record_121_copy(v_record)), v_i);
		s_record_127 s_item_708 = s_CFieldABI(s_record_104_get_name_take(s_record_104_copy(v_field)), s_record_104_get_kind_take(s_record_104_copy(v_field)), s_c_abi(s_record_122_copy(v_state), s_record_104_get_kind_take(s_record_104_copy(v_legacy))));
		v_result = s_ga_10127_append_take(v_result, s_item_708);
		s_record_104_drop(v_legacy);
		s_record_104_drop(v_field);
	}
	S_Array * s_return_709 = s_array_retain(v_result);
	s_array_release(v_result);
	s_array_release(v_canonical);
	s_record_121_drop(v_record);
	s_record_122_drop(v_state);
	return s_return_709;
	s_array_release(v_result);
	s_array_release(v_canonical);
	s_record_121_drop(v_record);
}

S_String * s_record_copy(int64_t v_kind, S_String * v_expr)
{
	S_String * v_function = s_retain_function(v_kind);
	if ((s_string_compare_take(s_string_retain(v_function), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		S_String * s_return_710 = s_string_retain(v_expr);
		s_string_release(v_function);
		s_string_release(v_expr);
		return s_return_710;
	}
	S_String * s_return_711 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_function), s_string_from_bytes("(", sizeof("(") - 1)), s_string_retain(v_expr)), s_string_from_bytes(")", sizeof(")") - 1));
	s_string_release(v_function);
	s_string_release(v_expr);
	return s_return_711;
	s_string_release(v_function);
}

S_String * s_record_drop(int64_t v_kind, S_String * v_expr)
{
	S_String * v_function = s_release_function(v_kind);
	if ((s_string_compare_take(s_string_retain(v_function), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		S_String * s_return_712 = s_string_from_bytes("", sizeof("") - 1);
		s_string_release(v_function);
		s_string_release(v_expr);
		return s_return_712;
	}
	S_String * s_return_713 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_function), s_string_from_bytes("(", sizeof("(") - 1)), s_string_retain(v_expr)), s_string_from_bytes(");", sizeof(");") - 1));
	s_string_release(v_function);
	s_string_release(v_expr);
	return s_return_713;
	s_string_release(v_function);
}

bool s_plain_record(s_record_122 v_state, int64_t v_kind)
{
	if (((v_kind < 100) || (v_kind >= 10000)))
	{
		bool s_return_714 = (!s_managed(v_kind));
		s_record_122_drop(v_state);
		return s_return_714;
	}
	{
		S_Array * v_s_iter_715 = s_record_121_get_fields_take(s_ga_10121_get_checked_take(s_record_122_get_records_take(s_record_122_copy(v_state)), (v_kind - 100)));
		for (int64_t s_index_715 = 0; s_index_715 < (int64_t)s_array_length(v_s_iter_715); ++s_index_715)
		{
			s_record_104 v_field = s_ga_10104_get_borrow(v_s_iter_715, s_index_715);
			if ((!s_plain_record(s_record_122_copy(v_state), s_record_104_get_kind_take(s_record_104_copy(v_field)))))
			{
				bool s_return_716 = false;
				s_record_104_drop(v_field);
				s_array_release(v_s_iter_715);
				s_record_122_drop(v_state);
				return s_return_716;
			}
			s_record_104_drop(v_field);
		}
		s_array_release(v_s_iter_715);
	}
	bool s_return_717 = true;
	s_record_122_drop(v_state);
	return s_return_717;
}

S_String * s_record_declarations(s_record_122 v_state)
{
	S_String * v_result = s_string_from_bytes("", sizeof("") - 1);
	for (int64_t v_i = 0, s_bound_718 = s_generic_array_length_take(s_record_122_get_records_take(s_record_122_copy(v_state))); v_i < s_bound_718; ++v_i)
	{
		const int64_t v_kind = (100 + v_i);
		S_String * v_type = s_record_126_get_c_name_take(s_c_abi(s_record_122_copy(v_state), v_kind));
		S_Array * v_fields = s_record_c_fields(s_record_122_copy(v_state), v_i);
		S_String *s_value_719 = s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("typedef struct {\n", sizeof("typedef struct {\n") - 1));
		s_string_release(v_result);
		v_result = s_value_719;
		{
			S_Array * v_s_iter_720 = s_array_retain(v_fields);
			for (int64_t s_index_720 = 0; s_index_720 < (int64_t)s_array_length(v_s_iter_720); ++s_index_720)
			{
				s_record_127 v_field = s_ga_10127_get_borrow(v_s_iter_720, s_index_720);
				S_String *s_value_721 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_126_get_c_name_take(s_record_127_get_abi_take(s_record_127_copy(v_field)))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_127_get_name_take(s_record_127_copy(v_field))), s_string_from_bytes(";\n", sizeof(";\n") - 1));
				s_string_release(v_result);
				v_result = s_value_721;
				s_record_127_drop(v_field);
			}
			s_array_release(v_s_iter_720);
		}
		S_String *s_value_722 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("} ", sizeof("} ") - 1)), s_string_retain(v_type)), s_string_from_bytes(";\n", sizeof(";\n") - 1));
		s_string_release(v_result);
		v_result = s_value_722;
		S_String *s_value_723 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_type)), s_string_from_bytes("_copy(", sizeof("_copy(") - 1)), s_string_retain(v_type)), s_string_from_bytes(" value) {\n", sizeof(" value) {\n") - 1));
		s_string_release(v_result);
		v_result = s_value_723;
		{
			S_Array * v_s_iter_724 = s_array_retain(v_fields);
			for (int64_t s_index_724 = 0; s_index_724 < (int64_t)s_array_length(v_s_iter_724); ++s_index_724)
			{
				s_record_127 v_field = s_ga_10127_get_borrow(v_s_iter_724, s_index_724);
				if ((s_string_compare_take(s_record_126_get_copy_take(s_record_127_get_abi_take(s_record_127_copy(v_field))), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					S_String *s_value_725 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\tvalue.", sizeof("\tvalue.") - 1)), s_record_127_get_name_take(s_record_127_copy(v_field))), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_126_get_copy_take(s_record_127_get_abi_take(s_record_127_copy(v_field)))), s_string_from_bytes("(value.", sizeof("(value.") - 1)), s_record_127_get_name_take(s_record_127_copy(v_field))), s_string_from_bytes(");\n", sizeof(");\n") - 1));
					s_string_release(v_result);
					v_result = s_value_725;
				}
				s_record_127_drop(v_field);
			}
			s_array_release(v_s_iter_724);
		}
		S_String *s_value_726 = s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\treturn value;\n}\n", sizeof("\treturn value;\n}\n") - 1));
		s_string_release(v_result);
		v_result = s_value_726;
		S_String *s_value_727 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_type)), s_string_from_bytes("_drop(", sizeof("_drop(") - 1)), s_string_retain(v_type)), s_string_from_bytes(" value) {\n", sizeof(" value) {\n") - 1));
		s_string_release(v_result);
		v_result = s_value_727;
		{
			S_Array * v_s_iter_728 = s_array_retain(v_fields);
			for (int64_t s_index_728 = 0; s_index_728 < (int64_t)s_array_length(v_s_iter_728); ++s_index_728)
			{
				s_record_127 v_field = s_ga_10127_get_borrow(v_s_iter_728, s_index_728);
				if ((s_string_compare_take(s_record_126_get_drop_take(s_record_127_get_abi_take(s_record_127_copy(v_field))), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					S_String *s_value_729 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_126_get_drop_take(s_record_127_get_abi_take(s_record_127_copy(v_field)))), s_string_from_bytes("(value.", sizeof("(value.") - 1)), s_record_127_get_name_take(s_record_127_copy(v_field))), s_string_from_bytes(");\n", sizeof(");\n") - 1));
					s_string_release(v_result);
					v_result = s_value_729;
				}
				s_record_127_drop(v_field);
			}
			s_array_release(v_s_iter_728);
		}
		S_String *s_value_730 = s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t(void)value;\n}\n", sizeof("\t(void)value;\n}\n") - 1));
		s_string_release(v_result);
		v_result = s_value_730;
		{
			S_Array * v_s_iter_731 = s_array_retain(v_fields);
			for (int64_t s_index_731 = 0; s_index_731 < (int64_t)s_array_length(v_s_iter_731); ++s_index_731)
			{
				s_record_127 v_field = s_ga_10127_get_borrow(v_s_iter_731, s_index_731);
				S_String *s_value_732 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_record_126_get_c_name_take(s_record_127_get_abi_take(s_record_127_copy(v_field)))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_type)), s_string_from_bytes("_get_", sizeof("_get_") - 1)), s_record_127_get_name_take(s_record_127_copy(v_field))), s_string_from_bytes("_take(", sizeof("_take(") - 1)), s_string_retain(v_type)), s_string_from_bytes(" owned) {\n", sizeof(" owned) {\n") - 1));
				s_string_release(v_result);
				v_result = s_value_732;
				S_String * v_copied = s_string_concat_take(s_string_from_bytes("owned.", sizeof("owned.") - 1), s_record_127_get_name_take(s_record_127_copy(v_field)));
				if ((s_string_compare_take(s_record_126_get_copy_take(s_record_127_get_abi_take(s_record_127_copy(v_field))), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					S_String *s_value_733 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_record_126_get_copy_take(s_record_127_get_abi_take(s_record_127_copy(v_field))), s_string_from_bytes("(", sizeof("(") - 1)), s_string_retain(v_copied)), s_string_from_bytes(")", sizeof(")") - 1));
					s_string_release(v_copied);
					v_copied = s_value_733;
				}
				S_String *s_value_734 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_126_get_c_name_take(s_record_127_get_abi_take(s_record_127_copy(v_field)))), s_string_from_bytes(" field = ", sizeof(" field = ") - 1)), s_string_retain(v_copied)), s_string_from_bytes(";\n", sizeof(";\n") - 1));
				s_string_release(v_result);
				v_result = s_value_734;
				S_String *s_value_735 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes("_drop(owned);\n\treturn field;\n}\n", sizeof("_drop(owned);\n\treturn field;\n}\n") - 1));
				s_string_release(v_result);
				v_result = s_value_735;
				s_string_release(v_copied);
				s_record_127_drop(v_field);
			}
			s_array_release(v_s_iter_731);
		}
		s_array_release(v_fields);
		s_string_release(v_type);
	}
	S_String * s_return_736 = s_string_retain(v_result);
	s_string_release(v_result);
	s_record_122_drop(v_state);
	return s_return_736;
	s_string_release(v_result);
}

S_String * s_optional_declarations(s_record_122 v_state)
{
	S_String * v_output = s_string_from_bytes("", sizeof("") - 1);
	S_Array_i64 * v_seen = s_array_i64_make(0, NULL);
	S_Array_i64 * v_values = s_array_i64_make(0, NULL);
	int64_t s_item_737 = 4;
	v_values = s_array_i64_append_take(v_values, s_item_737);
	{
		S_Array_i64 * v_s_iter_738 = s_generic_array_element_types(s_record_122_copy(v_state));
		for (int64_t s_index_738 = 0; s_index_738 < (int64_t)v_s_iter_738->length; ++s_index_738)
		{
			int64_t v_element = s_array_i64_get_checked(v_s_iter_738, s_index_738);
			int64_t s_item_739 = v_element;
			v_values = s_array_i64_append_take(v_values, s_item_739);
		}
		s_array_i64_release(v_s_iter_738);
	}
	{
		S_Array_i64 * v_s_iter_740 = s_record_122_get_dict_kinds_take(s_record_122_copy(v_state));
		for (int64_t s_index_740 = 0; s_index_740 < (int64_t)v_s_iter_740->length; ++s_index_740)
		{
			int64_t v_dict_kind = s_array_i64_get_checked(v_s_iter_740, s_index_740);
			int64_t s_item_741 = s_dict_value(v_dict_kind);
			v_values = s_array_i64_append_take(v_values, s_item_741);
		}
		s_array_i64_release(v_s_iter_740);
	}
	{
		S_Array_i64 * v_s_iter_742 = s_array_i64_retain(v_values);
		for (int64_t s_index_742 = 0; s_index_742 < (int64_t)v_s_iter_742->length; ++s_index_742)
		{
			int64_t v_value = s_array_i64_get_checked(v_s_iter_742, s_index_742);
			if ((v_value == 1))
			{
				continue;
			}
			bool v_duplicate = false;
			{
				S_Array_i64 * v_s_iter_743 = s_array_i64_retain(v_seen);
				for (int64_t s_index_743 = 0; s_index_743 < (int64_t)v_s_iter_743->length; ++s_index_743)
				{
					int64_t v_previous = s_array_i64_get_checked(v_s_iter_743, s_index_743);
					if ((v_previous == v_value))
					{
						v_duplicate = true;
					}
				}
				s_array_i64_release(v_s_iter_743);
			}
			if (v_duplicate)
			{
				continue;
			}
			int64_t s_item_744 = v_value;
			v_seen = s_array_i64_append_take(v_seen, s_item_744);
			const int64_t v_kind = s_optional_of(v_value);
			S_String * v_name = s_optional_helper(v_kind);
			if ((v_value == 4))
			{
				S_String *s_value_745 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("typedef S_Optional_string ", sizeof("typedef S_Optional_string ") - 1)), s_string_retain(v_name)), s_string_from_bytes(";\n", sizeof(";\n") - 1));
				s_string_release(v_output);
				v_output = s_value_745;
			}
			else
			{
				S_String *s_value_746 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("typedef struct { bool has_value; ", sizeof("typedef struct { bool has_value; ") - 1)), s_c_type(v_value)), s_string_from_bytes(" value; } ", sizeof(" value; } ") - 1)), s_string_retain(v_name)), s_string_from_bytes(";\n", sizeof(";\n") - 1));
				s_string_release(v_output);
				v_output = s_value_746;
			}
			s_record_126 v_abi = s_c_abi(s_record_122_copy(v_state), v_value);
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_747 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_name)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_name)), s_string_from_bytes("_copy(", sizeof("_copy(") - 1)), s_string_retain(v_name)), s_string_from_bytes(" owned) {\n", sizeof(" owned) {\n") - 1));
				s_string_release(v_output);
				v_output = s_value_747;
				S_String *s_value_748 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (owned.has_value) owned.value = ", sizeof("\tif (owned.has_value) owned.value = ") - 1)), s_record_copy(v_value, s_string_from_bytes("owned.value", sizeof("owned.value") - 1))), s_string_from_bytes(";\n\treturn owned;\n}\n", sizeof(";\n\treturn owned;\n}\n") - 1));
				s_string_release(v_output);
				v_output = s_value_748;
				S_String *s_value_749 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_name)), s_string_from_bytes("_drop(", sizeof("_drop(") - 1)), s_string_retain(v_name)), s_string_from_bytes(" owned) {\n", sizeof(" owned) {\n") - 1));
				s_string_release(v_output);
				v_output = s_value_749;
				S_String *s_value_750 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (owned.has_value) ", sizeof("\tif (owned.has_value) ") - 1)), s_record_drop(v_value, s_string_from_bytes("owned.value", sizeof("owned.value") - 1))), s_string_from_bytes("\n}\n", sizeof("\n}\n") - 1));
				s_string_release(v_output);
				v_output = s_value_750;
			}
			s_record_126_drop(v_abi);
			s_string_release(v_name);
		}
		s_array_i64_release(v_s_iter_742);
	}
	S_String * s_return_751 = s_string_retain(v_output);
	s_array_i64_release(v_values);
	s_array_i64_release(v_seen);
	s_string_release(v_output);
	s_record_122_drop(v_state);
	return s_return_751;
	s_array_i64_release(v_values);
	s_array_i64_release(v_seen);
	s_string_release(v_output);
}

S_Array_i64 * s_generic_array_element_types(s_record_122 v_state)
{
	S_Array_i64 * v_elements = s_array_i64_make(0, NULL);
	int64_t s_item_752 = 4;
	v_elements = s_array_i64_append_take(v_elements, s_item_752);
	for (int64_t v_i = 0, s_bound_753 = s_generic_array_length_take(s_record_122_get_records_take(s_record_122_copy(v_state))); v_i < s_bound_753; ++v_i)
	{
		int64_t s_item_754 = (100 + v_i);
		v_elements = s_array_i64_append_take(v_elements, s_item_754);
	}
	S_Array_i64 * v_frontier = s_array_i64_make(0, NULL);
	int64_t s_item_755 = 7;
	v_frontier = s_array_i64_append_take(v_frontier, s_item_755);
	int64_t s_item_756 = 8;
	v_frontier = s_array_i64_append_take(v_frontier, s_item_756);
	for (int64_t v_i = 0, s_bound_757 = s_generic_array_length_take(s_record_122_get_records_take(s_record_122_copy(v_state))); v_i < s_bound_757; ++v_i)
	{
		int64_t s_item_758 = s_array_of((100 + v_i));
		v_frontier = s_array_i64_append_take(v_frontier, s_item_758);
	}
	for (int64_t v_depth = 0, s_bound_759 = 2; v_depth < s_bound_759; ++v_depth)
	{
		S_Array_i64 * v_next_frontier = s_array_i64_make(0, NULL);
		{
			S_Array_i64 * v_s_iter_760 = s_array_i64_retain(v_frontier);
			for (int64_t s_index_760 = 0; s_index_760 < (int64_t)v_s_iter_760->length; ++s_index_760)
			{
				int64_t v_item = s_array_i64_get_checked(v_s_iter_760, s_index_760);
				int64_t s_item_761 = v_item;
				v_elements = s_array_i64_append_take(v_elements, s_item_761);
				int64_t s_item_762 = s_array_of(v_item);
				v_next_frontier = s_array_i64_append_take(v_next_frontier, s_item_762);
			}
			s_array_i64_release(v_s_iter_760);
		}
		S_Array_i64 * s_value_763 = s_array_i64_retain(v_next_frontier);
		s_array_i64_release(v_frontier);
		v_frontier = s_value_763;
		s_array_i64_release(v_next_frontier);
	}
	S_Array_i64 * s_return_764 = s_array_i64_retain(v_elements);
	s_array_i64_release(v_frontier);
	s_array_i64_release(v_elements);
	s_record_122_drop(v_state);
	return s_return_764;
	s_array_i64_release(v_frontier);
	s_array_i64_release(v_elements);
}

S_String * s_generic_array_declarations(s_record_122 v_state)
{
	S_String * v_output = s_string_from_bytes("", sizeof("") - 1);
	S_Array_i64 * v_elements = s_generic_array_element_types(s_record_122_copy(v_state));
	S_String *s_value_765 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Optional_i64 s_array_i64_get_optional_take(S_Array_i64 *owned, int64_t index) {\n", sizeof("static inline S_Optional_i64 s_array_i64_get_optional_take(S_Array_i64 *owned, int64_t index) {\n") - 1));
	s_string_release(v_output);
	v_output = s_value_765;
	S_String *s_value_766 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tS_Optional_i64 result = s_array_i64_get(owned, index);\n\ts_array_i64_release(owned);\n\treturn result;\n}\n", sizeof("\tS_Optional_i64 result = s_array_i64_get(owned, index);\n\ts_array_i64_release(owned);\n\treturn result;\n}\n") - 1));
	s_string_release(v_output);
	v_output = s_value_766;
	{
		S_Array_i64 * v_s_iter_767 = s_array_i64_retain(v_elements);
		for (int64_t s_index_767 = 0; s_index_767 < (int64_t)v_s_iter_767->length; ++s_index_767)
		{
			int64_t v_element = s_array_i64_get_checked(v_s_iter_767, s_index_767);
			const int64_t v_kind = s_array_of(v_element);
			s_record_126 v_abi = s_c_abi(s_record_122_copy(v_state), v_element);
			S_String * v_type = s_record_126_get_c_name_take(s_record_126_copy(v_abi));
			S_String * v_prefix = s_array_helper(v_kind);
			S_String * v_copy = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_drop = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_768 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_copy(void *dst, const void *src) { *(", sizeof("_copy(void *dst, const void *src) { *(") - 1)), s_string_retain(v_type)), s_string_from_bytes(" *)dst = ", sizeof(" *)dst = ") - 1)), s_record_copy(v_element, s_string_concat_take(s_string_concat_take(s_string_from_bytes("*(", sizeof("*(") - 1), s_string_retain(v_type)), s_string_from_bytes(" const *)src", sizeof(" const *)src") - 1)))), s_string_from_bytes("; }\n", sizeof("; }\n") - 1));
				s_string_release(v_output);
				v_output = s_value_768;
				S_String *s_value_769 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_drop(void *value) { ", sizeof("_drop(void *value) { ") - 1)), s_record_drop(v_element, s_string_concat_take(s_string_concat_take(s_string_from_bytes("*(", sizeof("*(") - 1), s_string_retain(v_type)), s_string_from_bytes(" *)value", sizeof(" *)value") - 1)))), s_string_from_bytes(" }\n", sizeof(" }\n") - 1));
				s_string_release(v_output);
				v_output = s_value_769;
				S_String *s_value_770 = s_string_concat_take(s_string_retain(v_prefix), s_string_from_bytes("_copy", sizeof("_copy") - 1));
				s_string_release(v_copy);
				v_copy = s_value_770;
				S_String *s_value_771 = s_string_concat_take(s_string_retain(v_prefix), s_string_from_bytes("_drop", sizeof("_drop") - 1));
				s_string_release(v_drop);
				v_drop = s_value_771;
			}
			S_String *s_value_772 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Array *", sizeof("static inline S_Array *") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_make_take(size_t count, ", sizeof("_make_take(size_t count, ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" *items) {\n", sizeof(" *items) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_772;
			S_String *s_value_773 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tS_Array *result = s_array_make(count, items, sizeof(", sizeof("\tS_Array *result = s_array_make(count, items, sizeof(") - 1)), s_string_retain(v_type)), s_string_from_bytes("), ", sizeof("), ") - 1)), s_string_retain(v_copy)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_drop)), s_string_from_bytes(");\n", sizeof(");\n") - 1));
			s_string_release(v_output);
			v_output = s_value_773;
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_774 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tfor (size_t i = 0; i < count; ++i) ", sizeof("\tfor (size_t i = 0; i < count; ++i) ") - 1)), s_record_drop(v_element, s_string_from_bytes("items[i]", sizeof("items[i]") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_774;
			}
			S_String *s_value_775 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn result;\n}\n", sizeof("\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_775;
			S_String *s_value_776 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Array *", sizeof("static inline S_Array *") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_append_take(S_Array *owned, ", sizeof("_append_take(S_Array *owned, ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" item) {\n", sizeof(" item) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_776;
			S_String *s_value_777 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tS_Array *result = s_array_append(owned, &item);\n", sizeof("\tS_Array *result = s_array_append(owned, &item);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_777;
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_778 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_element, s_string_from_bytes("item", sizeof("item") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_778;
			}
			S_String *s_value_779 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn result;\n}\n", sizeof("\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_779;
			S_String *s_value_780 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_get_checked_take(S_Array *owned, int64_t index) {\n", sizeof("_get_checked_take(S_Array *owned, int64_t index) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_780;
			S_String *s_value_781 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *p = (", sizeof(" const *p = (") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *)s_array_at(owned, index);\n", sizeof(" const *)s_array_at(owned, index);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_781;
			S_String *s_value_782 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (!p) s_panic(\"index out of bounds\");\n", sizeof("\tif (!p) s_panic(\"index out of bounds\");\n") - 1));
			s_string_release(v_output);
			v_output = s_value_782;
			S_String *s_value_783 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes(" value = ", sizeof(" value = ") - 1)), s_record_copy(v_element, s_string_from_bytes("*p", sizeof("*p") - 1))), s_string_from_bytes(";\n", sizeof(";\n") - 1));
			s_string_release(v_output);
			v_output = s_value_783;
			S_String *s_value_784 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\ts_array_release(owned);\n\treturn value;\n}\n", sizeof("\ts_array_release(owned);\n\treturn value;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_784;
			S_String *s_value_785 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_c_type(s_optional_of(v_element))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_get_optional_take(S_Array *owned, int64_t index) {\n", sizeof("_get_optional_take(S_Array *owned, int64_t index) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_785;
			S_String *s_value_786 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_c_type(s_optional_of(v_element))), s_string_from_bytes(" result = {0};\n", sizeof(" result = {0};\n") - 1));
			s_string_release(v_output);
			v_output = s_value_786;
			S_String *s_value_787 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *p = (", sizeof(" const *p = (") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *)s_array_at(owned, index);\n", sizeof(" const *)s_array_at(owned, index);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_787;
			S_String *s_value_788 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (p) { result.has_value = true; result.value = ", sizeof("\tif (p) { result.has_value = true; result.value = ") - 1)), s_record_copy(v_element, s_string_from_bytes("*p", sizeof("*p") - 1))), s_string_from_bytes("; }\n", sizeof("; }\n") - 1));
			s_string_release(v_output);
			v_output = s_value_788;
			S_String *s_value_789 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\ts_array_release(owned);\n\treturn result;\n}\n", sizeof("\ts_array_release(owned);\n\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_789;
			S_String *s_value_790 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_get_borrow(const S_Array *array, int64_t index) {\n", sizeof("_get_borrow(const S_Array *array, int64_t index) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_790;
			S_String *s_value_791 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *p = (", sizeof(" const *p = (") - 1)), s_string_retain(v_type)), s_string_from_bytes(" const *)s_array_at(array, index);\n", sizeof(" const *)s_array_at(array, index);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_791;
			S_String *s_value_792 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (!p) s_panic(\"index out of bounds\");\n", sizeof("\tif (!p) s_panic(\"index out of bounds\");\n") - 1));
			s_string_release(v_output);
			v_output = s_value_792;
			S_String *s_value_793 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn ", sizeof("\treturn ") - 1)), s_record_copy(v_element, s_string_from_bytes("*p", sizeof("*p") - 1))), s_string_from_bytes(";\n}\n", sizeof(";\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_793;
			S_String *s_value_794 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline void ", sizeof("static inline void ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_set_take(S_Array **owned, int64_t index, ", sizeof("_set_take(S_Array **owned, int64_t index, ") - 1)), s_string_retain(v_type)), s_string_from_bytes(" item) {\n", sizeof(" item) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_794;
			S_String *s_value_795 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\ts_array_set(owned, index, &item);\n", sizeof("\ts_array_set(owned, index, &item);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_795;
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_796 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_element, s_string_from_bytes("item", sizeof("item") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_796;
			}
			S_String *s_value_797 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("}\n", sizeof("}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_797;
			s_string_release(v_drop);
			s_string_release(v_copy);
			s_string_release(v_prefix);
			s_string_release(v_type);
			s_record_126_drop(v_abi);
		}
		s_array_i64_release(v_s_iter_767);
	}
	S_String *s_value_798 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline int64_t s_generic_array_length_take(S_Array *owned) { int64_t n = (int64_t)s_array_length(owned); s_array_release(owned); return n; }\n", sizeof("static inline int64_t s_generic_array_length_take(S_Array *owned) { int64_t n = (int64_t)s_array_length(owned); s_array_release(owned); return n; }\n") - 1));
	s_string_release(v_output);
	v_output = s_value_798;
	S_String * s_return_799 = s_string_retain(v_output);
	s_array_i64_release(v_elements);
	s_string_release(v_output);
	s_record_122_drop(v_state);
	return s_return_799;
	s_array_i64_release(v_elements);
	s_string_release(v_output);
}

S_String * s_dict_declarations(s_record_122 v_state)
{
	S_String * v_output = s_string_from_bytes("", sizeof("") - 1);
	{
		S_Array_i64 * v_s_iter_800 = s_record_122_get_dict_kinds_take(s_record_122_copy(v_state));
		for (int64_t s_index_800 = 0; s_index_800 < (int64_t)v_s_iter_800->length; ++s_index_800)
		{
			int64_t v_kind = s_array_i64_get_checked(v_s_iter_800, s_index_800);
			const int64_t v_key = s_dict_key(v_kind);
			const int64_t v_value = s_dict_value(v_kind);
			S_String * v_prefix = s_dict_helper(v_kind);
			s_record_126 v_key_abi = s_c_abi(s_record_122_copy(v_state), v_key);
			s_record_126 v_value_abi = s_c_abi(s_record_122_copy(v_state), v_value);
			S_String * v_key_type = s_record_126_get_c_name_take(s_record_126_copy(v_key_abi));
			S_String * v_val_type = s_record_126_get_c_name_take(s_record_126_copy(v_value_abi));
			S_String * v_key_copy = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_key_drop = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_val_copy = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_val_drop = s_string_from_bytes("NULL", sizeof("NULL") - 1);
			S_String * v_hash = s_string_from_bytes("s_hash_i64_key", sizeof("s_hash_i64_key") - 1);
			S_String * v_eq = s_string_from_bytes("s_eq_i64_key", sizeof("s_eq_i64_key") - 1);
			if ((v_key == 4))
			{
				S_String *s_value_801 = s_string_concat_take(s_array_helper(8), s_string_from_bytes("_copy", sizeof("_copy") - 1));
				s_string_release(v_key_copy);
				v_key_copy = s_value_801;
				S_String *s_value_802 = s_string_concat_take(s_array_helper(8), s_string_from_bytes("_drop", sizeof("_drop") - 1));
				s_string_release(v_key_drop);
				v_key_drop = s_value_802;
				S_String *s_value_803 = s_string_from_bytes("s_hash_string_key", sizeof("s_hash_string_key") - 1);
				s_string_release(v_hash);
				v_hash = s_value_803;
				S_String *s_value_804 = s_string_from_bytes("s_eq_string_key", sizeof("s_eq_string_key") - 1);
				s_string_release(v_eq);
				v_eq = s_value_804;
			}
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_value_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_805 = s_string_concat_take(s_array_helper(s_array_of(v_value)), s_string_from_bytes("_copy", sizeof("_copy") - 1));
				s_string_release(v_val_copy);
				v_val_copy = s_value_805;
				S_String *s_value_806 = s_string_concat_take(s_array_helper(s_array_of(v_value)), s_string_from_bytes("_drop", sizeof("_drop") - 1));
				s_string_release(v_val_drop);
				v_val_drop = s_value_806;
			}
			S_String *s_value_807 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Dict *", sizeof("static inline S_Dict *") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_make(void) { return s_dict_new(sizeof(", sizeof("_make(void) { return s_dict_new(sizeof(") - 1)), s_string_retain(v_key_type)), s_string_from_bytes("), sizeof(", sizeof("), sizeof(") - 1)), s_string_retain(v_val_type)), s_string_from_bytes("), ", sizeof("), ") - 1)), s_string_retain(v_key_copy)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_key_drop)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_val_copy)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_val_drop)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_hash)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_eq)), s_string_from_bytes("); }\n", sizeof("); }\n") - 1));
			s_string_release(v_output);
			v_output = s_value_807;
			S_String *s_value_808 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline S_Dict *", sizeof("static inline S_Dict *") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_insert_take(S_Dict *owned, ", sizeof("_insert_take(S_Dict *owned, ") - 1)), s_string_retain(v_key_type)), s_string_from_bytes(" key, ", sizeof(" key, ") - 1)), s_string_retain(v_val_type)), s_string_from_bytes(" value) {\n", sizeof(" value) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_808;
			S_String *s_value_809 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tS_Dict *result = s_dict_insert(owned, &key, &value);\n", sizeof("\tS_Dict *result = s_dict_insert(owned, &key, &value);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_809;
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_key_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_810 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_key, s_string_from_bytes("key", sizeof("key") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_810;
			}
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_value_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_811 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_value, s_string_from_bytes("value", sizeof("value") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_811;
			}
			S_String *s_value_812 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn result;\n}\n", sizeof("\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_812;
			S_String * v_option_type = s_c_type(s_optional_of(v_value));
			S_String *s_value_813 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline ", sizeof("static inline ") - 1)), s_string_retain(v_option_type)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_prefix)), s_string_from_bytes("_get_take(S_Dict *owned, ", sizeof("_get_take(S_Dict *owned, ") - 1)), s_string_retain(v_key_type)), s_string_from_bytes(" key) {\n", sizeof(" key) {\n") - 1));
			s_string_release(v_output);
			v_output = s_value_813;
			S_String *s_value_814 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_val_type)), s_string_from_bytes(" const *item = (", sizeof(" const *item = (") - 1)), s_string_retain(v_val_type)), s_string_from_bytes(" const *)s_dict_get(owned, &key);\n", sizeof(" const *)s_dict_get(owned, &key);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_814;
			S_String *s_value_815 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_string_retain(v_option_type)), s_string_from_bytes(" result = {0};\n", sizeof(" result = {0};\n") - 1));
			s_string_release(v_output);
			v_output = s_value_815;
			S_String *s_value_816 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\tif (item) { result.has_value = true; result.value = ", sizeof("\tif (item) { result.has_value = true; result.value = ") - 1)), s_record_copy(v_value, s_string_from_bytes("*item", sizeof("*item") - 1))), s_string_from_bytes("; }\n", sizeof("; }\n") - 1));
			s_string_release(v_output);
			v_output = s_value_816;
			S_String *s_value_817 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\ts_dict_release(owned);\n", sizeof("\ts_dict_release(owned);\n") - 1));
			s_string_release(v_output);
			v_output = s_value_817;
			if ((s_string_compare_take(s_record_126_get_copy_take(s_record_126_copy(v_key_abi)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				S_String *s_value_818 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\t", sizeof("\t") - 1)), s_record_drop(v_key, s_string_from_bytes("key", sizeof("key") - 1))), s_string_from_bytes("\n", sizeof("\n") - 1));
				s_string_release(v_output);
				v_output = s_value_818;
			}
			S_String *s_value_819 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("\treturn result;\n}\n", sizeof("\treturn result;\n}\n") - 1));
			s_string_release(v_output);
			v_output = s_value_819;
			s_string_release(v_option_type);
			s_string_release(v_eq);
			s_string_release(v_hash);
			s_string_release(v_val_drop);
			s_string_release(v_val_copy);
			s_string_release(v_key_drop);
			s_string_release(v_key_copy);
			s_string_release(v_val_type);
			s_string_release(v_key_type);
			s_record_126_drop(v_value_abi);
			s_record_126_drop(v_key_abi);
			s_string_release(v_prefix);
		}
		s_array_i64_release(v_s_iter_800);
	}
	S_String *s_value_820 = s_string_concat_take(s_string_retain(v_output), s_string_from_bytes("static inline int64_t s_dict_length_take(S_Dict *owned) { int64_t n = (int64_t)s_dict_length(owned); s_dict_release(owned); return n; }\n", sizeof("static inline int64_t s_dict_length_take(S_Dict *owned) { int64_t n = (int64_t)s_dict_length(owned); s_dict_release(owned); return n; }\n") - 1));
	s_string_release(v_output);
	v_output = s_value_820;
	S_String * s_return_821 = s_string_retain(v_output);
	s_string_release(v_output);
	s_record_122_drop(v_state);
	return s_return_821;
	s_string_release(v_output);
}

s_record_122 s_append_symbol(s_record_122 v_state, int64_t v_id, S_String * v_name, int64_t v_kind, bool v_mutable)
{
	s_record_122 v_result = s_record_122_copy(v_state);
	if (((v_kind == 0) || (v_kind == 5)))
	{
		s_record_122 s_return_822 = s_emit_error(s_record_122_copy(v_result), v_id, s_string_from_bytes("unsupported binding type", sizeof("unsupported binding type") - 1));
		s_record_122_drop(v_result);
		s_string_release(v_name);
		s_record_122_drop(v_state);
		return s_return_822;
	}
	s_record_107 v_canonical = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_result)), s_record_122_get_records_take(s_record_122_copy(v_result)), v_kind);
	if ((s_record_107_get_id_take(s_record_107_copy(v_canonical)) == 0))
	{
		s_record_122 s_return_823 = s_emit_error(s_record_122_copy(v_result), v_id, s_string_from_bytes("unsupported binding type", sizeof("unsupported binding type") - 1));
		s_record_107_drop(v_canonical);
		s_record_122_drop(v_result);
		s_string_release(v_name);
		s_record_122_drop(v_state);
		return s_return_823;
	}
	s_record_106 s_field_824 = s_record_107_get_table_take(s_record_107_copy(v_canonical));
	s_record_106_drop(v_result.types);
	v_result.types = s_field_824;
	S_Array * s_field_825 = s_ga_10110_append_take(s_record_122_get_symbols_take(s_record_122_copy(v_result)), s_SymbolInfo(s_string_retain(v_name), v_kind, s_record_107_get_id_take(s_record_107_copy(v_canonical)), v_mutable, s_record_122_get_scope_depth_take(s_record_122_copy(v_result))));
	s_array_release(v_result.symbols);
	v_result.symbols = s_field_825;
	s_record_122 s_return_826 = s_record_122_copy(v_result);
	s_record_107_drop(v_canonical);
	s_record_122_drop(v_result);
	s_string_release(v_name);
	s_record_122_drop(v_state);
	return s_return_826;
	s_record_107_drop(v_canonical);
	s_record_122_drop(v_result);
}

s_record_122 s_check_type(s_record_122 v_state, int64_t v_id, int64_t v_found, int64_t v_expected)
{
	if (((v_found == v_expected) && (v_found != 0)))
	{
		s_record_122 s_return_827 = s_record_122_copy(v_state);
		s_record_122_drop(v_state);
		return s_return_827;
	}
	s_record_107 v_left = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_state)), s_record_122_get_records_take(s_record_122_copy(v_state)), v_found);
	s_record_107 v_right = s_canonical_kind(s_record_107_get_table_take(s_record_107_copy(v_left)), s_record_122_get_records_take(s_record_122_copy(v_state)), v_expected);
	s_record_122 v_next = s_record_122_copy(v_state);
	s_record_106 s_field_828 = s_record_107_get_table_take(s_record_107_copy(v_right));
	s_record_106_drop(v_next.types);
	v_next.types = s_field_828;
	if (s_hir_types_equal(s_record_107_get_id_take(s_record_107_copy(v_left)), s_record_107_get_id_take(s_record_107_copy(v_right))))
	{
		s_record_122 s_return_829 = s_record_122_copy(v_next);
		s_record_122_drop(v_next);
		s_record_107_drop(v_right);
		s_record_107_drop(v_left);
		s_record_122_drop(v_state);
		return s_return_829;
	}
	S_String * s_left_830 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("expected ", sizeof("expected ") - 1), s_type_name(v_expected)), s_string_from_bytes(", found ", sizeof(", found ") - 1));
	S_String * s_right_831 = s_type_name(v_found);
	s_record_122 s_return_832 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_concat_take(s_left_830, s_right_831));
	s_record_122_drop(v_next);
	s_record_107_drop(v_right);
	s_record_107_drop(v_left);
	s_record_122_drop(v_state);
	return s_return_832;
	s_record_122_drop(v_next);
	s_record_107_drop(v_right);
	s_record_107_drop(v_left);
}

bool s_has_effect(s_record_122 v_state, int64_t v_id, S_Array * v_visited)
{
	s_record_101 v_entry = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if ((((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 41) || (s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 21)) || (s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 34)))
	{
		bool s_return_833 = true;
		s_record_101_drop(v_entry);
		s_array_release(v_visited);
		s_record_122_drop(v_state);
		return s_return_833;
	}
	if (((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 5) && ((s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("/", sizeof("/") - 1)) == 0) || (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("%", sizeof("%") - 1)) == 0))))
	{
		bool s_return_834 = true;
		s_record_101_drop(v_entry);
		s_array_release(v_visited);
		s_record_122_drop(v_state);
		return s_return_834;
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 10))
	{
		const int64_t v_callee = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
		if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee)) != 29))
		{
			bool s_return_835 = true;
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_122_drop(v_state);
			return s_return_835;
		}
		S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee);
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0))
		{
			bool s_return_836 = true;
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_122_drop(v_state);
			return s_return_836;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("sqrt", sizeof("sqrt") - 1)) == 0))
		{
			bool s_return_837 = false;
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_122_drop(v_state);
			return s_return_837;
		}
		const int64_t v_index = s_find_function(s_record_122_copy(v_state), s_string_retain(v_name));
		if ((v_index < 0))
		{
			bool s_return_838 = true;
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_122_drop(v_state);
			return s_return_838;
		}
		bool v_seen = false;
		{
			S_Array * v_s_iter_839 = s_array_retain(v_visited);
			for (int64_t s_index_839 = 0; s_index_839 < (int64_t)s_array_length(v_s_iter_839); ++s_index_839)
			{
				S_String * v_value = s_ga_8_get_borrow(v_s_iter_839, s_index_839);
				if ((s_string_compare_take(s_string_retain(v_value), s_string_retain(v_name)) == 0))
				{
					v_seen = true;
				}
				s_string_release(v_value);
			}
			s_array_release(v_s_iter_839);
		}
		if ((!v_seen))
		{
			s_record_118 v_info = s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_index);
			if (s_has_effect(s_record_122_copy(v_state), s_record_118_get_body_take(s_record_118_copy(v_info)), s_ga_8_append_take(s_array_retain(v_visited), s_string_retain(v_name))))
			{
				bool s_return_840 = true;
				s_record_118_drop(v_info);
				s_string_release(v_name);
				s_record_101_drop(v_entry);
				s_array_release(v_visited);
				s_record_122_drop(v_state);
				return s_return_840;
			}
			s_record_118_drop(v_info);
		}
		s_string_release(v_name);
	}
	for (int64_t v_i = 0, s_bound_841 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_841; ++v_i)
	{
		if (s_has_effect(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i), s_array_retain(v_visited)))
		{
			bool s_return_842 = true;
			s_record_101_drop(v_entry);
			s_array_release(v_visited);
			s_record_122_drop(v_state);
			return s_return_842;
		}
	}
	bool s_return_843 = false;
	s_record_101_drop(v_entry);
	s_array_release(v_visited);
	s_record_122_drop(v_state);
	return s_return_843;
	s_record_101_drop(v_entry);
}

bool s_uses_optional_fallback(s_record_122 v_state, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 5) && (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("or", sizeof("or") - 1)) == 0)))
	{
		const int64_t v_left = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
		s_record_101 v_leaf = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_left);
		if ((s_record_101_get_kind_take(s_record_101_copy(v_leaf)) == 29))
		{
			const int64_t v_index = s_find_symbol(s_record_122_copy(v_state), s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_left));
			if (((v_index >= 0) && s_is_optional(s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_index)))))
			{
				bool s_return_844 = true;
				s_record_101_drop(v_leaf);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_844;
			}
		}
		if ((s_record_101_get_kind_take(s_record_101_copy(v_leaf)) == 10))
		{
			const int64_t v_call = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_left, 0);
			if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_call)) == 4) && (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_call), s_string_from_bytes("get", sizeof("get") - 1)) == 0)))
			{
				bool s_return_845 = true;
				s_record_101_drop(v_leaf);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_845;
			}
		}
		s_record_101_drop(v_leaf);
	}
	for (int64_t v_i = 0, s_bound_846 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_846; ++v_i)
	{
		if (s_uses_optional_fallback(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i)))
		{
			bool s_return_847 = true;
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_847;
		}
	}
	bool s_return_848 = false;
	s_record_101_drop(v_entry);
	s_record_122_drop(v_state);
	return s_return_848;
	s_record_101_drop(v_entry);
}

bool s_ambiguous_order(s_record_122 v_state, int64_t v_a, int64_t v_b)
{
	bool s_return_849 = (s_has_effect(s_record_122_copy(v_state), v_a, s_ga_8_make_take(0, NULL)) && s_has_effect(s_record_122_copy(v_state), v_b, s_ga_8_make_take(0, NULL)));
	s_record_122_drop(v_state);
	return s_return_849;
}

s_record_123 s_specialize_call(s_record_122 v_state, int64_t v_id, s_record_118 v_info, S_Array_i64 * v_kinds, S_Array * v_arguments)
{
	s_record_125 v_signature = s_canonical_signature(s_record_122_copy(v_state), s_array_i64_retain(v_kinds));
	s_record_122 v_next = s_record_122_copy(v_state);
	s_record_106 s_field_850 = s_record_125_get_table_take(s_record_125_copy(v_signature));
	s_record_106_drop(v_next.types);
	v_next.types = s_field_850;
	const int64_t v_existing = s_find_instance(s_record_122_copy(v_next), s_record_118_get_name_take(s_record_118_copy(v_info)), s_record_125_get_ids_take(s_record_125_copy(v_signature)));
	if ((v_existing >= 0))
	{
		s_record_119 v_instance = s_ga_10119_get_checked_take(s_record_122_get_instances_take(s_record_122_copy(v_next)), v_existing);
		if ((s_record_119_get_kind_take(s_record_119_copy(v_instance)) == 0))
		{
			s_record_123 s_return_851 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("recursive generic call requires an explicit return type", sizeof("recursive generic call requires an explicit return type") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_119_drop(v_instance);
			s_record_122_drop(v_next);
			s_record_125_drop(v_signature);
			s_array_release(v_arguments);
			s_array_i64_release(v_kinds);
			s_record_118_drop(v_info);
			s_record_122_drop(v_state);
			return s_return_851;
		}
		s_record_123 s_return_852 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_record_119_get_symbol_take(s_record_119_copy(v_instance)), s_string_from_bytes("(", sizeof("(") - 1)), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_119_get_kind_take(s_record_119_copy(v_instance)));
		s_record_119_drop(v_instance);
		s_record_122_drop(v_next);
		s_record_125_drop(v_signature);
		s_array_release(v_arguments);
		s_array_i64_release(v_kinds);
		s_record_118_drop(v_info);
		s_record_122_drop(v_state);
		return s_return_852;
		s_record_119_drop(v_instance);
	}
	const int64_t v_index = s_generic_array_length_take(s_record_122_get_instances_take(s_record_122_copy(v_next)));
	S_String * v_symbol = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_", sizeof("s_") - 1), s_record_118_get_name_take(s_record_118_copy(v_info))), s_string_from_bytes("__", sizeof("__") - 1)), s_string_from_i64(v_index));
	S_Array * v_params = s_ga_10104_make_take(0, NULL);
	for (int64_t v_i = 0, s_bound_853 = s_array_i64_length_take(s_array_i64_retain(v_kinds)); v_i < s_bound_853; ++v_i)
	{
		s_record_104 s_item_854 = s_ParameterInfo(s_record_104_get_name_take(s_ga_10104_get_checked_take(s_record_118_get_params_take(s_record_118_copy(v_info)), v_i)), s_array_i64_get_checked_take(s_array_i64_retain(v_kinds), v_i));
		v_params = s_ga_10104_append_take(v_params, s_item_854);
	}
	S_Array * s_field_855 = s_ga_10119_append_take(s_record_122_get_instances_take(s_record_122_copy(v_next)), s_InstanceInfo(s_record_118_get_name_take(s_record_118_copy(v_info)), s_array_i64_retain(v_kinds), s_record_125_get_ids_take(s_record_125_copy(v_signature)), s_record_118_get_kind_take(s_record_118_copy(v_info)), s_string_retain(v_symbol), s_string_from_bytes("", sizeof("") - 1), 1));
	s_array_release(v_next.instances);
	v_next.instances = s_field_855;
	S_Array * v_previous_lines = s_record_122_get_lines_take(s_record_122_copy(v_next));
	S_Array * v_previous_symbols = s_record_122_get_symbols_take(s_record_122_copy(v_next));
	const int64_t v_previous_indent = s_record_122_get_indentation_take(s_record_122_copy(v_next));
	const int64_t v_previous_loop = s_record_122_get_loop_depth_take(s_record_122_copy(v_next));
	const int64_t v_previous_loop_scope = s_record_122_get_loop_scope_take(s_record_122_copy(v_next));
	const int64_t v_previous_scope = s_record_122_get_scope_depth_take(s_record_122_copy(v_next));
	const int64_t v_previous_return = s_record_122_get_return_kind_take(s_record_122_copy(v_next));
	S_String * v_previous_name = s_record_122_get_function_name_take(s_record_122_copy(v_next));
	S_Array * v_previous_params = s_record_122_get_active_params_take(s_record_122_copy(v_next));
	S_Array * s_field_856 = s_ga_8_make_take(0, NULL);
	s_array_release(v_next.lines);
	v_next.lines = s_field_856;
	v_next.indentation = 0;
	v_next.loop_depth = 0;
	v_next.loop_scope = 0;
	v_next.scope_depth = 0;
	s_record_122 s_value_857 = s_emit_function(s_record_122_copy(v_next), s_FunctionInfo(s_record_118_get_name_take(s_record_118_copy(v_info)), s_record_118_get_kind_take(s_record_118_copy(v_info)), s_array_retain(v_params), s_record_118_get_body_take(s_record_118_copy(v_info)), false, s_string_retain(v_symbol)));
	s_record_122_drop(v_next);
	v_next = s_value_857;
	S_String * v_body = s_string_from_bytes("", sizeof("") - 1);
	{
		S_Array * v_s_iter_858 = s_record_122_get_lines_take(s_record_122_copy(v_next));
		for (int64_t s_index_858 = 0; s_index_858 < (int64_t)s_array_length(v_s_iter_858); ++s_index_858)
		{
			S_String * v_row = s_ga_8_get_borrow(v_s_iter_858, s_index_858);
			S_String *s_value_859 = s_string_concat_take(s_string_concat_take(s_string_retain(v_body), s_string_retain(v_row)), s_string_from_bytes("\n", sizeof("\n") - 1));
			s_string_release(v_body);
			v_body = s_value_859;
			s_string_release(v_row);
		}
		s_array_release(v_s_iter_858);
	}
	const int64_t v_result = s_record_122_get_return_kind_take(s_record_122_copy(v_next));
	if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))
	{
		s_ga_10119_set_take(&v_next.instances, v_index, s_InstanceInfo(s_record_118_get_name_take(s_record_118_copy(v_info)), s_array_i64_retain(v_kinds), s_record_125_get_ids_take(s_record_125_copy(v_signature)), v_result, s_string_retain(v_symbol), s_string_retain(v_body), 2));
	}
	S_Array * s_field_860 = s_array_retain(v_previous_lines);
	s_array_release(v_next.lines);
	v_next.lines = s_field_860;
	S_Array * s_field_861 = s_array_retain(v_previous_symbols);
	s_array_release(v_next.symbols);
	v_next.symbols = s_field_861;
	v_next.indentation = v_previous_indent;
	v_next.loop_depth = v_previous_loop;
	v_next.loop_scope = v_previous_loop_scope;
	v_next.scope_depth = v_previous_scope;
	v_next.return_kind = v_previous_return;
	S_String * s_field_862 = s_string_retain(v_previous_name);
	s_string_release(v_next.function_name);
	v_next.function_name = s_field_862;
	S_Array * s_field_863 = s_array_retain(v_previous_params);
	s_array_release(v_next.active_params);
	v_next.active_params = s_field_863;
	if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_123 s_return_864 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
		s_string_release(v_body);
		s_array_release(v_previous_params);
		s_string_release(v_previous_name);
		s_array_release(v_previous_symbols);
		s_array_release(v_previous_lines);
		s_array_release(v_params);
		s_string_release(v_symbol);
		s_record_122_drop(v_next);
		s_record_125_drop(v_signature);
		s_array_release(v_arguments);
		s_array_i64_release(v_kinds);
		s_record_118_drop(v_info);
		s_record_122_drop(v_state);
		return s_return_864;
	}
	s_record_123 s_return_865 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_symbol), s_string_from_bytes("(", sizeof("(") - 1)), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments))), s_string_from_bytes(")", sizeof(")") - 1)), v_result);
	s_string_release(v_body);
	s_array_release(v_previous_params);
	s_string_release(v_previous_name);
	s_array_release(v_previous_symbols);
	s_array_release(v_previous_lines);
	s_array_release(v_params);
	s_string_release(v_symbol);
	s_record_122_drop(v_next);
	s_record_125_drop(v_signature);
	s_array_release(v_arguments);
	s_array_i64_release(v_kinds);
	s_record_118_drop(v_info);
	s_record_122_drop(v_state);
	return s_return_865;
	s_string_release(v_body);
	s_array_release(v_previous_params);
	s_string_release(v_previous_name);
	s_array_release(v_previous_symbols);
	s_array_release(v_previous_lines);
	s_array_release(v_params);
	s_string_release(v_symbol);
	s_record_122_drop(v_next);
	s_record_125_drop(v_signature);
}

s_record_122 s_check_hir_call(s_record_122 v_state, int64_t v_id, s_record_111 v_result)
{
	s_record_122 v_next = s_record_122_copy(v_state);
	s_record_106 s_field_866 = s_record_111_get_table_take(s_record_111_copy(v_result));
	s_record_106_drop(v_next.types);
	v_next.types = s_field_866;
	if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_122 s_return_867 = s_emit_error(s_record_122_copy(v_next), v_id, s_record_111_get_error_take(s_record_111_copy(v_result)));
		s_record_122_drop(v_next);
		s_record_111_drop(v_result);
		s_record_122_drop(v_state);
		return s_return_867;
	}
	if ((s_record_111_get_type_id_take(s_record_111_copy(v_result)) != 0))
	{
		S_Array * s_field_868 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_next)), s_HirNode(v_id, 6, s_record_111_get_type_id_take(s_record_111_copy(v_result)), (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_868;
	}
	s_record_122 s_return_869 = s_record_122_copy(v_next);
	s_record_122_drop(v_next);
	s_record_111_drop(v_result);
	s_record_122_drop(v_state);
	return s_return_869;
	s_record_122_drop(v_next);
}

s_record_125 s_call_type_ids(s_record_122 v_state, S_Array_i64 * v_kinds)
{
	s_record_125 s_return_870 = s_canonical_signature(s_record_122_copy(v_state), s_array_i64_retain(v_kinds));
	s_array_i64_release(v_kinds);
	s_record_122_drop(v_state);
	return s_return_870;
}

s_record_123 s_emit_expr(s_record_122 v_state, int64_t v_id)
{
	if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_123 s_return_871 = s_ExprCode(s_record_122_copy(v_state), s_string_from_bytes("", sizeof("") - 1), 0);
		s_record_122_drop(v_state);
		return s_return_871;
	}
	s_record_101 v_entry = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	S_String * v_value = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	const int64_t v_literal = s_resolve_hir_literal(s_record_101_get_kind_take(s_record_101_copy(v_entry)));
	if ((v_literal != 0))
	{
		s_record_122 v_next = s_record_122_copy(v_state);
		S_Array * s_field_872 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_next)), s_HirNode(v_id, 4, v_literal, (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_872;
		if ((v_literal == 4))
		{
			s_record_123 s_return_873 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_from_bytes(", sizeof("s_string_from_bytes(") - 1), s_string_retain(v_value)), s_string_from_bytes(", sizeof(", sizeof(", sizeof(") - 1)), s_string_retain(v_value)), s_string_from_bytes(") - 1)", sizeof(") - 1)") - 1)), 4);
			s_record_122_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_873;
		}
		s_record_123 s_return_874 = s_ExprCode(s_record_122_copy(v_next), s_string_retain(v_value), v_literal);
		s_record_122_drop(v_next);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_874;
		s_record_122_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 2))
	{
		if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) == 0))
		{
			s_record_123 s_return_875 = s_ExprCode(s_record_122_copy(v_state), s_string_from_bytes("s_array_i64_make(0, NULL)", sizeof("s_array_i64_make(0, NULL)") - 1), 7);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_875;
		}
		s_record_122 v_next = s_record_122_copy(v_state);
		S_Array * v_items = s_ga_8_make_take(0, NULL);
		int64_t v_effects = 0;
		int64_t v_element_kind = 0;
		for (int64_t v_i = 0, s_bound_876 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_876; ++v_i)
		{
			if (s_has_effect(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i), s_ga_8_make_take(0, NULL)))
			{
				v_effects += 1;
			}
			s_record_123 v_element = s_emit_expr(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i));
			s_record_122 s_value_877 = s_record_123_get_state_take(s_record_123_copy(v_element));
			s_record_122_drop(v_next);
			v_next = s_value_877;
			if ((v_i == 0))
			{
				v_element_kind = s_record_123_get_kind_take(s_record_123_copy(v_element));
			}
			else
			{
				s_record_122 s_value_878 = s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_element)), v_element_kind);
				s_record_122_drop(v_next);
				v_next = s_value_878;
			}
			S_String * s_item_879 = s_record_123_get_text_take(s_record_123_copy(v_element));
			v_items = s_ga_8_append_take(v_items, s_item_879);
			s_record_123_drop(v_element);
		}
		if ((v_effects > 1))
		{
			s_record_123 s_return_880 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("array initializer requires ordered evaluation", sizeof("array initializer requires ordered evaluation") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_release(v_items);
			s_record_122_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_880;
		}
		const int64_t v_kind = s_array_of(v_element_kind);
		if ((v_kind == 0))
		{
			s_record_123 s_return_881 = s_ExprCode(s_check_type(s_record_122_copy(v_next), v_id, v_element_kind, 1), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_release(v_items);
			s_record_122_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_881;
		}
		if ((v_kind == 7))
		{
			S_String * s_left_882 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_make(", sizeof("s_array_i64_make(") - 1), s_string_from_i64(s_record_101_get_count_take(s_record_101_copy(v_entry)))), s_string_from_bytes(", (int64_t[]){", sizeof(", (int64_t[]){") - 1));
			S_String * s_right_883 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_items));
			s_record_123 s_return_884 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_left_882, s_right_883), s_string_from_bytes("})", sizeof("})") - 1)), v_kind);
			s_array_release(v_items);
			s_record_122_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_884;
		}
		S_String * s_left_885 = s_string_concat_take(s_array_helper(v_kind), s_string_from_bytes("_make_take(", sizeof("_make_take(") - 1));
		S_String * s_right_886 = s_string_from_i64(s_record_101_get_count_take(s_record_101_copy(v_entry)));
		S_String * s_left_887 = s_string_concat_take(s_string_concat_take(s_left_885, s_right_886), s_string_from_bytes(", (", sizeof(", (") - 1));
		S_String * s_right_888 = s_c_type(v_element_kind);
		S_String * s_left_889 = s_string_concat_take(s_string_concat_take(s_left_887, s_right_888), s_string_from_bytes("[]){", sizeof("[]){") - 1));
		S_String * s_right_890 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_items));
		s_record_123 s_return_891 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_left_889, s_right_890), s_string_from_bytes("})", sizeof("})") - 1)), v_kind);
		s_array_release(v_items);
		s_record_122_drop(v_next);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_891;
		s_array_release(v_items);
		s_record_122_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 12))
	{
		if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) != 2))
		{
			s_record_123 s_return_892 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("only rank-1 indexing is supported", sizeof("only rank-1 indexing is supported") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_892;
		}
		s_record_123 v_array = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		s_record_123 v_index = s_emit_expr(s_record_123_get_state_take(s_record_123_copy(v_array)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
		s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_index));
		const int64_t v_element = s_array_element(s_record_123_get_kind_take(s_record_123_copy(v_array)));
		if ((v_element == 0))
		{
			s_record_123 s_return_893 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("checked indexing requires an array", sizeof("checked indexing requires an array") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_122_drop(v_next);
			s_record_123_drop(v_index);
			s_record_123_drop(v_array);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_893;
		}
		s_record_122 s_value_894 = s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_index)), 1);
		s_record_122_drop(v_next);
		v_next = s_value_894;
		if ((s_record_123_get_kind_take(s_record_123_copy(v_array)) == 7))
		{
			s_record_123 s_return_895 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_get_checked_take(", sizeof("s_array_i64_get_checked_take(") - 1), s_record_123_get_text_take(s_record_123_copy(v_array))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_index))), s_string_from_bytes(")", sizeof(")") - 1)), 1);
			s_record_122_drop(v_next);
			s_record_123_drop(v_index);
			s_record_123_drop(v_array);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_895;
		}
		s_record_123 s_return_896 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(s_record_123_get_kind_take(s_record_123_copy(v_array))), s_string_from_bytes("_get_checked_take(", sizeof("_get_checked_take(") - 1)), s_record_123_get_text_take(s_record_123_copy(v_array))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_index))), s_string_from_bytes(")", sizeof(")") - 1)), v_element);
		s_record_122_drop(v_next);
		s_record_123_drop(v_index);
		s_record_123_drop(v_array);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_896;
		s_record_122_drop(v_next);
		s_record_123_drop(v_index);
		s_record_123_drop(v_array);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 30))
	{
		if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) != 2))
		{
			s_record_123 s_return_897 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("only rank-1 indexing is supported", sizeof("only rank-1 indexing is supported") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_897;
		}
		s_record_123 v_array = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		s_record_123 v_index = s_emit_expr(s_record_123_get_state_take(s_record_123_copy(v_array)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
		s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_index));
		const int64_t v_element = s_array_element(s_record_123_get_kind_take(s_record_123_copy(v_array)));
		if ((v_element == 0))
		{
			s_record_123 s_return_898 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("optional indexing requires an array", sizeof("optional indexing requires an array") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_122_drop(v_next);
			s_record_123_drop(v_index);
			s_record_123_drop(v_array);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_898;
		}
		s_record_122 s_value_899 = s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_index)), 1);
		s_record_122_drop(v_next);
		v_next = s_value_899;
		if ((s_record_123_get_kind_take(s_record_123_copy(v_array)) == 7))
		{
			s_record_123 s_return_900 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_get_optional_take(", sizeof("s_array_i64_get_optional_take(") - 1), s_record_123_get_text_take(s_record_123_copy(v_array))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_index))), s_string_from_bytes(")", sizeof(")") - 1)), 6);
			s_record_122_drop(v_next);
			s_record_123_drop(v_index);
			s_record_123_drop(v_array);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_900;
		}
		s_record_123 s_return_901 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(s_record_123_get_kind_take(s_record_123_copy(v_array))), s_string_from_bytes("_get_optional_take(", sizeof("_get_optional_take(") - 1)), s_record_123_get_text_take(s_record_123_copy(v_array))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_index))), s_string_from_bytes(")", sizeof(")") - 1)), s_optional_of(v_element));
		s_record_122_drop(v_next);
		s_record_123_drop(v_index);
		s_record_123_drop(v_array);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_901;
		s_record_122_drop(v_next);
		s_record_123_drop(v_index);
		s_record_123_drop(v_array);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 44))
	{
		s_record_123 v_unboxed = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		if ((!s_is_optional(s_record_123_get_kind_take(s_record_123_copy(v_unboxed)))))
		{
			s_record_123 s_return_902 = s_ExprCode(s_emit_error(s_record_123_get_state_take(s_record_123_copy(v_unboxed)), v_id, s_string_from_bytes("unboxing requires an Optional", sizeof("unboxing requires an Optional") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_123_drop(v_unboxed);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_902;
		}
		s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_unboxed));
		S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_unwrap_", sizeof("s_unwrap_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
		v_next.counter += 1;
		s_record_122 s_value_903 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_unboxed))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_unboxed))), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_122_drop(v_next);
		v_next = s_value_903;
		s_record_122 s_value_904 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (!", sizeof("if (!") - 1), s_string_retain(v_temp)), s_string_from_bytes(".has_value) s_panic(\"missing optional value\");", sizeof(".has_value) s_panic(\"missing optional value\");") - 1)));
		s_record_122_drop(v_next);
		v_next = s_value_904;
		s_record_123 s_return_905 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_retain(v_temp), s_string_from_bytes(".value", sizeof(".value") - 1)), s_optional_value(s_record_123_get_kind_take(s_record_123_copy(v_unboxed))));
		s_string_release(v_temp);
		s_record_122_drop(v_next);
		s_record_123_drop(v_unboxed);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_905;
		s_string_release(v_temp);
		s_record_122_drop(v_next);
		s_record_123_drop(v_unboxed);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 33))
	{
		s_record_122 v_next = s_record_122_copy(v_state);
		S_Array * v_fields = s_ga_10104_make_take(0, NULL);
		S_Array * v_semantic_fields = s_ga_10104_make_take(0, NULL);
		S_Array * v_initializers = s_ga_8_make_take(0, NULL);
		int64_t v_effects = 0;
		S_String * v_touches = s_string_from_bytes("", sizeof("") - 1);
		for (int64_t v_i = 0, s_bound_906 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_906; ++v_i)
		{
			const int64_t v_field_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i);
			s_record_101 v_field = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_field_id);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 36))
			{
				S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_field_id);
				const int64_t v_symbol = s_find_symbol(s_record_122_copy(v_next), s_string_retain(v_name));
				if ((v_symbol < 0))
				{
					s_record_123 s_return_907 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_field_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown spread source '", sizeof("unknown spread source '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_string_release(v_touches);
					s_array_release(v_initializers);
					s_array_release(v_semantic_fields);
					s_array_release(v_fields);
					s_record_122_drop(v_next);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_122_drop(v_state);
					return s_return_907;
				}
				S_String *s_value_908 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_touches), s_string_from_bytes("(void)v_", sizeof("(void)v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1));
				s_string_release(v_touches);
				v_touches = s_value_908;
				const int64_t v_kind = s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)), v_symbol));
				s_record_107 v_owner = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_next)), s_record_122_get_records_take(s_record_122_copy(v_next)), v_kind);
				s_record_106 s_field_909 = s_record_107_get_table_take(s_record_107_copy(v_owner));
				s_record_106_drop(v_next.types);
				v_next.types = s_field_909;
				s_record_112 v_edit = s_hir_record_spread(s_record_122_get_types_take(s_record_122_copy(v_next)), s_array_retain(v_semantic_fields), s_record_107_get_id_take(s_record_107_copy(v_owner)));
				if ((s_string_compare_take(s_record_112_get_error_take(s_record_112_copy(v_edit)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_123 s_return_910 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_field_id, s_record_112_get_error_take(s_record_112_copy(v_edit))), s_string_from_bytes("", sizeof("") - 1), 0);
					s_record_112_drop(v_edit);
					s_record_107_drop(v_owner);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_string_release(v_touches);
					s_array_release(v_initializers);
					s_array_release(v_semantic_fields);
					s_array_release(v_fields);
					s_record_122_drop(v_next);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_122_drop(v_state);
					return s_return_910;
				}
				S_Array * s_value_911 = s_record_112_get_fields_take(s_record_112_copy(v_edit));
				s_array_release(v_semantic_fields);
				v_semantic_fields = s_value_911;
				{
					S_Array * v_s_iter_912 = s_record_121_get_fields_take(s_ga_10121_get_checked_take(s_record_122_get_records_take(s_record_122_copy(v_next)), (v_kind - 100)));
					for (int64_t s_index_912 = 0; s_index_912 < (int64_t)s_array_length(v_s_iter_912); ++s_index_912)
					{
						s_record_104 v_member = s_ga_10104_get_borrow(v_s_iter_912, s_index_912);
						int64_t v_found = (-1);
						for (int64_t v_j = 0, s_bound_913 = s_generic_array_length_take(s_array_retain(v_fields)); v_j < s_bound_913; ++v_j)
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
							s_record_104 s_item_914 = s_record_104_copy(v_member);
							v_fields = s_ga_10104_append_take(v_fields, s_item_914);
							S_String * s_item_915 = s_string_retain(v_code);
							v_initializers = s_ga_8_append_take(v_initializers, s_item_915);
						}
						s_string_release(v_code);
						s_record_104_drop(v_member);
					}
					s_array_release(v_s_iter_912);
				}
				s_record_112_drop(v_edit);
				s_record_107_drop(v_owner);
				s_string_release(v_name);
			}
			else if ((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 15))
			{
				S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_field_id);
				s_record_112 v_edit = s_hir_record_delete(s_array_retain(v_semantic_fields), s_string_retain(v_name));
				if ((s_string_compare_take(s_record_112_get_error_take(s_record_112_copy(v_edit)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_123 s_return_916 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_field_id, s_record_112_get_error_take(s_record_112_copy(v_edit))), s_string_from_bytes("", sizeof("") - 1), 0);
					s_record_112_drop(v_edit);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_string_release(v_touches);
					s_array_release(v_initializers);
					s_array_release(v_semantic_fields);
					s_array_release(v_fields);
					s_record_122_drop(v_next);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_122_drop(v_state);
					return s_return_916;
				}
				S_Array * s_value_917 = s_record_112_get_fields_take(s_record_112_copy(v_edit));
				s_array_release(v_semantic_fields);
				v_semantic_fields = s_value_917;
				S_Array * v_kept = s_ga_10104_make_take(0, NULL);
				S_Array * v_values = s_ga_8_make_take(0, NULL);
				for (int64_t v_j = 0, s_bound_918 = s_generic_array_length_take(s_array_retain(v_fields)); v_j < s_bound_918; ++v_j)
				{
					if ((v_j != s_record_112_get_index_take(s_record_112_copy(v_edit))))
					{
						s_record_104 s_item_919 = s_ga_10104_get_checked_take(s_array_retain(v_fields), v_j);
						v_kept = s_ga_10104_append_take(v_kept, s_item_919);
						S_String * s_item_920 = s_ga_8_get_checked_take(s_array_retain(v_initializers), v_j);
						v_values = s_ga_8_append_take(v_values, s_item_920);
					}
				}
				S_Array * s_value_921 = s_array_retain(v_kept);
				s_array_release(v_fields);
				v_fields = s_value_921;
				S_Array * s_value_922 = s_array_retain(v_values);
				s_array_release(v_initializers);
				v_initializers = s_value_922;
				s_array_release(v_values);
				s_array_release(v_kept);
				s_record_112_drop(v_edit);
				s_string_release(v_name);
			}
			else if ((s_record_101_get_kind_take(s_record_101_copy(v_field)) == 19))
			{
				S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_field_id);
				const int64_t v_value_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_field_id, 0);
				if (s_has_effect(s_record_122_copy(v_next), v_value_id, s_ga_8_make_take(0, NULL)))
				{
					v_effects += 1;
				}
				s_record_123 v_result = s_emit_expr(s_record_122_copy(v_next), v_value_id);
				s_record_122 s_value_923 = s_record_123_get_state_take(s_record_123_copy(v_result));
				s_record_122_drop(v_next);
				v_next = s_value_923;
				s_record_107 v_canonical = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_next)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_123_get_kind_take(s_record_123_copy(v_result)));
				s_record_106 s_field_924 = s_record_107_get_table_take(s_record_107_copy(v_canonical));
				s_record_106_drop(v_next.types);
				v_next.types = s_field_924;
				s_record_112 v_edit = s_hir_record_put(s_array_retain(v_semantic_fields), s_string_retain(v_name), s_record_107_get_id_take(s_record_107_copy(v_canonical)));
				if ((s_string_compare_take(s_record_112_get_error_take(s_record_112_copy(v_edit)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_123 s_return_925 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_field_id, s_record_112_get_error_take(s_record_112_copy(v_edit))), s_string_from_bytes("", sizeof("") - 1), 0);
					s_record_112_drop(v_edit);
					s_record_107_drop(v_canonical);
					s_record_123_drop(v_result);
					s_string_release(v_name);
					s_record_101_drop(v_field);
					s_string_release(v_touches);
					s_array_release(v_initializers);
					s_array_release(v_semantic_fields);
					s_array_release(v_fields);
					s_record_122_drop(v_next);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_122_drop(v_state);
					return s_return_925;
				}
				S_Array * s_value_926 = s_record_112_get_fields_take(s_record_112_copy(v_edit));
				s_array_release(v_semantic_fields);
				v_semantic_fields = s_value_926;
				s_record_104 v_member = s_RecordField(s_string_retain(v_name), s_record_123_get_kind_take(s_record_123_copy(v_result)));
				S_String * v_code = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes(".", sizeof(".") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result)));
				if ((s_record_112_get_index_take(s_record_112_copy(v_edit)) >= 0))
				{
					s_ga_10104_set_take(&v_fields, s_record_112_get_index_take(s_record_112_copy(v_edit)), s_record_104_copy(v_member));
					s_ga_8_set_take(&v_initializers, s_record_112_get_index_take(s_record_112_copy(v_edit)), s_string_retain(v_code));
				}
				else
				{
					s_record_104 s_item_927 = s_record_104_copy(v_member);
					v_fields = s_ga_10104_append_take(v_fields, s_item_927);
					S_String * s_item_928 = s_string_retain(v_code);
					v_initializers = s_ga_8_append_take(v_initializers, s_item_928);
				}
				s_string_release(v_code);
				s_record_104_drop(v_member);
				s_record_112_drop(v_edit);
				s_record_107_drop(v_canonical);
				s_record_123_drop(v_result);
				s_string_release(v_name);
			}
			else
			{
				s_record_123 s_return_929 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_field_id, s_string_from_bytes("invalid record field", sizeof("invalid record field") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_101_drop(v_field);
				s_string_release(v_touches);
				s_array_release(v_initializers);
				s_array_release(v_semantic_fields);
				s_array_release(v_fields);
				s_record_122_drop(v_next);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_929;
			}
			s_record_101_drop(v_field);
		}
		if (((v_effects > 0) && (s_record_101_get_count_take(s_record_101_copy(v_entry)) > 1)))
		{
			s_record_123 s_return_930 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("record initializer requires ordered evaluation", sizeof("record initializer requires ordered evaluation") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_touches);
			s_array_release(v_initializers);
			s_array_release(v_semantic_fields);
			s_array_release(v_fields);
			s_record_122_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_930;
		}
		s_record_111 v_semantic = s_resolve_hir_record(s_record_122_get_types_take(s_record_122_copy(v_next)), s_array_retain(v_semantic_fields));
		if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_semantic)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_123 s_return_931 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_record_111_get_error_take(s_record_111_copy(v_semantic))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_111_drop(v_semantic);
			s_string_release(v_touches);
			s_array_release(v_initializers);
			s_array_release(v_semantic_fields);
			s_array_release(v_fields);
			s_record_122_drop(v_next);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_931;
		}
		s_record_106 s_field_932 = s_record_111_get_table_take(s_record_111_copy(v_semantic));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_932;
		s_record_123 v_record = s_intern_record(s_record_122_copy(v_next), s_array_retain(v_fields));
		s_record_122 v_typed = s_record_123_get_state_take(s_record_123_copy(v_record));
		S_Array * s_field_933 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_typed)), s_HirNode(v_id, 7, s_record_111_get_type_id_take(s_record_111_copy(v_semantic)), (-1)));
		s_array_release(v_typed.hir);
		v_typed.hir = s_field_933;
		S_String * s_left_934 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_string_retain(v_touches)), s_string_from_bytes("((", sizeof("((") - 1)), s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_record)))), s_string_from_bytes("){ ", sizeof("){ ") - 1));
		S_String * s_right_935 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_initializers));
		s_record_123 s_return_936 = s_ExprCode(s_record_122_copy(v_typed), s_string_concat_take(s_string_concat_take(s_left_934, s_right_935), s_string_from_bytes(" }))", sizeof(" }))") - 1)), s_record_123_get_kind_take(s_record_123_copy(v_record)));
		s_record_122_drop(v_typed);
		s_record_123_drop(v_record);
		s_record_111_drop(v_semantic);
		s_string_release(v_touches);
		s_array_release(v_initializers);
		s_array_release(v_semantic_fields);
		s_array_release(v_fields);
		s_record_122_drop(v_next);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_936;
		s_record_122_drop(v_typed);
		s_record_123_drop(v_record);
		s_record_111_drop(v_semantic);
		s_string_release(v_touches);
		s_array_release(v_initializers);
		s_array_release(v_semantic_fields);
		s_array_release(v_fields);
		s_record_122_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 4))
	{
		s_record_123 v_receiver = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		s_record_107 v_owner = s_canonical_kind(s_record_122_get_types_take(s_record_123_get_state_take(s_record_123_copy(v_receiver))), s_record_122_get_records_take(s_record_123_get_state_take(s_record_123_copy(v_receiver))), s_record_123_get_kind_take(s_record_123_copy(v_receiver)));
		s_record_109 v_resolved = s_resolve_hir_field(s_record_107_get_table_take(s_record_107_copy(v_owner)), s_record_107_get_id_take(s_record_107_copy(v_owner)), s_string_retain(v_value));
		s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_receiver));
		s_record_106 s_field_937 = s_record_107_get_table_take(s_record_107_copy(v_owner));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_937;
		if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_123 s_return_938 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_record_109_get_error_take(s_record_109_copy(v_resolved))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_122_drop(v_next);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_owner);
			s_record_123_drop(v_receiver);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_938;
		}
		s_record_104 v_field = s_ga_10104_get_checked_take(s_record_121_get_fields_take(s_ga_10121_get_checked_take(s_record_122_get_records_take(s_record_122_copy(v_next)), (s_record_123_get_kind_take(s_record_123_copy(v_receiver)) - 100))), s_record_109_get_field_index_take(s_record_109_copy(v_resolved)));
		S_Array * s_field_939 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_next)), s_HirNode(v_id, 1, s_record_109_get_type_id_take(s_record_109_copy(v_resolved)), s_record_109_get_field_index_take(s_record_109_copy(v_resolved))));
		s_array_release(v_next.hir);
		v_next.hir = s_field_939;
		s_record_123 s_return_940 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_receiver))), s_string_from_bytes("_get_", sizeof("_get_") - 1)), s_record_104_get_name_take(s_record_104_copy(v_field))), s_string_from_bytes("_take(", sizeof("_take(") - 1)), s_record_123_get_text_take(s_record_123_copy(v_receiver))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_104_get_kind_take(s_record_104_copy(v_field)));
		s_record_104_drop(v_field);
		s_record_122_drop(v_next);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_owner);
		s_record_123_drop(v_receiver);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_940;
		s_record_104_drop(v_field);
		s_record_122_drop(v_next);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_owner);
		s_record_123_drop(v_receiver);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 29))
	{
		s_record_109 v_binding = s_resolve_hir_name(s_record_122_get_symbols_take(s_record_122_copy(v_state)), s_string_retain(v_value));
		if ((s_record_109_get_field_index_take(s_record_109_copy(v_binding)) >= 0))
		{
			s_record_122 v_next = s_record_122_copy(v_state);
			S_Array * s_field_941 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_next)), s_HirNode(v_id, 3, s_record_109_get_type_id_take(s_record_109_copy(v_binding)), s_record_109_get_field_index_take(s_record_109_copy(v_binding))));
			s_array_release(v_next.hir);
			v_next.hir = s_field_941;
			const int64_t v_kind = s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)), s_record_109_get_field_index_take(s_record_109_copy(v_binding))));
			S_String * v_copied = s_record_copy(v_kind, s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_value)));
			s_record_123 s_return_942 = s_ExprCode(s_record_122_copy(v_next), s_string_retain(v_copied), v_kind);
			s_string_release(v_copied);
			s_record_122_drop(v_next);
			s_record_109_drop(v_binding);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_942;
			s_string_release(v_copied);
			s_record_122_drop(v_next);
		}
		S_String * v_constant = s_find_constant(s_record_122_copy(v_state), s_string_retain(v_value));
		if ((s_string_compare_take(s_string_retain(v_constant), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_123 s_return_943 = s_ExprCode(s_record_122_copy(v_state), s_string_retain(v_constant), 1);
			s_string_release(v_constant);
			s_record_109_drop(v_binding);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_943;
		}
		s_record_123 s_return_944 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown name '", sizeof("unknown name '") - 1), s_string_retain(v_value)), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
		s_string_release(v_constant);
		s_record_109_drop(v_binding);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_944;
		s_string_release(v_constant);
		s_record_109_drop(v_binding);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 39))
	{
		s_record_123 v_arg = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		s_record_107 v_type_id = s_canonical_kind(s_record_122_get_types_take(s_record_123_get_state_take(s_record_123_copy(v_arg))), s_record_122_get_records_take(s_record_123_get_state_take(s_record_123_copy(v_arg))), s_record_123_get_kind_take(s_record_123_copy(v_arg)));
		s_record_109 v_resolved = s_resolve_hir_unary(s_record_107_get_table_take(s_record_107_copy(v_type_id)), s_record_107_get_id_take(s_record_107_copy(v_type_id)), s_string_retain(v_value));
		s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_arg));
		s_record_106 s_field_945 = s_record_107_get_table_take(s_record_107_copy(v_type_id));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_945;
		if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			if ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("not", sizeof("not") - 1)) == 0))
			{
				s_record_123 s_return_946 = s_ExprCode(s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_arg)), 3), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_122_drop(v_next);
				s_record_109_drop(v_resolved);
				s_record_107_drop(v_type_id);
				s_record_123_drop(v_arg);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_946;
			}
			s_record_123 s_return_947 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_record_109_get_error_take(s_record_109_copy(v_resolved))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_122_drop(v_next);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_type_id);
			s_record_123_drop(v_arg);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_947;
		}
		S_Array * s_field_948 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_next)), s_HirNode(v_id, 5, s_record_109_get_type_id_take(s_record_109_copy(v_resolved)), (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_948;
		if ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("not", sizeof("not") - 1)) == 0))
		{
			s_record_123 s_return_949 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("(!", sizeof("(!") - 1), s_record_123_get_text_take(s_record_123_copy(v_arg))), s_string_from_bytes(")", sizeof(")") - 1)), 3);
			s_record_122_drop(v_next);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_type_id);
			s_record_123_drop(v_arg);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_949;
		}
		s_record_123 s_return_950 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_string_retain(v_value)), s_record_123_get_text_take(s_record_123_copy(v_arg))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_123_get_kind_take(s_record_123_copy(v_arg)));
		s_record_122_drop(v_next);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_type_id);
		s_record_123_drop(v_arg);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_950;
		s_record_122_drop(v_next);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_type_id);
		s_record_123_drop(v_arg);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 5))
	{
		const int64_t v_left_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
		const int64_t v_right_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1);
		const bool v_needs_order = (((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("and", sizeof("and") - 1)) != 0) && (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("or", sizeof("or") - 1)) != 0)) && s_ambiguous_order(s_record_122_copy(v_state), v_left_id, v_right_id));
		s_record_123 v_left = s_emit_expr(s_record_122_copy(v_state), v_left_id);
		if (v_needs_order)
		{
			s_record_122 v_ordered = s_record_123_get_state_take(s_record_123_copy(v_left));
			S_String * v_name = s_string_concat_take(s_string_from_bytes("s_left_", sizeof("s_left_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_ordered))));
			v_ordered.counter += 1;
			s_record_122 s_value_951 = s_line(s_record_122_copy(v_ordered), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_left))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_left))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_ordered);
			v_ordered = s_value_951;
			s_record_123 s_value_952 = s_ExprCode(s_record_122_copy(v_ordered), s_string_retain(v_name), s_record_123_get_kind_take(s_record_123_copy(v_left)));
			s_record_123_drop(v_left);
			v_left = s_value_952;
			s_string_release(v_name);
			s_record_122_drop(v_ordered);
		}
		if (((((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("and", sizeof("and") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("or", sizeof("or") - 1)) == 0)) && (s_record_123_get_kind_take(s_record_123_copy(v_left)) == 3)) && s_uses_optional_fallback(s_record_122_copy(v_state), v_right_id)))
		{
			s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_left));
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_bool_", sizeof("s_bool_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
			v_next.counter += 1;
			s_record_122 s_value_953 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("bool ", sizeof("bool ") - 1), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_left))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_953;
			S_String * v_test = s_string_retain(v_temp);
			if ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("or", sizeof("or") - 1)) == 0))
			{
				S_String *s_value_954 = s_string_concat_take(s_string_from_bytes("!", sizeof("!") - 1), s_string_retain(v_temp));
				s_string_release(v_test);
				v_test = s_value_954;
			}
			s_record_122 s_value_955 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (", sizeof("if (") - 1), s_string_retain(v_test)), s_string_from_bytes(") {", sizeof(") {") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_955;
			v_next.indentation += 1;
			s_record_123 v_right = s_emit_expr(s_record_122_copy(v_next), v_right_id);
			s_record_122 s_value_956 = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_right)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_right)), 3);
			s_record_122_drop(v_next);
			v_next = s_value_956;
			if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_123 s_return_957 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_123_drop(v_right);
				s_string_release(v_test);
				s_string_release(v_temp);
				s_record_122_drop(v_next);
				s_record_123_drop(v_left);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_957;
			}
			s_record_122 s_value_958 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_temp), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_right))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_958;
			v_next.indentation -= 1;
			s_record_122 s_value_959 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_122_drop(v_next);
			v_next = s_value_959;
			s_record_123 s_return_960 = s_ExprCode(s_record_122_copy(v_next), s_string_retain(v_temp), 3);
			s_record_123_drop(v_right);
			s_string_release(v_test);
			s_string_release(v_temp);
			s_record_122_drop(v_next);
			s_record_123_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_960;
			s_record_123_drop(v_right);
			s_string_release(v_test);
			s_string_release(v_temp);
			s_record_122_drop(v_next);
		}
		if (((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("or", sizeof("or") - 1)) == 0) && s_is_optional(s_record_123_get_kind_take(s_record_123_copy(v_left)))))
		{
			const int64_t v_kind = s_optional_value(s_record_123_get_kind_take(s_record_123_copy(v_left)));
			s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_left));
			const int64_t v_number = s_record_122_get_counter_take(s_record_122_copy(v_next));
			v_next.counter += 1;
			S_String * v_option = s_string_concat_take(s_string_from_bytes("s_option_", sizeof("s_option_") - 1), s_string_from_i64(v_number));
			S_String * v_result = s_string_concat_take(s_string_from_bytes("s_fallback_", sizeof("s_fallback_") - 1), s_string_from_i64(v_number));
			s_record_122 s_value_961 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_left))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_option)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_left))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_961;
			s_record_122 s_value_962 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(v_kind), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_result)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_962;
			s_record_122 s_value_963 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (", sizeof("if (") - 1), s_string_retain(v_option)), s_string_from_bytes(".has_value) {", sizeof(".has_value) {") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_963;
			v_next.indentation += 1;
			s_record_122 s_value_964 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_option)), s_string_from_bytes(".value;", sizeof(".value;") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_964;
			v_next.indentation -= 1;
			s_record_122 s_value_965 = s_line(s_record_122_copy(v_next), s_string_from_bytes("} else {", sizeof("} else {") - 1));
			s_record_122_drop(v_next);
			v_next = s_value_965;
			v_next.indentation += 1;
			s_record_123 v_right = s_emit_expr(s_record_122_copy(v_next), v_right_id);
			s_record_122 s_value_966 = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_right)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_right)), v_kind);
			s_record_122_drop(v_next);
			v_next = s_value_966;
			if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_123 s_return_967 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_123_drop(v_right);
				s_string_release(v_result);
				s_string_release(v_option);
				s_record_122_drop(v_next);
				s_record_123_drop(v_left);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_967;
			}
			s_record_122 s_value_968 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_result), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_right))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_968;
			v_next.indentation -= 1;
			s_record_122 s_value_969 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_122_drop(v_next);
			v_next = s_value_969;
			s_record_123 s_return_970 = s_ExprCode(s_record_122_copy(v_next), s_string_retain(v_result), v_kind);
			s_record_123_drop(v_right);
			s_string_release(v_result);
			s_string_release(v_option);
			s_record_122_drop(v_next);
			s_record_123_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_970;
			s_record_123_drop(v_right);
			s_string_release(v_result);
			s_string_release(v_option);
			s_record_122_drop(v_next);
		}
		s_record_123 v_right = s_emit_expr(s_record_123_get_state_take(s_record_123_copy(v_left)), v_right_id);
		if (v_needs_order)
		{
			s_record_122 v_ordered = s_record_123_get_state_take(s_record_123_copy(v_right));
			S_String * v_name = s_string_concat_take(s_string_from_bytes("s_right_", sizeof("s_right_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_ordered))));
			v_ordered.counter += 1;
			s_record_122 s_value_971 = s_line(s_record_122_copy(v_ordered), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_right))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_right))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_ordered);
			v_ordered = s_value_971;
			s_record_123 s_value_972 = s_ExprCode(s_record_122_copy(v_ordered), s_string_retain(v_name), s_record_123_get_kind_take(s_record_123_copy(v_right)));
			s_record_123_drop(v_right);
			v_right = s_value_972;
			s_string_release(v_name);
			s_record_122_drop(v_ordered);
		}
		s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_right));
		if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_123 s_return_973 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_122_drop(v_next);
			s_record_123_drop(v_right);
			s_record_123_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_973;
		}
		s_record_107 v_left_type = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_next)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_123_get_kind_take(s_record_123_copy(v_left)));
		s_record_107 v_right_type = s_canonical_kind(s_record_107_get_table_take(s_record_107_copy(v_left_type)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_123_get_kind_take(s_record_123_copy(v_right)));
		s_record_109 v_resolved = s_resolve_hir_binary(s_record_107_get_table_take(s_record_107_copy(v_right_type)), s_record_107_get_id_take(s_record_107_copy(v_left_type)), s_record_107_get_id_take(s_record_107_copy(v_right_type)), s_string_retain(v_value));
		s_record_106 s_field_974 = s_record_107_get_table_take(s_record_107_copy(v_right_type));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_974;
		if ((s_string_compare_take(s_record_109_get_error_take(s_record_109_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_123 s_return_975 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_record_109_get_error_take(s_record_109_copy(v_resolved))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_right_type);
			s_record_107_drop(v_left_type);
			s_record_122_drop(v_next);
			s_record_123_drop(v_right);
			s_record_123_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_975;
		}
		S_Array * s_field_976 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_next)), s_HirNode(v_id, 2, s_record_109_get_type_id_take(s_record_109_copy(v_resolved)), (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_976;
		S_String * v_op = s_string_retain(v_value);
		int64_t v_kind = s_record_123_get_kind_take(s_record_123_copy(v_left));
		if ((s_record_109_get_type_id_take(s_record_109_copy(v_resolved)) == 3))
		{
			v_kind = 3;
		}
		if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("and", sizeof("and") - 1)) == 0))
		{
			S_String *s_value_977 = s_string_from_bytes("&&", sizeof("&&") - 1);
			s_string_release(v_op);
			v_op = s_value_977;
		}
		else if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("or", sizeof("or") - 1)) == 0))
		{
			S_String *s_value_978 = s_string_from_bytes("||", sizeof("||") - 1);
			s_string_release(v_op);
			v_op = s_value_978;
		}
		if (((s_record_123_get_kind_take(s_record_123_copy(v_left)) == 4) && (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("+", sizeof("+") - 1)) == 0)))
		{
			s_record_123 s_return_979 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_concat_take(", sizeof("s_string_concat_take(") - 1), s_record_123_get_text_take(s_record_123_copy(v_left))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_right))), s_string_from_bytes(")", sizeof(")") - 1)), 4);
			s_string_release(v_op);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_right_type);
			s_record_107_drop(v_left_type);
			s_record_122_drop(v_next);
			s_record_123_drop(v_right);
			s_record_123_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_979;
		}
		if (((s_record_123_get_kind_take(s_record_123_copy(v_left)) == 4) && ((((((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("==", sizeof("==") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("!=", sizeof("!=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<", sizeof("<") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("<=", sizeof("<=") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">", sizeof(">") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_op), s_string_from_bytes(">=", sizeof(">=") - 1)) == 0))))
		{
			s_record_123 s_return_980 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(s_string_compare_take(", sizeof("(s_string_compare_take(") - 1), s_record_123_get_text_take(s_record_123_copy(v_left))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_right))), s_string_from_bytes(") ", sizeof(") ") - 1)), s_string_retain(v_op)), s_string_from_bytes(" 0)", sizeof(" 0)") - 1)), 3);
			s_string_release(v_op);
			s_record_109_drop(v_resolved);
			s_record_107_drop(v_right_type);
			s_record_107_drop(v_left_type);
			s_record_122_drop(v_next);
			s_record_123_drop(v_right);
			s_record_123_drop(v_left);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_980;
		}
		s_record_123 s_return_981 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_record_123_get_text_take(s_record_123_copy(v_left))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_op)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_right))), s_string_from_bytes(")", sizeof(")") - 1)), v_kind);
		s_string_release(v_op);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_right_type);
		s_record_107_drop(v_left_type);
		s_record_122_drop(v_next);
		s_record_123_drop(v_right);
		s_record_123_drop(v_left);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_981;
		s_string_release(v_op);
		s_record_109_drop(v_resolved);
		s_record_107_drop(v_right_type);
		s_record_107_drop(v_left_type);
		s_record_122_drop(v_next);
		s_record_123_drop(v_right);
		s_record_123_drop(v_left);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 10))
	{
		const int64_t v_callee = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
		if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee)) == 23))
		{
			const int64_t v_dict_kind = s_dict_constructor_kind(s_record_122_copy(v_state), v_callee);
			if ((v_dict_kind != 0))
			{
				if ((s_record_101_get_count_take(s_record_101_copy(v_entry)) != 1))
				{
					s_record_123 s_return_982 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("dictionary constructor takes no arguments", sizeof("dictionary constructor takes no arguments") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_122_drop(v_state);
					return s_return_982;
				}
				s_record_122 s_arg_983 = s_register_dict(s_record_122_copy(v_state), v_dict_kind);
				S_String * s_arg_984 = s_string_concat_take(s_dict_helper(v_dict_kind), s_string_from_bytes("_make()", sizeof("_make()") - 1));
				int64_t s_arg_985 = v_dict_kind;
				s_record_123 s_return_986 = s_ExprCode(s_arg_983, s_arg_984, s_arg_985);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_986;
			}
			const int64_t v_kind = s_array_constructor_kind(s_record_122_copy(v_state), v_callee);
			if (((v_kind == 0) || (s_record_101_get_count_take(s_record_101_copy(v_entry)) != 1)))
			{
				s_record_123 s_return_987 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("unsupported array constructor", sizeof("unsupported array constructor") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_987;
			}
			if ((v_kind == 7))
			{
				s_record_123 s_return_988 = s_ExprCode(s_record_122_copy(v_state), s_string_from_bytes("s_array_i64_make(0, NULL)", sizeof("s_array_i64_make(0, NULL)") - 1), v_kind);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_988;
			}
			s_record_123 s_return_989 = s_ExprCode(s_record_122_copy(v_state), s_string_concat_take(s_array_helper(v_kind), s_string_from_bytes("_make_take(0, NULL)", sizeof("_make_take(0, NULL)") - 1)), v_kind);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_989;
		}
		if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee)) == 4))
		{
			S_String * v_method = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee);
			if (((((((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("byte_at", sizeof("byte_at") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("slice", sizeof("slice") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("find", sizeof("find") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("starts_with", sizeof("starts_with") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("ends_with", sizeof("ends_with") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("join", sizeof("join") - 1)) == 0)))
			{
				const int64_t v_owner_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee, 0);
				int64_t v_effects = 0;
				if (s_has_effect(s_record_122_copy(v_state), v_owner_id, s_ga_8_make_take(0, NULL)))
				{
					v_effects += 1;
				}
				for (int64_t v_i = 1, s_bound_990 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_990; ++v_i)
				{
					if (s_has_effect(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i), s_ga_8_make_take(0, NULL)))
					{
						v_effects += 1;
					}
				}
				if ((v_effects > 1))
				{
					s_record_123 s_return_991 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("string method requires ordered argument evaluation", sizeof("string method requires ordered argument evaluation") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
					s_string_release(v_method);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_122_drop(v_state);
					return s_return_991;
				}
				s_record_123 v_receiver = s_emit_expr(s_record_122_copy(v_state), v_owner_id);
				s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_receiver));
				S_Array * v_args = s_ga_8_make_take(0, NULL);
				S_Array_i64 * v_kinds = s_array_i64_make(0, NULL);
				for (int64_t v_i = 1, s_bound_992 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_992; ++v_i)
				{
					s_record_123 v_item = s_emit_expr(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i));
					s_record_122 s_value_993 = s_record_123_get_state_take(s_record_123_copy(v_item));
					s_record_122_drop(v_next);
					v_next = s_value_993;
					S_String * s_item_994 = s_record_123_get_text_take(s_record_123_copy(v_item));
					v_args = s_ga_8_append_take(v_args, s_item_994);
					int64_t s_item_995 = s_record_123_get_kind_take(s_record_123_copy(v_item));
					v_kinds = s_array_i64_append_take(v_kinds, s_item_995);
					s_record_123_drop(v_item);
				}
				s_record_125 v_signature = s_call_type_ids(s_record_122_copy(v_next), s_array_i64_retain(v_kinds));
				s_record_106 s_field_996 = s_record_125_get_table_take(s_record_125_copy(v_signature));
				s_record_106_drop(v_next.types);
				v_next.types = s_field_996;
				s_record_107 v_owner = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_next)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_123_get_kind_take(s_record_123_copy(v_receiver)));
				s_record_122 s_value_997 = s_check_hir_call(s_record_122_copy(v_next), v_id, s_resolve_hir_string_call(s_record_107_get_table_take(s_record_107_copy(v_owner)), s_record_107_get_id_take(s_record_107_copy(v_owner)), s_string_retain(v_method), s_record_125_get_ids_take(s_record_125_copy(v_signature))));
				s_record_122_drop(v_next);
				v_next = s_value_997;
				if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_123 s_return_998 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
					s_record_107_drop(v_owner);
					s_record_125_drop(v_signature);
					s_array_i64_release(v_kinds);
					s_array_release(v_args);
					s_record_122_drop(v_next);
					s_record_123_drop(v_receiver);
					s_string_release(v_method);
					s_string_release(v_value);
					s_record_101_drop(v_entry);
					s_record_122_drop(v_state);
					return s_return_998;
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
				s_record_123 s_return_999 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_", sizeof("s_string_") - 1), s_string_retain(v_method)), s_string_from_bytes("_take(", sizeof("_take(") - 1)), s_record_123_get_text_take(s_record_123_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_args))), s_string_from_bytes(")", sizeof(")") - 1)), v_result_kind);
				s_record_107_drop(v_owner);
				s_record_125_drop(v_signature);
				s_array_i64_release(v_kinds);
				s_array_release(v_args);
				s_record_122_drop(v_next);
				s_record_123_drop(v_receiver);
				s_string_release(v_method);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_999;
				s_record_107_drop(v_owner);
				s_record_125_drop(v_signature);
				s_array_i64_release(v_kinds);
				s_array_release(v_args);
				s_record_122_drop(v_next);
				s_record_123_drop(v_receiver);
			}
			s_string_release(v_method);
		}
		if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee)) == 4) && ((s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee), s_string_from_bytes("insert", sizeof("insert") - 1)) == 0) || (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee), s_string_from_bytes("get", sizeof("get") - 1)) == 0))))
		{
			S_String * v_method = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee);
			s_record_123 v_receiver = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee, 0));
			s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_receiver));
			S_Array * v_arguments = s_ga_8_make_take(0, NULL);
			S_Array_i64 * v_kinds = s_array_i64_make(0, NULL);
			for (int64_t v_i = 1, s_bound_1000 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_1000; ++v_i)
			{
				s_record_123 v_argument = s_emit_expr(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i));
				s_record_122 s_value_1001 = s_record_123_get_state_take(s_record_123_copy(v_argument));
				s_record_122_drop(v_next);
				v_next = s_value_1001;
				S_String * s_item_1002 = s_record_123_get_text_take(s_record_123_copy(v_argument));
				v_arguments = s_ga_8_append_take(v_arguments, s_item_1002);
				int64_t s_item_1003 = s_record_123_get_kind_take(s_record_123_copy(v_argument));
				v_kinds = s_array_i64_append_take(v_kinds, s_item_1003);
				s_record_123_drop(v_argument);
			}
			if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_123 s_return_1004 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_array_i64_release(v_kinds);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_record_123_drop(v_receiver);
				s_string_release(v_method);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1004;
			}
			s_record_125 v_signature = s_call_type_ids(s_record_122_copy(v_next), s_array_i64_retain(v_kinds));
			s_record_107 v_owner = s_canonical_kind(s_record_125_get_table_take(s_record_125_copy(v_signature)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_123_get_kind_take(s_record_123_copy(v_receiver)));
			s_record_122 s_value_1005 = s_check_hir_call(s_record_122_copy(v_next), v_id, s_resolve_hir_collection_call(s_record_107_get_table_take(s_record_107_copy(v_owner)), s_record_107_get_id_take(s_record_107_copy(v_owner)), s_string_retain(v_method), s_record_125_get_ids_take(s_record_125_copy(v_signature))));
			s_record_122_drop(v_next);
			v_next = s_value_1005;
			if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_123 s_return_1006 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_107_drop(v_owner);
				s_record_125_drop(v_signature);
				s_array_i64_release(v_kinds);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_record_123_drop(v_receiver);
				s_string_release(v_method);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1006;
			}
			s_record_122 s_value_1007 = s_register_dict(s_record_122_copy(v_next), s_record_123_get_kind_take(s_record_123_copy(v_receiver)));
			s_record_122_drop(v_next);
			v_next = s_value_1007;
			if ((s_string_compare_take(s_string_retain(v_method), s_string_from_bytes("get", sizeof("get") - 1)) == 0))
			{
				s_record_122 s_arg_1008 = s_record_122_copy(v_next);
				S_String * s_arg_1009 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_dict_helper(s_record_123_get_kind_take(s_record_123_copy(v_receiver))), s_string_from_bytes("_get_take(", sizeof("_get_take(") - 1)), s_record_123_get_text_take(s_record_123_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1));
				int64_t s_arg_1010 = s_optional_of(s_dict_value(s_record_123_get_kind_take(s_record_123_copy(v_receiver))));
				s_record_123 s_return_1011 = s_ExprCode(s_arg_1008, s_arg_1009, s_arg_1010);
				s_record_107_drop(v_owner);
				s_record_125_drop(v_signature);
				s_array_i64_release(v_kinds);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_record_123_drop(v_receiver);
				s_string_release(v_method);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1011;
			}
			S_String * s_left_1012 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_dict_helper(s_record_123_get_kind_take(s_record_123_copy(v_receiver))), s_string_from_bytes("_insert_take(", sizeof("_insert_take(") - 1)), s_record_123_get_text_take(s_record_123_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1));
			S_String * s_right_1013 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments));
			s_record_123 s_return_1014 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_left_1012, s_right_1013), s_string_from_bytes(")", sizeof(")") - 1)), s_record_123_get_kind_take(s_record_123_copy(v_receiver)));
			s_record_107_drop(v_owner);
			s_record_125_drop(v_signature);
			s_array_i64_release(v_kinds);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_record_123_drop(v_receiver);
			s_string_release(v_method);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1014;
			s_record_107_drop(v_owner);
			s_record_125_drop(v_signature);
			s_array_i64_release(v_kinds);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_record_123_drop(v_receiver);
			s_string_release(v_method);
		}
		if ((((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee)) == 4) && (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee), s_string_from_bytes("append", sizeof("append") - 1)) == 0)) && (s_record_101_get_count_take(s_record_101_copy(v_entry)) == 2)))
		{
			s_record_123 v_receiver = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee, 0));
			s_record_123 v_item = s_emit_expr(s_record_123_get_state_take(s_record_123_copy(v_receiver)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
			s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_item));
			s_record_125 v_signature = s_call_type_ids(s_record_122_copy(v_next), s_array_i64_make(1, (int64_t[]){s_record_123_get_kind_take(s_record_123_copy(v_item))}));
			s_record_107 v_owner = s_canonical_kind(s_record_125_get_table_take(s_record_125_copy(v_signature)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_123_get_kind_take(s_record_123_copy(v_receiver)));
			s_record_122 s_value_1015 = s_check_hir_call(s_record_122_copy(v_next), v_id, s_resolve_hir_collection_call(s_record_107_get_table_take(s_record_107_copy(v_owner)), s_record_107_get_id_take(s_record_107_copy(v_owner)), s_string_from_bytes("append", sizeof("append") - 1), s_record_125_get_ids_take(s_record_125_copy(v_signature))));
			s_record_122_drop(v_next);
			v_next = s_value_1015;
			if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_123 s_return_1016 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_107_drop(v_owner);
				s_record_125_drop(v_signature);
				s_record_122_drop(v_next);
				s_record_123_drop(v_item);
				s_record_123_drop(v_receiver);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1016;
			}
			if ((s_record_123_get_kind_take(s_record_123_copy(v_receiver)) == 7))
			{
				s_record_123 s_return_1017 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_append_take(", sizeof("s_array_i64_append_take(") - 1), s_record_123_get_text_take(s_record_123_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_item))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_123_get_kind_take(s_record_123_copy(v_receiver)));
				s_record_107_drop(v_owner);
				s_record_125_drop(v_signature);
				s_record_122_drop(v_next);
				s_record_123_drop(v_item);
				s_record_123_drop(v_receiver);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1017;
			}
			s_record_123 s_return_1018 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(s_record_123_get_kind_take(s_record_123_copy(v_receiver))), s_string_from_bytes("_append_take(", sizeof("_append_take(") - 1)), s_record_123_get_text_take(s_record_123_copy(v_receiver))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_item))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_123_get_kind_take(s_record_123_copy(v_receiver)));
			s_record_107_drop(v_owner);
			s_record_125_drop(v_signature);
			s_record_122_drop(v_next);
			s_record_123_drop(v_item);
			s_record_123_drop(v_receiver);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1018;
			s_record_107_drop(v_owner);
			s_record_125_drop(v_signature);
			s_record_122_drop(v_next);
			s_record_123_drop(v_item);
			s_record_123_drop(v_receiver);
		}
		if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee)) != 29))
		{
			s_record_123 s_return_1019 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("only named calls are supported", sizeof("only named calls are supported") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1019;
		}
		S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_callee);
		int64_t v_calls = 0;
		for (int64_t v_i = 1, s_bound_1020 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_1020; ++v_i)
		{
			if (s_has_effect(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i), s_ga_8_make_take(0, NULL)))
			{
				v_calls += 1;
			}
		}
		s_record_122 v_next = s_record_122_copy(v_state);
		S_Array * v_arguments = s_ga_8_make_take(0, NULL);
		S_Array_i64 * v_arg_types = s_array_i64_make(0, NULL);
		for (int64_t v_i = 1, s_bound_1021 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_1021; ++v_i)
		{
			s_record_123 v_arg = s_emit_expr(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i));
			s_record_122 s_value_1022 = s_record_123_get_state_take(s_record_123_copy(v_arg));
			s_record_122_drop(v_next);
			v_next = s_value_1022;
			S_String * v_expression = s_record_123_get_text_take(s_record_123_copy(v_arg));
			if ((((v_calls > 1) && (s_record_123_get_kind_take(s_record_123_copy(v_arg)) != 0)) && (s_record_123_get_kind_take(s_record_123_copy(v_arg)) != 5)))
			{
				S_String * v_temporary = s_string_concat_take(s_string_from_bytes("s_arg_", sizeof("s_arg_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
				v_next.counter += 1;
				s_record_122 s_value_1023 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_arg))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temporary)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_arg))), s_string_from_bytes(";", sizeof(";") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1023;
				S_String *s_value_1024 = s_string_retain(v_temporary);
				s_string_release(v_expression);
				v_expression = s_value_1024;
				s_string_release(v_temporary);
			}
			S_String * s_item_1025 = s_string_retain(v_expression);
			v_arguments = s_ga_8_append_take(v_arguments, s_item_1025);
			int64_t s_item_1026 = s_record_123_get_kind_take(s_record_123_copy(v_arg));
			v_arg_types = s_array_i64_append_take(v_arg_types, s_item_1026);
			s_string_release(v_expression);
			s_record_123_drop(v_arg);
		}
		if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_123 s_return_1027 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1027;
		}
		if (s_hir_is_builtin(s_string_retain(v_name)))
		{
			s_record_125 v_signature = s_call_type_ids(s_record_122_copy(v_next), s_array_i64_retain(v_arg_types));
			s_record_122 s_value_1028 = s_check_hir_call(s_record_122_copy(v_next), v_id, s_resolve_hir_builtin_call(s_record_125_get_table_take(s_record_125_copy(v_signature)), s_string_retain(v_name), s_record_125_get_ids_take(s_record_125_copy(v_signature))));
			s_record_122_drop(v_next);
			v_next = s_value_1028;
			if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_123 s_return_1029 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
				s_record_125_drop(v_signature);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1029;
			}
			s_record_125_drop(v_signature);
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("arguments", sizeof("arguments") - 1)) == 0))
		{
			s_record_123 s_return_1030 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("s_arguments()", sizeof("s_arguments()") - 1), 8);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1030;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("read_text", sizeof("read_text") - 1)) == 0))
		{
			s_record_123 s_return_1031 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_read_text_take(", sizeof("s_read_text_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), s_optional_of(4));
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1031;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("write_text", sizeof("write_text") - 1)) == 0))
		{
			s_record_123 s_return_1032 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_write_text_take(", sizeof("s_write_text_take(") - 1), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments))), s_string_from_bytes(")", sizeof(")") - 1)), 3);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1032;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("print", sizeof("print") - 1)) == 0))
		{
			S_String * v_printed_arg = s_ga_8_get_checked_take(s_array_retain(v_arguments), 0);
			const int64_t v_kind = s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0);
			if ((v_kind == 1))
			{
				s_record_123 s_return_1033 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("printf(\"%lld\\n\", (long long)(", sizeof("printf(\"%lld\\n\", (long long)(") - 1), s_string_retain(v_printed_arg)), s_string_from_bytes("))", sizeof("))") - 1)), 5);
				s_string_release(v_printed_arg);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1033;
			}
			if ((v_kind == 2))
			{
				s_record_123 s_return_1034 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("printf(\"%.17g\\n\", ", sizeof("printf(\"%.17g\\n\", ") - 1), s_string_retain(v_printed_arg)), s_string_from_bytes(")", sizeof(")") - 1)), 5);
				s_string_release(v_printed_arg);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1034;
			}
			if ((v_kind == 3))
			{
				s_record_123 s_return_1035 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("printf(\"%s\\n\", ", sizeof("printf(\"%s\\n\", ") - 1), s_string_retain(v_printed_arg)), s_string_from_bytes(" ? \"true\" : \"false\")", sizeof(" ? \"true\" : \"false\")") - 1)), 5);
				s_string_release(v_printed_arg);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1035;
			}
			if ((v_kind == 4))
			{
				s_record_123 s_return_1036 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_print_string_take(", sizeof("s_print_string_take(") - 1), s_string_retain(v_printed_arg)), s_string_from_bytes(")", sizeof(")") - 1)), 5);
				s_string_release(v_printed_arg);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1036;
			}
			s_record_123 s_return_1037 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("cannot print this type", sizeof("cannot print this type") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_string_release(v_printed_arg);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1037;
			s_string_release(v_printed_arg);
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("len", sizeof("len") - 1)) == 0))
		{
			if ((s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0) == 4))
			{
				s_record_123 s_return_1038 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_length_take(", sizeof("s_string_length_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 1);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1038;
			}
			if ((s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0) == 7))
			{
				s_record_123 s_return_1039 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_length_take(", sizeof("s_array_i64_length_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 1);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1039;
			}
			if (s_generic_array(s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0)))
			{
				s_record_123 s_return_1040 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_generic_array_length_take(", sizeof("s_generic_array_length_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 1);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1040;
			}
			if (s_is_dict(s_array_i64_get_checked_take(s_array_i64_retain(v_arg_types), 0)))
			{
				s_record_123 s_return_1041 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_dict_length_take(", sizeof("s_dict_length_take(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 1);
				s_array_i64_release(v_arg_types);
				s_array_release(v_arguments);
				s_record_122_drop(v_next);
				s_string_release(v_name);
				s_string_release(v_value);
				s_record_101_drop(v_entry);
				s_record_122_drop(v_state);
				return s_return_1041;
			}
			s_record_123 s_return_1042 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("len expects a string or array", sizeof("len expects a string or array") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1042;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("str", sizeof("str") - 1)) == 0))
		{
			s_record_123 s_return_1043 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_from_i64(", sizeof("s_string_from_i64(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 4);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1043;
		}
		if ((s_string_compare_take(s_string_retain(v_name), s_string_from_bytes("sqrt", sizeof("sqrt") - 1)) == 0))
		{
			s_record_123 s_return_1044 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("sqrt(", sizeof("sqrt(") - 1), s_ga_8_get_checked_take(s_array_retain(v_arguments), 0)), s_string_from_bytes(")", sizeof(")") - 1)), 2);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1044;
		}
		const int64_t v_index = s_find_function(s_record_122_copy(v_next), s_string_retain(v_name));
		if ((v_index < 0))
		{
			s_record_123 s_return_1045 = s_ExprCode(s_emit_error(s_record_122_copy(v_next), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown function '", sizeof("unknown function '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1045;
		}
		s_record_118 v_function = s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_next)), v_index);
		s_record_125 v_signature = s_call_type_ids(s_record_122_copy(v_next), s_array_i64_retain(v_arg_types));
		s_record_106 s_field_1046 = s_record_125_get_table_take(s_record_125_copy(v_signature));
		s_record_106_drop(v_next.types);
		v_next.types = s_field_1046;
		S_Array_i64 * v_parameter_ids = s_array_i64_make(0, NULL);
		{
			S_Array * v_s_iter_1047 = s_record_118_get_params_take(s_record_118_copy(v_function));
			for (int64_t s_index_1047 = 0; s_index_1047 < (int64_t)s_array_length(v_s_iter_1047); ++s_index_1047)
			{
				s_record_104 v_parameter = s_ga_10104_get_borrow(v_s_iter_1047, s_index_1047);
				s_record_107 v_canonical = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_next)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_104_get_kind_take(s_record_104_copy(v_parameter)));
				s_record_106 s_field_1048 = s_record_107_get_table_take(s_record_107_copy(v_canonical));
				s_record_106_drop(v_next.types);
				v_next.types = s_field_1048;
				int64_t s_item_1049 = s_record_107_get_id_take(s_record_107_copy(v_canonical));
				v_parameter_ids = s_array_i64_append_take(v_parameter_ids, s_item_1049);
				s_record_107_drop(v_canonical);
				s_record_104_drop(v_parameter);
			}
			s_array_release(v_s_iter_1047);
		}
		s_record_107 v_ret = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_next)), s_record_122_get_records_take(s_record_122_copy(v_next)), s_record_118_get_kind_take(s_record_118_copy(v_function)));
		s_record_122 s_value_1050 = s_check_hir_call(s_record_122_copy(v_next), v_id, s_resolve_hir_function_call(s_record_107_get_table_take(s_record_107_copy(v_ret)), s_string_retain(v_name), s_record_125_get_ids_take(s_record_125_copy(v_signature)), s_array_i64_retain(v_parameter_ids), s_record_107_get_id_take(s_record_107_copy(v_ret))));
		s_record_122_drop(v_next);
		v_next = s_value_1050;
		if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_123 s_return_1051 = s_ExprCode(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1), 0);
			s_record_107_drop(v_ret);
			s_array_i64_release(v_parameter_ids);
			s_record_125_drop(v_signature);
			s_record_118_drop(v_function);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1051;
		}
		if (s_is_generic(s_record_118_copy(v_function)))
		{
			s_record_123 v_specialized = s_specialize_call(s_record_122_copy(v_next), v_id, s_record_118_copy(v_function), s_array_i64_retain(v_arg_types), s_array_retain(v_arguments));
			s_record_122 v_result = s_record_123_get_state_take(s_record_123_copy(v_specialized));
			if (((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) == 0) && (s_record_123_get_kind_take(s_record_123_copy(v_specialized)) != 0)))
			{
				s_record_107 v_resolved = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_result)), s_record_122_get_records_take(s_record_122_copy(v_result)), s_record_123_get_kind_take(s_record_123_copy(v_specialized)));
				s_record_106 s_field_1052 = s_record_107_get_table_take(s_record_107_copy(v_resolved));
				s_record_106_drop(v_result.types);
				v_result.types = s_field_1052;
				S_Array * s_field_1053 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_result)), s_HirNode(v_id, 6, s_record_107_get_id_take(s_record_107_copy(v_resolved)), (-1)));
				s_array_release(v_result.hir);
				v_result.hir = s_field_1053;
				s_record_107_drop(v_resolved);
			}
			s_record_123 s_return_1054 = s_ExprCode(s_record_122_copy(v_result), s_record_123_get_text_take(s_record_123_copy(v_specialized)), s_record_123_get_kind_take(s_record_123_copy(v_specialized)));
			s_record_122_drop(v_result);
			s_record_123_drop(v_specialized);
			s_record_107_drop(v_ret);
			s_array_i64_release(v_parameter_ids);
			s_record_125_drop(v_signature);
			s_record_118_drop(v_function);
			s_array_i64_release(v_arg_types);
			s_array_release(v_arguments);
			s_record_122_drop(v_next);
			s_string_release(v_name);
			s_string_release(v_value);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1054;
			s_record_122_drop(v_result);
			s_record_123_drop(v_specialized);
		}
		s_record_123 s_return_1055 = s_ExprCode(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_record_118_get_symbol_take(s_record_118_copy(v_function)), s_string_from_bytes("(", sizeof("(") - 1)), s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_arguments))), s_string_from_bytes(")", sizeof(")") - 1)), s_record_118_get_kind_take(s_record_118_copy(v_function)));
		s_record_107_drop(v_ret);
		s_array_i64_release(v_parameter_ids);
		s_record_125_drop(v_signature);
		s_record_118_drop(v_function);
		s_array_i64_release(v_arg_types);
		s_array_release(v_arguments);
		s_record_122_drop(v_next);
		s_string_release(v_name);
		s_string_release(v_value);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_1055;
		s_record_107_drop(v_ret);
		s_array_i64_release(v_parameter_ids);
		s_record_125_drop(v_signature);
		s_record_118_drop(v_function);
		s_array_i64_release(v_arg_types);
		s_array_release(v_arguments);
		s_record_122_drop(v_next);
		s_string_release(v_name);
	}
	s_record_123 s_return_1056 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unsupported expression '", sizeof("unsupported expression '") - 1), s_node_kind_name(s_record_101_get_kind_take(s_record_101_copy(v_entry)))), s_string_from_bytes("'", sizeof("'") - 1))), s_string_from_bytes("", sizeof("") - 1), 0);
	s_string_release(v_value);
	s_record_101_drop(v_entry);
	s_record_122_drop(v_state);
	return s_return_1056;
	s_string_release(v_value);
	s_record_101_drop(v_entry);
}

bool s_contains_recur(s_record_102 v_tree, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_102_copy(v_tree), v_id);
	if ((s_record_101_get_kind_take(s_record_101_copy(v_entry)) == 34))
	{
		bool s_return_1057 = true;
		s_record_101_drop(v_entry);
		s_record_102_drop(v_tree);
		return s_return_1057;
	}
	for (int64_t v_i = 0, s_bound_1058 = s_record_101_get_count_take(s_record_101_copy(v_entry)); v_i < s_bound_1058; ++v_i)
	{
		if (s_contains_recur(s_record_102_copy(v_tree), s_ast_child(s_record_102_copy(v_tree), v_id, v_i)))
		{
			bool s_return_1059 = true;
			s_record_101_drop(v_entry);
			s_record_102_drop(v_tree);
			return s_return_1059;
		}
	}
	bool s_return_1060 = false;
	s_record_101_drop(v_entry);
	s_record_102_drop(v_tree);
	return s_return_1060;
	s_record_101_drop(v_entry);
}

s_record_122 s_emit_block(s_record_122 v_state, int64_t v_id)
{
	const int64_t v_count = s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)));
	s_record_122 v_next = s_record_122_copy(v_state);
	s_record_101 v_block = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	for (int64_t v_i = 0, s_bound_1061 = s_record_101_get_count_take(s_record_101_copy(v_block)); v_i < s_bound_1061; ++v_i)
	{
		s_record_122 s_value_1062 = s_emit_statement(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_id, v_i));
		s_record_122_drop(v_next);
		v_next = s_value_1062;
		if (!((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
			s_record_122 s_return_1063 = s_record_122_copy(v_next);
			s_record_101_drop(v_block);
			s_record_122_drop(v_next);
			s_record_122_drop(v_state);
			return s_return_1063;
		}
	}
	for (int64_t v_i = v_count, s_bound_1064 = s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_next))); v_i < s_bound_1064; ++v_i)
	{
		const int64_t v_index = (((s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_next))) + v_count) - v_i) - 1);
		s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)), v_index);
		if ((s_is_optional(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) && s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))))
		{
			s_record_122 s_value_1065 = s_line(s_record_122_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
			s_record_122_drop(v_next);
			v_next = s_value_1065;
		}
		if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 4))
		{
			s_record_122 s_value_1066 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_release(v_", sizeof("s_string_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1066;
		}
		if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 7))
		{
			s_record_122 s_value_1067 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_release(v_", sizeof("s_array_i64_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1067;
		}
		if (s_generic_array(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
		{
			s_record_122 s_value_1068 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_release(v_", sizeof("s_array_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1068;
		}
		if (s_is_dict(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
		{
			s_record_122 s_value_1069 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_dict_release(v_", sizeof("s_dict_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1069;
		}
		if (((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) >= 100) && (s_record_110_get_kind_take(s_record_110_copy(v_symbol)) < 10000)))
		{
			s_record_122 s_value_1070 = s_line(s_record_122_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
			s_record_122_drop(v_next);
			v_next = s_value_1070;
		}
		s_record_110_drop(v_symbol);
	}
	S_Array * v_restored = s_ga_10110_make_take(0, NULL);
	for (int64_t v_i = 0, s_bound_1071 = v_count; v_i < s_bound_1071; ++v_i)
	{
		s_record_110 s_item_1072 = s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)), v_i);
		v_restored = s_ga_10110_append_take(v_restored, s_item_1072);
	}
	S_Array * s_field_1073 = s_array_retain(v_restored);
	s_array_release(v_next.symbols);
	v_next.symbols = s_field_1073;
	s_record_122 s_return_1074 = s_record_122_copy(v_next);
	s_array_release(v_restored);
	s_record_101_drop(v_block);
	s_record_122_drop(v_next);
	s_record_122_drop(v_state);
	return s_return_1074;
	s_array_release(v_restored);
	s_record_101_drop(v_block);
	s_record_122_drop(v_next);
}

s_record_122 s_emit_nested_block(s_record_122 v_state, int64_t v_id)
{
	s_record_122 v_next = s_line(s_record_122_copy(v_state), s_string_from_bytes("{", sizeof("{") - 1));
	v_next.indentation += 1;
	v_next.scope_depth += 1;
	s_record_122 s_value_1075 = s_emit_block(s_record_122_copy(v_next), v_id);
	s_record_122_drop(v_next);
	v_next = s_value_1075;
	v_next.scope_depth -= 1;
	v_next.indentation -= 1;
	s_record_122 s_return_1076 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
	s_record_122_drop(v_next);
	s_record_122_drop(v_state);
	return s_return_1076;
	s_record_122_drop(v_next);
}

s_record_122 s_emit_return_value(s_record_122 v_state, int64_t v_id, int64_t v_value_id)
{
	s_record_123 v_result = s_emit_expr(s_record_122_copy(v_state), v_value_id);
	s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_result));
	if ((s_record_122_get_return_kind_take(s_record_122_copy(v_next)) == 0))
	{
		v_next.return_kind = s_record_123_get_kind_take(s_record_123_copy(v_result));
	}
	else
	{
		s_record_122 s_value_1077 = s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_result)), s_record_122_get_return_kind_take(s_record_122_copy(v_next)));
		s_record_122_drop(v_next);
		v_next = s_value_1077;
	}
	bool v_needs_cleanup = false;
	{
		S_Array * v_s_iter_1078 = s_record_122_get_symbols_take(s_record_122_copy(v_next));
		for (int64_t s_index_1078 = 0; s_index_1078 < (int64_t)s_array_length(v_s_iter_1078); ++s_index_1078)
		{
			s_record_110 v_symbol = s_ga_10110_get_borrow(v_s_iter_1078, s_index_1078);
			if (s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
			{
				v_needs_cleanup = true;
			}
			s_record_110_drop(v_symbol);
		}
		s_array_release(v_s_iter_1078);
	}
	if (v_needs_cleanup)
	{
		const int64_t v_serial = s_record_122_get_counter_take(s_record_122_copy(v_next));
		v_next.counter += 1;
		S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_return_", sizeof("s_return_") - 1), s_string_from_i64(v_serial));
		s_record_122 s_value_1079 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_result))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_122_drop(v_next);
		v_next = s_value_1079;
		for (int64_t v_i = 0, s_bound_1080 = s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_next))); v_i < s_bound_1080; ++v_i)
		{
			const int64_t v_index = ((s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_next))) - v_i) - 1);
			s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)), v_index);
			if ((s_is_optional(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) && s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))))
			{
				s_record_122 s_value_1081 = s_line(s_record_122_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
				s_record_122_drop(v_next);
				v_next = s_value_1081;
			}
			if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 4))
			{
				s_record_122 s_value_1082 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_release(v_", sizeof("s_string_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1082;
			}
			if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 7))
			{
				s_record_122 s_value_1083 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_release(v_", sizeof("s_array_i64_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1083;
			}
			if (s_generic_array(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
			{
				s_record_122 s_value_1084 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_release(v_", sizeof("s_array_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1084;
			}
			if (s_is_dict(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
			{
				s_record_122 s_value_1085 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_dict_release(v_", sizeof("s_dict_release(v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1085;
			}
			if (((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) >= 100) && (s_record_110_get_kind_take(s_record_110_copy(v_symbol)) < 10000)))
			{
				s_record_122 s_value_1086 = s_line(s_record_122_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
				s_record_122_drop(v_next);
				v_next = s_value_1086;
			}
			s_record_110_drop(v_symbol);
		}
		s_record_122 s_return_1087 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("return ", sizeof("return ") - 1), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
		s_string_release(v_temp);
		s_record_122_drop(v_next);
		s_record_123_drop(v_result);
		s_record_122_drop(v_state);
		return s_return_1087;
		s_string_release(v_temp);
	}
	s_record_122 s_return_1088 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("return ", sizeof("return ") - 1), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
	s_record_122_drop(v_next);
	s_record_123_drop(v_result);
	s_record_122_drop(v_state);
	return s_return_1088;
	s_record_122_drop(v_next);
	s_record_123_drop(v_result);
}

s_record_122 s_emit_statement(s_record_122 v_state, int64_t v_id)
{
	if (!((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
		s_record_122 s_return_1089 = s_record_122_copy(v_state);
		s_record_122_drop(v_state);
		return s_return_1089;
	}
	s_record_101 v_item = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 26) || (s_record_101_get_kind_take(s_record_101_copy(v_item)) == 40)))
	{
		S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
		const int64_t v_previous = s_find_symbol(s_record_122_copy(v_state), s_string_retain(v_name));
		if (((v_previous >= 0) && (s_record_110_get_depth_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_previous)) == s_record_122_get_scope_depth_take(s_record_122_copy(v_state)))))
		{
			s_record_122 s_return_1090 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("duplicate binding '", sizeof("duplicate binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1090;
		}
		int64_t v_kind = 0;
		int64_t v_expr_id = (-1);
		int64_t v_fallback_id = (-1);
		for (int64_t v_i = 0, s_bound_1091 = s_record_101_get_count_take(s_record_101_copy(v_item)); v_i < s_bound_1091; ++v_i)
		{
			const int64_t v_child = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i);
			const int64_t v_child_kind = s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_child));
			if ((v_child_kind == 38))
			{
				v_kind = s_resolve_type(s_record_122_copy(v_state), v_child);
			}
			else if ((v_child_kind == 17))
			{
				v_fallback_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_child, 0);
			}
			else
			{
				v_expr_id = v_child;
			}
		}
		if ((v_expr_id < 0))
		{
			s_record_122 s_return_1092 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("binding requires initializer", sizeof("binding requires initializer") - 1));
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1092;
		}
		s_record_123 v_value_result = s_emit_expr(s_record_122_copy(v_state), v_expr_id);
		s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_value_result));
		S_String * v_value_text = s_record_123_get_text_take(s_record_123_copy(v_value_result));
		int64_t v_value_kind = s_record_123_get_kind_take(s_record_123_copy(v_value_result));
		if ((v_fallback_id >= 0))
		{
			if ((!s_is_optional(s_record_123_get_kind_take(s_record_123_copy(v_value_result)))))
			{
				s_record_122 s_return_1093 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("else return requires an Optional", sizeof("else return requires an Optional") - 1));
				s_string_release(v_value_text);
				s_record_122_drop(v_next);
				s_record_123_drop(v_value_result);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1093;
			}
			v_value_kind = s_optional_value(s_record_123_get_kind_take(s_record_123_copy(v_value_result)));
			S_String * v_temporary = s_string_concat_take(s_string_from_bytes("s_guard_option_", sizeof("s_guard_option_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
			v_next.counter += 1;
			s_record_122 s_value_1094 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_value_result))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temporary)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_value_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1094;
			s_record_122 s_value_1095 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (!", sizeof("if (!") - 1), s_string_retain(v_temporary)), s_string_from_bytes(".has_value) {", sizeof(".has_value) {") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1095;
			v_next.indentation += 1;
			s_record_122 s_value_1096 = s_emit_return_value(s_record_122_copy(v_next), v_id, v_fallback_id);
			s_record_122_drop(v_next);
			v_next = s_value_1096;
			v_next.indentation -= 1;
			s_record_122 s_value_1097 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_122_drop(v_next);
			v_next = s_value_1097;
			S_String *s_value_1098 = s_string_concat_take(s_string_retain(v_temporary), s_string_from_bytes(".value", sizeof(".value") - 1));
			s_string_release(v_value_text);
			v_value_text = s_value_1098;
			s_string_release(v_temporary);
		}
		if ((v_kind == 0))
		{
			v_kind = v_value_kind;
		}
		s_record_122 s_value_1099 = s_check_type(s_record_122_copy(v_next), v_id, v_value_kind, v_kind);
		s_record_122_drop(v_next);
		v_next = s_value_1099;
		if (!((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
			s_record_122 s_return_1100 = s_record_122_copy(v_next);
			s_string_release(v_value_text);
			s_record_122_drop(v_next);
			s_record_123_drop(v_value_result);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1100;
		}
		S_String * v_declaration = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(v_kind), s_string_from_bytes(" v_", sizeof(" v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_value_text)), s_string_from_bytes(";", sizeof(";") - 1));
		if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 26) && (!s_managed(v_kind))))
		{
			S_String *s_value_1101 = s_string_concat_take(s_string_from_bytes("const ", sizeof("const ") - 1), s_string_retain(v_declaration));
			s_string_release(v_declaration);
			v_declaration = s_value_1101;
		}
		s_record_122 s_value_1102 = s_line(s_record_122_copy(v_next), s_string_retain(v_declaration));
		s_record_122_drop(v_next);
		v_next = s_value_1102;
		s_record_122 s_return_1103 = s_append_symbol(s_record_122_copy(v_next), v_id, s_string_retain(v_name), v_kind, (s_record_101_get_kind_take(s_record_101_copy(v_item)) == 40));
		s_string_release(v_declaration);
		s_string_release(v_value_text);
		s_record_122_drop(v_next);
		s_record_123_drop(v_value_result);
		s_string_release(v_name);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1103;
		s_string_release(v_declaration);
		s_string_release(v_value_text);
		s_record_122_drop(v_next);
		s_record_123_drop(v_value_result);
		s_string_release(v_name);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 3))
	{
		const int64_t v_target = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
		if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_target)) == 12))
		{
			if (((s_record_101_get_count_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_target)) != 2) || (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0)))
			{
				s_record_122 s_return_1104 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("unsupported indexed assignment", sizeof("unsupported indexed assignment") - 1));
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1104;
			}
			const int64_t v_root = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_target, 0);
			if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root)) == 4))
			{
				const int64_t v_owner_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root, 0);
				if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_owner_id)) != 29))
				{
					s_record_122 s_return_1105 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
					s_record_101_drop(v_item);
					s_record_122_drop(v_state);
					return s_return_1105;
				}
				S_String * v_owner = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_owner_id);
				const int64_t v_owner_index = s_find_symbol(s_record_122_copy(v_state), s_string_retain(v_owner));
				if (((v_owner_index < 0) || (!s_record_110_get_mutable_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_owner_index)))))
				{
					s_record_122 s_return_1106 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
					s_string_release(v_owner);
					s_record_101_drop(v_item);
					s_record_122_drop(v_state);
					return s_return_1106;
				}
				S_String * v_field_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root);
				const int64_t v_field_index = s_record_field_index(s_record_122_copy(v_state), s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_owner_index)), s_string_retain(v_field_name));
				if ((v_field_index < 0))
				{
					s_record_122 s_return_1107 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("indexed update requires an array field", sizeof("indexed update requires an array field") - 1));
					s_string_release(v_field_name);
					s_string_release(v_owner);
					s_record_101_drop(v_item);
					s_record_122_drop(v_state);
					return s_return_1107;
				}
				const int64_t v_array_kind = s_record_104_get_kind_take(s_ga_10104_get_checked_take(s_record_121_get_fields_take(s_ga_10121_get_checked_take(s_record_122_get_records_take(s_record_122_copy(v_state)), (s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_owner_index)) - 100))), v_field_index));
				const int64_t v_element_kind = s_array_element(v_array_kind);
				if ((v_element_kind == 0))
				{
					s_record_122 s_return_1108 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("indexed update requires an array", sizeof("indexed update requires an array") - 1));
					s_string_release(v_field_name);
					s_string_release(v_owner);
					s_record_101_drop(v_item);
					s_record_122_drop(v_state);
					return s_return_1108;
				}
				s_record_123 v_index = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_target, 1));
				s_record_123 v_item_value = s_emit_expr(s_record_123_get_state_take(s_record_123_copy(v_index)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
				s_record_122 v_next = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_item_value)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_index)), 1);
				s_record_122 s_value_1109 = s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_item_value)), v_element_kind);
				s_record_122_drop(v_next);
				v_next = s_value_1109;
				S_String * v_pointer = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("&v_", sizeof("&v_") - 1), s_string_retain(v_owner)), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_field_name));
				if ((v_array_kind == 7))
				{
					s_record_122 s_return_1110 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_set(", sizeof("s_array_i64_set(") - 1), s_string_retain(v_pointer)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_index))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_item_value))), s_string_from_bytes(");", sizeof(");") - 1)));
					s_string_release(v_pointer);
					s_record_122_drop(v_next);
					s_record_123_drop(v_item_value);
					s_record_123_drop(v_index);
					s_string_release(v_field_name);
					s_string_release(v_owner);
					s_record_101_drop(v_item);
					s_record_122_drop(v_state);
					return s_return_1110;
				}
				s_record_122 s_return_1111 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(v_array_kind), s_string_from_bytes("_set_take(", sizeof("_set_take(") - 1)), s_string_retain(v_pointer)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_index))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_item_value))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_string_release(v_pointer);
				s_record_122_drop(v_next);
				s_record_123_drop(v_item_value);
				s_record_123_drop(v_index);
				s_string_release(v_field_name);
				s_string_release(v_owner);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1111;
				s_string_release(v_pointer);
				s_record_122_drop(v_next);
				s_record_123_drop(v_item_value);
				s_record_123_drop(v_index);
				s_string_release(v_field_name);
				s_string_release(v_owner);
			}
			if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root)) != 29))
			{
				s_record_122 s_return_1112 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1112;
			}
			S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root);
			const int64_t v_symbol = s_find_symbol(s_record_122_copy(v_state), s_string_retain(v_name));
			if (((v_symbol < 0) || (!s_record_110_get_mutable_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_symbol)))))
			{
				s_record_122 s_return_1113 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("indexed update requires a mutable local array", sizeof("indexed update requires a mutable local array") - 1));
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1113;
			}
			const int64_t v_array_kind = s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_symbol));
			const int64_t v_element_kind = s_array_element(v_array_kind);
			if ((v_element_kind == 0))
			{
				s_record_122 s_return_1114 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("indexed update requires an array", sizeof("indexed update requires an array") - 1));
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1114;
			}
			s_record_123 v_index = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_target, 1));
			s_record_123 v_item_value = s_emit_expr(s_record_123_get_state_take(s_record_123_copy(v_index)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
			s_record_122 v_next = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_item_value)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_index)), 1);
			s_record_122 s_value_1115 = s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_item_value)), v_element_kind);
			s_record_122_drop(v_next);
			v_next = s_value_1115;
			if ((v_array_kind == 7))
			{
				s_record_122 s_return_1116 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_set(&v_", sizeof("s_array_i64_set(&v_") - 1), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_index))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_item_value))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				s_record_123_drop(v_item_value);
				s_record_123_drop(v_index);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1116;
			}
			s_record_122 s_return_1117 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(v_array_kind), s_string_from_bytes("_set_take(&v_", sizeof("_set_take(&v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_index))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_item_value))), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_122_drop(v_next);
			s_record_123_drop(v_item_value);
			s_record_123_drop(v_index);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1117;
			s_record_122_drop(v_next);
			s_record_123_drop(v_item_value);
			s_record_123_drop(v_index);
			s_string_release(v_name);
		}
		if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_target)) == 4))
		{
			const int64_t v_root = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_target, 0);
			if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root)) != 29))
			{
				s_record_122 s_return_1118 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("record update requires a local root binding", sizeof("record update requires a local root binding") - 1));
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1118;
			}
			S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_root);
			const int64_t v_index = s_find_symbol(s_record_122_copy(v_state), s_string_retain(v_name));
			if ((v_index < 0))
			{
				s_record_122 s_return_1119 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown binding '", sizeof("unknown binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1119;
			}
			s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_index);
			if ((!s_record_110_get_mutable_take(s_record_110_copy(v_symbol))))
			{
				s_record_122 s_return_1120 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("cannot assign immutable binding '", sizeof("cannot assign immutable binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1120;
			}
			S_String * v_field = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_target);
			const int64_t v_member = s_record_field_index(s_record_122_copy(v_state), s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_retain(v_field));
			if ((v_member < 0))
			{
				s_record_122 s_return_1121 = s_emit_error(s_record_122_copy(v_state), v_target, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown record field '", sizeof("unknown record field '") - 1), s_string_retain(v_field)), s_string_from_bytes("'", sizeof("'") - 1)));
				s_string_release(v_field);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1121;
			}
			const int64_t v_kind = s_record_104_get_kind_take(s_ga_10104_get_checked_take(s_record_121_get_fields_take(s_ga_10121_get_checked_take(s_record_122_get_records_take(s_record_122_copy(v_state)), (s_record_110_get_kind_take(s_record_110_copy(v_symbol)) - 100))), v_member));
			s_record_123 v_result = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
			s_record_122 v_next = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_result)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_result)), v_kind);
			S_String * v_op = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
			if (s_managed(v_kind))
			{
				if ((s_string_compare_take(s_string_retain(v_op), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
				{
					s_record_122 s_return_1122 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("managed record field update requires =", sizeof("managed record field update requires =") - 1));
					s_string_release(v_op);
					s_record_122_drop(v_next);
					s_record_123_drop(v_result);
					s_string_release(v_field);
					s_record_110_drop(v_symbol);
					s_string_release(v_name);
					s_record_101_drop(v_item);
					s_record_122_drop(v_state);
					return s_return_1122;
				}
				S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_field_", sizeof("s_field_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
				v_next.counter += 1;
				s_record_122 s_value_1123 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(v_kind), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1123;
				s_record_122 s_value_1124 = s_line(s_record_122_copy(v_next), s_record_drop(v_kind, s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_field))));
				s_record_122_drop(v_next);
				v_next = s_value_1124;
				s_record_122 s_return_1125 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_field)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
				s_string_release(v_temp);
				s_string_release(v_op);
				s_record_122_drop(v_next);
				s_record_123_drop(v_result);
				s_string_release(v_field);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1125;
				s_string_release(v_temp);
			}
			s_record_122 s_return_1126 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_field)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_op)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_op);
			s_record_122_drop(v_next);
			s_record_123_drop(v_result);
			s_string_release(v_field);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1126;
			s_string_release(v_op);
			s_record_122_drop(v_next);
			s_record_123_drop(v_result);
			s_string_release(v_field);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
		}
		const int64_t v_lhs = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
		const int64_t v_rhs = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1);
		if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_lhs)) != 29))
		{
			s_record_122 s_return_1127 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("only local-variable assignment is supported", sizeof("only local-variable assignment is supported") - 1));
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1127;
		}
		S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_lhs);
		const int64_t v_index = s_find_symbol(s_record_122_copy(v_state), s_string_retain(v_name));
		if ((v_index < 0))
		{
			s_record_122 s_return_1128 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown binding '", sizeof("unknown binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1128;
		}
		s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_state)), v_index);
		if ((!s_record_110_get_mutable_take(s_record_110_copy(v_symbol))))
		{
			s_record_122 s_return_1129 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("cannot assign immutable binding '", sizeof("cannot assign immutable binding '") - 1), s_string_retain(v_name)), s_string_from_bytes("'", sizeof("'") - 1)));
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1129;
		}
		if (((s_array_element(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) != 0) && (s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs)) == 10)))
		{
			s_record_101 v_call = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs);
			if ((s_record_101_get_count_take(s_record_101_copy(v_call)) == 2))
			{
				const int64_t v_method = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs, 0);
				if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_method)) == 4) && (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_method), s_string_from_bytes("append", sizeof("append") - 1)) == 0)))
				{
					const int64_t v_receiver = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_method, 0);
					if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_receiver)) == 29) && (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_receiver), s_string_retain(v_name)) == 0)))
					{
						s_record_123 v_element = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs, 1));
						s_record_122 v_next = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_element)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_element)), s_array_element(s_record_110_get_kind_take(s_record_110_copy(v_symbol))));
						if (!((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
							s_record_122 s_return_1130 = s_record_122_copy(v_next);
							s_record_122_drop(v_next);
							s_record_123_drop(v_element);
							s_record_101_drop(v_call);
							s_record_110_drop(v_symbol);
							s_string_release(v_name);
							s_record_101_drop(v_item);
							s_record_122_drop(v_state);
							return s_return_1130;
						}
						const int64_t v_serial = s_record_122_get_counter_take(s_record_122_copy(v_next));
						v_next.counter += 1;
						S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_item_", sizeof("s_item_") - 1), s_string_from_i64(v_serial));
						s_record_122 s_value_1131 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_element))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_element))), s_string_from_bytes(";", sizeof(";") - 1)));
						s_record_122_drop(v_next);
						v_next = s_value_1131;
						if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 7))
						{
							s_record_122 s_return_1132 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = s_array_i64_append_take(v_", sizeof(" = s_array_i64_append_take(v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(");", sizeof(");") - 1)));
							s_string_release(v_temp);
							s_record_122_drop(v_next);
							s_record_123_drop(v_element);
							s_record_101_drop(v_call);
							s_record_110_drop(v_symbol);
							s_string_release(v_name);
							s_record_101_drop(v_item);
							s_record_122_drop(v_state);
							return s_return_1132;
						}
						s_record_122 s_return_1133 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_array_helper(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))), s_string_from_bytes("_append_take(v_", sizeof("_append_take(v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(");", sizeof(");") - 1)));
						s_string_release(v_temp);
						s_record_122_drop(v_next);
						s_record_123_drop(v_element);
						s_record_101_drop(v_call);
						s_record_110_drop(v_symbol);
						s_string_release(v_name);
						s_record_101_drop(v_item);
						s_record_122_drop(v_state);
						return s_return_1133;
						s_string_release(v_temp);
						s_record_122_drop(v_next);
						s_record_123_drop(v_element);
					}
				}
			}
			s_record_101_drop(v_call);
		}
		if ((s_is_dict(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) && (s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs)) == 10)))
		{
			s_record_101 v_call = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs);
			if ((s_record_101_get_count_take(s_record_101_copy(v_call)) == 3))
			{
				const int64_t v_method = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs, 0);
				if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_method)) == 4) && (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_method), s_string_from_bytes("insert", sizeof("insert") - 1)) == 0)))
				{
					const int64_t v_receiver = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_method, 0);
					if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_receiver)) == 29) && (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_receiver), s_string_retain(v_name)) == 0)))
					{
						s_record_123 v_key = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs, 1));
						s_record_123 v_value = s_emit_expr(s_record_123_get_state_take(s_record_123_copy(v_key)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_rhs, 2));
						s_record_122 v_next = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_value)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_key)), s_dict_key(s_record_110_get_kind_take(s_record_110_copy(v_symbol))));
						s_record_122 s_value_1134 = s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_value)), s_dict_value(s_record_110_get_kind_take(s_record_110_copy(v_symbol))));
						s_record_122_drop(v_next);
						v_next = s_value_1134;
						if (!((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
							s_record_122 s_return_1135 = s_record_122_copy(v_next);
							s_record_122_drop(v_next);
							s_record_123_drop(v_value);
							s_record_123_drop(v_key);
							s_record_101_drop(v_call);
							s_record_110_drop(v_symbol);
							s_string_release(v_name);
							s_record_101_drop(v_item);
							s_record_122_drop(v_state);
							return s_return_1135;
						}
						const int64_t v_serial = s_record_122_get_counter_take(s_record_122_copy(v_next));
						v_next.counter += 1;
						S_String * v_k = s_string_concat_take(s_string_from_bytes("s_key_", sizeof("s_key_") - 1), s_string_from_i64(v_serial));
						S_String * v_v = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(v_serial));
						s_record_122 s_value_1136 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_key))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_k)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_key))), s_string_from_bytes(";", sizeof(";") - 1)));
						s_record_122_drop(v_next);
						v_next = s_value_1136;
						s_record_122 s_value_1137 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_value))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_v)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_value))), s_string_from_bytes(";", sizeof(";") - 1)));
						s_record_122_drop(v_next);
						v_next = s_value_1137;
						s_record_122 s_return_1138 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_dict_helper(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))), s_string_from_bytes("_insert_take(v_", sizeof("_insert_take(v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_k)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_v)), s_string_from_bytes(");", sizeof(");") - 1)));
						s_string_release(v_v);
						s_string_release(v_k);
						s_record_122_drop(v_next);
						s_record_123_drop(v_value);
						s_record_123_drop(v_key);
						s_record_101_drop(v_call);
						s_record_110_drop(v_symbol);
						s_string_release(v_name);
						s_record_101_drop(v_item);
						s_record_122_drop(v_state);
						return s_return_1138;
						s_string_release(v_v);
						s_string_release(v_k);
						s_record_122_drop(v_next);
						s_record_123_drop(v_value);
						s_record_123_drop(v_key);
					}
				}
			}
			s_record_101_drop(v_call);
		}
		s_record_123 v_result = s_emit_expr(s_record_122_copy(v_state), v_rhs);
		s_record_122 v_next = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_result)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_result)), s_record_110_get_kind_take(s_record_110_copy(v_symbol)));
		if ((s_array_element(s_record_110_get_kind_take(s_record_110_copy(v_symbol))) != 0))
		{
			if ((s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
			{
				s_record_122 s_return_1139 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("unsupported array assignment", sizeof("unsupported array assignment") - 1));
				s_record_122_drop(v_next);
				s_record_123_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1139;
			}
			const int64_t v_serial = s_record_122_get_counter_take(s_record_122_copy(v_next));
			v_next.counter += 1;
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(v_serial));
			s_record_122 s_value_1140 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_110_get_kind_take(s_record_110_copy(v_symbol))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1140;
			if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 7))
			{
				s_record_122 s_value_1141 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_release(v_", sizeof("s_array_i64_release(v_") - 1), s_string_retain(v_name)), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1141;
			}
			else
			{
				s_record_122 s_value_1142 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_release(v_", sizeof("s_array_release(v_") - 1), s_string_retain(v_name)), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1142;
			}
			s_record_122 s_return_1143 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_122_drop(v_next);
			s_record_123_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1143;
			s_string_release(v_temp);
		}
		if (s_is_dict(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
		{
			if ((s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
			{
				s_record_122 s_return_1144 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("unsupported dictionary assignment", sizeof("unsupported dictionary assignment") - 1));
				s_record_122_drop(v_next);
				s_record_123_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1144;
			}
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
			v_next.counter += 1;
			s_record_122 s_value_1145 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_110_get_kind_take(s_record_110_copy(v_symbol))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1145;
			s_record_122 s_value_1146 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_dict_release(v_", sizeof("s_dict_release(v_") - 1), s_string_retain(v_name)), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1146;
			s_record_122 s_return_1147 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_122_drop(v_next);
			s_record_123_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1147;
			s_string_release(v_temp);
		}
		if (((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) >= 100) && (s_record_110_get_kind_take(s_record_110_copy(v_symbol)) < 10000)))
		{
			if ((s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
			{
				s_record_122 s_return_1148 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("unsupported record assignment", sizeof("unsupported record assignment") - 1));
				s_record_122_drop(v_next);
				s_record_123_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1148;
			}
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
			v_next.counter += 1;
			s_record_122 s_value_1149 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_110_get_kind_take(s_record_110_copy(v_symbol))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1149;
			s_record_122 s_value_1150 = s_line(s_record_122_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name))));
			s_record_122_drop(v_next);
			v_next = s_value_1150;
			s_record_122 s_return_1151 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_122_drop(v_next);
			s_record_123_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1151;
			s_string_release(v_temp);
		}
		if (s_is_optional(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
		{
			if ((s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id), s_string_from_bytes("=", sizeof("=") - 1)) != 0))
			{
				s_record_122 s_return_1152 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("unsupported optional assignment", sizeof("unsupported optional assignment") - 1));
				s_record_122_drop(v_next);
				s_record_123_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1152;
			}
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
			v_next.counter += 1;
			s_record_122 s_value_1153 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_110_get_kind_take(s_record_110_copy(v_symbol))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1153;
			if (s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
			{
				s_record_122 s_value_1154 = s_line(s_record_122_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name))));
				s_record_122_drop(v_next);
				v_next = s_value_1154;
			}
			s_record_122 s_return_1155 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_temp);
			s_record_122_drop(v_next);
			s_record_123_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1155;
			s_string_release(v_temp);
		}
		if ((s_record_110_get_kind_take(s_record_110_copy(v_symbol)) == 4))
		{
			const int64_t v_serial = s_record_122_get_counter_take(s_record_122_copy(v_next));
			v_next.counter += 1;
			S_String * v_temp = s_string_concat_take(s_string_from_bytes("s_value_", sizeof("s_value_") - 1), s_string_from_i64(v_serial));
			S_String * v_operator = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
			if ((s_string_compare_take(s_string_retain(v_operator), s_string_from_bytes("+=", sizeof("+=") - 1)) == 0))
			{
				s_record_122 s_value_1156 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("S_String *", sizeof("S_String *") - 1), s_string_retain(v_temp)), s_string_from_bytes(" = s_string_concat_take(s_string_retain(v_", sizeof(" = s_string_concat_take(s_string_retain(v_") - 1)), s_string_retain(v_name)), s_string_from_bytes("), ", sizeof("), ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1156;
			}
			else if ((s_string_compare_take(s_string_retain(v_operator), s_string_from_bytes("=", sizeof("=") - 1)) == 0))
			{
				s_record_122 s_value_1157 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("S_String *", sizeof("S_String *") - 1), s_string_retain(v_temp)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1157;
			}
			else
			{
				s_record_122 s_return_1158 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("unsupported string assignment", sizeof("unsupported string assignment") - 1));
				s_string_release(v_operator);
				s_string_release(v_temp);
				s_record_122_drop(v_next);
				s_record_123_drop(v_result);
				s_record_110_drop(v_symbol);
				s_string_release(v_name);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1158;
			}
			s_record_122 s_value_1159 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_string_release(v_", sizeof("s_string_release(v_") - 1), s_string_retain(v_name)), s_string_from_bytes(");", sizeof(");") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1159;
			s_record_122 s_return_1160 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_temp)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_string_release(v_operator);
			s_string_release(v_temp);
			s_record_122_drop(v_next);
			s_record_123_drop(v_result);
			s_record_110_drop(v_symbol);
			s_string_release(v_name);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1160;
			s_string_release(v_operator);
			s_string_release(v_temp);
		}
		s_record_122 s_return_1161 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_result))), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_122_drop(v_next);
		s_record_123_drop(v_result);
		s_record_110_drop(v_symbol);
		s_string_release(v_name);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1161;
		s_record_122_drop(v_next);
		s_record_123_drop(v_result);
		s_record_110_drop(v_symbol);
		s_string_release(v_name);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 35))
	{
		if ((s_record_101_get_count_take(s_record_101_copy(v_item)) == 0))
		{
			if ((s_record_122_get_return_kind_take(s_record_122_copy(v_state)) != 5))
			{
				s_record_122 s_return_1162 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("non-void function must return a value", sizeof("non-void function must return a value") - 1));
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1162;
			}
			s_record_122 s_return_1163 = s_line(s_record_122_copy(v_state), s_string_from_bytes("return;", sizeof("return;") - 1));
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1163;
		}
		s_record_122 s_return_1164 = s_emit_return_value(s_record_122_copy(v_state), v_id, s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1164;
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 34))
	{
		{
			S_Array * v_s_iter_1165 = s_record_122_get_symbols_take(s_record_122_copy(v_state));
			for (int64_t s_index_1165 = 0; s_index_1165 < (int64_t)s_array_length(v_s_iter_1165); ++s_index_1165)
			{
				s_record_110 v_symbol = s_ga_10110_get_borrow(v_s_iter_1165, s_index_1165);
				if (s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol))))
				{
					s_record_122 s_return_1166 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("managed recur requires ownership lowering", sizeof("managed recur requires ownership lowering") - 1));
					s_record_110_drop(v_symbol);
					s_array_release(v_s_iter_1165);
					s_record_101_drop(v_item);
					s_record_122_drop(v_state);
					return s_return_1166;
				}
				s_record_110_drop(v_symbol);
			}
			s_array_release(v_s_iter_1165);
		}
		if ((s_record_101_get_count_take(s_record_101_copy(v_item)) != 1))
		{
			s_record_122 s_return_1167 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("recur expects a direct self-call", sizeof("recur expects a direct self-call") - 1));
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1167;
		}
		const int64_t v_call = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0);
		s_record_101 v_entry = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_call);
		if (((s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 10) || (s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_call, 0))) != 29)))
		{
			s_record_122 s_return_1168 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("recur expects a direct self-call", sizeof("recur expects a direct self-call") - 1));
			s_record_101_drop(v_entry);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1168;
		}
		S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_call, 0));
		if ((s_string_compare_take(s_string_retain(v_name), s_record_122_get_function_name_take(s_record_122_copy(v_state))) != 0))
		{
			s_record_122 s_return_1169 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("recur must call the enclosing function", sizeof("recur must call the enclosing function") - 1));
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1169;
		}
		S_Array * v_params = s_record_122_get_active_params_take(s_record_122_copy(v_state));
		if (((s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1) != s_generic_array_length_take(s_array_retain(v_params))))
		{
			s_record_122 s_return_1170 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("recur argument count mismatch", sizeof("recur argument count mismatch") - 1));
			s_array_release(v_params);
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1170;
		}
		s_record_122 v_next = s_record_122_copy(v_state);
		const int64_t v_serial = s_record_122_get_counter_take(s_record_122_copy(v_next));
		v_next.counter += 1;
		for (int64_t v_i = 0, s_bound_1171 = s_generic_array_length_take(s_array_retain(v_params)); v_i < s_bound_1171; ++v_i)
		{
			s_record_123 v_arg = s_emit_expr(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_call, (v_i + 1)));
			s_record_122 s_value_1172 = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_arg)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_arg)), s_record_104_get_kind_take(s_ga_10104_get_checked_take(s_array_retain(v_params), v_i)));
			s_record_122_drop(v_next);
			v_next = s_value_1172;
			if (!((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
				s_record_122 s_return_1173 = s_record_122_copy(v_next);
				s_record_123_drop(v_arg);
				s_record_122_drop(v_next);
				s_array_release(v_params);
				s_string_release(v_name);
				s_record_101_drop(v_entry);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1173;
			}
			S_String * s_left_1174 = s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_arg))), s_string_from_bytes(" s_tail_", sizeof(" s_tail_") - 1));
			S_String * s_right_1175 = s_string_from_i64(v_serial);
			S_String * s_left_1176 = s_string_concat_take(s_string_concat_take(s_left_1174, s_right_1175), s_string_from_bytes("_", sizeof("_") - 1));
			S_String * s_right_1177 = s_string_from_i64(v_i);
			s_record_122 s_value_1178 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_left_1176, s_right_1177), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_arg))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1178;
			s_record_123_drop(v_arg);
		}
		for (int64_t v_i = 0, s_bound_1179 = s_generic_array_length_take(s_array_retain(v_params)); v_i < s_bound_1179; ++v_i)
		{
			S_String * s_left_1180 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_104_get_name_take(s_ga_10104_get_checked_take(s_array_retain(v_params), v_i))), s_string_from_bytes(" = s_tail_", sizeof(" = s_tail_") - 1)), s_string_from_i64(v_serial)), s_string_from_bytes("_", sizeof("_") - 1));
			S_String * s_right_1181 = s_string_from_i64(v_i);
			s_record_122 s_value_1182 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_left_1180, s_right_1181), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1182;
		}
		s_record_122 s_return_1183 = s_line(s_record_122_copy(v_next), s_string_from_bytes("goto s_recur;", sizeof("goto s_recur;") - 1));
		s_record_122_drop(v_next);
		s_array_release(v_params);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1183;
		s_record_122_drop(v_next);
		s_array_release(v_params);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 43))
	{
		s_record_123 v_condition = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		s_record_122 v_next = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_condition)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_condition)), 3);
		if (!((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_next)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
			s_record_122 s_return_1184 = s_record_122_copy(v_next);
			s_record_122_drop(v_next);
			s_record_123_drop(v_condition);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1184;
		}
		s_record_122 s_value_1185 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (!(", sizeof("if (!(") - 1), s_record_123_get_text_take(s_record_123_copy(v_condition))), s_string_from_bytes(")) {", sizeof(")) {") - 1)));
		s_record_122_drop(v_next);
		v_next = s_value_1185;
		v_next.indentation += 1;
		s_record_122 s_value_1186 = s_emit_statement(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_id, 1));
		s_record_122_drop(v_next);
		v_next = s_value_1186;
		v_next.indentation -= 1;
		s_record_122 s_return_1187 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
		s_record_122_drop(v_next);
		s_record_123_drop(v_condition);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1187;
		s_record_122_drop(v_next);
		s_record_123_drop(v_condition);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 18))
	{
		s_record_123 v_result = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		if ((s_record_123_get_kind_take(s_record_123_copy(v_result)) != 5))
		{
			s_record_122 s_return_1188 = s_emit_error(s_record_123_get_state_take(s_record_123_copy(v_result)), v_id, s_string_from_bytes("expression statement must be effectful", sizeof("expression statement must be effectful") - 1));
			s_record_123_drop(v_result);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1188;
		}
		s_record_122 s_return_1189 = s_line(s_record_123_get_state_take(s_record_123_copy(v_result)), s_string_concat_take(s_record_123_get_text_take(s_record_123_copy(v_result)), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_123_drop(v_result);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1189;
		s_record_123_drop(v_result);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 22))
	{
		s_record_122 v_next = s_record_122_copy(v_state);
		for (int64_t v_i = 0, s_bound_1190 = s_record_101_get_count_take(s_record_101_copy(v_item)); v_i < s_bound_1190; ++v_i)
		{
			const int64_t v_arm = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, v_i);
			s_record_101 v_branch = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_arm);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_branch)) == 16))
			{
				s_record_122 s_value_1191 = s_line(s_record_122_copy(v_next), s_string_from_bytes("else", sizeof("else") - 1));
				s_record_122_drop(v_next);
				v_next = s_value_1191;
				s_record_122 s_value_1192 = s_emit_nested_block(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_arm, 0));
				s_record_122_drop(v_next);
				v_next = s_value_1192;
			}
			else
			{
				if (((v_i > 0) && s_uses_optional_fallback(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_arm, 0))))
				{
					s_record_122 s_return_1193 = s_emit_error(s_record_122_copy(v_next), v_arm, s_string_from_bytes("optional fallback in elif requires CFG lowering", sizeof("optional fallback in elif requires CFG lowering") - 1));
					s_record_101_drop(v_branch);
					s_record_122_drop(v_next);
					s_record_101_drop(v_item);
					s_record_122_drop(v_state);
					return s_return_1193;
				}
				s_record_123 v_cond = s_emit_expr(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_arm, 0));
				s_record_122 s_value_1194 = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_cond)), v_arm, s_record_123_get_kind_take(s_record_123_copy(v_cond)), 3);
				s_record_122_drop(v_next);
				v_next = s_value_1194;
				S_String * v_header = s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (", sizeof("if (") - 1), s_record_123_get_text_take(s_record_123_copy(v_cond))), s_string_from_bytes(")", sizeof(")") - 1));
				if ((v_i > 0))
				{
					S_String *s_value_1195 = s_string_concat_take(s_string_from_bytes("else ", sizeof("else ") - 1), s_string_retain(v_header));
					s_string_release(v_header);
					v_header = s_value_1195;
				}
				s_record_122 s_value_1196 = s_line(s_record_122_copy(v_next), s_string_retain(v_header));
				s_record_122_drop(v_next);
				v_next = s_value_1196;
				s_record_122 s_value_1197 = s_emit_nested_block(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_arm, 1));
				s_record_122_drop(v_next);
				v_next = s_value_1197;
				s_string_release(v_header);
				s_record_123_drop(v_cond);
			}
			s_record_101_drop(v_branch);
		}
		s_record_122 s_return_1198 = s_record_122_copy(v_next);
		s_record_122_drop(v_next);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1198;
		s_record_122_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 41))
	{
		s_record_122 v_next = s_line(s_record_122_copy(v_state), s_string_from_bytes("while (true) {", sizeof("while (true) {") - 1));
		v_next.indentation += 1;
		v_next.scope_depth += 1;
		v_next.loop_depth += 1;
		const int64_t v_previous_loop_scope = s_record_122_get_loop_scope_take(s_record_122_copy(v_next));
		v_next.loop_scope = s_record_122_get_scope_depth_take(s_record_122_copy(v_next));
		s_record_123 v_cond = s_emit_expr(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_id, 0));
		s_record_122 s_value_1199 = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_cond)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_cond)), 3);
		s_record_122_drop(v_next);
		v_next = s_value_1199;
		s_record_122 s_value_1200 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("if (!(", sizeof("if (!(") - 1), s_record_123_get_text_take(s_record_123_copy(v_cond))), s_string_from_bytes(")) break;", sizeof(")) break;") - 1)));
		s_record_122_drop(v_next);
		v_next = s_value_1200;
		s_record_122 s_value_1201 = s_emit_block(s_record_122_copy(v_next), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_id, 1));
		s_record_122_drop(v_next);
		v_next = s_value_1201;
		v_next.loop_depth -= 1;
		v_next.loop_scope = v_previous_loop_scope;
		v_next.scope_depth -= 1;
		v_next.indentation -= 1;
		s_record_122 s_return_1202 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
		s_record_123_drop(v_cond);
		s_record_122_drop(v_next);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1202;
		s_record_123_drop(v_cond);
		s_record_122_drop(v_next);
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 21))
	{
		const int64_t v_iterable = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_item)) - 2));
		const int64_t v_body = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_item)) - 1));
		if (((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_iterable)) != 5) || (s_string_compare_take(s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_iterable), s_string_from_bytes("..", sizeof("..") - 1)) != 0)))
		{
			s_record_123 v_source = s_emit_expr(s_record_122_copy(v_state), v_iterable);
			s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_source));
			const int64_t v_element_kind = s_array_element(s_record_123_get_kind_take(s_record_123_copy(v_source)));
			if ((v_element_kind == 0))
			{
				s_record_122 s_return_1203 = s_emit_error(s_record_122_copy(v_next), v_id, s_string_from_bytes("for expects an array or range", sizeof("for expects an array or range") - 1));
				s_record_122_drop(v_next);
				s_record_123_drop(v_source);
				s_record_101_drop(v_item);
				s_record_122_drop(v_state);
				return s_return_1203;
			}
			const int64_t v_original = s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)));
			const int64_t v_serial = s_record_122_get_counter_take(s_record_122_copy(v_next));
			v_next.counter += 1;
			S_String * v_iter = s_string_concat_take(s_string_from_bytes("s_iter_", sizeof("s_iter_") - 1), s_string_from_i64(v_serial));
			S_String * v_index = s_string_concat_take(s_string_from_bytes("s_index_", sizeof("s_index_") - 1), s_string_from_i64(v_serial));
			S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_id);
			s_record_122 s_value_1204 = s_line(s_record_122_copy(v_next), s_string_from_bytes("{", sizeof("{") - 1));
			s_record_122_drop(v_next);
			v_next = s_value_1204;
			v_next.indentation += 1;
			v_next.scope_depth += 1;
			s_record_122 s_value_1205 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_123_get_kind_take(s_record_123_copy(v_source))), s_string_from_bytes(" v_", sizeof(" v_") - 1)), s_string_retain(v_iter)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_source))), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1205;
			s_record_122 s_value_1206 = s_append_symbol(s_record_122_copy(v_next), v_id, s_string_retain(v_iter), s_record_123_get_kind_take(s_record_123_copy(v_source)), false);
			s_record_122_drop(v_next);
			v_next = s_value_1206;
			S_String * v_length = s_string_concat_take(s_string_concat_take(s_string_from_bytes("(int64_t)v_", sizeof("(int64_t)v_") - 1), s_string_retain(v_iter)), s_string_from_bytes("->length", sizeof("->length") - 1));
			if (s_generic_array(s_record_123_get_kind_take(s_record_123_copy(v_source))))
			{
				S_String *s_value_1207 = s_string_concat_take(s_string_concat_take(s_string_from_bytes("(int64_t)s_array_length(v_", sizeof("(int64_t)s_array_length(v_") - 1), s_string_retain(v_iter)), s_string_from_bytes(")", sizeof(")") - 1));
				s_string_release(v_length);
				v_length = s_value_1207;
			}
			s_record_122 s_value_1208 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("for (int64_t ", sizeof("for (int64_t ") - 1), s_string_retain(v_index)), s_string_from_bytes(" = 0; ", sizeof(" = 0; ") - 1)), s_string_retain(v_index)), s_string_from_bytes(" < ", sizeof(" < ") - 1)), s_string_retain(v_length)), s_string_from_bytes("; ++", sizeof("; ++") - 1)), s_string_retain(v_index)), s_string_from_bytes(")", sizeof(")") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1208;
			s_record_122 s_value_1209 = s_line(s_record_122_copy(v_next), s_string_from_bytes("{", sizeof("{") - 1));
			s_record_122_drop(v_next);
			v_next = s_value_1209;
			v_next.indentation += 1;
			v_next.scope_depth += 1;
			v_next.loop_depth += 1;
			const int64_t v_previous_loop_scope = s_record_122_get_loop_scope_take(s_record_122_copy(v_next));
			v_next.loop_scope = s_record_122_get_scope_depth_take(s_record_122_copy(v_next));
			S_String * v_getter = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_get_checked(v_", sizeof("s_array_i64_get_checked(v_") - 1), s_string_retain(v_iter)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_index)), s_string_from_bytes(")", sizeof(")") - 1));
			if (s_generic_array(s_record_123_get_kind_take(s_record_123_copy(v_source))))
			{
				S_String *s_value_1210 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_array_helper(s_record_123_get_kind_take(s_record_123_copy(v_source))), s_string_from_bytes("_get_borrow(v_", sizeof("_get_borrow(v_") - 1)), s_string_retain(v_iter)), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_index)), s_string_from_bytes(")", sizeof(")") - 1));
				s_string_release(v_getter);
				v_getter = s_value_1210;
			}
			s_record_122 s_value_1211 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(v_element_kind), s_string_from_bytes(" v_", sizeof(" v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_getter)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1211;
			s_record_122 s_value_1212 = s_append_symbol(s_record_122_copy(v_next), v_id, s_string_retain(v_name), v_element_kind, false);
			s_record_122_drop(v_next);
			v_next = s_value_1212;
			s_record_122 s_value_1213 = s_emit_block(s_record_122_copy(v_next), v_body);
			s_record_122_drop(v_next);
			v_next = s_value_1213;
			if (s_managed(v_element_kind))
			{
				s_record_122 s_value_1214 = s_line(s_record_122_copy(v_next), s_record_drop(v_element_kind, s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name))));
				s_record_122_drop(v_next);
				v_next = s_value_1214;
			}
			v_next.loop_depth -= 1;
			v_next.loop_scope = v_previous_loop_scope;
			v_next.scope_depth -= 1;
			v_next.indentation -= 1;
			s_record_122 s_value_1215 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_122_drop(v_next);
			v_next = s_value_1215;
			if ((s_record_123_get_kind_take(s_record_123_copy(v_source)) == 7))
			{
				s_record_122 s_value_1216 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_i64_release(v_", sizeof("s_array_i64_release(v_") - 1), s_string_retain(v_iter)), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1216;
			}
			else
			{
				s_record_122 s_value_1217 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("s_array_release(v_", sizeof("s_array_release(v_") - 1), s_string_retain(v_iter)), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_next);
				v_next = s_value_1217;
			}
			v_next.scope_depth -= 1;
			v_next.indentation -= 1;
			s_record_122 s_value_1218 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
			s_record_122_drop(v_next);
			v_next = s_value_1218;
			S_Array * v_symbols = s_ga_10110_make_take(0, NULL);
			for (int64_t v_i = 0, s_bound_1219 = v_original; v_i < s_bound_1219; ++v_i)
			{
				s_record_110 s_item_1220 = s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)), v_i);
				v_symbols = s_ga_10110_append_take(v_symbols, s_item_1220);
			}
			S_Array * s_field_1221 = s_array_retain(v_symbols);
			s_array_release(v_next.symbols);
			v_next.symbols = s_field_1221;
			s_record_122 s_return_1222 = s_record_122_copy(v_next);
			s_array_release(v_symbols);
			s_string_release(v_getter);
			s_string_release(v_length);
			s_string_release(v_name);
			s_string_release(v_index);
			s_string_release(v_iter);
			s_record_122_drop(v_next);
			s_record_123_drop(v_source);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1222;
			s_array_release(v_symbols);
			s_string_release(v_getter);
			s_string_release(v_length);
			s_string_release(v_name);
			s_string_release(v_index);
			s_string_release(v_iter);
			s_record_122_drop(v_next);
			s_record_123_drop(v_source);
		}
		s_record_123 v_from = s_emit_expr(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_iterable, 0));
		s_record_123 v_to = s_emit_expr(s_record_123_get_state_take(s_record_123_copy(v_from)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_iterable, 1));
		s_record_122 v_next = s_check_type(s_record_123_get_state_take(s_record_123_copy(v_to)), v_id, s_record_123_get_kind_take(s_record_123_copy(v_from)), 1);
		s_record_122 s_value_1223 = s_check_type(s_record_122_copy(v_next), v_id, s_record_123_get_kind_take(s_record_123_copy(v_to)), 1);
		s_record_122_drop(v_next);
		v_next = s_value_1223;
		S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_id);
		S_String * v_bound = s_string_concat_take(s_string_from_bytes("s_bound_", sizeof("s_bound_") - 1), s_string_from_i64(s_record_122_get_counter_take(s_record_122_copy(v_next))));
		v_next.counter += 1;
		s_record_122 s_value_1224 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("for (int64_t v_", sizeof("for (int64_t v_") - 1), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_from))), s_string_from_bytes(", ", sizeof(", ") - 1)), s_string_retain(v_bound)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_to))), s_string_from_bytes("; v_", sizeof("; v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(" < ", sizeof(" < ") - 1)), s_string_retain(v_bound)), s_string_from_bytes("; ++v_", sizeof("; ++v_") - 1)), s_string_retain(v_name)), s_string_from_bytes(")", sizeof(")") - 1)));
		s_record_122_drop(v_next);
		v_next = s_value_1224;
		v_next.scope_depth += 1;
		s_record_122 s_value_1225 = s_append_symbol(s_record_122_copy(v_next), v_id, s_string_retain(v_name), 1, false);
		s_record_122_drop(v_next);
		v_next = s_value_1225;
		v_next.loop_depth += 1;
		const int64_t v_previous_loop_scope = s_record_122_get_loop_scope_take(s_record_122_copy(v_next));
		v_next.loop_scope = (s_record_122_get_scope_depth_take(s_record_122_copy(v_next)) + 1);
		s_record_122 s_value_1226 = s_emit_nested_block(s_record_122_copy(v_next), v_body);
		s_record_122_drop(v_next);
		v_next = s_value_1226;
		v_next.loop_scope = v_previous_loop_scope;
		v_next.loop_depth -= 1;
		v_next.scope_depth -= 1;
		S_Array * v_restored = s_ga_10110_make_take(0, NULL);
		for (int64_t v_i = 0, s_bound_1227 = s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_state))); v_i < s_bound_1227; ++v_i)
		{
			s_record_110 s_item_1228 = s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)), v_i);
			v_restored = s_ga_10110_append_take(v_restored, s_item_1228);
		}
		S_Array * s_field_1229 = s_array_retain(v_restored);
		s_array_release(v_next.symbols);
		v_next.symbols = s_field_1229;
		s_record_122 s_return_1230 = s_record_122_copy(v_next);
		s_array_release(v_restored);
		s_string_release(v_bound);
		s_string_release(v_name);
		s_record_122_drop(v_next);
		s_record_123_drop(v_to);
		s_record_123_drop(v_from);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1230;
		s_array_release(v_restored);
		s_string_release(v_bound);
		s_string_release(v_name);
		s_record_122_drop(v_next);
		s_record_123_drop(v_to);
		s_record_123_drop(v_from);
	}
	if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 9) || (s_record_101_get_kind_take(s_record_101_copy(v_item)) == 13)))
	{
		if ((s_record_122_get_loop_depth_take(s_record_122_copy(v_state)) == 0))
		{
			s_record_122 s_return_1231 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("loop control outside loop", sizeof("loop control outside loop") - 1));
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1231;
		}
		s_record_122 v_next = s_record_122_copy(v_state);
		for (int64_t v_i = 0, s_bound_1232 = s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_next))); v_i < s_bound_1232; ++v_i)
		{
			s_record_110 v_symbol = s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_next)), ((s_generic_array_length_take(s_record_122_get_symbols_take(s_record_122_copy(v_next))) - 1) - v_i));
			if (((s_record_110_get_depth_take(s_record_110_copy(v_symbol)) >= s_record_122_get_loop_scope_take(s_record_122_copy(v_next))) && s_managed(s_record_110_get_kind_take(s_record_110_copy(v_symbol)))))
			{
				s_record_122 s_value_1233 = s_line(s_record_122_copy(v_next), s_record_drop(s_record_110_get_kind_take(s_record_110_copy(v_symbol)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_110_get_name_take(s_record_110_copy(v_symbol)))));
				s_record_122_drop(v_next);
				v_next = s_value_1233;
			}
			s_record_110_drop(v_symbol);
		}
		if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 9))
		{
			s_record_122 s_return_1234 = s_line(s_record_122_copy(v_next), s_string_from_bytes("break;", sizeof("break;") - 1));
			s_record_122_drop(v_next);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1234;
		}
		s_record_122 s_return_1235 = s_line(s_record_122_copy(v_next), s_string_from_bytes("continue;", sizeof("continue;") - 1));
		s_record_122_drop(v_next);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1235;
		s_record_122_drop(v_next);
	}
	s_record_122 s_return_1236 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unsupported statement '", sizeof("unsupported statement '") - 1), s_node_kind_name(s_record_101_get_kind_take(s_record_101_copy(v_item)))), s_string_from_bytes("'", sizeof("'") - 1)));
	s_record_101_drop(v_item);
	s_record_122_drop(v_state);
	return s_return_1236;
	s_record_101_drop(v_item);
}

s_record_122 s_collect_function(s_record_122 v_state, s_record_114 v_header)
{
	const int64_t v_id = s_record_114_get_source_take(s_record_114_copy(v_header));
	S_String * v_name = s_record_114_get_name_take(s_record_114_copy(v_header));
	S_Array * v_params = s_ga_10104_make_take(0, NULL);
	int64_t v_result = 0;
	{
		S_Array * v_s_iter_1237 = s_record_114_get_parameters_take(s_record_114_copy(v_header));
		for (int64_t s_index_1237 = 0; s_index_1237 < (int64_t)s_array_length(v_s_iter_1237); ++s_index_1237)
		{
			s_record_113 v_parameter = s_ga_10113_get_borrow(v_s_iter_1237, s_index_1237);
			int64_t v_kind = 0;
			if ((s_record_113_get_type_node_take(s_record_113_copy(v_parameter)) >= 0))
			{
				v_kind = s_resolve_type(s_record_122_copy(v_state), s_record_113_get_type_node_take(s_record_113_copy(v_parameter)));
				if (((v_kind == 0) || (v_kind == 5)))
				{
					s_record_122 s_return_1238 = s_emit_error(s_record_122_copy(v_state), s_record_113_get_source_take(s_record_113_copy(v_parameter)), s_string_from_bytes("unsupported parameter type", sizeof("unsupported parameter type") - 1));
					s_record_113_drop(v_parameter);
					s_array_release(v_s_iter_1237);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_114_drop(v_header);
					s_record_122_drop(v_state);
					return s_return_1238;
				}
			}
			s_record_104 s_item_1239 = s_ParameterInfo(s_record_113_get_name_take(s_record_113_copy(v_parameter)), v_kind);
			v_params = s_ga_10104_append_take(v_params, s_item_1239);
			s_record_113_drop(v_parameter);
		}
		s_array_release(v_s_iter_1237);
	}
	if ((s_record_114_get_result_type_node_take(s_record_114_copy(v_header)) >= 0))
	{
		v_result = s_resolve_type(s_record_122_copy(v_state), s_record_114_get_result_type_node_take(s_record_114_copy(v_header)));
	}
	s_record_122 v_next = s_record_122_copy(v_state);
	const bool v_constructor = s_record_114_get_constructor_take(s_record_114_copy(v_header));
	if (v_constructor)
	{
		{
			S_Array * v_s_iter_1240 = s_array_retain(v_params);
			for (int64_t s_index_1240 = 0; s_index_1240 < (int64_t)s_array_length(v_s_iter_1240); ++s_index_1240)
			{
				s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_1240, s_index_1240);
				if ((s_record_104_get_kind_take(s_record_104_copy(v_param)) == 0))
				{
					s_record_122 s_return_1241 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("generic constructors are not supported yet", sizeof("generic constructors are not supported yet") - 1));
					s_record_104_drop(v_param);
					s_array_release(v_s_iter_1240);
					s_record_122_drop(v_next);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_114_drop(v_header);
					s_record_122_drop(v_state);
					return s_return_1241;
				}
				s_record_104_drop(v_param);
			}
			s_array_release(v_s_iter_1240);
		}
		const int64_t v_body_id = s_record_114_get_body_take(s_record_114_copy(v_header));
		s_record_101 v_body = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_body_id);
		s_record_122 v_probe = s_record_122_copy(v_state);
		S_Array * s_field_1242 = s_ga_10110_make_take(0, NULL);
		s_array_release(v_probe.symbols);
		v_probe.symbols = s_field_1242;
		S_Array * v_fields = s_ga_10104_make_take(0, NULL);
		S_Array * v_semantic_fields = s_ga_10104_make_take(0, NULL);
		{
			S_Array * v_s_iter_1243 = s_array_retain(v_params);
			for (int64_t s_index_1243 = 0; s_index_1243 < (int64_t)s_array_length(v_s_iter_1243); ++s_index_1243)
			{
				s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_1243, s_index_1243);
				s_record_122 s_value_1244 = s_append_symbol(s_record_122_copy(v_probe), v_id, s_record_104_get_name_take(s_record_104_copy(v_param)), s_record_104_get_kind_take(s_record_104_copy(v_param)), false);
				s_record_122_drop(v_probe);
				v_probe = s_value_1244;
				s_record_104_drop(v_param);
			}
			s_array_release(v_s_iter_1243);
		}
		for (int64_t v_i = 0, s_bound_1245 = s_record_101_get_count_take(s_record_101_copy(v_body)); v_i < s_bound_1245; ++v_i)
		{
			const int64_t v_pub_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_body_id, v_i);
			s_record_101 v_pub = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_pub_id);
			if ((s_record_101_get_kind_take(s_record_101_copy(v_pub)) != 32))
			{
				s_record_122 s_return_1246 = s_emit_error(s_record_122_copy(v_state), v_pub_id, s_string_from_bytes("constructors currently require direct pub statements", sizeof("constructors currently require direct pub statements") - 1));
				s_record_101_drop(v_pub);
				s_array_release(v_semantic_fields);
				s_array_release(v_fields);
				s_record_122_drop(v_probe);
				s_record_101_drop(v_body);
				s_record_122_drop(v_next);
				s_array_release(v_params);
				s_string_release(v_name);
				s_record_114_drop(v_header);
				s_record_122_drop(v_state);
				return s_return_1246;
			}
			S_String * v_field_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_pub_id);
			int64_t v_kind = 0;
			if ((s_record_101_get_count_take(s_record_101_copy(v_pub)) == 0))
			{
				const int64_t v_symbol = s_find_symbol(s_record_122_copy(v_probe), s_string_retain(v_field_name));
				if ((v_symbol < 0))
				{
					s_record_122 s_return_1247 = s_emit_error(s_record_122_copy(v_state), v_pub_id, s_string_concat_take(s_string_concat_take(s_string_from_bytes("unknown published binding '", sizeof("unknown published binding '") - 1), s_string_retain(v_field_name)), s_string_from_bytes("'", sizeof("'") - 1)));
					s_string_release(v_field_name);
					s_record_101_drop(v_pub);
					s_array_release(v_semantic_fields);
					s_array_release(v_fields);
					s_record_122_drop(v_probe);
					s_record_101_drop(v_body);
					s_record_122_drop(v_next);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_114_drop(v_header);
					s_record_122_drop(v_state);
					return s_return_1247;
				}
				v_kind = s_record_110_get_kind_take(s_ga_10110_get_checked_take(s_record_122_get_symbols_take(s_record_122_copy(v_probe)), v_symbol));
			}
			else
			{
				const int64_t v_value_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_pub_id, 0);
				if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_value_id)) == 4))
				{
					S_String *s_value_1248 = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_value_id);
					s_string_release(v_field_name);
					v_field_name = s_value_1248;
				}
				s_record_123 v_value = s_emit_expr(s_record_122_copy(v_probe), v_value_id);
				s_record_122 s_value_1249 = s_record_123_get_state_take(s_record_123_copy(v_value));
				s_record_122_drop(v_probe);
				v_probe = s_value_1249;
				if (!((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_probe)), s_string_from_bytes("", sizeof("") - 1)) == 0))) {
					s_record_122 s_return_1250 = s_record_122_copy(v_probe);
					s_record_123_drop(v_value);
					s_string_release(v_field_name);
					s_record_101_drop(v_pub);
					s_array_release(v_semantic_fields);
					s_array_release(v_fields);
					s_record_122_drop(v_probe);
					s_record_101_drop(v_body);
					s_record_122_drop(v_next);
					s_array_release(v_params);
					s_string_release(v_name);
					s_record_114_drop(v_header);
					s_record_122_drop(v_state);
					return s_return_1250;
				}
				v_kind = s_record_123_get_kind_take(s_record_123_copy(v_value));
				s_record_123_drop(v_value);
			}
			s_record_107 v_canonical = s_canonical_kind(s_record_122_get_types_take(s_record_122_copy(v_probe)), s_record_122_get_records_take(s_record_122_copy(v_probe)), v_kind);
			s_record_106 s_field_1251 = s_record_107_get_table_take(s_record_107_copy(v_canonical));
			s_record_106_drop(v_probe.types);
			v_probe.types = s_field_1251;
			if (((s_record_107_get_id_take(s_record_107_copy(v_canonical)) == 0) || (s_record_107_get_id_take(s_record_107_copy(v_canonical)) == 5)))
			{
				s_record_122 s_return_1252 = s_emit_error(s_record_122_copy(v_state), v_pub_id, s_string_from_bytes("constructor fields currently require unmanaged values", sizeof("constructor fields currently require unmanaged values") - 1));
				s_record_107_drop(v_canonical);
				s_string_release(v_field_name);
				s_record_101_drop(v_pub);
				s_array_release(v_semantic_fields);
				s_array_release(v_fields);
				s_record_122_drop(v_probe);
				s_record_101_drop(v_body);
				s_record_122_drop(v_next);
				s_array_release(v_params);
				s_string_release(v_name);
				s_record_114_drop(v_header);
				s_record_122_drop(v_state);
				return s_return_1252;
			}
			s_record_112 v_edit = s_hir_constructor_publish(s_array_retain(v_semantic_fields), s_string_retain(v_field_name), s_record_107_get_id_take(s_record_107_copy(v_canonical)));
			if ((s_string_compare_take(s_record_112_get_error_take(s_record_112_copy(v_edit)), s_string_from_bytes("", sizeof("") - 1)) != 0))
			{
				s_record_122 s_return_1253 = s_emit_error(s_record_122_copy(v_state), v_pub_id, s_record_112_get_error_take(s_record_112_copy(v_edit)));
				s_record_112_drop(v_edit);
				s_record_107_drop(v_canonical);
				s_string_release(v_field_name);
				s_record_101_drop(v_pub);
				s_array_release(v_semantic_fields);
				s_array_release(v_fields);
				s_record_122_drop(v_probe);
				s_record_101_drop(v_body);
				s_record_122_drop(v_next);
				s_array_release(v_params);
				s_string_release(v_name);
				s_record_114_drop(v_header);
				s_record_122_drop(v_state);
				return s_return_1253;
			}
			S_Array * s_value_1254 = s_record_112_get_fields_take(s_record_112_copy(v_edit));
			s_array_release(v_semantic_fields);
			v_semantic_fields = s_value_1254;
			s_record_104 s_item_1255 = s_RecordField(s_string_retain(v_field_name), v_kind);
			v_fields = s_ga_10104_append_take(v_fields, s_item_1255);
			s_record_112_drop(v_edit);
			s_record_107_drop(v_canonical);
			s_string_release(v_field_name);
			s_record_101_drop(v_pub);
		}
		if ((s_generic_array_length_take(s_array_retain(v_fields)) == 0))
		{
			s_record_122 s_return_1256 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("constructor must publish at least one field", sizeof("constructor must publish at least one field") - 1));
			s_array_release(v_semantic_fields);
			s_array_release(v_fields);
			s_record_122_drop(v_probe);
			s_record_101_drop(v_body);
			s_record_122_drop(v_next);
			s_array_release(v_params);
			s_string_release(v_name);
			s_record_114_drop(v_header);
			s_record_122_drop(v_state);
			return s_return_1256;
		}
		s_record_111 v_resolved = s_resolve_hir_record(s_record_122_get_types_take(s_record_122_copy(v_probe)), s_array_retain(v_semantic_fields));
		if ((s_string_compare_take(s_record_111_get_error_take(s_record_111_copy(v_resolved)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_122 s_return_1257 = s_emit_error(s_record_122_copy(v_state), v_id, s_record_111_get_error_take(s_record_111_copy(v_resolved)));
			s_record_111_drop(v_resolved);
			s_array_release(v_semantic_fields);
			s_array_release(v_fields);
			s_record_122_drop(v_probe);
			s_record_101_drop(v_body);
			s_record_122_drop(v_next);
			s_array_release(v_params);
			s_string_release(v_name);
			s_record_114_drop(v_header);
			s_record_122_drop(v_state);
			return s_return_1257;
		}
		s_record_106 s_field_1258 = s_record_111_get_table_take(s_record_111_copy(v_resolved));
		s_record_106_drop(v_probe.types);
		v_probe.types = s_field_1258;
		s_record_123 v_record = s_intern_record(s_record_122_copy(v_probe), s_array_retain(v_fields));
		s_record_122 s_value_1259 = s_record_123_get_state_take(s_record_123_copy(v_record));
		s_record_122_drop(v_next);
		v_next = s_value_1259;
		S_Array * s_field_1260 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_next)), s_HirNode(v_id, 8, s_record_111_get_type_id_take(s_record_111_copy(v_resolved)), (-1)));
		s_array_release(v_next.hir);
		v_next.hir = s_field_1260;
		v_result = s_record_123_get_kind_take(s_record_123_copy(v_record));
		s_record_123_drop(v_record);
		s_record_111_drop(v_resolved);
		s_array_release(v_semantic_fields);
		s_array_release(v_fields);
		s_record_122_drop(v_probe);
		s_record_101_drop(v_body);
	}
	S_Array * s_field_1261 = s_ga_10118_append_take(s_record_122_get_functions_take(s_record_122_copy(v_next)), s_FunctionInfo(s_string_retain(v_name), v_result, s_array_retain(v_params), s_record_114_get_body_take(s_record_114_copy(v_header)), v_constructor, s_string_concat_take(s_string_from_bytes("s_", sizeof("s_") - 1), s_string_retain(v_name))));
	s_array_release(v_next.functions);
	v_next.functions = s_field_1261;
	s_record_122 s_return_1262 = s_record_122_copy(v_next);
	s_record_122_drop(v_next);
	s_array_release(v_params);
	s_string_release(v_name);
	s_record_114_drop(v_header);
	s_record_122_drop(v_state);
	return s_return_1262;
	s_record_122_drop(v_next);
	s_array_release(v_params);
	s_string_release(v_name);
}

s_record_123 s_evaluate_constant(s_record_122 v_state, int64_t v_id)
{
	s_record_101 v_item = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	S_String * v_value = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 24))
	{
		s_record_123 s_return_1263 = s_ExprCode(s_record_122_copy(v_state), s_string_retain(v_value), 1);
		s_string_release(v_value);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1263;
	}
	if ((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 29))
	{
		S_String * v_found = s_find_constant(s_record_122_copy(v_state), s_string_retain(v_value));
		if ((s_string_compare_take(s_string_retain(v_found), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_123 s_return_1264 = s_ExprCode(s_record_122_copy(v_state), s_string_retain(v_found), 1);
			s_string_release(v_found);
			s_string_release(v_value);
			s_record_101_drop(v_item);
			s_record_122_drop(v_state);
			return s_return_1264;
		}
		s_string_release(v_found);
	}
	if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 39) && ((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("-", sizeof("-") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("+", sizeof("+") - 1)) == 0))))
	{
		s_record_123 v_inner = s_evaluate_constant(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		s_record_123 s_return_1265 = s_ExprCode(s_record_123_get_state_take(s_record_123_copy(v_inner)), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_string_retain(v_value)), s_record_123_get_text_take(s_record_123_copy(v_inner))), s_string_from_bytes(")", sizeof(")") - 1)), 1);
		s_record_123_drop(v_inner);
		s_string_release(v_value);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1265;
		s_record_123_drop(v_inner);
	}
	if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 5) && (((s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("+", sizeof("+") - 1)) == 0) || (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("-", sizeof("-") - 1)) == 0)) || (s_string_compare_take(s_string_retain(v_value), s_string_from_bytes("*", sizeof("*") - 1)) == 0))))
	{
		s_record_123 v_left = s_evaluate_constant(s_record_122_copy(v_state), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0));
		s_record_123 v_right = s_evaluate_constant(s_record_123_get_state_take(s_record_123_copy(v_left)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 1));
		s_record_123 s_return_1266 = s_ExprCode(s_record_123_get_state_take(s_record_123_copy(v_right)), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("(", sizeof("(") - 1), s_record_123_get_text_take(s_record_123_copy(v_left))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_value)), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_123_get_text_take(s_record_123_copy(v_right))), s_string_from_bytes(")", sizeof(")") - 1)), 1);
		s_record_123_drop(v_right);
		s_record_123_drop(v_left);
		s_string_release(v_value);
		s_record_101_drop(v_item);
		s_record_122_drop(v_state);
		return s_return_1266;
		s_record_123_drop(v_right);
		s_record_123_drop(v_left);
	}
	s_record_123 s_return_1267 = s_ExprCode(s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("unsupported module constant initializer", sizeof("unsupported module constant initializer") - 1)), s_string_from_bytes("", sizeof("") - 1), 0);
	s_string_release(v_value);
	s_record_101_drop(v_item);
	s_record_122_drop(v_state);
	return s_return_1267;
	s_string_release(v_value);
	s_record_101_drop(v_item);
}

s_record_122 s_collect_constant(s_record_122 v_state, int64_t v_id)
{
	s_record_101 v_entry = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
	if (((s_record_101_get_kind_take(s_record_101_copy(v_entry)) != 26) || (s_record_101_get_count_take(s_record_101_copy(v_entry)) < 1)))
	{
		s_record_122 s_return_1268 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("unsupported module statement", sizeof("unsupported module statement") - 1));
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_1268;
	}
	if (((s_string_compare_take(s_find_constant(s_record_122_copy(v_state), s_string_retain(v_name)), s_string_from_bytes("", sizeof("") - 1)) != 0) || (s_find_function(s_record_122_copy(v_state), s_string_retain(v_name)) >= 0)))
	{
		s_record_122 s_return_1269 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("duplicate module name", sizeof("duplicate module name") - 1));
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_1269;
	}
	if (((s_record_101_get_count_take(s_record_101_copy(v_entry)) > 1) && (s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0))) == 38)))
	{
		if ((s_ast_type(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, 0)) != 1))
		{
			s_record_122 s_return_1270 = s_emit_error(s_record_122_copy(v_state), v_id, s_string_from_bytes("module constants must have type i64", sizeof("module constants must have type i64") - 1));
			s_string_release(v_name);
			s_record_101_drop(v_entry);
			s_record_122_drop(v_state);
			return s_return_1270;
		}
	}
	const int64_t v_initializer = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id, (s_record_101_get_count_take(s_record_101_copy(v_entry)) - 1));
	s_record_123 v_result = s_evaluate_constant(s_record_122_copy(v_state), v_initializer);
	if ((s_string_compare_take(s_record_122_get_error_take(s_record_123_get_state_take(s_record_123_copy(v_result))), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_122 s_return_1271 = s_record_123_get_state_take(s_record_123_copy(v_result));
		s_record_123_drop(v_result);
		s_string_release(v_name);
		s_record_101_drop(v_entry);
		s_record_122_drop(v_state);
		return s_return_1271;
	}
	s_record_122 v_next = s_record_123_get_state_take(s_record_123_copy(v_result));
	S_Array * s_field_1272 = s_ga_10120_append_take(s_record_122_get_constants_take(s_record_122_copy(v_next)), s_ConstantInfo(s_string_retain(v_name), s_record_123_get_text_take(s_record_123_copy(v_result))));
	s_array_release(v_next.constants);
	v_next.constants = s_field_1272;
	s_record_122 s_return_1273 = s_record_122_copy(v_next);
	s_record_122_drop(v_next);
	s_record_123_drop(v_result);
	s_string_release(v_name);
	s_record_101_drop(v_entry);
	s_record_122_drop(v_state);
	return s_return_1273;
	s_record_122_drop(v_next);
	s_record_123_drop(v_result);
	s_string_release(v_name);
	s_record_101_drop(v_entry);
}

s_record_122 s_emit_function(s_record_122 v_state, s_record_118 v_info)
{
	s_record_122 v_next = s_record_122_copy(v_state);
	S_Array * s_field_1274 = s_ga_10110_make_take(0, NULL);
	s_array_release(v_next.symbols);
	v_next.symbols = s_field_1274;
	v_next.return_kind = s_record_118_get_kind_take(s_record_118_copy(v_info));
	S_String * s_field_1275 = s_record_118_get_name_take(s_record_118_copy(v_info));
	s_string_release(v_next.function_name);
	v_next.function_name = s_field_1275;
	S_Array * s_field_1276 = s_record_118_get_params_take(s_record_118_copy(v_info));
	s_array_release(v_next.active_params);
	v_next.active_params = s_field_1276;
	S_Array * v_args = s_ga_8_make_take(0, NULL);
	{
		S_Array * v_s_iter_1277 = s_record_118_get_params_take(s_record_118_copy(v_info));
		for (int64_t s_index_1277 = 0; s_index_1277 < (int64_t)s_array_length(v_s_iter_1277); ++s_index_1277)
		{
			s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_1277, s_index_1277);
			S_String * s_item_1278 = s_string_concat_take(s_string_concat_take(s_c_type(s_record_104_get_kind_take(s_record_104_copy(v_param))), s_string_from_bytes(" v_", sizeof(" v_") - 1)), s_record_104_get_name_take(s_record_104_copy(v_param)));
			v_args = s_ga_8_append_take(v_args, s_item_1278);
			s_record_122 s_value_1279 = s_append_symbol(s_record_122_copy(v_next), s_record_118_get_body_take(s_record_118_copy(v_info)), s_record_104_get_name_take(s_record_104_copy(v_param)), s_record_104_get_kind_take(s_record_104_copy(v_param)), false);
			s_record_122_drop(v_next);
			v_next = s_value_1279;
			s_record_104_drop(v_param);
		}
		s_array_release(v_s_iter_1277);
	}
	const int64_t v_header = s_generic_array_length_take(s_record_122_get_lines_take(s_record_122_copy(v_next)));
	s_record_122 s_value_1280 = s_line(s_record_122_copy(v_next), s_string_from_bytes("", sizeof("") - 1));
	s_record_122_drop(v_next);
	v_next = s_value_1280;
	s_record_122 s_value_1281 = s_line(s_record_122_copy(v_next), s_string_from_bytes("{", sizeof("{") - 1));
	s_record_122_drop(v_next);
	v_next = s_value_1281;
	v_next.indentation += 1;
	if (s_contains_recur(s_record_122_get_tree_take(s_record_122_copy(v_next)), s_record_118_get_body_take(s_record_118_copy(v_info))))
	{
		s_record_122 s_value_1282 = s_line(s_record_122_copy(v_next), s_string_from_bytes("s_recur: ;", sizeof("s_recur: ;") - 1));
		s_record_122_drop(v_next);
		v_next = s_value_1282;
	}
	v_next.scope_depth += 1;
	if (s_record_118_get_constructor_take(s_record_118_copy(v_info)))
	{
		s_record_101 v_body = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_next)), s_record_118_get_body_take(s_record_118_copy(v_info)));
		S_String * v_output = s_string_from_bytes("s_constructor_result", sizeof("s_constructor_result") - 1);
		s_record_122 s_value_1283 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_118_get_kind_take(s_record_118_copy(v_info))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_string_retain(v_output)), s_string_from_bytes(" = {0};", sizeof(" = {0};") - 1)));
		s_record_122_drop(v_next);
		v_next = s_value_1283;
		S_Array * v_transferred = s_ga_8_make_take(0, NULL);
		for (int64_t v_i = 0, s_bound_1284 = s_record_101_get_count_take(s_record_101_copy(v_body)); v_i < s_bound_1284; ++v_i)
		{
			const int64_t v_pub_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), s_record_118_get_body_take(s_record_118_copy(v_info)), v_i);
			s_record_101 v_pub = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_pub_id);
			S_String * v_name = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_pub_id);
			S_String * v_expression = s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_string_retain(v_name));
			if ((s_record_101_get_count_take(s_record_101_copy(v_pub)) == 1))
			{
				const int64_t v_value_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_pub_id, 0);
				if ((s_record_101_get_kind_take(s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_value_id)) == 4))
				{
					S_String *s_value_1285 = s_ast_text(s_record_122_get_tree_take(s_record_122_copy(v_next)), v_value_id);
					s_string_release(v_name);
					v_name = s_value_1285;
				}
				s_record_123 v_value = s_emit_expr(s_record_122_copy(v_next), v_value_id);
				s_record_122 s_value_1286 = s_record_123_get_state_take(s_record_123_copy(v_value));
				s_record_122_drop(v_next);
				v_next = s_value_1286;
				S_String *s_value_1287 = s_record_123_get_text_take(s_record_123_copy(v_value));
				s_string_release(v_expression);
				v_expression = s_value_1287;
				s_record_123_drop(v_value);
			}
			else
			{
				S_String * s_item_1288 = s_string_retain(v_name);
				v_transferred = s_ga_8_append_take(v_transferred, s_item_1288);
			}
			s_record_122 s_value_1289 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_string_from_bytes(".", sizeof(".") - 1)), s_string_retain(v_name)), s_string_from_bytes(" = ", sizeof(" = ") - 1)), s_string_retain(v_expression)), s_string_from_bytes(";", sizeof(";") - 1)));
			s_record_122_drop(v_next);
			v_next = s_value_1289;
			s_string_release(v_expression);
			s_string_release(v_name);
			s_record_101_drop(v_pub);
		}
		{
			S_Array * v_s_iter_1290 = s_record_118_get_params_take(s_record_118_copy(v_info));
			for (int64_t s_index_1290 = 0; s_index_1290 < (int64_t)s_array_length(v_s_iter_1290); ++s_index_1290)
			{
				s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_1290, s_index_1290);
				bool v_kept = false;
				{
					S_Array * v_s_iter_1291 = s_array_retain(v_transferred);
					for (int64_t s_index_1291 = 0; s_index_1291 < (int64_t)s_array_length(v_s_iter_1291); ++s_index_1291)
					{
						S_String * v_name = s_ga_8_get_borrow(v_s_iter_1291, s_index_1291);
						if ((s_string_compare_take(s_string_retain(v_name), s_record_104_get_name_take(s_record_104_copy(v_param))) == 0))
						{
							v_kept = true;
						}
						s_string_release(v_name);
					}
					s_array_release(v_s_iter_1291);
				}
				if (((!v_kept) && s_managed(s_record_104_get_kind_take(s_record_104_copy(v_param)))))
				{
					s_record_122 s_value_1292 = s_line(s_record_122_copy(v_next), s_record_drop(s_record_104_get_kind_take(s_record_104_copy(v_param)), s_string_concat_take(s_string_from_bytes("v_", sizeof("v_") - 1), s_record_104_get_name_take(s_record_104_copy(v_param)))));
					s_record_122_drop(v_next);
					v_next = s_value_1292;
				}
				s_record_104_drop(v_param);
			}
			s_array_release(v_s_iter_1290);
		}
		s_record_122 s_value_1293 = s_line(s_record_122_copy(v_next), s_string_concat_take(s_string_concat_take(s_string_from_bytes("return ", sizeof("return ") - 1), s_string_retain(v_output)), s_string_from_bytes(";", sizeof(";") - 1)));
		s_record_122_drop(v_next);
		v_next = s_value_1293;
		s_array_release(v_transferred);
		s_string_release(v_output);
		s_record_101_drop(v_body);
	}
	else
	{
		s_record_122 s_value_1294 = s_emit_block(s_record_122_copy(v_next), s_record_118_get_body_take(s_record_118_copy(v_info)));
		s_record_122_drop(v_next);
		v_next = s_value_1294;
	}
	v_next.scope_depth -= 1;
	v_next.indentation -= 1;
	if ((s_record_122_get_return_kind_take(s_record_122_copy(v_next)) == 0))
	{
		s_record_122 s_return_1295 = s_emit_error(s_record_122_copy(v_next), s_record_118_get_body_take(s_record_118_copy(v_info)), s_string_from_bytes("cannot infer function return type", sizeof("cannot infer function return type") - 1));
		s_array_release(v_args);
		s_record_122_drop(v_next);
		s_record_118_drop(v_info);
		s_record_122_drop(v_state);
		return s_return_1295;
	}
	S_String * s_left_1296 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_122_get_return_kind_take(s_record_122_copy(v_next))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_118_get_symbol_take(s_record_118_copy(v_info))), s_string_from_bytes("(", sizeof("(") - 1));
	S_String * s_right_1297 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_args));
	s_ga_8_set_take(&v_next.lines, v_header, s_string_concat_take(s_string_concat_take(s_left_1296, s_right_1297), s_string_from_bytes(")", sizeof(")") - 1)));
	s_record_122 s_return_1298 = s_line(s_record_122_copy(v_next), s_string_from_bytes("}", sizeof("}") - 1));
	s_array_release(v_args);
	s_record_122_drop(v_next);
	s_record_118_drop(v_info);
	s_record_122_drop(v_state);
	return s_return_1298;
	s_array_release(v_args);
	s_record_122_drop(v_next);
}

s_record_124 s_compile_c(S_String * v_source)
{
	s_record_103 v_parsed = s_parse_module(s_string_retain(v_source));
	if ((s_string_compare_take(s_record_102_get_error_take(s_record_103_get_state_take(s_record_103_copy(v_parsed))), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_124 s_return_1299 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_record_102_get_error_take(s_record_103_get_state_take(s_record_103_copy(v_parsed))), s_record_102_get_error_line_take(s_record_103_get_state_take(s_record_103_copy(v_parsed))));
		s_record_103_drop(v_parsed);
		s_string_release(v_source);
		return s_return_1299;
	}
	s_record_115 v_module_index = s_hir_module_declarations(s_record_103_get_state_take(s_record_103_copy(v_parsed)), s_record_103_get_node_take(s_record_103_copy(v_parsed)));
	if ((s_string_compare_take(s_record_115_get_error_take(s_record_115_copy(v_module_index)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_record_124 s_return_1300 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_record_115_get_error_take(s_record_115_copy(v_module_index)), s_record_115_get_error_line_take(s_record_115_copy(v_module_index)));
		s_record_115_drop(v_module_index);
		s_record_103_drop(v_parsed);
		s_string_release(v_source);
		return s_return_1300;
	}
	s_record_122 v_state = s_EmitState(s_record_103_get_state_take(s_record_103_copy(v_parsed)));
	s_record_101 v_root = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_record_103_get_node_take(s_record_103_copy(v_parsed)));
	int64_t v_function_index = 0;
	for (int64_t v_i = 0, s_bound_1301 = s_record_101_get_count_take(s_record_101_copy(v_root)); v_i < s_bound_1301; ++v_i)
	{
		const int64_t v_id = s_ast_child(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_record_103_get_node_take(s_record_103_copy(v_parsed)), v_i);
		s_record_101 v_item = s_ast_node(s_record_122_get_tree_take(s_record_122_copy(v_state)), v_id);
		if (((s_record_101_get_kind_take(s_record_101_copy(v_item)) == 14) || (s_record_101_get_kind_take(s_record_101_copy(v_item)) == 42)))
		{
			s_record_122 s_value_1302 = s_collect_function(s_record_122_copy(v_state), s_ga_10114_get_checked_take(s_record_115_get_functions_take(s_record_115_copy(v_module_index)), v_function_index));
			s_record_122_drop(v_state);
			v_state = s_value_1302;
			v_function_index += 1;
		}
		else
		{
			s_record_122 s_value_1303 = s_collect_constant(s_record_122_copy(v_state), v_id);
			s_record_122_drop(v_state);
			v_state = s_value_1303;
		}
		if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) != 0))
		{
			s_record_124 s_return_1304 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_record_122_get_error_take(s_record_122_copy(v_state)), s_record_122_get_error_line_take(s_record_122_copy(v_state)));
			s_record_101_drop(v_item);
			s_record_101_drop(v_root);
			s_record_122_drop(v_state);
			s_record_115_drop(v_module_index);
			s_record_103_drop(v_parsed);
			s_string_release(v_source);
			return s_return_1304;
		}
		s_record_101_drop(v_item);
	}
	S_Array * v_constants = s_ga_8_make_take(0, NULL);
	{
		S_Array * v_s_iter_1305 = s_record_122_get_constants_take(s_record_122_copy(v_state));
		for (int64_t s_index_1305 = 0; s_index_1305 < (int64_t)s_array_length(v_s_iter_1305); ++s_index_1305)
		{
			s_record_120 v_constant = s_ga_10120_get_borrow(v_s_iter_1305, s_index_1305);
			S_String * s_item_1306 = s_record_120_get_name_take(s_record_120_copy(v_constant));
			v_constants = s_ga_8_append_take(v_constants, s_item_1306);
			s_record_120_drop(v_constant);
		}
		s_array_release(v_s_iter_1305);
	}
	{
		S_Array * v_s_iter_1307 = s_record_115_get_functions_take(s_record_115_copy(v_module_index));
		for (int64_t s_index_1307 = 0; s_index_1307 < (int64_t)s_array_length(v_s_iter_1307); ++s_index_1307)
		{
			s_record_114 v_function = s_ga_10114_get_borrow(v_s_iter_1307, s_index_1307);
			if ((!s_record_114_get_constructor_take(s_record_114_copy(v_function))))
			{
				s_record_117 v_body = s_hir_analyze_module_body(s_record_122_get_tree_take(s_record_122_copy(v_state)), s_record_114_copy(v_function), s_record_115_get_functions_take(s_record_115_copy(v_module_index)), s_array_retain(v_constants));
				if ((s_string_compare_take(s_record_117_get_error_take(s_record_117_copy(v_body)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_124 s_return_1308 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_record_117_get_error_take(s_record_117_copy(v_body)), s_record_117_get_error_line_take(s_record_117_copy(v_body)));
					s_record_117_drop(v_body);
					s_record_114_drop(v_function);
					s_array_release(v_s_iter_1307);
					s_array_release(v_constants);
					s_record_101_drop(v_root);
					s_record_122_drop(v_state);
					s_record_115_drop(v_module_index);
					s_record_103_drop(v_parsed);
					s_string_release(v_source);
					return s_return_1308;
				}
				{
					S_Array * v_s_iter_1309 = s_record_117_get_nodes_take(s_record_117_copy(v_body));
					for (int64_t s_index_1309 = 0; s_index_1309 < (int64_t)s_array_length(v_s_iter_1309); ++s_index_1309)
					{
						s_record_108 v_typed = s_ga_10108_get_borrow(v_s_iter_1309, s_index_1309);
						S_Array * s_field_1310 = s_ga_10108_append_take(s_record_122_get_hir_take(s_record_122_copy(v_state)), s_record_108_copy(v_typed));
						s_array_release(v_state.hir);
						v_state.hir = s_field_1310;
						s_record_108_drop(v_typed);
					}
					s_array_release(v_s_iter_1309);
				}
				s_record_117_drop(v_body);
			}
			s_record_114_drop(v_function);
		}
		s_array_release(v_s_iter_1307);
	}
	const int64_t v_main_index = s_find_function(s_record_122_copy(v_state), s_string_from_bytes("main", sizeof("main") - 1));
	if ((((v_main_index < 0) || (s_record_118_get_kind_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_main_index)) != 1)) || (s_generic_array_length_take(s_record_118_get_params_take(s_ga_10118_get_checked_take(s_record_122_get_functions_take(s_record_122_copy(v_state)), v_main_index))) != 0)))
	{
		s_record_124 s_return_1311 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_string_from_bytes("expected def main() -> i64", sizeof("expected def main() -> i64") - 1), 1);
		s_array_release(v_constants);
		s_record_101_drop(v_root);
		s_record_122_drop(v_state);
		s_record_115_drop(v_module_index);
		s_record_103_drop(v_parsed);
		s_string_release(v_source);
		return s_return_1311;
	}
	s_record_122 s_value_1312 = s_line(s_record_122_copy(v_state), s_string_from_bytes("#include <stdint.h>", sizeof("#include <stdint.h>") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1312;
	s_record_122 s_value_1313 = s_line(s_record_122_copy(v_state), s_string_from_bytes("#include <stdbool.h>", sizeof("#include <stdbool.h>") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1313;
	s_record_122 s_value_1314 = s_line(s_record_122_copy(v_state), s_string_from_bytes("#include <stdio.h>", sizeof("#include <stdio.h>") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1314;
	s_record_122 s_value_1315 = s_line(s_record_122_copy(v_state), s_string_from_bytes("#include <string.h>", sizeof("#include <string.h>") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1315;
	s_record_122 s_value_1316 = s_line(s_record_122_copy(v_state), s_string_from_bytes("#include <math.h>", sizeof("#include <math.h>") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1316;
	s_record_122 s_value_1317 = s_line(s_record_122_copy(v_state), s_string_from_bytes("#include \"sirius_runtime.h\"", sizeof("#include \"sirius_runtime.h\"") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1317;
	s_record_122 s_value_1318 = s_line(s_record_122_copy(v_state), s_string_from_bytes("", sizeof("") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1318;
	{
		S_Array * v_s_iter_1319 = s_record_122_get_functions_take(s_record_122_copy(v_state));
		for (int64_t s_index_1319 = 0; s_index_1319 < (int64_t)s_array_length(v_s_iter_1319); ++s_index_1319)
		{
			s_record_118 v_info = s_ga_10118_get_borrow(v_s_iter_1319, s_index_1319);
			if ((!s_is_generic(s_record_118_copy(v_info))))
			{
				S_Array * v_args = s_ga_8_make_take(0, NULL);
				{
					S_Array * v_s_iter_1320 = s_record_118_get_params_take(s_record_118_copy(v_info));
					for (int64_t s_index_1320 = 0; s_index_1320 < (int64_t)s_array_length(v_s_iter_1320); ++s_index_1320)
					{
						s_record_104 v_param = s_ga_10104_get_borrow(v_s_iter_1320, s_index_1320);
						S_String * s_item_1321 = s_c_type(s_record_104_get_kind_take(s_record_104_copy(v_param)));
						v_args = s_ga_8_append_take(v_args, s_item_1321);
						s_record_104_drop(v_param);
					}
					s_array_release(v_s_iter_1320);
				}
				S_String * s_left_1322 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_c_type(s_record_118_get_kind_take(s_record_118_copy(v_info))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_118_get_symbol_take(s_record_118_copy(v_info))), s_string_from_bytes("(", sizeof("(") - 1));
				S_String * s_right_1323 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_args));
				s_record_122 s_value_1324 = s_line(s_record_122_copy(v_state), s_string_concat_take(s_string_concat_take(s_left_1322, s_right_1323), s_string_from_bytes(");", sizeof(");") - 1)));
				s_record_122_drop(v_state);
				v_state = s_value_1324;
				s_array_release(v_args);
			}
			s_record_118_drop(v_info);
		}
		s_array_release(v_s_iter_1319);
	}
	s_record_122 s_value_1325 = s_line(s_record_122_copy(v_state), s_string_from_bytes("", sizeof("") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1325;
	{
		S_Array * v_s_iter_1326 = s_record_122_get_functions_take(s_record_122_copy(v_state));
		for (int64_t s_index_1326 = 0; s_index_1326 < (int64_t)s_array_length(v_s_iter_1326); ++s_index_1326)
		{
			s_record_118 v_info = s_ga_10118_get_borrow(v_s_iter_1326, s_index_1326);
			if ((!s_is_generic(s_record_118_copy(v_info))))
			{
				s_record_122 s_value_1327 = s_emit_function(s_record_122_copy(v_state), s_record_118_copy(v_info));
				s_record_122_drop(v_state);
				v_state = s_value_1327;
				if ((s_string_compare_take(s_record_122_get_error_take(s_record_122_copy(v_state)), s_string_from_bytes("", sizeof("") - 1)) != 0))
				{
					s_record_124 s_return_1328 = s_CompileResult(s_string_from_bytes("", sizeof("") - 1), s_record_122_get_error_take(s_record_122_copy(v_state)), s_record_122_get_error_line_take(s_record_122_copy(v_state)));
					s_record_118_drop(v_info);
					s_array_release(v_s_iter_1326);
					s_array_release(v_constants);
					s_record_101_drop(v_root);
					s_record_122_drop(v_state);
					s_record_115_drop(v_module_index);
					s_record_103_drop(v_parsed);
					s_string_release(v_source);
					return s_return_1328;
				}
				s_record_122 s_value_1329 = s_line(s_record_122_copy(v_state), s_string_from_bytes("", sizeof("") - 1));
				s_record_122_drop(v_state);
				v_state = s_value_1329;
			}
			s_record_118_drop(v_info);
		}
		s_array_release(v_s_iter_1326);
	}
	s_record_122 s_value_1330 = s_line(s_record_122_copy(v_state), s_string_from_bytes("int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_main(); }", sizeof("int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_main(); }") - 1));
	s_record_122_drop(v_state);
	v_state = s_value_1330;
	S_String * v_declarations = s_string_from_bytes("", sizeof("") - 1);
	S_String * v_definitions = s_string_from_bytes("", sizeof("") - 1);
	{
		S_Array * v_s_iter_1331 = s_record_122_get_instances_take(s_record_122_copy(v_state));
		for (int64_t s_index_1331 = 0; s_index_1331 < (int64_t)s_array_length(v_s_iter_1331); ++s_index_1331)
		{
			s_record_119 v_instance = s_ga_10119_get_borrow(v_s_iter_1331, s_index_1331);
			S_Array * v_args = s_ga_8_make_take(0, NULL);
			{
				S_Array_i64 * v_s_iter_1332 = s_record_119_get_kinds_take(s_record_119_copy(v_instance));
				for (int64_t s_index_1332 = 0; s_index_1332 < (int64_t)v_s_iter_1332->length; ++s_index_1332)
				{
					int64_t v_kind = s_array_i64_get_checked(v_s_iter_1332, s_index_1332);
					S_String * s_item_1333 = s_c_type(v_kind);
					v_args = s_ga_8_append_take(v_args, s_item_1333);
				}
				s_array_i64_release(v_s_iter_1332);
			}
			S_String * s_left_1334 = s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_retain(v_declarations), s_c_type(s_record_119_get_kind_take(s_record_119_copy(v_instance)))), s_string_from_bytes(" ", sizeof(" ") - 1)), s_record_119_get_symbol_take(s_record_119_copy(v_instance))), s_string_from_bytes("(", sizeof("(") - 1));
			S_String * s_right_1335 = s_string_join_take(s_string_from_bytes(", ", sizeof(", ") - 1), s_array_retain(v_args));
			S_String *s_value_1336 = s_string_concat_take(s_string_concat_take(s_left_1334, s_right_1335), s_string_from_bytes(");\n", sizeof(");\n") - 1));
			s_string_release(v_declarations);
			v_declarations = s_value_1336;
			S_String *s_value_1337 = s_string_concat_take(s_string_concat_take(s_string_retain(v_definitions), s_record_119_get_code_take(s_record_119_copy(v_instance))), s_string_from_bytes("\n", sizeof("\n") - 1));
			s_string_release(v_definitions);
			v_definitions = s_value_1337;
			s_array_release(v_args);
			s_record_119_drop(v_instance);
		}
		s_array_release(v_s_iter_1331);
	}
	S_String * v_output = s_string_from_bytes("", sizeof("") - 1);
	for (int64_t v_i = 0, s_bound_1338 = s_generic_array_length_take(s_record_122_get_lines_take(s_record_122_copy(v_state))); v_i < s_bound_1338; ++v_i)
	{
		S_String *s_value_1339 = s_string_concat_take(s_string_concat_take(s_string_retain(v_output), s_ga_8_get_checked_take(s_record_122_get_lines_take(s_record_122_copy(v_state)), v_i)), s_string_from_bytes("\n", sizeof("\n") - 1));
		s_string_release(v_output);
		v_output = s_value_1339;
		if ((v_i == 5))
		{
			S_String *s_value_1340 = s_string_concat_take(s_string_retain(v_output), s_record_declarations(s_record_122_copy(v_state)));
			s_string_release(v_output);
			v_output = s_value_1340;
			S_String *s_value_1341 = s_string_concat_take(s_string_retain(v_output), s_optional_declarations(s_record_122_copy(v_state)));
			s_string_release(v_output);
			v_output = s_value_1341;
			S_String *s_value_1342 = s_string_concat_take(s_string_retain(v_output), s_generic_array_declarations(s_record_122_copy(v_state)));
			s_string_release(v_output);
			v_output = s_value_1342;
			S_String *s_value_1343 = s_string_concat_take(s_string_retain(v_output), s_dict_declarations(s_record_122_copy(v_state)));
			s_string_release(v_output);
			v_output = s_value_1343;
			S_String *s_value_1344 = s_string_concat_take(s_string_retain(v_output), s_string_retain(v_declarations));
			s_string_release(v_output);
			v_output = s_value_1344;
		}
	}
	S_String *s_value_1345 = s_string_concat_take(s_string_retain(v_output), s_string_retain(v_definitions));
	s_string_release(v_output);
	v_output = s_value_1345;
	s_record_124 s_return_1346 = s_CompileResult(s_string_retain(v_output), s_string_from_bytes("", sizeof("") - 1), 0);
	s_string_release(v_output);
	s_string_release(v_definitions);
	s_string_release(v_declarations);
	s_array_release(v_constants);
	s_record_101_drop(v_root);
	s_record_122_drop(v_state);
	s_record_115_drop(v_module_index);
	s_record_103_drop(v_parsed);
	s_string_release(v_source);
	return s_return_1346;
	s_string_release(v_output);
	s_string_release(v_definitions);
	s_string_release(v_declarations);
	s_array_release(v_constants);
	s_record_101_drop(v_root);
	s_record_122_drop(v_state);
	s_record_115_drop(v_module_index);
	s_record_103_drop(v_parsed);
}

int64_t s_main()
{
	S_Array * v_args = s_arguments();
	if ((s_generic_array_length_take(s_array_retain(v_args)) != 3))
	{
		s_print_string_take(s_string_from_bytes("usage: emit_c input.sirius output.c", sizeof("usage: emit_c input.sirius output.c") - 1));
		int64_t s_return_1347 = 2;
		s_array_release(v_args);
		return s_return_1347;
	}
	S_Optional_string s_option_1348 = s_read_text_take(s_ga_8_get_checked_take(s_array_retain(v_args), 1));
	S_String * s_fallback_1348;
	if (s_option_1348.has_value) {
		s_fallback_1348 = s_option_1348.value;
	} else {
		s_fallback_1348 = s_string_from_bytes("", sizeof("") - 1);
	}
	S_String * v_source = s_fallback_1348;
	s_record_124 v_result = s_compile_c(s_string_retain(v_source));
	if ((s_string_compare_take(s_record_124_get_error_take(s_record_124_copy(v_result)), s_string_from_bytes("", sizeof("") - 1)) != 0))
	{
		s_print_string_take(s_string_concat_take(s_string_concat_take(s_string_concat_take(s_string_from_bytes("ERROR ", sizeof("ERROR ") - 1), s_string_from_i64(s_record_124_get_line_take(s_record_124_copy(v_result)))), s_string_from_bytes(": ", sizeof(": ") - 1)), s_record_124_get_error_take(s_record_124_copy(v_result))));
		int64_t s_return_1349 = 1;
		s_record_124_drop(v_result);
		s_string_release(v_source);
		s_array_release(v_args);
		return s_return_1349;
	}
	if ((!s_write_text_take(s_ga_8_get_checked_take(s_array_retain(v_args), 2), s_record_124_get_code_take(s_record_124_copy(v_result)))))
	{
		s_print_string_take(s_string_from_bytes("cannot write output", sizeof("cannot write output") - 1));
		int64_t s_return_1350 = 2;
		s_record_124_drop(v_result);
		s_string_release(v_source);
		s_array_release(v_args);
		return s_return_1350;
	}
	int64_t s_return_1351 = 0;
	s_record_124_drop(v_result);
	s_string_release(v_source);
	s_array_release(v_args);
	return s_return_1351;
	s_record_124_drop(v_result);
	s_string_release(v_source);
	s_array_release(v_args);
}

int main(int argc, char **argv) { s_set_arguments(argc, argv); return (int)s_main(); }
