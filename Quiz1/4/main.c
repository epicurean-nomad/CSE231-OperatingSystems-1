#include <stdio.h>

int long_add(int a, int b, int c, int d);

int main() {
	int a, b, c, d;
	scanf("%d %d %d %d", &a, &b, &c, &d);
	printf("%d\n", long_add(a, b, c, d));
}