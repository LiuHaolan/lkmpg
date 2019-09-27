#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/random.h>
#include <asm/uaccess.h>
MODULE_LICENSE("Dual BSD/GPL");

#define procfs_name "wuklab"
struct proc_dir_entry *wuklab_file;

#define BUF_SIZE 100

static int rand=0;

static ssize_t procfile_read(struct file * f,char __user* ubuf, size_t i, loff_t * offset)
{
	if(*offset >0)
		return 0;
	
	get_random_bytes(&rand, sizeof(rand));
//	printk(KERN_ALERT"random number generated %d \n",rand);
	char buf[BUF_SIZE];
	int len = sprintf(buf,"%d\n",rand);
	if(copy_to_user(ubuf, buf, len)){
		printk(KERN_ALERT"copy failed");		
		return -EFAULT;
	}

	*offset = len;
	return len;
}

static ssize_t procfile_write(struct file * f,const char * data, size_t count, loff_t * offset)
{
	if(*offset >0)
		return 0;
	char buf[BUF_SIZE];
	if(copy_from_user(buf, data, count))
		return -EFAULT;
	buf[count] = '\0';
	printk(KERN_ALERT "%s", buf);
	*offset = count;
	return count;

}

static const struct file_operations hello_proc_fops = {
	.owner = THIS_MODULE,
	.read = procfile_read,
	.write = procfile_write
};

static int hello_init(void){
	printk(KERN_ALERT "Hello wuklab\n");
	wuklab_file = proc_create(procfs_name, 0660, NULL, &hello_proc_fops);
	if(wuklab_file == NULL){
		proc_remove(wuklab_file);
		printk(KERN_ALERT "Initialize %s failed\n", procfs_name);
		return -ENOMEM;
	}

	printk(KERN_ALERT "Initialize %s success\n", procfs_name);
	return 0;
}

static void hello_exit(void){
	proc_remove(wuklab_file);
	printk(KERN_ALERT "wuklab removed\n");
}


module_init(hello_init);
module_exit(hello_exit);
