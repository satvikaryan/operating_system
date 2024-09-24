//SATVIK ARYAN
//CS22B1033

#include <stdio.h>
#include<sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int pipe1[2], pipe2[2]; 
    int pid1, pid2;  
    int returnstatus1, returnstatus2;
    char parentmsg[100], childmsg[100], gchildmsg[100]; 

    
    printf("Enter parent message: ");
    scanf("%s", parentmsg);

    returnstatus1 = pipe(pipe1);
    if (returnstatus1 < 0) {
        printf("pipe1 creation failed\n");
        return 1;
    }

    pid1 = fork();
    
    
    if (pid1 == 0) 
    {  
        returnstatus2 = pipe(pipe2);
        if (returnstatus2 < 0) 
        {
            printf("pipe2 creation failed\n");
            return 1;
        }

        pid2 = fork();
        
        if (pid2 == 0) 
        {  
            printf("Grandchild's parent PID: %d\n", getppid());

            printf("Enter grandchild message: ");
            scanf("%s", gchildmsg);

            // close(pipe2[0]);  
            write(pipe2[1], gchildmsg, strlen(gchildmsg) + 1);  
            // close(pipe2[1]);  
        } 
        else 
        {  
            
            printf("Child's parent PID: %d\n", getppid());

            
            //close(pipe1[1]); 
            read(pipe1[0], childmsg, sizeof(childmsg));
            // close(pipe1[0]); 

            
            //close(pipe2[1]);  
            read(pipe2[0], gchildmsg, sizeof(gchildmsg));
            //close(pipe2[0]);  

            int diff = strlen(childmsg) - strlen(gchildmsg);
            printf("Difference in message lengths: %d\n", diff);
        }

    } 
    else 
    {  
        
        printf("Parent's parent PID: %d\n", getppid());

        
        //close(pipe1[0]);  
        write(pipe1[1], parentmsg, strlen(parentmsg) + 1);  
        //close(pipe1[1]);  
        wait(NULL);
    }

    return 0;
}