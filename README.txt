*** HOW TO COMPILE ***

gcc server.c -o pserver

gcc client.c -o client 

*** HOW TO RUN ***

./pserver <portno>

./client <portno>

*** HOW TO USE ***

When the program is executed, the user is prompted to enter a URL on the client side.
The user must enter a URL beginning with "http://". The user must also provide a sufficient
port number in range for valid connection. The URL will then be sent to the proxy server
which will send a GET request to the original host, and the proxy server will send the contents
of the webpage back the client to display.
