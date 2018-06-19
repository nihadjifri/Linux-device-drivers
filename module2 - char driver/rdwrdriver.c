#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#define DEV_SIZE 50
dev_t  devno;  //to store the device no//
int scull_major=0;  // to store the major no//
int scull_minor=0;
unsigned int l;
char kbuf[DEV_SIZE];
struct scull_dev  //dev structure//
{
    char kbuf[35];
    int curr;
    int max_size;
    struct cdev cdev;
};
struct scull_dev mydev=
{
	.kbuf="\0",
	.curr=0,
	.max_size=35,
};
MODULE_LICENSE("DUAL BSD/GPL");
static int scull_open(struct inode *,struct file *);
static int scull_release(struct inode*,struct file *);
ssize_t scull_read (struct file *, char __user *, size_t, loff_t *);
ssize_t scull_write (struct file *, const char __user *, size_t, loff_t *);
loff_t scull_lseek (struct file *, loff_t, int);
ssize_t scull_read (struct file *f, char __user *ubuf, size_t s, loff_t *of)
{
      printk(KERN_INFO "reading module\n");
	copy_to_user(ubuf,kbuf,s);
	printk(KERN_ALERT"kbuf : %s\t ubuf : %s\n",kbuf,ubuf);
	l=f->f_pos;
	printk(KERN_ALERT"Cursor position : %u\n",l);
      return s;
}
ssize_t scull_write (struct file *f, const char __user *ubuf, size_t s, loff_t *of)
{
	printk(KERN_ALERT"writing to module\n");
	copy_from_user(kbuf,ubuf,s);
	printk(KERN_ALERT"kbuf : %s\t ubuf : %s\n",kbuf,ubuf);
	l=f->f_pos;
	printk(KERN_ALERT"cursor position : %u \n",l);
	return s;
}
loff_t scull_lseek (struct file *filp, loff_t off, int whence)
{
	loff_t newpos;
	printk(KERN_ALERT"in lseek\n");
//	struct scull_dev *dev;
//	dev = (struct scull_dev *)filp->private_data;
	switch (whence)
	{
		case 0: /*SEEK_SET*/
			newpos = off;
			break;
		case 1: /*SEEK_CUR*/
			newpos = filp->f_pos + off;
			break;
		case 2: /*SEEK_END*/
			newpos = DEV_SIZE + off;
			break;
		default: /*can't happen*/
			return -EINVAL;

	}

	if(newpos<0) return -EINVAL;
	filp->f_pos = newpos;
	return newpos;
}
static int scull_open(struct inode *inode,struct file *filp)
{
      printk(KERN_INFO "open module\n");
      return 0;
}
static int scull_release(struct inode *inode,struct file *filp)
{
      printk(KERN_INFO "t_release module\n");
      return 0;
}
struct file_operations scull_fops=
{
	.owner=THIS_MODULE,
	.open=scull_open,
	.release=scull_release,
	.read=scull_read,
	.write=scull_write,
	.llseek=scull_lseek,
};
static int hello_init(void)
{
    int result;
    int dno=0;
    int err;
    printk(KERN_INFO "inserting module\n");
	if(scull_major)
	{
	       dno=MKDEV(scull_major,scull_minor);
               result=register_chrdev_region(dno,1,"char_drv1");
        }
       else
        {
              result=alloc_chrdev_region(&devno, scull_minor,1,"char_drv1");
              if(result>=0)
              {  
                     scull_major=MAJOR(devno);
		     scull_minor=MINOR(devno);
                     printk(KERN_WARNING "major %d minor %d \n",scull_major,scull_minor);
              }
        } 
     if(result<0)
    {
         printk(KERN_WARNING "scull can't get major\n");
         printk("dev size =%d\n",mydev.max_size);
         return -1;
    }
  cdev_init(&mydev.cdev,&scull_fops);
  mydev.cdev.owner=THIS_MODULE;
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
    cdev_del(&mydev.cdev);
    unregister_chrdev_region(devno,1);
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

