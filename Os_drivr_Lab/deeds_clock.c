#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/time.h>

static int deeds_clock_proc_show(struct seq_file *m, void *v) {
  struct timeval time;
  unsigned long long secs;
  do_gettimeofday(&time);
  secs = (unsigned long long)time.tv_sec;
  seq_printf(m, "current time:%llu\n",secs);
  return 0;
}

static int deeds_clock_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, deeds_clock_proc_show, NULL);
}

static const struct file_operations deeds_clock_proc_fops = {
  .owner = THIS_MODULE,
  .open = deeds_clock_proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

static int __init deeds_clock_proc_init(void) {
	struct proc_dir_entry *proc_dir = NULL;
	proc_dir = proc_create("deeds_clock", 0, NULL, &deeds_clock_proc_fops);
	if( NULL == proc_dir )
	{
		printk(KERN_INFO,"proc creation failed\n");
		return -1;
	}
	else
	{
		printk(KERN_INFO,"proc creation successful\n");
	}
  return 0;
}

static void __exit deeds_clock_proc_exit(void) {
  remove_proc_entry("deeds_clock", NULL);
}

MODULE_LICENSE("GPL");
module_init(deeds_clock_proc_init);
module_exit(deeds_clock_proc_exit);
