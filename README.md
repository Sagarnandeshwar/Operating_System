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

- **help** Displays all the commands 
- **quit** Exits / terminates the shell with “Bye!” 
- **set VAR STRING** Assigns a value to shell memory 
- **print VAR** Displays the STRING assigned to VAR 
- **run SCRIPT** Executes the commands in the file SCRIPT 
- **echo** Displays strings which are passed as arguments on the command line 
- **my_ls** Lists all the files present in the current directory
- **exec prog1 prog2 prog3 POLICY** Executes up to 3 concurrent programs, according to a given scheduling policy 

### One-liners 
Chaining of instructions so that the shell can take as input multiple commands separated by semicolons (the ; symbol)

Assumptions:
- The instructions separated by semicolons are executed one after the other. 
- The total length of the combined instructions does not exceed MAX_USER_INPUT. 
- There will be at most 10 chained instructions 
- Semicolon is the only accepted separator

## Concurrent processing
Concurrent processing, also known as concurrent computing or parallel processing, refers to a computing paradigm where multiple tasks or processes are executed simultaneously or overlapping in time. The primary goal of concurrent processing is to increase overall system efficiency, reduce processing time, and improve resource utilization by handling multiple tasks concurrently. 

### Scheduling Infrastructure 
The run command to use the scheduler and run SCRIPT as a process; it sets up the scaffolding for the exec command. The program execute the run command followingly,
1. **Code loading:** Instead of loading and executing each line of the SCRIPT one by one, the program will load the entire source code of the SCRIPT file into the OS Shell memory. 
2. **PCB:** Create a data-structure to hold the SCRIPT PCB. PCB is a struct, which has  
The process PID.  
The spot in the Shell memory where the program loaded the SCRIPT instructions 
The current instruction to execute 
3. **Ready Queue:** Create a data structure for the ready queue. The ready queue contains the PCBs of all the processes currently executing. 
4. **Scheduler logic:**
  - The PCB for SCRIPT is added at the tail of the ready queue 
  - The scheduler runs the process at the head of the ready queue, by sending the process’ current instruction to the interpreter. 
  - The scheduler switches processes in and out of the ready queue, according to the scheduling policy 
  - When a process is done executing, it is cleaned up and the next process in the ready queue starts executing. 
5. **Clean-up:** Finally, after the SCRIPT terminates, the program remove the SCRIPT source code from the Shell memory.


Assumptions:
- The shell memory is large enough to hold three scripts and still have a bit of extra space. The size of the Shell memory is 1000 lines, thus each script will have at most 300 lines of source code.  
- You can also assume that each command (i.e., line) in the scripts will not be larger than 100 characters.

### exec Command
**exec prog1 prog2 prog3 POLICY** Executes up to 3 concurrent programs, according to a given scheduling policy

- exec takes up to four arguments.
- POLICY is always the last parameter of exec. 
- POLICY can take the following three values: FCFS, SJF, RR, or AGING.
- If other arguments are given, the shell outputs an error message, and exec terminates, returning the command prompt to the user.

Behavior:
**Exec behavior for single-process** The behavior of exec prog1 POLICY is the same as the behavior of run prog1, regardless of the policy value.  
**Exec behavior for multi-process** Exec runs multiple processes concurrently as follows: 
- The entire source code of each process is loaded into the Shell memory. 
- PCBs are created for each process. 
- PCBs are added to the ready queue, according to the scheduling policy.  
- When processes finish executing, they are removed from the ready queue and their code is cleaned up from the shell memory.

Assumptions 
- For simplicity, we are simulating a single core CPU. 
- We do not support threading. 
- We will not be testing recursive exec calls. 
- Each exec argument is the name of a different script filename. If two exec arguments are identical, the shell displays an error and exec terminates, returning the command prompt to the user or keeps running the remaining instructions, if in batch mode. 
- If there is a code loading error (e.g., running out of space in the shell memory), then no programs run. The shell displays an error, the command prompt is returned, and the user will have to input the exec command again.

### Scheduling Policies
The OS supports the following Scheduling Policies:
- **FCFS (First come First Serve):** The interpreter executes the tasks in the order they arrive or enter the system. 
- **SJF (Shortest Job First):** The interpreter gives highest priority to the task with the shortest execution time and is scheduled for execution first, 
- **RR (Round-Robin):** The interpreter allocates a fixed time slice to each task in the system, and when a task's time slice expires, it is preempted, and the next task in the queue is given the CPU for its time slice. 
- **SJF with aging:** Aging is a technique used to gradually increase the priority of waiting tasks over time, ensuring that long-waiting tasks eventually get a chance to execute. 

Mechanism:  
- **For SJF** use the number of lines of code in each program to estimate the job length. 
- **For RR** schedulers typically use a timer to determine when the turn of a process ended. We use a fixed number of instructions as a time slice. Each process gets to run 2 instructions before getting switched out. 
- **For SJF with aging**
  - Instead of sorting jobs by estimated job length, we will sort them by a “job length score”. The job length score is tracked in the PCB.
  - In the beginning of the exec command, the “job length score” of each job is equal to their job length (i.e., the number of lines of code in the script)
  - The scheduler will re-assess the ready queue every time slice. For this exercise, we will use a time slice of 1 instruction.
    - After a given time-slice, the scheduler “ages” all the jobs that are in the ready queue, apart from the current head of the queue.
    - The aging process decreases a job’s “job length score” by 1. The job length score cannot be lower than 0.
    - If after the aging procedure there is a job in the queue with a score that is lower than the current running job, the following happens:
      - The current running job is preempted
      - the job with the new lowest job length score is placed at the head of the running queue. In case of a tie, the process closer to the head of the running queue has priority.
      - The scheduler runs the new process in the head of the ready queue.
    - If after the aging procedure the current head of the ready queue is still the job with the lowest “job length score”, then the current job continues to run for the next time slice. 
  
