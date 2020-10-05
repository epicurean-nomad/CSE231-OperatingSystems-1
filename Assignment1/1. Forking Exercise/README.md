# Forking Exercise
We fork the program, by using fork() system call. This creates a new copy of the process. fork() returns 0 in the child process, and returns  positive  value in the parent process. If the fork has failed, it returns -1. We are handling all these three cases in our main function.

Since the instruction counter is incremented after fork(), both the processes continue from if-else statement in main.

## In Child Process:
We open the .csv file using open system call. Once the file is successfully opened, we start reading the students. In case of error, we print ‘error while opening file’ and terminate the process.

We use a function called read_line_from_file, which reads a line from the file using system calls, and stores it in a buffer. We parse this data to ‘struct student’ for further use using the function ‘parseStudent’.

After successful reading of the file, we check if this student is from section A, or section B. If the student is from section A, we print details of the student using printStudent function.

## In Parent Process:
First of all, we have to wait for the child process to complete, so that we don’t interfere, while the child process is writing to STDOUT. After this, we follow the same procedure as the child process did. But, instead of printing details of the A section students, we print details for B section students.

It is important to note that we are using only system calls to write to STDOUT. We’ve made functions for printing strings, integers, float values to STDOUT using:

```diff
-syscall(1, STDOUT_FILENO, str, size);
```