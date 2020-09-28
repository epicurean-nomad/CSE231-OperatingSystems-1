#include <stdio.h>

int main() {
	char x[64];
	int *ptr = (int *)x;

	for(int i=0; i<16; ++i)
		*(ptr+i) = 32;

	for(int i=0; i<16; ++i) 
		printf("%d ", *(ptr+i));

	printf("\n");

}
