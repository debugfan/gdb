#ifndef VCPSR_CMD_H
#define VCPSR_CMD_H

#ifndef BOOL
#define BOOL int
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

int vcpsr_command(const char *cmd_str);
BOOL get_vcpsr_by_addr(const unsigned char *addr, unsigned int *vcpsr);

#endif
