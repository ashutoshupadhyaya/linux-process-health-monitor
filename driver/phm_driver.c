#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "phmctl"
#define BUFFER_SIZE 256

static dev_t phm_dev;
static struct cdev phm_cdev;
static struct class *phm_class;
static struct device *phm_device;

static char device_buffer[BUFFER_SIZE];
static size_t device_buffer_size;
static DEFINE_MUTEX(phm_mutex);

static int phm_open(struct inode *inode, struct file *file)
{
    pr_info("PHM: device opened.\n");
    return 0;
}

static int phm_release(struct inode *inode, struct file *file)
{
    pr_info("PHM: device released.\n");
    return 0;
}

static ssize_t phm_write(
    struct file *file,
    const char __user *buffer,
    size_t count,
    loff_t *offset)
{
    size_t bytesToCopy;

    bytesToCopy = count;

    if (bytesToCopy >= BUFFER_SIZE)
    {
        bytesToCopy = BUFFER_SIZE - 1;
    }

    mutex_lock(&phm_mutex);

    if (copy_from_user(
            device_buffer,
            buffer,
            bytesToCopy))
    {
        mutex_unlock(&phm_mutex);
        return -EFAULT;
    }

    device_buffer[bytesToCopy] = '\0';
    device_buffer_size = bytesToCopy;

    mutex_unlock(&phm_mutex);

    pr_info(
        "PHM: received command: %s\n",
        device_buffer);

    return bytesToCopy;
}

static ssize_t phm_read(
    struct file *file,
    char __user *buffer,
    size_t count,
    loff_t *offset)
{
    size_t bytesToCopy;

    mutex_lock(&phm_mutex);

    if (*offset >= device_buffer_size)
    {
        mutex_unlock(&phm_mutex);
        return 0;
    }

    bytesToCopy =
        device_buffer_size - *offset;

    if (bytesToCopy > count)
    {
        bytesToCopy = count;
    }

    if (copy_to_user(
            buffer,
            device_buffer + *offset,
            bytesToCopy))
    {
        mutex_unlock(&phm_mutex);
        return -EFAULT;
    }

    *offset += bytesToCopy;

    mutex_unlock(&phm_mutex);

    return bytesToCopy;
}

static const struct file_operations phm_fops =
{
    .owner = THIS_MODULE,
    .open = phm_open,
    .release = phm_release,
    .read = phm_read,
    .write = phm_write
};

static int __init phm_init(void)
{
    int ret;

    ret = alloc_chrdev_region(
        &phm_dev,
        0,
        1,
        DEVICE_NAME);

    if (ret < 0)
    {
        pr_err(
            "PHM: failed to allocate device number.\n");
        return ret;
    }

    cdev_init(&phm_cdev, &phm_fops);

    phm_cdev.owner = THIS_MODULE;

    ret = cdev_add(
        &phm_cdev,
        phm_dev,
        1);

    if (ret < 0)
    {
        unregister_chrdev_region(
            phm_dev,
            1);

        pr_err(
            "PHM: failed to add character device.\n");

        return ret;
    }

    phm_class = class_create(DEVICE_NAME);

    if (IS_ERR(phm_class))
    {
        cdev_del(&phm_cdev);
        unregister_chrdev_region(
            phm_dev,
            1);

        pr_err(
            "PHM: failed to create device class.\n");

        return PTR_ERR(phm_class);
    }

    phm_device = device_create(
        phm_class,
        NULL,
        phm_dev,
        NULL,
        DEVICE_NAME);

    if (IS_ERR(phm_device))
    {
        class_destroy(phm_class);
        cdev_del(&phm_cdev);
        unregister_chrdev_region(
            phm_dev,
            1);

        pr_err(
            "PHM: failed to create device.\n");

        return PTR_ERR(phm_device);
    }

    mutex_lock(&phm_mutex);

    device_buffer_size =
        scnprintf(
            device_buffer,
            BUFFER_SIZE,
            "PHM driver ready");

    mutex_unlock(&phm_mutex);

    pr_info(
        "PHM: driver initialized.\n");

    return 0;
}

static void __exit phm_exit(void)
{
    device_destroy(
        phm_class,
        phm_dev);

    class_destroy(phm_class);

    cdev_del(&phm_cdev);

    unregister_chrdev_region(
        phm_dev,
        1);

    pr_info(
        "PHM: driver unloaded.\n");
}

module_init(phm_init);
module_exit(phm_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(
    "Linux Process Health Monitor character device driver");
MODULE_VERSION("1.0");
