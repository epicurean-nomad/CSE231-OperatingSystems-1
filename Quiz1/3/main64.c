#include <stdio.h>

int main() {
	char x[64];
	long *ptrLong = (long *)x;

	for(int i=0; i<8; ++i)
		*(ptrLong+i) = 64;

	for(int i=0; i<8; ++i) 
		printf("%ld ", *(ptrLong+i));
		
	printf("\n");

}
