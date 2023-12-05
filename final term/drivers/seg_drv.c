#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marco Park/Linux");
MODULE_DESCRIPTION("This is Digital Minus");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "svnSegment"
#define DRIVER_CLASS "SevenSegmentModule"
/* @brief Write data to buffer */

#define SEGOFF 0
#define SEGON  1

#define OFF 1
#define ON  0

#define A  21
#define B  20
#define C  16
#define D  12
#define E  7
#define F  8
#define G  25
#define DP 24

#define SEG01 2
#define SEG02 3
#define SEG03 4
#define SEG04 17

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) { 
    int to_copy, not_copied, delta;
    unsigned short value = 0;
    /* Get amount of data to copy */
    to_copy = min(count, sizeof(value));
    /* Copy data to user */
    not_copied = copy_from_user(&value, user_buffer, to_copy);

    gpio_set_value(A, OFF);
    gpio_set_value(B, OFF);
    gpio_set_value(C, OFF);
    gpio_set_value(D, OFF);
    gpio_set_value(E, OFF);
    gpio_set_value(F, OFF);
    gpio_set_value(G, OFF);
    gpio_set_value(DP, OFF);

    int segValue = (value & 0xf0);
    int gpioValue = value & 0x0f;

    if(segValue & 0x10)
        gpio_set_value(SEG01, SEGON);
    else
        gpio_set_value(SEG01, SEGOFF);
    
    if(segValue & 0x20)
        gpio_set_value(SEG02, SEGON);
    else
        gpio_set_value(SEG02, SEGOFF);
    
    if(segValue & 0x40)
        gpio_set_value(SEG03, SEGON);
    else
        gpio_set_value(SEG03, SEGOFF);
    
    if(segValue & 0x80)
        gpio_set_value(SEG04, SEGON);
    else
        gpio_set_value(SEG04, SEGOFF);

    switch(gpioValue){
        case 0:
            gpio_set_value(A, ON);
            gpio_set_value(B, ON);
            gpio_set_value(C, ON);
            gpio_set_value(D, ON);
            gpio_set_value(E, ON);
            gpio_set_value(F, ON);
            gpio_set_value(G, OFF);
            gpio_set_value(DP, OFF);
            break;
        case 1:
            gpio_set_value(A, OFF);
            gpio_set_value(B, ON);
            gpio_set_value(C, ON);
            gpio_set_value(D, OFF);
            gpio_set_value(E, OFF);
            gpio_set_value(F, OFF);
            gpio_set_value(G, OFF);
            gpio_set_value(DP, OFF);
            break;
        case 2:
            gpio_set_value(A, ON);
            gpio_set_value(B, ON);
            gpio_set_value(C, OFF);
            gpio_set_value(D, ON);
            gpio_set_value(E, ON);
            gpio_set_value(F, OFF);
            gpio_set_value(G, ON);
            gpio_set_value(DP, OFF);
            break;
        case 3:
            gpio_set_value(A, ON);
            gpio_set_value(B, ON);
            gpio_set_value(C, ON);
            gpio_set_value(D, ON);
            gpio_set_value(E, OFF);
            gpio_set_value(F, OFF);
            gpio_set_value(G, ON);
            gpio_set_value(DP, OFF);
            break;
        case 4:
            gpio_set_value(A, OFF);
            gpio_set_value(B, ON);
            gpio_set_value(C, ON);
            gpio_set_value(D, OFF);
            gpio_set_value(E, OFF);
            gpio_set_value(F, ON);
            gpio_set_value(G, ON);
            gpio_set_value(DP, OFF);
            break;
        case 5:
            gpio_set_value(A, ON);
            gpio_set_value(B, OFF);
            gpio_set_value(C, ON);
            gpio_set_value(D, ON);
            gpio_set_value(E, OFF);
            gpio_set_value(F, ON);
            gpio_set_value(G, ON);
            gpio_set_value(DP, OFF);
            break;
        case 6:
            gpio_set_value(A, ON);
            gpio_set_value(B, OFF);
            gpio_set_value(C, ON);
            gpio_set_value(D, ON);
            gpio_set_value(E, ON);
            gpio_set_value(F, ON);
            gpio_set_value(G, ON);
            gpio_set_value(DP, OFF);
            break;
        case 7:
            gpio_set_value(A, ON);
            gpio_set_value(B, ON);
            gpio_set_value(C, ON);
            gpio_set_value(D, OFF);
            gpio_set_value(E, OFF);
            gpio_set_value(F, ON);
            gpio_set_value(G, OFF);
            gpio_set_value(DP, OFF);
            break;
        case 8:
            gpio_set_value(A, ON);
            gpio_set_value(B, ON);
            gpio_set_value(C, ON);
            gpio_set_value(D, ON);
            gpio_set_value(E, ON);
            gpio_set_value(F, ON);
            gpio_set_value(G, ON);
            gpio_set_value(DP, OFF);
            break;
        case 9:
            gpio_set_value(A, ON);
            gpio_set_value(B, ON);
            gpio_set_value(C, ON);
            gpio_set_value(D, ON);
            gpio_set_value(E, OFF);
            gpio_set_value(F, ON);
            gpio_set_value(G, ON);
            gpio_set_value(DP, OFF);
            break;
        default:
            gpio_set_value(A, OFF);
            gpio_set_value(B, OFF);
            gpio_set_value(C, OFF);
            gpio_set_value(D, OFF);
            gpio_set_value(E, OFF);
            gpio_set_value(F, OFF);
            gpio_set_value(G, OFF);
            gpio_set_value(DP, OFF);
            break;
    }
    delta = to_copy - not_copied; 
    return delta;
}
/*@brief This function is called, when the device file is opened */
static int driver_open(struct inode *device_file, struct file *instance) { 
    printk("segment open was called!\n"); 
    return 0;
}
/*@brief This function is called, when the device file is opened*/
static int driver_close(struct inode *device_file, struct file *instance) { 
    printk("segment close was called!\n"); 
    return 0;
}
static struct file_operations fops = {
.owner = THIS_MODULE,
.open = driver_open,
.release = driver_close,
//.read = driver_read,
.write = driver_write
};

/*@brief This function is called, when the module is loaded into the kernel*/
static int __init ModuleInit(void) { 
    printk("Hello, Kernel!\n");
    /* Allocate a device nr */
    if(alloc_chrdev_region (&my_device_nr, 0, 1, DRIVER_NAME) < 0) { 
        printk("Device Nr. could not be allocated!\n"); return -1;
    }
    printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);
    /* Create device class */
    if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) { 
        printk("Device class can not e created!\n"); goto ClassError;
    }
    /* create device file */
    if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) { 
        printk("Can not create device file!\n"); goto FileError;
    }
    /* Initialize device file */ 
    cdev_init(&my_device, &fops);
    /* Regisering device to kernel */
    if(cdev_add(&my_device, my_device_nr, 1) == -1) { 
        printk("Registering of device to kernel failed!\n"); goto AddError;
    }
    /* Set GPIO 2 direction */
    if (gpio_direction_output(2, 0)) {
        printk("Can not set GPIO 2 to output!\n"); goto Gpio2Error;
    }
    if(gpio_request(3, "rpi-gpio-3")) { 
        printk("Can not allocate GPIO 3\n"); goto AddError;
    }
    /* Set GPIO 3 direction */ if (gpio_direction_output(3, 0)) { 
        printk("Can not set GPIO 3 to output!\n"); goto Gpio3Error;
    }
    /* GPIO 4 init */ if(gpio_request(4, "rpi-gpio-4")) { 
        printk("Can not allocate GPIO 4\n"); goto AddError;
    }
    /* Set GPIO 4 direction */ if(gpio_direction_output(4, 0)) {
        printk("Can not set GPIO 4 to output!\n"); goto Gpio4Error;
    }
    /* GPIO 17 init */ if (gpio_request(17, "rpi-gpio-17")) { 
        printk("Can not allocate GPIO 17\n"); goto AddError;
    }
    /* Set GPIO 17 direction */
    if(gpio_direction_output(17, 0)) { 
        printk("Can not set GPIO 17 to output!\n"); goto Gpio17Error;
    }
    /* Set A~DP segments GPIO */
    /* GPIO 21 init */
    if(gpio_request(21, "rpi-gpio-21")) { 
        printk("Can not allocate GPIO 21\n"); goto AddError;
    }
    /* Set GPIO 21 direction */
    if (gpio_direction_output(21, 0)) { 
        printk("Can not set GPIO 21 to output!\n"); goto Gpio21Error;
    }
    /* GPIO 20 init */
    if (gpio_request(20, "rpi-gpio-20")) { 
        printk("Can not allocate GPIO_20\n"); goto AddError;
    }
    /* Set GPIO 20 direction */
    if (gpio_direction_output (20, 0)) { 
        printk("Can not set GPIO 20 to output!\n"); goto Gpio20Error;
    }
    /* GPIO 16 init */
    if(gpio_request(16, "rpi-gpio-16")) { 
        printk("Can not allocate GPIO 16\n"); goto AddError;
    }
    /* Set GPIO 16 direction */
    if(gpio_direction_output(16, 0)) { 
        printk("Can not set GPIO 16 to output!\n"); goto Gpio16Error;
    }
    /* GPIO 12 init */
    if(gpio_request(12, "rpi-gpio-12")) { 
        printk("Can not allocate GPIO 12\n"); goto AddError;
    }
    /* Set GPIO 12 direction */
    if(gpio_direction_output(12, 0)) { 
        printk("Can not set GPIO 12 to output!\n"); goto Gpio12Error;
    }
    /* GPIO 7 init */
    if (gpio_request(7, "rpi-gpio-7")) { 
        printk("Can not allocate GPIO 7\n"); goto AddError;
    }
    /* Set GPIO 7 direction */
    if(gpio_direction_output(7, 0)) { 
        printk("Can not set GPIO 7 to output!\n"); goto Gpio7Error;
    }
    /* GPIO 8 init */
    if(gpio_request(8, "rpi-gpio-8")) { 
        printk("Can not allocate GPIO 8\n"); goto AddError;
    }
    /* Set GPIO 8 direction */
    if (gpio_direction_output(8, 0)) { 
        printk("Can not set GPIO 8 to output!\n"); goto Gpio8Error;
    }
    /* GPIO 25 init */ 
    if(gpio_request(25, "rpi-gpio-25")) { 
        printk("Can not allocate GPIO 25\n"); goto AddError;
    }
    /* Set GPIO 25 direction */ 
    if(gpio_direction_output(25, 0)) { 
        printk("Can not set GPIO 25 to output!\n"); goto Gpio25Error;
    }
    /* GPIO 24 init */ 
    if(gpio_request(24, "rpi-gpio-24")) {
        printk("Can not allocate GPIO 24\n"); goto AddError;
    }
    /* Set GPIO 24 direction */ 
    if(gpio_direction_output(24, 0)) { 
        printk("Can not set GPIO 24 to output!\n"); goto Gpio24Error;
    }
    return 0;
    Gpio2Error: gpio_free(2);
    Gpio3Error: gpio_free(3);
    Gpio4Error: gpio_free(4);
    Gpio17Error: gpio_free(17);
    Gpio21Error: gpio_free(21);
    Gpio20Error: gpio_free(20);
    Gpio16Error: gpio_free(16);
    Gpio12Error: gpio_free(12);
    Gpio7Error: gpio_free(7);
    Gpio8Error: gpio_free(8);
    Gpio25Error: gpio_free(25);
    Gpio24Error: gpio_free(24);
    AddError: device_destroy (my_class, my_device_nr);
    FileError: class_destroy(my_class);
    ClassError:
    unregister_chrdev_region (my_device_nr, 1);
    return -1;
}
/*1
/@brief This function is called, when the module is removed from the kernel
*/
static void __exit ModuleExit(void) {
    gpio_set_value(2, 0);
    gpio_set_value(3, 0);
    gpio_set_value(4, 0);
    gpio_set_value(17, 0);
    gpio_set_value(21, 0);
    gpio_set_value(20, 0);
    gpio_set_value(16, 0);
    gpio_set_value(12, 0);
    gpio_set_value(7, 0);
    gpio_set_value(8, 0);
    gpio_set_value(25, 0);
    gpio_set_value(24, 0);
    gpio_free(2);
    gpio_free(3);
    gpio_free(4);
    gpio_free(17);
    gpio_free(21);
    gpio_free(20);
    gpio_free(16);
    gpio_free(12);
    gpio_free(7);
    gpio_free(8);
    gpio_free(25);
    gpio_free(24);
    cdev_del(&my_device);
    device_destroy (my_class, my_device_nr);
    class_destroy(my_class);
    unregister_chrdev_region (my_device_nr, 1);
    printk("Goodbye, Kernel\n");
}
module_init(ModuleInit);
module_exit(ModuleExit);
