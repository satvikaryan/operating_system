//SATVIK ARYAN
//CS22B1033

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define BUFFER_SIZE 100

int main() {
    int pipefd[2]; 
    pid_t pid;
    char buffer[BUFFER_SIZE];
    
    if (pipe(pipefd) == -1) { 
        perror("pipe");
        exit(1);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {  
        close(pipefd[1]);  
        read(pipefd[0], buffer, BUFFER_SIZE);
        printf("Child process received: %s\n", buffer);  
        close(pipefd[0]);  
    } else {  
        close(pipefd[0]);  
        printf("Parent process: Enter a string: ");
        fgets(buffer, BUFFER_SIZE, stdin);  
        write(pipefd[1], buffer, strlen(buffer) + 1); 
        close(pipefd[1]); 
        wait(NULL);
    }

    return 0;
}

/* Analysis:
1. Commenting Out System Calls:
    a. Comment out `close(pipefd[1])` in the child process:
        The write end of the pipe remains open in the child (not required as child only reads. )
        won't cause any errors here (but can lead to resource leakage)

    b. Comment out `close(pipefd[0])` in the parent process:
        The read end of the pipe is not needed in the parent. By not closing it, the program might behave normally in this simple case, but again, it is inefficient and bad practice as resources are not freed.

    c. Comment out both `close()` calls:
        If neither end of the pipe is closed, the program will still run however chumma waste of resources.

2. What will happen if both ends of the pipe are closed before any read or write operations are performed?
    If both ends of the pipe are closed before the read/write operations, the communication between the parent and child process would fail.
    closing the write end before writing would result in an error when trying to write to the pipe
    
3. What happens if you call `read()` and `write()` on the same pipe in the same process?
    If the same process tries to both read and write from the same pipe without forking, it will likely cause a deadlock.
    The process will wait indefinitely for the data to arrive, but since it's trying to handle both operations (read and write) within the same context, the pipe will never be filled, and the program will hang.

4. What happens if you write multiple times to the pipe without corresponding reads?
    In this program, the pipe buffer is small, so writing continuously without reading would block the process once the buffer is full.

5. Other random scenarios:
    Writing after the pipe is closed on the write end:
        If the parent closes the write end chumma before calling `write()`, the program will fail, and the `write()` system call will return -1, indicating an error.
        The program could also generate a SIGPIPE signal, terminating the process unless it is handled.

    Reading from an empty pipe:
        If the child tries to read from the pipe when no data has been written yet, it will block and wait until data becomes available.
        This is a common synchronization behavior, as pipes are typically used for communication between processes where one writes and the other reads.

    Pipe buffer overflow:
        Continuously writing to the pipe without corresponding reads will eventually lead to buffer overflow. The writer process will block until the reader consumes data from the pipe.
*/
