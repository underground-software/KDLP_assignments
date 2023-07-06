// SPDX-License-Identifier: GPL

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/string.h>

#include "asciiart.h"
#include "dotmatrix.h"

#define DEVICE_NAME "asciiart"
#define DEVICE_CLASS "asciiart_class"
#define NUM_DEVICES 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sepehr Madani <ssepehrmadani@gmail.com>");

static dev_t device_number;
static struct cdev *cdev;
static struct class *asciiart_class; // sysfs class structure
static struct device *asciiart_device;

// device data holder, this structure may be extended to hold additional data
struct asciiart_device_data {
	struct cdev cdev;
};

struct asciiart_private {
	struct mutex mtx;
	char *message;
	size_t size;
};

static int asciiart_open(struct inode *inode, struct file *file)
{
	struct device *device =
		class_find_device_by_devt(asciiart_class, inode->i_rdev);
	struct asciiart_private *priv = dev_get_drvdata(device);

	file->private_data = priv;
	return 0;
}

static int asciiart_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t asciiart_read(struct file *file, char __user *buf, size_t size,
			     loff_t *off)
{
	struct asciiart_private *priv = file->private_data;

	if (mutex_lock_interruptible(&priv->mtx))
		return -ERESTARTSYS;

	if (*off >= priv->size) {
		mutex_unlock(&priv->mtx);
		return 0; // EOF
	}

	size = min(size, (size_t)(priv->size - *off));

	if (copy_to_user(buf, priv->message + *off, size)) {
		mutex_unlock(&priv->mtx);
		return -EFAULT; // bad address?
	}

	*off += size;
	mutex_unlock(&priv->mtx);
	return size;
}

void transform(char *output, char *input)
{
	output[0] = '\0'; // initialize the output
	for (size_t i = 0; i < CHAR_HEIGHT; i++) { // for each line
		char line[LINE_LIMIT] = { '\0' }; // buffer to build the line
			// for each character in input
		for (size_t j = 0; j < strlen(input); j++) {
			// if the character is not in the alphabet
			if (input[j] < 32 || input[j] > 127)
				continue; // skip the character
			char *ascii_block = alphabet[input[j] - 32];
			int block_width = strlen(ascii_block) / CHAR_HEIGHT;

			// if the line is not full
			if (strlen(line) + block_width + 1 < LINE_LIMIT) {
				// append the character block to the line
				strncat(line, ascii_block + i * block_width,
					block_width);
			} else { // if the line is full
				break; // break the loop
			}
		}
		strcat(output, line); // append the line to the output
		strcat(output, "\n"); // write newline at the end of each line
	}
}

static ssize_t asciiart_write(struct file *file, const char __user *buf,
			      size_t size, loff_t *off)
{
	struct asciiart_private *priv = file->private_data;
	char *temp_buffer = kmalloc(size, GFP_KERNEL);

	if (!temp_buffer)
		return -ENOMEM;

	if (copy_from_user(temp_buffer, buf, size)) {
		kfree(temp_buffer);
		return -EFAULT;
	}

	mutex_lock(&priv->mtx);

	/* Delete the old message */
	kfree(priv->message);
	priv->message = NULL;
	priv->size = 0;

	/* Allocate space for the new message (up to 160 characters as ASCII art) */
	priv->message = kmalloc((LINE_LIMIT * CHAR_HEIGHT + 1) * sizeof(char),
				GFP_KERNEL);
	if (!priv->message) {
		kfree(temp_buffer);
		return -ENOMEM;
	}
	/* Clear the memory */
	memset(priv->message, 0, (LINE_LIMIT * CHAR_HEIGHT + 1) * sizeof(char));

	/* Transform the input string into ASCII art */
	transform(priv->message, temp_buffer);
	priv->size = strlen(priv->message);

	mutex_unlock(&priv->mtx);
	kfree(temp_buffer);

	return size;
}

static loff_t asciiart_llseek(struct file *file, loff_t offset, int whence)
{
	loff_t ret = -EINVAL, pos;
	struct asciiart_private *priv = file->private_data;

	if (mutex_lock_interruptible(&(priv->mtx)))
		return -ERESTARTSYS;
	switch (whence) {
	case SEEK_SET:
		pos = offset;
		break;
	case SEEK_CUR:
		pos = file->f_pos + offset;
		break;
	case SEEK_END:
		pos = priv->size + offset;
		break;
	default:
		goto out;
	}
	if (pos < 0 || priv->size < pos)
		goto out;
	file->f_pos = pos;
	ret = pos;
out:
	mutex_unlock(&(priv->mtx));
	return ret;
}

static long asciiart_ioctl(struct file *file, unsigned int cmd,
			   unsigned long arg)
{
	struct asciiart_private *priv = file->private_data;

	if (cmd != IOC_ASCIIART_RESET && cmd != IOC_ASCIIART_GET_LENGTH)
		return -EINVAL;
	if (mutex_lock_interruptible(&(priv->mtx)))
		return -ERESTARTSYS;
	if (cmd == IOC_ASCIIART_GET_LENGTH) {
		if (copy_to_user((size_t __user *)arg, &(priv->size),
				 sizeof(size_t))) {
			mutex_unlock(&(priv->mtx));
			return -EFAULT;
		}
	} else {
		kfree(priv->message);
		priv->message = NULL;
		priv->size = 0;
	}

	mutex_unlock(&(priv->mtx));
	return 0;
}

static const struct file_operations asciiart_fops = {
	.owner = THIS_MODULE,
	.open = asciiart_open,
	.release = asciiart_release,
	.read = asciiart_read,
	.write = asciiart_write,
	.unlocked_ioctl = asciiart_ioctl,
	.llseek = asciiart_llseek,
};

static char *asciiart_devnode(const struct device *dev, umode_t *mode)
{
	if (mode)
		*mode = 0666;
	return NULL;
}

int __init asciiart_init(void)
{
	struct asciiart_private *asciiart_priv;
	int ret;

	ret = alloc_chrdev_region(&device_number, 0, NUM_DEVICES, DEVICE_NAME);

	if (ret) {
		pr_err("Unable to allocate chrdev region: %i\n", ret);
		goto err_alloc;
	}

	cdev = cdev_alloc();
	if (!cdev) {
		ret = -ENOMEM;
		pr_err("Unable to allocate cdev: %i\n", ret);
		goto err_cdev_alloc;
	}

	cdev_init(cdev, &asciiart_fops);
	ret = cdev_add(cdev, device_number, NUM_DEVICES);
	if (ret) {
		pr_err("Unable to add cdev %i\n", ret);
		goto err_cdev_add;
	}
	asciiart_class = class_create(THIS_MODULE, DEVICE_NAME);
	if (IS_ERR(asciiart_class)) {
		ret = PTR_ERR(asciiart_class);
		pr_err("Unable to create class for asciiart devices: %i\n",
		       ret);
		goto err_cdev_add;
	}
	asciiart_class->devnode = asciiart_devnode;
	asciiart_priv = kzalloc(sizeof(*asciiart_priv), GFP_KERNEL);
	if (!asciiart_priv) {
		ret = -ENOMEM;
		pr_err("Unable to create buffer for asciiart device: %i\n",
		       ret);
		goto err_class_create;
	}
	mutex_init(&asciiart_priv->mtx);
	asciiart_device = device_create(asciiart_class, NULL, device_number,
					asciiart_priv, DEVICE_NAME);
	if (IS_ERR(asciiart_device)) {
		ret = PTR_ERR(asciiart_device);
		pr_err("Unable to create device for asciiart device: %i\n",
		       ret);
		goto err_ASCIIART_priv_allocate;
	}
	return 0;

err_ASCIIART_priv_allocate:
	mutex_destroy(&asciiart_priv->mtx);
	kfree(asciiart_priv);
err_class_create:
	class_destroy(asciiart_class);
err_cdev_add:
	cdev_del(cdev);
err_cdev_alloc:
	unregister_chrdev_region(device_number, NUM_DEVICES);
err_alloc:
	return ret;
}

void __exit asciiart_exit(void)
{
	struct device *device =
		class_find_device_by_devt(asciiart_class, device_number);
	struct asciiart_private *priv = dev_get_drvdata(device);

	mutex_destroy(&priv->mtx);
	kfree(priv);
	device_destroy(asciiart_class, device_number);
	class_unregister(asciiart_class);
	class_destroy(asciiart_class);
	cdev_del(cdev);
	unregister_chrdev_region(device_number, NUM_DEVICES);
}

module_init(asciiart_init);
module_exit(asciiart_exit);
