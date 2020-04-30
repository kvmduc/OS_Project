#include "shell.h"

int main(void){
    char input_string[MAX_LINE] = {};/* command line string */
    char input_string_temp[MAX_LINE] = {};
    char * args_normal[MAX_LIST] = {};
    char * args_pipe[MAX_LIST] = {};
    int shouldrun = 1; /* flag to determine when to exit program */
    int system_Num;
    do{
        int background = 0;
        printf("osh");
        fflush(stdout);
        /**
        * After reading user input, the steps are:* 
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) parent will invoke wait() unless command included &*/
        if(take_input(input_string) == 0){
           continue;
        }

        if (strstr(input_string, "&") != NULL) // Search for Ampersand
        {
            strncpy(input_string_temp, input_string, strlen(input_string) - 1);
            strcpy(input_string, input_string_temp);
            background = 1;
        }
       
        system_Num = input_classification(input_string,args_normal,args_pipe);

        if(system_Num == -1)
        {
            break;
        }

        if(system_Num == 0){
            shouldrun = osh_normal_execute(args_normal, background);
        }

        if(system_Num == 1){
            /*Pipe() - Task 4*/
        }
        else if (system_Num == 2)
        {
            continue;
        }
        
    } while(shouldrun);
    return 0;
}