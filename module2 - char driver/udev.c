#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int udev_init(void)
{

	return 0;
}
static void udev_exit(void)
{

}

MODULE_AUTHOR("Cranes Varsity");
MODULE_LICENSE("DUAL BSD/GPL");
module_init(udev_init);
module_exit(udev_exit);
