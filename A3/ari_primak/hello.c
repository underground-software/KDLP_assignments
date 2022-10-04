// Copied wholesale from the slides, except for the string literals changed
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int hello_init(void)
{
	printk(KERN_ALERT "here comes dat boi\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "oh shit waddup\n");
}
module_init(hello_init);
module_exit(hello_exit);
