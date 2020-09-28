global long_add

section .text

long_add:
	push rbp
	mov rbp, rsp
	
	xor rax, rax;
	
	mov eax, edi;
	add eax, esi;
	add eax, edx;
	add eax, ecx;

	pop rbp
	ret;
