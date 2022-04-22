#include "util.h"


int main(int argc, char** argv){
	int on_inputfile = 0;
	int on_outputfile = 0;
	
    const char* inputfile_flag = "-i";
	const char* outputfile_flag = "-o";
	
    int input_descriptor = 

    for(int i = 1; i < argc; i=i+1){
		if (strncmp(argv[i], inputfile_flag, 2)==0){
			on_inputfile = 1;
            input = fopen(argv[i]+2, "r");
		}
        else if (strncmp(argv[i], outputfile_flag, 2)==0){
			on_outputfile = 1;
            output = fopen(argv[i]+2, "w+");
		}
    }
    
        char character = fgetc(input);
        int numberOfLetters = 0;
		while(  character != EOF ){ 
		    if(character != 10){
                
                if ((character >= 'A') & (character <= 'Z')){
                    numberOfLetters++;
		    	    character = character + 32;
                    fputc(character, output);
		        }
                else{
                    fputc(character, output);
                }
                
            }
            character = fgetc(input);
            if(character == 10 || character == EOF){
                fputc('\n', output);
            }
        }
    }

    if(on_inputfile){
        fclose(input);
    }
    if(on_outputfile){
        fclose(output);
    }
}