#include<stdio.h>
#include <stdlib.h>
#include <string.h>

char* map(char *array, int array_length, char (*f) (char));

char censor(char c);

char encrypt(char c);/* Gets a char c and returns its encrypted form by adding 3 to its value. 
          If c is not between 0x20 and 0x7E it is returned unchanged */

char decrypt(char c); /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */

char cprt(char c); /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */

char my_get(char c); /* Ignores c, reads and returns a character from stdin using fgetc. */


char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
	char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
	for(unsigned i = 0; i < array_length ; i++){
		mapped_array[i] = f(array[i]); 
	}
	return mapped_array;
}


//Task 1b
char encrypt(char c){
	if (c >= 0x20 && c <= 0x7E){
        return c+3;
    }
    return c;
}

char decrypt(char c){
	if (c>= 0x20  && c <= 0x7E){
        return c-3;
    }
    return c;

}
char cprt(char c){
	
	if (c<=0x7E && c>= 0x20){
		printf("%c\n",c);
	}
	else{
		printf(".\n");
	}
	return c;
}

char my_get(char c){
	int out = fgetc(stdin);
	return out;
} 


struct fun_desc {
  char *name;
  char (*fun)(char);
};

int main(int argc, char ** argv){
	char carray[5];
	int limit = 4;
	struct fun_desc menu[] = { { "censor", &censor },{"encrypt",&encrypt},{"decrypt",&decrypt},{"cprt",&cprt},{"my_get",&my_get} , { NULL, NULL } };

	char input[2];
	while(1){
		printf("Please choose a function:\n");
		for(unsigned i = 0 ; ; i++){
			if (menu[i].name != NULL){
				printf("%d.%s\n", i,menu[i].name);
				
			}
			else{
				break;
			}
		}
		printf("Option: ");
		
		scanf("%s", input);
		fgetc(stdin);
		
		int func_num = 0;
		func_num= (int)atoi(input);
	
		if (func_num<=limit && func_num>=0){
			printf("\nWithin bounds\n");
			strcpy(carray, map(carray, 5, menu[func_num].fun));
			printf("DONE.\n\n");
		}
		else{
			printf("\nNot Within bounds\n");
			
			exit(0);
		}
	}

	return (0);
}
