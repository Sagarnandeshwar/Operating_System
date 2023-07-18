#include<stdlib.h>
#include<string.h>
#include<stdio.h>


struct memory_struct{
	char *var;
	char *value;
};

// Shell memory functions
struct memory_struct shellmemory[1000];

int *free_space;
int *starting;

void mem_init(){

	int i;
	char* index;
	for (i=0; i<900; i++){		
		//char* index = (char*)i;
		//sprintf(index, "%d", i);
		//shellmemory[i].var = index;
		shellmemory[i].var = "empty";
		shellmemory[i].value = "none";
		//printf("%s",shellmemory[i].var);
	}

	/*
	for (i=0; i<900; i++){

		printf("%s",shellmemory[i].var);
	}
	*/

	for (i=900; i<1000; i++){		
		shellmemory[i].var = "none";
		shellmemory[i].value = "none";
	}
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {

	int i;
	/*
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}*/

	//reserve last 100 spot for vairable saving  
	for (i=900; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}

	for (i=900; i<1000; i++){
		if (strcmp(shellmemory[i].var, "none") == 0){
			shellmemory[i].var = strdup(var_in);
			shellmemory[i].value = strdup(value_in);
			return;
		} 
	}
	return;

}

//get value based on input key
char *mem_get_value(char *var_in) {
	int i;
	/*
	for (i=0; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			return strdup(shellmemory[i].value);
		} 
	}
	return "Variable does not exist";
	*/
	for (i=900; i<1000; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			return strdup(shellmemory[i].value);
		} 
	}
	return "Variable does not exist";

}

//RAM
/*
int get_free_space(){
	int i;
	int count = 0;
	for(i = 0; i<900; i++){
		if (strcmp(shellmemory[i].var, "empty") == 0){
			++count;
		}
	}
	return count;
}

int space(int lines){
	int spaceleft = free_space();
	if (spaceleft == 0){
		return 0;
	}

	if (lines > spaceleft){
		return 0;
	}

	return lines;
}
*/


int load_script(char* script, int length){
	int i;
	int start = -1;
	int end = 0;

	for(i = 0; i<900; ++i){
		if (strcmp(shellmemory[i].var, "empty") == 0){
			start = i;
			break;
		}
	}

	end = length + start;
	int errCode = 0;
	char line[1000];
	FILE *p = fopen(script,"rt");

	fgets(line,999,p);

	for(i = start; i<end; i++){
		shellmemory[i].var = "occupied";
		shellmemory[i].value = strdup(line);
		if(feof(p)){
			break;
		}
		fgets(line,999,p);
	}

	fclose(p);
	
	return start;
}

char* get_line1(int index) {
	/*int i;
	char c = (char)index;
	char* var_in = (char*) c;
	for (i=0; i<900; i++){
		if (strcmp(shellmemory[i].var, var_in) == 0){
			return strdup(shellmemory[i].value);
		} 
	}
	*/
	return strdup(shellmemory[index].value);

}

int get_free_space(){
	int i;
	int space = 0;
	for(i = 0; i<900; ++i){
		if (strcmp(shellmemory[i].var, "empty") == 0){
			++space;
		}
	}
	return space;
}

//clean up
void clean_up(int index){
	shellmemory[index].var = "empty";
	shellmemory[index].value = "none";
	return;
}
