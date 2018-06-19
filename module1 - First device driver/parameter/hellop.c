/*****************************************************************************************
	program to Demo how to pass command line input to the driver at load time
	syntax : insmod hellop.ko howmany=5 whom="Nagma"
	Try modinfo hello.ko
	Also check the /sys/module/hellop/parameters.
******************************************************************************************/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");
static char *whom = "world";
static int howmany = 1;
module_param(howmany,int,S_IRUGO);	//read permission for user,group & others
module_param(whom,charp,S_IWUSR);	//write permission for owner

static int hello_init(void)
{
	int i;
	for(i=0;i<howmany;i++)
		printk(KERN_ALERT "(%d)Hello, %s\n",i,whom);
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye ,cruel World\n");
}

module_init(hello_init);
module_exit(hello_exit);
