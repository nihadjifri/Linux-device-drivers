//filename : one.c

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
MODULE_LICENSE("Dual BSD/GPL");

int add(int,int);

int a=10;
int b=20;
EXPORT_SYMBOL(a);
EXPORT_SYMBOL(b);
static int hello_init(void)
{
	printk(KERN_ALERT "Hello World\n");
	printk("Add : %d\n",add(a,b));
	return 0;
}

static void hello_exit(void)
{
	printk("Goodbye ,cruel World\n");
}

MODULE_AUTHOR("Nagma");

//EXPORT_SYMBOL(add);

module_init(hello_init);
module_exit(hello_exit);
