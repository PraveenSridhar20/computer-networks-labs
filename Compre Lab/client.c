#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<netdb.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<signal.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>

int padding = RSA_PKCS1_PADDING;

RSA * createRSA(unsigned char * key,int isPublic)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        printf( "Failed to create BIO of key");
        return 0;
    }
    if(isPublic)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        printf( "Failed to create RSA structure info");
    }

    return rsa;
}


int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}

int private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,0);
    int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}

void printLastError(char *msg)
{
    char * err = malloc(130);;
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), err);
    printf("%s ERROR: %s\n",msg, err);
    free(err);
}

int main(int argc, char* argv[]){
	
	
	if (argc!=5){
		printf("Invalid argument count\n");
		exit(0);
	}
	
	int port_num=atoi(argv[2]);
	char server_ip_address[100];
	strcpy(server_ip_address,argv[1]);

    int encrypted_length;
    int decrypted_length;
    char publicKey[8192];
    char privateKey[8192];
    unsigned char decrypted[1024]={};
    unsigned char encrypted[1024]={};
    int size;


	FILE* fp =fopen(argv[4],"r");
	if(fp==NULL)
    {
        printf("First create a public key\n");
        exit(0);
    }	
    fseek(fp,0,SEEK_END);
    size= ftell(fp);
    fseek(fp,0,SEEK_SET);
    fread(publicKey,size,1,fp);
    fclose(fp);
    fp = fopen(argv[3],"r");
    if(fp==NULL)
    {
        printf("First create a private key file\n");
        exit(0);
    }
    fseek(fp,0,SEEK_END);
    size= ftell(fp);
    fseek(fp,0,SEEK_SET);
    fread(privateKey,size,1,fp);
    fclose(fp);

	int socket_fd;
	struct sockaddr_in server_address;
	while (1){
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
        int pid1=fork();
        if (pid1==-1){
            printf("Fork failed\n");
            exit(0);
        }
        if (pid1!=0){
            while (1){
                char req_file_name[1024];
                
                printf("\n>>>");
                fgets(req_file_name,sizeof(req_file_name),stdin);
                encrypted_length = public_encrypt(req_file_name,strlen(req_file_name),publicKey,encrypted);
                
                if(encrypted_length == -1)
                {
                    printLastError("Private Encrypt failed");
                    kill(pid1,SIGKILL);
                    exit(0);
                }

                if (!strcmp(req_file_name,"exit\n")){
                    printf("Client exit\n");
                    kill(pid1,SIGKILL);
                    close(socket_fd);
                    exit(0);
                }
                
                write(socket_fd,encrypted,encrypted_length);

            }
        }
		
        else{
            while (1){
                char req_file[1026];
                memset(decrypted,0,sizeof(decrypted));
                memset(req_file,0,sizeof(req_file));
                read(socket_fd,req_file,sizeof(req_file));
                encrypted_length=1024;
                if (strlen(req_file)==0){
                    printf("No content received, connection to server lost\n");
                    kill(getppid(),SIGKILL);
                    exit(0);
                }
                else{
                    
                    printf("\nEncrypted message:\n<<<%s\n\n",req_file);
                    decrypted_length = private_decrypt(req_file,encrypted_length,privateKey, decrypted);
                    if(decrypted_length == -1)
                    {
                        printLastError("Public Decrypt failed");
                        kill(getppid(),SIGKILL);
                        exit(0);
                    }
                    printf("Decrypted message:\n<<%s\n",decrypted);
                    printf("\n>>>");
                    fflush(stdout);
                            
                        
                }
            }
        }
		close(socket_fd);
	}
	
		
	return 0; 
}
