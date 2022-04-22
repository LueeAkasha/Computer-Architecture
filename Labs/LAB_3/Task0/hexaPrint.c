#include <stdio.h>
#include <stdlib.h>


void PrintHex(char* buffer, int length){
	for(unsigned i = 0 ; i < length ; ++i){
		unsigned char temp =*(buffer + i);
		printf("%02X  ", temp);
	}
	printf("\n");
}


int main(int argc, char **argv) {
    
	FILE *fp;
	fp = fopen(argv[1], "r");
	char* buffer = (char*) malloc(1024);
	
	fread(buffer, 1 ,  1024, fp);
	fseek(fp, 0, SEEK_END);
	PrintHex(buffer, ftell(fp) );
	fclose(fp);
	free(buffer);
	
	
    return 0;
}