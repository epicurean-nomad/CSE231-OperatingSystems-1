global long_add

section .text

long_add:
	push ebp
	mov ebp, esp
	
	mov eax, [esp+8];
	add eax, [esp+12];
	add eax, [esp+16];
	add eax, [esp+20];

	pop ebp
	ret;
