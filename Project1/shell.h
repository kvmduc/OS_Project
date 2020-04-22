#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_LINE 800 /* The maximum length command */
#define MAX_LIST 100 /* The maximum length arg */
#define CMD_LIST 4 /* Number of undefined arg */

 


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

int defined_execute(char ** args_normal){
    pid_t pid, wpid;
    int status;
    pid = fork();

    if (pid == 0) {
        // child process
        if (execvp(args_normal[0], args_normal) == -1) {
            perror("osh");
        }
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) {
        perror("osh");
    } 
    else {
        //fork() thanh cong
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
            //wpid = wait(&status);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int undefined_execute(char ** args_normal, int Num_of_CMD){
    switch (Num_of_CMD)
    {
    case 0:
        printf("This is MacMeEmDi 's Terminal !!! \n"
        "Wanna play safe ? \n"
        "Go get help with \"help\" command \n"
        );
        break;

    case 1:
        chdir(args_normal[1]);
        break;

    case 2:
        printf("\nList of Commands supported:"
        "\n>hello"
        "\n>cd"
        "\n>exit"
        "\n>all other general commands available in UNIX shell"
        "\n>ONE pipe handling"
        "\n>improper space handling");
        break;

    case 3:
        exit(0);
        break;
    default:
        break;
    }
    return 1;
}

int osh_normal_execute(char ** args_normal){
    char * list_of_cmd[CMD_LIST]; //Mot so command duoc build truc tiep tren shell chu khong duoc build thanh mot chuong trinh, nen phai tu dinh nghia

    list_of_cmd[0] = "hello";
    list_of_cmd[1] = "cd";
    list_of_cmd[2] = "help";
    list_of_cmd[3] = "exit";

    if(args_normal[0]==NULL){
        return 1; //Command empty;
    }
    for(int i = 0; i < CMD_LIST; i++){
        if(strcmp(args_normal[0], list_of_cmd[i]) == 0){
            return undefined_execute(args_normal, i);
        }
    }
    return defined_execute(args_normal);

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
        return 0;
    }

    else if(pipe == 1){
        parse_space(str_partition[0], args_normal);
        parse_space(str_partition[1], args_pipe);
        return 1;
    }
 
    else{
        printf("More than one pipeline !!!");
        return pipe;
    }
}
