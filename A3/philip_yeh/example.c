#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int hello_init(void)
{
    printk(KERN_ALERT "What is the difference between roast beef and pea soup?\n");
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT "You can roast beef, but you can't pea soup.\n");
}
module_init(hello_init);
module_exit(hello_exit);
