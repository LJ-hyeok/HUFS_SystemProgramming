#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void sig_catch (int sig_no) {
  //
}

int main() {
  int pid;
  int status;
  signal(SIGUSR1, sig_catch);
  if ((pid = fork()) == 0) { // child
  // this pause may block forever: why?
    pause();
    printf("Child wake up\n");
    exit(0);
  } else { // parent
    sleep(1);
    kill(pid, SIGUSR1); // send SIGUSR1 to the child process
    wait(&status);
  }
}