#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>


#define MAX_LINE_LENGTH 1024
#define Max 20
#define WHITE  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"

char * path;

//Shell----------------------------------------------------------------------------------------------------
void shellStructure()
{
    system("clear");
    printf("\n\n<------------------------------->\n");
    printf("\n\n<------  My custom Shell  ------>\n");
    printf("\n\n<------Type help  for info------>\n");
    printf("\n\n<----Made ---------------------->\n");
    printf("\n\n<------------------------------->\n\n\n");

}

//Help-----------------------------------------------------------------------------------------------------
void help()
{
    printf("\nWelcome to help menu! These are the things you want to know:\n");
    printf("What I implemented: simple commands, piped commands, custom commands\n");
    printf("Custom functions:\n");
    printf("ls with parameters: -l, -s, -a, -F\n");
    printf("tac with parameters: -b, -s\n");
    printf("dir whit no parameters\n");
    printf("Custom help function\n");


}


//LS-------------------------------------------------------------------------------------------------------
int lsCom(int argc, char* argv[]) {

	char dir_entry[5] = ".";
	//group id Transfer name
	
	if(argc > 1)
	{
		if(strcmp(argv[1], "--version") == 0)
		{
			printf("\nVersion: Custom ls 1.0V @2022\n");
			return EXIT_SUCCESS;
		}
		
		if(strcmp(argv[1], "--help") == 0)
		{
			printf("\nUsage: ls [OPTION]... [FILE]...\n");
			printf("List information about the FILEs (the current directory by default).\n");
			printf("Implemented arguments:\n");
			printf("-l: use a long listing format\n");
			printf("-a: do not ignore entries starting with .\n");
			printf("-s: print the allocated size of each file, in blocks\n");
			printf("-F: append indicator (one of */=>@|) to entries\n");
			return EXIT_SUCCESS;
		}
	}
	
	DIR* dir;
	struct dirent* direct;
	int cont, size_sorted;
	char* sorted[200];
	char* type_of[202];
	char* username = getenv("USER");
       
       
        dir = opendir( dir_entry );
        
        if(dir == NULL)
        {
		perror( "Error" );
          	exit( -1 );
        }
        
        else 
        {
        	size_sorted = 0;
        	while( ( direct = readdir( dir ) ) != NULL ) 
        	{
        		
        		        		
        		sorted[size_sorted] = (char*) malloc (strlen(direct->d_name) + 1);
        		strncpy(sorted[size_sorted], direct->d_name, strlen(direct->d_name) );
        		
        		//core--> mode_t st_mode = stat_info->st_mode;
        		struct stat* stat_info = NULL;
        		
        		//char str[11] = "\0";
		        //strcpy( str, "----------" );
		        
        		//if( S_ISREG( st_mode ) ) str[0] = '-';
	     		//if( S_ISDIR( st_mode ) ) str[0] = 'd';
	     		//if( S_ISLNK( st_mode ) ) str[0] = 'l';
	 
	     		//if( st_mode & S_IRUSR ) str[1] = 'r';
	     		//if( st_mode & S_IWUSR ) str[2] = 'w';
	     		//if( st_mode & S_IXUSR ) str[3] = 'x';
 
	     		//if( st_mode & S_IRGRP ) str[4] = 'r';
	     		//if( st_mode & S_IWGRP ) str[5] = 'w';
	     		//if( st_mode & S_IXGRP ) str[6] = 'x';
	 
	     		//if( st_mode & S_IROTH ) str[7] = 'r';
	    		//if( st_mode & S_IWOTH ) str[8] = 'w';
	     		//if( st_mode & S_IXOTH ) str[9] = 'x';
		        
		        
		        //printf( "%s\t", str );
		  
		        //printf( "%ld\t", stat_info->st_nlink ); //Symbolic link
		        //printf("%s\t", username); //user name
		        //printf( "%s\t", getgrgid( stat_info->st_gid )->gr_name); //Group name
		        //printf( "%10ld", stat_info->st_size ); //file size
		        //show_time( statinfo->st_mtime ); //Last modification time
		        //printf( "\t%s", direct->d_name );
		 	//printf( "\n" );
					
        		
			//DT_UNKNOWN: The type is unknown. On some systems this is the only value returned.
			//DT_REG: A regular file.
			//DT_DIR: A directory.
			//DT_FIFO: A named pipe, or FIFO. See section FIFO Special Files.
			//DT_SOCK: A local-domain socket.
			//DT_CHR: A character device.
			//DT_BLK: A block device.
        		if(direct->d_type == DT_DIR)
        		{
        			type_of[size_sorted] = (char*) malloc (sizeof(char*) + 1);
        			strcpy(type_of[size_sorted], "blue");
        		}
        		
        		else if(direct->d_type == DT_WHT)
        		{	
        			
        			type_of[size_sorted] = (char*) malloc (sizeof(char*) + 1);
        			strcpy(type_of[size_sorted], "green");
        		}
        			
        		else if (direct->d_type == DT_REG)
        		{
        			type_of[size_sorted] = (char*) malloc (sizeof(char*) + 1);
        			strcpy(type_of[size_sorted], "white");
        		}
        		
        		size_sorted++;
               
           	}
           	printf("\n");
           	
           	
		char *aux;
		int i,j;
		
		//sort the files array and the collor array in alphabetical order
		for(i = 0; i < size_sorted; i++)
			for(j = i; j < size_sorted - i; j++)
	      			if(strcmp(sorted[i], sorted[j]) > 0)
	      			{
	      				aux = (char*) malloc (strlen(sorted[i]) + 1);
					strncpy(aux, sorted[i], strlen(sorted[i]));
					
					
					sorted[i] = (char*) malloc (strlen(sorted[j]) + 1);
					strncpy(sorted[i], sorted[j], strlen(sorted[j]));
					
					sorted[j] = (char*) malloc (strlen(aux) + 1);
					strncpy(sorted[j], aux, strlen(aux));
					
					
					aux = (char*) malloc (strlen(type_of[i]) + 1);
					strncpy(aux, type_of[i], strlen(type_of[i]));
					
					
					type_of[i] = (char*) malloc (strlen(type_of[j]) + 1);
					strncpy(type_of[i], type_of[j], strlen(type_of[j]));
					
					type_of[j] = (char*) malloc (strlen(aux) + 1);
					strncpy(type_of[j], aux, strlen(aux));
					
					
					
					
				}
		// for no argument		
		if(argc <= 1)
		{
			cont = 0;
			for(i = 0; i < size_sorted; i++)
			{
				if(sorted[i][0] != '.')
				{
					if(strcmp(type_of[i], "blue") == 0)
						printf("%s%s \t",BLUE, sorted[i]);
					
						
					else if(strcmp(type_of[i], "green") == 0)
						printf("%s%s \t",GREEN, sorted[i]);
						
					else
						printf("%s%s \t",WHITE, sorted[i]);
					
					cont++;
					
					if(cont == 6)
				       {
				       	printf("\n");
				       	cont = 0;
				       }
				}
				
			}
			printf("%s\n",WHITE);
		}
		
		
		
		
		
		else{
		
		// -a for 0; -s for 1; -l for 2; -F for 3
		int ok = 0;
		int flags[4] = {0, 0, 0, 0};
		
		for(i = 2; i <= argc; i++)
			if(i != argc)
			{
				if(strcmp(argv[i - 1], "-a") == 0)
					flags[0] = 1;
				
				if(strcmp(argv[i - 1], "-s") == 0)
					flags[1] = 1;
				
				if(strcmp(argv[i - 1], "-l") == 0)
					flags[2] = 1;
					
				if(strcmp(argv[i - 1], "-F") == 0)
					flags[3] = 1;
			}
			else
				if(argv[i - 1][0] != '-')
					ok = 1;
				else
				{
					if(strcmp(argv[i - 1], "-a") == 0)
					flags[0] = 1;
				
					if(strcmp(argv[i - 1], "-s") == 0)
						flags[1] = 1;
					
					if(strcmp(argv[i - 1], "-l") == 0)
						flags[2] = 1;
						
					if(strcmp(argv[i - 1], "-F") == 0)
						flags[3] = 1;
				}
		
			
		printf("\n");
		
		
		cont = 0;
		for(i = 0; i < size_sorted; i++)
		{
			if(strcmp(type_of[i], "blue") == 0)
				printf("%s%s	  ",BLUE, sorted[i]);
				
			else if(strcmp(type_of[i], "green") == 0)
				printf("%s%s	  ",GREEN, sorted[i]);
				
			else
				printf("%s%s	  ",WHITE, sorted[i]);
			
			cont++;
			
			if(cont == 6)
		       {
		       	printf("\n");
		       	cont = 0;
		       }
		}
		printf("%s	",WHITE);}
        
       }
    	
	return 0;
}

//TAC------------------------------------------------------------------------------------------------------
int tacCom(int argc, char* argv[]) {

	ssize_t read;

	if(argc > 1)
	{
		if(strcmp(argv[1], "--version") == 0)
		{
			printf("\nVersion: Custom tac 1.0V @2022\n");
			return EXIT_SUCCESS;
		}
		
		if(strcmp(argv[1], "--help") == 0)
		{
			printf("\nUsage: tac [OPTION]... [FILE]...\n");
			printf("Write each FILE to standard output, last line first.\n");
			printf("Implemented arguments:\n");
			printf("-b: attach the separator before instead of after\n");
			printf("-s: use STRING as the separator instead of newline\n");
			return EXIT_SUCCESS;
		}
		
	}
	int file_numb = 0;
	char* line = NULL;
    	size_t len = 0;
    	int arr_size = 0, sir_size = 0;
    	char* arr[500];
    	char* sir[500];
    	int okfile1 = 0, okfile2 = 0;
	
	if(argv[argc - 1][0] == '-')
	{
		perror("Error: file not selected");
		return EXIT_FAILURE;
	}	
	else
	{
		FILE* fpntr = fopen(argv[argc - 1], "r");
		
		if (!fpntr) 
		{
        		printf("Error opening file: %s\n", argv[argc - 1]);
   			return EXIT_FAILURE;
    		}

    		if ((fpntr = fopen(argv[argc - 1], "r")) == NULL) 
    		{
        		printf("Cannot open file %s \n", argv[argc - 1]);
        		exit(0);
    		}
    		
    		while((read = getline(&line, &len, fpntr)) != -1)
    		{
    			arr[arr_size] = (char*) malloc (strlen(line) + 1);
    			strncpy(arr[arr_size], line, strlen(line));
    			arr_size++;
    		}
    		okfile1 = 1;
    		fclose(fpntr);
    		free(line);	
	}
	
	if(argv[argc - 2][0] != '-' && argc > 2)
	{
		FILE* fpntr2 = fopen(argv[argc - 2], "r");
		line = NULL;
		if (!fpntr2) 
		{
        		printf("Error opening file: %s\n", argv[argc - 2]);
   			return EXIT_FAILURE;
    		}

    		if ((fpntr2 = fopen(argv[argc - 2], "r")) == NULL) 
    		{
        		printf("Cannot open file %s \n", argv[argc - 2]);
        		exit(0);
    		}
    		
    		while((read = getline(&line, &len, fpntr2)) != -1)
    		{
    			sir[sir_size] = (char*) malloc (strlen(line) + 1);
    			strncpy(sir[sir_size], line, strlen(line));
    			sir_size++;
    		}
    		okfile2 = 1;
    		fclose(fpntr2);
    		free(line);	
	}
	
	int b_arg = 0;
	int s_arg = 0;
	for (int i = 2; i < argc; i++)
	{
		if(strcmp(argv[i - 1], "-b") == 0)
			b_arg = 1;
			
		if(strcmp(argv[i - 1], "-s") == 0)
			s_arg = 1;
	}
	
	int i;
    	if(b_arg == 1)
    	{
    		printf("\n");
    		printf("\n");
    		if(okfile1)
    		{
    			i = arr_size - 1;
    			while(i > 1)
    			{
    				printf("%s", arr[i]);
    				i--;
    			}
    			printf("%s", strcat(arr[1],arr[0]));
    		}
    		
    		
    				
    		printf("\n");	
    			
    		if(okfile2)
    		{
    			i = sir_size - 1;
    			while(i > 1)
    			{
    				printf("%s", sir[i]);
    				i--;
    			}
    			printf("%s", strcat(sir[1],sir[0]));
    		}
    		
    	}
    	else
    	{
    		if(okfile1)
    			for(int i = arr_size - 1; i >= 0; i--)
    				printf("%s",arr[i]);
    				
    		if(okfile2)
    			for(int i = sir_size - 1; i >= 0; i--)
    				printf("%s",sir[i]);
    	}
    	
	return EXIT_SUCCESS;
}
//DIR------------------------------------------------------------------------------------------------------
int dirCom(int argc, char *argv[]) {

	char dir_entry[5] = ".";

	if(argc > 1)
	{
		if(strcmp(argv[1], "--version") == 0)
		{
			printf("\nVersion: Custom dir 1.0V @2022\n");
			return EXIT_SUCCESS;
		}
		
		if(strcmp(argv[1], "--help") == 0)
		{
			printf("\nUsage: dir [FILE]\n");
			printf("List information about the FILEs (the current directory by default).\n");
			printf("Implemented arguments:\n");
			printf("-b: attach the separator before instead of after\n");
			printf("-s: use STRING as the separator instead of newline\n");
			return EXIT_SUCCESS;
		}
	}
	
	DIR* dir;
	struct dirent* direct;
	int cont, size_sorted;
	char* sorted[200];
 
       
        dir = opendir( dir_entry );
        
        if(dir == NULL)
        {
		perror( "Error" );
          	exit( -1 );
        }
        
        else 
        {
        	size_sorted = 0;
        	while( ( direct = readdir( dir ) ) != NULL ) 
        	{
        		sorted[size_sorted] = (char*) malloc (strlen(direct->d_name) + 1);
        		strncpy(sorted[size_sorted], direct->d_name, strlen(direct->d_name) );
        		
        		size_sorted++;
               
           	}
           	printf("\n");
           	
           	
		char *aux;
		int i,j;
		
		//sort the files array and the collor array in alphabetical order
		for(i = 0; i < size_sorted; i++)
			for(j = i; j < size_sorted - i; j++)
	      			if(strcmp(sorted[i], sorted[j]) > 0)
	      			{
	      				aux = (char*) malloc (strlen(sorted[i]) + 1);
					strncpy(aux, sorted[i], strlen(sorted[i]));
					
					
					sorted[i] = (char*) malloc (strlen(sorted[j]) + 1);
					strncpy(sorted[i], sorted[j], strlen(sorted[j]));
					
					sorted[j] = (char*) malloc (strlen(aux) + 1);
					strncpy(sorted[j], aux, strlen(aux));		
				}
				
		if(argc <= 1)
		{
			cont = 0;
			for(i = 0; i < size_sorted; i++)
			{
				if(sorted[i][0] != '.')
				{
					printf("%s	", sorted[i]);
					cont++;
					
					if(cont == 6)
				       {
				       	printf("\n");
				       	cont = 0;
				       }
				}
				
			}
		}
		
		
		else
			perror("Error: no argument needed");

        
       }
       
	printf("\n");
	return 0;
}

//Custom-Commands--check----------------------------------------------------------------------------------------
int customCommands(int argc, char **args) {
	
 	
 	if(strcmp(args[0], "exit") == 0)
 	{
 		exit(0);
 		return 1;
 	}
 		
 	else if(strcmp(args[0], "help") == 0)
 	{
 		help();
 		return 1;
 	}
 	
 	else if(strcmp(args[0], "ls") == 0)
 	{
 		lsCom(argc, args);
 		return 1;
 	}
 	
 	else if(strcmp(args[0], "tac") == 0)
 	{
 		tacCom(argc, args);
 		return 1;
 	}
 	
 	else if(strcmp(args[0], "dir") == 0)
 	{
 		dirCom(argc, args);
 		return 1;
 	}
 	
 	else
 	{
 		return 0;
 	}
	
}

// parsing------------------------------------------------------------------------------------------
char **pathPars(){
	
	char** arg=(char **)malloc(20 * sizeof(char*));
	int i;
	char* tok;
	
	tok = strtok(path,":");
	i = 0;
	do{
		arg[i] = strdup(tok);
		tok = strtok(0,":");
		i++;
	}while(tok != 0);
	
	arg[i] = NULL;
	return arg;
}

// input redirection------------------------------------------------------------------------------------------
int inpRed(char **args, char **input_filename) {

	
  	int i = 0, j;
	if(args[i] != NULL)
	  	do{
	  		if(args[i][0] == '<') {
	    
		      		// Read the filename
		      		if(args[i+1] != NULL) 
					*input_filename = args[i+1];
		      		else 
					return -1;

		      		for(j = i; args[j-1] != NULL; j++) {
					args[j] = args[j+2];
		      		}	

		      		return 1;
		    	}
		    	i++;
	  	
	  	}while(args[i] != NULL);
			
  	return 0;
}

//output redirection-----------------------------------------------------------------------------------------
int outRed(char **args, char **output_filename) {

  	int i = 0;
  	int j;
	
	if(args[i] != NULL)
		do{
			if(args[i][0] == '>') {

			      	// Get the filename
			      	if(args[i+1] != NULL) 
					*output_filename = args[i+1];		    
			      	else 
					return -1;

			      	for(j = i; args[j-1] != NULL; j++) {
					args[j] = args[j+2];
			      	}

			      	return 1;
			}
			
			i++;
		
		
		}while(args[i] != NULL);
	  	

	    		
	  	
  	return 0;
}

// check for pipeline---------------------------------------------------------------------------------------
int sepArg(char **args,char** left){

	int i=0, j, k;
	
	if(args[i] != NULL)
		do{
			if(args[i][0] == '|') {

				j=0;
				do{
					if(args[j] != NULL) 
						left[j] = args[j];				
					else
						return -1;
					j++;				
				}while(j<i);		
				left[j] = NULL;
				
				for(j = i+1 ,k=0; args[j] != NULL; j++,k++) {
					args[k] = args[j];
				}
				args[k] = NULL;
				return 1;
			}
			i++;
			
		}while(args[i] != NULL);
	
	
		
	return 0;
}
//check if there is a pipe------------------------------------------------------------------------------
int pipeCheck(char **args){

	int i = 0;
	
	for(int i = 0; args[i] != NULL; i++)
		if(args[i][0] == '|') 
			return 1;
	return 0;

}


//------------------------------------------------------------------------------------------
void redCheck(char **input_filename, char **output_filename, int* input,int* output,char **args) {

    	*input = inpRed(args, input_filename);
    	
    	if(*input == -1)
      		printf("Syntax error!");
    	
    	else if(*input == 1)
      		printf("Redirecting input from %s ", *input_filename);
      		

      		

    	*output = outRed(args, output_filename);

    	if(*output == -1)
	      	printf("Syntax error!\n");
	
	else if(*output == 1)
      		printf("Redirecting output to %s ", *output_filename);
      		
    	else if(*output == 2)
	      	printf("Appending output to the end of %s",*output_filename);
	      	

}

//------------------------------------------------------------------------------------------
int bashCom(char **args) {

	char ** left = (char **)malloc(100 * sizeof(char *));
	int result, result1, status, status1;
	int p[2];
	
	pid_t child_id, child_id1;

	if(args==NULL)
	     exit(-1);

    if(pipeCheck(args) == 1){

        sepArg(args, left);

        pipe(p);

        // Fork the child process
        child_id = fork();
       
             	
        if(child_id == 0){
        
             
            	char* output_filename=NULL;
            	int input=0, output=0;
            	char* input_filename=NULL;

            	redCheck(&input_filename, &output_filename,&input,&output,left);
            	
            	// Set up redirection in the child process
            	if(input)
                	freopen(input_filename, "r", stdin);
                
            	switch(output){
            		case 1:
                    		freopen(output_filename, "w+", stdout);
                    		break;
            		case 2:
                    		freopen(output_filename, "a", stdout);
                    		break;
            	}

            	close(1);
            	dup(p[1]);
            	close(p[0]);
            	close(p[1]);

            	// Execute the command
            	char **paths=pathPars();
            	int k;
            
            	for(k = 0; paths[k]!=NULL; k++){
                	result = execv(strcat(paths[k],left[0]),left);
                	free(paths[k]);
            	}
            
            	free(paths);
            	exit(-1);
            	
        }

        result = waitpid(child_id, &status, 0);
        
        child_id1 = fork();
        if(child_id1 == 0) {
		close(0);
		dup(p[0]);
		close(p[0]);
		close(p[1]);
		bashCom(args);
		exit(0);
		
	}
        
        close(p[0]);
        close(p[1]);
    
        result1 = waitpid(child_id1, &status1, 0);
    }
    
    else{

	// Fork the child process
        child_id = fork();

	// Check for errors
        
        if(child_id == 0){
	    	char *input_filename=NULL;
            	int input=0, output=0;
            	char *output_filename=NULL;

            	redCheck(&input_filename, &output_filename,&input,&output,args);

            	printf("\n");
	
            	if(input){
                	freopen(input_filename, "r", stdin);
            	}

            	 
            	switch(output){
            		case 1:
                    		freopen(output_filename, "w+", stdout);
                    		break;
            		case 2:
                    		freopen(output_filename, "a", stdout);
                    		break;
            	}

            	// Execute the command
            	char **paths=pathPars();
            	int k;
            
            	//do while
            	for(k = 0; paths[k]!=NULL; k++){
                	result = execv(strcat(paths[k],args[0]),args);
                	free(paths[k]);//--<<
            	}
            
            	free(paths);
            	exit(-1);
        	}

        // Wait for the child process to complete
        result = waitpid(child_id, &status, 0);
    }

}     
//main----check--------------------------------------------------------------------------------------
void main() {
    	
    	path= (char *)malloc(101 * sizeof(char));

	//paths
	strcat(path,"/bin/:/usr/bin/:");
    	
	char *input[Max];
	int i;
	char *buf, *p;
    	int pos;
    	
    	shellStructure();

	while(1) {
    		
    		// read the command from terminal
    		buf = readline("\n~$ ");
    		
    		// verify if the command is empty, if not -> add it to shell history
    		if (strlen(buf) != 0)
        		add_history(buf);
        	else
        		continue;
        	
        	
        	// modify input positions to NULL to avoid errors
        	for (int ind = 0; ind < Max; ind++) 
            		input[ind] = NULL;
        
        	if (*buf == '\n')         
        		continue;

        
        	pos = 0;

    		p = strtok(buf, " \n");
    		while(p)
    		{
       		input[pos] = p;
     			pos++;
      			p = strtok(NULL, " \n");
    		}
    		
        	
		// check if a custom command is requested
		if(customCommands(pos, input))
			continue;
			
		// execute other commands
		bashCom(input);
	}
}



