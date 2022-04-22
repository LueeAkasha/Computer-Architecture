#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include "LineParser.h"
#include <sys/stat.h>
#include <fcntl.h>

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0


typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
void print_list(process** process_list);

void clean_list(process** head);
char* status_string(int);

void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);
void redirection(cmdLine *pCmdLine);
void pipe_execute(cmdLine *pCmdLine1, cmdLine *pCmdLine2);
void execute(cmdLine *pCmdLine);
void print_history(char** history, int n);
void free_history(char** history, int n);


process * process_list = NULL;

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){    
    if(*process_list == NULL){
        process *  processToAdd = calloc(1, sizeof(process));
       processToAdd->cmd = cmd;
       processToAdd->status = RUNNING;
       processToAdd->pid = pid;
       processToAdd->next = NULL;
       
       *process_list =  processToAdd;
    }

    else if ((*process_list)->next != NULL){
        addProcess(&(*process_list)->next, cmd, pid);
        
    }
    else
    {
        process *  processToAdd = calloc(1, sizeof(process));
       processToAdd->cmd = cmd;
       processToAdd->status = RUNNING;
       processToAdd->pid = pid;
       processToAdd->next = NULL;
        (*process_list)->next =  processToAdd;

    }
    
    
}

void printProcessList(process** process_list){ // <process id> <the command> <process status>
    if(*process_list != NULL)
        updateProcessList(process_list);
    fprintf(stdout, "PID          Command      STATUS\n");
    if(*process_list != NULL)
        print_list(process_list);
    clean_list(process_list);
}

void print_list(process** process){
    if ((*process) != NULL)
    {
        if ((*process)->cmd != NULL)
        {
        pid_t pid = (*process)->pid;
        char* command = ((*process)->cmd)->arguments[0];
        char* status = status_string((*process)->status);
        fprintf(stdout, "%d          %s      %s\n", pid, command, status);
       print_list(&(*process)->next);
       }
    }
     
    
}



void clean_list(process** head){
    process* temp = *head;
    process* prev;

    while (temp != NULL && temp->status == TERMINATED)
    {
        
        *head = temp->next;
        freeCmdLines(temp->cmd);
        //fprintf(stdout, "cmd mem freed in line: 145 \n");
        free(temp);
        //fprintf(stdout, "process mem freed in line: 96 \n");
        temp = *head;
    }
    while (temp != NULL)
    {
        
        while (temp != NULL && temp->status != TERMINATED)
        {
            prev = temp;
            temp = temp->next;
        }
        
        
        if (temp == NULL)
        {
            return;
        }

        prev->next = temp->next;
        freeCmdLines(temp->cmd);
        //fprintf(stdout, "cmd mem freed in line: 145 \n");
        free(temp);
        //fprintf(stdout, "process mem freed in line: 115 \n");

        temp = prev->next;
        
    }
}

char* status_string(int status){
    char* output ="";
    if (status == 0){
        output = "SUSPENDED";
    }
    else if (status == 1) {
        output = "RUNNING";
    }
    else if (status == -1) {
        output = "TERMINATED";
    }
    return output;
    
}

void freeProcessList(process* process_list){
    
    process* temp= process_list;
    while (process_list != NULL)
    {
        process_list = temp->next;
        freeCmdLines(temp->cmd);
        //fprintf(stdout, "cmd mem freed in line: 145 \n");
        free(temp);
        //fprintf(stdout, "process mem freed in line: 147 \n");
        temp = process_list;  
    }

}


void updateProcessList(process **process_list){
   int  status =0;
   int updated_status;
   int res = waitpid((*process_list)->pid, &status , WNOHANG | WUNTRACED | WCONTINUED); 
        if (res)
        {
            if (WIFEXITED(status) | WIFSIGNALED(status)){
                updated_status = TERMINATED;
            }

            else if(WIFSTOPPED(status)){
                updated_status = SUSPENDED;
            }
            else if(WIFCONTINUED(status)){
                updated_status = RUNNING;
            }
            updateProcessStatus(*process_list, (*process_list)->pid, updated_status);
        }
        
    
    if ((*process_list)->next != NULL)
    {
        updateProcessList(&(*process_list)->next);
    }
    
}
void updateProcessStatus(process* process_list, int pid, int status){
    if (process_list->pid == pid){
        process_list->status = status;
        
    }
    else if (process_list->next != NULL){
        updateProcessStatus(process_list->next, pid, status);
    }
}

void redirection(cmdLine *pCmdLine){
    if (pCmdLine->inputRedirect)
            {
                int fd = fileno(fopen(pCmdLine->inputRedirect, "r"));
                dup2(fd , 0);
            }      
            if (pCmdLine->outputRedirect)
            {
                int fd = fileno(fopen(pCmdLine->outputRedirect, "w+"));
                dup2(fd, 1);
            }
}
void pipe_execute(cmdLine *pCmdLine1, cmdLine *pCmdLine2){
    int fd[2];
    if (pipe(fd)){
        perror("Error: ");
        return;
    }
    pid_t child1 = fork();
    if (child1 == 0){
        close(1);        
        dup(fd[1]);
        close(fd[1]);
        redirection(pCmdLine1);
        execvp(pCmdLine1->arguments[0], pCmdLine1->arguments);
    }
    
    close(fd[1]);

    pid_t child2 = fork();
    if (child2 == 0)
    {
        close(0);
                
        dup(fd[0]);
        close(fd[0]);
        redirection(pCmdLine2);
        execvp(pCmdLine2->arguments[0], pCmdLine2->arguments);
    }

    close(fd[0]);

    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);
}

void execute(cmdLine *pCmdLine){
     int childPid = fork();
        if (childPid  == 0){
            //sleep(20); //for testing
            redirection(pCmdLine);
            execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            perror("ERROR: ");
            exit(1);
        }

        
        addProcess(&process_list, pCmdLine, childPid);
        
        if(pCmdLine->blocking){
            waitpid(childPid, NULL, 0);
        }
        
}

void print_history(char** history, int n){

    for (size_t i = 0; i < n; i++)
    {
        fprintf(stdout, "#%d %s \n", i, history[i]);
    }
    
}

void free_history(char** history, int n){
    for (size_t i = 0; i < n; i++)
    {
        
        free(history[i]);
        
        
        
    }
}

int main(int argc, char** argv){  
    char input[2048];
    char* history[10];
    int p = 0;
    char ptr[PATH_MAX];
    while(1){
        // 1
        getcwd(ptr, PATH_MAX);
        fprintf(stdout, ptr, "%s");
        fprintf(stdout, "$ ");

        // 2
        fgets(input, 2048, stdin);
        
        if (strncmp(input, "!", 1)==0)
        {
            int index = atoi(input+1);
            if (index >= 0 && index < p)
            {
                strcpy(input, history[index]);
            }
            else
            {
                perror("Error: index is out of bound! ");
                continue;
            }
            
            
        }
        
        if (strstr(input, "|")){
            char* pipe_left = calloc(2048, 1);
            char* pipe_right = calloc(2048, 1);
            strncpy(pipe_left, input, (int)(strchr(input, '|') - input));
            strcpy(pipe_right, input + (int)(strchr(input, '|') - input)+1);
            cmdLine* left_command = parseCmdLines(pipe_left);
            cmdLine* right_command = parseCmdLines(pipe_right);

            pipe_execute(left_command, right_command);

            free(pipe_left);
            free(pipe_right);
            freeCmdLines(left_command);
            freeCmdLines(right_command);
            
            history[p] = malloc(2048);
            memcpy(history[p], input, 2048);
            p++;
            continue;
        }
        
        
        // 3
        cmdLine* pCmdLine = parseCmdLines(input);
	    


        if (strcmp(pCmdLine->arguments[0], "history")==0)
        {
            freeCmdLines(pCmdLine);
            print_history(history, p);
            history[p] = malloc(2048);
            memcpy(history[p], input, 2048);
            p++;
            continue;
        }
        //fprintf(stdout, "cmd mem allocated in line: 231 \n");   


        if (strcmp(pCmdLine->arguments[0], "cd")==0){
            char* newPath;
            int last_index = strlen(ptr)-1;
            
            if (pCmdLine->argCount == 1){
                for (unsigned i = 1; i < strlen(ptr); i++)
                {
                    if(ptr[i] == '/'){
                        last_index = i;
                        break;
                    }
                }
                
            }
            else if ((pCmdLine->argCount>1 && strcmp(pCmdLine->arguments[1], "..")==0))
            {
                for (unsigned i = strlen(ptr)-1; i >= 0; i--)
                {
                    if(ptr[i] == '/'){
                       last_index = i;
                       break;
                    }
                }
            }
            else if (pCmdLine->argCount > 1)
            {
                strcat(ptr, "/");
                strcat(ptr, pCmdLine->arguments[1]);
                last_index = strlen(ptr);
            }
            
            
            newPath = calloc( last_index + 1,1);


            memcpy(newPath, ptr,last_index);
            if (chdir(newPath) != 0) 
            {
                perror("Error: "); 
                
            }
            free(newPath);

            addProcess(&process_list, pCmdLine, getpid());
            history[p] = malloc(2048);
            memcpy(history[p], input, 2048);
            p++;
            continue;
            
        }
        if (strcmp(pCmdLine->arguments[0], "kill")== 0)
        {
            if(kill(atoi(pCmdLine->arguments[1]), SIGINT)){
                fprintf(stdout, "failed to kill process with pid: %d!\n",atoi(pCmdLine->arguments[1]));
            }
            else
            {
                fprintf(stdout, "procsses with pid: %d was killed successfully!\n",atoi(pCmdLine->arguments[1]));
            }
            addProcess(&process_list, pCmdLine, getpid());
            history[p] = malloc(2048);
            memcpy(history[p], input, 2048);
            p++;
            continue;
            
        }
        if (strcmp(pCmdLine->arguments[0], "suspend")== 0)
        {
            pid_t child_pid = fork();
            if (child_pid  == 0)
            {
                kill(atoi(pCmdLine->arguments[1]), SIGTSTP);
                sleep(atoi(pCmdLine->arguments[2]));
                kill(atoi(pCmdLine->arguments[1]), SIGCONT);
                exit(1);
            }
            addProcess(&process_list, pCmdLine, child_pid);
            history[p] = malloc(2048);
            memcpy(history[p], input, 2048);
            p++;
            continue;
        }
        
        
        // 6
        if(strcmp(pCmdLine->arguments[0], "quit") == 0){
            freeCmdLines(pCmdLine);
             //fprintf(stdout, "cmd mem freed in line: 314\n");   
            break;
        }
        if (strcmp(pCmdLine->arguments[0], "procs")== 0){
            printProcessList(&process_list);
           // fprintf(stdout, "cmd mem freed in line: 319 \n");   
            freeCmdLines(pCmdLine);
            history[p] = malloc(2048);
            memcpy(history[p], input, 2048);
            p++;
            continue;
        }
        // 4
        execute(pCmdLine);
        history[p] = malloc(2048);
        memcpy(history[p], input, 2048);
        p++;
    }
    freeProcessList(process_list);
    free_history(history, p);
    exit(EXIT_SUCCESS);
}