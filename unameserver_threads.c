  #include<stdio.h>
  #include<stdlib.h>
  #include<sys/socket.h>
  #include<netinet/in.h>
  #include<string.h>
  #include <arpa/inet.h>
  #include <fcntl.h> // for open
  #include <unistd.h> // for close
  #include<pthread.h>
  //#include "thread.h"
  #define MAXSZ 1024
  #define MAX_THREADS 32767


char msg[MAXSZ];
int sd = 0;
void * worker(void *);
void * socketThread(void *);



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
    printf("Message is %s",buffer);

    return (char *)buffer;
  }
}


int main(int argc, char **argv)
{
             int sockfd;//to create socket
             int newSocket;//to accept connection
             int port_number;
             char* protocol_name;
             struct timeval timeout;
             timeout.tv_sec = 4;
             timeout.tv_usec = 0;
             fd_set Rds;

             struct sockaddr_in serverAddress;//server receive on this address
             struct sockaddr_in clientAddress;
             struct hostent *hostp;
             struct sockaddr_storage serverStorage;//server sends to client on this address

             int n,len;
             char msg[MAXSZ];
             int clientAddressLength;
             int pid;
             int optval;
             socklen_t addr_size;
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

              if(listen(sockfd,50)==0)
              {

                printf("Listening\n");
              }

              else
              {
                printf("Error Listening\n");
                return 1;
              }



              pthread_t tid[60];

              int i = 0;
              while(1)
              {

                        //Accept call creates a new socket for the incoming connection

                addr_size = sizeof serverStorage;

                newSocket = accept(sockfd, (struct sockaddr *) &serverStorage, &addr_size);

          //for each client request creates a thread and assign the client request to it to process

         //so the main thread can entertain next request

                if( pthread_create(&tid[i], NULL, socketThread, &newSocket) != 0 )

                 printf("Failed to create thread\n");

              if( i >= 50)

              {

                i = 0;

                while(i < 50)

                {

                  pthread_join(tid[i++],NULL);

                }

                i = 0;

              }

                 }//close exterior while

                 return 0;
               }


               else if (strcmp(protocol_name, "-udp") == 0){
                
            sd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sd == -1) { 
          printf("Socket creation failed\n"); 
          exit(0); 
        } 

        optval=1;
        setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int));
        if (bind(sd, (struct sockaddr *)&serverAddress,sizeof(serverAddress)) < 0 ){ 
          perror("Bind failed"); 
          exit(0); 
        }
        getsockname(sd,(struct sockaddr *)&serverAddress, &size1);
        printf("Listening on Port: %hu\n", ntohs(serverAddress.sin_port));
        pthread_t threads[MAX_THREADS];
        

        int thread_no=0;

        while(1){ 
              
              len = recvfrom(sd, (char *)msg, MAXSZ, MSG_WAITALL, (struct sockaddr *)&clientAddress, &sin_size); 

          if (len < 0){
            printf("Not able to read from Socket");
            return 1;
          }
          msg[len] = '\0';
          int thread_no = 0;
          printf("connected to client: %s\n",inet_ntoa(clientAddress.sin_addr));
                            //hostp = gethostbyaddr((const char *)&clientAddress.sin_addr.s_addr , sizeof(clientAddress.sin_addr.s_addr), AF_INET);

          /*rc = pthread_create(&threads[thread_no], NULL, socketThreadUDP, sockfd);
          if(rc){
            printf("A request could not be processed\n");
          }
          else{
            thread_no++;
          }*/
          //uname(msg);
          //printf("Message is %s",msg);

          //len = sendto(sockfd, msg, MAXSZ , 0, (struct sockaddr *)&clientAddress, sin_size);
          pthread_t thread;
        pthread_create(&thread, NULL, worker, (void*) &clientAddress);
        pthread_join(thread, NULL);

          if(len < 0){
            printf("Unable to send the message \n");
          }


        }//
                            return 0;
                          }//else
                          
                          

                        }

                
void * socketThread(void *arg)

{

  int newSocket = *((int *)arg);
  int n;
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

  recv(newSocket , msg , MAXSZ , 0);
  printf("inside sock thread %s\n", msg);

      // Send message to the client socket 

  pthread_mutex_lock(&lock);

  //msg[n]='\0';
  uname(msg);

  pthread_mutex_unlock(&lock);

  sleep(1);

  send(newSocket,msg,MAXSZ,0);
  printf("%s",msg);

  printf("Exit socketThread \n");

  close(newSocket);

  pthread_exit(NULL);

}   

void *worker(void *arg)
{
    struct sockaddr_in address;
    socklen_t  addressLen = sizeof(address);
    //char msg[MAXSZ];

    //printf("%s(): sd = %d\n", __func__, sd);
    ssize_t n = recvfrom(sd, msg, MAXSZ, 0, (struct sockaddr*) &address, &addressLen);
    printf("received message is %s ",msg);
    if (n < 0)
    {
      printf("recv from thread failed");
       // err_syserr("recvfrom() (worker) failed: ");
    }
    else if (n == 0)
    {
      printf(" thread completed");
        //err_report(ERR_EXIT, 0, "orderly shutdown by peer\n");
    }

    //err_remark("received: (%d bytes) %.*s\n", (int)n, (int)n, msg);
   // msg[n] = '\0';
    uname(msg);
    printf("%s",msg);

    //strcat(msg, ": example");
   // size_t len = strlen(msg);
    if ((n = sendto(sd, msg, MAXSZ, 0, (struct sockaddr*) &address, addressLen)) < 0)
        //err_syserr("error on sendto(): ");
    //err_remark("sent %d bytes\n", (int)n);
    return(0);
}              


