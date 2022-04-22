#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
	unsigned short SigSize;
	char virusName[16];
	unsigned char* sig;
} virus;

virus* readVirus(FILE* fp){
	int cursor = ftell(fp);
	if (cursor == 0){
		fseek(fp, 4, SEEK_SET);
	}
	else{
		fseek(fp, 0 , SEEK_END);
		int end = ftell(fp);
	fseek(fp,cursor,SEEK_SET);
	
	if(cursor== end) return NULL;
	}
	
	
	virus* v = (virus*)calloc(1,sizeof(*v)); // to return
	
	fread(v, 1, 18, fp);
	
	int tempPointer = (int)&(v->SigSize);
	char*p = (char*)tempPointer;
	char temp = *p;
	*(p) = *(p + 1);
	*(p+1) = temp;
	int intd = (((*p & 0xff) << 8)|(*(p+1) & 0xff));
	
	
	
	v->sig = calloc(intd,sizeof(unsigned char));
	
	fread(v->sig,1 , intd,fp);
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
	int tempPointer = (int)&(virus->SigSize);
	char*p = (char*)tempPointer;
	
	int intd = (int)(((*p & 0xff) << 8)|(*(p+1) & 0xff));
	fprintf(output, "%d" , intd);
	fprintf(output, "\n");
	
	 
	
	char* Signature = "signature:";
	fprintf(output, "%s" , Signature);
	fprintf(output, "\n");
	
	for(unsigned i = 1; i <= intd; ++i){ 
		fprintf(output, "%02X ", *(virus->sig+i-1));
		if(i%20 == 0 && i<intd){
			fprintf(output, "\n");
		}
	}
	fprintf(output, "\n\n");
}


typedef struct link link;
struct link{
	link *nextVirus;
	virus *vir;
};

void list_print(link *virus_list, FILE* fp){
	if(virus_list == NULL){
		printf("%s\n", "#Link is undefined!");
		return;
	}
	if(virus_list->vir == NULL){
		printf("%s\n", "#Link is empty!");
		return;
	}
	printVirus(virus_list->vir, fp);
	if(virus_list->nextVirus != NULL){
		list_print(virus_list->nextVirus, fp);
	}
}

link* list_append(link* virus_list, virus* data){ // to last of list.
	if(virus_list == NULL){
		virus_list = (link*) calloc(1,sizeof(*virus_list));
		virus_list->vir= data;
		virus_list->nextVirus = NULL;
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
	return virus_list;
}

void list_free(link* virus_list){
	if(virus_list == NULL) return;
	list_free(virus_list->nextVirus);
	free_virus(virus_list->vir);
	free(virus_list);
}



void Load_signatures(){}
void Print_signatures(){}
void Detect_viruses(){}

int main(int argc, char **argv) {
	
	/*     Task 1a
	FILE* fp = fopen(argv[1],"r");
	FILE* out = fopen("output.txt","w+");

	virus* v;
	do{
		printf("%d\n", "1");
		v = readVirus(fp);
		if(v != NULL){
			printVirus(v, out);
			free_virus(v);
		 }
	}while(v!= NULL);
	fclose(fp);
	fclose(out);
	
	*/
	
	
	/*    Task 1b
	FILE* fp = fopen(argv[1],"r");
	FILE* out = fopen("output1.txt","w+");
	link* linka = NULL;
	while(1){
		
		virus* v = readVirus(fp);
		if(v == NULL) break;
		 linka  = list_append(linka, v);
	}
	list_print(linka, out);
	list_free(linka);
	
	fclose(fp);
	fclose(out);
	*/
	
	
    return 0;
}