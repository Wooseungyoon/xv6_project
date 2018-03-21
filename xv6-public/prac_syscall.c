#include "types.h"
#include "defs.h"


//Wrapper for my_syscall
int 
sys_myfunction(void) {
	char * str;
	if (argstr(0, &str) < 0)
		return -1;
	return printk_str(str);
}

int
printk_str(char * str)
{
	cprintf("%s\n", str);
	return 0xABCDABCD;
}



