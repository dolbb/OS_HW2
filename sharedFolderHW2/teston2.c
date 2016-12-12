#include <time.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>
#include <sys/resource.h>
#include "hw2_syscalls.h"

#define SCHED_OTHER		0
#define SCHED_FIFO		1
#define SCHED_RR		2
//HW 2

#define SCHED_SHORT 5
int nice(int);//WTF?

struct sched_param_ex {
	int sched_priority;
	int requested_time;
};

#define SET_PARAMS(name, priority, time)\
	struct sched_param_ex name;\
	name.sched_priority = priority;\
	name.requested_time = time;\

void test_is_short();
void test_basic_short_remaining_time();
void test_was_short();
void test_getsetparam();
void test_forking();

void test_is_illegal();

int main() {
	SET_PARAMS(p2, 2, 1000);
	assert(sched_getparam(getpid(), (struct sched_param *) &p2) == 0);
	assert(p2.sched_priority == 0);
	SET_PARAMS(params, 2, 2500);
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&params) == 0);
	assert(sched_getparam(getpid(), (struct sched_param *)&p2) == 0);
	assert(p2.requested_time == 2500);

	int pid = fork();
	if(pid == 0){
		printf("im son\n");
	}else{
		printf("im loser\n");
	}
	printf("done\n");
	return 0;
}



