#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int fd;
    char status;
    char read_status;
    if (argc != 3) {
        printf("argc != 3");
        return -1;
    }
    
    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        printf("open err");
        return -1;
    }
    
    read(fd, &read_status, 1);
    printf("read status = %d\n", read_status);
        
    if (strcmp(argv[2], "on") == 0) {
        status = 1;
        printf("on");
        write(fd, &status, 1);
    } else {
        status = 0;
        printf("off");
        write(fd, &status, 1);
    }
    
    close(fd);

    return 0;
}