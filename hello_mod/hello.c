/* hello_mod/hello.c */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#include <asm/errno.h>

#define DEVICE_NAME "hello"

static int major;
static struct class *cls;

static int hello_open(struct inode *inode, struct file *file);
static int hello_release(struct inode *inode, struct file *file);
static ssize_t hello_read(struct file *file, char __user *buf, size_t len,
			  loff_t *off);
static ssize_t hello_write(struct file *file, const char __user *buf,
			   size_t len, loff_t *off);

static struct file_operations hello_cdev_fops = {
	.read = hello_read,
	.write = hello_write,
	.open = hello_open,
	.release = hello_release,
};

static int __init hello_init(void)
{
	major = register_chrdev(0, DEVICE_NAME, &hello_cdev_fops);

	if (major < 0) {
		pr_alert("failed to register cdev %d\n", major);
		return major;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0)
	cls = class_create(DEVICE_NAME);
#else
	cls = class_create(THIS_MODULE, DEVICE_NAME);
#endif

	device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

	pr_info("created /dev/%s\n", DEVICE_NAME);

	return 0;
}

static void __exit hello_exit(void)
{
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major, DEVICE_NAME);

	pr_info("destroyed /dev/%s\n", DEVICE_NAME);
}

static int hello_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int hello_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t hello_read(struct file *file, char __user *buf, size_t len,
			  loff_t *offset)
{
	const char msg[] = "Hello, World!\n";
	const char *msg_off;
	size_t msg_len;
	unsigned long res;

	if (*offset >= sizeof(msg)) {
		*offset = 0;
		return 0;
	}

	msg_off = msg + *offset;
	msg_len = min(len, strlen(msg_off));

	res = copy_to_user(buf, msg_off, msg_len);

	*offset += (msg_len - res);

	return msg_len - res;
}

static ssize_t hello_write(struct file *file, const char __user *buf,
			   size_t len, loff_t *off)
{
	pr_alert("unsupported operation\n");
	return -EINVAL;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aven Bross <email@example.com>");
MODULE_DESCRIPTION("A kernel space hello world character device");
MODULE_VERSION("0.1");
