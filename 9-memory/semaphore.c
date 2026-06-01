//Semaphore의 Synchronization 동작방식 이해
//Semaphore.c 코드 작성 및 테스트
//Thread1의 Count 동작 1000개를 다 수행한 후, Thread2 Count 동작1000개가 수행되도록wait/post를 이용하여 Synchronization 상황을 테스트하시오.


#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
int cnt = 0;
static sem_t hsem;
static sem_t sync_sem;

void *Thread1(void *arg);
void *Thread2(void *arg);

int main(int argc, char *argv[]){
  pthread_t thread1, thread2;

  if(sem_init(&hsem, 0, 1) < 0 || sem_init(&sync_sem, 0, 0)){
    fprintf(stderr, "Semaphore Initilization Error\n");
    return 1;
  }
  pthread_create(&thread1, NULL, Thread1, NULL);
  pthread_create(&thread2, NULL, Thread2, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  printf("%d\n", cnt);
  sem_destroy(&hsem);

  return 0;
}

void *Thread1(void *arg){
  int i, tmp;
  for(i=0;i<1000;i++){
    sem_wait(&hsem);
    tmp = cnt;
    usleep(1000);
    cnt = tmp+1;
    sem_post(&hsem);
    //printf("thread 1: %d\n", i);
  }
  printf("Thread1 End\n");
  sem_post(&sync_sem);
  return NULL;
}

void *Thread2(void *arg){
  int i, tmp;
  sem_wait(&sync_sem);
  for(i=0;i<1000;i++){
    sem_wait(&hsem);
    tmp = cnt;
    usleep(1000);
    cnt = tmp+1;
    sem_post(&hsem);
    //printf("thread2: %d\n", i);
  }
  printf("Thread2 End\n");

  return NULL;
}