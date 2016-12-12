
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
	
	assert(was_short(getpid() == true));
}

int main()
{
	cout << "Hello world. I am an OTHER process." << endl;

	testOtherThatWasntShort();
	
	// Become SHORT
	cout << "I will now resched as a SHORT process" << endl;
	my_sched_param param;
	param.sched_priority = -1;
	param.requested_time = 100;
	assert(sched_setscheduler(getpid(), SCHED_SHORT, (sched_param*)(&param)) == 0);
	
	testShort();
	int remaining_time = short_remaining_time(getpid());
	
	// Become overdue
	while (is_short(getpid()) == 1) {
		
		// Get current state
		int tmp_remaining_time = short_remaining_time(getpid());
		
		// Print status when timeslice changes
		if (remaining_time != tmp_remaining_time){
			remaining_time = tmp_remaining_time;
			printf("DEBUG: remaining_time=%d\n", remaining_time);
		}	
	}
	
	cout << "I am an OVERDUE process" << endl;
	testOverdue();
	
	// Become 
	while (is_short(getpid()) == 0) {
		
		// Get current state
		int tmp_remaining_time = short_remaining_time(getpid());
		
		// Print status when timeslice changes
		if (remaining_time != tmp_remaining_time){
			remaining_time = tmp_remaining_time;
			printf("DEBUG: remaining_time=%d\n", remaining_time);
		}
	}
	
	for(int i=0 ; i<100 ; ++i){
		sched_yield();
	}
	
	testOtherThatWasShort();
}
