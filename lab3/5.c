//SATVIK ARYAN
//CS22B1033
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() 
{
    pid_t C1, C2, C3, C4, C5;

    printf("Parent P1: My PID:  %d, Parent PID:  %d\n", getpid(), getppid());

    C1 = fork();
    
    if (C1 == 0)
    { 
        printf("Child C1: My PID:  %d, Parent PID:  %d\n", getpid(), getppid());

        C3 = fork();
        if(C3==0)
        {
            printf("Child C3: My PID:  %d, Parent PID:  %d\n", getpid(), getppid());
            return 0;  
            
        }
        else
        {
            wait(NULL); 

            C4 = fork();
            if (C4 == 0)
            {  
                printf("Child C4: My PID:  %d, Parent PID:  %d\n", getpid(), getppid());
                return 0;  
            } 
            else
            {
                wait(NULL);
                
            }
        }

        return 0;  
    } 
    else 
    {
        wait(NULL);  

        
        C2 = fork();
        if (C2 == 0) 
        {  
            printf("Child C2: My PID:  %d, Parent PID:  %d\n", getpid(), getppid());

            C5 = fork();
            if (C5 == 0) 
            { 
                printf("Child C5: My PID:  %d, Parent PID:  %d\n", getpid(), getppid());
                return 0;  
            } 
            else
            {
                wait(NULL); 
                
            }

            return 0; 
        } 
        else 
        {
        wait(NULL);
        }
    }

 
    printf("Parent P1: All processesfinished.\n");

    return 0;
}