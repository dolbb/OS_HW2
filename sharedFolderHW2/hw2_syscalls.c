#include <linux/module.h>  
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/sched.h> 
#include <linux/list.h>
#include <asm-i386/errno.h> 
#include <asm-i386/uaccess.h> 

#define INIT_PID 1
#define ROOT_USER_ID 0

asmlinkage int sys_is_short(pid_t PID){
	printk("func name is sys_is_short\n");
	return PID;
}

asmlinkage int sys_short_remaining_time(pid_t PID){
	printk("func name is sys_short_remaining_time\n");
	return PID;
}

asmlinkage int sys_was_short(pid_t PID){
	printk("func name is sys_was_short\n");
	return PID;
}
