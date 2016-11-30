#include <linux/module.h>  
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/sched.h> 
#include <linux/list.h>
#include <asm-i386/errno.h> 
#include <asm-i386/uaccess.h> 

#define INIT_PID 1
#define ROOT_USER_ID 0
#define IS_SHORT_PROCESS 1
#define IS_NOT_SHORT_PROCESS 0

asmlinkage int sys_is_short(pid_t PID){
	int ret = IS_NOT_SHORT_PROCESS;
	if(PID < 0){
		return -EINVAL;	//error for invalid pid.
	}
	
	task_t *p = find_process_by_pid(pid);
	if(p == NULL){
		return -ESRCH;	//error for non-existing process.
	}

	if(p->policy == SCHED_SHORT){
		if(timeLeft > 0){
			ret = IS_SHORT_PROCESS;
		}
	}
	return ret;
}

asmlinkage int sys_short_remaining_time(pid_t PID){
	int ret = 0;
	if(PID < 0){
		return -EINVAL;	//error for invalid pid.??
	}
	
	task_t *p = find_process_by_pid(PID);
	if(p == NULL){
		return -ESRCH;	//error for non-existing process.??
	}

	if(p->policy == SCHED_SHORT){
		if(p->timeLeft > 0){
			ret = p->timeLeft;
		}else{
			ret = -(p->timeleft);
		}
	}
	return ret;
}

asmlinkage int sys_was_short(pid_t PID){
	int ret = 0;
	if(PID < 0){
		return -EINVAL;	//error for invalid pid.??
	}
	
	task_t *p = find_process_by_pid(PID);
	if(p == NULL){
		return -ESRCH;	//error for non-existing process.??
	}

	if(p->policy == SCHED_SHORT){
		return -EINVAL;
	}

	return p->iWasShort;
}
