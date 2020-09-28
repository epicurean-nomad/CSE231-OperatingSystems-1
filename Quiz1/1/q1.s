global _start
global checkGreater

section .data
	align 2
	one: db "1", 10
	zero: db "0", 10

section .text

_start:
	call checkGreater;

	mov rax, 60;
	mov rdi, 0;
	syscall;

checkGreater:
	push rbp
	mov rbp, rsp

	cmp rdi, rsi

	jg greater;
	mov rsi, zero;
	jmp after;

greater:
	mov rsi, one;
	
after:
	mov rax, 1;
	mov rdi, 1;
	mov rdx, 2;
	syscall;

	pop rbp

	mov rax, 60;
	mov rdi, 0;
	syscall;

	ret