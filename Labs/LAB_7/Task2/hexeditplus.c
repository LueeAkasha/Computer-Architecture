
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
void LoadIntoMemory(state*);
void MemoryDisplay(state*);
void SaveIntoFile(state*);
void MemoryModify(state*);
void Quit(state*);


char* unit_to_format(int unit_size, int isHex) {
    static char* hex_format[] = {"%01hhX\n", "%01hX\n", "No such unit", "%01X\n"};
    static char* dec_format[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};
    if(isHex){
        return hex_format[unit_size - 1];
    }
    else
    {
        return dec_format[unit_size - 1];
    }
}  


/************ These are the functions that actually do the work ***************/
/* Reads units from file */
void read_units_to_memory(FILE* fp, state* _state, int length) {
    fread(_state->mem_buf, _state->unit_size, length, fp);
    _state->mem_count += length;
}

/* Prints the buffer to screen by converting it to text with printf */
void print_units(FILE* output, state* _state,unsigned char* buffer, int length, int format) {
    unsigned char* end = buffer + _state->unit_size*length;
    
    while (buffer < end) {
        //print ints
        int var = *((int*)(buffer));
        fprintf(output, unit_to_format(_state->unit_size, format), var);
        buffer += _state->unit_size;
    }
}

/* Writes buffer to file without converting it to text with write */
void write_units(FILE* output, state* _state, char* buffer, int target_address, int length) {
    fseek(output, 0, SEEK_END);
    int file_size = ftell(output);
    fseek(output, target_address, SEEK_SET);
    
    if(target_address < file_size){
        fwrite(buffer, _state->unit_size, length, output);
    }
    else
    {
        fprintf(stderr, "<target-location> could not be greater than <file_name>\n");
    }
}
/*****************************************************************************/


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
    fprintf(stdout, "Enter file name\n> ");
    char fileName_input[NAME_LEN];
    fgets(fileName_input, NAME_LEN, stdin);
    sscanf(fileName_input, "%s", _state->file_name);
    if(_state->debug_mode){
        fprintf(stdout, "Debug: fine name set to %s\n", _state->file_name);
    }
}

void SetUnitSize(state* _state){
     fprintf(stdout, "Enter unit size\n> ");
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

void LoadIntoMemory(state* _state){
    if(strcmp(_state->file_name, "") == 0){
        fprintf(stdout, "empty filename!\n");
        return;
    }
    else
    {
        
        
        FILE* fp = fopen(_state->file_name, "r");
        if(fp){
            rewind(fp);
            char input[14]; 
            int location;
            int length;
            
            fprintf(stdout, "Please enter <location> <length>\n> ");
            
            fgets(input, 14, stdin);
            sscanf(input, "%x %d", &location, &length);
            
            
            if(_state->debug_mode){
                fprintf(stdout, "%s     %x      %d\n", _state->file_name, location, length);
            }
            
            fseek(fp, location, SEEK_SET);
            read_units_to_memory(fp,_state, length);
            
            fprintf(stdout, "Loaded %d units into memory\n", length);
            fclose(fp);
        }
        else
        {
            fprintf(stderr, "file %s was not found!\n", _state->file_name );
            return;
        }
    }
    
}

void MemoryDisplay(state* _state){
    char input[20]; 
    int address;
    int length;
    fprintf(stdout, "Enter address and length\n> ");
    fgets(input, 20, stdin);
    sscanf(input, "%x %d", &address, &length);
    

    if (address){
        unsigned char* ptr = (unsigned char *) &address;
        fprintf(stdout, "\nHexdecimal\n===========\n");
        print_units(stdout, _state, ptr, length, 1);
        fprintf(stdout, "\nDecimal\n=======\n");
        print_units(stdout, _state, ptr, length, 0);
    }
    else
    {
        fprintf(stdout, "\nHexdecimal\n===========\n");
        print_units(stdout, _state, _state->mem_buf, length, 1);
        fprintf(stdout, "\nDecimal\n=======\n");
        print_units(stdout, _state, _state->mem_buf, length, 0);
    }
    
}





void SaveIntoFile(state* _state){
    fprintf(stdout, "Enter <source-address> <target-address> <length>\n> ");
    int source_address;
    int target_address;
    int length;

    char input[20];
    fgets(input, 20, stdin);
    sscanf(input, "%x %x %d", &source_address, &target_address, &length);


    if (source_address == 0)
    {
        source_address = (int) _state->mem_buf;
    }
    else{
        
        source_address += (int)_state->mem_buf;
    }

    
    FILE* fp = fopen(_state->file_name, "r+");

    if(fp){
        if (_state->debug_mode)
        {
            fprintf(stdout, "%s file was opened for wtiting successfully!\n", _state->file_name);
        }
        fseek(fp, target_address, SEEK_SET);
        write_units(fp, _state, (char *)source_address, target_address, length);
    }
    else
    {
         if (_state->debug_mode)
        {
            fprintf(stdout, "Failed to open %s file for wtiting!\n", _state->file_name);
        }
    }
    


}

void MemoryModify(state* _state){
    fprintf(stdout, "Please enter <location> <val>\n");
    char input[100];
    int location;
    int val;

    fgets(input, 100, stdin);
    sscanf(input, "%x %x", &location, &val);

    

    if(_state->debug_mode){
        fprintf(stdout, "location: %x val: %x\n", location, val);
    }
    memcpy(_state->mem_buf + location, &val, _state->unit_size);

}


void FindFunctionFileOffset(state* _state){
    int function_virtual_address;
    int section_virtual_address;
    int section_file_offset;
    char input[100];
    fprintf(stdout, "Please enter <section file offset> <section virtual address> <function virtual address>\n");
    fgets(input, 100, stdin);
    sscanf(input, "%x %x %x", &section_file_offset, &section_virtual_address, &function_virtual_address);
    int function_file_offset =  section_file_offset + (function_virtual_address - section_virtual_address);
    fprintf(stdout, "Function file offset is %x\n", function_file_offset);   
}



void Quit(state* _state){
    if (_state->debug_mode)
    {
        free(_state);
        fprintf(stdout, "quitting\n");
    }
    exit(0);
}

int main(int argc, char ** argv){
	int limit = 8;
    state* _state = calloc(1, sizeof(state));
	struct fun_desc menu[] = { { "Toggle Debug Mode", &ToggleDebugMode },{"Set File Name",&SetFileName},{"Set Unit Size",&SetUnitSize},{"Load Into Memory",&LoadIntoMemory},{"Memory Display",&MemoryDisplay} , { "Save Into File", &SaveIntoFile }, { "Memory Modify", &MemoryModify }, { "Quit", &Quit }, {"FindFunctionFileOffset", &FindFunctionFileOffset}, { NULL, NULL } };
	
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
		printf("> ");
		
        
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
