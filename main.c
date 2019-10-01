/*
 * main.c
 *
 *  Created on: Sep 15, 2019
 *      Author: khoben
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#include "phone_book.h"

#define DEV_MAJOR 75
#define DEV_MINOR 0
#define DEV_NAME "phonebook"

#define PROCFS_NAME "pb_storage"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Phonebook");
MODULE_AUTHOR("-_-");

#define MAXSTR 100

static char msg[MAXSTR] =
{ 0 };
static int times = 0;

static struct PhoneBook *phonebook;

static int
dev_open(struct inode*, struct file*);
static int
dev_rls(struct inode*, struct file*);
static ssize_t
dev_read(struct file*, char*, size_t, loff_t*);
static ssize_t
dev_write(struct file*, const char*, size_t, loff_t*);

static struct file_operations fops =
{ .read = dev_read, .open = dev_open, .write = dev_write, .release = dev_rls };


void load_from_procfs(void)
{
	struct file *f;
	char buf[128];
	mm_segment_t fs;
	int i;
	for(i=0;i<128;i++)
		buf[i] = 0;
	f = filp_open("/proc/pb_storage", O_RDONLY, 0);
	if (f == NULL)
	{
		printk(KERN_ALERT "filp_open error.\n");
	}
	else{
		fs = get_fs();
		set_fs(KERNEL_DS);
		f->f_op->read(f, buf, 128, &f->f_pos);
		set_fs(fs);
		loadPhoneBook(phonebook, buf, 128);
	}
	printk(KERN_INFO "buf:%s\n",buf);
	filp_close(f, NULL);
}

void print_phonebook(void)
{

}

void init_phonebook(void)
{
	phonebook = createPhoneBook();
	if (phonebook == NULL)
		return;
	// try to read from procfs
	load_from_procfs();
	print_phonebook();
}

int init_module(void)
{
	int t = register_chrdev(DEV_MAJOR, DEV_NAME, &fops);

	if (t < 0)
		printk (KERN_ALERT "Device registration Failed");
	else
	{
		printk (KERN_ALERT "Device registered \n");
		init_phonebook();
		if (phonebook == NULL)
			printk (KERN_ALERT "Phonebook can`t load properly\n");
	}

	return t;
}

void cleanup_module(void)
{
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
}


static int dev_open(struct inode *inod, struct file *fil)
{
	times++;
	printk (KERN_ALERT "Device opened %d times\n", times);

	return 0;
}


static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off)
{

	int sz = len >= strlen(msg) ? strlen(msg) : len;

	if (*off >= strlen(msg))
		return 0;

	if (copy_to_user(buff, msg, sz))
		return -EFAULT;

	*off += sz;

	return len;

}


static void process(void)
{
	char getStr[1024] = { };
	strncpy(getStr, getRecord(getStr, *phonebook, msg), MAXSTR);
	strncpy(msg, getStr, MAXSTR);
	strncpy(msg, strcat(msg, "\n"), MAXSTR);
}


static ssize_t dev_write(struct file *fil, const char *buff, size_t len,
		loff_t *off)
{
	unsigned long ret;
	printk (KERN_INFO "dev write \n");
	if (len > sizeof(msg) - 1)
		return -EINVAL;
	ret = copy_from_user(msg, buff, len);
	if (ret)
		return -EFAULT;
	msg[len] = '\0';
	process();
	return len;
}


static int dev_rls(struct inode *inod, struct file *fil)
{
	printk (KERN_ALERT "Device Closed\n");

	return 0;
}

