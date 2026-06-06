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
  int sd, nsd;
  struct sockaddr_in servaddr, cliaddr;
  int cliaddrsize;
  pid_t pid;
  char buf[MAX];

  if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    frpintf(stderr, "can't open socket.\n");
    exit(1);
  }
  bzero((char*) &servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY means all of IP addres of a multi-home host
  servaddr.sin_port = htons(PORT);

  if(bind(sd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
    fprintf(stderr, "can't bind to socket.\n");
    exit(1);
  }
  listen(sd, BACKLOG);
  printf("TCP Chat Server[%d]\n", PORT);
  cliaddrsize = sizeof(cliaddr);
  if((nsd = accept(sd, (struct sockaddr *) &cliaddr, &cliaddrsize)) < 0) {
    fprintf(stderr, "can't accept connection.\n");
    exit(1);
  }

  printf("connect!\n");
  close(sd);

  if(pid == 0) { //자식 프로세스: 메시지 수신
    int bytes;
    while(1) { 
      bytes = recv(nsd, buf, MAX-1, 0);
      if(bytes ==0) {
        printf("\ndisconnected\n");
        break;
      }
      else if(bytes < 0) {
        fprintf(stderr, "can't receive data.\n");
        break;
      }
      buf[bytes] = 0;
      printf("\n[Client]: %s", buf);
      printf("[You]: ");
      fflush(stdout);
    }
    kill(getppid(), SIGINT);
    exit(0);
  }
  else { //부모 프로세스: 입력 + 메시지 전송
    printf("[You]: ");
    fflush(stdout);
    while(fgets(buf, MAX, stdin) != NULL) {
      if(send(nsd, buf, strlen(buf), 0) < 0) {
        fprintf(stderr, "can't send data.\n");
        break;
      }
      printf("[You]: ");
      fflush(stdout);
    }
    close(nsd);
    kill(pid, SIGINT);
  }
  return 0;
}