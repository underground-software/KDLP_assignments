#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int hello_init(void)
{
	printk(KERN_ALERT
	       "When you think about it, aren't the people creating genetically modified corn the real kernel engineers?\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT
	       "Why do they use Linux in space?\n");
	printk(KERN_ALERT
	       "Because you can't open Windows in space\n");
}

module_init(hello_init);
module_exit(hello_exit);
