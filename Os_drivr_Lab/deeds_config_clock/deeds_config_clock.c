#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/rtc.h>

#define PROCFS_MAX_SIZE	1024
#define PROCFS_NAME 	"buffer1k"

/**
 * The buffer used to store character for this module
 *
 */
static char procfs_buffer[PROCFS_MAX_SIZE];

/**
 * The size of the buffer
 *
 */
static unsigned long procfs_buffer_size = 0;

/* Flag to check if a recent write has been done */
int flag=3;

static int deeds_clock_proc_show(struct seq_file *m, void *v) 
{
	struct timeval time;
	const char config_1[1] = "1";
	const char config_0[1] = "0";
	struct rtc_time tm;
	unsigned long local_time;
	do_gettimeofday(&time);
	local_time = (u32)(time.tv_sec - (sys_tz.tz_minuteswest * 60));
	if((strncmp(procfs_buffer,config_0,1) == 0) && flag == 1)
	{
		seq_printf(m,"Current time :%llu\n",(unsigned long long)local_time);
		flag = 0;
	}
        else if( (strncmp(procfs_buffer,config_1,1) == 0) && flag == 1)
	{
		rtc_time_to_tm(local_time, &tm);
		seq_printf(m,"Current time: %04d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		flag = 0;
	}
	else if (flag == 1)
	{
		seq_printf(m,"Invalid option\n");
		flag = 0;
	}
	else if(flag == 3)
	{
			seq_printf(m,"No Configuration was written.Please write\n");
	}
	else
	{
		seq_printf(m,"Configuration = %s",procfs_buffer);
	}
	
	return 0;
}

static int deeds_clock_proc_open(struct inode *inode, struct  file *file) {
	return single_open(file, deeds_clock_proc_show, NULL);
}


int write_proc(struct file *file, const char *buffer, unsigned long count,
		   void *data)
{
	/* get buffer size */
	procfs_buffer_size = count;
	if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}
	
	/* write data to the buffer */
	if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
		return -EFAULT;
	}
	flag =1;
	
	return procfs_buffer_size;
}

static const struct file_operations deeds_config_clock_proc_fops = {
	.owner = THIS_MODULE,
	.open = deeds_clock_proc_open,
	.read = seq_read,
	.write = write_proc,
};

static int __init deeds_clock_proc_init(void) {
	struct proc_dir_entry *proc_dir = NULL;
	proc_dir = proc_create("deeds_config_clock", 0666, NULL, &deeds_config_clock_proc_fops);
	if( NULL == proc_dir )
	{
		printk(KERN_ERR"proc creation failed\n");
		return -1;
	}
	else
	{
		printk(KERN_INFO "proc initialized");
	}
  	return 0;
}

static void __exit deeds_clock_proc_exit(void) {
	remove_proc_entry("deeds_config_clock", NULL);
}

MODULE_LICENSE("GPL");
module_init(deeds_clock_proc_init);
module_exit(deeds_clock_proc_exit);
