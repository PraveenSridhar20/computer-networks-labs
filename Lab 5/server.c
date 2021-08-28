#include <stdio.h> 
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include <netinet/in.h> 
#include <sys/types.h> 
#include<unistd.h>

int main() 
{ 	
	int port_num;
	int socket_fd;
	printf("Enter port number:");
	scanf("%d",&port_num);
	while(1){
		struct sockaddr_in server_address,client;
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
		int l=sizeof(client);
		int connect_fd=accept(socket_fd,(struct sockaddr*)&client,&l);
		
		if (connect_fd==-1){
			printf("Accept failed\n");
			exit(0);
		}
		
		printf("Accept successful\n");
		
		char file_name[100];
		memset(file_name,0,sizeof(file_name));
		
		read(connect_fd,file_name,sizeof(file_name));
		printf("\nThe file name received from client:%s\n",file_name);
		
		int fd=open(file_name,O_RDONLY);
		if (fd<0){
			printf("The requested file was not found\n");
			write(connect_fd,"d",10);
			
		}
		else{
			char file_content[100];
			memset(file_content,0,sizeof(file_content));
			read(fd,file_content,10);
			char mod_file_content[2]="e";
			strcat(mod_file_content,file_content);
			printf("File contents sent to client:%s\n",file_content);
			write(connect_fd,mod_file_content,11);
			close(fd);
		}
		sleep(1);
		close(connect_fd);
		close(socket_fd);
	}	
	return 0;
} 
