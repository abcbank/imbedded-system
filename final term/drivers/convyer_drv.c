#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marco Park/Linux");
MODULE_DESCRIPTION("This is Digital Minus");

static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define CONVYER_PIN 17

#define DRIVER_NAME "convyer"
#define DRIVER_CLASS "ConvyerModuleClass"

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    unsigned char tmp;

    to_copy= min(count, sizeof(tmp));

    tmp = gpio_get_value(CONVYER_PIN);

    not_copied = copy_to_user(user_buffer, &tmp, to_copy);

    /* Calculate data */
    delta = to_copy - not_copied;
    return delta;
}

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    unsigned char value;

    to_copy = min(count, sizeof(value));

    not_copied = copy_from_user(&value, user_buffer, to_copy);

    if(value){
        gpio_set_value(CONVYER_PIN, 1);
    }
    else{
        gpio_set_value(CONVYER_PIN, 0);
    }
    
    /* Calculate data */
    delta =  to_copy - not_copied;
    return delta;
}

static int driver_open(struct inode *device_file, struct file *instance) {
    return 0;
}

static int driver_close(struct inode *device_file, struct file *instance) {
    return 0;
}

static struct file_operations fops={
    .owner=THIS_MODULE,
    .open=driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write
};

static int __init ModuleInit(void) {
    printk("Hello, Kernel!\n");

    if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
        printk("Device Nr. could not be allocated!\n");
        return -1;
    }
    
    printk("read [_write - Device Nr. Major: Xd, Minor: Xd was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

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

    if(gpio_request(CONVYER_PIN, "rpi-gpio-17")){
        printk("Can not allocate GPIO 17\n");
        goto AddError;
    }

    if(gpio_direction_output(CONVYER_PIN, 0)) {
        printk("Can not set GPIO 17 to output!\n");
        goto Gpio17Error;
    }


    return 0;

    Gpio17Error:
        gpio_free(CONVYER_PIN);
    FileError:
        class_destroy(my_class);
    ClassError:
        unregister_chrdev_region(my_device_nr, 1);
    return -1;
}

static void __exit ModuleExit(void) {
    gpio_free(CONVYER_PIN);
    cdev_del(&my_device);
    device_destroy(my_class, my_device_nr);
    class_destroy(my_class);
    unregister_chrdev_region(my_device_nr, 1);
    printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);