//SATVIK ARYAN
//CS22B1033
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t child1_pid, child2_pid;
    int status;

    child1_pid = fork();
    
    if (child1_pid < 0) {
        perror("Failed to create Child 1");
        exit(1);
    }

    if (child1_pid == 0) {
        printf("Child 1 process\n");
        printf("Process ID: %d, Parent ID: %d, Return value of fork: %d\n", getpid(), getppid(), child1_pid);

        execl("./program32", "program32", NULL);    
        
        //exec(); doesnt work
        // execlp("program32", "program32", NULL);  
        // execv("./program32", (char *[]){ "program32", NULL });  
        // execvp("program32", (char *[]){ "program32", NULL });   
        perror("Failed to execute program32");
        exit(1);
    } else {
        
        child2_pid = fork();
        
        if (child2_pid < 0) {
            
            perror("Failed to create Child 2");
            exit(1);
        }

        if (child2_pid == 0) {
            printf("Child 2 process\n");
            printf("Process ID: %d, Parent ID: %d, Return value of fork: %d\n", getpid(), getppid(), child2_pid);
            
            //exec(); doesnt work
            execl("./program33", "program33", NULL);    
            // execlp("program33", "program33", NULL);  
            // execv("./program33", (char *[]){ "program33", NULL });  
            // execvp("program33", (char *[]){ "program33", NULL });   

            perror("Failed to execute program33");
            exit(1);
        }
    }

    wait(NULL);
    wait(NULL);
    
    // waitpid(child1_pid, &status, 0);  
    // waitpid(child2_pid, &status, 0); 

    printf("\nParent Process\n");
    printf("Institute: IIIT KANCHEPURAM\n");
    printf("Process ID: %d, Parent ID: %d\n", getpid(), getppid());
    printf("Child 1 PID: %d, Child 2 PID: %d\n", child1_pid, child2_pid);

    return 0;
}
