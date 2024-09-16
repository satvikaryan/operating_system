//SATVIK ARYAN
//CS22B1033
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <stdlib.h>  // for exit()
#include <limits.h>  // for INT_MIN, INT_MAX
#include <stdbool.h>  // for bool datatype


int main()
{
    int status;
    // pid_t p1 = getpid();
    pid_t c1, c2, c3, c4, c5;
    if (c1 = fork())
    {
        waitpid(c1, &status, 0);
        // wait(NULL);
        if((c2=fork()))
        {
            // wait(NULL);
            waitpid(c2, &status, 0);

            printf("p1 Id: %d Parent id: %d.\n", getpid(), getppid());
            printf("All processes have finished.\n");
        }
        else if (c1)
        {
            if ((c5=fork()))
            {
                // wait(NULL);
                waitpid(c5, &status, 0);
                printf("Child 2 Id: %d Parent id: %d.\n\n", getpid(), getppid());
            }
            else
            {
                printf("Child 5 Id: %d Parent id: %d.\n\n", getpid(), getppid());
            }
            
        }
    }
    else // child 1 code
    {
        if(c3 = fork())
        {
            // wait(NULL);
            waitpid(c3, &status, 0);
            if (c4 = fork())
            {
                // wait(NULL);
                waitpid(c4, &status, 0);
                printf("Child 1 Id: %d Parent id: %d.\n\n", getpid(), getppid());
            }
            else if (c3)
            {
                printf("Child 4 Id: %d Parent id: %d.\n\n", getpid(), getppid());
            }

        }
        else
        {
            printf("Child 3 Id: %d Parent id: %d.\n\n", getpid(), getppid());
        }
    }

    return 0;
}
