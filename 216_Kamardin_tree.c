#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

int channel = -1;

bool inputChildren() 
{
    char c;
    int chld_num, son;
    char buf[100];
    pid_t pid;
    setbuf(stdin, NULL);
    scanf("%d", &chld_num);
    int fd[4 * chld_num];
    snprintf(buf, 100, "%d ", getpid());
    write(1, buf, strlen(buf));
    for(int i = 0; i < chld_num; ++i) 
    {
        pipe(fd + (i * 4));
        pipe(fd + (i * 4) + 2);
        setbuf(stdin, NULL);
        scanf("%d", &son);
        pid = fork();
        if (pid == 0) 
        {
            channel = fd[i * 4 + 1];
            read(fd[i * 4 + 2], &c, 1);
            return true;
        }
        else 
        {
            snprintf(buf, 100, "%d ", pid);
            write(1, buf, strlen(buf));
        }
    }
    write(1, "\n", 1);
    for(int i = 0; i < chld_num; ++i) 
    {
        write(fd[i * 4 + 3], &c, 1);
        read(fd[i * 4], &c, 1);
    }
    write(channel, &c, 1);
    return false;
}


int main() 
{
    int n;
    setbuf(stdin, NULL);
    scanf("%d", &n);
    bool more_chld = 1;
    while(more_chld) more_chld = inputChildren();
    return 0;
}