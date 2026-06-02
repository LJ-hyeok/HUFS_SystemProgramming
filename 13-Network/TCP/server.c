#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024 //max client data length
#define PORT 30000 //server prot
#define BACKLOG 5 //queue length

int main(int argc, char *argv[]) {
  int sd, nsd, pid, byte, cliaddrsize;
  struct sockaddr_in cliaddr, servaddr;
  char data[MAX];

  if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    frpintf(stderr, "can't open socket.\n");
    exit(1);
  }
  //to bind the server itself to the socket
  bzero((char*) &servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY means all of IP addres of a multi-home host
  servaddr.sin_port = htons(PORT);

  if(bind(sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
    fprintf(stderr, "can't bind to socket.\n");
    exit(1);
  } //bind itself to the socket
  

}