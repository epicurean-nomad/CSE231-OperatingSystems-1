default: run1
	

compile1:
	nasm -o boot.bin q1_2019043.asm

run1: compile1
	qemu-system-x86_64 boot.bin

clean:
	rm boot.bin