#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void PrintHex(unsigned char* buffer, int length, FILE* output){
    for (int i = 0; i < length; i++) {
        fprintf(output, "%02X ", *(buffer + i));
    }
        fprintf(output, "\n\n");
}

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

virus* readVirus(FILE* input){
    virus* vir = (virus*) malloc(sizeof(virus));
    char* buffer = (char*) malloc(sizeof(char)*18);

    if(fread(buffer, 18, 1, input) <= 0){
        free(vir);
        free(buffer);
        return NULL;
    }

    char temp[2];
    memcpy(temp,buffer,2);
    vir->SigSize = temp[0] + temp[1]*16*16;
    memcpy(vir->virusName, buffer+2, 16);
    free(buffer);

    vir->sig = (unsigned char*) malloc(sizeof(unsigned char*) * (vir->SigSize));
    if(fread(vir->sig, vir->SigSize, 1, input) <= 0){
        printf("No sig found\n");
        free(vir);
        return NULL;
    }
    
    return vir;
}

void printVirus(virus* virus, FILE* output){
    fprintf(output, "Virus name: %s\nVirus size: %d\nsignature:\n", virus->virusName, virus->SigSize);
    PrintHex(virus->sig, virus->SigSize, output);
}



typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};
void list_print(link *virus_list, FILE* output){
    link* temp = virus_list;
    while(temp != NULL){
        printVirus(temp->vir, output);
        temp = temp->nextVirus;
    }
}
 
link* list_append(link* virus_list, virus* data){
    
    link* temp = (link*) malloc(sizeof(link));
    
    temp->vir = data;
    temp->nextVirus = NULL;
   
    if(virus_list == NULL){
        //fprintf(stderr, "it's a null\n");
        return temp;
    }
    link* temp2 = virus_list;
    while(virus_list->nextVirus != NULL){
        virus_list = virus_list->nextVirus;
    }
    virus_list->nextVirus = temp;
    return temp2;
}
 void free_virus(virus* virus){
	free(virus->sig);
	free(virus);
}

void list_free(link *virus_list){
    if(virus_list == NULL) return;
	list_free(virus_list->nextVirus);
	free_virus(virus_list->vir);
	free(virus_list);
} 
link* load_signatures(link *virus_list, FILE* output){
    virus* temp;
    if(virus_list!=NULL)
        list_free(virus_list);
    virus_list = NULL;
    char inputPath[100];
    scanf("%s", inputPath);
    FILE* input;
    if((input = fopen("signatures-L", "r")) == NULL){
        fprintf(stderr, "\nCan't find file name: %s\n", inputPath);
        exit(0);
    }
    char temp2[4];
    fread(temp2, 4, 1, input);
    while((temp = readVirus(input)) != NULL){
        //fprintf(stderr, "Adding a virus\n");
        virus_list = list_append(virus_list, temp);
    }
    fclose(input);
    return virus_list;
}

link* detect_virus(char *buffer, unsigned int size, link *virus_list, FILE* output){
    link* temp;
    for (int i = 0; i < size; i++){
        temp = virus_list;
        while(temp != NULL){
            if(size - i > temp->vir->SigSize){
                if(memcmp(buffer+i, temp->vir->sig, temp->vir->SigSize) == 0){
                    fprintf(output, "Starting Byte: %d\nVirus Name: %s\nSignature Size: %d\n", i, temp->vir->virusName, temp->vir->SigSize);
                }
            }
            temp = temp->nextVirus;
        }
    }
    return virus_list;
}






int main(int argc, char **argv) {
    int choosen = 0;
    link* virus_list = NULL;
    char* fileBuffer;
    FILE* fileToDetect;

    while(1){
        printf("1) Load signatures\n2) Print signatures\n3) Detect virus\nOption: ");
        scanf("%d", &choosen);
        switch (choosen){
        case 1:
            virus_list = load_signatures(virus_list, stdout);
            break;
        case 2:
            list_print(virus_list, stdout);
            break;
        case 3:
            fileToDetect = fopen("signatures-L", "r");
            fseek(fileToDetect, 0, SEEK_END);
            size_t fileSize = ftell(fileToDetect);
            fseek(fileToDetect, 0, SEEK_SET);
            fread(fileBuffer, fileSize, 1, fileToDetect);
            fclose(fileToDetect); 
            detect_virus(fileBuffer, fileSize, virus_list, stdout);
            break;
        default:
            exit(0);
            break;
        }
    }
    return 0;
}




