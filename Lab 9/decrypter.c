#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h> 
//DECRYPTER
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
 
int main(int argc, char * argv[]){
 
    if (argc!=4){
        printf("Invalid argument count\n");
    }
 
    FILE *f,*fout;
    f=fopen(argv[2],"r");
    fout=fopen(argv[3],"w");
    char encrypted[4096]; 
    int ind=0;
    fread(encrypted, 1,4096,f);
    char *privateKey=argv[1];
        
 
    unsigned char decrypted[4096];
 
    int decrypted_length = private_decrypt(encrypted,strlen(encrypted),privateKey, decrypted);
    if(decrypted_length == -1)
    {
        printLastError("Private Decrypt failed ");
        exit(0);
    }
    printf("Decrypted Text =\n%s\n",decrypted);
    printf("\nFile decrypted and stored in: %s\n",argv[3]);
 
    
    fwrite(decrypted,decrypted_length,1,fout);
    fclose(fout);
    fclose(f);
    
    return 0; 
    
}