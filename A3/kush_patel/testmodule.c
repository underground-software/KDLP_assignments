#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int testmodule_init(void)
{
    printk(KERN_ALERT "Whats the difference between a virus and Windows 11?\n");
    return 0;
}

static void testmodule_exit(void)
{
    printk(KERN_ALERT "Viruses rarely fail\n");
}

module_init(testmodule_init);
module_exit(testmodule_exit);