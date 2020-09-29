default:
	$(MAKE) --no-print-directory clean
	mkdir build
	gcc -E main.c -o build/main.i
	gcc -S build/main.i -o build/main.s
	gcc -c build/main.s -o build/main.o
	gcc -o build/main build/main.o

make clean:
	rm -r build