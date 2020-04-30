#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_LINE 400 /* The maximum length command */
#define MAX_LIST 50 /* The maximum length arg */
#define CMD_LIST 4 /* Number of undefined arg */

 



int take_input(char * input_string){
    char * buffer_string = readline(">>");
    if(strlen( buffer_string )==0){
        return 0;
    }
    else{
        //add_history(buffer_string); //Neu ranh thi co the viet mot ham Overload !!!
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
        return 0;
    }
    return 1;
}

void parse_space(char * str_with_space, char ** parsed){
    parsed[0] = strtok(str_with_space, " ");
    if(parsed[0] == NULL){
        return;
    }
    for(int i = 1; i < MAX_LIST ; i++){
        parsed[i] = strtok(NULL, " ");  
        if(parsed[i] == NULL){
            break;
        }
        if(strlen(parsed[i]) == 0){
            i--; //Nhieu space lien tiep
        }
    }

}

int defined_execute(char ** args_normal, int background){
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
        if(background == 1){
            //do {
                //wpid = waitpid(pid, &status, WUNTRACED);
                wpid = waitpid(pid, &status, WNOHANG); //from GITHUB
                //wpid = wait(&status); TEST
            //} while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        else if(background == 0){
            wpid = waitpid(pid, &status, 0);
        }
  }
  return 1;
}

int undefined_execute(char ** args_normal, int Num_of_CMD){
    switch (Num_of_CMD)
    {
    case 0:
        printf("This is Kieu Vu Minh Duc & friends 's Terminal !!! \n"
        "Wanna play safe ? \n"
        "Go get help with \"help\" command \n"
        );
        break;

    case 1:
        chdir(args_normal[1]);
        break;

    case 2:
        printf("List of Commands supported:"
        "\n>hello"
        "\n>cd"
        "\n>exit"
        "\n>all other general commands available in UNIX shell"
        "\n>ONE pipe handling"
        "\n>improper space handling\n");
        break;

    case 3:
        exit(0);
        break;
    default:
        break;
    }
    return 1;
}

int osh_normal_execute(char ** args_normal, int background){
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
    return defined_execute(args_normal, background);
}



int find_redirect(char * input_string, char ** redirect_partition){
    char * pch;
    /*
        return -1: error string
        return 0: khong tim thay redirect
        return 1: stdout_stderr "2>&1"
        return 2: append ">>"
        return 3: stdin "<"
        return 4: stderror "2>"
        return 5: stdout ">"
    */

    if (strstr(input_string, "2>&1") != NULL) {
        printf("Not compatible with this operator \n");
        return 1;
    }
    else if (strstr(input_string, ">>") != NULL) {
        redirect_partition[0] = strtok (input_string,">");

        redirect_partition[1] = strtok (NULL, ">");
        return 2;
    }
    else if (strstr(input_string, "<") != NULL) {
        redirect_partition[0] = strtok (input_string,"<");
        
        redirect_partition[1] = strtok (NULL, "<");
        return 3;
    }
    else if (strstr(input_string, "2>") != NULL) {
        redirect_partition[0] = strtok (input_string,"2");

        redirect_partition[1] = strtok (NULL, ">");
        return 4;
    }
    else if (strstr(input_string, ">") != NULL) {
        redirect_partition[0] = strtok (input_string,">");
        
        redirect_partition[1] = strtok (NULL, ">");
        return 5;
    }
    return 0;
}

int input_classification(char * input_string, char ** args_normal, char ** args_pipe){
    int pipe = 0, redirect_char = 0, fout, ferr;
    int kidpid, fd;
    char * str_partition[2]; //Yeu cau 1 pipe line nen chia string input ra lam 2 phan
    char * redirect_partition[2]; //Yeu cau 1 redirect nen chia string lam 2 phan
    char * path[1];
    pipe = find_pipe(input_string, str_partition);
    redirect_char = find_redirect(str_partition[0], redirect_partition);

    if(pipe == -1 || redirect_char == -1){
        printf("Error string input ! \n");
        return -1;
    }
    else if(pipe == 0 && redirect_char == 0){
        parse_space(str_partition[0], args_normal);
        return 0;
    }
    else if(pipe == 1){
        parse_space(str_partition[0], args_normal);
        parse_space(str_partition[1], args_pipe);
        return 1;
    }
    else if(redirect_char != 0){
        parse_space(redirect_partition[0],args_normal);
        parse_space(redirect_partition[1],path);
        switch (redirect_char)
        {
        case 1:
            
            break;
        case 2:
            fd = open(path[0], O_WRONLY|O_APPEND); //redirect_partition[1]
            if (fd < 0) { perror("open"); abort(); }
            switch (kidpid = fork()) {
            case -1: perror("fork"); abort();
            case 0:
                if (dup2(fd, 1) < 0) { perror("dup2"); abort(); }
                close(fd);
                execvp(args_normal[0], args_normal); perror("execvp"); abort();
            default:
                close(fd);
                /* do whatever the parent wants to do. */
            }
            break;
        case 3:
            fd = open(path[0], O_RDONLY); //redirect_partition[1]
            if (fd < 0) { perror("open"); abort(); }
            switch (kidpid = fork()) {
            case -1: perror("fork"); abort();
            case 0:
                if (dup2(fd, 0) < 0) { perror("dup2"); abort(); }
                close(fd);
                execvp(args_normal[0], args_normal); perror("execvp"); abort();
            default:
                close(fd);
                /* do whatever the parent wants to do. */
            }
            break;
        case 4:
            /* redirect stderr */
            switch (kidpid = fork()) {
            case -1: perror("fork"); abort();
            case 0:
                if ((ferr = open(path[0], O_WRONLY| O_APPEND| O_CREAT | O_TRUNC, 0600))== -1){
                perror("open");
                exit(3);
                }
                dup2(ferr, fileno(stderr));
                execvp(args_normal[0],args_normal);
            default:
                close(ferr);
                /* do whatever the parent wants to do. */
            }
            break;
            
        case 5:
            fd = open(path[0], O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); //0644 //redirect_partition[1]
            if (fd < 0) { perror("open"); abort(); }
            switch (kidpid = fork()) {
            case -1: perror("fork"); abort();
            case 0:
                if (dup2(fd, 1) < 0) { perror("dup2"); abort(); }
                close(fd);
                execvp(args_normal[0], args_normal); perror("execvp"); abort();
            default:
                close(fd);
                /* do whatever the parent wants to do. */
            }
            break;
        default:
            break;
        }
        return 2;
    }
}
int osh_piped_execute(char ** args_normal, char** args_pipe){
    int fd[2];
    pid_t p1, p2;
    if(pipe(fd) < 0){
        perror("can't pipe");
    }
    p1=fork();
    if (p1<0){
        printf("\n Could not fork");
        return;
    }
    if(p1==0){
        close(fd[0]);
        dup2(fd[1],1);
        close(fd[1]);
        if(execvp(args_normal[0],args_normal)<0){
            printf("\n Could not execute command 1");
            exit(0);
        }
    }else {
        p2=fork();
        if(p2<0){
            printf("\n Could not fork");
            return;
        }    
        if(p2==0){
            close(fd[1]);
            dup2(fd[0],0);
            close(fd[0]);
            if(execvp(args_pipe[0],args_pipe)<0){
                printf("\n Could not execute command 2");
                exit(0);
            }
        }else{
            close(fd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
    return 1;
}