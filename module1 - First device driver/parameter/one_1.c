#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
MODULE_LICENSE("Dual BSD/GPL");
int add(int,int);

static int hello_init(void)
{
	return 0;
}

static void hello_exit(void)
{

}

int add(int a,int b)
{
	int sum;
	sum=a + b;
	return sum;
}

MODULE_AUTHOR("Nagma");
EXPORT_SYMBOL(add);

module_init(hello_init);
module_exit(hello_exit);
