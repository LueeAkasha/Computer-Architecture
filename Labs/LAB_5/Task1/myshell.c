#include <stdio.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include "LineParser.h"





void execute(cmdLine *pCmdLine){
     int childPid = fork();
        if (childPid  == 0){
            //sleep(10); //for testing

           execvp(pCmdLine->arguments[0], pCmdLine->arguments);
    
                perror("Error: ");
                _exit(EXIT_FAILURE);
            //freeCmdLines(pCmdLine);


        }
        if(pCmdLine->blocking){
                    waitpid(childPid, NULL, 0);
        }
       
        
}

int main(int argc, char** argv){
    char input[2048];
    char ptr[PATH_MAX];
    while(1){
        // 1
        getcwd(ptr, PATH_MAX);
        fprintf(stdout, ptr, "%s");
        fprintf(stdout, "$ ");

        // 2
        fgets(input, 2048, stdin);

        // 3
        cmdLine* pCmdLine = parseCmdLines(input);

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
            freeCmdLines(pCmdLine);
            continue;
            
        }
        // 6
        if(strcmp(pCmdLine->arguments[0], "quit") == 0){
            freeCmdLines(pCmdLine);
            break;
        }
        // 4
        execute(pCmdLine);
        freeCmdLines(pCmdLine);
    }
    exit(EXIT_SUCCESS);
}