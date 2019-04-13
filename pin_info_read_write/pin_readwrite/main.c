#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
extern int errno; 
int main() 
{      
    char buffer[1000000];
    // if file does not have in directory  
//    malloc(100 * sizeof(char));
    // then file foo.txt is created. 
      
    int fd = open("input.txt", O_RDONLY | O_CREAT);  
    read(fd, buffer, 1000000);
    read(fd, buffer, 5000);
    read(fd, buffer, 500000);

    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int write_count = write(fd, buffer, strlen(buffer));
    write_count = write(fd, buffer, 650);
    if (fd ==-1) 
    { 
        // print which type of error have in a code 
        printf("Error Number % d\n", errno);  
          
        // print program detail "Success or failure" 
        perror("Program");                  
    } 
    return 0; 
} 
