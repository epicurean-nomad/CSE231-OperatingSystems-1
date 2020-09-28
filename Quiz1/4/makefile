default:
	make --no-print-directory elf32
	make --no-print-directory elf64

elf32:
	nasm -f elf32 -o long_add.o long_add.s
	gcc -m32 main.c long_add.o -o main32
	
elf64:
	nasm -f elf64 -o extended_add.o extended_add.s
	gcc main.c extended_add.o -o main64
	
clean:
	rm *.o main32 main64
