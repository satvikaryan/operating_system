#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct {
    long mtype;
    char mtext[100];
}msgbuf;

int count_vowels(char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (tolower(str[i]) == 'a' || tolower(str[i]) == 'e' || 
            tolower(str[i]) == 'i' || tolower(str[i]) == 'o' || 
            tolower(str[i]) == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    key_t key;
    int msgid;

    
    key = 10331;

    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
    printf("msgid: %d\n",msgid);
    msgbuf message;

    
    if (msgrcv(msgid, &message, sizeof(message.mtext), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

    
    printf("Receiver: Message from Sender1: %s\n", message.mtext);
    printf("Receiver: Length of message: %ld\n", strlen(message.mtext));
    printf("Receiver: Number of vowels: %d\n", count_vowels(message.mtext));

    
    if (msgrcv(msgid, &message, sizeof(message.mtext), 2, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

    
    printf("Receiver: Message from Sender2: %s\n", message.mtext);
    printf("Receiver: Length of message: %ld\n", strlen(message.mtext));
    printf("Receiver: Number of vowels: %d\n", count_vowels(message.mtext));

    
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    return 0;
}


