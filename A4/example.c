#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");

static dev_t example_device;
static struct cdev *example_cdev;

static uintptr_t counter = 0;

static int example_open(struct inode *inode, struct file *file)
{
	file->private_data = (void *)counter;
	printk(KERN_INFO "Character Driver Module %lu: open (number %lu)\n", counter++, (uintptr_t)file->private_data);
	return 0;
}

static int example_release(struct inode *inode, struct file *file)
{

	printk(KERN_INFO "Character Driver Module %lu: release (number %lu)\n", counter++, (uintptr_t)file->private_data);
	return 0;
}

static ssize_t example_read(struct file *file, char __user *data, size_t count, loff_t *f_pos)
{
	printk(KERN_INFO "Character Driver Module %lu: read (number %lu)\n", counter++, (uintptr_t)file->private_data);
	return 0;
}

static ssize_t example_write(struct file *file, const char __user *data, size_t count, loff_t *f_pos)
{
	printk(KERN_INFO "Character Driver Module %lu: write (number %lu)\n", counter++, (uintptr_t)file->private_data);
	return count;
}

static const struct file_operations example_fops = {
	.owner = THIS_MODULE,
	.open = example_open,
	.release = example_release,
	.read = example_read,
	.write = example_write,
};

static int example_init(void)
{
	int ret;
	printk(KERN_INFO "Character Driver Module %lu: init\n", counter++);
	ret = alloc_chrdev_region(&example_device, 0, 1, "example");
	if (ret < 0) {
		printk(KERN_ERR "Character Driver Module: unnable to allocate region %i\n", ret);
		goto fail_region;
	}

	example_cdev = cdev_alloc();
	if (!example_cdev) {
		ret = -ENOMEM;
		printk(KERN_ERR "Character Driver Module: unable to allocate char dev %i\n", ret);
		goto fail_cdev;
	}

	cdev_init(example_cdev, &example_fops);

	ret = cdev_add(example_cdev, example_device, 1);
	if (ret < 0) {
		printk(KERN_ERR "Character Driver Module: unable to add char dev %i\n", ret);
		goto fail_add;
	}

	printk(KERN_INFO "Character Driver Module: got major # %i got minor # %i\n", MAJOR(example_device), MINOR(example_device));

	return 0;

fail_add:
	cdev_del(example_cdev);
fail_cdev:
	unregister_chrdev_region(example_device, 1);
fail_region:
	return ret;
}

static void example_exit(void)
{
	printk(KERN_INFO "Character Driver Module %lu: exit\n", counter++);
	cdev_del(example_cdev);
	unregister_chrdev_region(example_device, 1);
}
module_init(example_init);
module_exit(example_exit);
