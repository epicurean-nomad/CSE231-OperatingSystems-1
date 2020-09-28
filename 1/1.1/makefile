default:
	gcc -E main.c -o main.i
	gcc -S main.i
	gcc -c main.s
	gcc -o main main.o

make clean:
	rm main main.o main.s main.i