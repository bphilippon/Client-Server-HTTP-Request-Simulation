#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc != 2)	//check fir invalid arguments
        {
                printf("Error: insufficient arguments\n");
                return 1;
        }

    	char str[40960], request[40960], webResponse[40960];
    	int listen_fd, conn_fd, web_fd, portnum;
    	struct sockaddr_in servaddr;

	//getaddrinfo() components
	struct addrinfo hints, *res, *r;

	//Buffers for web server
	char url[100], host[100], path[100];

    	/* AF_INET - IPv4 IP , Type of socket, protocol*/
    	listen_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Zeroing the bits in the buffer
    	bzero(&servaddr, sizeof(servaddr));
	bzero(webResponse, sizeof(webResponse));

	//Records port number upon execution
	portnum = atoi(argv[1]);

    	servaddr.sin_family = AF_INET;
    	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    	servaddr.sin_port = htons(portnum); /* Pick another port number to avoid conflict with other students */

    	/* Binds the above details to the socket */
	bind(listen_fd,  (struct sockaddr *) &servaddr, sizeof(servaddr));

	/* Start listening to incoming connections */
	listen(listen_fd, 10);

   	while(1)
    	{
      		/* Accepts an incoming connection */
	  	conn_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);

		//Further zeroing the buffers
      		bzero(str, sizeof(str));
		bzero(request, sizeof(request));
		bzero(url, sizeof(url));
		bzero(host, sizeof(host));
		bzero(path, sizeof(path));

		//Read incoming URL
		read(conn_fd, url, sizeof(url), 0);

		//Scan necessary components of HTTP request
		sscanf(url, "http://%99[^/]/%s", host, path);

		//Initialize hints for getaddrinfo()
		memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_STREAM;
                getaddrinfo(host, "80", &hints, &res);

		for(r = res; r != NULL; r = r->ai_next)
		{
			//Create socket for web server
			web_fd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);

			if(web_fd == -1) 	//if socket fails, proceed to next host address
				continue;

			//if host aligns
			if(connect(web_fd, r->ai_addr, r->ai_addrlen) != -1)
			{
				//Create HTTP request
				snprintf(request, sizeof(request),
                        		"GET /%s HTTP/1.1\r\n"
                        		"Host: %s\r\n"
                        		"Accept: text/html\r\n"
					"Connection: close\r\n"
					"\r\n\0", path, host);

				//Send request and receive webpage contents, copy to buffer
				send(web_fd, request, strlen(request), 0);
				recv(web_fd, webResponse, sizeof(webResponse), MSG_WAITALL);
				strcpy(str, webResponse);

				break;
			}
			close(web_fd);	//close the connection
		}
                freeaddrinfo(res);

		write(conn_fd, str, strlen(str));       // write to the client
                close (conn_fd);                        //close the connection
    	}
}
