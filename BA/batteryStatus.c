#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

//#define DEVICE_FILE_NAME "/proc/battery_test"

#define DEVICE_DRIVER_NAME "/dev/battery_driver_test"

#define MAX_BUF_SIZE 128

void main(){

	int device;
	int i,j;
	int level =0;

	char rbuf[MAX_BUF_SIZE];

	while(1){
		device = open(DEVICE_DRIVER_NAME, O_RDWR | O_NDELAY);

		//device = open(DEVICE_FILE_NAME, O_RDWR | O_NDELAY);
		if( device >=0) {
			read(device,rbuf,10);	
			level = atoi(rbuf);
			printf(" ---------- \n|");
			for( i=0; i< level/10; i++)
				printf("#");
			for( j=10; i< j ; i++)
				printf(" ");
			printf("| ( %d% )\n", level);
			//printf("|##########| ( %d% )\n", test_level);
			printf(" ---------- \n\n");
		}
		else{
			perror("Device file open failed\n");
			break;
		}
		close(device);
		sleep(1);	
	}
}

