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
extern char **environ;


void help();
void version();
void envv();
void headd(int argc, char *argv[]);
void new_headd(int argc, char *argv[]);
void catt(int argc, char *argv[]);

void getPath();
void getCommand();

int checkPipe(char *original);
int checkRedirection(char *original);

void execCommand(char *original);
void execPipe(char *original);

int checkCustom(char *original);
void checkCommand(char *original);


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


/*---------------------Check_Command---------------------------*/
void checkCommand(char *original){

    //Make a copy of the string
    char *cmd = strdup(original);
    
    if(checkPipe(cmd)){
        execPipe(cmd);
        return;
    }        

    if(checkRedirection(cmd)){
        printf("\nRedirection found\n");
        return;
    }        

    if(!checkCustom(cmd)){
        execCommand(cmd);
    }
}


/*---------------------Check_Pipe---------------------------*/
int checkPipe(char *original){

    //Make a copy of the string
    char *cmd = strdup(original);
    
    if(strstr(cmd, "|"))
        return 1;
    else
        return 0;
}


/*---------------------Check_Redirection---------------------------*/
int checkRedirection(char *original){

    //Make a copy of the string
    char *cmd = strdup(original);
    
    if(strstr(cmd, ">"))
        return 1;
    else if(strstr(cmd, "<"))
        return 1;
    else
        return 0;
}


/*---------------------Check_Custom_Command---------------------------*/
int checkCustom(char *original){

    //Make a copy of the string
    char *cmd = strdup(original);
    

    //Here add that processing of cmd
    char *file;
    char *rest; 
    char *arguments[10];

    int argc = 0;    

    file = strtok(cmd, " ");

    rest = strtok(NULL, "");

    arguments[argc++] = file;

    char *arg = strtok(rest, " ");

    while(arg != NULL){

        arguments[argc++] = arg;
        arg = strtok(NULL, " ");
    }

    arguments[argc] = NULL;

    // Checks for exit
    if(strstr(file, "exit")){
        exit(0);
    }

    // Checks for help
    if(strstr(file, "help")){

        help();
        return 1;
    }

    // Checks for version
    if(strstr(file, "version")){

        version();
        return 1;
    }

    // Checks for catt
    if(strstr(file, "catt")){

        catt(argc, arguments);
        return 1;
    }

    // Checks for headd
    if(strstr(file, "headd")){

        new_headd(argc, arguments);
        return 1;
    }

    // Checks for envv
    if(strstr(file, "envv")){

        envv();
        return 1;
    }
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


/*---------------------------ENVV-------------------------------------*/
void envv(){

    // Print all environment variables
    for(char **env = environ; *env != NULL; env++){

        printf("%s\n", *env);
    }
}


/*---------------------------CATT-------------------------------------*/
void catt(int argc, char *argv[]) {
    int i, bflag = 0, Eflag = 0, nflag = 0, sflag = 0;
    char *file;
    char last_char = '\0';
    int line = 1;
    int line_printed = 0;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
                case 'b':
                    bflag = 1;
                    break;
                case 'E':
                    Eflag = 1;
                    break;
                case 'n':
                    nflag = 1;
                    break;
                case 's':
                    sflag = 1;
                    break;
                default:
                    printf("cat: invalid option -- '%c'\n", argv[i][1]);
                    exit(1);
            }
        } else {
            file = argv[i];
            break;
        }
    }

    if (file == NULL) {
        printf("cat: missing file operand\n");
        exit(1);
    }

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        printf("cat: %s: No such file or directory\n", file);
        exit(1);
    }

    int c, next_c;
    int bflag_line = 1;
    while ((c = fgetc(fp)) != EOF) {
        if(sflag && last_char == '\n' && c == '\n'){
            while(c == '\n'){
                c = fgetc(fp);
            }
            putchar('\n');
        }
        if (last_char == '\n') {
            //putchar(c);
            line_printed = 0;
        }
        if (bflag && c == '\n' && last_char == '\n') {
            putchar('\n');
            continue;
        }
        else if(bflag && !line_printed){
            printf("%d ", bflag_line);
            line_printed = 1;
            bflag_line++;
        }
        if (nflag && !line_printed && !bflag) {
            
            printf("%d ", line);
            line_printed = 1;
            line++;
            
        }
        if (Eflag && c == '\n') {
            putchar('$');
            //putchar(c);
        }
        putchar(c);

        last_char = c;
    }


    fclose(fp);
}




/*---------------------------HEADD-------------------------------------*/
void headd(int argc, char *argv[]){

    // Set the default number of lines to print to 10
    int num_lines = 10;

    // Check if the -n flag was used to specify a different number of lines
    if(argc > 2 && argv[1][0] == '-' && argv[1][1] == 'n'){

        // Parse the number of lines from the argument
        num_lines = atoi(argv[2]);
        argc -= 2;
        argv += 2;
    }

    // Check if any file names were passed as arguments
    if(argc < 2){

        // If not, read from stdin
        char c;
        int lines_printed = 0;
        while((c = getchar()) != EOF && lines_printed < num_lines){

            putchar(c);
            if (c == '\n'){

                lines_printed++;
            }
        }
    }
    else{

        // If file names were passed as arguments, read from each file
        for(int i = 1; i < argc; i++){

            // Open the file
            FILE *fp = fopen(argv[i], "r");
            if(fp == NULL){
            
                fprintf(stderr, "Error: Could not open file %s\n", argv[i]);
                continue;
            }

            // Read the first num_lines lines of the file and print to stdout
            char c;
            int lines_printed = 0;
            while((c = fgetc(fp)) != EOF && lines_printed < num_lines){
            
                putchar(c);
                if(c == '\n'){
                
                    lines_printed++;
                }
            }

            // Close the file
            fclose(fp);
        }
    }

}





/*---------------------Execute_Command---------------------------*/
void execCommand(char *original){

    //Make a copy of the string
    char *cmd = strdup(original);
    
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


/*---------------------Execute_Pipe---------------------------*/
void execPipe(char *original){

    //Make a copy of the string
    char *cmd = strdup(original);
    
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
        printf("\nDup2 return value: %d.\n", dup2(fd[1], STDOUT_FILENO));

        if(checkCustom(cmd1)){
            printf("\nDup2 return value: %d.\n", dup2(fd[1], STDOUT_FILENO));
            exit(0);
        }
        execvp(cmd1, cmd1_args);
        
        //close(1);
        close(fd[1]);
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
        close(fd[0]);
        exit(0);
    }

    close(fd[0]);
    close(fd[1]);

    waitpid(id, NULL,  0);
    waitpid(id2, NULL, 0);
}

