#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int getcmd(char *prompt, char *args[], int *background)
{
int length, i = 0;

char *token, *loc;

char *line = NULL;

size_t linecap = 0;


printf("%s", prompt);


length = getline(&line, &linecap, stdin);
if (length <= 0) {
exit(-1);
}

// Check if background is specified..
if ((loc = index(line, '&')) != NULL) {
*background = 1;
*loc = ' ';
} else
*background = 0;
while ((token = strsep(&line, " \t\n")) != NULL) {
for (int j = 0; j < strlen(token); j++)
if (token[j] <= 32)
token[j] = '\0';
if (strlen(token) > 0)
args[i++] = token;
}
return i;
}

//Signal Handler For Control C Function
static void sigHandlerforC(int sig)
{
  exit(0);
}

//Signal Handler For Control Z Function
static void sigHandlerforZ(int sig)
{
  ;
}

int main(void)
{  

char *args[20];
int bg;


//Signal Handler For Control C Function
if (signal(SIGINT,sigHandlerforC)== SIG_ERR){
printf("ERROR Could not bind the signal Error \n");
exit(1);
}

//Signal Handler For Control Z Function
if (signal(SIGTSTP,sigHandlerforZ)== SIG_ERR){
printf("ERROR Could not bind the signal Error \n");
exit(1);
}

int index = 1;
int row = 64;
pid_t table[row];

while(1) {
bg = 0;
int cnt = getcmd("\n>> ", args, &bg);

args[cnt + 1] = NULL; // Terminating Array with NULL


if ( strcmp(args[0],"cd") == 0 ) // cd Command 
{
  if (cnt == 2){
  if ( chdir(args[1]) == -1)
  {
    printf("ERROR with cd command");
  }
  }
  else{
    printf("Inccorect argument got cd");
  }
}
else if (strcmp(args[0],"pwd") == 0) // pwd Command 
{
  if (cnt == 1)
  {
  char cwd[256];
  getcwd(cwd, sizeof(cwd));
  printf("%s",cwd);
  }
  else{
    printf("Inccorect argument for pwd");
  }

}
else if (strcmp(args[0],"exit") == 0) // Exit Command 
{
  if (cnt == 1)
  {
  exit(0);
  }
  else
  {
    printf("Inccorect argument for else");
  }
}
else if (strcmp(args[0],"jobs") == 0)
{
  if (cnt == 1)
  {
  for (int i = 1; i < index+1; ++i)
  {
        printf("%ld \n", (long)table[i]);
  }
  }
  else
  {
    printf("Inccorect argument for jobs");
  }
}
else if (strcmp(args[0],"fg") == 0)
{
  if (cnt == 1)
  {
    pid_t fgpid = table[1];
    int status;
    waitpid(fgpid, &status, WUNTRACED);
  }
  else if (cnt == 2)
  {
    int status;
    int indexnumber = atoi(args[0]);
    pid_t fgpid = table[indexnumber];
    waitpid(fgpid, &status, WUNTRACED);
  }
  else
  {printf("Inccorect argument for fg");}
}
else
{
  pid_t child_pid = fork();
  if (child_pid == 0)
  { 
    if (strcmp(args[cnt-2],">") == 0)
    {
      char *argr[20];
      int i = 0;
      for (i = 0; i < cnt - 2; ++i){
        argr[i] = args[i];
      }
      close(1);
      int fd = open (args[cnt-1],O_CREAT, S_IXUSR);
      execvp(argr[0],argr);
    }
    else
    {
    execvp(args[0],args);
    }
    printf("ERROR IN Child process \n");
  }
  else if (child_pid < 0)
  { 
    printf("Fork error \n");
  }
  else
  {
    if (bg == 0)
    {
      int status;
      waitpid(child_pid, &status, WUNTRACED);
    }
    else
    {
      int status;
      pid_t return_pid = waitpid(child_pid, &status, WNOHANG);
      if (return_pid == -1) {printf("Error in child process \n");}
      else if (return_pid == 0) { 
      table[index] =  child_pid;
      index = index + 1;
      }
      else { }
    }
  }
}



/* the steps can be..:
(1) fork a child process using fork()
(2) the child process will invoke execvp()
(3) if background is not specified, the parent will wait,
otherwise parent starts the next command... */

}
}