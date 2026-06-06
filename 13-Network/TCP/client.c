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
#define HOSTADDR "127.0.0.1"

int main(int argc, char *argv[]) {
  int sd;
  struct sockaddr_in servaddr;
  pid_t pid;
  char buf[MAX];

  if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "can't open socket.\n");
    exit(1);
  }
  printf("connet!\n");

  if((pid = fork()) < 0) {
    fprintf(stderr, "can't fork process.\n");
    exit(1);
  }

  if(pid == 0) {
    int bytes;
    while(1) {
      bytes = recv(sd, buf, MAX-1, 0);
      if(bytes == 0) {
        printf("\ndisconnected\n");
        break;
      }
      else if(bytes < 0) {
        fprintf(stderr, "can't receive data.\n");
        break;
      }
      buf[bytes] = 0;
      printf("\n[Server]: %s", buf);
      printf("[You]: ");
      fflush(stdout);
    }
    kill(getppid(), SIGINT);
    exit(0);
  }
  else {
    printf("[You]: ");
    fflush(stdout);
    while(fgets(buf, MAX, stdin) != NULL) {
      if(send(sd, buf, strlen(buf), 0) < 0) {
        fprintf(stderr, "can't send data.\n");
        break;
      }
      printf("[You]: ");
      fflush(stdout);
    }
    close(sd);
    kill(pid, SIGINT);
  }
  return 0;
}