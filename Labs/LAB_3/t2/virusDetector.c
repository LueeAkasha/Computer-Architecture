#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define swap(x) ((x >> 8) | (x << 8))



typedef struct virus {
	unsigned short SigSize;
	char virusName[16];
	unsigned char* sig;
} virus;

typedef struct link link;
struct link{
	link *nextVirus;
	virus *vir;
};

struct fun_desc {
  char *name;
  void (*fun)(link*);
};

virus* readVirus(FILE* fp);
void free_virus(virus* virus);
void printVirus(virus* virus, FILE* output);
void list_print(link *virus_list, FILE* fp);
void list_append(link* virus_list, virus* data);
void list_free(link* virus_list);
void Load_signatures(link* head);
void Print_signatures(link* head);
void Detect_viruses(link* head);
void virus_detect(char* buffer, unsigned int size, link* virus_list, FILE* output);
void Fix_file();
void kill_virus(char* fileName, int signatureOffset, int signatureSize);
void map(link* virus, void (*f)(link*));

virus* readVirus(FILE* fp){
	int cursor = ftell(fp);
	
	
	fseek(fp, 0 , SEEK_END);
	int end = ftell(fp);
	fseek(fp,cursor,SEEK_SET);
	
	if(cursor== end) return NULL;
	
	virus* v = (virus*)calloc(1,sizeof(*v)); // to return
	
	fread(v, 1, 18, fp);
	v->sig = (unsigned char *)calloc(v->SigSize,sizeof(unsigned char));
	
	fread(v->sig,1 , v->SigSize,fp);
	return v;
}

void free_virus(virus* virus){
	free(virus->sig);
	free(virus);
}

void printVirus(virus* virus, FILE* output){
	
	char* VirusName = "Virus name: ";
	fprintf(output,  "%s" ,VirusName);
	fprintf(output, "%s" , virus->virusName);
	fprintf(output, "\n");
	
	char* VirusSize = "Virus size: ";
	fprintf(output,  "%s" ,VirusSize);
	fprintf(output, "%d" , virus->SigSize);
	fprintf(output, "\n");
	
	 
	
	char* Signature = "signature:";
	fprintf(output, "%s" , Signature);
	fprintf(output, "\n");
	
	for(unsigned i = 1; i <virus->SigSize; ++i){ 
		fprintf(output, "%02X ", *(virus->sig+i-1) & 0xff);
		if(i%20 == 0 && i<virus->SigSize){
			fprintf(output, "\n");
		}
	}
	fprintf(output, "\n\n");
}




void list_print(link *virus_list, FILE* fp){
	if(virus_list == NULL){
		return;
	}
	if(virus_list->vir == NULL){
		return;
	}
	printVirus(virus_list->vir, fp);
	if(virus_list->nextVirus != NULL){
		list_print(virus_list->nextVirus, fp);
	}
}

void list_append(link* virus_list, virus* data){ // to last of list.
	if (virus_list->vir == NULL)
	{
		virus_list->vir = data;
	}
	
	else if(virus_list->nextVirus == NULL){
		link* newNextLink = calloc(1, sizeof(*newNextLink));
		newNextLink->nextVirus = NULL;
		newNextLink->vir = data;
		virus_list->nextVirus = newNextLink;
	}
	else{
		list_append(virus_list->nextVirus, data);
	}
}

void list_free(link* virus_list){
	if(virus_list == NULL) return;
	
	if (virus_list->nextVirus != NULL)
	{
		list_free(virus_list->nextVirus);
	}
	
	if (virus_list->vir != NULL )
	{
		free_virus(virus_list->vir);
	}
	free(virus_list);
}



void Load_signatures(link* head){

	char input[1024];
	char * sigFileName = (char *)calloc(1024,1);
	printf("Enter name of signatures file, please: \n");
	fgets(input, 1024, stdin);
	if(strlen(input)>1){
	memcpy(sigFileName, input,strlen(input)-1);
	FILE* signaturesFile = fopen(sigFileName, "r");
	fseek(signaturesFile, 4,SEEK_SET);
	
	while(1){
		virus* v = readVirus(signaturesFile);
		
		if(v == NULL) break;
		list_append(head, v);
	}
	fclose(signaturesFile);
	}
	free(sigFileName);
}
void Print_signatures(link* head){
	list_print(head, stdout);
}
void Detect_viruses(link* head){
	printf("Enter name of suspected file, please: \n");
		char input[1024];
		fgets(input, 1024, stdin);
		char * suspectedFileName = (char *)calloc(1024,1);
		if(strlen(input) > 1){
			memcpy(suspectedFileName, input,strlen(input)-1);
			FILE* suspectedFile = fopen(suspectedFileName, "r");
			char * buffer = (char *) calloc(10, 1024);
			fread(buffer, 1024,10, suspectedFile);
			fseek(suspectedFile, 0, SEEK_END); 
            int size  = ftell(suspectedFile);
            rewind(suspectedFile);
			fread(buffer, 1024,10, suspectedFile);
			virus_detect(buffer, size, head, stdout);
			fclose(suspectedFile);
			free(buffer);
		}
		free(suspectedFileName);
}

void virus_detect(char* buffer, unsigned int size, link* virus_list, FILE* output){
	link* vir_list;
	for (int indexOfStartByte = 0; indexOfStartByte < size; indexOfStartByte++)
	{
			 vir_list= virus_list;
			while (vir_list)
			{
				if (memcmp(buffer + indexOfStartByte,vir_list->vir->sig, vir_list->vir->SigSize  ) == 0)
				{
					fprintf(stdout, "%d\n", indexOfStartByte);
					fprintf(stdout,"%d\n", vir_list->vir->SigSize);
					fprintf(stdout,"%s\n", vir_list->vir->virusName);
					indexOfStartByte = indexOfStartByte + vir_list->vir->SigSize;
				}
				vir_list = vir_list->nextVirus;
			}
			
	}
	vir_list = NULL;
}

void Fix_file(){
	char input[1024];
	printf("Enter infected file's name: \n");
	fgets(input, 1024, stdin);
	char * infectedFileName = (char *)calloc(1024,1);
	if (strlen(input) > 1)
	{
		memcpy(infectedFileName, input,strlen(input)-1);
		char location_input[5];
		printf("Enter location of virus: \n");
		fgets(location_input, 5, stdin);
		char * location_str = (char*) calloc(4,1);
		memcpy(location_str, location_input, strlen(location_input)-1);
		int location = atoi(location_str);
		free(location_str);

		char sigSize_input[5];
		printf("Enter size of the virus's signature: \n");
		fgets(sigSize_input, 5, stdin);
		char * sigSize_str = (char*) calloc(4,1);
		memcpy(sigSize_str, sigSize_input, strlen(sigSize_input)-1);
		int sigSize = atoi(sigSize_str);
		free(sigSize_str);

		kill_virus(infectedFileName, location, sigSize);
	}
	free(infectedFileName);
	

}
void kill_virus(char* fileName, int signatureOffset, int signatureSize){
	FILE* fileToClean = fopen(fileName, "r+");
	fseek(fileToClean, signatureOffset, SEEK_SET);
	char nullChar[signatureSize];
	for (unsigned i = 0; i < signatureSize; i++)
	{
		nullChar[i] = 0x90;
	}
	
	fwrite(nullChar,1,signatureSize, fileToClean);

	fclose(fileToClean);
}

void map(link* virus, void (*f)(link*)){
	f(virus);
}

int main(int argc, char **argv) {
	
	int limit = 4;
	char * input = (char *) calloc(3,1);
	struct fun_desc menu[] = { { "Load signatures", &Load_signatures},{"Print signatures",&Print_signatures},{"Detect viruses",&Detect_viruses}, {"Fix file", &Fix_file},{ NULL, NULL } };
	link * head = (link*) calloc(1,sizeof(*head));
	while(1){
		printf("Please choose a function:\n");
		for(unsigned i = 0 ; ; i++){
			if (menu[i].name != NULL){
				printf("%d.%s\n", i+1,menu[i].name);
			}
			else{
				break;
			}
		}
		printf("Option: ");
		int func_num = 0;
		fgets(input, 3,stdin);
		
		func_num= (int)atoi(input);
		if (func_num<=limit && func_num>0){
			printf("\nWithin bounds\n");
			map(head, menu[func_num-1].fun);
			printf("DONE.\n\n");			
		}
		else{
			printf("\nNot Within bounds\n");
			free(input);
			list_free(head);
			exit(0);
		}
	}
	return (0);
}