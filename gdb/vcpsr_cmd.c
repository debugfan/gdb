#include "vcpsr_cmd.h"
#include "code_vcpsr_map.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

code_vcpsr_map_t g_code_vcpsr_map = {NULL};

BOOL get_vcpsr_by_addr(const unsigned char *addr, unsigned int *vcpsr)
{
    return code_vcpsr_map_get_vcpsr_by_addr(&g_code_vcpsr_map, addr, vcpsr);
}

BOOL vcpsr_cmd_get_oper_args(char *cmd_args, unsigned char **code_addr, int *code_len, unsigned int *vcpsr)
{
    char *token = strtok(cmd_args, " ,");
    if(token != NULL)
    {    
        printf("code_addr token: %s\n", token);
        *code_addr = (unsigned char *)strtoul(token, NULL, 0);
    }
    else
    {
        *code_addr = NULL;
    }
    
    if(code_len != NULL)
    {
        if(token != NULL)
        {
            token = strtok(NULL, " ,");
            if(token != NULL)
            {
                printf("code_len token: %s\n", token);
                *code_len = strtol(token, NULL, 0);
            }
            else
            {
                *code_len = 2;
            }
        }
        else
        {
            *code_len = 2;
        }
    }
    
    if(vcpsr != NULL)
    {
        if(token != NULL)
        {
            token = strtok(NULL, " ,");
            if(token != NULL)
            {
                printf( "vcpsr token: %s\n", token);
                *vcpsr = strtoul(token, NULL, 0);
            }
            else
            {
                *vcpsr = 0x20;
            }
        }
        else
        {
            *vcpsr = 0x20;
        }
    }
       
    return TRUE;
}

int get_similar(const char *s1, const char *s2)
{
	int i = 0;
	while(*s1 != '\0' && *s2 != '\0')
	{
		if(*s1 != *s2)
		{
			break;	
		}
		i++;
		s1++;
		s2++;
	}
	return i;
}

int vcpsr_command(const char *cmd_str)
{
    if(strncmp(cmd_str, "v", strlen("v")) == 0)
    {
        code_range_t code_range;
        unsigned int vcpsr;
        char loc_cmd_str[256];
        char *cmd;
		int similar;
        
        memset(loc_cmd_str, 0, sizeof(loc_cmd_str));
        strcpy(loc_cmd_str, cmd_str);
        cmd = &loc_cmd_str[0];

        cmd += get_similar(cmd, "vcpsr");
		
        while(cmd != NULL && (*cmd == ' ' || *cmd == '\t'))
            cmd++;
            
        if(strncmp(cmd, "a", strlen("a")) == 0)
        {
	    	cmd += get_similar(cmd, "add");
            while(cmd != NULL && (*cmd == ' ' || *cmd == '\t'))
                cmd++;
                
            vcpsr_cmd_get_oper_args(cmd, &code_range.code_addr, &code_range.code_len, &vcpsr);
            code_vcpsr_map_set(&g_code_vcpsr_map, &code_range, &vcpsr);
        }
        else if(strncmp(cmd, "d", strlen("d")) == 0)
        {
            cmd += get_similar(cmd, "del");
			
            while(cmd != NULL && (*cmd == ' ' || *cmd == '\t'))
                cmd++;
                
            vcpsr_cmd_get_oper_args(cmd, &code_range.code_addr, &code_range.code_len, NULL);
            code_vcpsr_map_erase(&g_code_vcpsr_map, &code_range);
        }
        
        else if(strncmp(cmd, "l", strlen("l")) == 0)
        {
            cmd += get_similar(cmd, "list");	
            code_vcpsr_map_print(&g_code_vcpsr_map);
        }

		else if(strncmp(cmd, "c", strlen("c")) == 0)
        {
            cmd += get_similar(cmd, "clear");	
            code_vcpsr_map_clear(&g_code_vcpsr_map);
        }
		else
		{
			return 0;
		}
        
        return 1;
    }
	else
	{
		return 0;
	}
}
