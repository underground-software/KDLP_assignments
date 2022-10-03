#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int hello_init(void) {
    printk(KERN_ALERT "What did the linux developer say to the corn farmer?\n");
    return 0;
}

static void hello_exit(void) {
    printk(KERN_ALERT "I'd show you a real kernel, but I couldn't exit vim...\n");
}
module_init(hello_init);
module_exit(hello_exit);
