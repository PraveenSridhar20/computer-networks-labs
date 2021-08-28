#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
int main(){
	
	
	int port_num;
	char server_ip_address[100];
	printf("\nEnter server IP address:");
	scanf("%s",server_ip_address);
	printf("Enter port number:");
	scanf("%d",&port_num);
	while (1){
		
		char req_file_name[100];
		printf("\nEnter file name:");
		while (getchar()!='\n');//clear any new lines in input 
		scanf("%[^\n]s",req_file_name);
		sleep(1);
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
		write(socket_fd,req_file_name,100);
		
		char req_file[100];
		memset(req_file,0,100);
		read(socket_fd,req_file,100);
		if (strlen(req_file)==0){
			printf("No content received, connection to server lost\n");
			exit (0);
		}
		else{
			if (req_file[0]=='d'){
				printf("Empty file received\n");
				FILE* f=fopen(req_file_name,"w"); 
				fclose(f);
			}
			else{
				int n=strlen(req_file);
				for (int i=0;i<n;i++){
					req_file[i]=req_file[i+1];
				}//for removing the first byte 
				printf("File contents:%s\n",req_file);
				FILE* f=fopen(req_file_name,"w"); 
				fprintf(f,"%s",req_file);
				fclose(f);
			}
		}
		close(socket_fd);
		}
	return 0; 
}
