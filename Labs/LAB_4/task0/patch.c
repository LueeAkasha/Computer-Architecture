#include "util.h"
extern int system_call();
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define lseek 19
#define SEEK_SET 0
#define RWMODE 2
#define STDOUT 1


int main (int argc , char* argv[], char* envp[])
{
    if(argc <3){
        system_call(SYS_WRITE, STDOUT, "args are missed!",16 );
        return -1;
    }
    int i = 0;
    
    
    char * filname = argv[1];
    char * output = argv[2];
    char * del = "\0\0\0\0\0";


    int descriptor = system_call(SYS_OPEN, filname, RWMODE);
    

    i = system_call(lseek, descriptor, 656, SEEK_SET);
    i = system_call(SYS_WRITE, descriptor, del, 5);
    i = system_call(lseek, descriptor, 656, SEEK_SET);
    i = system_call(SYS_WRITE, descriptor, output, strlen(output));
    i = system_call(lseek, descriptor, 656+strlen(output), SEEK_SET);
    i = system_call(SYS_WRITE, descriptor, "\n", strlen(output));

    if(i == -1){
        return -1;
    }


    i = system_call(SYS_CLOSE, descriptor);
    if(i == -1){
        return -1;
    }
    
    return 0;
}