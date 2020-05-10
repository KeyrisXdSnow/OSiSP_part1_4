#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

char world[1024] = "if";
int threads_amount = 0;
int opened_thread = 0 ;
int step  = 0 ;

void delete(char *str, int  start, int len){
    while (*(str+start++ - 1) = *(str+ start + len - 1)){}
}

void open_directory (DIR *dir, char path[]) {
    struct dirent *entry ;
    struct stat statistic;
    char filepath[400];

    while ( (entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 ^ strcmp(entry->d_name, "..") == 0) continue;

        strcpy(filepath, path);
        strcat(filepath, "/");
        strcat(filepath, entry->d_name);

        lstat(filepath, &statistic);

        if (S_ISDIR(statistic.st_mode)) {
            // open subdirectory
            DIR *new_dir = opendir(filepath);
            if (new_dir == NULL) {
                printf("Can not open directory \n");
            } else open_directory(new_dir, filepath);
            closedir(new_dir);
        }
        else if (statistic.st_mode) {
            if ( threads_amount != 0) {
                if (opened_thread == threads_amount) {
                    wait(0);
                    --opened_thread;
                }
                opened_thread++;
            }
            step++;

            pid_t pid ;
            if (threads_amount != 0) pid = fork();
            else pid = 0 ;

            if ( pid == 0) {
                FILE *file = fopen(filepath, "r");
                int count = 0;
                char *viewed_str;
                if (file == NULL) {
                    printf("Can not open file \n");
                    exit(0);
                    //continue;
                }
                char buffer[1024];
                int bytes = 0;
                while (fgets(buffer, 1024, file) != NULL) {
                    bytes += strlen(buffer);
                    viewed_str = strstr(buffer, world);
                    while (viewed_str != NULL) {
                        ++count;
                        delete(viewed_str, 0, strlen(world));
                        viewed_str = strstr(viewed_str, world);
                    }
                }

                closedir(file);
                printf("parent %d --> pid %d, file_path : %s, bytes : %u, count of entries : %d \n", getppid(), getpid(), filepath, bytes, count);
                if (threads_amount != 0) exit(0);
            }
        }

    }
}

int find_directory (char path[]) {

    DIR *dir = opendir(path);
    if (dir == NULL ) {
        printf("Directory not exist\n");
        return -1;
    }
    open_directory(dir,path);
    closedir(dir);

    if (threads_amount != 0 ) {
        for (int i = 0; i < opened_thread; i++) { wait(NULL); }
    }

    return 0;

}

int main() {

    char str[400] = "/home/keyris/test/";
    threads_amount = 30;

    time(NULL);
    clock_t time = clock();

    //printf("Threads amount = ");
    //scanf("%d", &threads_amount);
    //printf("Put directory : ");
    //scanf( "%s", str);
    printf("Path : %s\n",str);

    if (str[strlen(str)-1] == '/')
        str[strlen(str)-1] = '\0';

    find_directory(str);

    time = clock()-time;
    printf("Time : %f",(double)time/CLOCKS_PER_SEC);

    return 0;
}


