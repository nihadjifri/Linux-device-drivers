#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/uaccess.h>

#define PCD_MAGIC 'X'
#define PCD_GET_CUR_FS_SZ_IOR(PCD_MAGIC,1,int)
#define PCD_GET_CUR_FS_SZ_IOR(PCD_MAGIC,2,int)
//#define PCD_SET_CUR_FS_SZ_IOW(PCD_MAGIC,3,int)
#define NO_CMDS 2
dev_t devno;
int scull_major=0,scull_minor=0;

//device structure
struct scull_dev{
	char kbuf[35];
	int curr;
	int max_size;
	//struct cdev *cdev;	//embed the cdev struct in device struct
	struct cdev cdev;	//embed the cdev struct in device struct
};

struct scull_dev mydev={
	.kbuf="\0",
	.curr=0,
	.max_size=35,
};

MODULE_LICENSE("Dual BSD/GPL");
static int scull_open(struct inode *,struct file *);
static int scull_release(struct inode *,struct file *);
static loff_t scull_seek(struct file *,loff_t,int);
static int scull_ioctl(struct file *,unsigned int,unsigned long);
static ssize_t scull_write(struct file *,const char __user *,size_t,loff_t *);
static ssize_t scull_read(struct file *,char __user *,size_t,loff_t *);

static ssize_t scull_write(struct file *filp,const char __user *ubuf,size_t size,loff_t *off)
{
	struct scull_dev *dev;
	dev=(struct scull_dev *)filp->private_data;
	printk("<1> in scull_write \n");
	printk("curr :: %d\n",dev->curr);
	printk("size :: %d\n",size);
	
	if(copy_from_user((char *)dev->kbuf+dev->curr,(const char *)ubuf,size))
	{
		printk("<1> write error\n");
		return -EFAULT;
	}
	//offset=offset+size;
	dev->curr=dev->curr+size;
	*off+=size;
	printk(KERN_ALERT"*** kbuf: %s\nubuf : %s\noff : %d\ncurr : %d\n",dev->kbuf,ubuf,*off,dev->curr);
	return size;
}

static ssize_t scull_read(struct file *filp,char __user *ubuf,size_t size,loff_t *off)
{
	struct scull_dev *dev;
	dev=(struct scull_dev *)filp->private_data;
	printk("<1> in scull_read\n");
	printk("size :: %d\n",size);
	if(copy_to_user((char *)ubuf,(const char *)dev->kbuf,size))
	{
		printk("<1> write error\n");
		return -EFAULT;
	}

	printk(KERN_ALERT" kbuf : %s\n ubuf: %s\n loff_t : %d\n",dev->kbuf,ubuf,filp->f_pos);
	return size;
}

static int scull_open(struct inode *inode,struct file *filp){
	struct scull_dev *dev;
	dev=container_of(inode->i_cdev,struct scull_dev,cdev);
	dev->curr=0;
	filp->private_data=dev;
		printk(KERN_INFO "open module\n");
	printk(KERN_NOTICE "in int dev size is : %s\n",dev->kbuf);
	printk(KERN_NOTICE "in open dev size is : %d\n",dev->max_size);
	
	if((filp->f_flags & O_ACCMODE) == O_WRONLY){
		printk(KERN_INFO "module opened for write only\n");
	}
	else if((filp->f_flags & O_ACCMODE) == O_RDONLY){
		printk(KERN_INFO "module opened for read only\n");
	}
	else{
		printk(KERN_INFO "mode not supported\n");
		return -1;
	}
	return 0;
}

static int scull_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "t_release module\n");
	return 0;
}

int scull_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct scull_dev *dev;
	dev=(struct scull_dev*)filp->private_data;
	static int err;	

	printk("in IOCTL -- nagma \n");
	if(_IOC_TYPE(cmd)!=PCD_MAGIC)
	{
		printk("work command : cmd \n");
		return -ENODEV;
	}
	if(_IOC_DIR(cmd)&_IOC_READ)
		err=!access_ok(VERIFY_WRITE,(void *)arg,_IOC_SIZE(cmd));
	else if(_IOC_DIR(cmd)&_IOC_WRITE)
		err=!access_ok(VERIFY_READ,(void *)arg,_IOC_SIZE(cmd));
	
	if(err)
		return -EFAULT;

	switch (cmd){
		case PCD_GET_CUR_FS_SZ:
			printk("in ioctl PCD_GET_CUR_FS_SZ\n");
			ret=__put_user(dev->curr,(int __user*)arg);
			printk("In ioctl current dev size = %d\n",dev->curr);
			break;
		case PCD_GET_MAX_FS_SZ:
			ret=__put_user(dev->max_size,(int __user*)arg);
			printk("In ioctl dev max size = %d\n",dev->max_size);
			break;
		/*
			//////////////commented//////////////

		*/
		default :
			printk("no such cmd\n");
			return -ENOTTY;
	}
	return ret;
}

static loff_t scull_seek(struct file *filp,loff_t off,int pos)
{
	loff_t new_pos;
	//struct scull_dev *dev;
	//dev = (struct scull_dev*)filp->private_data;
	printk("In lseek\n");
	switch(pos)
	{
		case 0:
			printk("SEEK SET\n");
			new_pos=off;
			break;
		case 1:
			printk("SEEK CUR\n");
			new_pos=filp->f_pos+off;
			break;
		case 2:
			printk("SEEk END\n");
			new_pos=DEV_SIZE + off;
			break;
		default:
			return -EINVAL; 
	}
	if(new_pos > DEV_SIZE)
        new_pos = DEV_SIZE;
    if(new_pos < 0)
        new_pos = 0;
	filp->f_pos=new_pos;
	printk("new offset is %u\n",filp->f_pos);
	return new_pos;
}

//fs.h

struct file_operations scull_fops={
	.owner=THIS_MODULE,
	.open=scull_open,
	.write=scull_write,
	.read=scull_read,
	.unlocked_ioctl=scull_ioctl,
	.llseek=scull_seek,
	.release=scull_release,
};

static int hello_init(void)
{
	int result;
	int dno=0;
	int err;
		printk(KERN_INFO "inserting module\n");
	if(scull_major) {
	/*if we have major number and minor number and we want to turn that to device number (dev_t)*/
		dno=MKDEV(scull_major,scull_minor);
		/*Dynamic request for Device numbers*/
		result=register_chrdev_region(dno,1,"char_drv1");//obtaining one or more device numbers - setting up a char device 
	} else {
		result=alloc_chrdev_region(&devno,scull_minor,1,"char_drv1");//obtain your major device number using dynamic allocation
		if(result >= 0) {
			scull_major=MAJOR(devno);
			scull_minor=MINOR(devno);
			printk(KERN_WARNING "major: %d\n minor: %d\n",scull_major,scull_minor);
		}	//end of if
	}// end of else

	if (result < 0) {
		printk(KERN_WARNING "scull cant get major \n");
		printk("dev size = %d\n",mydev.max_size);
		return -1;
	}

/*Kernel uses structure of type struct cdev to represent char devices internally,before the kernel invokes your device operations,
you must allocate and register one or more of these structures.*/
	//udelay(10);
	//mydev.cdev=cdev_alloc();	//allocate mem to *cdev

/*	if we want to embed the cdev structure with in the a device-specific structure of owr own.
	 then we need to initialize the structure that we have already allocatted with		*/
	cdev_init(&mydev.cdev,&scull_fops);
	mydev.cdev.owner=THIS_MODULE;	//struct cdev has an owner field that should be set to THIS_MODULE

/*	Once cdev structure is setup, the final step is to tell the kernel about it with a call to cdev_add()*/
	err=cdev_add(&mydev.cdev,devno,1);
	if(err)
	{
		printk(KERN_NOTICE "error %d adding scull",err);
		return -1;
	}
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye\n");
	/*Remove a char device from the system*/
	cdev_del(&mydev.cdev);
	/*When a module is being unloaded from the system, the major number must be released*/
	unregister_chrdev_region(devno,1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
















