#include "Mymessage.h"

int main(int argc, char *argv[]){
  int qid;
  long type;
  struct umsg mymsg;
  if( argc != 3) {
    fprintf (stderr, "usage : %s [key] [type]\n", argv[0]);
    perror ("msgq");
    exit(1);
  }
  if(( qid = msgget ((key_t) atoi(argv[1]), IPC_CREAT | 0660)) == -1) {
    perror("msgget");
    exit(1);
  }
  type = (long)atoi(argv[2]);
  mymsg.mtype = type;

  while(1){
    printf(">type message: ");
    fgets(mymsg.mtext, sizeof(mymsg.mtext), stdin);
    mymsg.mtext[strlen(mymsg.mtext)] = '\0';
    if(msgsnd(qid, &mymsg, strlen(mymsg.mtext), 0) == -1){
      perror("msgsnd");
      exit(2);
    }
    if(strncmp(mymsg.mtext, "quit", 4) == 0) break;
  }
}