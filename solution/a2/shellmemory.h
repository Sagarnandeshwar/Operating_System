void mem_init();
char *mem_get_value(char *var);
void mem_set_value(char *var, char *value);
int load_script(char* script, int length);
//char *get_line(int index);
char *get_line1(int index);
void clean_up(int index);
int get_free_space();
