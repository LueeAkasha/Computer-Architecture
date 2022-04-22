#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char** argv){

    const FILE* input = stdin;
    const FILE* output = stdout;
    const FILE* debug = stderr;

    bool on_encrypt_p = false;
    bool on_encrypt_n = false;
    bool on_debug = false;

    const char* debug_flag = "-D";
    const char* encrypt_flag_p = "+e";
    const char* encrypt_flag_n = "-e";
    
    int key = 0;
    const char A = 'A';
    const char F = 'F';
    for(int i = 1; i < argc; i=i+1){
        if (strncmp(argv[i], debug_flag, 2)==0){
            
            on_debug = true;
            
        }
       else if (strncmp(argv[i], encrypt_flag_p, 2)==0){
            on_encrypt_p = true;
        } 
        else if (strncmp(argv[i], encrypt_flag_n, 2)==0){
            on_encrypt_n = true;
        }
        if(on_encrypt_p || on_encrypt_n){
            char char_key = *(argv[i]+2);

            if (char_key>= A && char_key <=F ){
                key = char_key - 55;
            }
            else{
                key = char_key - 48;
            }
        }
    }
if(on_debug){
		fprintf(debug, "\n%s\n", debug_flag);
	}
    if(on_encrypt_n){
        key = key * -1;
    }
    
    if(on_encrypt_p || on_encrypt_n){
        char character = fgetc(input);
        while (character != EOF){
            if(character != 10){
                fputc(character+ key, output);
                if(on_debug)
                    fprintf(debug, "%d%5d\n",character-key, character);
            }

            character = fgetc(input);
             if(character == 10 || character == EOF){
                fputc('\n', output);
            }
        }
    }



    else{
        char character = fgetc(input);
        int numberOfLetters = 0;
		while(  character != EOF ){ 
		    if(character != 10){
                
                if ((character >= 'A') & (character <= 'Z')){
                    numberOfLetters++;
		    	    character = character + 32;
                    fputc(character, output);
                    if(on_debug)
                        fprintf(debug, "%d%5d\n",character-32, character);
		        }
                else{
                    fputc(character, output);
                    if(on_debug)
                        fprintf(debug, "%d%5d\n",character, character);
                }
                
            }
            character = fgetc(input);
            if(character == 10 && on_debug){
                fprintf(debug, "\nthe number of letters: %d\n\n", numberOfLetters);
                fputc('\n', output);
            }
        }
    }
}