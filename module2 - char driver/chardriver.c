#include<linux/init.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/uaccess.h>

dev_t  devno;  //to store the device no//
int scull_major=0;  // to store the major no//
int scull_minor=0;

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

