#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include <dirent.h>


int spawn(char** argv);

//Macros
#define DEFAULT_COMMAND_SIZE (size_t) 20
#define DEFAULT_ARGUMENT_COUNT 10

//Global Variables
char *shell_name;
char *shell_version;

char **log_buffer;

static int mem_cleanup(char *shell_name, char *shell_version, size_t *command_size, char **lineptr, char **argv){
	
	free((void *) shell_name);
	free((void *) shell_version);

	free((void *) *lineptr);
	free((void *) command_size);
	
	free((void *) argv);

	free((void *) log_buffer);
}

static int shell_exit(void){
	
	fprintf(stdout, "Good Luck!\n");
	return 0;
}

static int init_shell(char name[], char version[], char **command_line, size_t **command_size, char ***lineptr, char ***argv, int argument_count_coeff){
	shell_name    = (char *) malloc(strlen(name) * sizeof(char));
	shell_version = (char *) malloc(strlen(version) * sizeof(char));

	strcpy(shell_name, name);
	strcpy(shell_version, version);
	
	(*command_line) = (char *) malloc(DEFAULT_COMMAND_SIZE);
	(*lineptr) = command_line;
	
	(*command_size) = (size_t *) malloc(sizeof(int));
	(**command_size) = DEFAULT_COMMAND_SIZE;

	(*argv) = (char **) malloc((argument_count_coeff) * DEFAULT_ARGUMENT_COUNT * sizeof(char *));

	return 0;
}

int main(int argc, char* argv[]){
	int retval;
	int arg_counter;
	int argument_count_coeff = 1;


	size_t *command_size;

	char *delimiter = " ";
	char *command_line;
	char **lineptr;
	
	char user_name[100] = "maroufi";
	char machine_name[100] = "baktash";
	
	init_shell("JShell", "0.1", &command_line, &command_size , &lineptr, &argv, argument_count_coeff);
//	fprintf(stdout, "Welcome to \"%s-v%s\"\n", shell_name, shell_version);
	
	do{	
		arg_counter = 0;
		
		fprintf(stdout, "%s@%s$  ", user_name, machine_name);
		retval = getline(lineptr, command_size, stdin);
		(*lineptr)[retval-1] = '\0';
		
		for(argv[arg_counter] = strtok(*lineptr, delimiter) ; argv[arg_counter] != NULL ; arg_counter++, argv[arg_counter] = strtok(NULL, delimiter)){
			if(arg_counter == DEFAULT_ARGUMENT_COUNT - 1)
				argv = realloc(argv, (++argument_count_coeff) * DEFAULT_ARGUMENT_COUNT * sizeof(char *));
		}
		
//		printf("ARG: %s\n", argv[0]);
		if (strcmp(argv[0], "echo") == 0) {
			FILE * pFile;
			pFile = fopen (argv[3],"w");
			if (pFile!=NULL) {
			    fputs (argv[1],pFile);
			    fclose (pFile);
			}
		} else {
			spawn(argv);	
		}

	}while(strcmp(*lineptr, "exit") != 0);
		
	mem_cleanup(shell_name, shell_version, command_size, lineptr, argv);

	return 0;
}


int spawn(char** argv)
{
    pid_t child_pid;

    child_pid = fork();

    if(child_pid != 0) {
    	wait(NULL);
    	if (strcmp(argv[0], "cat") == 0) {
    		printf("\n");
		}
        return child_pid;
    }
    else {
    	if (execvp(argv[0], argv) == -1) {
        	printf("No such file or directory!\n");
		}
    }
}
