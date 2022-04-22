#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
int main(int argc, char** argv){

    const char* debug_flag = "-D";
    bool on_debug = argc>1 && strlen(argv[1])==2 && strncmp(argv[1], debug_flag,2)==0;
    FILE* input = stdin;
	FILE* debug = stderr;
    FILE* output = stdout;
    char character= fgetc(input);
    char c;
    int numberOfLetters=0;
	if(on_debug){
		fprintf(debug, "\n%s\n", debug_flag);
	}
    else{
		printf("\n");
	}
		while(character != EOF ){
            if(character != 10){
                c = character;
                if(character>= 'A' && character<= 'Z'){
                 c = character + 32;
                  numberOfLetters++;  
                }
                if (on_debug){
                    fprintf(debug, "%d%5d\n",character, c);
                }
				fputc(c, output);
                      
            }
           
              

            character = fgetc(input);
         
			if(character == 10 ){
                if(on_debug){
                    fprintf(debug, "\nthe number of letters: %d\n\n", numberOfLetters);
                }
                fputc('\n', output);
            }
        }
        
        exit(0);       
}