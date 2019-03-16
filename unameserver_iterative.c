			#include<stdio.h>
			#include<sys/types.h>
			#include<sys/socket.h>
			#include<string.h>
			#include<stdlib.h>
			#include<netinet/in.h>
			#include<unistd.h>

			//#define PORT 8000
			#define MAXSZ 1024

			char* uname(char* buffer){


				char uname[100] = {'u', 'n', 'a', 'm', 'e', ' ', '-'};
			    char cap[100] = {' ', '2', '>', '&', '1'};
			    int status;

				strcat(uname, buffer);
			    strcat(uname, cap);
			    
			    bzero(buffer, MAXSZ);

			    FILE *in = popen(uname, "r");
			    if (in == NULL){
			    	perror("popen: ");
			    	exit(EXIT_FAILURE);
			    }
			    fgets(buffer, MAXSZ, in);

			    status = pclose(in);
			    if (status == -1) {
			    	perror("pclose: ");
			    	exit(EXIT_FAILURE);

			    return (char *)buffer;
			    }
			}



			int main(int argc, char **argv)
			{
			 int sockfd,newsockfd,n,len;//to accept connection
			 char* protocol_name;
			 int port_number;
			 struct sockaddr_in serverAddress;//server receive on this address
			 struct sockaddr_in clientAddress;
			 char msg[MAXSZ];
			 int clientAddressLength;
			 struct timeval timeout;
			 timeout.tv_sec = 4;
			 timeout.tv_usec = 0;
			 fd_set Rds;

			 protocol_name = argv[1];

			 memset(&serverAddress,0,sizeof(serverAddress));
			 serverAddress.sin_family=AF_INET;
			 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
			 socklen_t size1 =sizeof(serverAddress);
			 clientAddressLength=sizeof(clientAddress);
			 if(argv[2]!=NULL && strcmp(argv[2],"-port")!=0)
			 {
			 	port_number=atoi(argv[2]);
			 	printf("port_number %d\n",port_number);
			 	serverAddress.sin_port=htons(port_number);
			 	//printf("2\n");

			 }
			 else if(argv[2]==NULL || argv[3]==NULL)
			 {
			 	serverAddress.sin_port=htons(port_number);
			 	//getsockname(sockfd,(struct sockaddr *)&serverAddress, &size1);

			 }
			 else if(argv[3]!=NULL)
			 {
			 	port_number=atoi(argv[3]);
			 	serverAddress.sin_port=htons(port_number);
			 	//printf("3\n");

			 }
			 else {
			 	printf("Invalid Port Number");
			 	return 1;
			 }


			 

			 if(strcmp(protocol_name,"-tcp")==0)
			 {
				 //create socket
			 	sockfd=socket(AF_INET,SOCK_STREAM,0);
					 //initialize the socket addresses
			 	if (sockfd < 0) 
			 	{
			 		printf("could not create socket\n");
			 		return 1;
			 	}
			 	else{
			 		printf("Socket Created Successfully\n");
			 	}	
			 	//bzero((char *) &serverAddress, sizeof(serverAddress));
			 	
					 //bind the socket with the server address and port
			 	if((bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress)))!=0)
			 	{
			 		printf("socket bind failed\n"); 
			 		exit(0);

			 	}

			 	else
			 	{
			 		printf("Socket binded successfully\n");
			 	}
			 	if(getsockname(sockfd,(struct sockaddr *)&serverAddress, &size1)==-1)
			 	{
			 		printf("Error in getsockname \n");
			 	}

			 	if ((listen(sockfd, 5)) != 0) { 
			 		printf("Listen failed\n"); 
			 		exit(0); 
			 	}	
			 	else{
			 		//getsockname(sockfd,(struct sockaddr *)&serverAddress, &size1);
			 		printf("listening on Port: %hu\n", ntohs(serverAddress.sin_port)); 
			 		
			 	}
					 //printf("Listening on port: %d\n",serverAddress.sin_port);
					 //accept a connection
			 	while(1)
			 	{


			 		newsockfd=accept(sockfd, (struct sockaddr*)&clientAddress, &clientAddressLength);
			 		if(newsockfd < 0)
			 		{
			 			printf("Connection failed\n"); 
			 			exit(0); 
			 		}
						  //rceive from client
			 		else{
			 			printf("Connected\n");

			 			bzero(msg,MAXSZ); 

					        // read the message from client and copy it in buffer check
			 			FD_ZERO(&Rds);
			 			FD_SET(newsockfd, &Rds);

			 			int t = select(newsockfd + 1, &Rds, NULL, NULL, &timeout);
			 			if (t == 0){
			 				fprintf(stderr, "Resopnse timeout\n");
			 				exit(1);
			 			}else{
			 				bzero(msg,MAXSZ);
			 				n=read(newsockfd,msg,MAXSZ);
			 				if (n < 0){
			 					printf("Not able to read from Socket");
			 					exit(1);
			 				}	

			 				uname(msg);
			 				//msg[n]=0;
			 				n=write(newsockfd,msg,MAXSZ);
					  	    /*if (n < 0){
					      		error("ERROR! Not able to write to socket");*/
			 			}
			 			close(newsockfd);
			 		} 
			 		
			 	}
			 }


			 	else if (strcmp(protocol_name, "-udp") == 0){
			 		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
			 		if (sockfd == -1) { 
			 			printf("Socket creation failed\n"); 
			 			exit(0); 
			 		} 
			 

			 		if (bind(sockfd, (struct sockaddr *)&serverAddress,sizeof(serverAddress)) < 0 ){ 
			 			perror("Bind failed"); 
			 			exit(0); 
			 		}
			 		getsockname(sockfd,(struct sockaddr *)&serverAddress, &size1);
			 		printf("Listening on Port: %hu\n", ntohs(serverAddress.sin_port));
			 		
			 		while(1){ 
			 			len = recvfrom(sockfd, (char *)msg, MAXSZ, MSG_WAITALL, (struct sockaddr *)&clientAddress, &clientAddressLength); 
			 			if (n < 0){
			 				printf("Not able to read from Socket");
			 				return 1;
			 			}
			 			msg[len] = '\0';

			 			uname(msg);

			 			len = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&clientAddress, clientAddressLength);

			 			if(n < 0){
			 				printf("Unable to send the message \n");
			 			}
			 		}
			 		
			 	
			 	}
			 	close(sockfd);
			 	return 0;

			 }	


			