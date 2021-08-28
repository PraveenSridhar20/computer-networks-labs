#include <stdio.h>
#include <unistd.h> 
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 



int main(){

	int fdr,fdw;
	char* serverfileName="serverfifo";
	char* clientfileName="clientfifo";
	mkfifo(clientfileName,0666);
	mkfifo(serverfileName,0666);
	int shmid=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	int x=fork();
	
	if (x!=0){//parent is the writer
		signal (SIGPIPE,SIG_IGN);
		while (1){
		
			char wrt[100];
			printf ("Send from Client:");
			fflush(stdout);
			fgets(wrt,100,stdin);
			fflush(stdin);
	
			fdw=open(serverfileName,O_WRONLY);
			write (fdw,wrt,strlen(wrt)+1);
			close (fdw);
			
			if (!strcmp(wrt,"exit\n")){
				kill(x,SIGKILL);
				unlink(clientfileName);
				exit(0);
			}
			
		}	
	}
	else{
		
		while (1){
			char rd[100];
			mkfifo(clientfileName,0666);
		 	fdr=open(clientfileName,O_RDONLY);
		 	read(fdr,rd,100);
		 	
		 	printf("\nReceived from Server:%s",rd);
		 	close (fdr);
		 	
		 	if (!strcmp(rd,"exit\n")){
		 		printf("Server offline\n");
		 	}
		 	
		 	printf("Send from Client:");
		 	fflush(stdout);
		 	}
		
	}
	

	return 0;
}
