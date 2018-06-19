/* Necessary includes for drivers */

#include <linux/init.h>

//#include <linux/config.h>

#include <linux/module.h>

#include <linux/kernel.h> /* printk() */

#include <linux/slab.h> /* kmalloc() */

#include <linux/fs.h> /* everything... */

#include <linux/errno.h> /* error codes */

#include <linux/types.h> /* size_t */

#include <linux/proc_fs.h>

#include <linux/fcntl.h> /* O_ACCMODE */

#include <linux/ioport.h>

//#include <asm/system.h> /* cli(), *_flags */

#include <asm/uaccess.h> /* copy_from/to_user */

#include <asm/io.h> /* inb, outb */



MODULE_LICENSE("Dual BSD/GPL");



/* Function declaration of parlelport.c */ 

int parlelport_open(struct inode *inode, struct file *filp); 

int parlelport_release(struct inode *inode, struct file *filp); 

ssize_t parlelport_read(struct file *filp, char *buf, size_t count, loff_t *f_pos); 

ssize_t port_write(struct file *filp,const char *buf, size_t count, loff_t *f_pos);  
void parlelport_exit(void);

int parlelport_init(void);



/* Structure that declares the common */

/* file access fcuntions */

struct file_operations parlelport_fops = { 
  .read=parlelport_read,
  .write=port_write,
  .open=parlelport_open,
  .release=parlelport_release
};



/* Driver global variables */

/* Major number */

int parlelport_major = 61;



/* Control variable for memory */ 

/* reservation of the parallel port*/

int port;

///////////////////Init Module////////////////////
int parlelport_init(void) { 
  void *ptr;
  int result;

  /* Registering device */

  result = register_chrdev(parlelport_major, "parlelport", 

      &parlelport_fops);

  if (result < 0) { 

    printk("<1>parlelport: cannot obtain major number %d\n",

      parlelport_major); 

    return result; 

  } 

//  <parlelport modified init module>
/* Registering port */
/*
  port = check_region(0x378, 1);

  if (port) {

    printk("<1>parlelport: cannot reserve 0x378\n");

    result = port;

    goto fail;

  }
*/
  ptr = request_region(0x378, 1, "parlelport");
	if(ptr==NULL)
		printk("<1>parlelport: 0x378 reserved\n");
///////////////////////////

  printk("<1>Inserting parlelport module\n"); 

  return 0;


/*
  fail: 

    parlelport_exit(); 

    return result;
*/
}
////////////////////Opening the device as a file///////////////
int parlelport_open(struct inode *inode, struct file *filp) {
  /* Success */
printk(KERN_ALERT "open module\n");
  return 0;
}
////////////////////Reading the device////////////////////////
ssize_t parlelport_read(struct file *filp, char *buf,size_t count, loff_t *f_pos) {

 

  /* Buffer to read the device */

  char parlelport_buffer;
//  <parlelport inport>
	
	/* Reading port */

	parlelport_buffer = inb(0x378);

printk(KERN_ALERT "read module\n");

  /* We transfer data to user space */

  copy_to_user(buf,&parlelport_buffer,1); 
  /* We change the reading position as best suits */

  if (*f_pos == 0) { 

    *f_pos+=1; 

    return 1; 

  } else { 

    return 0; 

  }

}
//////////////////////Writing to the device//////////////
ssize_t port_write( struct file *filp,const char *buf, size_t count, loff_t *f_pos) 
{
  const char *tmp;
  /* Buffer writing to the device */

  char parlelport_buffer;
printk(KERN_INFO "write module\n");


  tmp=buf+count-1;

  copy_from_user(&parlelport_buffer,tmp,1);



  //<parlelport outport>

  /* Writing to the port */

	outb(parlelport_buffer,0x378);

  return 1; 

}
////////////////////closing the device as a file///////////////
int parlelport_release(struct inode *inode, struct file *filp) {
  /* Success */
printk(KERN_ALERT "release module\n");

  return 0; 
}
////////////////////Removing the module/////////////////////////
void parlelport_exit(void) {

  /* Make major number free! */
  unregister_chrdev(parlelport_major, "parlelport");
 // <parlelport modified exit module>
/* Make port free! */

  if (!port) {

    release_region(0x378,1);

  }
  printk("<1>Removing parlelport module\n");

}


///////////////////////////////////////////////////////////////

module_init(parlelport_init);

module_exit(parlelport_exit);
