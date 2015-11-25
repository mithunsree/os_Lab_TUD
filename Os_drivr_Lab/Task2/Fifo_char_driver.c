/* Linux kernel headers */
#include <linux/module.h>
#include <linux/init.h>

#include<linux/fs.h> //For Charachter driver support


MODULE_LICENSE("GPL"); /* code license for the module */
MODULE_AUTHOR("Clindo"); /* name of module author */
#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
int ret, fd;
char stringToSend[BUFFER_LENGTH];
 

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

printk(KERN_ALERT,"Inside the InitFifo function");

//registring the driver with kernel register_chrdev(majornumber,name,file_operations)

register_chrdev(240,"Fifo_char_driver",&Fifo_file_operations);


return 0; /* or some error code */

}
/* module cleanup, last function to be called */
static void __exit exit_Fifo_Driver(void) {
printk(KERN_ALERT,"Inside the exit Fifo function");

//unregistring the driver with kernel register_chrdev(majornumber,name)

unregister_chrdev(240,"FIFO_Driver");

}



static int dev_open(struct inode *inodep, struct file *filep){

printk(KERN_ALERT,"Inside the dev_open function");


fd = open("/dev/Fifo_char_driver", O_RDWR);             // Open the device with read/write access

   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }

   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){

printk(KERN_ALERT,"Inside the dev_read function");

  printf("Press ENTER to read back from the device...\n");
   getchar();

   printf("Reading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("The received message is: [%s]\n", receive);
   return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
printk(KERN_ALERT,"Inside the dev_write function");

 printf("Type in a short string to send to the kernel module:\n");
   scanf("%[^\n]%*c", stringToSend);                // Read in a string (with spaces)
   printf("Writing message to the device [%s].\n", stringToSend);
   ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }

   return len;
}

static int dev_release(struct inode *inodep, struct file *filep){

printk(KERN_ALERT,"Inside the dev_release function");
   return 0;
}



/* declare init/exit functions for the kernel */
module_init(Init_Fifo_Driver);
module_exit(exit_Fifo_Driver);
