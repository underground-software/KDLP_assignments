#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int my_module_init(void)
{
	printk(KERN_ALERT "I'm like hey what's up hello\n");
	return 0;
}

static void my_module_exit(void)
{
	printk(KERN_ALERT "I be in the kitchen cooking pies\n");
}
module_init(my_module_init);
module_exit(my_module_exit);
