#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_char"
#define BUF_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple character driver");

static int major_number;
static char device_buffer[BUF_SIZE];

static int simple_char_open(struct inode *inode, struct file *filp) {
    return 0; // Success
}

static int simple_char_release(struct inode *inode, struct file *filp) {
    return 0; // Success
}

static ssize_t simple_char_read(struct file *filp, char __user *user_buffer, size_t count, loff_t *offset) {
    int bytes_to_copy;
    if (*offset >= BUF_SIZE)
        return 0; // EOF

    bytes_to_copy = min(count, (size_t)(BUF_SIZE - *offset));
    if (copy_to_user(user_buffer, &device_buffer[*offset], bytes_to_copy) != 0)
        return -EFAULT;

    *offset += bytes_to_copy;
    return bytes_to_copy;
}

static ssize_t simple_char_write(struct file *filp, const char __user *user_buffer, size_t count, loff_t *offset) {
    int bytes_to_copy;
    if (*offset >= BUF_SIZE)
        return -ENOSPC; // No space left on device

    bytes_to_copy = min(count, (size_t)(BUF_SIZE - *offset));
    if (copy_from_user(&device_buffer[*offset], user_buffer, bytes_to_copy) != 0)
        return -EFAULT;

    *offset += bytes_to_copy;
    return bytes_to_copy;
}

static struct file_operations simple_char_fops = {
    .open = simple_char_open,
    .read = simple_char_read,
    .write = simple_char_write,
    .release = simple_char_release,
};

static int __init simple_char_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &simple_char_fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }

    printk(KERN_INFO "Registered character driver with major number %d\n", major_number);
    return 0;
}

static void __exit simple_char_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Unregistered character driver\n");
}

module_init(simple_char_init);
module_exit(simple_char_exit);

