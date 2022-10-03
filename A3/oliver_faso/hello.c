#include <linux/init.h>
#include <linux/module.h>
#include <linux/random.h>

#include "license.h"

static void print_cool(const int *msg)
{
	int n;

	while ((n = *msg++) != 0) {
		if (n < 0)
			for (int s = 0; s < -n; s++)
				printk(KERN_NOTICE KERN_CONT " ");
		else if (n == 69)
			printk(KERN_NOTICE KERN_CONT "\n");
		else
			for (int i = 0; i < n; i++) {
				unsigned char c = get_random_u32() % 26 + 65;
				printk(KERN_NOTICE KERN_CONT "%c", c);
			}
	}
}

const int msg[] = { 1,	-3, 1,	-2, 3,	-2, 1,	-5, 1,	-6, 3,	69, 1,	-3,
		    1,	-1, 1,	-3, 1,	-1, 1,	-5, 1,	-5, 1,	-3, 1,	69,
		    5,	-1, 4,	-2, 1,	-5, 1,	-5, 1,	-3, 1,	69, 1,	-3,
		    1,	-1, 1,	-5, 1,	-5, 1,	-5, 1,	-3, 1,	69, 1,	-3,
		    1,	-2, 3,	-2, 5,	-1, 5,	-2, 3,	69, -1, 69, 1,	-3,
		    1,	-2, 3,	-3, 3,	-2, 1,	-5, 4,	69, 1,	-3, 1,	-1,
		    1,	-3, 1,	-1, 1,	-3, 1,	-1, 1,	-5, 1,	-3, 1,	69,
		    1,	-1, 1,	-1, 1,	-1, 1,	-3, 1,	-1, 3,	-3, 1,	-5,
		    1,	-3, 1,	69, 1,	-1, 1,	-1, 1,	-1, 1,	-3, 1,	-1,
		    1,	-2, 1,	-2, 1,	-5, 1,	-3, 1,	69, -1, 1,  -1, 1,
		    -3, 3,  -2, 1,  -3, 1,  -1, 5,  -1, 4,  69, -1, 69, 0 };

static int hello_init(void)
{
	printk(KERN_NOTICE "Module Hello is licensed by: %s\n", LICENSE);
	print_cool(msg);
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_NOTICE "Goodbye, Hello\n");
}

MODULE_LICENSE(LICENSE);
module_init(hello_init);
module_exit(hello_exit);
