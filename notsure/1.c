// Ishank Kumar
// CS22B1043

#include<stdio.h>
#include<stdlib.h>

typedef struct pcb
{
    int pid, at, pr, bt, ct, tat, wt, rt;
} pcb;

pcb newProcess(int pid, int at, int pr, int bt)
{
    pcb temp;
    temp.pid = pid;
    temp.at = at;
    temp.pr = pr;
    temp.bt = bt;
    return temp;
}

typedef struct heap {
    pcb* arr;
    int size;
    int capacity;
} heap;


void maxHeapify(heap* h, int index)
{
	int left = index * 2 + 1;
	int right = index * 2 + 2;
	int max = index;

	// Checking whether our left or child element
	// is at right index of not to avoid index error
	if (left >= h->size || left < 0)
		left = -1;
	if (right >= h->size || right < 0)
		right = -1;

	// store left or right element in max if
	// any of these is smaller that its parent
	if (left != -1 && h->arr[left].pr > h->arr[max].pr)
		max = left;
	if (right != -1 && h->arr[right].pr > h->arr[max].pr)
		max = right;

	// Swapping the nodes
	if (max != index) {
		pcb temp = h->arr[max];
		h->arr[max] = h->arr[index];
		h->arr[index] = temp;

		// recursively calling for their child elements
		// to maintain max heap
		maxHeapify(h, max);
	}
}


pcb dequeue(heap* queue)
{ 
    pcb process = queue->arr[0];
    queue->arr[0] = queue->arr[queue->size - 1];
    // Decrement the size of heap
    queue->size--;
 
    // Call maxheapify_top_down for 0th index
    // to maintain the heap property
    maxHeapify(queue, 0);

    return process;
} 

// Defining maxHeapify_bottom_up function
void insertHelper(heap* h, int index)
{
    // Store parent of element at index
    // in parent variable
    int parent = (index - 1) / 2;
 
    if (h->arr[parent].pr < h->arr[index].pr) {
        // Swapping when child is smaller
        // than parent element
        pcb temp = h->arr[parent];
        h->arr[parent] = h->arr[index];
        h->arr[index] = temp;
 
        // Recursively calling maxHeapify_bottom_up
        insertHelper(h, parent);
    }
}


void enqueue(heap* queue, int pid, int at, int pr, int bt)
{
    // pcb creation
    pcb process = newProcess(pid, at, pr, bt);   

    // Checking if heap is full or not
    if (queue->size < queue->capacity) 
    {
        // Inserting data into an array
        queue->arr[queue->size] = process;
        // Calling maxHeapify_bottom_up function
        insertHelper(queue, queue->size);
        // Incrementing size of array
        queue->size++;
    }
    else 
    {
        queue->capacity *= 2;
        queue->arr = (pcb*)realloc(queue->arr, queue->capacity * sizeof(pcb));
        // Inserting data into an array
        queue->arr[queue->size] = process;
        // Calling maxHeapify_bottom_up function
        insertHelper(queue, queue->size);
        // Incrementing size of array
        queue->size++;
    }
}

int schedule_size = 0;
pcb* scheduler(int n, int* pids, int* ats, int* prs, int* bts)
{
    // ready queue creation
    heap* queue = (heap*)malloc(sizeof(heap));
    queue->size = 0;
    queue->capacity = 3;
    queue->arr = (pcb*)malloc(queue->capacity * sizeof(pcb));


    // to store the schedule
    pcb* scheduleHead = (pcb*)malloc(20 * sizeof(pcb));

    int system_time = 0;

    // CONSTRUCT Ready queue
    for (int i=0; i<n; i++)
    {
        enqueue(queue, pids[i], ats[i], prs[i], bts[i]);
    }

    // ----------------------------------------------------------------------
    // Assign processes to CPU
    while (queue->size > 0) {
        pcb process = dequeue(queue);
        if (system_time < process.at) {
            system_time = process.at;
        }
        system_time += process.bt;
        process.ct = system_time;
        process.tat = process.ct - process.at;
        process.wt = process.tat - process.bt;
        process.rt = process.wt; // Non-preemptive

        // Save accounting data
        scheduleHead[schedule_size++] = process;

        printf("Current queue size: %d\n", queue->size);
        printf("System time: %d\n", system_time);
        // Prompt user for additional processes
        char c = 't';
        while (c != 'n') {
            printf("Do you want to insert another process (y/n): ");
            scanf(" %c", &c);
            if (c == 'y') {
                int pid, at, pr, bt;
                printf("Input process id: ");
                scanf("%d", &pid);
                printf("Input arrival time: ");
                scanf("%d", &at);
                printf("Input priority id: ");
                scanf("%d", &pr);
                printf("Input burst time: ");
                scanf("%d", &bt);
                // if (at <= system_time) {
                //     enqueue(queue, pid, at, pr, bt);
                // } else {
                //     printf("Arrival time is greater than system time.\n");
                // }
                enqueue(queue, pid, at, pr, bt);
            } else if (c == 'n') {
                break;
            } else {
                printf("Invalid input. Please enter 'y' or 'n'.\n\n");
            }
        }
        printf("\n\n");
    }

    // Free the memory allocated for the queue
    free(queue->arr);
    free(queue);
    
    return scheduleHead;
}

int main()
{
    int n;

    while(1)
    {
        printf("Enter number of processes: ");
        scanf("%d", &n);
        if (n>3)
            printf("Number of processes entered exceed queue size limit of 3\n");
        else
            break;
    }

    int pids[n];
    int ats[n];
    int prs[n];
    int bts[n];

    for (int i=0; i<n; i++)
    {
        printf("Input process id%d: ", i+1);
        scanf("%d", &pids[i]);
        printf("Input arrival time%d: ", i+1);
        scanf("%d", &ats[i]);
        printf("Input priority for pid%d: ", i+1);
        scanf("%d", &prs[i]);
        printf("Input burst time%d: ", i+1);
        scanf("%d", &bts[i]);
        printf("\n\n\n");
    }

    pcb* scheduleHead;
    scheduleHead = scheduler(n, pids, ats, prs, bts);


    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    // PRINT Schedule
    printf("%-5s %-5s %-5s %-5s %-5s %-5s %-5s\n",
           "PID", "AT", "BT", "CT", "TAT", "WT", "RT");
    printf("--------------------------------------------------\n");
    for (int i=0; i<schedule_size; i++)
    {
        printf("%-5d %-5d %-5d %-5d %-5d %-5d %-5d\n",
               scheduleHead[i].pid, scheduleHead[i].at, scheduleHead[i].bt, scheduleHead[i].ct,
               scheduleHead[i].tat, scheduleHead[i].wt, scheduleHead[i].rt);        
        
        avg_waiting_time += scheduleHead[i].wt;
        avg_turnaround_time += scheduleHead[i].tat;
    }

    printf("Average waiting time: %.2f \n", avg_waiting_time/n);
    printf("Average turn around time: %.2f \n", avg_turnaround_time/n);
}