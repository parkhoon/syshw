#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define CHR_DEV_NAME "chr_dev"	//  name of device
#define CHR_DEV_MAJOR 240	//  major number of deivce
#define BUFF_SIZE 128		//  buffer size 

static int test_level =50;
static char buffer[BUFF_SIZE];


int chr_open(struct inode *inode, struct file *filp){

	int number = MINOR(inode ->i_rdev); // store minor number at ( var : number )
	printk("Virtual Character Device Open : Minor Number is %d\n", number);
	return 0;
}

ssize_t chr_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos){
	
	printk("write data: %s\n", buf);	//application write();
	return count;				
}	

ssize_t chr_read(struct file *filp, const char *buf, size_t count, loff_t *f_pos){

	snprintf(buffer,sizeof(buffer),"%d",test_level);
	copy_to_user(buf,buffer,BUFF_SIZE);     //kernel to user 
	printk("read data: %s\n", buf);		//application read();
	return count;
}

int chr_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg){
	
	switch(cmd){
		case 0:
			printk("cmd value is %d\n", cmd);
			break;
		case 4:
			printk("cmd value is %d\n", cmd);
			break;
	}
	return 0;
}

int chr_release(struct inode *inode, struct file *filp){
	
	printk("Virtual Character Device Release\n");
	return 0;
}
		
struct file_operations chr_fops = {
	
	owner: THIS_MODULE,
	unlocked_ioctl: chr_ioctl,
	write: chr_write,
	read: chr_read,
	open: chr_open,
	release: chr_release
};

int sample_init(void){
	
	int registration;

	printk("Registration Character Device to Kernel\n");
	registration = register_chrdev(CHR_DEV_MAJOR,CHR_DEV_NAME, &chr_fops);
		
//	buffer = (char*)kmalloc(BUFF_SIZE,GFP_KERNEL);
//	memset(buffer,0,BUFF_SIZE);
  	//strncpy(buffer,test_level,BUFF_SIZE);	
	if( registration < 0)
		return registration;
	
	printk("Major Number:%d\n", registration);
	return 0;
}

void sample_cleanup(void){
//	kfree(buffer);	
	printk("Unregistration Character Device to Kernel\n");
	unregister_chrdev(CHR_DEV_MAJOR, CHR_DEV_NAME);
}

MODULE_LICENSE("GPL");
module_init(sample_init);
module_exit(sample_cleanup);
	
