//SATVIK ARYAN
//CS22B1033
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sortArray(int *arr, int size) {
    for(int i = 0; i < size - 1; i++) {
        for(int j = 0; j < size - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void reverseArray(int *arr, int size) {
    int start = 0, end = size - 1;
    while(start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

void printArrayAndAddress(int *arr, int size) 
{
    for(int i = 0; i < size; i++) 
    {
        printf("Element at index %d: %d, Address: %p\n", i, arr[i], (void*)&arr[i]);
    }
}

int main() 
{
    int *arr;
    int size;
    
    printf("Enter the size of the array: ");
    scanf("%d", &size);
    
    arr = (int *)malloc(size * sizeof(int));
    
    if (arr == NULL) 
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter %d elements of the array:\n", size);
    for(int i = 0; i < size; i++) 
    {
        scanf("%d", &arr[i]);
    }
    
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        printf("\nChild Process:\n");
        printf("Array elements and their addresses:\n");
        printArrayAndAddress(arr, size);
        
        reverseArray(arr, size);
        
        printf("Reversed array:\n");
        for (int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");

        exit(0);
    }
    else {
        int status;
        wait(&status);

        printf("\nParent Process:\n");
        printf("Array elements and their addresses:\n");
        printArrayAndAddress(arr, size);
        
        sortArray(arr, size);
        
        printf("Sorted array:\n");
        for (int i = 0; i < size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    free(arr);
    
    return 0;
}
