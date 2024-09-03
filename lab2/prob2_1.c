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

node* dequeue(node** head) {
    if (*head == NULL) return NULL;
    node* temp = *head;
    *head = (*head)->next;
    return temp;
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
node* scheduler(int quantum) {
    node* queueHead = NULL;
    node* bufferHead = NULL;
    node* scheduleHead = NULL;
    node* scheduleTail = NULL;

    while (1) {
        // Add new processes based on system time
        char c = 't';
        while (c != 'n') {
            printf("Current system time: %d\n", system_time);
            printf("Do you want to insert another process (y/n): ");
            scanf(" %c", &c);
            if (c == 'y') {
                int pid, at, bt;
                printf("Input process id: ");
                scanf("%d", &pid);
                printf("Input arrival time: ");
                scanf("%d", &at);
                printf("Input burst time: ");
                scanf("%d", &bt);
                if (at <= system_time) {
                    enqueue(&queueHead, pid, at, bt);
                } else {
                    printf("Arrival time is greater than system time.\n");
                }
            } else if (c == 'n' || c == 's') {
                break;
            } else {
                printf("Invalid input. Please enter 'y' or 'n'.\n\n");
                c = 'n';
            }
        }

        // Move processes from buffer to queue
        while (bufferHead) {
            node* process = dequeueBuffer(&bufferHead);
            enqueue(&queueHead, process->pid, process->at, process->rem_bt);
        }

        if (queueHead == NULL) {
            if (c == 's') break; // Exit simulation if 's' was entered
            system_time++;
            continue;
        }

        node* process = dequeue(&queueHead);
        context_switches++;

        if (process->rem_bt > quantum) {
            system_time += quantum;
            process->rem_bt -= quantum;
            enqueueBuffer(&bufferHead, process->pid, process->at, process->rem_bt);
        } else {
            system_time += process->rem_bt;
            process->rem_bt = 0;
        }

        if (scheduleHead == NULL) {
            if (process->rem_bt == 0) {
                scheduleHead = process;
                scheduleTail = process;
            }
        } else {
            if (process->rem_bt == 0) {
                scheduleTail->next = process;
                scheduleTail = scheduleTail->next;
            }
        }

        process->ct = system_time;
        process->tat = process->ct - process->at;
        process->wt = process->tat - process->bt;
        process->rt = process->wt;
    }

    context_switches++;
    printf("Total context switches: %d\n", context_switches);
    return scheduleHead;
}

int main() {
    int quantum;
    printf("Enter time quantum: ");
    scanf("%d", &quantum);

    node* scheduleHead = scheduler(quantum);

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
    float cpu_util = ((float)(system_time - context_switches) / system_time) * 100;
    printf("CPU utilization: %.1f%%\n", cpu_util);
    printf("Average waiting time: %.2f\n", avg_waiting_time / context_switches);
    printf("Average turn around time: %.2f\n", avg_turnaround_time / context_switches);

    return 0;
}
