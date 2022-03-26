#include "kernel/types.h"

#include "kernel/fs.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

#include "user/user.h"

char* fmtname(char *path) {
    static char *p;

    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    return p;
}
   
void find(char *path, char *file_name);
void find_dir(char *path, char *file_name);
   
int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(2, "ERROR: You need pass in only 2 arguments\n");
		exit(1);
	}
	char *target_path = argv[1];
	char *target_file = argv[2];
	find(target_path, target_file);
	exit(0);
}


void find(char *path, char *file_name) {

    int fd;
    
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type) {
        case T_FILE: {
            //有可能出现给出的path 是文件的情况 那我们之间返回这个文件的名字就行

            char* current_path;
            current_path = fmtname(path);
            if (strcmp(file_name, current_path) == 0 ) {
                printf("%s\n",path);     
            }
            close(fd);
            break;
            }

        case T_DIR:
            close(fd);
            find_dir(path, file_name);
    }
}

void find_dir(char *path, char *target_file) {
	int fd;
	struct stat st;
	struct dirent de;
	char buf[512], *p;
   
	if ((fd = open(path, 0)) < 0) {
		fprintf(2, "ERROR: cannot open %s\n", path);
		return;
	}
   	
	if (fstat(fd, &st) < 0) {
		fprintf(2, "ERROR: cannot stat %s\n", path);
		close(fd);
		return;
	}
   
	while (read(fd, &de, sizeof(de)) == sizeof(de)) {
		strcpy(buf, path);
		p = buf+strlen(buf);
		*p++ = '/';
		if (de.inum == 0)
			continue;


		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
   
		if (stat(buf, &st) < 0) {
			fprintf(2, "ERROR: cannot stat %s\n", buf);
		}
   
		switch (st.type) {
		case T_FILE:
			if (strcmp(target_file, de.name) == 0) {
				printf("%s\n", buf);
			}
			break;
		case T_DIR:
			if ((strcmp(de.name, ".") != 0) && 
            (strcmp(de.name, "..") != 0)) {
				find_dir(buf, target_file);
            }
		}	
	}
    close(fd);
	return;
}


