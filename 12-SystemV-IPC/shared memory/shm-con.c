#include "Myshm.h"

int mian(void) {
  int i, shmid;
  char c;

  signal(SIGINT, sig_handler);
  if((mysem = sem_open("mysem", 0, 0777, 0)) == SEM_FAILED) {
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
  
  while(!key_pressed) {
    for(i=0;i<10;i++){
      sem_wait(mysem);
      for(int i=0;i<10;i++) printf("%d", shm[i]);
      putchar('\n');
      sem_post(mysem);
      sleep(1);
    }
  }

  while(key_pressed == 1) {
    c = 'A';
    for(i=0;i<10;i++) {
      sem_wait(mysem);
      for(int i=0;i<10;i++) shm[i] = c;
      sem_post(mysem);
      sleep(1);
      if(++c > 'Z') c = 'A';
    }
  }

  sem_close(mysem);
  shmdt(shm);

  return 0;
}