#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char **argv) {
  int sockfd;
  int portno;
  int clientlen;
  struct sockaddr_in serveraddr;
  struct sockaddr_in clientaddr;
  struct hostent *hostp;
  char buf[BUFSIZE];
  char *hostaddrp;
  int optval;
  int n;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) error("failed to open socket");

  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  fprintf(stdout, "listening to %d on %s \n", portno, inet_ntoa(serveraddr.sin_addr));

  if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) error("binding failure");

  clientlen = sizeof(clientaddr);
  while (1) {
    bzero(buf, BUFSIZE);
    n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0) error("fail in recvfrom");

    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL) error("failure to inet_ntoa\n");
    printf("\nreceived %d/%d bytes from %s\n%s\n.\n", strlen(buf), n, hostaddrp, buf);
  }
}

