#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <fcntl.h>

int path_size;
char PATH[50][100];


void getCommand();
void help();
void version();
void getPath();
void execCommand(char *cmd);
int checkPipe(char *cmd);
int checkRedirection(char *cmd);
void execPipe(char *cmd);
int checkCustom(char *cmd);
void checkCommand(char *cmd);


/*---------------------------MAIN-------------------------------------*/
int main(int argc, char *argv[]){

    //Clears the screen
    system("clear");

    //Prints the help message
    help();

    //Gets the path
    getPath();

    //Gets and executes the command
    getCommand();

    return 0;
}




/*--------------------------------HELP-------------------------------*/
void help(){

    printf( "\nWelcome!\n\n"
            "This is an application written in C.\n"
            "It's goal is to replicate a terminal.\n\n"
            "By typing `help` you will see this menu.\n\n"
            "The UNIX commands `cat`, `head` and `env` were implemented "
            "as custom commands\n"
            "within the program under the name: "
            "`catt`, `headd` and `envv`.\n\n"
            "The following options for each command are also "
            "implemented:\n\n"
            "* catt:\n"
            "\t-b : number nonempty output lines\n\n"
            "\t-E : displays `$` at the end of each line\n\n"
            "\t-n : number all output lines\n\n"
            "\t-s : suppress repeated empty output lines\n\n\n"
            "* headd:\n"
            "\t-c : print the first NUM bytes of each file;\n"
            "\t     with the leading '-', print all but the last NUM bytes\n\n"
            "\t-n : print the first NUM of lines instead of the first 10\n\n"
            "\t-q : never print headers giving file names\n\n"
            "\t-v : always print headers giving file names\n\n"
            "* envv:\n"
            "\t-u : remove variable from the environment\n\n");
}


/*--------------------------------Version-------------------------------*/
void version(){

    printf( "\n---Version---\n\n"
            "This application was created by Andrei Suba.\n"
            "Student in second year, Computer Science.\n"
            "West University of Timisoara.\n"
            "Email: andrei.suba00@e-uvt.ro.\n\n");
}


/*---------------------Prompt_and_Input---------------------------*/
void getCommand(){

    //Will store the input.
    char *cmd = NULL;   

    //Tab completion
    rl_bind_key('\t', rl_complete);

    //Starts the loop
    while(true){

        cmd = readline("$: ");
        if(!cmd){

            //User pressed CTRL + D (EOF)
            break;
        }

        if(strlen(cmd) == 0){
            continue;
        }

        //Adds the command to history
        add_history(cmd);


        //Checks the command
        checkCommand(cmd);
    }

    //Free the memeory
    free(cmd);
}



/*-----------------------Get_Path-------------------------------*/
void getPath(){

    char *path = NULL;
    size_t size_path;

    FILE *fp = popen("echo $PATH", "r");
    if(fp == NULL){
        printf("Failed to run command.\n");
        return;
    }
    if(getline(&path, &size_path, fp) < 0){

        printf("Failed to read output.\n");
        return;
    }
    pclose(fp);

    int size = strlen(path);

    char new_path[size];
    strcpy(new_path, path);

    char *token;

    token = strtok(path, ":");
    int num_tokens = 1;


    while(token != NULL){
        num_tokens++;
        token = strtok(NULL, ":");
    }

    char paths[num_tokens][100];
    char *new_token;
    
    new_token = strtok(new_path, ":");

    int i = 0;

    while(new_token != NULL){
        strcpy(paths[i], new_token);
        new_token = strtok(NULL, ":");       
        i++;
    }


    // Number of unique strings
    int num_unique_tokens = 0;  

    // Iterate through the original array and add unique strings to the new array
    for(int i = 0; i < num_tokens; i++){

        // Assume the string is unique
        int is_unique = 1;  

        // Check if the string is already in the unique array
        for(int j = 0; j < num_unique_tokens; j++){

            if(strcmp(paths[i], PATH[j]) == 0){

                // The string is not unique
                is_unique = 0;
                break;
            }
        }

        // If the string is unique, add it to the unique array
        if(is_unique){

            strcpy(PATH[num_unique_tokens], paths[i]);
            num_unique_tokens++;
        }
    }

    path_size = num_unique_tokens;
    free(path);
}


/*---------------------Execute_Command---------------------------*/

void execCommand(char *cmd){

    char *file;
    char *rest; 
    char *arguments[10];

    int i = 0;    

    file = strtok(cmd, " ");

    rest = strtok(NULL, "");

    arguments[i++] = file;

    char *arg = strtok(rest, " ");

    while(arg != NULL){

        arguments[i++] = arg;
        arg = strtok(NULL, " ");
    }

    arguments[i] = NULL;


    //To prevent infinite loop
    int success = 0;

    //Initializes the filename
    char *filename = NULL;

    for(int i = 0; i < path_size; i++){
       
        //Is +2 because you need to add also '/'
        filename = malloc((strlen(PATH[i]) * sizeof(char) + strlen(file) * sizeof(char) + 2));

        strcpy(filename, PATH[i]);
        strcat(filename, "/");
        strcat(filename, file);

        if(access(filename, F_OK) == 0){
            
            success = 1;
            break;
        }

        if(i == (path_size - 1) && access(filename, F_OK) != 0){

            printf("\nError: Command not found.\n");
        }
    }


    //Checks if the command is not found and exists while loop
    if(success == 0){
        return;
    } 

    //Starts the process
    pid_t process = fork();

    if(process == -1){

        printf("Process could not be created.");
        exit(1);
    }
    else if(process == 0){
        //char *argv[] = {filename, arguments, NULL};
        //execve(filename, argv, NULL);
        execve(filename, arguments, NULL);
    }
    else{
        //Wait for the process to finish
        waitpid(process, NULL, 0);
    }

    
    free(filename);
}


int checkPipe(char *cmd){

    if(strstr(cmd, "|"))
        return 1;
    else
        return 0;
}


int checkRedirection(char *cmd){

    if(strstr(cmd, ">"))
        return 1;
    else if(strstr(cmd, "<"))
        return 1;
    else
        return 0;
}

void execPipe(char *cmd){

    char *first_cmd;
    char *second_cmd;

    first_cmd = strtok(cmd, "|");
    second_cmd = strtok(NULL, "") + 1;

    //Start the processing of the first command
    char *first_token = strtok(first_cmd, " ");

    char *cmd1 = first_token;

    char *cmd1_args[10];

    int argc_1 = 0;

    while(first_token != NULL){

        cmd1_args[argc_1] = first_token;
        argc_1++;
        first_token = strtok(NULL, " ");
    }
    
    cmd1_args[argc_1] = NULL;


    //Start the processing of the second command
    char *second_token = strtok(second_cmd, " ");

    char *cmd2 = second_token;

    char *cmd2_args[10];

    int argc_2 = 0;

    while(second_token != NULL){

        cmd2_args[argc_2] = second_token;
        argc_2++;
        second_token = strtok(NULL, " ");
    }
    
    cmd2_args[argc_2] = NULL;


    int fd[2], id, id2;

    if(pipe(fd) < 0)
    {
        perror ("Error creating pipe\n");
        exit(2);
    }
    if ((id = fork()) < 0)
    {
        perror ("Error creating first child process\n");
        exit(4);
    }

    if(id == 0)
    {
        if (dup2(fd[1], STDOUT_FILENO) < 0)
        {
            perror("Error trying to duplicate output");
            exit(3);
        }
        close(fd[1]);
        close(fd[0]);
        execvp(cmd1, cmd1_args);
    }

    if ((id2 = fork()) < 0)
    {
        perror ("Error creating second child process\n");
        exit(6);
    }

    if (id2 == 0)
    {
        if (dup2(fd[0], STDIN_FILENO) < 0)
        {
            perror("Error trying to duplicate input\n");
            exit(5);
        }
        close(fd[1]);
        close(fd[0]);
        execvp(cmd2, cmd2_args);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(id, NULL,  0);
    waitpid(id2, NULL, 0);
}



void checkCommand(char *cmd){

    if(checkPipe(cmd)){
        execPipe(cmd);
        return;
    }        

    if(checkRedirection(cmd)){
        printf("\nRedirection found\n");
        return;
    }        

/*    if(checkCustom(cmd)){
        printf("Custom command found.\n");
    }
    else{
        //Execute the command
        execCommand(cmd);
    }
*/
    if(!checkCustom(cmd)){
        execCommand(cmd);
    }
}


int checkCustom(char *cmd){

    // Checks for exit
    if(strstr(cmd, "exit")){

        exit(0);
    }

    // Checks for help
    if(strstr(cmd, "help")){

        help();
        return 1;
    }

    // Checks for version
    if(strstr(cmd, "version")){

        version();
        return 1;
    }

    // Checks for catt
    if(strstr(cmd, "catt")){

        help();
        return 1;
    }

    // Checks for headd
    if(strstr(cmd, "headd")){

        help();
        return 1;
    }

    // Checks for envv
    if(strstr(cmd, "envv")){

        help();
        return 1;
    }
}
