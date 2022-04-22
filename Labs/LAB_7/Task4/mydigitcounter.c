
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

 int  digit_counter(char*);

int digit_counter(char* str){
    int output = 0;
    while (*str != '\0')
    {
        ++output;
        str = str + 1;
    }
    return output;
}

int main(int argc, char ** argv){
    fprintf(stdout, "%d\n", digit_counter(argv[1]));
	return (0);
}
