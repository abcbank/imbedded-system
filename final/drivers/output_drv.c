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

#define DRIVER_NAME "OutputController"
#define DRIVER_CLASS "OutputControlModule"

#define PIN_01 16
#define PIN_02 20
#define PIN_03 21
#define PIN_04 12
#define PIN_05 13
#define PIN_06 16
#define PIN_07 17
#define PIN_08 18

typedef struct _PinArray{
    unsigned char Pin_01: 1;
    unsigned char Pin_02: 1;
    unsigned char Pin_03: 1;
    unsigned char Pin_04: 1;
    unsigned char Pin_05: 1;
    unsigned char Pin_06: 1;
    unsigned char Pin_07: 1;
    unsigned char Pin_08: 1;
}PinArray;

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    PinArray tmp;

    to_copy= min(count, sizeof(tmp));
    
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

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs){
    int to_copy, not_copied, delta;
    PinArray value;

    to_copy = min(count, sizeof(value));

    not_copied = copy_from_user(&value, user_buffer, to_copy);

    if(value.Pin_01){
        gpio_set_value(PIN_01, 1);
    }
    else{
        gpio_set_value(PIN_01, 0);
    }
    if(value.Pin_02){
        gpio_set_value(PIN_02, 1);
    }
    else{
        gpio_set_value(PIN_02, 0);
    }
    if(value.Pin_03){
        gpio_set_value(PIN_03, 1);
    }
    else{
        gpio_set_value(PIN_03, 0);
    }
    // if(value.Pin_04){
    //     gpio_set_value(PIN_04, 1);
    // }
    // else{
    //     gpio_set_value(PIN_04, 0);
    // }
    // if(value.Pin_05){
    //     gpio_set_value(PIN_05, 1);
    // }
    // else{
    //     gpio_set_value(PIN_05, 0);
    // }
    // if(value.Pin_06){
    //     gpio_set_value(PIN_06, 1);
    // }
    // else{
    //     gpio_set_value(PIN_06, 0);
    // }
    // if(value.Pin_07){
    //     gpio_set_value(PIN_07, 1);
    // }
    // else{
    //     gpio_set_value(PIN_07, 0);
    // }
    // if(value.Pin_08){
    //     gpio_set_value(PIN_08, 1);
    // }
    // else{
    //     gpio_set_value(PIN_08, 0);
    // }

    delta =  to_copy - not_copied;
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

    if(gpio_request(PIN_01, "rpi-gpio-10")){
        printk("Can not allocate GPIO 17\n");
        goto AddError;
    }

    if(gpio_direction_output(PIN_01, 0)) {
        printk("Can not set GPIO 4 to output!\n");
        goto PIN01_ERROR;
    }

    if(gpio_request(PIN_02, "rpi-gpio-11")){
        printk("Can not allocate GPIO 17\n");
        goto AddError;
    }

    if(gpio_direction_output(PIN_02, 0)) {
        printk("Can not set GPIO 4 to output!\n");
        goto PIN02_ERROR;
    }

    if(gpio_request(PIN_03, "rpi-gpio-12")){
        printk("Can not allocate GPIO 17\n");
        goto AddError;
    }

    if(gpio_direction_output(PIN_03, 0)) {
        printk("Can not set GPIO 4 to output!\n");
        goto PIN03_ERROR;
    }

    // if(gpio_request(PIN_04, "rpi-gpio-13")){
    //     printk("Can not allocate GPIO 17\n");
    //     goto AddError;
    // }

    // if(gpio_direction_output(PIN_04, 0)) {
    //     printk("Can not set GPIO 4 to output!\n");
    //     goto PIN04_ERROR;
    // }

    // if(gpio_request(PIN_05, "rpi-gpio-14")){
    //     printk("Can not allocate GPIO 17\n");
    //     goto AddError;
    // }

    // if(gpio_direction_output(PIN_05, 0)) {
    //     printk("Can not set GPIO 4 to output!\n");
    //     goto PIN05_ERROR;
    // }

    // if(gpio_request(PIN_06, "rpi-gpio-15")){
    //     printk("Can not allocate GPIO 17\n");
    //     goto AddError;
    // }

    // if(gpio_direction_output(PIN_06, 0)) {
    //     printk("Can not set GPIO 4 to output!\n");
    //     goto PIN06_ERROR;
    // }

    // if(gpio_request(PIN_07, "rpi-gpio-16")){
    //     printk("Can not allocate GPIO 17\n");
    //     goto AddError;
    // }
    
    // if(gpio_direction_output(PIN_07, 0)) {
    //     printk("Can not set GPIO 4 to output!\n");
    //     goto PIN07_ERROR;
    // }

    // if(gpio_request(PIN_08, "rpi-gpio-17")){
    //     printk("Can not allocate GPIO 17\n");
    //     goto AddError;
    // }
    
    // if(gpio_direction_output(PIN_08, 0)) {
    //     printk("Can not set GPIO 4 to output!\n");
    //     goto PIN08_ERROR;
    // }

    return 0;

    PIN01_ERROR:
        gpio_free(PIN_01);
    PIN02_ERROR:
        gpio_free(PIN_02);
    PIN03_ERROR:
        gpio_free(PIN_03);
    // PIN04_ERROR:
    //     gpio_free(PIN_04);
    // PIN05_ERROR:
    //     gpio_free(PIN_05);
    // PIN06_ERROR:
    //     gpio_free(PIN_06);
    // PIN07_ERROR:
    //     gpio_free(PIN_07);
    // PIN08_ERROR:
    //     gpio_free(PIN_08);
    AddError:
        device_destroy(my_class, my_device_nr);
    FileError:
        class_destroy(my_class);
    ClassError:
        unregister_chrdev_region(my_device_nr, 1);
    return -1;
}

static void __exit ModuleExit(void) {
    gpio_free(PIN_01);
    gpio_free(PIN_02);
    gpio_free(PIN_03);
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