# Custom Terminal
## Assumptions:
- Path names cannot be more than 99 characters long
- No blank command should be supplied to the terminal

## To run:
#####  make 
#####  ./build/main

## Internal Commands implemented:

### 1. pwd : 
This command prints the current working directory.
Calling Convention : 	pwd [-L | -P]

##### Flags / Options:
- -L : Displays logical current working directory, ie, symlinks are not resolved
- -P : Displays physical current working directory, ie, symlinks are resolved

##### Corner cases handled:
- Buffer overflow: The directory address is too long, and we run out of allocated space  
- Invalid option: The user supplies some unsupported flag


### 2. cd : 
This command changed the current working directory.
Calling Convention : 	cd [-L | -P] [dir]

##### Flags / Options:
- -L : Force symbolic links to be followed: symbolic links not resolved
- -P : Use the physical directory structure without following symbolic links: symbolic links are resolved to physical address

##### Corner cases handled:
- Invalid Path: The directory address is not valid, we print error on STDERR
- Invalid option: The user supplies some unsupported flag


### 3. history : 
This command prints the history of commands used in the terminal.
Calling Convention : 	history [-c | -d <entry_no>]

##### Flags / Options:
- -c : Erases history
- -d <entry_no> : Delete the entry number <entry_no> from history

##### Corner cases handled:
- No integer provided after -d flag: We print error on STDERR
- <entry_no> : Entry number provided is incorrect
- Invalid option: The user supplies some unsupported flag


### 4. echo : 
This command prints the input on the console.
Calling Convention : 	echo [-n , -e , -E]

##### Flags / Options:
- -n : Do not output the trailing newline
- -e : Enable interpretation of backslash escapes
- -E : Disable interpretation of backslash escapes (default)

##### Corner cases handled:
- Empty string : It handles the case of empty string
- Incorrect escape sequence : Prints it as it is
- Invalid option: The user supplies some unsupported flag


### 5. exit : 
This command prints the input on the console.
Calling Convention : 	exit [n]

##### Flags / Options:
- <n> : Return value of the process
- -- help : Returns help on exit command
	
##### Corner cases handled:
- No corner cases



## External Commands Implemented:

### 1. cat : 
This command prints the contents of a file on the console.
Calling Convention : 	cat [-n , -E] [FILE]...

##### Flags / Options:
- -n or --number : Shows line number, before starting of each line
- -E or --show-ends : Display $ at end of each line

##### Corner cases handled:
- Missing operand : The user doesn’t provide any files
- Error while opening file : Handled using errno, relevant error message is printed on STDERR
- Invalid option: The user supplies some unsupported flag


### 2. date : 
This command prints the date on the console
Calling Convention : 	date [-u, -R] +[format]

##### Flags / Options:
- -u : Display UTC time
- -R : Output date and time in RFC 5322 format.  
    Example: Mon,  14  Aug 2006 02:34:56 -0600

##### Corner cases handled:
- Formatting enabled
- Invalid option: The user supplies some unsupported flag


### 3. ls : 
This command prints the contents of a directory on the console.
Calling Convention : 	ls [-a, -A] [dir]...

##### Flags / Options:
- -a or --all : Shows all entries, including hidden entries and ‘.’ and ‘..’ 
- -A or --almost-all : Display $ at end of each line

##### Corner cases handled:
- Missing operand : The user doesn’t provide any files, we open current directory
- Error while opening directory : Handled using errno, relevant error message is printed on STDERR
- Multiple directories are handled
- Invalid option: The user supplies some unsupported flag


### 4. mkdir : 
This command creates an empty directory with the specified name.
Calling Convention : 	mkdir [-m <privilege_level> , -v] DIR...

##### Flags / Options:
- -m <priviege_no> : Set the privilege level
- -v : Verbose mode, print a message for each created directory

##### Corner cases handled:
- Missing operand : The user doesn’t provide any directory names
- Error while creating directories : Handled using errno, relevant error message is printed on STDERR
- Invalid option: The user supplies some unsupported flag


### 5. rm : 
This command prints the contents of a file on the console.
Calling Convention : 	rm [-d , -r] [FILE]...

##### Flags / Options:
- -d or --dir : Deletes empty directories
- -r or --R : Deletes directories, and its contents recursively

##### Corner cases handled:
- Errors related to different options
- Error while deleting directory: Handles using errno, relevant error message is printed on STDERR, could be due to insufficient permissions, etc
- Invalid option: The user supplies some unsupported flag
