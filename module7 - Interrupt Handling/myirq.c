#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/ioport.h>
#include<linux/kernel.h>
#include<linux/irq.h>
#include<linux/interrupt.h>
#include<asm/signal.h>
#include<linux/workqueue.h>
#include<linux/sched.h>
#include<asm/irq_vectors.h>

MODULE_LICENSE("DUAL BSD/GPL");

void my_tq_func(unsigned long);
irq_handler_t my_isr(int, void *,struct pt_regs*);

void my_tq_func(unsigned long i){
	printk("in my tq function\n");
}

struct tasklet_struct tq;
unsigned long iii,x;

int result,err;
static int hello_init(void){
	int irqno = 1;
	printk("In init module\n");
	//
	tasklet_init(&tq,my_tq_func,x);
	if(request_irq(irqno,my_isr,IRQF_SHARED,"test_isr",&x))
	{
		printk(KERN_ERR "device cannot register IRQ %d\n",irqno);
		return -EIO;
	}
	return 0;
}

static void hello_exit(void)
{
	printk("cleanup Module\n");
	free_irq(1,&x);
}

int count = 0;

irq_handler_t my_isr(int irq,void *dev_id,struct pt_regs *regs)
{
	printk(KERN_ALERT "WENT TO ISR\n");
	tasklet_schedule(&tq);
	return IRQ_HANDLED;
}

module_init(hello_init);
module_exit(hello_exit);
