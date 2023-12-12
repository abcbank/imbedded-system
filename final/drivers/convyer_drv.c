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

#define AA_PIN 7
#define AB_PIN 8

#define DRIVER_NAME "ConvyerController"
#define DRIVER_CLASS "ConvyerControlModule"

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    unsigned char tmp;

    to_copy= min(count, sizeof(tmp));

    tmp = gpio_get_value(AB_PIN) << 1 & gpio_get_value(AA_PIN);

    not_copied = copy_to_user(user_buffer, &tmp, to_copy);

    delta = to_copy - not_copied;

    return delta;
}

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    unsigned char value;

    to_copy = min(count, sizeof(value));

    not_copied = copy_from_user(&value, user_buffer, to_copy);

    switch(value){

        case 0:
            gpio_set_value(AA_PIN, 0);
            gpio_set_value(AB_PIN, 0);
        break;
        
        case 1:
            gpio_set_value(AA_PIN, 1);
            gpio_set_value(AB_PIN, 0);
        break;

        case 2:
            gpio_set_value(AA_PIN, 0);
            gpio_set_value(AB_PIN, 1);
        break;
        
        default:
            gpio_set_value(AA_PIN, 0);
            gpio_set_value(AB_PIN, 0);
        break;
    }


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

    if(gpio_request(AA_PIN, "rpi-gpio-18")){
        printk("Can not allocate GPIO 19\n");
        goto AddError;
    }

    if(gpio_direction_output(AA_PIN, 0)) {
        printk("Can not set GPIO 19 to output!\n");
        goto Gpio19Error;
    }

    if(gpio_request(AB_PIN, "rpi-gpio-20")){
        printk("Can not allocate GPIO 20\n");
        goto AddError;
    }

    if(gpio_direction_output(AB_PIN, 0)) {
        printk("Can not set GPIO 20 to output!\n");
        goto Gpio20Error;
    }


    return 0;

    Gpio19Error:
        gpio_free(AA_PIN);
    Gpio20Error:
        gpio_free(AB_PIN);
    AddError:
        device_destroy(my_class, my_device_nr);
    FileError:
        class_destroy(my_class);
    ClassError:
        unregister_chrdev_region(my_device_nr, 1);
    return -1;
}

static void __exit ModuleExit(void) {

    gpio_set_value(AA_PIN, 0);
    gpio_set_value(AB_PIN, 0);
    gpio_free(AA_PIN);
    gpio_free(AB_PIN);
    cdev_del(&my_device);
    device_destroy(my_class, my_device_nr);
    class_destroy(my_class);
    unregister_chrdev_region(my_device_nr, 1);
    printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);