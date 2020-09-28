#include <stdio.h>

int main(int argc, char const *argv[]) {
	FILE *fptr;
	fptr = fopen("output.txt", "w");
	fprintf(fptr, "%s\n", "Harsh Kumar");
	fclose(fptr);
	return 0;
}
