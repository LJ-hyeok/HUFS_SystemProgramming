#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#define SIZE 128
#define FIFO_S2C "fifo_s2c" // 서버 -> 클라이언트 전송용
#define FIFO_C2S "fifo_c2s" // 클라이언트 -> 서버 전송용

int fd_read, fd_write;

// 읽기 용도의 스레드 함수
void *read_thread(void *arg) {
    char buffer[SIZE];
    while (1) {
        memset(buffer, 0, SIZE);
        // blocking read: 메시지가 올 때까지 대기
        if (read(fd_read, buffer, SIZE) > 0) {
            // 전송받은 메시지가 "quit"인지 확인
            if (strncmp(buffer, "quit", 4) == 0) {
                printf("\nQuit chatting\n");
                exit(0);
            }
            // server의 메시지를 출력
            printf("\n[SERVER] %s", buffer);
            printf("[CLIENT] ");
            fflush(stdout);
        }
    }
    return NULL;
}

int main(void) {
    char buffer[SIZE];
    pthread_t tid;

    // 1. FIFO Open (클라이언트는 생성하지 않고 접근만 함)
    // Server의 write용은 Client의 read용이 됨
    if ((fd_read = open(FIFO_S2C, O_RDWR)) == -1) {
        perror("open failed");
        exit(1);
    }
    if ((fd_write = open(FIFO_C2S, O_RDWR)) == -1) {
        perror("open failed");
        exit(1);
    }

    // 2. Thread를 하나 생성 (읽기 담당)
    if (pthread_create(&tid, NULL, read_thread, NULL) != 0) {
        perror("pthread_create failed");
        exit(1);
    }

    // 3. Main 함수 (쓰기 담당)
    while (1) {
        printf("[CLIENT] ");
        fflush(stdout);
        
        memset(buffer, 0, SIZE);
        // fgets를 통해 한 문장을 입력받음
        fgets(buffer, SIZE, stdin);
        
        // 입력받은 문자열(메시지)를 server에게 전송
        write(fd_write, buffer, SIZE);
        
        // 사용자의 입력이 "quit"일 경우 프로그램 종료
        if (strncmp(buffer, "quit", 4) == 0) {
            printf("Quit chatting\n");
            exit(0);
        }
    }

    return 0;
}