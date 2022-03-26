#include "kernel/types.h"

#include "user/user.h"

#define READ 0
#define WRITE 1

int main(int argc, char  *argv[])
{
    /* code */
    // p2c means parent to child
    // c2p means child to parent
    int p2c[2] ,c2p[2];
    int pid;
    char *buff=0;
    if (pipe(p2c) != 0 || pipe(c2p) != 0 ) {
        printf("create pip error\n");
        exit(1);
    } 
    if (fork() == 0) {
        //child
        pid = getpid();
        
        //关闭父进程到子进程的写端 和子进程到父进程读端
        close(p2c[WRITE]);
        close(c2p[READ]);

        read(p2c[READ], buff,1) ;

        printf("%d: received ping\n",pid);
        write(c2p[WRITE], "x", 1);
        close(c2p[WRITE]);
        close(p2c[READ]);
        
        exit(0);  
    }else {
        //parent
        pid = getpid();
        // 关闭父进程到子进程的读端 和子进程到父进程写端
        close(p2c[READ]);
        close(c2p[WRITE]);

        write(p2c[WRITE], "x", 1);
        read(c2p[READ], buff,1);
        printf("%d: received pong\n",pid);
        close(p2c[WRITE]);
        close(c2p[READ]);
        
        exit(0);
    }
}
