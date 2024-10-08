#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd;
    int A, B1;
    char * myfifo = "/tmp/myfifo";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open FIFO for writing");
        exit(1);
    }

    printf("Enter an integer value A: ");
    scanf("%d", &A);

    if (write(fd, &A, sizeof(int)) < 0) {
        perror("Failed to write A to FIFO");
        close(fd);
        exit(1);
    }

    printf("Sender: Wrote A = %d to FIFO.\n", A);

    close(fd);

    fd = open(myfifo, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open FIFO for reading");
        exit(1);
    }

    if (read(fd, &B1, sizeof(int)) < 0) {
        perror("Failed to read B1 from FIFO");
        close(fd);
        exit(1);
    }

    printf("Sender: Received B1 (one's complement of A) = %d from FIFO.\n", B1);

    close(fd);

    return 0;
}
