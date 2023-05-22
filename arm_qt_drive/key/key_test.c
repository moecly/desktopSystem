#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int fd;
    int val;
    int i;
    if (argc != 2) {
        printf("argc != 2");
        return -1;
    }
    
    fd = open(argv[1], O_RDWR | O_NONBLOCK);
    if (fd == -1) {
        printf("open err");
        return -1;
    }
    
    for (i = 0; i < 20; i++) {
        if (read(fd, &val, 4) == 4) {
            printf("button value 0x%x\n", val);
        } else {
            printf("get err");
        }
    }
    
    close(fd);

    return 0;
}