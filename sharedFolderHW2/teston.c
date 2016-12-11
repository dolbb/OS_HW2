
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
	sleep(2);//wait 3 seconds
	printf("\x1B[31m");
	int pid = fork();
	if (pid == 0) {
		test_is_short();
		//while(1)
		//{
		// 	printf(".\n");
		// 	sched_yield();
		// }
		printf("bye...\n");
		//exit(0);
	}
	else {
		//pid = fork();
		/*if (pid == 0) {
			//test_basic_short_remaining_time();
			exit(0);
		}
		else {
			//pid = fork();
			if (pid == 0) {
				//test_was_short();
				exit(0);
			}
			else {
				//pid = fork();
				if (pid == 0) {
					//test_getsetparam();
					exit(0);
				}
				else {
					//pid = fork();
					if (pid == 0) {
						//test_forking(); 
						exit(0);
					}
					else {
						//pid = fork(); 
						if (pid == 0) {
							//test_is_illegal();
							exit(0);
						} 
					}
				}
			}
		}*/
		printf("3\n");
		while (waitpid(-1, NULL, 0) != -1);
		printf("\x1B[32mDONE.\n\x1B[0mIf no asserts - you`re GOOD. \n");
		return 0;
	}
}


void test_forking() {
	SET_PARAMS(params, 2, 40);
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&params) == 0);//SUCCESSED SETPARAM
	int is_short_flag = is_short(getpid());
	assert(is_short_flag == 1);
	int time_slice_before_fork = short_remaining_time(getpid());
	int time_slice_div2 = time_slice_before_fork / 2;
	int child = fork();
	if (child == 0)
	{
		int child_time_slice = short_remaining_time(getpid());
		is_short_flag = is_short(getpid());
		assert(is_short_flag == 1);
		int should_be = time_slice_div2 + (time_slice_before_fork % 2);
		assert(child_time_slice >= should_be - 1 && child_time_slice < should_be + 1);
		exit(0);
	}
	int current_time_slice = short_remaining_time(getpid());
	assert(current_time_slice >= time_slice_div2 - 1 && current_time_slice < time_slice_div2 + 1);
	while (waitpid(-1, NULL, 0) != -1);
	while (is_short_flag == 1) {
		for (int i = 0; i < 10000; i++)
		{
		}
		is_short_flag = is_short(getpid());
	}
	assert(is_short_flag == 0);
	time_slice_before_fork = short_remaining_time(getpid());
	child = fork();
	if (child == 0)
	{
		int child_time_slice = short_remaining_time(getpid());
		is_short_flag = is_short(getpid());
		assert(is_short_flag == 0);
		assert(child_time_slice >= time_slice_before_fork - 1 && child_time_slice < time_slice_before_fork + 1);
	}
	current_time_slice = short_remaining_time(getpid());
	assert(current_time_slice >= time_slice_before_fork - 1 && current_time_slice < time_slice_before_fork + 1);
	while (waitpid(-1, NULL, 0) != -1);

}

void test_is_short() {
	/* FAILED 1 */
	assert(is_short(getpid()) == -1);
	assert(errno == EINVAL);
	/* SUCCESS 1 */
	SET_PARAMS(params, 2, 20);
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&params) == 0);//SUCCESSED SETPARAM
    /* end of tests */
	int is_short_flag = is_short(getpid());
	assert(is_short_flag == 1);
	int tmp;

	printf("\n1\n");
	
	while (is_short_flag == 1) {
		for (int i = 0; i < 10000; i++)
		{
			//if(!((tmp = short_remaining_time(getpid())) % 5))
				printf("DEBUG: remainingTime=%d\n", tmp);
			if (i == 8000)
			{
				printf("exiting...\n");
				exit(0);
			}
		}
		is_short_flag = is_short(getpid());
		printf("DEBUG: is_short_flag=%d\n", is_short_flag);
	}
	printf("\n2\n");
	/*
	assert(is_short_flag == 0);
	while (is_short_flag == 0) {
		for (int i = 0; i < 10000; i++)
		{
		}
		is_short_flag = is_short(getpid());
	}
	assert(is_short_flag == -1);
	assert(errno == EINVAL);*/
}

void test_basic_short_remaining_time() {
	int res;
	assert(short_remaining_time(getpid()) == -1);
	assert(errno == EINVAL);

	SET_PARAMS(params, 2, 20);

	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&params) == 0);
	int time;
	time = short_remaining_time(getpid());
	int is_short_flag = is_short(getpid());


	while (is_short_flag == 1) {
		time = short_remaining_time(getpid());
		assert(time > 0 && time < 20);
		for (int i = 0; i < 10000; i++)
		{
		}
		is_short_flag = is_short(getpid());
	}
	assert(is_short_flag == 0);
	time = short_remaining_time(getpid());
	assert(time > 35 && time < 40);
}

void test_was_short() {

	assert(was_short(getpid()) == 0); // the process was NOT short before

	SET_PARAMS(params, 2, 20);
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&params) == 0);
	assert(was_short(getpid()) == -1); // the process is short right now
	assert(errno == EINVAL);
	int is_short_flag = is_short(getpid());
	int time;
	assert(is_short_flag == 1);
	while (is_short_flag == 1) {
		time = short_remaining_time(getpid());
		for (int i = 0; i < 10000; i++)
		{
		}
		is_short_flag = is_short(getpid());
	}
	assert(is_short_flag == 0);
	while (is_short_flag == 0) {
		time = short_remaining_time(getpid());
		for (int i = 0; i < 10000; i++)
		{
		}
		is_short_flag = is_short(getpid());
	}
	assert(is_short_flag == -1);
	assert(errno == EINVAL);

	assert(was_short(getpid()) == 1); // the process was short before

}
void test_is_illegal() {
	/* FAILED 1 */
	SET_PARAMS(illegal, 0, 20);
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&illegal) == 0);// we make PID: current process as SHORT
	assert(sched_setscheduler(getpid(), SCHED_OTHER, (struct sched_param *)&illegal) == -1);// try to change it to OTHER. -1 and errno=EPERM shold be returned.
	assert(errno == EPERM);
	/* FAILED 2 */
	SET_PARAMS(illegal2, 2, 3001);
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&illegal2) == -1);//FAILED PARAM: REQUESTED_TIME > 3000
	assert(errno == EINVAL);
	/* FAILED 3 */
	SET_PARAMS(illegal3, 2, 0);
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&illegal3) == -1);//FAILED PARAM: REQUESTED_TIME < 1
	assert(errno == EINVAL);
}

void test_getsetparam() {
	SET_PARAMS(p2, 2, 1000);
	assert(sched_getparam(getpid(), (struct sched_param *) &p2) == 0);
	assert(p2.sched_priority == 0);
	SET_PARAMS(params, 2, 2500);
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (struct sched_param *)&params) == 0);
	assert(sched_getparam(getpid(), (struct sched_param *)&p2) == 0);
	assert(p2.requested_time == 2500);

	fork();
	while (is_short(getpid()) != 1);
	assert(sched_getparam(getpid(), (struct sched_param *)&p2) == 0);
	assert(p2.requested_time == 2500);
	printf("\nexiting test_getsetparam\n");
}
