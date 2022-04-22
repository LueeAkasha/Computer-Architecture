#include <stdio.h>

int main (int argc, char **argv){
	int iarray[3];
	float farray[3];
	double darray[3];
	char carray[3];
	int* iptrarray[3];
	char* cptrarray[3];





	printf("iarray cells:\n");
	printf("iarray[0]: %5p\n", iarray);
	printf("iarray[1]: %5p\n",iarray+1);
	printf("iarray[2]: %p\n\n",iarray+2);
	
	printf("farray cells:\n");
	printf("farray[0]: %5p\n", farray);
        printf("farray[1]: %5p\n",farray+1);
        printf("farray[2]: %p\n\n",farray+2);
	
	printf("darray cells:\n");
	printf("darray[0]: %5p\n", darray);
        printf("darray[1]: %5p\n",darray+1);
        printf("darray[2]: %p\n\n",darray+2);
	
	printf("carray cells:\n");
	printf("carray[0]: %5p\n", carray);
        printf("carray[1]: %5p\n",carray+1);
        printf("carray[2]: %p\n\n",carray+2);
	


	printf("iptrarray cells:\n");
        printf("iptrarray[0]: %5p\n", iptrarray);
        printf("iptrarray[1]: %5p\n",iptrarray+1);
        printf("iptrarray[2]: %p\n\n",iptrarray+2);

	printf("cptrarray cells:\n");
        printf("cptrarray[0]: %5p\n", cptrarray);
        printf("cptrarray[1]: %5p\n",cptrarray+1);
        printf("cptrarray[2]: %p\n\n",cptrarray+2);

}
