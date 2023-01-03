#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

int path_size;
char PATH[50][100];


void getCommand();
void help();
void getPath();





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

/*---------------------Prompt_and_execution---------------------------*/
void getCommand(){

    //Will store the input.
    char *cmd = NULL;   

    rl_bind_key('\t', rl_complete);

    while(true){

        cmd = readline("$:");
        if(!cmd){

            //User pressed CTRL + D (EOF)
            break;
        }

        if(strlen(cmd) == 0){
            continue;
        }

        //Adds the command to history
        add_history(cmd);

        // Checks for exit
        if(strstr(cmd, "exit")){
    
            exit(0);
        }

        // Checks for help
        if(strstr(cmd, "help")){

            help();
            continue;
        }

        // Checks for help
        if(strstr(cmd, "catt")){

            help();
            continue;
        }

        // Checks for help
        if(strstr(cmd, "headd")){

            help();
            continue;
        }

        // Checks for help
        if(strstr(cmd, "envv")){

            help();
            continue;
        }

        //To prevent infinite loop
        int success = 0;

        //Initializes the filename
        //char *filename = strcat(PATH[0], "/");
        char *filename = NULL;

        for(int i = 0; i < path_size; i++){
           
            //Is +2 because you need to take also '/'
            filename = malloc((strlen(PATH[i]) * sizeof(char) + strlen(cmd) * sizeof(char) + 2));

            strcpy(filename, PATH[i]);
            strcat(filename, "/");
            strcat(filename, cmd);

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
            continue;
        } 

        //Starts the process
        pid_t process = fork();

        if(process == -1){

            printf("Process could not be created.");
            exit(1);
        }
        else if(process == 0){
            char *argv[] = {filename, NULL};
            execve(filename, argv, NULL);
        }
        else{
            //Wait for the process to finish
            waitpid(process, NULL, 0);
        }
    
        free(filename);
    }

    //Free the memeory
    free(cmd);
}



/*-----------------------Get_Path-------------------------------*/
void getPath(){

    char *path = getenv("PATH");
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


    int num_unique_tokens = 0;  // Number of unique strings

    // Iterate through the original array and add unique strings to the new array
    for(int i = 0; i < num_tokens; i++){
        int is_unique = 1;  // Assume the string is unique

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
}
