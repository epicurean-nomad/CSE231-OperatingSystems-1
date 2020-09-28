default:
	nasm -f elf64 -o q1.o q1.s
	gcc -c q1c.c -o q1c.o
	ld -e main -o q1 q1c.o q1.o
	
clean:
	rm *.o q1
