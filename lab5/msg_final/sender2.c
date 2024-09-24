#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>


struct msgbuf {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key;
    int msgid;
    

    key = 10331;

    
    msgid = msgget(key, 0666|IPC_CREAT);

    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    printf("msgid: %d\n",msgid);

    struct msgbuf message;
    message.mtype = 2; 

    
    printf("Sender2: Enter a message: ");
    fgets(message.mtext, sizeof(message.mtext), stdin);
    message.mtext[strcspn(message.mtext, "\n")] = '\0';  

    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Sender2: Message sent to queue.\n");
    return 0;
}
