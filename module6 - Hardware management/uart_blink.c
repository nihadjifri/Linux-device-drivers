#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <asm/io.h>
 
#include <linux/serial_reg.h>
 
#define SERIAL_PORT_BASE 0x3F8
/*
static void blink(void)
{
 int i;
    u8 data;

    data = inb(SERIAL_PORT_BASE + UART_LCR);
    for (i = 0; i < 5; i++)
    {*/
        /* Pulling the Tx line low */
       /* data |= UART_LCR_SBC;
        outb(data, SERIAL_PORT_BASE + UART_LCR);
        printk(KERN_ALERT "LED ON\n");
        msleep(2000);
       */ /* Defaulting the Tx line high */
        /*data &= ~UART_LCR_SBC;
        outb(data, SERIAL_PORT_BASE + UART_LCR);
        printk(KERN_ALERT "LED OFF\n");
        msleep(2000);
}
*/
 
int __init init_module()
{
    int i;
    u8 data;
// 	blink();
	printk(KERN_ALERT "init module\n");
    data = inb(SERIAL_PORT_BASE + UART_LCR);
    for (i = 0; i < 20; i++)
    {
        /* Pulling the Tx line low */
        data |= UART_LCR_SBC;
        outb(data, SERIAL_PORT_BASE + UART_LCR);
	printk(KERN_ALERT "LED ON\n");
        msleep(1000);
        /* Defaulting the Tx line high */
        data &= ~UART_LCR_SBC;
        outb(data, SERIAL_PORT_BASE + UART_LCR);
	printk(KERN_ALERT "LED OFF\n");
        msleep(1000);
    }
    return 0;
}
 
void __exit cleanup_module()
{
 //	blink();
	printk(KERN_ALERT "Goodbye\n");
}
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email_at_sarika-pugs_dot_com>");
MODULE_DESCRIPTION("Blinking LED Hack");
