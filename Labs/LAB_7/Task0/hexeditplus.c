#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NAME_LEN 128
#define BUF_SZ 10000

typedef struct
{
    char debug_mode;
    char file_name[NAME_LEN];
    int unit_size;
    unsigned char mem_buf[BUF_SZ];
    size_t mem_count;
    /*
    .
    .
    .
    .
    Any additional fields you deem necessary
    */    
} state;
void execute(state*, void (*f)(state*));
void ToggleDebugMode(state*);
void SetFileName(state*);
void SetUnitSize(state*);
void Quit(state*);


void execute(state* _state, void (*f)(state*)){
    f(_state);
}

struct fun_desc {
  char *name;
  void (*fun)(state*);
};

void ToggleDebugMode(state* _state){
    if(_state->debug_mode){
        _state->debug_mode = 0;
        fprintf(stdout, "Debug flag now off\n");
    }
    else
    {
        _state->debug_mode = 1;
        fprintf(stdout, "Debug flag now on\n");
    }
}

void SetFileName(state* _state){
    char fileName_input[NAME_LEN];
    fgets(fileName_input, NAME_LEN, stdin);
    sscanf(fileName_input, "%s", _state->file_name);
    if(_state->debug_mode){
        fprintf(stdout, "Debug: fine name set to %s\n", _state->file_name);
    }
}

void SetUnitSize(state* _state){
    char input_number = fgetc(stdin);
    int number = atoi(&input_number);
    if (number == 1 || number == 2 || number == 4)
    {
        _state->unit_size = number;
        if (_state->debug_mode)
        {
            fprintf(stdout, "Debug: set size to %d\n", _state->unit_size);
        }
        
    }
    else
    {
        fprintf(stderr, "Invalid size\n");
    }
}


void Quit(state* _state){
    if (_state->debug_mode)
    {
        fprintf(stdout, "quitting\n");
    }
    exit(0);
}

int main(int argc, char ** argv){
	int limit = 7;
    state* _state = calloc(1, sizeof(state));
	struct fun_desc menu[] = { { "Toggle Debug Mode", &ToggleDebugMode },{"Set File Name",&SetFileName},{"Set Unit Size",&SetUnitSize},{"Load Into Memory",&LoadIntoMemory},{"Memory Display",NULL} , { "Save Into File", NULL }, { "Memory Modify", NULL }, { "Quit", &Quit }, { NULL, NULL } };
	
    int input;
	while(1){
		printf("Please choose a function:\n");
		for(unsigned i = 0 ; ; i++){
			if (menu[i].name){
				printf("%d.%s\n", i,menu[i].name);
			}
			else{
				break;
			}
		}
		printf("Option: ");
		
        
        int result = EOF;
        while (result == EOF)
        {
            result = scanf("%d", &input);
        }
        fgetc(stdin);
	
		if (input<=limit && input>=0){
			execute(_state, menu[input].fun);
            fprintf(stdout, "\n");
		}
		else{
			printf("\nNot Within bounds\n");
			exit(0);
		}
	}

	return (0);
}
