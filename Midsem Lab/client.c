#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>

void strrev(char* a){
	int n=strlen(a);
	for (int i=0;i<n/2;i++){
		char temp=a[i];
		a[i]=a[n-i-1];
		a[n-i-1]=temp;
	}
}

int main(int argc, char* argv[]){
	
	
	if (argc!=3){
		printf("Invalid argument count\n");
		exit(0);
	}
	
	int port_num=atoi(argv[2]);
	char server_ip_address[100];
	strcpy(server_ip_address,argv[1]);

	
		
	int socket_fd;
	struct sockaddr_in server_address;
	socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (socket_fd==-1){
		printf("Socket creation failed\n");
		exit(0);
	}
		
	printf("\nSocket creation successful\n");
		
	memset(&server_address,0,sizeof(server_address));
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=inet_addr(server_ip_address);
	server_address.sin_port=htons(port_num);
		
	int connection=connect(socket_fd,(struct sockaddr*)&server_address, sizeof(server_address));
	if (connection==-1){
		printf("Connection to server failed\n");
		exit(0);
	}
		
	printf("Connection successful\n");
	char req_file_name[1000];
	printf("\nEnter contents to send to server:");
	fgets(req_file_name,sizeof(req_file_name),stdin);
	write(socket_fd,req_file_name,strlen(req_file_name));
		
	printf("Waiting for server response...\n");
	char req_file[1000];
	memset(req_file,0,sizeof(req_file));
	read(socket_fd,req_file,sizeof(req_file));
	if (strlen(req_file)==0){
		printf("No content received, connection to server lost\n");
		exit (0);
	}
	else{
		
		printf("Contents received from server:%s\n",req_file);
		strrev(req_file);
		printf("The reversed string:%s\n",req_file);
				
			
	}
	close(socket_fd);
		
	return 0; 
}
