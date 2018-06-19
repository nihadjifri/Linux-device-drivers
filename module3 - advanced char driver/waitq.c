/*
 * Simple charecter driver with wait queue
 * cat /dev/scull goes to block, on a condition 
 * wakes up only when the condition is met, dmesg to check
 * execute echo "hello" > /dev/scull the writer process
 * wakes up the reader check the dmesg
 */

#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/wait.h>
#include<linux/sched.h>

struct scull_dev 
{
	unsigned int roffset,woffset;
	unsigned int curr;
	char data[10];
	unsigned int max;
	wait_queue_head_t wq;
	struct cdev cdev;
};
struct scull_dev dev;
int flag = 0;

static int scull_open (struct inode *, struct file *);
static int scull_release (struct inode *, struct file *);
ssize_t scull_read (struct file *filp, char __user *buf, size_t count, loff_t *pos);
ssize_t scull_write (struct file *filp, const char __user *buf, size_t count, loff_t *pos);

dev_t devno;

struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.read = scull_read,
	.write = scull_write,
	.open = scull_open,
	.release = scull_release,
};

static int scull_open (struct inode *inode, struct file *filp)
{
	struct scull_dev *dev1 = container_of (inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev1;
	printk(KERN_INFO"open module\n");
	return 0;
}

static int scull_release (struct inode *inode, struct file *filp)
{
	printk(KERN_INFO"release module\n");
	return 0;
}

ssize_t scull_read (struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	/*Now the flag is 0, the condition is false so goes to wait queue*/
	printk(KERN_DEBUG"process %i (%s) going to sleep\n",current->pid, current->comm);
	wait_event_interruptible (dev.wq, flag != 0);
	flag = 0;
	printk(KERN_DEBUG "Awoken %i (%s)\n",current->pid, current->comm);
	return 0;	/*EOF*/
}

ssize_t scull_write (struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
	//struct scull_dev *dev1 = filp->private_data;
	printk(KERN_ALERT "max size is %d\n",dev.max);
	printk(KERN_DEBUG "process %i (%s) awakening the readers ..\n",current->pid, current->comm);
	flag = 1;
	wake_up_interruptible (&dev.wq);
	return count;	/* succeed ,to avoid retrial*/
}


static int scull_init (void)
{
	static int scull_major = 0,scull_minor = 0;
	int result;
	int dno = 0;
	int err;
	dev.roffset = 0;
	dev.woffset = 0;
	dev.curr = 0;
	dev.max = 10;
	
	printk(KERN_INFO "inserting module\n");
	
	if (scull_major) {
		result = register_chrdev_region(dno,1,"scull");
	} else {
		result = alloc_chrdev_region(&devno,scull_minor,1,"scull");
		scull_major = MAJOR(devno);
		printk(KERN_WARNING "major %d minor %d \n",scull_major,scull_minor);
	}

	if (result < 0) {
		printk(KERN_WARNING "scull cant get major %d\n",scull_major);
	}

	//dev.cdev = cdev_alloc();
	//dev.cdev->ops = &scull_fops;

	init_waitqueue_head(&dev.wq);	////////////<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	cdev_init(&dev.cdev,&scull_fops);
	dev.cdev.owner = THIS_MODULE;
	err = cdev_add(&dev.cdev,devno,1);
	
	if (err) {
		printk(KERN_NOTICE "error %d adding scull",err);
		cdev_del(&dev.cdev);
		unregister_chrdev_region(devno,1);
	}
	return 0;
}

static void scull_exit(void)
{
	printk(KERN_ALERT "Goodbye\n");
	cdev_del(&dev.cdev);
	unregister_chrdev_region(devno,1);
}

MODULE_LICENSE("DUAL BSD/GPL");
MODULE_AUTHOR("Cranes software");
module_init(scull_init);
module_exit(scull_exit);

