#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int P_id;         // Process ID
    int AT;           // Arrival Time
    int PR;           // Priority
    int BT;           // Burst Time
    int CT;           // Completion Time
    int TAT;          // Turnaround Time
    int WT;           // Waiting Time
    int RT;           // Response Time
} Process;
typedef struct {
    Process* heap;    // Array to store heap elements
    int size;         // Current number of elements in the heap
    int capacity;     // Maximum number of elements in the heap
} MinHeap;
void swap(Process* a, Process* b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(MinHeap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->heap[left].PR < heap->heap[smallest].PR)
        smallest = left;
    
    if (right < heap->size && heap->heap[right].PR < heap->heap[smallest].PR)
        smallest = right;
    
    if (smallest != idx) {
        swap(&heap->heap[idx], &heap->heap[smallest]);
        heapify(heap, smallest);
    }
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->heap = (Process*)malloc(capacity * sizeof(Process));
    return heap;
}

void insertHeap(MinHeap* heap, Process p) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->heap = (Process*)realloc(heap->heap, heap->capacity * sizeof(Process));
    }
    int i = heap->size++;
    heap->heap[i] = p;

    while (i != 0 && heap->heap[(i - 1) / 2].PR > heap->heap[i].PR) {
        swap(&heap->heap[i], &heap->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Process extractMin(MinHeap* heap) {
    if (heap->size == 0) {
        Process p = {0, 0, 0, 0, 0, 0, 0, 0};
        return p;
    }
    Process root = heap->heap[0];
    if (heap->size > 1) {
        heap->heap[0] = heap->heap[heap->size - 1];
        heapify(heap, 0);
    }
    heap->size--;
    return root;
}
int main() {
    MinHeap* heap = createMinHeap(3);
    int currentTime = 0;

    while (1) {
        printf("Enter process details (P_id AT PR BT), or -1 to end:\n");
        Process p;
        int res = scanf("%d %d %d %d", &p.P_id, &p.AT, &p.PR, &p.BT);

        if (res == EOF) {
            break;
        }
        if (p.P_id == -1) {
            break;
        }

        if (p.AT > currentTime) {
            currentTime = p.AT; // Move time forward if needed
        }

        insertHeap(heap, p);

        // Execute the process with the highest priority
        while (heap->size > 0) {
            Process currentProcess = extractMin(heap);
            currentProcess.RT = currentTime - currentProcess.AT;
            currentTime += currentProcess.BT;
            currentProcess.CT = currentTime;
            currentProcess.TAT = currentProcess.CT - currentProcess.AT;
            currentProcess.WT = currentProcess.TAT - currentProcess.BT;

            printf("Process %d: AT=%d, PR=%d, BT=%d, CT=%d, TAT=%d, WT=%d, RT=%d\n",
                   currentProcess.P_id, currentProcess.AT, currentProcess.PR, currentProcess.BT,
                   currentProcess.CT, currentProcess.TAT, currentProcess.WT, currentProcess.RT);

            printf("Do you want to add another process? (1: Yes, 0: No): ");
            int addMore;
            scanf("%d", &addMore);
            if (addMore == 0) {
                break;
            }
        }
    }

    free(heap->heap);
    free(heap);
    return 0;
}
