#include "kernel/types.h"

#include "user/user.h"


int main(int argc, char  *argv[])
{
    int times;
    if (argc < 2) {
        printf("no argment");
        exit(0);
    }
    times  = atoi(argv[1]);
    sleep(times);
    exit(0);
}
