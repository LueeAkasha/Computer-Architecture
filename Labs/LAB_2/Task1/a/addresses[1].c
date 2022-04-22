#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5; //bss
int addr6; //bss 

int foo();//text segment
void point_at(void *p);//text segment
void foo1();//text segment
void foo2();//text segment

int main (int argc, char** argv){
    int addr2;//stack
    int addr3;//stack
    char* yos="ree"; //yos in on stack and "ree" on text segment
    int * addr4 = (int*)(malloc(50)); // heap
    printf("- &addr2: %p\n",&addr2);
    printf("- &addr3: %p\n",&addr3);
    printf("- foo: %p\n",foo);
    printf("- &addr5: %p\n",&addr5);
    
	point_at(&addr5);
	
    printf("- &addr6: %p\n",&addr6);
    printf("- yos: %p\n",yos);
    printf("- addr4: %p\n",addr4);
    printf("- &addr4: %p\n",&addr4);
    
    printf("- &foo1: %p\n" ,&foo1);
    printf("- &foo1: %p\n" ,&foo2);
    printf("- &foo2 - &foo1: %ld\n" ,&foo2 - &foo1);
    return 0;
}

int foo(){
    return -1;
}

void point_at(void *p){
    int local; //stack
	static int addr0 = 2; //initialized segment
    static int addr1; //bss


    long dist1 = (size_t)&addr6 - (size_t)p; //stack
    long dist2 = (size_t)&local - (size_t)p;//stack
    long dist3 = (size_t)&foo - (size_t)p;//stack
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p\n", & addr0);
    printf("- addr1: %p\n",&addr1);
}

void foo1 (){    
    printf("foo1\n"); 
}

void foo2 (){    
    printf("foo2\n");    
}
