#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/ioport.h>
#include<linux/kernel.h>
#define baseaddr 0xffb80000

MODULE_LICENSE("DUAL BSD/GPL");

int result,err;
static int hello_init(void){
	int dno,scull_minor=0,scull_major=0;
	dev_t devno;
	printk(KERN_INFO "inserting module\n");
	if(scull_major){
		dno=MKDEV(scull_major,scull_minor);
		result=register_chrdev_region(dno,1,"scull");
	} else {
		result= alloc_chrdev_region(&devno,scull_minor,1,"scull");
		scull_major=MAJOR(devno);
		printk(KERN_WARNING "major %d minor %d\n",scull_major,scull_minor);
	}
	if (result<0){
		printk(KERN_WARNING"scull cant get major %d\n",scull_major);
	}

/*	if((err=check_mem_region(baseaddr,100)) < 0){
		return err;
	}
*/
	request_mem_region(baseaddr,100,"my_uart");
	return 0;
}


static void hello_exit(void)
{
	printk(KERN_ALERT "cleanup module\n");
	release_mem_region(baseaddr,100);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");



