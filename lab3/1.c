//SATVIK ARYAN
//CS22B1033
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>

#define SIZE 5

int arr[SIZE];

void parentProcess(){
    printf("Parent Process:\n");

    for (int i = 0; i < SIZE; i++){
        arr[i] -= 3;
    }

    int min = INT_MAX;
    for (int i = 0; i < SIZE; i++){
        printf(" parent Element: %d, Address: %p\n", arr[i], (void*)&arr[i]);
        if (arr[i] < min) {
            min = arr[i];
        }
    }

    printf("Minimum element: %d\n", min);
    printf("Parent Process ID: %d\n", getpid());
    printf("Parent's Parent Process ID: %d\n\n", getppid());
}

void childProcess() {
    printf("Child Process:\n");

    for (int i = 0; i < SIZE; i++) {
        arr[i] += 2;
    }

    int max = INT_MIN;
    for (int i = 0; i < SIZE; i++) {
        printf(" child Element: %d, Address: %p\n", arr[i], (void*)&arr[i]);
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    printf("Maximum element: %d\n", max);
    printf("Child Process ID: %d\n", getpid());
    printf("Parent Process ID: %d\n\n", getppid());
}

int main() {
    printf("Enter %d elements of the array:\n", SIZE);
    for (int i = 0; i < SIZE; i++) {
        scanf("%d", &arr[i]);
    }

    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        return 1;
    } else if (pid == 0) {
        childProcess();
    } else {
        parentProcess();
    }

    return 0;
}
