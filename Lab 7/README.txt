Run the following command to compile the client: 
gcc client.c -lssl -lcrypto

To execute run the following command 
./a.out url 

where url can be the url of the file specifcied. It can also be a webpage

Following examples were used for testing: 
http://wwww.google.com 
https://www.google.com 
https://www.gstatic.com/webp/gallery/1.jpg
https://mk0nationaltodayijln.kinstacdn.com/wp-content/uploads/2020/08/superhero-flying-48-1000x834.jpg
http://goidirectory.nic.in/images/logo.png
https://web.mst.edu/~huwen/teaching_Predator_Prey_Tyler_Blaszak.pdf

Note that if the URL has & in it, the entire string should be given in quotes otherwise the command line argument terminates abruptly 

The code for OpenSSL was referred to from the following link:https://gist.github.com/nevesnunes/eb5fa19e056109319f1d99f220f5647f

The client sends an HTTP / HTTPS GET message to the server, receives the response and stores the content in the entity body (the actual file content/payload) in a file
It also prints the received content in the terminal window (including the header)
After this the client exits 

Also, occassionally the server does not send data the first time and the program exits with an appropriate error code. However, re running the program with the same url with get the correct file. 
