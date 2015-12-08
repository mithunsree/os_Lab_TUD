/* Linux kernel headers */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <asm/uaccess.h>
#include<linux/fs.h> //For Charachter driver support
#define  DEVICE_NAME "Fifo_char_Driver"    ///< The device will appear at /dev/ebbchar using this value
#define  CLASS_NAME  "FifoDiverClass"        ///< The device class -- this is a character device driver


MODULE_LICENSE("GPL"); /* code license for the module */


static int    majorNumber;                  ///< Stores the device number -- determined automatically
static char   message[256] = {0};           ///< Memory for the string that is passed from userspace
static short  size_of_message;              ///< Used to remember the size of the string stored
static int    numberOpens = 0;              ///< Counts the number of times the device is opened
static struct class*  FifoDriverClass  = NULL; ///< The device-driver class struct pointer
static struct device* FifoDriver = NULL; ///< The device-driver device struct pointer


// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);


/** @brief Devices are represented as file structure in the kernel. The file_operations structure from
*  /linux/fs.h lists the callback functions that you wish to associated with your file operations
*  using a C99 syntax structure. char devices usually implement open, read, write and release calls
*/
static struct file_operations Fifo_file_operations =
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};



/* module initialization, first function to be called */
static int __init Init_Fifo_Driver(void) {
    
    printk(KERN_ALERT"Inside the InitFifo function");
    
    //getting majo number for our LKM
    register_chrdev(240,"Fifo_char_driver",&Fifo_file_operations);
    majorNumber=40;
    printk(KERN_INFO "Fifo Char Driver: registered correctly with major number %dn", majorNumber);
    
    
    // Register the device class
    Fifo_char_DriverClass = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(Fifo_char_DriverClass)){                // Check for error and clean up if there is
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device classn");
        return PTR_ERR(Fifo_char_DriverClass);          // Correct way to return an error on a pointer
    }
    printk(KERN_INFO "Fifo Char Driver: device class registered correctlyn");
    
    // Register the device driver
    Fifo_char_Driver = device_create(Fifo_char_DriverClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(Fifo_char_Driver)){               // Clean up if there is an error
        class_destroy(Fifo_char_DriverClass);           // Repeated code but the alternative is goto statements
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the devicen");
        return PTR_ERR(Fifo_char_Driver);
    }
    printk(KERN_INFO "FifoDriver: device class created correctlyn"); // Made it! device was initialized
    return 0;
}

/* module cleanup, last function to be called */
static void __exit exit_Fifo_Driver(void) {
    printk(KERN_ALERT"Inside the exit Fifo function");
    
    device_destroy(FifoDriverClass, MKDEV(majorNumber, 0));     // remove the device
    class_unregister(FifoDriverClass);                          // unregister the device class
    class_destroy(FifoDriverClass);                             // remove the device class
    unregister_chrdev(240,"FIFO_Driver");
    printk(KERN_INFO "FifoDriver: Goodbye from the LKM!n");
    
    
}



static int dev_open(struct inode *inodep, struct file *filep){
    
   numberOpens++;
   printk(KERN_INFO "FIFO Driver: Device has been opened %d time(s)\n", numberOpens);
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
    
  int error_count = 0;
   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, message, size_of_message);

   if (error_count==0){            // if true then have success
      printk(KERN_INFO "FIFO Driver: Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "FIFO Driver: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%d letters)", buffer, len);   // appending received string with its length
   size_of_message = strlen(message);                 // store the length of the stored message
   printk(KERN_INFO "FIFO Driver: Received %d characters from the user\n", len);
   return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
    
   printk(KERN_INFO "FIFO Driver: Device successfully closed\n");
   return 0;
}



/* declare init/exit functions for the kernel */
module_init(Init_Fifo_Driver);
module_exit(exit_Fifo_Driver);
