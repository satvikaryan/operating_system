#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void handle_sigfpe(int sig) {
    printf("Grandchild: Caught SIGFPE \n");
    kill(getpid(), SIGKILL); 
}

void handle_sigint(int sig) {
    printf("Child: Caught SIGINT (Ctrl+C)\n");
    kill(getpid(),SIGKILL);
    // exit(0);
}

void handle_sigusr1(int sig) {
    printf("Parent: Caught SIGUSR1\n");
}

int main() {
    pid_t child_pid, grandchild_pid;

    
    signal(SIGUSR1, handle_sigusr1);

    child_pid = fork(); 
    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (child_pid == 0) { 
        printf("Child: Created (PID: %d, Parent PID: %d)\n", getpid(), getppid());

        signal(SIGINT, handle_sigint);

        grandchild_pid = fork();
        if (grandchild_pid < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (grandchild_pid == 0) { 
            printf("Grandchild: Created (PID: %d, Parent PID: %d)\n", getpid(), getppid());

            
            signal(SIGFPE, handle_sigfpe);
            raise(SIGFPE);
            // int x = 5;
            // int y = 0;
            // int z = x / y; 
            //raise();
        }
        else {
            
            wait(NULL);
            while(1);
            // raise(SIGINT);
            printf("Child: Grandchild has terminated, now terminating child process...\n");
            exit(0);
        }
    }
    else {
        
        wait(NULL);
        // raise(SIGUSR1);
        kill(getpid(),SIGUSR1);
        printf("Parent: Child has terminated, parent terminated\n");
        exit(0);
    }

    return 0;
}
