For some reason I am getting "Unknown Command" after the end of every test
I tried to debug it and found after the last command the shell sends a blank command to interpretor. I have also changed the address of program in tests.
Rules:
Shell Memory:
0-899: For loading scripts
900-999: For shell variable