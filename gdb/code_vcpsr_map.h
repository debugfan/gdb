#ifndef CODE_VCPSR_MAP_H
#define CODE_VCPSR_MAP_H

#define MAP_BOOL	int
#define MAP_FALSE	0
#define MAP_TRUE	1

typedef struct _code_range
{
    unsigned char *code_addr;
    int code_len;
} code_range_t;

typedef struct _code_vcpsr_map_node_t
{
	struct _code_vcpsr_map_node_t *next;
	code_range_t first;
	unsigned int second;
} code_vcpsr_map_node_t;

typedef struct
{
   struct _code_vcpsr_map_node_t *next; 
} code_vcpsr_map_t;

void code_vcpsr_map_init(code_vcpsr_map_t *map);
void code_vcpsr_map_erase(code_vcpsr_map_t *map, code_range_t *first);
void code_vcpsr_map_clear(code_vcpsr_map_t *map);
MAP_BOOL code_vcpsr_map_exists(code_vcpsr_map_t *map, code_range_t *first);
MAP_BOOL code_vcpsr_map_set(code_vcpsr_map_t *map, code_range_t *first, unsigned int *second);
MAP_BOOL code_vcpsr_map_get(code_vcpsr_map_t *map, code_range_t *first, unsigned int *second);
void code_vcpsr_map_print(code_vcpsr_map_t *map);
MAP_BOOL code_vcpsr_map_get_vcpsr_by_addr(code_vcpsr_map_t *map, const unsigned char *addr, unsigned int *vcpsr);

#endif
