/*static int deeds_clock_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, NULL, NULL);
}*/



#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#define MAX_PROC_SIZE 100
static char proc_data[MAX_PROC_SIZE];


static ssize_t read_proc(char *buf,char **start,off_t offset,size_t count,int *eof,void *data )
{
ssize_t len=0;
 len = sprintf(buf,"\n %s\n ",proc_data);

return len;
}

static ssize_t  write_proc(struct file *file,const char *buf,ssize_t count,void *data )
{

if(count > MAX_PROC_SIZE)
    count = MAX_PROC_SIZE;
if(copy_from_user(proc_data, buf, count))
    return -EFAULT;

return count;
}

static const struct file_operations deeds_config_clock_proc_fops = {
  .owner = THIS_MODULE,
//  .open = deeds_clock_proc_open,
  .read = read_proc,
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
		//proc_dir->read_proc = &read_proc ;
		//proc_dir->write_proc = &write_proc;
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
