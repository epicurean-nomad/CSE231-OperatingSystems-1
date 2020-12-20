# Bootloader

Name: Harsh Kumar
Roll No: 2019043

## Instructions to boot

***Note: `QEMU` and `nasm` should be installed on the system to run this bootloader program***

- To compile the bootloader program, type `make compile1` from your terminal.
- To compile and run, type `make run1` from your terminal.
- The default behaviour, without any target defined is to run and compile the program and is equivalent to `make run1`.

## Explnation

- First of all, we disable interrupts.
- We then load our GDT for the bootloader.
- We then switch to protected mode (32 bit). This is done by changing the control reigister `CR0`. We change the LSB to 1.
- We now calculate the position where BIOS left the cursor on the screen. We callculate this by reading values from special memory address, used to maintain row and column position on the screen as well as the width of the screen by the BIOS.
- We then print a `Hello World` message on the screen. This is done by printing character by character on the screen, using a special instruction, `stosw`, which transfers character to a given memory location, and then updates the positions for the next transfer. We then load the next byte to be printed, using the instruction `lodsb`.
- Finally, we print value of `CR0` register, bit by bit. We copy the contents to another general purpose register. We left shift by one, set the carry bit with offset of 48 (which is ASCII for zero) to be printed on the screen, and then print the character on the screen.
