#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <asm/gpio.h>
#include <asm/hardware.h>

MODULE_LICENSE("GPL");

static int timer_major = 61;

static int percentage = 0;
static int cur_percentage = 0;
static int steps;
static int led;
static int lux;
static int smart_mode = 0;
static int running_flag = 0;
static int step_size = 12;
static int step_speed = 5;

// Character device functions
static ssize_t  timer_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t  timer_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);

// Module functions
static int  timer_init(void);
static void timer_exit(void);
static int  timer_open(struct inode *inode, struct file *filp);
static int  timer_release(struct inode *inode, struct file *filp);

// LEDs, buttons, state variables
int gpio_irq = 0;
int counting_irq = 0;
int pwm_irq = 0;
//static int brightness = 0;

unsigned char led_status = 1;
unsigned char counting_period = 2; // 1 second default
unsigned char direction = 1; // 1 -> up, 0 -> down
unsigned char counting = 0; // 1 -> up, 0 -> down
char brightness_level;

// Structure for file operations
struct file_operations timer_fops = {
	read: timer_read,
	write: timer_write,
	open: timer_open,
	release: timer_release
};

// Buffer definitions
#define LENGTH_READ_BUFFER 150 // string that contains: <message> <time remaining>\n
#define LENGTH_TIMER_MESSAGE 128 // max number of chars for message
#define LENGTH_TIMER_REMAINING 20 // max number of chars for # of seconds remaining
#define LENGTH_WRITE_BUFFER 148 // receive buffer

// Buffers for reading and writing
static char *read_buffer;
static char *write_buffer;

// struct to hold data for a timer
struct timer_data {
  char *message;
};

// Timer and its data
static struct timer_list main_timer;

static void timer_done(unsigned long data); // Function that gets called when a timer is done

// Initialization of module
static int timer_init()
{
  int retvalue = 0;
  
  // Initialize buffers
  read_buffer = (char *)kmalloc(LENGTH_READ_BUFFER, GFP_KERNEL); // make space for read_buffer
  memset(read_buffer, '\0', LENGTH_READ_BUFFER); // Initialize it
  
  write_buffer = (char *)kmalloc(LENGTH_WRITE_BUFFER, GFP_KERNEL); // make space for write_buffer
  memset(write_buffer, '\0', LENGTH_WRITE_BUFFER); // Initialize
  
  // Register device
  retvalue = register_chrdev(timer_major, "mygpio", &timer_fops);
  if(retvalue < 0)
    printk(KERN_ALERT "mygpio: Cannot obtain major number %d\n", timer_major);

  // Initialize timer information
  //init_timer(&main_timer);
  setup_timer(&main_timer, timer_done, 0);
  
  // Set up LEDs
  gpio_direction_output(28, 1);
  gpio_direction_output(29, 1);
  gpio_direction_output(30, 1);
  gpio_direction_output(31, 1);
  gpio_direction_output(101, 1);
  pxa_gpio_set_value(28, 1);
  pxa_gpio_set_value(29, 0);
  pxa_gpio_set_value(30, 1);
  pxa_gpio_set_value(31, 0);
  pxa_gpio_set_value(101, 0);

  mod_timer(&main_timer, get_jiffies_64() + msecs_to_jiffies(500 * counting_period));
  return retvalue;
}
module_init(timer_init);

// Exit of module
static void timer_exit()
{  
  // Free up memory
  kfree(read_buffer);
  kfree(write_buffer);
  del_timer(&main_timer);
  
  //clear LEDs
  pxa_gpio_set_value(28, 1);
  pxa_gpio_set_value(29, 0);
  pxa_gpio_set_value(30, 1);
  pxa_gpio_set_value(31, 0);
  pxa_gpio_set_value(101, 0);
  // Remove device, proc, irq's
  unregister_chrdev(timer_major, "mygpio");
}
module_exit(timer_exit);

// Function called when timer is finished
static void timer_done(unsigned long data)
{
  mod_timer(&main_timer, get_jiffies_64() + msecs_to_jiffies( step_speed * counting_period));
  
  if (steps > 0){
	running_flag = 1;	 
  	if(direction == 1){  
	   led_status++;
	   led_status %= 4;
	   steps --;	   
	   switch(led_status){
	  	case 0:
		  pxa_gpio_set_value(28, 1);
		  pxa_gpio_set_value(29, 0);
		  pxa_gpio_set_value(30, 1);
		  pxa_gpio_set_value(31, 0);
		  break;
		case 1:
		  pxa_gpio_set_value(28, 1);
		  pxa_gpio_set_value(29, 0);
		  pxa_gpio_set_value(30, 0);
		  pxa_gpio_set_value(31, 1);
		  break;
		case 2:
		  pxa_gpio_set_value(28, 0);
		  pxa_gpio_set_value(29, 1);
		  pxa_gpio_set_value(30, 0);
		  pxa_gpio_set_value(31, 1);
		  break;
		case 3:
		  pxa_gpio_set_value(28, 0);
		  pxa_gpio_set_value(29, 1);
		  pxa_gpio_set_value(30, 1);
		  pxa_gpio_set_value(31, 0);
		  break;
	  };
	}else if(direction==0){
	   led_status--;
	   led_status %= 4;
	   steps --;
	   switch(led_status){
	  	case 0:
		  pxa_gpio_set_value(28, 1);
		  pxa_gpio_set_value(29, 0);
		  pxa_gpio_set_value(30, 1);
		  pxa_gpio_set_value(31, 0);
		  break;
		case 1:
		  pxa_gpio_set_value(28, 1);
		  pxa_gpio_set_value(29, 0);
		  pxa_gpio_set_value(30, 0);
		  pxa_gpio_set_value(31, 1);
		  break;
		case 2:
		  pxa_gpio_set_value(28, 0);
		  pxa_gpio_set_value(29, 1);
		  pxa_gpio_set_value(30, 0);
		  pxa_gpio_set_value(31, 1);
		  break;
		case 3:
		  pxa_gpio_set_value(28, 0);
		  pxa_gpio_set_value(29, 1);
		  pxa_gpio_set_value(30, 1);
		  pxa_gpio_set_value(31, 0);
		  break;
	  };
	}
  }else{
	running_flag = 0;
  }
}


// Read (sending data)
static ssize_t timer_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
  unsigned int sprintf_index = 0;
  return 0;
}

// Write (receiving data)
static ssize_t timer_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
 
  memset(write_buffer, '\0', LENGTH_WRITE_BUFFER);

  if(*f_pos >= LENGTH_WRITE_BUFFER)
    return -ENOSPC;
  
  if(copy_from_user(write_buffer + *f_pos, buf, count))
    return -EFAULT;

  char flag = write_buffer[0];
  char* endptr;
  
  if(flag == 's'){
    int num = simple_strtoul(write_buffer+1, &endptr, 16);
    smart_mode = num;
  }


  else if(flag == 'f'){
    int num = simple_strtoul(write_buffer+1, &endptr, 16);
    led = num;	
    if (smart_mode == 0){
    	if(led == 1){
   	 	pxa_gpio_set_value(101, 1);
  	}else if(led == 0){
    		pxa_gpio_set_value(101, 0);
   	}
    }    
  }

  else if(flag == 'a'){
    int num = simple_strtoul(write_buffer+1, &endptr, 16);
    step_speed = num * 5;
  }

  else if(flag == 'l'){
    int num = simple_strtoul(write_buffer+1, &endptr, 10);
    lux = num;
    if (smart_mode == 1){
	if (lux > 5000){
	    	if(lux <= 30000){
	   	 	pxa_gpio_set_value(101, 1);
			if (running_flag == 0){
				percentage = 155;
				if ((percentage - cur_percentage) > 0){
				  	direction = 1;
				  	steps = (percentage - cur_percentage)*step_size;
				  	cur_percentage = percentage;
			  	}else if((percentage - cur_percentage) < 0){
				  	direction = 0;
				  	steps = (cur_percentage - percentage)*step_size;
				  	cur_percentage = percentage;
			  	}
			}
	  	}else{
	    		pxa_gpio_set_value(101, 0);
			if (running_flag == 0){
				percentage = 0;
				if ((percentage - cur_percentage) > 0){
					direction = 1;
					steps = (percentage - cur_percentage)*step_size;
					cur_percentage = percentage;
				}else if((percentage - cur_percentage) < 0){
					direction = 0;
					steps = (cur_percentage - percentage)*step_size;
					cur_percentage = percentage;
				}
			}
	   	}
	} 
    }   
  }

  else if(flag == 'v'){
    int num = simple_strtoul(write_buffer+1, &endptr, 16);
    percentage = num;
    if (smart_mode == 0){
	  if ((percentage - cur_percentage) > 0){
	  	direction = 1;
	  	steps = (percentage - cur_percentage)*step_size;
	  	cur_percentage = percentage;
	  }else if((percentage - cur_percentage) < 0){
	  	direction = 0;
	  	steps = (cur_percentage - percentage)*step_size;
	  	cur_percentage = percentage;
	  }
    }
  }
  
  
  
  *f_pos += count;
  return count;
}

// Opening the special file
static int timer_open(struct inode *inode, struct file *filp)
{
  return 0;
}

// Releasing the special file
static int timer_release(struct inode *inode, struct file *filp)
{
  
  return 0;
}
