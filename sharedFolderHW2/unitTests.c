#include <stdio.h>
#include "hw2_syscalls.h"


int main(){
	printf("starting indiana:\n\n");
	int returnMe = 1948;
	int ret1 = is_short(returnMe);
	int ret2 = short_remaining_time(returnMe);
	int ret3 = was_short(returnMe);

	printf("\nfunc1 returns: %d \n",ret1);
	printf("func2 returns: %d \n",ret2);
	printf("func3 returns: %d \n",ret3);

	printf("therefore we should return 1948\nending indiana:\n");
	return 0;
}
