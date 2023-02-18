#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv)
{
	if(argc != 2)	//check for invalid arguments
        {
                printf("Error: insufficient arguments\n");
                return 1;
        }

    	int sockfd, n, portnum;
    	int len = sizeof(struct sockaddr);
    	char recvline[40960], url[100];
    	struct sockaddr_in servaddr;

    	/* AF_INET - IPv4 IP , Type of socket, protocol*/
    	sockfd=socket(AF_INET, SOCK_STREAM, 0);
    	bzero(&servaddr,sizeof(servaddr));

	//Stores port number upon execution
	portnum = atoi(argv[1]);

    	servaddr.sin_family=AF_INET;
    	servaddr.sin_port=htons(portnum); // Server port number

    	/* Convert IPv4 and IPv6 addresses from text to binary form */
	inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));

    	/* Connect to the server */
    	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

	//Prompt for url
	printf("url: ");
	scanf("%s", &url);

	//Send to proxy server
	write(sockfd, url, strlen(url));

    	while(n = read(sockfd, recvline, sizeof(recvline)) > 0)
    	{
        	printf("%s",recvline); // print the received text from server
    	}
}
