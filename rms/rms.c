#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/random.h>
#include <linux/uaccess.h>
MODULE_LICENSE("Dual BSD/GPL");

#define procfs_name "mp2"
#define status_name "status"
struct proc_dir_entry *mp2_dir;
struct proc_dir_entry *status_entry;

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
	printk(KERN_ALERT "Initializing Rate-Montonic Scheduler\n");

//	struct proc_dir_entry *proc_mp2;
	mp2_dir = proc_mkdir(procfs_name, NULL);
	if(!mp2_dir){
		printk(KERN_INFO, "Error creating mp2 directory");
		return -ENOMEM;
	}
//	create_

	status_entry = proc_create(status_name, 0666, mp2_dir, &hello_proc_fops);
	if(status_entry == NULL){
		proc_remove(status_entry);
		printk(KERN_ALERT "Initialize %s failed\n", procfs_name);
		return -ENOMEM;
	}

	printk(KERN_ALERT "Initialize %s success\n", procfs_name);
	return 0;
}

static void hello_exit(void){
	remove_proc_entry(status_name, mp2_dir);
	remove_proc_entry(procfs_name,NULL);
	printk(KERN_ALERT "wuklab removed\n");
}


module_init(hello_init);
module_exit(hello_exit);
