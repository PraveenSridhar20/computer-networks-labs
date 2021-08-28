#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h> 

int padding = RSA_PKCS1_PADDING;
 
RSA * createRSA(unsigned char * key,int public)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    
    if (keybio==NULL)
    {
        
        printf( "Failed to create key BIO");
        exit(0);
        return 0;
    }
    if(public)
    {
        
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        printf( "Failed to create RSA");
    }
 
    return rsa;
}
 
int public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    int result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}

void printLastError(char *msg)
{
    char * err = malloc(130);
    ERR_load_crypto_strings();
    ERR_error_string(ERR_get_error(), err);
    printf("%s ERROR: %s\n",msg, err);
    free(err);
}
 
int main(int argc,char* argv[]){
    if (argc!=4){
        printf("Invalid argument count\n");
        return 0;
    }
    FILE *f,*fout;
    f=fopen(argv[2],"r");
    fout=fopen(argv[3],"w");
    char str[1024]; 
    fread(str, 1,1024,f);   

    char* publicKey=argv[1];
    unsigned char  encrypted[2048];
    int encrypted_length= public_encrypt(str,strlen(str),publicKey,encrypted);

    if(encrypted_length == -1)
    {
        printLastError("Public Encrypt failed ");
        exit(0);
    }
    printf("Plain text input file read:\n%s\n",str);
    printf("\nFile encrypted and stored in: %s\n",argv[3]);
    fwrite(encrypted,encrypted_length,1,fout);

    fclose(fout);
    fclose(f);
    return 0;
 
}