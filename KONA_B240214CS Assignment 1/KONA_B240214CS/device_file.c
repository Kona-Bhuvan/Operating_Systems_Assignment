#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/device.h>  // Necessary for class_create/device_create
#include <linux/err.h>	   // Necessary for IS_ERR/PTR_ERR
#include <linux/uaccess.h> // Required for copy_to_user
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include "device_file.h"

// State tracking
extern struct timer_list my_timer;
bool order = true;
bool read_done = false;
bool write_done = false;
char user_name[32] = {0};

static dev_t g_devno;
static struct cdev g_cdev;
static struct class *g_class = NULL;
static struct device *g_device = NULL;
static const char *class_name = "my_dev_class";

DECLARE_WAIT_QUEUE_HEAD(read_wait);

static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer, size_t count, loff_t *position)
{
	char *string = "Write your username.\n";
	size_t size = strlen(string);

	if (*position >= size)
		return 0;

	if (*position + count > size)
		count = size - *position;

	if (copy_to_user(user_buffer, string + *position, count) != 0)
		return -EFAULT;

	*position += count;
	read_done = true;
	wake_up_interruptible(&read_wait);

	return count;
}

static ssize_t device_file_write(struct file *file_ptr, const char __user *user_buffer, size_t count, loff_t *position)
{
	if (!read_done)
	{
		order = false;
		pr_err("Simple-driver: ERROR - Write attempted before Read. Order violated!\n");
	}

	if (count > 31)
		count = 31;
	if (copy_from_user(user_name, user_buffer, count) != 0)
		return -EFAULT;
	
	if (count > 0 && user_name[count-1] == '\n') {
		user_name[count-1] = '\0';
	} else {
		user_name[count] = '\0';
	}
	write_done = true;
	pr_info("Simple-driver: Username '%s' recorded.\n", user_name);

	if (del_timer(&my_timer))
	{
		pr_info("Simple-driver: Actions completed! Timer stopped successfully.\n");
	}
	else
	{
		pr_info("Simple-driver: Actions completed, but timer had already expired.\n");
	}

	return count;
}

static struct file_operations simple_driver_fops = {
	.owner = THIS_MODULE,
	.read = device_file_read,
	.write = device_file_write,
};

int register_device(void)
{
	pr_notice("Simple-driver: register_cdev() is called.\n");
	unsigned baseminor = 0;
	unsigned minor_count_required = 1;
	const char *device_name = "my_dev";

	int result = alloc_chrdev_region(&g_devno, baseminor, minor_count_required, device_name);
	if (result)
	{
		pr_err("Simple-driver: alloc_chrdev_region failed: %d\n", result);
		goto err_out;
	}

	cdev_init(&g_cdev, &simple_driver_fops);
	g_cdev.owner = THIS_MODULE;
	result = cdev_add(&g_cdev, g_devno, minor_count_required);
	if (result)
	{
		pr_err("Simple-driver: cdev_add failed: %d\n", result);
		goto err_unregister_chrdev_region;
	}

	g_class = class_create(class_name);
	if (IS_ERR(g_class))
	{
		result = PTR_ERR(g_class);
		pr_err("Simple-driver: class_create failed: %d\n", result);
		goto err_cdev_del;
	}

	g_device = device_create(g_class, NULL, g_devno, NULL, device_name);
	if (IS_ERR(g_device))
	{
		result = PTR_ERR(g_device);
		pr_err("Simple-driver: device_create failed: %d\n", result);
		goto err_class_destroy;
	}
	extern int time;
	pr_notice("Simple-driver: Registered with Major = %i, Minor = %i, Timer = %d\n", MAJOR(g_devno), MINOR(g_devno), time);
	return 0;

err_class_destroy:
	class_destroy(g_class);
err_cdev_del:
	cdev_del(&g_cdev);
err_unregister_chrdev_region:
	unregister_chrdev_region(g_devno, minor_count_required);
err_out:
	return result;
}

void unregister_device(void)
{
	pr_notice("Simple-driver: unregister_device() is called\n");
	if (g_device)
	{
		device_destroy(g_class, g_devno);
		g_device = NULL;
	}

	if (g_class)
	{
		class_destroy(g_class);
		g_class = NULL;
	}

	cdev_del(&g_cdev);

	if (g_devno)
	{
		unregister_chrdev_region(g_devno, 1);
		g_devno = 0;
	}
	pr_info("Simple-driver: Unregistered\n");
}
