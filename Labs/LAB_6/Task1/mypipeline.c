#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char**argv){
    int on_debug = 0;
    for (unsigned i = 0; i < argc; i++)
    {
        if (strncmp("-d", argv[i],2) == 0)
        {
            on_debug = 1;
        }
    }
    int fd[2];
    if (pipe(fd)){
        return 1;
    }
    if (on_debug)
    {
        fprintf(stderr, "parent_process>forking...\n");
    }

    pid_t child1 = fork();
    if (child1 == 0){
        close(1);
        if (on_debug)
        {
            fprintf(stderr, "child1>redirecting stdout to the write end of the pipe...\n");
        }
        
        dup(fd[1]);
        close(fd[1]);
        char* command1[3] = {"ls", "-l", NULL};
        if (on_debug)
        {
            fprintf(stderr, "child1>going to execute cmd: ...\n");
        }
        execvp(command1[0], command1);
    }
    else
    {
        if (on_debug)
        {
            fprintf(stderr, "parent_process>created process with id: %d\n", child1);
        }
        
        
    }
    
    if (on_debug)
    {
      fprintf(stderr, "parent_process>closing the write end of the pipe...\n");
    }
    

    //close(fd[1]);

    if (on_debug)
    {
        fprintf(stderr, "parent_process>forking...\n");
    }
    
    pid_t child2 = fork();
    if (child2 == 0)
    {
        close(0);
        if (on_debug)
        {
            fprintf(stderr, "child2>redirecting stdin to the read end of the pipe...\n");
        }
        
        
        dup(fd[0]);
        close(fd[0]);
        char* command2[4] = {"tail", "-n", "2", NULL};
        if (on_debug)
        {
            fprintf(stderr, "child2>going to execute cmd: ...\n");
        }
        execvp(command2[0], command2);
    }
    else
    {
        if (on_debug)
        {
            fprintf(stderr, "parent_process>created process with id: %d\n", child2);
        }
        
        
    }
    if (on_debug)
    {
        fprintf(stderr, "parent_process>closing the read end of the pipe...\n");
    }
    close(fd[0]);
    
    if (on_debug)
    {
        fprintf(stderr, "parent_process>waiting for child procsses to terminate...\n");
    }
    
    
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    if (on_debug)
    {
        fprintf(stderr, "parent_process>exiting...\n");
    }    
    _exit(0);
    
}