#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
    int pid, at, bt, ct, tat, wt, rt;
    struct node* next;
} node;

node* newNode(int pid, int at, int bt)
{
    node* temp = (node*)malloc(sizeof(node));
    temp->pid = pid;
    temp->at = at;
    temp->bt = bt;
    temp->next = NULL;
}

node* dequeue(node** head) 
{ 
    node* temp = *head; 
    *head = (*head)->next; 
    return temp;
} 

void enqueue(node** head, int pid, int at, int bt)
{
    node* temp = newNode(pid, at, bt);   
    if (*head==NULL)
    {
        *head = temp;
        return;
    }
    node* curr = *head;
    if(curr->bt > temp->bt)
    {
        temp->next = curr;
        *head = temp;
        return;
    }
    
    
    while(curr->next)
    {
        if(curr->next->bt > temp->bt)
            break;
        curr = curr->next;
    }
    
    node* temp2 = curr->next;
    curr->next = temp;
    temp->next = temp2;
}


node* scheduler(int n, int* pids, int* ats, int* bts)
{
    node* queueHead = NULL;
    node* scheduleHead = NULL;
    node* scheduleTail = NULL;
    int system_time = 0; 

    int i = 0;
    while(i < n || queueHead != NULL)
    {
        
        while (i < n && ats[i] <= system_time)
        {
            enqueue(&queueHead, pids[i], ats[i], bts[i]);
            i++;
        }

        if (queueHead == NULL)
        {
            if (i < n)
            {
                system_time = ats[i];
                continue;
            }
            break;
        }

      
        node* process = dequeue(&queueHead);
        if (system_time < process->at)
        {
            system_time = process->at;
        }
        system_time += process->bt;
        process->ct = system_time; 
        process->tat = process->ct - process->at;
        process->wt = process->tat - process->bt;
        process->rt = process->wt;

        if(scheduleHead == NULL)
        {
            scheduleHead = process;
            scheduleTail = process;
        }
        else
        {
            scheduleTail->next = process;
            scheduleTail = scheduleTail->next;
        }
    }

    return scheduleHead;
}


int main()
{
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int pids[n];
    int ats[n];
    int bts[n];

    printf("Input process ids: ");
    for (int i=0; i<n; i++)
        scanf("%d", &pids[i]);
    printf("Input arrival times: ");
    for (int i=0; i<n; i++)
        scanf("%d", &ats[i]);
    printf("Input burst times: ");
    for (int i=0; i<n; i++)
        scanf("%d", &bts[i]);

    node* scheduleHead;
    scheduleHead = scheduler(n, pids, ats, bts);


    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    // PRINT Schedule
    printf("%-5s %-5s %-5s %-5s %-5s %-5s %-5s\n",
           "PID", "AT", "BT", "CT", "TAT", "WT", "RT");
 
    while(scheduleHead)
    {
        printf("%-5d %-5d %-5d %-5d %-5d %-5d %-5d\n",
               scheduleHead->pid, scheduleHead->at, scheduleHead->bt, scheduleHead->ct,
               scheduleHead->tat, scheduleHead->wt, scheduleHead->rt);        
        
        avg_waiting_time += scheduleHead->wt;
        avg_turnaround_time += scheduleHead->tat;
        scheduleHead = scheduleHead->next;
    }

    printf("Average waiting time: %.2f \n", avg_waiting_time/n);
    printf("Average turn around time: %.2f \n", avg_turnaround_time/n);
}