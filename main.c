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
#define DEV_NAME "altstu"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Character device driver AltSTU");
MODULE_AUTHOR("Folk");

#define MAXSTR 100

static char msg[MAXSTR] =
{ 0 };
static int times = 0;

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

int init_module(void)
{
	int t = register_chrdev(DEV_MAJOR, DEV_NAME, &fops);

	if (t < 0)
	printk (KERN_ALERT "Device registration Failed");
	else
	printk (KERN_ALERT "Device registered \n");

	return t;
}

void cleanup_module(void)
{
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
}

/**
 * РћС‚РєСЂС‹С‚РёРµ СѓСЃС‚СЂРѕР№СЃС‚РІР°
 */
static int dev_open(struct inode *inod, struct file *fil)
{
	times++;
	printk (KERN_ALERT "Device opened %d times\n", times);

	return 0;
}

/**
 * РћР±СЂР°Р±РѕС‚С‡РёРє С‡С‚РµРЅРёСЏ РґР°РЅРЅС‹С… СЃ СѓСЃС‚СЂРѕР№СЃС‚РІР°
 */
static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off)
{

	int sz = len >= strlen(msg) ? strlen(msg) : len;

	if (*off >= strlen(msg))
		return 0;

	if (copy_to_user(buff, msg, sz))
		return -EFAULT;

	*off += sz; //РєРѕСЂСЂРµРєС†РёСЏ С‡РёСЃР»Р° РѕР±СЂР°Р±РѕС‚Р°РЅРЅС‹С… РґР°РЅРЅС‹С…: РёРЅР°С‡Рµ Р±СѓРґРµС‚ РІСЃРµ-РІСЂРµРјСЏ РѕС‚РґР°РІР°С‚СЊ  С‚Сѓ-Р¶Рµ СЃС‚СЂРѕРєСѓ СЃ РЅР°С‡Р°Р»Р°

	return len;

}

/**
 *Р§С‚Рѕ-С‚Рѕ РґРµР»Р°РµРј СЃРѕ СЃС‚СЂРѕРєРѕР№ РѕС‚ РїРѕР»СЊР·РѕРІР°С‚РµР»СЏ
 */
static void process(void)
{
//  char hi[MAXSTR] = "Hello, ";
//  strncpy (msg, strcat (hi, msg), MAXSTR);
//  strncpy (msg, strcat (msg, "\n"), MAXSTR);
	struct PhoneBook *book;
	char getStr[1024] = { };
	book = createPhoneBook();
	putRecord(book, "1", "2", "3");
//	putRecord(book, "1", "2", "3");
	strncpy(getStr, getRecord(getStr, *book, msg), MAXSTR);
	strncpy(msg, getStr, MAXSTR);
	strncpy(msg, strcat(msg, "\n"), MAXSTR);
}

/**
 * РћР±СЂР°Р±РѕС‚С‡РёРє Р·Р°РїРёСЃРё РІ СѓСЃС‚СЂРѕР№СЃС‚РІРѕ
 */
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
	process(); //РѕР±СЂР°Р±РѕС‚РєР° РїРѕСЃР»Рµ РїРѕР»СѓС‡РµРЅРёСЏ С‚СѓС‚
	return len;
}

/**
 * Р—Р°РєСЂС‹С‚РёРµ СѓСЃС‚СЂРѕР№СЃС‚РІР°
 */
static int dev_rls(struct inode *inod, struct file *fil)
{
	printk (KERN_ALERT "Device Closed\n");

	return 0;
}

