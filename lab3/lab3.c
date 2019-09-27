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

LIST_HEAD(my_list);

DEFINE_RWLOCK(lock);

static int add_monkey(long a)
{
//	printk(KERN_INFO "%d", a);
	//haolan: is kmalloc thread-safe?
	struct monkey *ple = kmalloc(sizeof *ple, GFP_KERNEL);
	if(!ple)
		return -ENOMEM;
	ple->id = a;
	
	write_lock(&lock);
	list_add(&ple->list, &my_list);
	write_unlock(&lock);

	return 0;
}

typedef struct {
	long n;
	long input;
} argu_add_item ;

static int add_item(void* data){
	argu_add_item* argu = (argu_add_item*)data;
	long n = argu->n;
	long input = argu->input;
	printk(KERN_INFO "In thread\n");
	// input n argus
	int q = 0;	
	for(q=0;q<n;q++)
		add_monkey(input);	
//	do_exit(0);
//	return 0;
	while(!kthread_should_stop()){
		flush_signals(current);
		set_current_state(TASK_INTERRUPTIBLE);
		if(!kthread_should_stop())schedule();
		set_current_state(TASK_RUNNING);
	}
	kfree(argu);
	return 0;
}
#define BUFSIZE 100
static void show_list(void)
{
	struct list_head *i, *n;
	struct monkey *ple;

//	read_lock(&lock);
	int buf[BUFSIZE];
	int cnt = 0;
	list_for_each_safe(i, n, &my_list){
		ple = list_entry(i, struct monkey, list);
		buf[cnt++] = ple->id;
	}
	int d=0;
	printk(KERN_INFO "\n");
	for(d=0;d<cnt;d++){
		printk("%d ",buf[d]);
	}
	printk(KERN_INFO "\n");
//	read_unlock(&lock);
}

static void destroy_list(void)
{
	struct list_head *i, *n;
	struct monkey *ple;

//	write_lock(&lock);
	list_for_each_safe(i, n, &my_list){
		ple = list_entry(i, struct monkey, list);
		list_del(i);
		kfree(ple);
	}
//	write_unlock(&lock);
}

static struct task_struct *thread_list[4];
int thread_init(long n){
	printk(KERN_INFO "kthread init\n");

	argu_add_item* argu[4];
	int j=0;	
	for(j=0;j<4;j++){
		argu[j] = kmalloc(sizeof *argu, GFP_KERNEL);
		argu[j]->n = n;
	}	
	
	
	long a = 0;	
	for(j=0;j<4;j++){
		argu[j]->input = j;
		thread_list[j] = kthread_run(add_item, (void*)(argu[j]), "haolan%d",j);
	}

	return 0;
}

void thread_destroy(void){
	int ret;
	int a=0;
	for(a=0;a<4;a++){
		
		ret = kthread_stop(thread_list[a]);
		if(!ret)
			printk(KERN_INFO "thread stopped %d\n", a);
		
	}
}

static int lab3_init(void){
	printk(KERN_ALERT "Hello wuklab\n");
	
//	add_monkey(1);
	thread_init(3);

	return 0;
}

static void lab3_exit(void){
	thread_destroy();
	//test
	show_list();
	destroy_list();
	printk(KERN_ALERT "wuklab removed\n");
}


module_init(lab3_init);
module_exit(lab3_exit);
