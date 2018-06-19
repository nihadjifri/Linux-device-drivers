#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/types.h>

#define DEVICENAME	"charecter driver1"
MODULE_LICENSE("DUAL BSD/GPL");		//?? BSD vs GPL
int device_major_no = 0;	/*Will hold the major number extracted by dev_t*/
int device_minor_no = 0;
dev_t dev_num			/*Will hold the major number that the kernel gives*/

static int mydriver_init(void)
{
	int result;
	dev_t dno;
	printk(KERN_ALERT "Init Module\n");	//different printk priorities??
	if(device_major_no)
	{
		dno = MKDEV(device_major_no,device_major_no);	//converting the available major and minor number to device number
		result = register_chrdev_region(dno,1,DEVICENAME);
	}
	else
		result = alloc_chrdev_region(&dev_num,0,1,"charecter driver1"); //function to obtain the device Major number dynamically
	if(result < 0)
	{
		printk(KERN_WARNING "charecter driver1 : Device failed to get the device Major number \n");
		return result;          //return -1;
	}
	else
	{
		printk(KERN_INFO "charecter driver1 : Major number allocation success\n");
		device_major_no = MAJOR(dev_num);
		device_minor_no = MINOR(dev_num);
		printk(KERN_ALERT" Major No : %d Minor No : %d\n",device_major_no,device_minor_no);
		printk(KERN_INFO"To create device file use: mknod /dev/%s c %d %d\n",DEVICENAME,device_major_no,device_minor_no);
	}

	return 0;
}
static void mydriver_exit(void)
{
	unregister_chrdev_region(devno,1);
	printk("Unregistered the device numbers\n");
	printk(KERN_ALERT "Exitting the Module\n");
}
module_init(mydriver_init);
module_exit(mydriver_exit);
