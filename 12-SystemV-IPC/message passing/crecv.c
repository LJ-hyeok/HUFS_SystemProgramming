#include "Mymessage.h"

int main(int argc, char *argv[]){
  int qid, size;
  struct umsg mymsg;
  long type;
  
  if(argc != 3){
    fprintf(stderr, "usage: %s [key] [type]\n", argv[0]);
    perror("msgq");
    exit(1);
  }
  if((qid = msgget((key_t)atoi(argv[1]), IPC_CREAT | 0660)) == -1 ){
    perror("msgget");
    exit(1);
  }
  type = (long)atoi(argv[2]);
  if(type == 0){
    msgctl(qid, IPC_RMID, NULL);
    exit(0);
  }
  while(1){
    memset(mymsg.mtext, 0, MSIZE);
    if((size = msgrcv (qid, &mymsg, (size_t)MSIZE, (long)atoi(argv[2]), 0)) == -1) {
      perror("msgrcv");
      exit(2);
    }
    else {
      mymsg.mtext[strlen(mymsg.mtext)] = '\n';
      printf("At receiver, mytype = %ld, mtext(%d) = %s\n",mymsg.mtype, size, mymsg.mtext);
      if(mymsg.mtext == "quit")
        break;
    }
  }
}