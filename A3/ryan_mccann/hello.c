#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int hello_init(void)
{
	printk(KERN_ALERT "If I wasn't into computer science and engineering I'd probably be in eastern Kansas playing the guitar to a group of well behaved cattle.\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "I'd pay big money for an app that could tell me how many Wendy's plain spicy chicken sandwiches I've eaten in my lifetime.\n");
}

module_init(hello_init);
module_exit(hello_exit);
