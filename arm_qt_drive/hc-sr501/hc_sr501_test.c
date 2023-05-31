#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>

int fd;
int val;

void my_sig_handler(int sig) {
    printf("sig !!");
    if (read(fd, &val, 4) == 4) {
        printf("value %d\n", val);
    }
}

int main(int argc, char **argv) {
    int i;
    int flags;
    int temp_f, temp_b, humidity_f, humidity_b;
    if (argc != 2) {
        printf("argc != 2");
        return -1;
    }
    
    fd = open(argv[1], O_RDWR | O_NONBLOCK);
    if (fd == -1) {
        printf("open err");
        return -1;
    }
    
    signal(SIGIO, my_sig_handler);
    
    fcntl(fd, F_SETOWN, getpid()); 
    flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | FASYNC);
    // while (1)  {
    //     if (read(fd, &val, 4) == 4) {
    //         printf("value %d\n", val);
    //     }
    //     sleep(1);
    // }
    
    while (1) {
        sleep(2);
    }

    close(fd);

    return 0;
}