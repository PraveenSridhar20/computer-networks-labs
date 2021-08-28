#include <stdio.h> 
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include <netinet/in.h> 
#include <sys/types.h> 
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<signal.h>


int main(int argc, char* argv[]) 
{ 	
	
	if (argc!=2){
		printf("Invalid argument count\n");
		exit(0);
	}
	int port_num=atoi(argv[1]);
	int socket_fd;
	
	struct sockaddr_in server_address,client1,client2;
	socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (socket_fd==-1){
		printf("Socket creation failed\n");
		exit(0);
	}
		
	printf("\nSocket creation successful\n");
		
	memset(&server_address,0,sizeof(server_address));
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=htonl(INADDR_ANY);
	server_address.sin_port=htons(port_num);
		
	int bindval=bind(socket_fd,(struct sockaddr*)&server_address,sizeof(server_address));
	if (bindval==-1){
		printf("Binding failed\n");
		exit(0);
	}
		
	printf("Binding successful\n");
		
	int listenval=listen(socket_fd,5);
	if (listenval==-1){
		printf("Listening failed\n");
		exit(0);
	}
		
	printf("Listening success\n");
	int l1=sizeof(client1);
	int l2=sizeof(client2);
	int shmid1 = shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT); 
	int shmid2 = shmget(IPC_PRIVATE,sizeof(int),0666|IPC_CREAT);
	int *connect_fd1 = (int*) shmat(shmid1,0,0);
	int *connect_fd2 = (int*) shmat(shmid2,0,0);
	
	while(1){
		printf("\nWaiting for 2 clients to connect\n");
		*connect_fd1=accept(socket_fd,(struct sockaddr*)&client1,&l1);
		*connect_fd2=accept(socket_fd,(struct sockaddr*)&client2,&l2);
		if (*connect_fd1==-1 || *connect_fd2 == -1){
				printf("Accept failed\n");
				exit(0);
			}
		printf("2 clients connected\n");
		int pid1=fork();
		if (pid1==-1){
			printf("Fork failed\n");
			exit(0);
			
		}
		if (pid1==0){
			while (1){
				char file_name[1024];
				memset(file_name,0,sizeof(file_name));
				read(*connect_fd1,file_name,sizeof(file_name));
				if (strlen(file_name)==0){
					write(*connect_fd2,file_name,sizeof(file_name));
					sleep(1);
					close(*connect_fd1);
					close(*connect_fd2);
					exit(0);
				}
				printf("Message received from client 1. Sending to client 2\n");
				write(*connect_fd2,file_name,sizeof(file_name));
				
			}
		}
		else {
			while (1){
				char file_name[1024];
				memset(file_name,0,sizeof(file_name));
				read(*connect_fd2,file_name,sizeof(file_name));
				if (strlen(file_name)==0){
					printf("Connection to clients lost\n");
					write(*connect_fd1,file_name,sizeof(file_name));
					sleep(1);
					close(*connect_fd1);
					close(*connect_fd2);				
					break;
				}
				printf("Message received from client 2. Sending to client 1\n");
				write(*connect_fd1,file_name,sizeof(file_name));
			}
		}
	}
	return 0;
} 
