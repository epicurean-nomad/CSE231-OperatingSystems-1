default:
	mkdir build
	gcc -E main.c -o build/main.i
	gcc -S build/main.i -o build/main.s
	gcc -c build/main.s -o build/main.o
	gcc -o build/main build/main.o

clean:
	rm -r build

rebuild:
	$(MAKE) --no-print-directory clean
	$(MAKE) --no-print-directory default