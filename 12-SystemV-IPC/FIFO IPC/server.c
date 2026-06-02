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
            // client의 메시지를 출력
            printf("\n[CLIENT] %s", buffer);
            printf("[SERVER] ");
            fflush(stdout);
        }
    }
    return NULL;
}

int main(void) {
    char buffer[SIZE];
    pthread_t tid;

    // 1. 두 개의 FIFO 생성 (Server에서만 생성)
    // 파일이 이미 존재할 경우의 에러를 무시하기 위해 에러 처리는 생략하거나 unlink 활용 가능
    mkfifo(FIFO_S2C, 0666);
    mkfifo(FIFO_C2S, 0666);

    // 2. FIFO Open
    // 강의 자료의 fifo-recv.c 처럼 O_RDWR로 열어 대기 상태 방지 (deadlock 방지)
    if ((fd_write = open(FIFO_S2C, O_RDWR)) == -1) {
        perror("open failed");
        exit(1);
    }
    if ((fd_read = open(FIFO_C2S, O_RDWR)) == -1) {
        perror("open failed");
        exit(1);
    }

    // 3. Thread를 하나 생성 (읽기 담당)
    if (pthread_create(&tid, NULL, read_thread, NULL) != 0) {
        perror("pthread_create failed");
        exit(1);
    }

    // 4. Main 함수 (쓰기 담당)
    while (1) {
        printf("[SERVER] ");
        fflush(stdout);
        
        memset(buffer, 0, SIZE);
        // fgets를 통해 한 문장을 입력받음
        fgets(buffer, SIZE, stdin);
        
        // 입력받은 문자열(메시지)를 client에게 전송
        write(fd_write, buffer, SIZE);
        
        // 사용자의 입력이 "quit"일 경우 프로그램 종료
        if (strncmp(buffer, "quit", 4) == 0) {
            printf("Quit chatting\n");
            exit(0);
        }
    }

    return 0;
}