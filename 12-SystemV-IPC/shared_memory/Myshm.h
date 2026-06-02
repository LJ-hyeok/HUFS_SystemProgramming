#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h> //shared memory 기반 IPC 관련 함수
#include <sys/types.h>
#include <signal.h>

static int key_pressed = 0;
static size_t shsize = 1024;
static const int key = 16000;
static char *shm;
static sem_t *mysem;

static void sig_handler(int signo){
  key_pressed++;
  printf("continue to the next stgae");
  if(key_pressed == 1 && signo == SIGINT)
    signal(SIGINT, sig_handler);
  else if(signo == SIGINT)
    signal(SIGINT, SIG_DFL);
}