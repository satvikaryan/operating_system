#include<stdio.h>
#include<stdlib.h>

#define MAX_PROCESS 100

int system_time = 0;

typedef struct {
    int pid, AT, BT, FT, TAT, WT, RT;
} process;

typedef struct node {
    process data;
    struct node* next;
} node;

node* enqueue(node* head, process proc) {
    node* temp = (node*)malloc(sizeof(node));
    temp->data = proc;
    temp->next = NULL;
    if (head == NULL) {
        head = temp;
        return head;
    }
    node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = temp;
    return head;
}

node* dequeue(node** head) {
    if (*head == NULL){
        return NULL;
    }
    node* temp = *head;
    *head = (*head)->next;
    return temp;
}

void processFCFS(node* head, int process_count) {
    int total_TAT = 0, total_WT = 0;
    node* current = head;
    
    printf("P_id\tAT\tBT\tFT\tTAT\tWT\tRT\n");
    while (current != NULL) {
        if (system_time < current->data.AT) {
            system_time = current->data.AT;
        }
        current->data.FT = system_time + current->data.BT;
        current->data.TAT = current->data.FT - current->data.AT;
        current->data.WT = current->data.TAT - current->data.BT;
        current->data.RT = system_time - current->data.AT;

        total_TAT += current->data.TAT;
        total_WT += current->data.WT;

        system_time += current->data.BT;

        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
               current->data.pid, current->data.AT, current->data.BT, 
               current->data.FT, current->data.TAT, current->data.WT, current->data.RT);
        
        current = current->next;
    }

    printf("Avg waiting time: %f\n", (float)total_WT / process_count);
    printf("Avg turnaround time: %f\n", (float)total_TAT / process_count);
    
    printf("Schedule: ");
    current = head;
    while (current != NULL) {
        printf("P%d ", current->data.pid);
        current = current->next;
    }
    printf("\n");
}

int main() {
    node* head = NULL;
    int process_count;

    printf("ENTER THE NO OF PROCESSES\n");
    scanf("%d", &process_count);

    int pid[MAX_PROCESS];
    int AT[MAX_PROCESS];
    int BT[MAX_PROCESS];

    printf("P_id: ");
    for (int i = 0; i < process_count; i++) {
        scanf("%d", &pid[i]);
    }

    printf("AT  : ");
    for (int i = 0; i < process_count; i++) {
        scanf("%d", &AT[i]);
    }

    printf("BT  : ");
    for (int i = 0; i < process_count; i++) {
        scanf("%d", &BT[i]);
    }

    for (int i = 0; i < process_count; i++) {
        process flag;
        flag.pid = pid[i];
        flag.AT = AT[i];
        flag.BT = BT[i];
        flag.FT = flag.TAT = flag.WT = flag.RT = 0;

        head = enqueue(head, flag);
    }

    processFCFS(head, process_count);

    return 0;
}
