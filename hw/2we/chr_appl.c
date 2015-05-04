#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/ioctl.h> 
#include <unistd.h> 

#define DEVICE_FILE_NAME "/dev/chr_dev"
#define BUFF_SIZE 128

int main(int argc, char *argv[]){

	int device;
	char wbuf[BUFF_SIZE] = "Write buffer data";
	char rbuf[BUFF_SIZE] = "Read buffer data";
	int n = atoi(argv[1]);
	int battery =0;	

	device = open(DEVICE_FILE_NAME, O_RDWR | O_NDELAY);
	
	if(device >= 0){
		strcpy(wbuf, argv[2]); 
		printf("Device file Open\n");
		ioctl(device, n);
		write(device, wbuf, BUFF_SIZE);
		printf("Write value is %s\n", wbuf);
		read(device, rbuf, BUFF_SIZE);
		printf("Read value is %s\n", rbuf);
	}
	else
		perror("Device file open fail");
	
	return 0;
}

