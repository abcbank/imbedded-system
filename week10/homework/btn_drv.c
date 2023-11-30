#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple gpio driver for setting a LED and reading a button");

static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "SegmentButtons"
#define DRIVER_CLASS "MyModuleClass"

#define BTN01 26
#define BTN02 19

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    unsigned char tmp = 0xf0;

    to_copy= min(count, sizeof(tmp));
    
    tmp |=  (gpio_get_value(BTN01) << 0) |
            (gpio_get_value(BTN02) << 1);

    not_copied = copy_to_user(user_buffer, &tmp, to_copy);

    /* Calculate data */
    delta = to_copy - not_copied;
    return delta;
}

static int driver_open(struct inode *device_file, struct file *instance) {
    printk("led_button - open was called!\n");
    return 0;
}

static int driver_close(struct inode *device_file, struct file *instance) {
    printk("led_button close was called!\n");
    return 0;
}

static struct file_operations fops={
    .owner=THIS_MODULE,
    .open=driver_open,
    .release = driver_close,
    .read = driver_read
};

static int __init ModuleInit(void) {
    printk("Hello, Kernel!\n");

    if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
        printk("Device Nr. could not be allocated!\n");
        return -1;
    }
    
    printk("Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

    if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
        printk("Device class can not e created!\n");
        goto ClassError;
    }
    
    /* create device file */
    if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
        printk("Can not create device file!\n");
        goto FileError;
    }

    cdev_init(&my_device, &fops);

    if(cdev_add(&my_device, my_device_nr, 1) == -1) {
        printk("Registering of device to kernel failed!\n");
        goto AddError;
    }

    if(gpio_request(BTN01, "rpi-gpio-26")){
        printk("Can not allocate BTN 01\n");
        goto AddError;
    }

/* Set GPIO 17 direction */
    if(gpio_direction_input(BTN01)) {
        printk("Can not set BTN 01 to input!\n");
        goto BTN01Error;
    }

    if(gpio_request(BTN02, "rpi-gpio-19")){
        printk("Can not allocate BTN 02\n");
        goto AddError;
    }

/* Set GPIO 17 direction */
    if(gpio_direction_input(BTN02)) {
        printk("Can not set BTN 02 to input!\n");
        goto BTN02Error;
    }

    return 0;

    BTN01Error:
        gpio_free(BTN01);
    BTN02Error:
        gpio_free(BTN02);
    AddError:
        device_destroy(my_class, my_device_nr);
    FileError:
        class_destroy(my_class);
    ClassError:
        unregister_chrdev_region(my_device_nr, 1);
    return -1;
}

static void __exit ModuleExit(void) {
    gpio_free(BTN01);
    gpio_free(BTN02);
    cdev_del(&my_device);
    device_destroy(my_class, my_device_nr);
    class_destroy(my_class);
    unregister_chrdev_region(my_device_nr, 1);
    printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);