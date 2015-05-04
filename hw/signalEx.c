#include <signal.h>
#include <stdio.h>

static void sig_usr(int); /* one handler for both signals */
int main(void){
	printf("%d\n", getpid());
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		//err_sys("can't catch SIGUSR1");
		printf("can't catch SIGUSR1\n");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		//err_sys("can't catch SIGUSR2");
		printf("can't catch SIGUSR2\n");
	for ( ; ; ) pause();
}
static void sig_usr(int signo) { /* argument is signal number */
	if (signo == SIGUSR1){
		printf("received SIGUSR1\n");
		printf("------ Save mode ------\n");
	}
	else if (signo == SIGUSR2){
		printf("received SIGUSR2\n");
		printf("------ Normal mode ------\n");
	}
	else 
		//err_dump("received signal %d\n", signo);
		printf("received signal %d\n", signo);
	
	return;
}

