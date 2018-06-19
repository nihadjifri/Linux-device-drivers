#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/workqueue.h>

/*
//#include<linux/init.h>
#include<linux/timer.h>
#include<linux/interrupt.h>
//#include<linux/kernel.h>
//#include<linux/module.h>
#include<linux/sched.h>
#include<linux/irq.h>
*/


static void mykmod_work_handler(struct work_struct *w);
static struct workqueue_struct *wq=0;	//work queue

//for submitting the work to the work queue
static int DECLARE_DELAYED_WORK(my_work,mykmod_work_work_handler);
static unsigned long mydelay;

static void mykmod_work_handler(struct work_struct *w){
	printk("Workqueue handler executed after %u seconds\n",(unsigned)mydelay);
}

static int mykmod_init(void){
	mydelay = jiffies + HZ*10;
	printk("In init module\n");
	if(!wq)
		wq = create_singlethread_workqueue("mykmod");
	if(wq)
		queue_delayed_work(wq,&my_work,mydelay);//submitting the work to the workQ
	return 0;
}

static void mykmod_exit(void)
{
	if(wq)
		destroy_workqueue(wq);
	printk("mykmod exit\n");
}
module_init(mykmod_init);
module_exit(mykmod_exit);
MODULE_DESCRIPTION("mykmod");
MODULE_LICENSE("GPL");
