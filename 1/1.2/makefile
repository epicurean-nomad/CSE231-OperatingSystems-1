default:
	$(MAKE) --no-print-directory clean
	mkdir build
	gcc ls.c -o build/ls
	gcc date.c -o build/date
	gcc mkdir.c -o build/mkdir
	gcc cat.c -o build/cat
	gcc rm.c -o build/rm

	gcc main.c -o build/main


clean: 
	rm -r build
