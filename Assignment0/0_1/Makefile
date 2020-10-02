default:
	@$(MAKE) --no-print-directory preprocess

	@read char
	@$(MAKE) --no-print-directory compile

	@read char
	@$(MAKE) --no-print-directory assemble

	@read char
	@$(MAKE) --no-print-directory link

	@echo "\nAll tasks completed! Press enter to exit."
	@read char


preprocess:
	@echo "=> Preprocessing hello.c"
	gcc -E hello.c -o hello.i

compile:
	@echo "=> Compiling hello.i"
	gcc -S hello.i

assemble:
	@echo "=> Assembling hello.s"
	gcc -c hello.s

link:
	@echo "=> Linking hello.o"
	gcc hello.o -o hello

clean:
	@echo "=> Clearing all build files"
	rm hello hello.s hello.o hello.i