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
#define DRIVER_CLASS "SegmentControlModule"
/* @brief Write data to buffer */

#define SEGOFF 1
#define SEGON  0

#define OFF 0
#define ON  1

#define A  10
#define B  11
#define C  13
#define D  5
#define E  0
#define F  9
#define G  19
#define DP 6

#define SEG01 2 
#define SEG02 3
#define SEG03 4
#define SEG04 7

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
    if(gpio_request(A, "rpi-gpio-10")) { 
        printk("Can not allocate A\n"); goto AddError;
    }
    /* Set GPIO 2 direction */
    if (gpio_direction_output(A, 0)) {
        printk("Can not set A to output!\n"); goto Gpio2Error;
    }
    if(gpio_request(B, "rpi-gpio-3")) { 
        printk("Can not allocate B\n"); goto AddError;
    }
    /* Set GPIO 3 direction */ 
    if (gpio_direction_output(B, 0)) { 
        printk("Can not set B to output!\n"); goto Gpio3Error;
    }
    /* GPIO 4 init */ 
    if(gpio_request(C, "rpi-gpio-4")) { 
        printk("Can not allocate C\n"); goto AddError;
    }
    /* Set GPIO 4 direction */ 
    if(gpio_direction_output(C, 0)) {
        printk("Can not set C to output!\n"); goto Gpio4Error;
    }
    /* GPIO 17 init */ 
    if (gpio_request(D, "rpi-gpio-17")) { 
        printk("Can not allocate D\n"); goto AddError;
    }
    /* Set GPIO 17 direction */
    if(gpio_direction_output(D, 0)) { 
        printk("Can not set D to output!\n"); goto Gpio17Error;
    }
    /* Set A~DP segments GPIO */
    /* GPIO 21 init */
    if(gpio_request(E, "rpi-gpio-21")) { 
        printk("Can not allocate E\n"); goto AddError;
    }
    /* Set GPIO 21 direction */
    if (gpio_direction_output(E, 0)) { 
        printk("Can not set E to output!\n"); goto Gpio21Error;
    }
    /* GPIO 20 init */
    if (gpio_request(F, "rpi-gpio-20")) { 
        printk("Can not allocate F\n"); goto AddError;
    }
    /* Set GPIO 20 direction */
    if (gpio_direction_output (F, 0)) { 
        printk("Can not set F to output!\n"); goto Gpio20Error;
    }
    /* GPIO 16 init */
    if(gpio_request(G, "g")) { 
        printk("Can not allocate G\n"); goto AddError;
    }
    /* Set GPIO 16 direction */
    if(gpio_direction_output(G, 0)) { 
        printk("Can not set G to output!\n"); goto Gpio16Error;
    }
    /* GPIO 12 init */
    if(gpio_request(DP, "DP")) { 
        printk("Can not allocate DP\n"); goto AddError;
    }
    /* Set GPIO 12 direction */
    if(gpio_direction_output(DP, 0)) { 
        printk("Can not set DP to output!\n"); goto Gpio12Error;
    }
    /* GPIO 7 init */
    if (gpio_request(SEG01, "seg01")) { 
        printk("Can not allocate seg01\n"); goto AddError;
    }
    /* Set GPIO 7 direction */
    if(gpio_direction_output(SEG01, 0)) { 
        printk("Can not set seg01 to output!\n"); goto Gpio7Error;
    }
    /* GPIO 8 init */
    if(gpio_request(SEG02, "seg02")) { 
        printk("Can not allocate seg02\n"); goto AddError;
    }
    /* Set GPIO 8 direction */
    if (gpio_direction_output(SEG02, 0)) { 
        printk("Can not set SEG02 to output!\n"); goto Gpio8Error;
    }
    /* GPIO 25 init */ 
    if(gpio_request(SEG03, "seg03")) { 
        printk("Can not allocate SEG03\n"); goto AddError;
    }
    /* Set GPIO 25 direction */ 
    if(gpio_direction_output(SEG03, 0)) { 
        printk("Can not set SEG03 to output!\n"); goto Gpio25Error;
    }
    /* GPIO 24 init */ 
    if(gpio_request(SEG04, "seg04")) {
        printk("Can not allocate seg04\n"); goto AddError;
    }
    /* Set GPIO 24 direction */ 
    if(gpio_direction_output(SEG04, 0)) { 
        printk("Can not set seg04 to output!\n"); goto Gpio24Error;
    }
    return 0;
    Gpio2Error: gpio_free(A);
    Gpio3Error: gpio_free(B);
    Gpio4Error: gpio_free(C);
    Gpio17Error: gpio_free(D);
    Gpio21Error: gpio_free(E);
    Gpio20Error: gpio_free(F);
    Gpio16Error: gpio_free(G);
    Gpio12Error: gpio_free(DP);
    Gpio7Error: gpio_free(SEG01);
    Gpio8Error: gpio_free(SEG02);
    Gpio25Error: gpio_free(SEG03);
    Gpio24Error: gpio_free(SEG04);
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
    gpio_set_value(A, 0);
    gpio_set_value(B, 0);
    gpio_set_value(C, 0);
    gpio_set_value(D, 0);
    gpio_set_value(E, 0);
    gpio_set_value(F, 0);
    gpio_set_value(G, 0);
    gpio_set_value(DP, 0);
    gpio_set_value(SEG01, 0);
    gpio_set_value(SEG02, 0);
    gpio_set_value(SEG03, 0);
    gpio_set_value(SEG04, 0);
    gpio_free(A);
    gpio_free(B);
    gpio_free(C);
    gpio_free(D);
    gpio_free(E);
    gpio_free(F);
    gpio_free(G);
    gpio_free(DP);
    gpio_free(SEG01);
    gpio_free(SEG02);
    gpio_free(SEG03);
    gpio_free(SEG04);
    cdev_del(&my_device);
    device_destroy (my_class, my_device_nr);
    class_destroy(my_class);
    unregister_chrdev_region (my_device_nr, 1);
    printk("Goodbye, Kernel\n");
}
module_init(ModuleInit);
module_exit(ModuleExit);
