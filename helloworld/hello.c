#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/random.h>
MODULE_LICENSE("Dual BSD/GPL");

#define procfs_name "wuklab"
struct proc_dir_entry *wuklab_file;

//int procfile_read(char* buffer, char*buffer_location, off_t offset, int buffer_lenght, int* eof, void *data)
int procfile_read(struct file * f,char * data, size_t i, loff_t * offset)
{
	int rand;
	get_random_bytes(&rand, sizeof(rand));
	printk(KERN_ALERT"random number generated %d \n",rand);
	return 0;
}

int procfile_write(struct file * f,const char * data, size_t i, loff_t * offset)
{

	printk(KERN_ALERT"echo: %d",(int)i);
	return -EINVAL;
}

static const struct file_operations hello_proc_fops = {
	.owner = THIS_MODULE,
//	.readproc = hello
	.read = procfile_read,
	.write = procfile_write
};

static int hello_init(void){
	printk(KERN_ALERT "Hello wuklab\n");
	wuklab_file = proc_create(procfs_name, 0, NULL, &hello_proc_fops);
	if(wuklab_file == NULL){
		proc_remove(wuklab_file);
		printk(KERN_ALERT "Initialize %s failed\n", procfs_name);
		return -ENOMEM;
	}
/*	
	wuklab_file->read_proc = procfile_read;
	wuklab_file->owner = THIS_MODULE;
	wuklab_file->mode = S_IFREG | S_IRUGO;
	wuklab_file->uid = 0;
	wuklab_file->gid = 0;
	wuklab_file->size = 37;
*/
	printk(KERN_ALERT "Initialize %s success\n", procfs_name);
	return 0;
}

static void hello_exit(void){
	proc_remove(wuklab_file);
	printk(KERN_ALERT "wuklab removed\n");
}


module_init(hello_init);
module_exit(hello_exit);
