#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>
#include <linux/timer.h>

#include "device_file.h"

int kernel_version[2] = {0, 0};
int time;
module_param_array(kernel_version, int, NULL, 0644);
module_param_named(timer, time, int, 0644);

struct timer_list my_timer;
bool timer_expired = false;

static void my_timer_callback(struct timer_list *t)
{
    timer_expired = true;
    pr_info("Simple-driver: Timer expired!\n");
}

static int __init simple_driver_init(void)
{
    int major = ((LINUX_VERSION_CODE >> 16) & 0xFF);
    int minor = ((LINUX_VERSION_CODE >> 8) & 0xFF);

    if (kernel_version[0] != major || kernel_version[1] != minor)
    {
        pr_err("Simple-driver: Version mismatch! Kernel is %d.%d\n", major, minor);
        return -EINVAL;
    }

    if (register_device() < 0)
        return -1;

    timer_setup(&my_timer, my_timer_callback, 0);
    mod_timer(&my_timer, jiffies + msecs_to_jiffies(time * 1000));

    return 0;
}

static void __exit simple_driver_exit(void)
{
    extern bool read_done, write_done, timer_expired, order;
    extern char user_name[];

    if (read_done && write_done && !timer_expired && order)
    {
        pr_info("Simple-driver: Successfully completed the actions within time. User: %s\n", user_name);
    }
    else if(!order)
    {
        pr_err("Simple-driver: Task failure: Actions not completed in order.\n");
    }
    else
    {
        pr_err("Simple-driver: Task failure: Time Expired.\n");
    }

    unregister_device();
}

module_init(simple_driver_init);
module_exit(simple_driver_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Character Device");
MODULE_AUTHOR("Kona Bhuvaneswar Reddy - B240214CS");
MODULE_VERSION("1:1.0");
