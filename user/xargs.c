#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#define MAXLEN 100

int main(int argc, char  *argv[]) {
    char cache[MAXARG][MAXLEN];
    char buff;
    char * command = argv[1];
    
        // 参数缓存置0 
                // printf("ok");
                // printf("%d %d %d",&buff,command,cache);
        memset(cache , 0, MAXARG*MAXLEN);

        // argv[0] 是 xargs
        for (int i = 1; i < argc; i++) {
            strcpy(cache[i-1], argv[i]); 
        }

        // 读入的参数应该从 cache[argc-1][0] 开始存
        int start = argc -1;
        int idx = 0;


        while (read(0, &buff,1) == 1 ) {
            if (buff == ' ') {
                cache[start][idx] = '\0';
                start++;
                idx = 0;
            }else if (buff == '\n') {
                cache[start][idx] = '\0';
                // 为什么是start+2 ？ start是最后一个 索引 所以长度是start+1 但是需要额外的一个位置
                // 存放argv的 结束 NULL 所以是start+2
                char * new_arg[start+2];
                for (int i = 0 ; i < start+1;i++) {
                    new_arg[i] = cache[i];
                }
                // argsv 最后一个参数为null
                new_arg[start+1] = '\0';

                if (fork() == 0 ) {
                    exec(command,new_arg);
                    // 不应该到达
                    printf("exec : command faild!\n");
                    exit(0);
                }else {
                    wait((int *) 0);
                    start = argc -1;
                    idx = 0 ;
                    memset(cache[argc-1], 0, MAXARG*MAXLEN - (argc-1)*MAXLEN );
                }

            }else {
                cache[start][idx] = buff;
                idx++;
            }
        }
    exit(0); 
}

