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
    
    if (read(fd, &val, 4) == 4) {
        printf("value %d\n", val);
        humidity_f = (val >> 24) & 0xff;
        humidity_b = (val >> 16) & 0xff;
        temp_f = (val >> 8) & 0xff;
        temp_b = (val) & 0xff;
        printf("temp = %d.%d, humidity = %d.%d\n", temp_f, temp_b, humidity_f, humidity_b);
    }

    close(fd);

    return 0;
}