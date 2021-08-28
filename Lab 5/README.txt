Run the following commands to compile both client and the server: 
gcc -o c.out client.c
gcc -o s.out server.c


Then on one terminal run for server: ./s.out (Run this first)
And on the other run for client: ./c.out 

Enter the port number for the server first
Once it is running, enter the ip address, port number and file name for the client 
The client will connect to the server and get the corresponding file (empty case handled)

A sleep has been put before closing the file descriptors for the opened sockets on the server. 
This is to ensure that the server closes after the client. Without this, we get a binding error. 

A sleep has also been put after taking the file name input in the client. 
This is to ensure that the server is up and running after closing the previous connection. 

All inputs including file name are taken before connecting to the server.
So if the server exits midway, the connection will fail directly. 

In the message the server sends back, the first byte is reserved for telling if the file exists. 
If it does not, then we print a message saying empty file and create empty file. 
Else we write the contents from the socket into the file. 
