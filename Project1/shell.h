#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_LINE 800 /* The maximum length command */
#define MAX_LIST 100 /* The maximum length arg */

int take_input(char * input_string){
    char * buffer_string = readline(">>");
    if(strlen( buffer_string )==0){
        return 0;
    }
    else{
        add_history(buffer_string); //Neu ranh thi co the viet mot ham Overload !!!
        strcpy(input_string , buffer_string);
        return 1;
    }
}

int find_pipe(char * input_string, char ** str_partition){
    str_partition[0] = strtok(input_string, "|");
    if(str_partition[0] == NULL){
        return -1;
    }
    str_partition[1] = strtok(NULL, "|");
    if(str_partition[1] == NULL){
        return 0; //Khong tim thay pipeline
    }
    return 1; // Tim thay 1 pipeline
}

void parse_space(char * str_with_space, char ** parsed){
    parsed[0] = strtok(str_with_space, " ");
    if(parsed[0] == NULL){
        return;
    }
    for(int i = 1; i < MAX_LIST ; i++){
        parsed[i] = strtok(NULL, " ");  
        if(parsed[i] == NULL){
            return;
        }
        if(strlen(parsed[i]) == 0){
            i--; //Nhieu space lien tiep
        }
    }
}

void CmdHandler(char** parsed){
    execvp(parsed[0], parsed);
}

int input_classification(char * input_string, char ** args_normal, char ** args_pipe){
    int pipe = 0;
    char * str_partition[2]; //Yeu cau 1 pipe line nen chia string input ra lam 2 phan
    pipe = find_pipe(input_string, str_partition);

    if(pipe == -1){
        printf("Error string input ! \n");
        return -1;
    }
    
    else if(pipe == 0){
        parse_space(str_partition[0], args_normal);
        CmdHandler(args_normal);
    }

    else if(pipe == 1){
        parse_space(str_partition[0], args_normal);
        parse_space(str_partition[1], args_pipe);
        CmdHandler(args_normal);
        CmdHandler(args_pipe);
    }

    else{
        printf("More than one pipeline !!!");
        return pipe;
    }

}


int osh_execute(){
    
    return 0;
}