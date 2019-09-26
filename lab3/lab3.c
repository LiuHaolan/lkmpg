#include <linux/init.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>

MODULE_LICENSE("Dual BSD/GPL");

struct monkey{
	unsigned long id;
	struct list_head list;
};
//struct monkey wuk = {
//	.id = 0,
//	.list = LIST_HEAD_INIT(wuk.list),
//};
LIST_HEAD(my_list);

DEFINE_RWLOCK(lock);

static int add_monkey(unsigned long a)
{
	struct monkey *ple = kmalloc(sizeof *ple, GFP_KERNEL);
	if(!ple)
		return -ENOMEM;
	ple->id = a;
	
	write_lock(&lock);
	list_add(&ple->list, &my_list);
	write_unlock(&lock);

	return 0;
}

static int add_item(void){
	printk(KERN_INFO "In thread\n");
//	return add_monkey(1);	
	do_exit(0);
//	return 0;
}

static void destroy_list(void)
{
	struct list_head *i, *n;
	struct monkey_list *ple;

//	write_lock(&lock);
	list_for_each_safe(i, n, &my_list){
		ple = list_entry(i, struct monkey, list);
		list_del(i);
		kfree(ple);
	}
//	write_unlock(&lock);
}

static struct task_struct *thread_list[4];
int thread_init(void){
	printk(KERN_INFO "kthread init\n");
//	char* thread_name[4] = {"haolan1","haolan2","haolan3","haolan4"};
	int j=0;
	int a = 0;	
	for(j=0;j<4;j++){
		
		thread_list[j] = kthread_run(add_item, NULL, "haolan%d",j);
	}
/*
	for(a=0;a<4;j++){
		if(thread_list[a]){
			wake_up_process(thread_list[a]);
		}
	}
*/
	return 0;
}

void thread_destroy(void){
	int ret;
	int a=0;
	for(a=0;a<4;a++){
		ret = kthread_stop(thread_list[a]);
		if(!ret)
			printk(KERN_INFO "thread stopped");
	}
}

static int lab3_init(void){
	printk(KERN_ALERT "Hello wuklab\n");
	
	add_monkey(1);
//	thread_init();

	return 0;
}

static void lab3_exit(void){
//	thread_destroy();
	destroy_list();
	printk(KERN_ALERT "wuklab removed\n");
}


module_init(lab3_init);
module_exit(lab3_exit);
