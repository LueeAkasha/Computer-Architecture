#include "util.h"

extern int system_call();
#define STDIN 0
#define STDOUT 1 
#define STDERR 2

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_LSEEK 19

#define R_MODE 0
#define W_MODE 1
#define RW_MODE 2
#define A_MODE 1024
#define CREATE 64

int main(int argc, char** argv){

    char character[0];
    char c;
    int numberOfLetters=0;
    int notTerminated = system_call(SYS_READ,STDIN, character,1);

		while(notTerminated){
            if(character[0] != 10){
                c = character[0];
                if(character[0]>= 'A' && character[0]<= 'Z'){
                 c = character[0] + 32;
                  numberOfLetters++;  
                }
                
                char* pchar = &c;
                system_call(SYS_WRITE, STDOUT, pchar ,1);
                      
            }
           
            notTerminated = system_call(SYS_READ, STDIN, character , 1);
         
			if(character[0] == 10 ){
                
                system_call(SYS_WRITE, STDOUT, "\n", 1);
            }
        }
        
        return 0;
}