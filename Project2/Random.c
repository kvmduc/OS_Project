#include <linux/init.h>   
#include <linux/module.h> 
#include <linux/device.h>
#include <linux/kernel.h> 
#include <linux/fs.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#define DEVICE_NAME "RANDOMDevice" 
#define CLASS_NAME "RANDOM"      

MODULE_LICENSE("GPL");

static int RANDOMDevice_init(void) __init;
static void RANDOMDevice_exit(void) __exit;
static int random_open(struct inode *, struct file *);
static ssize_t random_read(struct file *, char *, size_t, loff_t *);
static int random_release(struct inode *inodep, struct file *filep);

static struct file_operations fops =
{
        .open = random_open,
        .read = random_read,
        .release = random_release,
};

static int majorNumber;
static struct class *randomClass = NULL;   
static struct device *randomDevice = NULL;
static int randomNum;

static int __init RANDOMDevice_init(void) {
    //dang ky major number
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if(majorNumber < 0){
        printk(KERN_ALERT "RANDOMDevice : failed to register major number ");
        return majorNumber;
    }
    printk(KERN_INFO "RANDOMDevice : Registered correctly with major number %d\n", majorNumber);

    //create class randomClass
    randomClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(randomClass)){
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "RANDOMDevice : Failed to register device class\n");
        return PTR_ERR(randomClass);
    }
    printk(KERN_INFO "RANDOMDevice : Create random class correctly %d\n", majorNumber);

    //create device randomDevice
    randomDevice = device_create(randomClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(randomDevice))
    {                              
        class_destroy(randomClass);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "RANDOMDevice : Failed to create the device\n");
        return PTR_ERR(randomDevice);
    }
    printk(KERN_INFO "RANDOMDevice : Create random device correctly %d\n", majorNumber);
    printk(KERN_INFO "RANDOMDevice : device class created correctly\n"); 
    return 0;
}

static void __exit RANDOMDevice_exit(void) {
    device_destroy(randomClass, MKDEV(majorNumber, 0)); // huy device
    class_unregister(randomClass);                      // huy dang ky device class
    class_destroy(randomClass);                         // xoa device class
    unregister_chrdev(majorNumber, DEVICE_NAME);        // huy dang ky device major number
}

static int random_open(struct inode *inodep, struct file *filep ){
    int len = 0;
    printk(KERN_INFO "RANDOMDevice : Device is opening\n");
    return 0;
}

static ssize_t random_read(struct file *filep, char *buffer, size_t len, loff_t *off){
	get_random_bytes(&randomNum,sizeof(int));
	printk(KERN_INFO "RANDOMDevice : Random number is %d\n", randomNum);
    if (copy_to_user(buffer,&randomNum,sizeof(randomNum)) != 0 )
    {
        printk(KERN_INFO "Kernel -> userspace copy failed!\n" );
        return 13;
    }   
    return 0;
}

static int random_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "RANDOMDevice : Device successfully closed\n");
    return 0;
}

module_init(RANDOMDevice_init);
module_exit(RANDOMDevice_exit);
