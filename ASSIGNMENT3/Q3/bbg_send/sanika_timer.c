
/**************************************************************************************
* Reference: http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/
* Robert love: Linux kernel development: page 222-224 (Timers)
**************************************************************************************/
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/stat.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SANIKA DONGRE");
MODULE_DESCRIPTION("Creating own kernel module that uses a kernel timer to wakeup periodically every 500msec");
MODULE_VERSION("0.1");

struct timer_list timerkernel;
static char *name = "Sanika";
static unsigned long counter = 0;
static unsigned long period = 500;


module_param(name, charp, 0644);
module_param(period,ulong,0644);
MODULE_PARM_DESC(name, "Enter the name");
MODULE_PARM_DESC(period, "Enter the period");
void timefunc(struct timer_list* timer)
{
	counter++;
        mod_timer(&timerkernel, jiffies+msecs_to_jiffies(period));
        //printk("Timeout is for 500 ms\n");
	printk(KERN_INFO "The name and the counter value is: %s\n and %lu\n",name,counter);
}

static int __init mymodule_init(void)
{
	printk(KERN_INFO "mymodule_init Initialization\n");
        printk(KERN_INFO "timer module is inserted into the kernel.Function%s\n",__FUNCTION__);
	timer_setup(&timerkernel, timefunc,0);
	add_timer(&timerkernel);
	//mod_timer(&timerkernel, jiffies+msecs_to_jiffies(period));
	return 0;
}

static void __exit mymodule_exit(void)
{
	counter=0;
	del_timer(&timerkernel);
	printk(KERN_INFO "timer module is exited from kernel.Function%s\n",__FUNCTION__);
	return;

}

module_init(mymodule_init);
module_exit(mymodule_exit);


