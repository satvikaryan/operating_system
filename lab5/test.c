#include <stdio.h>
#include <string.h>

int main() {
    char satvik[100];  
    int n = sizeof(satvik);

    printf("Enter input: ");
    fgets(satvik, n, stdin);
    satvik[strcspn(satvik, "\n")] = '\0';

    printf("You entered: %s\n", satvik); 

    return 0;
}
