#include "Myshm.h"

int main(void){
  int i, shmid;

  signal(SIGINT, sig_handler);

  sem_unlink("mysem"); // remove the old semaphore if any
  if((mysem = sem_open("mysem", O_EXCL | O_CREAT, 0777, 1)) == SEM_FAILED) {
    perror("Sem Open Error");
    exit(1);
  }
  if((shmid = shmget((size_t)key, shsize, IPC_CREAT | 0666)) < 0) { //shmget 공유 메모리 생성, 접근
    perror("shmget");
    exit(1);
  }
  if((shm = (char*)shmat(shmid, NULL, 0)) == (char*)-1) { //공유 메모리를 현재에 연결, attach
    perror("shmat");
    exit(1);
  }

  for(int i=0;i<10;i++) {
    shm[i] = 0;
  }

  //write data to the shared memory
  while((!key_pressed)) {
    for(i=0;i<10;i++){
      sem_wait(mysem);
      for(int i=0;i<10;i++) shm[i] = (shm[i]+1) % 10;
      sem_post(mysem);
      sleep(1);
    }
  }

  //read data from the shared memory
  while(key_pressed == 1){
    for(i=0;i<10;i++) {
      sem_wait(mysem);
      for(int i=0;i<10;i++) printf("%c", (char)shm[i]);
      sem_post(mysem);
      sleep(1);
      printf("\n\n");
      fflush(stdout);
    }
  }
  sem_close(mysem);
  sem_unlink("mysem");
  shmdt(shm);
  shmctl(shmid, IPC_RMID, 0);

  return 0;
}
