#include <linux/module.h>
#include <linux/init.h>

static int __init a3_init(void)
{
	printk(KERN_INFO "Bin_Zeng_A3 module initialize:\n");
	printk(KERN_INFO "-Daddy, what are clouds made of?\n");
	printk(KERN_INFO "-Linux servers, mostly.\n");
	return 0;
}

static void __exit a3_exit(void)
{
	printk(KERN_INFO "Bin_Zeng_A3 module exit:\n");
	printk(KERN_INFO "I have to admit I copied this joke online...\n");
}

module_init(a3_init);
module_exit(a3_exit);
MODULE_LICENSE("GPL");
