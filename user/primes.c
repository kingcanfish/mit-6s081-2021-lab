#include "kernel/types.h"
#include "user/user.h"
#define MAX 35
#define READ 0
#define WRITE 1

void child_do(int *left);


int main(int argc, char const *argv[])
{
    int right[2];
    int wstatus;
    pipe(right);

    if (fork() == 0) {
        child_do(right);
    }else {
        close(right[READ]);
        for (int i =2 ; i <= MAX; i++ ) {
            write(right[WRITE], &i,sizeof(int));
        }
        close(right[WRITE]);
        wait(&wstatus);
    }
    exit(0);
}


void child_do(int *left) {
    close(left[WRITE]);
    int right[2];
    pipe(right);
    int number;
    int flag;
    int wstatus;
    if (read(left[READ],&flag,sizeof(int)) == 0) {
        exit(0);
    }

    printf("prime %d\n", flag);

    if (fork()== 0 ) {
        child_do(right);
    }else {
        close(right[READ]);
        while (read(left[READ],&number,sizeof(int)) == sizeof(int)) {
            if (number % flag != 0 ) {
                write(right[WRITE], &number,sizeof(int));
            }
        }
        close(right[WRITE]);
        wait(&wstatus);
    }
    exit(0);
}


