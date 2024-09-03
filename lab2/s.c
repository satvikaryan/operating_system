#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int pid, at, bt, ct, tat, wt, rt, rem_bt;
    struct node* next;
} node;

node* newNode(int pid, int at, int bt) {
    node* temp = (node*)malloc(sizeof(node));
    temp->pid = pid;
    temp->at = at;
    temp->bt = bt;
    temp->rem_bt = bt;
    temp->next = NULL;
    return temp;
}

node* dequeue(node** head) {
    if (*head == NULL) return NULL;
    node* temp = *head;
    *head = (*head)->next;
    return temp;
}

void enqueue(node** head, int pid, int at, int bt) {
    node* temp = newNode(pid, at, bt);
    if (*head == NULL) {
        *head = temp;
        return;
    }
    node* start = *head;
    while (start->next) {
        start = start->next;
    }
    start->next = temp;
}

void printQueue(node* head) {
    node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->pid);
        temp = temp->next;
    }
    printf("\n");
}

void enqueueBuffer(node** head, int pid, int at, int rem_bt) {
    node* newNode = (node*)malloc(sizeof(node));
    newNode->pid = pid;
    newNode->at = at;
    newNode->rem_bt = rem_bt;
    newNode->bt = 0; // Not used in buffer
    newNode->ct = 0;
    newNode->tat = 0;
    newNode->wt = 0;
    newNode->rt = 0;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        node* temp = *head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

node* dequeueBuffer(node** head) {
    if (*head == NULL) return NULL;
    node* temp = *head;
    *head = (*head)->next;
    temp->next = NULL;
    return temp;
}

int system_time = 0;
int context_switches = 0;
node* scheduler(int n, int* pids, int* ats, int* bts, int quantum) {
    
    node* queueHead = NULL;
    node* bufferHead = NULL;
    node* scheduleHead = NULL;
    node* scheduleTail = NULL;
    
    
    int i = 0;

    while (i < n || queueHead || bufferHead) {
        
        while (i < n && ats[i] <= system_time) {
            enqueue(&queueHead, pids[i], ats[i], bts[i]);
            i++;
        }

        while (bufferHead) {
            node* process = dequeueBuffer(&bufferHead);
            enqueue(&queueHead, process->pid, process->at, process->rem_bt);
        }


        if (queueHead == NULL) {
            if (i < n) {
                system_time = ats[i];
                continue;
            } else {
                break;
            }
        }

        node* process = dequeue(&queueHead);
        context_switches++;
        

        

        if (process->rem_bt > quantum) {
            system_time += quantum+1;
            process->rem_bt -= quantum;

            enqueueBuffer(&bufferHead, process->pid, process->at, process->rem_bt);
        } else {
            system_time += process->rem_bt+1;
            process->rem_bt = 0;
        }

        if (scheduleHead == NULL) {
            if(process->rem_bt == 0){
                scheduleHead = process;
                scheduleTail = process;
            }
        } else {
            if(process->rem_bt == 0){
                scheduleTail->next = process;
                scheduleTail = scheduleTail->next;
            }
        }

        process->ct = system_time;
        process->tat = process->ct - process->at;
        process->wt = process->tat - process->bt;
        process->rt = process->wt;
        
        process->next = NULL;
        printf("do you want to enter a new process\n");
        scanf("%d ",&choice);
        if(choice == ){
            scanf("%d",pids);
        }
    }

    context_switches++;
    printf("Total context switches: %d\n", context_switches);

    return scheduleHead;
}



int main() {
    int n, quantum;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pids[n];
    int ats[n];
    int bts[n];

    printf("Input process ids: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &pids[i]);

    printf("Input arrival times: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &ats[i]);

    printf("Input burst times: ");
    for (int i = 0; i < n; i++)
        scanf("%d", &bts[i]);

    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    node* scheduleHead = scheduler(n, pids, ats, bts, quantum);

    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;

    printf("%-5s %-5s %-5s %-5s %-5s %-5s %-5s\n", "PID", "AT", "BT", "CT", "TAT", "WT", "RT");
    printf("--------------------------------------------------\n");
    
    node* temp = scheduleHead;
    while (temp) {
        printf("%-5d %-5d %-5d %-5d %-5d %-5d %-5d\n",
               temp->pid, temp->at, temp->bt, temp->ct,
               temp->tat, temp->wt, temp->rt);

        avg_waiting_time += temp->wt;
        avg_turnaround_time += temp->tat;

        node* next = temp->next;
        free(temp);
        temp = next;
    }
    float cpu_util = ((float)(system_time-context_switches)/(system_time))*100;
    printf("%d\n",system_time);
    printf("%d\n",context_switches);
    printf("CPU utilization: %.1f\n",cpu_util);
    printf("Average waiting time: %.2f \n", avg_waiting_time / n);
    printf("Average turn around time: %.2f \n", avg_turnaround_time / n);

    return 0;
}
