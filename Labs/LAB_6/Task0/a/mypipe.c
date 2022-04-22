#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>



int main(){
    int fd[2];
    if (pipe(fd))
    {
        return  1;
    }
    int child_pid = fork();
    if(child_pid  == 0){
        close(fd[0]);
        
        write(fd[1], "hello", 5);
        close(fd[1]);
        

    }
    else
    {
        close(fd[1]);
        char message[5];
        read(fd[0],message,  5);
        close(fd[0]);
        fprintf(stdout, "%s\n", message);
    }
    return 0;
    
}