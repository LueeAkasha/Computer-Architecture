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
    int input_descriptor = STDIN;
    int output_descriptor = STDOUT;
	
	int on_inputfile = 0;
	int on_outputfile = 0;
	
    
    const char* inputfile_flag = "-i";
	const char* outputfile_flag = "-o";
	
    int i;
    for(i = 1; i < argc; i=i+1){
       
		if (strncmp(argv[i], inputfile_flag, 2)==0){
			on_inputfile = 1;
            input_descriptor = system_call(SYS_OPEN,argv[i]+2 ,R_MODE);
            if (input_descriptor < 0){
                system_call(SYS_WRITE, STDERR, "No such file found!\n", 20);
                system_call(1,0x55);
            }
		}
        else if (strncmp(argv[i], outputfile_flag, 2)==0){
			on_outputfile = 1;
            output_descriptor = system_call(SYS_OPEN,argv[i]+2 ,W_MODE | CREATE, 0777);
        }
    }

        
        char character[1];
        int notTerminated = system_call(SYS_READ,input_descriptor, character,1);
        int numberOfLetters = 0;
		while(notTerminated){ 
            
		    if(character[0] != 10){
                
                if ((character[0] >= 'A') & (character[0] <= 'Z')){
                    numberOfLetters++;
		    	    character[0] = character[0] + 32;
                    system_call(SYS_WRITE, output_descriptor, character,1);                   
		        }
                else{
                    system_call(SYS_WRITE, output_descriptor, character, 1);
                }
                
            }            
             
            notTerminated = system_call(SYS_READ,input_descriptor, character,1);

            if(character[0] == 10){
                system_call(SYS_WRITE, output_descriptor, "\n",1);
            }
        }
    
    if(on_inputfile){
        system_call(SYS_CLOSE, input_descriptor);
        
    }
    if(on_outputfile){
        system_call(SYS_CLOSE, output_descriptor);
    }

    return system_call(1,0);
}