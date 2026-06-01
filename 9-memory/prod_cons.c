//Semaphore의 wait/post 동작방식 이해
//Semaphore의 prod-con.c 코드 작성 및 테스트
//Sleep의 위치에 따라서 producer와 consumer가 각각 연속적으로 실행되는 상황을 테스트하시오.
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>

#define MAX_BSIZE 10
int cnt = 0;
static sem_t hsem, num_buff, num_data;

void *Producer(void *arg);
void *Consumer(void *arg);

int main(int argc, char *argv[]){
  pthread_t thread1;
  pthread_t thread2;

  if(sem_init(&hsem, 0, 1) < 0){
    fprintf(stderr, "Semaphore Initialization Error\n");
    return 1;
  }
  if(sem_init(&num_buff, 0, MAX_BSIZE) < 0){
    fprintf(stderr, "Semaphore Initialization Error\n");
    return 1;
  }
  if(sem_init(&num_data, 0, 0) < 0){
    fprintf(stderr, "Semaphore Initialization Error\n");
    return 1;
  }
  pthread_create(&thread1, NULL, Producer, NULL);
  pthread_create(&thread2, NULL, Consumer, NULL);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  printf("%d\n", cnt);
  sem_destroy(&hsem);
  sem_destroy(&num_buff); //
  sem_destroy(&num_data); //
  return 0;
}

void *Producer(void *arg){
  int i, tmp;
  for(;;){
    sem_wait(&num_buff);
    sem_wait(&hsem);
    cnt++;
    printf("prod cnt: %d\n", cnt);
    sem_post(&hsem);
    sem_post(&num_data);
    sleep(1);
  }
  printf("Producer Ends\n");
  return NULL;
}

void *Consumer(void *arg){
  int i, tmp;
  for(;;){
    sem_wait(&num_data);
    sem_wait(&hsem);
    cnt--;
    printf("cons cnt: %d\n", cnt);
    sem_post(&hsem);
    sem_post(&num_buff);
    sleep(1);
  }
  printf("Consumer Ends\n");
  return NULL;
}