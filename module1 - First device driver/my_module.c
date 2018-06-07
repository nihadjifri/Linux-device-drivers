#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int my_module_init(void)		/*constructor*/
{
	printk(KERN_ALERT "My_module: Loading Module\n");
	return 0;
}

static void my_module_exit(void)	/*destructor*/
{
	printk(KERN_ALERT "My_module: Unloading Module\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Nihad jifri <nihadjifri@gmail.com>");
MODULE_DESCRIPTION("Our First Driver");
