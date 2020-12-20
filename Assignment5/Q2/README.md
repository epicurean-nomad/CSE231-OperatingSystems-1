# Advisory File Locks

Name: Harsh Kumar
Roll No: 2019043

## Steps to run

***Make sure `xterm` and `gcc` are installed on your system before running this program.***

- To compile the program, type `make` from your terminal. This will create `editor` in your folder.
- To run the program, you can use `./editor <filename>`. This will open the editor. It will display the contents of the file, and ask if you want to add some text to the file.
- To run the test script, to show the working of the advisory lock, you can run the shell script, by typing `./shell.sh` in your terminal.
- To automatically compile and run the test, type `make run` in your terminal.

## Explanation and Working

1. First of all we open the file, whose path is supplied as a command line argument.
2. We then take a lock using `flock` system call. We pass `LOCK_EX` and `LOCK_NB` flags. This ensures that lock is non blocking, and exclusive. If locking fails, we check if some other program has acquired lock. If yes, then we display a warning and continue. Else, the program exits.
3. We now read the contents of the file and store it in a string named `data`.
4. Now, we display the contents of the string `data`.
5. Finally, we take a string input from the user, which will be appended to the end of the file.
6. We release the lock, and then close the file. We also free all the allocated dynamic memory allocated.
