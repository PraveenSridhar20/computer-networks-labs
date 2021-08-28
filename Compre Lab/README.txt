To generate the private and the public keys we use the following commands: 
openssl genrsa -out pr1.pem 8192
openssl rsa -in pr1.pem -outform PEM -pubout -out pu1.pem
openssl genrsa -out pr2.pem 8192
openssl rsa -in pr2.pem -outform PEM -pubout -out pu2.pem

pr1 and pr2 are private keys. pu1 and pu2 are public keys. 

To compile the client and the server, run the following commands: 

gcc server.c -o s
gcc client.c -o c -lcrypto

Start the server first: 
./s 8090

The server waits for 2 clients to connect. To run the clients: 
./c 127.0.0.1 8090 pr1.pem pu2.pem
./c 127.0.0.1 8090 pr2.pem pu1.pem

Now the 2 clients can exchange encrypted messages to each other. 
Should either of the clients type exit (or press ctrl C), the other client leaves as well and the server shows a message saying the connection is lost. 
Then it waits for 2 new clients to connect again. 

If the server exits, both the clients exit as well. 

Note: Reference for the code using openssl: http://hayageek.com/rsa-encryption-decryption-openssl-c/