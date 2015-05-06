#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

//one handler for both signals 
static void sig_usr(int);


//proc file name 
#define DEVICE_BATTERY_NOTIFY "/proc/battery_notify"
#define DEVICE_BATTERY_THRESHOLD "/proc/battery_threshold"
#define MAX_BUF_SIZE 128

//void main(int argc, char *argv[]){
void main(){	
	int deviceNotify, deviceThreshold;
	char notifyBuf[MAX_BUF_SIZE];
	char threshBuf[MAX_BUF_SIZE];
	char threshold[3];		
	signal(SIGUSR1,sig_usr);
	signal(SIGUSR2,sig_usr);
	//scanf("%s",threshold);
	//To do
	//1. write pid at procf
	deviceNotify = open(DEVICE_BATTERY_NOTIFY, O_RDWR | O_NDELAY);
	if( deviceNotify >= 0 ){
		printf( "active notify and pid %d\n",getpid());
		sprintf(notifyBuf, "%d", getpid());
		write(deviceNotify,notifyBuf,10);
		close(deviceNotify);
	}

	else
		perror( " notify failed\n");

	//2. write threshold at procf
	deviceThreshold =open(DEVICE_BATTERY_THRESHOLD, O_RDWR | O_NDELAY);
	if( deviceThreshold >= 0){
		scanf("%s",threshold);
		printf(" active threshold and threshold %s\n",threshold);
		write(deviceThreshold, threshold,10);
		
		//int threshold = atoi(argv[1]);
		//sprintf(threshBuf, "%s", argv[1]);
		//write(deviceThreshold,threshBuf,10);
		//close(deviceThreshold);
	}
	else
		perror(" threshold failed\n");
	while(1){
		
		scanf("%s",threshold);
		printf(" active threshold and threshold %s\n",threshold);
		write(deviceThreshold, threshold,10);
	}
	close(deviceThreshold);
	//3. handling SIGUSR1 
	//4. handling SIGUSR2
	
}



/*
one handler for both signals
argument is signal number
*/
static void sig_usr(int signo){
	
	if (signo == SIGUSR1){
		printf("---------Received SIGUSR1---------\n");
		printf("           SAVE MODE\n");
		printf("----------------------------------\n");
	}
	else if( signo ==SIGUSR2){
		printf("---------Received SIGUSR2---------\n");
		printf("           NORMAL MODE\n");
		printf("----------------------------------\n");
	}
	else
		//err_dump("received signal %d\n", signo);
		printf("receivced signal %d\n", signo);
	return;
}
