#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv) {
	char* server_name;
	char* string_name;
	char* protocol_name;
	const int server_port = 7;
	struct sockaddr_in server_address;
	int sock;
	int n = 0;
	int len = 0, maxlen = 100;
	char buffer[maxlen];
	char* pbuffer = buffer;
	struct timeval timeout;
	timeout.tv_sec=10;
	timeout.tv_usec=0;


	if (argc != 4) {
       		fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       		exit(0);
    	}
	string_name = argv[1];
	//printf("%s",string_name);
	server_name = argv[2];
	//printf("%s",server_name);
	protocol_name = argv[3];
	//printf("%s",protocol_name);	
	
	
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	inet_pton(AF_INET, server_name, &server_address.sin_addr);
	//servaddr.sin_addr.s_addr = INADDR_ANY;

	
	if(strcmp(protocol_name,"-tcp")==0)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) 
			{
				printf("could not create socket\n");
				return 1;
			}
		if (connect(sock, (struct sockaddr*)&server_address,
	            sizeof(server_address)) < 0) 
			{
				printf("could not connect to server\n");
				return 1;
			}
		if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
		{
			printf("Timeout Occoured");
		}
		
		if(setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
		{
			printf("Timeout Occoured");
		}


		n=send(sock, string_name, strlen(string_name), 0);
	
	
	
		while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) 
		{
			pbuffer += n;
			maxlen -= n;
			len += n;
			n=n-1;
			buffer[len] = '\0';
			printf("Server Responding '%s'\n", buffer);
			//close(sock);
		}

	// close the socket
	close(sock);
	return 0;

	}
	
	else if(strcmp(protocol_name,"-udp")==0)
	{
		sock = socket(AF_INET, SOCK_DGRAM,0);
		if (sock < 0) 
		{
			printf("could not create socket\n");
			return 1;
		}
		
		if(setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
		{
			printf("Timeout Occoured");
		}
		
		if(setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
		{
			printf("Timeout Occoured");
		}
		
		
		else
		{
		
			len = sendto(sock, string_name, strlen(string_name), 0,(struct sockaddr*)&server_address, sizeof(server_address));
 
          		char buffer[100];
			recvfrom(sock, buffer, len, 0, NULL, NULL);

			buffer[len] = '\0';	
			printf("recieved: '%s'\n", buffer);

			close(sock);
			return 0;
		} 
	}

	else{
		printf("Invalid Protocol");
			return 1;
	}

	

}
