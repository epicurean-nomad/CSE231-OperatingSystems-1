#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>

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
    char ch = '\0';

    key = ftok(q_file, 'a');
    check_err(key, "ftok");

    qid = msgget(key, 0644);
    check_err(qid, "msgget");

    printf("Message queue ready!\nSend a 'quit' message to exit\n\n");

    while(1) {
        tmp = msgrcv(qid, &buf, sizeof(buf), 0, 0);
        check_err(tmp, "msgrcv");
        printf("%s\n", buf.msg);

        if(!strcmp(buf.msg, "end"))
            break;
    }

    printf("\nAll messages received!\n");
    return 0;
}