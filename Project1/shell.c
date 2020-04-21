#include "shell.h"

int main(void){
    char input_string[MAX_LINE] = {};/* command line string */
    char * args_normal[MAX_LIST] = {};
    char * args_pipe[MAX_LIST] = {};
    int shouldrun = 1; /* flag to determine when to exit program */
    while (shouldrun) {
        printf("osh>");
        fflush(stdout);
        /**
        * After reading user input, the steps are:* 
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) parent will invoke wait() unless command included &*/
       if(take_input(input_string) == 0){
           continue;
       }

       shouldrun = osh_execute();
       }
        return 0;
}