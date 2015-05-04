#include <linux/init.h>
#include <linux/module.h>
#include <linux/unistd.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void){
	printk(KERN_ALERT "Hello, World #2\n");
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);



