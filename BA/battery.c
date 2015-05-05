#include <linux/sched.h>
#include <linux/rcupdate.h>
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <asm/siginfo.h>
#include <linux/signal.h>

MODULE_LICENSE("GPL");


#define PROCFS_MAX_SIZE         128 
#define PROCFS_TESTLEVEL        "battery_test"
#define PROCFS_NOTIFYPID        "battery_notify"
#define PROCFS_THRESHOLD        "battery_threshold"
#define TRUE	1
#define FALSE 0


/* Declaration of variables used in this module */

static int level = 99;
static int test_level = 0;                      //indicates level of battery remain.
static int oldest_level = 0;
static int already_alert= FALSE;
static int notify_pid = -1;
static int threshold = -1;
/* End of declaration */



/* Declaration of ancillary variables */

static char procfs_buffer[PROCFS_MAX_SIZE];     
static unsigned long procfs_buffer_size = 0;    //size of receive side buffer
static struct proc_dir_entry *proc_entry;       //indicates procfs entry.


static char notify_buffer[PROCFS_MAX_SIZE];     
static unsigned long notify_buffer_size = 0;    //size of receive side buffer
static struct proc_dir_entry *notify_entry; 

static char threshold_buffer[PROCFS_MAX_SIZE];     
static unsigned long threshold_buffer_size = 0;    //size of receive side buffer
static struct proc_dir_entry *threshold_entry;

/* End of declaration */

/*
        Implementation of procfs write function
*/
static int test_level_write( struct file *filp, const char *user_space_buffer, unsigned long len, loff_t *off )
{

        int status = 0;
        int requested;

        procfs_buffer_size = len;

	struct siginfo info;
	struct task_struct* task = NULL;

	if(notify_pid < 0 || threshold<0){
		printk("test_level_write\n");
		return;
	}

        if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
                procfs_buffer_size = PROCFS_MAX_SIZE;
        }

        /* write data to the buffer */
        if ( copy_from_user(procfs_buffer, user_space_buffer, procfs_buffer_size) ) {
                return -EFAULT;
        }

        status  = kstrtoint(procfs_buffer, 10, &requested);
        if(status < 0)
        {
                printk(KERN_INFO "Error while called kstrtoint(...)\n");
                return -ENOMEM;
        }
        // validate level value.
        if(requested< 0 || requested > 100){
                printk(KERN_INFO "Invalid battery level.\n");
                return -ENOMEM;
        }
        // accept value.
        test_level = requested;

	memset(&info, 0, sizeof(struct siginfo));
	info.si_signo = SIGUSR1;
	info.si_code = SI_QUEUE;
	
	task = pid_task(find_vpid(notify_pid), PIDTYPE_PID);
	send_sig_info(SIGUSR1, &info,task);

	/*
	//first	time
	if( oldest_level){
		
		if( already_alert == FALSE){
			//task = pid_task(find_vpid(notify_pid), PIDTYPE_PID);
			//memset(&info, 0, sizeof(struct siginfo));
			//info.si_code = SI_QUEUE;
			
			//decrease	
			if( oldest_level - test_level < 0){
				//signal to user "going to saving mode"
				//info.si_signo = SIGUSR1;
				//send_sig_info(SIGUSR1, &info, task);	
			}	
			//increase
			else{
				//signal to user "going to normal mode"
				//info.si_signo = SIGUSR2;
				//send_sig_info(SIGUSR2, &info, task);	
			}
			
			already_alert == TRUE;
		}
			
	}	
	else{
		oldest_level = test_level ;
	}
	*/
        return procfs_buffer_size;
}

/*
        Implementation of procfs read function
*/
static int test_level_read( struct file *filp, char *user_space_buffer, size_t count, loff_t *off )
{
        int ret = 0;
        int flag = 0;

        if(*off < 0) *off = 0;

        snprintf(procfs_buffer, 16, "%d\n", test_level);
        procfs_buffer_size = strlen(procfs_buffer);

        if(*off > procfs_buffer_size){
                return -EFAULT;
        }else if(*off == procfs_buffer_size){
                return 0;
        }

        if(procfs_buffer_size - *off > count)
                ret = count;
        else
                ret = procfs_buffer_size - *off;

       flag = copy_to_user(user_space_buffer, procfs_buffer + (*off), ret);

        if(flag < 0)
                return -EFAULT;

        *off += ret;

        return ret;
}


static int battery_notify_write( struct file *filp, const char *user_space_buffer, unsigned long len, loff_t *off )
{

        int status = 0;
        int requested;

        notify_buffer_size = len;

        if (notify_buffer_size > PROCFS_MAX_SIZE ) {
                notify_buffer_size = PROCFS_MAX_SIZE;
        }

        /* write data to the buffer */
        if ( copy_from_user(notify_buffer, user_space_buffer, notify_buffer_size) ) {
                return -EFAULT;
        }

        status  = kstrtoint(notify_buffer, 10, &requested);
        if(status < 0)
        {
                printk(KERN_INFO "Error while called kstrtoint(...)\n");
                return -ENOMEM;
        }
        
	// accept value.
        notify_pid = requested;

	printk("####################notify_pid %d 1111111111111\n",notify_pid);

        return notify_buffer_size;

}


static int notify_read( struct file *filp, char *user_space_buffer, size_t count, loff_t *off )
{
        int ret = 0;
        int flag = 0;

        if(*off < 0) *off = 0;

        snprintf(notify_buffer, 16, "%d\n", notify_pid);
        notify_buffer_size = strlen(notify_buffer);


        if(*off > notify_buffer_size){
                return -EFAULT;
        }else if(*off == notify_buffer_size){
                return 0;
        }

        if(notify_buffer_size - *off > count)
                ret = count;
        else
                ret = notify_buffer_size - *off;

       flag = copy_to_user(user_space_buffer, notify_buffer + (*off), ret);

        if(flag < 0)
                return -EFAULT;

        *off += ret;

        return ret;
}

static int battery_threshold_write( struct file *filp, const char *user_space_buffer, unsigned long len, loff_t *off )
{

        int status = 0;
        int requested;

       	threshold_buffer_size = len;

        if (threshold_buffer_size > PROCFS_MAX_SIZE ) {
                threshold_buffer_size = PROCFS_MAX_SIZE;
        }

        /* write data to the buffer */
        if ( copy_from_user(threshold_buffer, user_space_buffer, threshold_buffer_size) ) {
                return -EFAULT;
        }

        status  = kstrtoint(threshold_buffer, 10, &requested);
        if(status < 0)
        {
                printk(KERN_INFO "Error while called kstrtoint(...)\n");
                return -ENOMEM;
        }
        // validate level value.
        if(requested< 0 || requested > 100){
                printk(KERN_INFO "Invalid battery level.\n");
                return -ENOMEM;
        }
        // accept value.
        threshold = requested;

        return threshold_buffer_size;

}

static int threshold_read( struct file *filp, char *user_space_buffer, size_t count, loff_t *off )
{
        int ret = 0;
        int flag = 0;

        if(*off < 0) *off = 0;

        snprintf(threshold_buffer, 16, "%d\n", threshold);
        threshold_buffer_size = strlen(threshold_buffer);

        if(*off > threshold_buffer_size){
                return -EFAULT;
        }else if(*off == threshold_buffer_size){
                return 0;
        }

        if(threshold_buffer_size - *off > count)
                ret = count;
        else
                ret = threshold_buffer_size - *off;

       flag = copy_to_user(user_space_buffer, threshold_buffer + (*off), ret);

        if(flag < 0)
                return -EFAULT;

        *off += ret;

        return ret;
}

/*
        Configuration of file_operations

        This structure indicate functions when read or write operation occured.
*/
static const struct file_operations my_proc_fops = {
        .write = test_level_write,
        .read = test_level_read,
};


static const struct file_operations battery_notify_fops = {
        .write = battery_notify_write,
        .read = notify_read,
};


static const struct file_operations battery_threshold_fops = {
        .write = battery_threshold_write,
        .read = threshold_read,
};




/*
        This function will be called on initialization of  kernel module
*/
int init_module(void)
{

        int ret = 0;

        proc_entry = proc_create(PROCFS_TESTLEVEL, 0666, NULL, &my_proc_fops);
 	notify_entry =proc_create(PROCFS_NOTIFYPID,0666,NULL,&battery_notify_fops);	
	threshold_entry = proc_create(PROCFS_THRESHOLD,0666,NULL, &battery_threshold_fops);


        if(proc_entry == NULL && notify_entry== NULL && threshold_entry==NULL)
        {
                return -ENOMEM;
        }
        return ret;

}

/*
        This function will be called on cleaning up of kernel module
*/
void cleanup_module(void)
{
        remove_proc_entry(PROCFS_TESTLEVEL, proc_entry);
        remove_proc_entry(PROCFS_NOTIFYPID, notify_entry);
        remove_proc_entry(PROCFS_THRESHOLD, threshold_entry);
}
