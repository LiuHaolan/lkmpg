/*
 * Copyright (c) 2016-2017 Wuklab, Purdue University. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

//#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/jiffies.h>

#define MAX_FILENAME_LENGTH 100



struct mc_msg_queue{
	char* msg_data;
	unsigned int msg_size;
	struct list_head list;
};

//static LIST_HEAD(name_nid_dict);

unsigned int append(char* msg_data, unsigned int msg_size, struct list_head* name_nid_dict){
	struct mc_msg_queue* tmp;
	
	tmp = kmalloc(sizeof(struct mc_msg_queue), GFP_KERNEL);
	if(!tmp){
		printk("allocated wrong\n");
		return 0;
	}

	tmp->msg_data = kmalloc(sizeof(char)*(msg_size+1), GFP_KERNEL);
	strcpy(tmp->msg_data, msg_data);
	tmp->msg_size = msg_size;
	list_add_tail(&tmp->list, name_nid_dict);	
	
	return 1;
	// return error if nothing
}



// the msg data passed in should copy a new memory here, msg_data should point to a continous memory
//
unsigned int pop(char* msg_data, int* msg_size, struct list_head* list){
	struct list_head* next = list->next;
	
	struct mc_msg_queue* item = list_entry(next, struct mc_msg_queue, list);
		
	strcpy(msg_data, item->msg_data);
	*msg_size = item->msg_size;
	list_del(list);

	kfree(item->msg_data);	
	kfree(item);
	list = next;

	return 1;
}

/*
 * print name_nid_dict
 */


void print(struct list_head* name_nid_dict){
	struct mc_msg_queue *pos = NULL;
	list_for_each_entry(pos, name_nid_dict, list){
		printk(pos->msg_data);
		printk(" ");
		printk("message data: %d\n", pos->msg_size);
	}
}


void free_all(struct list_head* name_nid_dict){
	struct list_head* cur=name_nid_dict->next;
	while(cur != name_nid_dict){
		cur = cur->next;
		struct mc_msg_queue* item = list_entry(cur, struct mc_msg_queue, list);
		kfree(item->msg_data);
		kfree(item);
	}
	struct mc_msg_queue* item = list_entry(name_nid_dict, struct mc_msg_queue, list);
	kfree(item->msg_data);
	kfree(item);
}

LIST_HEAD(yi_list);
	
static int mq_test_module_init(void)
{
	printk("loaded yi\n");
	
	append("NVSL", 4, &yi_list);
	append("STABLE", 3, &yi_list);

	print(&yi_list);
	return 0;
}


static void mq_test_module_exit(void)
{
	printk("offload! \n");
	free_all(&yi_list);
	printk("free done\n");
//	pr_info("lego memory monitor module exit\n");
}

module_init(mq_test_module_init);
module_exit(mq_test_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wuklab@Purdue");
