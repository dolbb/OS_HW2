#include <linux/module.h>  
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/sched.h> 
#include <linux/list.h>
#include <asm-i386/errno.h> 
#include <asm-i386/uaccess.h> 

/*===================================================================
*							constants
===================================================================*/
#define INIT_PID 1
#define ROOT_USER_ID 0
#define IS_SHORT_PROCESS 1
#define IS_NOT_SHORT_PROCESS 0

/*===================================================================
*							macros
===================================================================*/

#define GET_PROCESS_AND_RETURN_IF_MISSING 							\
	if(PID < 0){													\
		return -EINVAL;		/*error for invalid pid.*/				\
	}																\
																	\
	if(p == NULL){													\
		return -ESRCH;		/*error for non-existing process.*/		\
	}

/*===================================================================
*							functions
===================================================================*/

asmlinkage int sys_is_short(pid_t PID){
	int ret = IS_NOT_SHORT_PROCESS;
	GET_PROCESS_AND_RETURN_IF_MISSING
	
	if(p->policy == SCHED_SHORT){
		if(!p->iAmOverdue){
			ret = IS_SHORT_PROCESS;
		}
	}
	return ret;
}

asmlinkage int sys_short_remaining_time(pid_t PID){
	int ret = 0;
	GET_PROCESS_AND_RETURN_IF_MISSING

	if(p->policy == SCHED_SHORT){
		ret = p->time_slice;
	}else{
		ret = -EINVAL;
	}
	return ret;
}

asmlinkage int sys_was_short(pid_t PID){
	int ret = 0;
	GET_PROCESS_AND_RETURN_IF_MISSING

	if(p->policy == SCHED_SHORT){
		ret = -EINVAL;
	}else{
		ret = p->iWasShort;
	}
	return ret;
}
