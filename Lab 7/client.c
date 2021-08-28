#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
 #include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>

 void strrev(char* a){
	int n=strlen(a);
	for (int i=0;i<n/2;i++){
		char temp=a[i];
		a[i]=a[n-i-1];
		a[n-i-1]=temp;
	}
}
 
int main(int argc, char* argv[1024]){
	
	
	if (argc!=2){
		printf("Invalid argument count\n");
		exit(0);
	}
	
	
	
	char http_or_https[1024];
	char host_name[1024];
	int j=0,sc=0;
	int i ;
	char file_path[1024];
	char file_name[1024]="op.html";
	
	file_path[0]='\0';
	
	for ( i=0;i<strlen(argv[1]);i++){
		if (sc>=3){
			file_path[j++]=argv[1][i];
		}
		else if(argv[1][i]!='/' && sc==0){
			
			http_or_https[j++]=argv[1][i];
		
		}
		else if (argv[1][i]!='/' && sc==2)
			host_name[j++]=argv[1][i];
		else {
			if (sc==0){
				http_or_https[j++]='\0';
				
			}
			else if (sc==2){
				host_name[j++]='\0';
				
			}
			sc++;
			j=0;
			
		}
	}
	
	file_path[j++]='\0';
	
	
 
	char msg[1024]="GET /";
	int flag=0;
		
	
	if (strlen(file_path)>0){
		strcat(msg,file_path);
		
		int temp=strlen(file_path)-1;
		if (file_path[temp]=='/')
			temp--;
			
		int startInd=temp;
		while(file_path[temp]!='/'){
			if (file_path[temp]=='.'){
				flag=1;
				break;
			}
			temp--;
		}
		
		if (flag){
			temp=startInd;
			memset(file_name,0,sizeof(file_name));
			int j1=0;
			while(file_path[temp]!='/')
				file_name[j1++]=file_path[temp--];
			file_name[j++]='\0';
			strrev(file_name);
			if (!strcmp(file_name,host_name)){
				strcpy(file_name,"op.html");
			}
		}
	}
	
	strcat(msg," HTTP/1.0\r\nHost: ");
	strcat(msg,host_name);
	strcat(msg,"\r\n");
	
	if (!strcmp(http_or_https,"http:")){
		strcat(msg,"\r\n");
		int port_num=80;
		struct hostent *server; 
		int socket_fd;
		struct sockaddr_in server_address;
		socket_fd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (socket_fd==-1){
			printf("Socket creation failed\n");
			exit(0);
		}
			
		printf("\nSocket creation successful\n");
		
		
		server = gethostbyname(host_name) ;
		if (server==NULL) {
			printf("No such host\n");
			exit(0);
		} 
		
		
		memset(&server_address,0,sizeof(server_address));
		server_address.sin_family=AF_INET;
		memcpy(&server_address.sin_addr.s_addr,server->h_addr,server->h_length);
		server_address.sin_port=htons(port_num);
		
		int connection=connect(socket_fd,(struct sockaddr*)&server_address, sizeof(server_address));
		if (connection==-1){
			printf("Connection to server failed\n");
			exit(0);
		}
			
		printf("Connection successful\n");
		write(socket_fd,msg,strlen(msg));
				
		char req_file[4096]="";
		memset(req_file,0,sizeof(req_file));
		printf("Received content:\n");
		int b=0;
 
		int j1=3;
		do{
			
			b=recv(socket_fd,&req_file[j1],1,0);		
			printf ("%c",req_file[j1]);
			if (b==-1){
				printf("\nError reading content. Exiting...\n");
				exit (0);
			}
			if (req_file[j1]=='\n' && req_file[j1-1]=='\r' && req_file[j1-2]=='\n' && req_file[j1-3]=='\r'){
				break;
			}
			
			j1++;	
			
				
		}while (b>0);//this loop is simply to clear the entity boydy 
		
		FILE* f=fopen(file_name,"wb");
		memset(req_file,0,sizeof(req_file));
		do{
			
			b=recv(socket_fd,req_file,4096,0);		
			
			if (b==-1){
				printf("\nError reading content. Exiting...\n");
				exit (0);
			}
			
			fwrite(req_file,1,b,f);
			printf("%s", req_file);
				
		}while (b>0);
		
		
		fclose (f);
		close(socket_fd);
	
	}
	
	
	else if (!strcmp(http_or_https,"https:")){
		    strcat(msg,"Connection: Close\r\n\r\n");
		    SSL_CTX *ctx = SSL_CTX_new(TLSv1_2_client_method());
		    if (ctx == NULL) {
			printf("errored; unable to load context.\n");
			ERR_print_errors_fp(stderr);
			return -3;
		    }
		    char host_name_https[1000];
		    strcpy(host_name_https,host_name);
		    strcat (host_name_https,":https");
		    BIO *bio = BIO_new_ssl_connect(ctx);
		    
		    SSL *ssl;
		    BIO_get_ssl(bio, &ssl);
		    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
		    
		    BIO_set_conn_hostname(bio, host_name_https);
		    
		    if (BIO_do_connect(bio) <= 0) {
			BIO_free_all(bio);
			printf("errored; unable to connect.\n");
			ERR_print_errors_fp(stderr);
			return -2;
		    }
		    
		    const char *request = msg;
		    
		    if (BIO_puts(bio, request) <= 0) {
			BIO_free_all(bio);
			printf("errored; unable to write.\n");
			ERR_print_errors_fp(stderr);
			return -1;
		    }
		    
		    char req_file[4096]="";
		    
		    memset(req_file,0,sizeof(req_file));
		    printf("Received content:\n");
		    int b=0;
 
		    int j1=3;
		    do{
				
			b=BIO_read(bio,&req_file[j1],1);		
			printf ("%c",req_file[j1]);
			if (b==-1){
				printf("\nError reading content. Exiting...\n");
				exit (0);
			}
			if (req_file[j1]=='\n' && req_file[j1-1]=='\r' && req_file[j1-2]=='\n' && req_file[j1-3]=='\r'){
				break;
			}
			
			j1++;	
				
					
		    }while (b>0);//this loop is simply to clear the entity body 
		    remove(file_name);
		    FILE* f=fopen(file_name,"wb");
		    
		    memset(req_file,0,sizeof(req_file));
		    do{
			
			b=BIO_read(bio,req_file,4096);		
			
			if (b==-1){
				printf("\nError reading content. Exiting...\n");
				exit (0);
			}
			if (b==0)
				break;
			
			fwrite(req_file,1,b,f);
			printf("%s", req_file);
				
		    }while (b>0);
		    
		    
		    BIO_free_all(bio);
		    SSL_CTX_free(ctx);
		    fclose(f);
	}
	
	else {
		printf("Invalid URL\n");
		exit(0);
	}
	printf("\n\nClient exit\n");
	return 0; 
}
