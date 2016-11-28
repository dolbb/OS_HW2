#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

/*	
	The wrapper will return 1 if the given process is a SHORT process, 
	or 0 if it is already overdue.
*/
int is_short(pid_t PID){
	unsigned int res = 0;
	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (243) ,"b" (PID)
		: "memory"
	);
	if (res >= (unsigned long) (-125)){
		errno = -res;
		res = (unsigned int)(-1);
	}
	return (int) res;
}

/*
	For a regular SHORT process, the wrapper will return the time (in ms) 
	left before it becomes overdue. 
	For an overdue process, it should return the time left before the 
	process policy will be changed to SCHED_OTHER.
*/
int short_remaining_time(pid_t PID){
	unsigned int res = 0;
	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (244) ,"b" (PID)
		: "memory"
	);
	if (res >= (unsigned long) (-125)){
		errno = -res;
		res = (unsigned int)(-1);
	}
	return (int) res;
}

/*
	If the process policy is SHORT, EINVAL error code should be returned. 
	Otherwise the wrapper should return 1 if the process was a SHORT process 
	before and 0 if not.
*/
int was_short(pid_t PID){
	unsigned int res = 0;
	__asm__(
		"int $0x80;"
		: "=a" (res)
		: "0" (245) ,"b" (PID)
		: "memory"
	);
	if (res >= (unsigned long) (-125)){
		errno = -res;
		res = (unsigned int)(-1);
	}
	return (int) res;
}

