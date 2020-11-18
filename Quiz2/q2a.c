#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>

const char *file = "para1.txt";
const char *q_file = "queue.dat";

struct msg {
    long type;
    char msg[100];
};

void check_err(int return_value, char *msg) {
    if(return_value < 0) {
        perror(msg);
        exit(return_value);
    }
}

int main(int argc, char *argv[]) {
    struct msg buf;
    key_t key;
    int qid, tmp;
    buf.type = 1;

    key = ftok(q_file, 'a');
    check_err(key, "ftok");

    qid = msgget(key, 0644 | IPC_CREAT);
    check_err(qid, "msgget");

    FILE *fptr = fopen(file, "r");
    if(fptr == NULL){
        perror("fopen");
        exit(1);
    }

    while(fscanf(fptr, "%s", buf.msg) != EOF) {
        tmp = msgsnd(qid, &buf, strlen(buf.msg)+1, 0);
        check_err(tmp, "msgsnd");
    }

    printf("All messages sent from %s\n", file);
    return 0;
}