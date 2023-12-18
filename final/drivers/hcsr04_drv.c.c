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

#define DRIVER_NAME "HCSR04Controller"
#define DRIVER_CLASS "HCSR04ControlModule"

#define ECHO 17
#define TRIGGER 18

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    PinArray tmp;

    to_copy= min(count, sizeof(tmp));
    
    gpio_set_value(TRIGGER, 0);
    gpio_set_value(TRIGGER, 1);

    sleep()

    tmp.Pin_01 = gpio_get_value(PIN_01);
    tmp.Pin_02 = gpio_get_value(PIN_02);
    tmp.Pin_03 = gpio_get_value(PIN_03);
    tmp.Pin_04 = 0; //gpio_get_value(PIN_04);
    tmp.Pin_05 = 0; //gpio_get_value(PIN_05);
    tmp.Pin_06 = 0; //gpio_get_value(PIN_06);
    tmp.Pin_07 = 0; //gpio_get_value(PIN_07);
    tmp.Pin_08 = 0; //gpio_get_value(PIN_08);

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

    if(gpio_request(TRIGGER, "rpi-gpio-17")){
        printk("Can not allocate GPIO 17\n");
        goto AddError;
    }

    if(gpio_direction_input(TRIGGER)) {
        printk("Can not set GPIO 17 to input!\n");
        goto PIN01_ERROR;
    }

    if(gpio_request(ECHO, "rpi-gpio-18")){
        printk("Can not allocate GPIO 17\n");
        goto AddError;
    }

    if(gpio_direction_input(ECHO)) {
        printk("Can not set GPIO 17 to input!\n");
        goto PIN02_ERROR;
    }

    return 0;

    PIN01_ERROR:
        gpio_free(TRIGGER);
    PIN02_ERROR:
        gpio_free(ECHO);
    AddError:
    gpio_free(PIN_03);
        device_destroy(my_class, my_device_nr);
    FileError:
        class_destroy(my_class);
    ClassError:
        unregister_chrdev_region(my_device_nr, 1);
    return -1;
}

static void __exit ModuleExit(void) {
    gpio_free(TRIGGER);
    gpio_free(ECHO);
    // gpio_free(PIN_04);
    // gpio_free(PIN_05);
    // gpio_free(PIN_06);
    // gpio_free(PIN_07);
    // gpio_free(PIN_08);
    cdev_del(&my_device);
    device_destroy(my_class, my_device_nr);
    class_destroy(my_class);
    unregister_chrdev_region(my_device_nr, 1);
    printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);