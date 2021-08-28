Run the following commands to compile both client and the server: 
gcc -o c.out client.c
gcc -o s.out server.c


Then on one terminal run for server: ./s.out 8090(Run this first)
And on the other run for client: ./c.out 127.0.0.1 8090 

This will create a server with port number 8090 and the client will connect to it. 

The client can then send a message to the server. The server will receive the message and print a reversed message.
Then client waits as the server sends a new message. The client then reverses this message, prints and exits. 
The server waits for a new client to join. 

A sleep has been put before closing the file descriptors for the opened sockets on the server. 
This is to ensure that the server closes after the client. Without this, we get a binding error. 
