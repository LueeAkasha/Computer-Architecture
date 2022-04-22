#include "util.h"
extern int system_call();
#define SYS_WRITE 4
#define STDOUT 1


int main (int argc , char* argv[], char* envp[])
{
    system_call(SYS_WRITE,STDOUT,"Hello World! \n", 14);
    return 0;
}