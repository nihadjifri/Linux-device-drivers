Hi all lets biggin with our very first driver/module. we already know that our operating system is made of n number of modules, just like our Process management module, memory management, file management, I/O managment etc and the Linux device drivers. here we wanna know how they are build internally, and if we talk specifically about the devices we have seen lots of different types of device:

charecter devices	[accessed as a stream of bytes]
block devices		[accessed as multiples of blocks]
Network devices		[make use of data packets]
Pseudo devices		[can be char/block/n/w driver but without 				       any hardware eg: pseudo terminal - pty]

so lets see how to write a simple module first, irrespective of whether its a block, char or any other. here we are using memory as the hardware base of the device, that allows anyone to run the sample code without the need to acquare a special hardware.
we already know that module is a program that is dynamically linked to kernel and running in kernel space. so that we can't use our c funcitons or c libraries,system calls in kernel space and we don't have any thing like our main() function from where our normal c programme starts its excecution.

One intresting fact about the kernel is that it is an object-oriented implementation in C. Any linux driver has a constructor and destructor.The module's constructor is called when the module is successfully loaded into the kernel, and the destroctor is called when we unload the module from kernel.

	#include <linux/init.h>
	#include <linux/module.h>
	#include <linux/kernel.h>
	
	static int my_module_init(void)		/*constructor*/
	{
		printk(KERN_ALERT "My_module: Loading Module\n");
		return 0;
	}
	
	static void my_module_exit(void)		/*destructor*/
	{
		printk(KERN_ALERT "My_module: Unloading Module\n");
	}
	
	module_init(my_module_init);
	module_exit(my_module_exit);
	
	MODULE_LICENSE("Dual BSD/GPL");
	MODULE_AUTHOR("Nihad jifri <nihadjifri@gmail.com>");
	MODULE_DESCRIPTION("Our First Driver");
	

Lets name this driver "My_module",the module defines two functions, one to be invoked when the module is loaded into the kernel (hello_init)and one for when the module is removed (hello_exit). The module_init and module_exit lines use special kernel macros to indicate the role of these two functions. Another special macro (MODULE_LICENSE)is used to tell the kernel that this module bears a free license; without such a declaration, the kernel complains when the module is loaded.

To build a Linux driver, you need to have the kernel source (or, at least, the kernel headers) installed on your system. inorder to setup the kernel source tree checkout the "setup kernel source tree.txt" file in the previous directory.

Now Its the time to compile it and create the module file my_module.ko. We use the kernel build system to do this. The Makefile invokes the kernel’s build system from the kernel source, and the kernel’s Makefile will, in turn, invoke our first driver’s Makefile to build our first driver.

once the C code and the Makefile is ready.all we need to do is to invoke "make" to build our first driver(my_module.ko)

now we will get the .ko file.

update the Makefile when ever you compile each code
