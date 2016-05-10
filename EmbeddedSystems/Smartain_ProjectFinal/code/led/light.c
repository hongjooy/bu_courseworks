#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "i2c-dev.h"
#include <errno.h>
#include <string.h>
#include <time.h>
#include "luxcalc.h"

#define COMMAND 0x80
#define CONTROL 0x00
#define ON 0x03
#define OFF 0x00
#define TIMING 0x01
#define WORD 0x20
#define LOW0 0x0C
#define LOW1 0x0E
#define GAIN 0x00

//https://www.kernel.org/doc/Documentation/i2c/dev-interface
//Adafruit documentation


int main()
{
	int file;
	volatile unsigned addr = 0x29;							//One of the I2C Address provided
	char *filename = "/dev/i2c-0";
	unsigned buf,buf2, channel0, channel1;
	unsigned int Lux;
	int avgLux;
	char str[10];
	int loop;
	int avg;
	avg = 4;

	if((file = open(filename, O_RDWR)) < 0){
		printf("Failed to open\n");
		exit(1);
	}
	// Change slave address
	if (ioctl(file,I2C_SLAVE_FORCE,addr) < 0){
		printf("Failed\n");
		exit(1);
	}
	// SMbus commands
	i2c_smbus_write_byte_data(file,COMMAND|CONTROL,ON);
	i2c_smbus_write_byte_data(file,COMMAND|TIMING,GAIN|TIMING);
	
	while(1){
		avgLux = 0;
		loop = 0;
		while(loop<avg){
			// More SMbus commands
			i2c_smbus_write_byte_data(file,COMMAND|CONTROL,ON);
			usleep(101000);							//Delay the reading time
			channel0 = i2c_smbus_read_word_data(file,COMMAND|WORD|LOW0);
			channel1 = i2c_smbus_read_word_data(file,COMMAND|WORD|LOW1);
			Lux = CalculateLux(0,1,channel0,channel1,0);
			avgLux += Lux;
			loop++;
			i2c_smbus_write_byte_data(file,COMMAND|CONTROL,OFF);
		}
		avgLux = avgLux/avg;

		if(avgLux < 500){
			avgLux = 80000;							//Set a limit for the average lux
		}
		FILE * pFile = fopen("/dev/mygpio", "w");				//Store the lux values in the node file
		sprintf(str,"l%d",avgLux);						//l is used as a flag so that we know its a lux value
		fwrite(str,1,sizeof(str),pFile);
		fclose(pFile);
		//printf("Lux: %u\n",avgLux);						//Was used to check the lux value live

		
	}
		close(file);
}
