#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024
#define PORT 30000

int main(int argc, char *argv[]) {
  int sd;
  struct sockaddr_in servaddr, cliaddr;
  int cliaddrsize;
  char data[MAX];
  int bytes;

  if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    fprintf(stderr, "can't open socket.\n");
    exit(1);
  }

  bzero((char*)&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  if(bind(sd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
    fprintf(stderr, "can't bind to socket.\n");
    exit(1);
  }
  printf("UDP Echo Server[%d] \n", PORT);

  while(1) {
    cliaddrsize = sizeof(cliaddr);
    bytes = recvfrom(sd, data, MAX, 0, (struct sockaddr *)&cliaddr, &cliaddrsize);
    if(bytes < 0) {
      fprintf(stderr, "can't receive data. \n");
      continue;
    }
    data[bytes] = 0;
    printf("[Client IP: %s, Port: %d] message: %s",inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), data);
    if(sendto(sd, data, bytes, 0, (struct sockaddr *)&cliaddr, cliaddrsize) != bytes) {
      fprintf(stderr, "cant send data. \n");
    }
  }

  close(sd);
  return 0;
}