# COMP 310: Operating Systems
Built simulation of an operating system that performs OS Shell tasks, Multi-process Scheduling, and Memory management.

## Operating Systems
An operating system (OS) is a software program that acts as an intermediary between computer hardware and user applications. It manages the computer's resources, provides a user interface, and allows applications to run efficiently. Essentially, the operating system is the foundation on which all other software and user interactions rely. 

The primary functions of an operating system include Hardware Management, Process Management, Memory Management, File System Management, User Interface, Device Management, and Security. The following are some popular example of OS: Windows, macOS, Linux, iOS and Android. 

## Compiling starter shell 
- Use the following command to compile: make mysh 
- Re-compiling the shell after making modifications: make clean; make mysh

Please Note: The starter code compiles and runs on McGill mimi server. If you’d like to run the code in your own Linux, virtual machine, you may need to install build essentials to be able to compile C code: sudo apt-get install build-essential 

## Running starter shell
There are two mode:
- **Interactive mode:** From the command line prompt type: ./mysh 
- **Batch mode:** You can also use input files to run your shell. To use an input file, from the command line prompt type: ./mysh < testfile.txt

## Shell interface

The starter shell supports the following commands: 

- **help:** Displays all the commands 
- **quit:** Exits / terminates the shell with “Bye!” 
- **set VAR STRING:** Assigns a value to shell memory 
- **print VAR:** Displays the STRING assigned to VAR 
- **run SCRIPT:** Executes the commands in the file SCRIPT 
- **echo:** Displays strings which are passed as arguments on the command line 
- **my_ls:** Lists all the files present in the current directory 

### One-liners 
Chaining of instructions so that the shell can take as input multiple commands separated by semicolons (the ; symbol). 
