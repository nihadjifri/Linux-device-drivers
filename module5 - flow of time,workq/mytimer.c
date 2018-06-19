/*
	whenever we need to schedule an action to happen later
	without blocking the current process until that time arrives
	kernel timer are used
*/

#include<linux/init.h>
#include<linux/timer.h>
#include<linux/interrupt.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/sched.h>
#include<linux/irq.h>
#define SEC 15
MODULE_LICENSE("DUAL BSD/GPL");
MODULE_AUTHOR("Cranes software");

static struct timer_list timer;
static char module[] = "my timer";
static unsigned up_sec=1000;

static void my_timer(unsigned long data){
	printk(" In my timer function %lu \n",data);
}

int init_module(void){
	printk("<1> Installing %s module \n",module);
	init_timer(&timer); 	//structure must be initialized before use takes care of the opque members also. ////////
	
	/* The below three members must be explicitly initialized by the driver other */         /////////////
	timer.expires=jiffies + HZ*SEC;
	timer.data=(unsigned)&up_sec;
	timer.function=my_timer;
	add_timer(&timer);	//	To add the timer to the system
	return 0;
}

void cleanup_module(void){
	del_timer_sync(&timer);	//Remove the timer from the system
	printk("<1> Removing module\n");
}
//module_init(init_module);
//module_exit(cleanup_module);
