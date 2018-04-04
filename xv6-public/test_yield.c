#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char * argv[]) 
{
	if(fork() == 0) {
		while(1) {
			printf(1, "Child\n");
			sleep(1);
			yield();
		}
	} else {
		while(1) {
			printf(1, "Parent\n");
			sleep(1);
			yield();	
		}
	}
}
