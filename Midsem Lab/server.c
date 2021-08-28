#include <stdio.h> 
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include <netinet/in.h> 
#include <sys/types.h> 
#include<unistd.h>

void strrev(char* a){
	int n=strlen(a);
	for (int i=0;i<n/2;i++){
		char temp=a[i];
		a[i]=a[n-i-1];
		a[n-i-1]=temp;
	}
}

int main(int argc, char* argv[]) 
{ 	
	
	if (argc!=2){
		printf("Invalid argument count\n");
		exit(0);
	}
	int port_num=atoi(argv[1]);
	int socket_fd;
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
		
		char file_name[1000];
		memset(file_name,0,sizeof(file_name));
		
		read(connect_fd,file_name,sizeof(file_name));
		printf("\nThe string received from client:%s",file_name);
		strrev(file_name);
		printf("The reversed string:%s\n\n",file_name);
		
		char file_content[1000];
		memset(file_content,0,sizeof(file_content));
		printf("Enter string to send to client:");
		fgets(file_content,sizeof(file_content),stdin);
		write(connect_fd,file_content,strlen(file_content));
		
		sleep(1);
		close(connect_fd);
		close(socket_fd);
	}	
	return 0;
} 
