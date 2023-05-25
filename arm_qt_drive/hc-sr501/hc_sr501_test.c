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
    int temp_f, temp_b, humidity_f, humidity_b;
    if (argc != 2) {
        printf("argc != 2");
        return -1;
    }
    
    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        printf("open err");
        return -1;
    }
    
    while (1)  {
        if (read(fd, &val, 4) == 4) {
            printf("value %d\n", val);
        }
        sleep(1);
    }

    close(fd);

    return 0;
}