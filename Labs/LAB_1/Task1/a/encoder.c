#include <stdio.h>

int main(){
	FILE* input = stdin;
	FILE* output = stdout;
	int character = fgetc(input);
			while(  character != EOF ){ 
			if ((character >= 'A') & (character <= 'Z')){
				character = character + 32;
			}
			fprintf(output, "%c", character);
			character = fgetc(input);
	}

}
