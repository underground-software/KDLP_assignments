#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int A3_init(void)
{
    printk(KERN_ALERT "What do Karl Marx and the founder of linux have in common?\n");
    printk(KERN_ALERT "They both hate classes\n");
    return 0;
}

static void A3_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
}
module_init(A3_init);
module_exit(A3_exit);