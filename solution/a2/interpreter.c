#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "shell.h"

int MAX_ARGS_SIZE = 7;

int help();
int quit();
int badcommand();
int badcommandTooManyTokens();
int badcommandFileDoesNotExist();
int badcommand_samefile();
int no_space_error();
int set(char* var, char* value);
int print(char* var);
//int run(char* script);
int my_ls();
int echo();


// PCB
struct PCB{
	int pid;
	int start;
	int length;
	int counter;
	int score;
	struct PCB *next;
};

void clear_memory(struct PCB* process);

struct PCB* create_pcb(int start, int length, int pid){
    struct PCB* process = (struct PCB*)malloc(sizeof(struct PCB));
	process->pid = pid;
    process->start = start;
	process->counter = start;
	process->length = length;
	process->score = length;
	process->next = NULL;
    return process;
}

//int pid_id[1000];

//Ready Queue
struct PCB *head = NULL;
//struct PCB *curr = NULL;

void create_list(struct PCB* process){
	/*if (process == NULL){
		printf("%s\n","create_list():  NULL input");
		return 0;
	}*/
    head = process;
}

struct PCB* get_tail(){
	struct PCB* ptr = head;
	/*if (ptr == NULL){
		printf("%s\n","get_tail():  empty list");
		return 0;
	}*/
	while (ptr->next != NULL){
		ptr = ptr->next;
	}
	return ptr;
}


void add_to_list(struct PCB* process){
	/*if (process == NULL){
		printf("%s\n","add_to_list():  NULL input");
		//return 0;
	}*/
	if(head == NULL){
        (create_list(process));
		return;
    }
	else{
		struct PCB *ptr = get_tail();
		ptr->next = process;
	}
}

void remove_head(){
	struct PCB* ptr = head->next;
	head->next = NULL;
	head = ptr;
}

void remove_ptr(struct PCB* process){
	if (process->pid = head->pid){
		head = head->next;
	}
	else{
		struct PCB* prev = head;
		struct PCB* ptr = head->next;
		while(ptr!=NULL){
			if (ptr->pid == process->pid){
				prev->next = process->next;
				break;
			}
			else
			{
			prev = prev->next;
			ptr = ptr->next;
			}

		}	
	}
}

// Clean Up
void clear_memory(struct PCB* process){
	int start = process->start;
	int length = process->length;
	int i;
	int end;
	end = start + length;
	for (i = start; i < end; i++){
		clean_up(i);
	}
}



//Execution
void execute_process(struct PCB* process){
	int start = process->start;
	int length = process->length;
	int i;
	int end;
	end = start + length;
	for (i = start; i < end; i++){
		//printf("%d\n",i);
		parseInput(get_line1(i));
		clean_up(i);
	}
}

int execute_process_rr(struct PCB* process){
	int counter = process->counter;
	int length = process->length;
	int start = process->start;
	int end = start + length;
	int i = 0;
	while (counter < end){
		parseInput(get_line1(counter));
		clean_up(counter);
		++i;
		++counter;
		if (i == 2){
			break;
		}
	}
	process->counter = counter;
	/*
	printf("%d\n",counter);
	printf("%d\n",end);
	printf("%d\n",start);
	*/
	if ( counter >= end){
		return 1;
	}
	return 0;
}

int execute_process_aging(struct PCB* process){
	int counter = process->counter;
	int start = process->start;
	int length = process->length;
	int end = start + length;
	int score = process->score;
	int i;
	if (score == 0){
		for (i=counter;i<end;++i){
			parseInput(get_line1(counter));
			clean_up(counter);
			++counter;
			process->counter = counter;
		}
		return 1;
	}

	parseInput(get_line1(counter));
	clean_up(counter);
	++counter;
	process->counter = counter;
	if (counter >= end){
		return 1;
	}
	return 0;
}

void swap_content(struct PCB* temp1, struct PCB* temp2){
	int pid;
	int start;
	int counter;
	int length;
	int score;
	//struct PCB *tempnext;

	pid = temp1->pid;
	start = temp1->start;
	counter = temp1->counter;
	length = temp1->length;
	score = temp1->score;
	//tempnext = temp1->next;
				
	temp1->pid = temp2->pid;
	temp1->start = temp2->start;
	temp1->counter = temp2->counter;
	temp1->length = temp2->length;
	temp1->score = temp2->score;
	//temp1->next = temp2->next;
				
	temp2->pid = pid;
	temp2->start = start;
	temp2->counter = counter;
	temp2->length = length;
	temp2->score = score;
	//temp2->next = tempnext;
}

void sort_ready_length(){
	struct PCB* main_node;
	struct PCB* loop_variable;
	main_node = head;
	while(main_node != NULL){
		loop_variable = main_node;
		while(loop_variable->next != NULL){
			if (loop_variable->length > loop_variable->next->length){
				swap_content(loop_variable,loop_variable->next);
			}
			loop_variable = loop_variable->next;
		}
		main_node = main_node->next;
	}
}

void sort_ready_score(){
	struct PCB* main_node;
	struct PCB* loop_variable;
	main_node = head;
	while(main_node != NULL){
		loop_variable = main_node;
		while(loop_variable->next != NULL){
			if (loop_variable->score > loop_variable->next->score){
				swap_content(loop_variable,loop_variable->next);
			}
			loop_variable = loop_variable->next;
			
		}
		
		main_node = main_node->next;
	}
}


//AGING
void ageing(){
	struct PCB* ptr = head;
	ptr = ptr->next;
	while(ptr != NULL){
		ptr->score = ptr->score - 1;
		ptr = ptr->next;
	}
}

int my_schedular(char* policy){
	if (strcmp(policy, "FCFS")==0){
		struct PCB* ptr = head;
		while(1){
			execute_process(ptr);
			if (ptr->next == NULL){
				break;
				}
			ptr = ptr->next;
		}
		head = NULL;
		/*
		printf("%d\n",get_free_space());
		if (head == NULL){
			printf("%s\n","TRUE");
		}
		*/
		return 0;
	}
	else if (strcmp(policy, "SJF")==0){
		sort_ready_length();
		struct PCB* ptr = head;
		while(1){
			execute_process(ptr);
			if (ptr->next == NULL){
				break;
				}
			ptr = ptr->next;
		}
		head = NULL;
		
		return 0;
	}
	else if (strcmp(policy, "RR")==0){
		struct PCB* ptr = head;
		//struct PCB* temp;
		int result = 1;
		int remove = 0;
		while(ptr!=NULL){
			remove = execute_process_rr(ptr);
			if (remove == 0){
				remove_head();
				add_to_list(ptr);
			}
			else{
				remove_head();
			}
			ptr = head;
		}
		
		return 0;
	}
	else if (strcmp(policy, "AGING")==0){
		sort_ready_score();
		//display();
		struct PCB* ptr = head;
		int result = 1;
		int remove = 0;
		while (head != NULL){
			remove = execute_process_aging(head);
			if (remove == 1){
				remove_head();
			}

			if (head == NULL){
				break;
			}
			ageing();
			sort_ready_score();
			//ptr = head;

		}
		
		return 0;
	}
	else {
		return badcommand();
	}
}

//File length
int get_file_length(char *script){
	int i;
	int length = 0;
	char line[300];

	printf("%s\n",script);

	FILE *p = fopen(script,"rt");

	if(p == NULL){
		return -1;
	}

	fgets(line,999,p);

	for(i = 0; i<900; i++){
		if(feof(p)){
			break;
		}
		++length;
		fgets(line,999,p);
	}

	fclose(p);

	return length;
}

//main codes
int interpreter(char* command_args[], int args_size){
	int i;

	if ( args_size < 1 || args_size > MAX_ARGS_SIZE){
		if (strcmp(command_args[0], "set")==0 && args_size > MAX_ARGS_SIZE) {
			return badcommandTooManyTokens();
		}
		return badcommand();
	}

	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
		//printf("hi%shi\n",command_args[i]);
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size < 3) return badcommand();
		char* value = (char*)calloc(1,150);
		char spaceChar = ' ';

		for(int i = 2; i < args_size; i++){
			strncat(value, command_args[i], 30);
			if(i < args_size-1){
				strncat(value, &spaceChar, 1);
			}
		}
		return set(command_args[1], value);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		//return run(command_args[1]);
		//return run(command_args[1]);
		int space = get_free_space();

		int length = get_file_length(command_args[1]);

		if (length == -1){
			return badcommandFileDoesNotExist();
		}

		if (space<length){
			return no_space_error();
		}

		int start = load_script(command_args[1],length);
		struct PCB *process = create_pcb(start,length,1);
		add_to_list(process);
		my_schedular("FCFS");
		//schedular();
		return 0;
	
	} else if (strcmp(command_args[0], "exec")==0) {
		if (args_size > 5) return badcommand();
		if (args_size < 3) return badcommand();

		if (strcmp(command_args[1], command_args[2])==0){
			return badcommand_samefile();
		}
		if (strcmp(command_args[1], command_args[3])==0){
			return badcommand_samefile();
		}
		if (strcmp(command_args[2], command_args[3])==0){
			return badcommand_samefile();
		}

		int space = get_free_space();
		int require_space = 0;

		if (args_size == 3){
			int length_pr1 = get_file_length(command_args[1]);

			if (length_pr1 == -1){
				return badcommandFileDoesNotExist();
			}

			require_space = length_pr1;
			if (space<require_space){
				return no_space_error();
			}
			int start_point = load_script(command_args[1],length_pr1);
			struct PCB *process = create_pcb(start_point,length_pr1,1);
			add_to_list(process);
		}
		else if (args_size == 4){
			int length_pr1 = get_file_length(command_args[1]);
			int length_pr2 = get_file_length(command_args[2]);
			require_space = length_pr1 + length_pr2;

			if (length_pr1 == -1){
			return badcommandFileDoesNotExist();
			}

			if (length_pr2 == -1){
			return badcommandFileDoesNotExist();
			}

			if (space<require_space){
				return no_space_error();
			}

			int start_pr1 = load_script(command_args[1],length_pr1);
			struct PCB *process1 = create_pcb(start_pr1,length_pr1,1);

			int start_pr2 = load_script(command_args[2],length_pr2);
			struct PCB *process2 = create_pcb(start_pr2,length_pr2,2);

			add_to_list(process1);
			add_to_list(process2);

			/*
			printf("%d\n",length_pr1);
			printf("%d\n",start_pr1);
			
			printf("%d\n",length_pr2);
			printf("%d\n",start_pr2);
			*/

		}
		else if (args_size == 5){
			int length_pr1 = get_file_length(command_args[1]);
			int length_pr2 = get_file_length(command_args[2]);
			int length_pr3 = get_file_length(command_args[3]);

			require_space = length_pr1 + length_pr2;
			require_space = require_space + length_pr3;

			if (length_pr1 == -1){
			return badcommandFileDoesNotExist();
			}

			if (length_pr2 == -1){
			return badcommandFileDoesNotExist();
			}

			if (length_pr3 == -1){
			return badcommandFileDoesNotExist();
			}


			if (space<require_space){
				return no_space_error();
			}

			int start_pr1 = load_script(command_args[1],length_pr1);
			struct PCB *process1 = create_pcb(start_pr1,length_pr1,1);

			int start_pr2 = load_script(command_args[2],length_pr2);
			struct PCB *process2 = create_pcb(start_pr2,length_pr2,2);

			int start_pr3 = load_script(command_args[3],length_pr3);
			struct PCB *process3 = create_pcb(start_pr3,length_pr3,3);


			add_to_list(process1);
			add_to_list(process2);
			add_to_list(process3);
		}

		return my_schedular(command_args[args_size-1]);

	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size > 2) return badcommand();
		return my_ls();
	
	} else if (strcmp(command_args[0], "echo")==0) {
		if (args_size > 2) return badcommand();
		return echo(command_args[1]);
	
	} else {
		return badcommand();
	}
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	//printf("hi%shi\n","hellow:wq");
	printf("%s\n", "Unknown Command");
	return 1;
}

int badcommandTooManyTokens(){
	printf("%s\n", "Bad command: Too many tokens");
	return 2;
}

int badcommandFileDoesNotExist(){
	printf("%s\n", "Bad command: File not found");
	return 3;
}

int badcommand_samefile(){
	printf("%s\n", "Bad command: Same File name");
	return 4;
}

int no_space_error(){
	printf("%s\n", "No space in Shell Memory");
	return 5;
}

int set(char* var, char* value){

	char *link = "=";
	char buffer[1000];
	strcpy(buffer, var);
	strcat(buffer, link);
	strcat(buffer, value);

	mem_set_value(var, value);

	return 0;

}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

/*
int run(char* script){
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");  // the program is in a file

	if(p == NULL){
		return badcommandFileDoesNotExist();
	}

	fgets(line,999,p);
	while(1){
		errCode = parseInput(line);	// which calls interpreter()
		memset(line, 0, sizeof(line));

		if(feof(p)){
			break;
		}
		fgets(line,999,p);
	}

    fclose(p);

	return errCode;
}
*/

int my_ls(){
	int errCode = system("ls | sort");
	return errCode;
}

int echo(char* var){
	if(var[0] == '$'){
		var++;
		printf("%s\n", mem_get_value(var)); 
	}else{
		printf("%s\n", var); 
	}
	return 0; 
}