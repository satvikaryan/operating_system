#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include<sys/wait.h>

#define SHM_SIZE 10
#define Key 1033

int main() {
    
    key_t key;// = ftok("shmfile", 65); 
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT|0666); 
    if (shmid < 0) {
        printf("shmget failed");
        exit(1);
    }

    
    pid_t pid = fork();

    if (pid < 0) {
        printf("fork failed");
        exit(1);
    }

    
    if (pid == 0) {
        int *shm_ptr = (int*) shmat(shmid, NULL, 0);
        if (shm_ptr == (int*) -1) {
            printf("shmat failed (child)");
            exit(1);
        }

        // int values[10];// = {1, 5, 11, 18, 24, 35, 47, 53, 62, 78};
        // for (int i = 0; i < 10; i++) {
        //     scanf("%d",&values[i]);
        // }
        
        int values[10] = {1, 5, 11, 18, 24, 35, 47, 53, 62, 78};
        
        printf("Child Process writing: ");
        for (int i = 0; i < 10; i++) {
            shm_ptr[i] = values[i];
            printf("%d ", values[i]);
        }
        printf("\n");

        shmdt(shm_ptr);
        exit(0);
    }

    else {

        wait(NULL);

        int *shm_ptr = (int*) shmat(shmid, NULL, 0);
        if (shm_ptr == (int*) -1) {
            printf("shmat failed (parent)");
            exit(1);
        }

        printf("Parent Process reading: ");
        for (int i = 0; i < 10; i++) {
            printf("%d ", shm_ptr[i]);
        }
        printf("\n");

        
        printf("Parent Process modifying:---\n");
        for (int i = 0; i < 10; i++) {
            shm_ptr[i] += 2;
        }

        printf("Parent Process displaying: ");
        for (int i = 0; i < 10; i++) {
            printf("%d ", shm_ptr[i]);
        }
        printf("\n");

        shmdt(shm_ptr);

        // shmctl(shmid, IPC_RMID, NULL);
        printf("shmctl parent :%d\n",shmctl(shmid, IPC_RMID, NULL));
    }

    return 0;
}



/* 
1. Comment out `shmdt(shm_ptr)` in the child process:
    - If the `shmdt(shm_ptr)` call is commented out, the child process will not detach from the shared memory after writing.
    - The program will still work, but it's bad practice since the shared memory segment will remain attached to the child process unnecessarily.
    - There won't be any memory leaks because the process terminates, but it's still a resource that should be freed.

2. Comment out `wait(NULL)` in the parent process:
    - If `wait(NULL)` is commented out, the parent process might start executing before the child process completes its writing to shared memory.
    - This could lead to the parent reading uninitialized data from shared memory (if the child hasn't written yet).
    - The behavior would be unpredictable, as both processes might compete for shared memory access.

3. Comment out `shmctl(shmid, IPC_RMID, NULL)` in the parent process:
    - If this call is commented out, the shared memory segment will not be removed from the system.
    - The program will still run correctly, but the shared memory will persist in the system, potentially leading to memory leaks or the need for manual cleanup.
    - You can verify this by running `ipcs -m` on the system after the program terminates to see if the shared memory still exists.

4. Comment out `fork()`:
    - If the `fork()` call is commented out, there will be no child process, and the shared memory will not be written to by the child.
    - The parent process will still execute, but it will read garbage or uninitialized values from shared memory, resulting in undefined behavior.
    - The shared memory will still be created and removed correctly.

5. Comment out the shared memory attachment `shmat(shmid, NULL, 0)` in either process:
    - If `shmat` is commented out, the process (either child or parent) will not attach to shared memory, leading to a failure when trying to read/write to shared memory.
    - The `shmat` call is crucial for both processes to interact with the shared memory segment.

6. Comment out `shm_ptr[i] = values[i]` in the child process:
    - If this line is commented out, the child process will not write anything to shared memory.
    - The parent will read the initial (default) values in shared memory, likely garbage or zeros, and print them instead of the intended values.
    - The parent will still modify and print these values, but the results will not be based on what the child wrote.

7. Comment out the parent process modifying loop:
    - If the parent process does not modify the shared memory, it will only display the values written by the child.
    - No modifications will be made, so the parent will print the same values twice (before and after the "modifying" section).
*/
