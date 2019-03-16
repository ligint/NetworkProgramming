#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <netinet/tcp.h>
#include<fcntl.h>
#include <unistd.h>
#define true 1
#define false 0


void main()
{
  int sockfd,sock;

  socklen_t i;
  int len,flags;
  struct linger lin;
  socklen_t y=sizeof(lin);
  //lin.l_onoff;
  //lin.l_linger;



  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  sock = socket(AF_INET, SOCK_DGRAM, 0);

  flags = fcntl(sockfd, F_GETFL, O_NONBLOCK);
  //printf("Flags is %d",flags);
  printf("O_NONBLOCK = %s\n",(flags ? "true" : "false"));


  flags = fcntl(sock, F_GETFL, O_NONBLOCK);
  //printf("Flags is %d",flags);
  printf("O_NONBLOCK_UDP = %s\n",(flags ? "true" : "false"));


  flags = fcntl(sockfd, F_GETFL, O_ASYNC);
  //printf("Flags is %d",flags);
  printf("O_ASYNC = %s\n",(flags ? "true" : "false"));


  flags = fcntl(sock, F_GETFL, O_ASYNC);
  //printf("Flags is %d",flags);
  printf("O_ASYNC_UDP = %s\n",(flags ? "true" : "false"));


  i = sizeof(len);
  if (getsockopt(sock, SOL_SOCKET, SO_BROADCAST, &len, &i) < 0) {
    perror(": getsockopt");
  }
  //printf("SO_BROADCAST = %hu\n", len);

  printf("SO_BROADCAST = %s\n",(len ? "true" : "false"));

  if (getsockopt(sock, SOL_SOCKET, SO_DONTROUTE, &len, &i) < 0) {
    perror(": getsockopt");
  }
  //printf("SO_DONTROUTE = %hu\n", len);
  printf("SO_DONTROUTE = %s\n",(len ? "true" : "false"));

  if (getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &len, &i) < 0) {
    perror(": getsockopt");
  }

  //printf("SO_KEEPALIVE = %d , ", len);
  printf("SO_KEEPALIVE = %s , ",(len ? "true" : "false"));


  if (getsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("IDLE Timer = %d , ", len);

  if (getsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("Interval Timer = %d , ", len);

  if (getsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("Connected Timer = %d\n", len);

  if (getsockopt(sockfd, SOL_SOCKET, SO_LINGER, &lin, &y) < 0) {
    perror(": getsockopt");
  }

  //printf("SO_LINGER = %d\n", y);
  //printf("SO_LINGER = %d , ",lin.l_onoff);
  printf("SO_LINGER = %s , ",(lin.l_onoff ? "true" : "false"));
  printf("Timer =  %d\n",lin.l_linger);


  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("SO_RCVBUF_TCP = %d\n", len);

  if (getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("SO_RCVBUF_UDP = %d\n", len);

  if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("SO_SNDBUF_TCP = %d\n", len);


  if (getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("SO_SNDBUF_UDP = %d\n", len);

  if (getsockopt(sockfd, SOL_SOCKET, SO_RCVLOWAT, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("SO_RCVLOWAT_TCP = %d\n", len);


  if (getsockopt(sock, SOL_SOCKET, SO_RCVLOWAT, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("SO_RCVLOWAT_UDP = %d\n", len);

  if (getsockopt(sockfd, SOL_SOCKET, SO_SNDLOWAT, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("SO_SNDLOWAT_TCP = %d\n", len);

  if (getsockopt(sock, SOL_SOCKET, SO_SNDLOWAT, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("SO_SNDLOWAT_UDP = %d\n", len);


  if (getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &len, &i) < 0) {
    perror(": getsockopt");
  }

  //printf("SO_REUSEADDR = %hu\n", len);
  printf("SO_REUSEADDR = %s\n",(len ? "true" : "false"));


  if (getsockopt(sockfd, SOL_TCP, TCP_MAXSEG, &len, &i) < 0) {
    perror(": getsockopt");
  }

  printf("TCP_MAXSEG = %hu\n", len);


  if (getsockopt(sockfd, SOL_TCP, TCP_NODELAY, &len, &i) < 0) {
    perror(": getsockopt");
  }

  //printf("TCP_NODELAY = %hu\n", len);
  printf("TCP_NODELAY = %s\n",(len ? "true" : "false"));

  close(sockfd);
  close(sock);

}
