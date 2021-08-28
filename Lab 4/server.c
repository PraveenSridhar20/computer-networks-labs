#include <stdio.h>
#include <unistd.h> 
#include<string.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<stdlib.h>
#include<signal.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 



int main(){

	int fdr,fdw;
	char* clientfileName="clientfifo";
	char* serverfileName="serverfifo";
	mkfifo(clientfileName,0666);
	mkfifo(serverfileName,0666);
	int shmid=shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	int x=fork();
	
	
	if (x!=0){//parent is the writer
		
		while (1){
			char wrt[100];
			printf ("Send from Server:");
			fflush(stdout);
			fgets(wrt,100,stdin);
			fflush(stdin);
			
			fdw=open(clientfileName,O_WRONLY);
			write (fdw,wrt,strlen(wrt)+1);
			close (fdw);

			if (!strcmp(wrt,"exit\n")){
				
				kill(x,SIGKILL);
				unlink(serverfileName);
				exit(0);
			}
			
		}	
	}
	else{
		//int parent_id=getppid();

		while (1){
			char rd[100];
			mkfifo(serverfileName,0666);
			fdr=open(serverfileName,O_RDONLY);
			read(fdr,rd,100);
	
			printf("\nReceived from Client:%s",rd);
			close (fdr);
			
			if (!strcmp(rd,"exit\n")){
				printf("Client offline\n");
			}
			
			printf("Send from Server:");
			fflush(stdout);
		 	
		}
	}

	return 0;
}
