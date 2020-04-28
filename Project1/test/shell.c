#include "shell.h"

int main(void){
    char input_string[MAX_LINE] = {};/* command line string */
    char * args_normal[MAX_LIST] = {};
    char * args_pipe[MAX_LIST] = {};
    int shouldrun = 1; /* flag to determine when to exit program */
    int system_Num;
    do{
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
       
        system_Num = input_classification(input_string,args_normal,args_pipe);
        if(system_Num == -1){
            break;
        }
        if(system_Num == 0){
            shouldrun = osh_normal_execute(args_normal);
        }
        if(system_Num == 1){
            //
        }
        if(system_Num == 2){
            continue;
        }
    } while(shouldrun);
    return 0;
}