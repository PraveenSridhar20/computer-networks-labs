Run the following commands to compile both client and the server: 
gcc -o c.out client.c
gcc -o s.out server.c

Then on one terminal run for server: ./s.out 8090(Run this first)
And on the other run for client: ./c.out 127.0.0.1 8090 

This will create a server with port number 8090 and the client will connect to it. 
You can re run the client command multiple times to have multiple clients joining in parallel. 

Once after 4 clients join, the server will reject the connection
It first connects, and if the count is greater than 4, closes it immediately.
The client would be waiting for input because the process would have gone for IO. 
But after input it received, it would print that the connection was lost because the server closed the connection. 

To implement multiple clients joining in parallel, we use a fork to create multiple child processes 
The parent process is the server which is ready to accept a new connection 
Each child process is responsible for handling one connection (ie receiving string from client, reversing, sending message to client)

***When multiple clients have sent a message and the server must respond to all of them, the implementation used here uses a fifo policy
The first client to send a message is the first client to which the server will send the response message. ***

for example, if c1 and c2 are connected to the server and c1 sends h1 as the message and c2 sends h2 as the message 
Now the server is waiting for input. Let us say the server types hs1 as the message, this message will go to c1 as c1 sent the message first
Then the next message, say hs2 will go to the c2. 

The client can then send a message to the server. The server will receive the message and print a reversed message.
Then client waits as the server sends a new message. The client then reverses this message and prints it. 
After this, the connection is closed and a new connection is established for the same client. Then it is ready to send a new message. 

If a client exits and the count is less than 4, the server is ready to accept a new client

