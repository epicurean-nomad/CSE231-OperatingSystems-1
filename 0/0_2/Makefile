default:
	$(MAKE) --no-print-directory assemble-add
	$(MAKE) --no-print-directory compile-prog
	$(MAKE) --no-print-directory link

assemble-add:
	nasm -felf64 add.asm

compile-prog:
	gcc -c prog-add.c

link:
	gcc add.o prog-add.o

clean:
	rm prog-add.o add.o a.out