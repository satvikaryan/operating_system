//SATVIK ARYAN
//CS22B1033
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <stdlib.h>  
#include <limits.h>  
#include <stdbool.h>  




int main()
{
    pid_t c1, c2, c3, c4;
    if (c2 = fork())
    { 
        // parent code
        wait(NULL);
        if(!(c1 = fork()))
        {
            printf("Child 1 My id is %d and my parent id is %d.\n", getpid(), getppid());
            printf("Chennai\n\n");
        }
    }
    else // child 2 code
    {
        if ((c3 = fork()) && (c4 = fork()))
        {
            wait(NULL);
            wait(NULL);
            // waitpid(c3);
            // waitpid(c4);
            printf("Child 2 My id is %d and my parent id is %d.\n", getpid(), getppid());
            printf("Kandigai\n\n");
        }
        else
        {
            if (c4 == 0 && c3 != 0)
            {
                // child 4 code
                printf("Child 4 My id is %d and my parent id is %d.\n", getpid(), getppid());
                printf("IITDM Kancheepuram\n\n");
            }
            else if (c3 == 0)
            {
                // child 3 code
                printf("Child 3 My id is %d and my parent id is %d.\n", getpid(), getppid());
                printf("SATVIK ARYAN, CS22B1033, CSE\n\n");
            }
        }
        
    }


    return 0;
}