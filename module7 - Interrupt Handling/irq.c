//we are writing a driver to handle the keyboard intrrupts

#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/cdev.h> 
#include <linux/slab.h> 
#include <linux/kernel.h> 
#include <linux/fs.h> 
#include <linux/types.h> 
#include <linux/ioport.h> 
#include <linux/kernel.h> 
#include <linux/irq.h> 
#include <linux/interrupt.h> 
#include <asm/signal.h> 
#include <linux/workqueue.h> 
#include <linux/sched.h> 
#include <asm/irq_vectors.h> 

MODULE_LICENSE("DUAL BSD/GPL"); 

irq_handler_t  my_isr(int , void *,struct pt_regs*); 
unsigned long iii,x=10;
int irqno=12;  // 1 = PS2 Keyboard :: 12 = PS2 mouse  
int result,err; 

static int hello_init(void)
{ 
	printk("In %s\n",__FUNCTION__); 
	if(request_irq(irqno,my_isr,IRQF_SHARED,"test_isr",&x)) 
	{ 
		printk("KERN_ERR device cannot register IRQ %d \n",irqno); 
		return -EIO; 
	} 
	return 0; 
} 


static void hello_exit(void) 
{ 
	printk("cleanup module\n"); 
	free_irq(irqno,&x); 
} 

int count=0; 

irq_handler_t my_isr(int irq, void *dev_id,struct pt_regs *regs) 
{
	static int count;   
	printk(KERN_ALERT "WENT TO ISR %s, count=%d\n",__FUNCTION__,count++); 
	return IRQ_HANDLED; 
} 

module_init(hello_init); 
module_exit(hello_exit); 
