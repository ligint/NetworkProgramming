    #include<stdio.h>
    #include<sys/types.h>//socket
    #include<sys/socket.h>//socket
    #include<string.h>//memset
    #include<stdlib.h>//sizeof
    #include<netinet/in.h>//INADDR_ANY
    #include<stdio.h>    
    #include<unistd.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <signal.h>

    #define MAXSZ 1024
		
	        //void signal_term(int signo);
          //void signal_int(int signo);
          //void signal_kill(int signo);
          //void signal_hup(int signo);
          //void signal_other(int signo);


          void sigchld_handler(int sig)
          {
            while (waitpid(-1, 0, WNOHANG) > 0);
            return;
          }

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
           int sockfd;//to create socket
           int newsockfd;//to accept connection
           int port_number;
           char* protocol_name;
           struct timeval timeout;
           timeout.tv_sec = 4;
           timeout.tv_usec = 0;
           fd_set Rds;

           struct sockaddr_in serverAddress;//server receive on this address
           struct sockaddr_in clientAddress;
           struct hostent *hostp;//server sends to client on this address

           int n,len;
           char msg[MAXSZ];
           int clientAddressLength;
           int pid;
           int optval;
           socklen_t sin_size;
           sin_size = sizeof(struct sockaddr);

           protocol_name = argv[1];
			
	   //signal (SIGHUP, signal_hup);
	   //signal (SIGINT, signal_int);
           //signal (SIGKILL, signal_kill);
	   //signal (SIGTERM, signal_term);
 


           //create socket
           //initialize the socket addresses
           
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
                  sockfd=socket(AF_INET,SOCK_STREAM,0);
                  if (sockfd < 0) 
                  {
                    printf("could not create socket\n");
                    return 1;
                  }
                  else{
                    printf("Socket Created Successfully\n");
                  }
                  optval=1;
                  setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(optval));
                  setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,(const void *)&optval,sizeof(optval));

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
                  signal(SIGCHLD, sigchld_handler);
                  signal(SIGCHLD, SIG_IGN);
                  while(1)
                  {
                    printf("\nServer waiting for new client connection:\n");
                    //clientAddressLength=sizeof(clientAddress);
                    newsockfd=accept(sockfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
                    if(newsockfd < 0)
                {
                  printf("Connection failed\n"); 
                  exit(0); 
                }
            else{
                    printf("connected to client: %s\n",inet_ntoa(clientAddress.sin_addr));
                    pid=fork();
                      if(pid==0)//child process rec and send
            {
             //rceive from client
               while(1)
               {
                n=recv(newsockfd,msg,MAXSZ,0);
                if(n==0)
                {
                 close(newsockfd);
                 break;
                }
                else if (n < 0){
                      printf("Not able to read from Socket");
                      exit(1);
                    }
                msg[n]='\0';
                uname(msg);
                
                send(newsockfd,msg,MAXSZ,0);
                close(newsockfd);
                printf("Receive and set:%s\n",msg);
               }//close interior while
              exit(0);
              }
              else
              {
               close(newsockfd);//sock is closed BY PARENT
              }


              }       
             }//close exterior while

           return 0;
          }


        else if (strcmp(protocol_name, "-udp") == 0){
                sockfd = socket(AF_INET, SOCK_DGRAM, 0);
                if (sockfd == -1) { 
                  printf("Socket creation failed\n"); 
                  exit(0); 
                } 
             
                optval=1;
                setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int));
                if (bind(sockfd, (struct sockaddr *)&serverAddress,sizeof(serverAddress)) < 0 ){ 
                  perror("Bind failed"); 
                  exit(0); 
                }
                getsockname(sockfd,(struct sockaddr *)&serverAddress, &size1);
                printf("Listening on Port: %hu\n", ntohs(serverAddress.sin_port));
                
          
                  while(1){ 
                  //bzero(msg,MAXSZ);  
                  len = recvfrom(sockfd, (char *)msg, MAXSZ, MSG_WAITALL, (struct sockaddr *)&clientAddress, &sin_size); 

                  if (len < 0){
                    printf("Not able to read from Socket");
                    return 1;
                  }
                  msg[len] = '\0';
                  signal(SIGCHLD, sigchld_handler);
                  signal(SIGCHLD, SIG_IGN);
                  printf("connected to client: %s\n",inet_ntoa(clientAddress.sin_addr));
                  //hostp = gethostbyaddr((const char *)&clientAddress.sin_addr.s_addr , sizeof(clientAddress.sin_addr.s_addr), AF_INET);
                  pid=fork();
                  
                  if(pid==0)
                  {

                  uname(msg);
                  printf("Message is %s",msg);

                  len = sendto(sockfd, msg, MAXSZ , 0, (struct sockaddr *)&clientAddress, sin_size);

                  if(len < 0){
                    printf("Unable to send the message \n");
                  }
                  close(sockfd);
              return 0;
                  
                  }
                }
                
                
              
              }

              
              

             }
