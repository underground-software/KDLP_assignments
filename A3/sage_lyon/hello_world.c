#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int hello_world_init(void) {
	printk(KERN_ALERT "Hello\n");
	return 0;
}

static void hello_world_exit(void) {
	printk(KERN_ALERT "Goodbye...\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);
