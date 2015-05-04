#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void main(int argc, char *argv[]){
	int pid;
	sscanf(argv[1], "%d", &pid); 
	while(1){
		kill(pid,SIGUSR1);
		printf("klll -USR1 %s\n", argv[1]);
		
		sleep(1);
		
		kill (pid,SIGUSR2);
		printf("kill -USR2 %s\n", argv[1]);
		sleep(1);	
	}
}
