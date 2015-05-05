#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE_FILE_NAME "/proc/battery_test"
#define MAX_BUF_SIZE 128

void main(){

	int device;
	char rbuf[MAX_BUF_SIZE];

	while(1){
		device = open(DEVICE_FILE_NAME, O_RDWR | O_NDELAY);
		if( device >=0) {
			read(device,rbuf,10);
			printf("test_level : %s\n", rbuf);
		}
		else{
			perror("Device file open failed\n");
			break;
		}
		close(device);
		sleep(3);	
	}
}

