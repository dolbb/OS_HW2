
#include <iostream>
#include <cassert>
//#include <sched.h>
//#include "/usr/src/linux-2.4.18-14custom/include/linux/sched.h"
#include "hw2_syscalls.h"
#include <unistd.h>

#define SCHED_SHORT		5	/* OS course defines */
struct my_sched_param {
	int sched_priority;
	int requested_time;		/* OS course addition */
};

using namespace std;

void testOtherThatWasntShort()
{
	assert(is_short(getpid()) == -1);
	assert(errno == EINVAL);
	
	assert(short_remaining_time(getpid()) == -1);
	assert(errno == EINVAL);
	
	assert(was_short(getpid()) == false);
}

void testOtherThatWasShort()
{
	assert(is_short(getpid()) == -1);
	assert(errno == EINVAL);
	
	assert(short_remaining_time(getpid()) == -1);
	assert(errno == EINVAL);
	
	assert(was_short(getpid()) == true);
}

void testShort()
{
	assert(is_short(getpid()) == 1);
	
	int remainingTime = short_remaining_time(getpid());
	assert(remainingTime > 0);
	
	assert(was_short(getpid()) == -1);
	assert(errno == EINVAL);
}

void testOverdue()
{
	assert(is_short(getpid()) == 0);
	
	int remainingTime = short_remaining_time(getpid());
	assert(remainingTime > 0);
	
	assert(was_short(getpid()) == -1);
	assert(errno == EINVAL);
}

int main()
{
	cout << "Hello world. I am an OTHER process." << endl;

	testOtherThatWasntShort();
	
	// Become SHORT
	cout << "I will now resched as a SHORT process" << endl;
	my_sched_param param;
	param.sched_priority = -1;
	param.requested_time = 200;
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (sched_param*)(&param)) == 0);
	
	testShort();
	
	int is_short_flag = is_short(getpid());
	assert(is_short_flag == 1);
	int remaining_time = short_remaining_time(getpid());
	printf("DEBUG: is_short_flag=%d, remaining_time=%d\n", is_short_flag, remaining_time);
	
	// Become overdue, and print remaining time (when it changes)
	while (is_short_flag == 1) {
		
		// Get current state
		is_short_flag = is_short(getpid());
		int tmp_remaining_time = short_remaining_time(getpid());
		
		// Print status when timeslice changes
		if (remaining_time > tmp_remaining_time){
			remaining_time = tmp_remaining_time;
			printf("DEBUG: is_short_flag=%d, remaining_time=%d\n", is_short_flag, tmp_remaining_time);
		}
	}
	
	cout << "I am an OVERDUE process" << endl;
	testOverdue();
	
	remaining_time = short_remaining_time(getpid());
	while (remaining_time > 10) {
		
		// Get current state
		is_short_flag = is_short(getpid());
		int tmp_remaining_time = short_remaining_time(getpid());
		
		// Print status when timeslice changes
		if (remaining_time != tmp_remaining_time){
			remaining_time = tmp_remaining_time;
			printf("DEBUG: is_short_flag=%d, remaining_time=%d\n", is_short_flag, tmp_remaining_time);
		}
	}
	
	printf("no time!\n");

	while (remaining_time < 11){
		
		// Get current state
		is_short_flag = is_short(getpid());
		int tmp_remaining_time = short_remaining_time(getpid());
		
		// Print status when timeslice changes
		if (remaining_time != tmp_remaining_time){
			remaining_time = tmp_remaining_time;
			printf("DEBUG: is_short_flag=%d, remaining_time=%d\n", is_short_flag, tmp_remaining_time);
		}
		
	}

	
	// Become overdue
	
	//testOtherThatWasShort();
}
