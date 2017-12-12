#include "code_vcpsr_map.h"
#include <stdio.h>
#include <stdlib.h>

int code_range_compare(const code_range_t *a, const code_range_t *b)
{
    if(a->code_addr > b->code_addr)
    {
        return 1;
    }
    else if(a->code_addr == b->code_addr)
    {
        if(a->code_len > b->code_len)
        {
            return 1;
        }
        else if(a->code_len == b->code_len)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

void code_vcpsr_map_init(code_vcpsr_map_t *map)
{
	map->next = NULL;
}

void code_vcpsr_map_clear(code_vcpsr_map_t *map)
{
	code_vcpsr_map_node_t *map_node = NULL;
	code_vcpsr_map_node_t *temp = NULL;

    map_node = map->next;
    while(map_node)
    {
        temp = map_node;
        map->next = map_node->next;
        free(temp);

        map_node = map->next;
    }
}

void code_vcpsr_map_print(code_vcpsr_map_t *map)
{
	code_vcpsr_map_node_t *map_node = NULL;

    map_node = map->next;
    while(map_node)
    {
        printf("code_addr: 0x%x, code_len: 0x%x, vcpsr: 0x%x\n", (unsigned int)map_node->first.code_addr, map_node->first.code_len, map_node->second);

        map_node = map_node->next;
    }
}

code_vcpsr_map_node_t *code_vcpsr_map_node_find(code_vcpsr_map_t *map, code_range_t *first)
{
    code_vcpsr_map_node_t *map_node = map->next;   
	while(map_node != NULL)
	{
		if(code_range_compare(&map_node->first, first) == 0)
		{
			break;
		}
		map_node = map_node->next;
	}
	return map_node;
}

MAP_BOOL code_vcpsr_map_exists(code_vcpsr_map_t *map, code_range_t *first)
{
	if(NULL != code_vcpsr_map_node_find(map, first))
	{
		return MAP_TRUE;
	}
	else
	{
		return MAP_FALSE;
	}
}

code_vcpsr_map_node_t *code_vcpsr_map_node_find_prev(code_vcpsr_map_t *map, code_range_t *first)
{
    code_vcpsr_map_node_t *map_node = map->next;
	if(map_node)
	{
		if(code_range_compare(&map_node->first, first) == 0)
		{
			return (code_vcpsr_map_node_t *)map;
		}
		
		while(map_node)
		{
			if(map_node->next)
			{
				if(code_range_compare(&map_node->next->first, first) == 0)
				{
					break;
				}
				map_node = map_node->next;
			}
			else
			{
				map_node = NULL;
				break;
			}
		}
	}
	return map_node;
}

MAP_BOOL code_vcpsr_map_set(code_vcpsr_map_t *map, code_range_t *first, unsigned int *second)
{
	MAP_BOOL res = MAP_FALSE;
	code_vcpsr_map_node_t *map_node = NULL;
	map_node = code_vcpsr_map_node_find(map, first);
	if(map_node)
	{
		map_node->second = *second;
		res = MAP_TRUE;
	}
	else
	{
		map_node = (code_vcpsr_map_node_t *)malloc(sizeof(code_vcpsr_map_node_t));
		if(map_node)
		{
			map_node->next = map->next;
			map_node->first = *first;
			map_node->second = *second;

			map->next = map_node;
			res = MAP_TRUE;
		}
	}
	return res;
}

MAP_BOOL code_vcpsr_map_get(code_vcpsr_map_t *map, code_range_t *first, unsigned int *second)
{
	MAP_BOOL res = MAP_FALSE;
	code_vcpsr_map_node_t *map_node = NULL;
	map_node = code_vcpsr_map_node_find(map, first);
	if(map_node != NULL)
	{
		*second = map_node->second;
		res = MAP_TRUE;
	}
	else
	{
		res = MAP_FALSE;
	}
	return res;
}

void code_vcpsr_map_erase(code_vcpsr_map_t *map, code_range_t *first)
{
	MAP_BOOL res = MAP_FALSE;
	code_vcpsr_map_node_t *map_node = NULL;
	code_vcpsr_map_node_t *temp = NULL;
	map_node = code_vcpsr_map_node_find_prev(map, first);
	while(map_node != NULL && map_node->next != NULL)
	{
		temp = map_node->next;
		map_node->next = map_node->next->next;
		free(temp);

		map_node = code_vcpsr_map_node_find_prev(map, first);
	}
}

MAP_BOOL addr_is_in_code_range(const unsigned char *addr, code_range_t *code_range)
{
    if(addr >= code_range->code_addr
        && addr < code_range->code_addr+code_range->code_len)
    {
        return MAP_TRUE;
    }
    else
    {
        return MAP_FALSE;
    }
}

MAP_BOOL code_vcpsr_map_get_vcpsr_by_addr(code_vcpsr_map_t *map, const unsigned char *addr, unsigned int *vcpsr)
{
	MAP_BOOL res = MAP_FALSE;
	code_vcpsr_map_node_t *map_node = NULL;

    map_node = map->next;
    while(map_node)
    {
        if(addr_is_in_code_range(addr, &map_node->first))
        {
            *vcpsr = map_node->second;
            res = MAP_TRUE;
            break;
        }

        map_node = map_node->next;
    }
	return res;
}
