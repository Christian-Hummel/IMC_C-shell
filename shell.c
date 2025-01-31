#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>



int main(){

    // initialize variables for comparison
    const char version[] = "version";
    const char execute[] = "exec";
    const char filemod[] = ">";
    const char background[] = "&";
    const char quit[] = "quit";
    const char delimiter[] = " ";

    // initialize flags for modifiers and commands

    int exe = 0;
    int sfile = 0;
    int bg = 0;
    
    // initialize input buffer
    char input[128];
    
    
    while (1){

        if (fgets(input, sizeof(input), stdin) == 0){

            printf("Error reading input");
            break;

        } else {

            // remove newline character at the end
            input[strcspn(input, "\n")] = 0;

            char *token = strtok(input, delimiter);

            if (strcmp(quit,token) == 0){
                
                printf("Exiting...\n");
                break;

            } else if (strcmp(version,token) ==0) {

                printf("IMCSH Version 1.0 created by Alexander Berger and Christian Hummel\n");

            } else if (strcmp(execute,token) == 0) {

                // call fork to start child process 

                pid_t pid;

                pid = fork();

                if (pid < 0) {

                    fprintf(stderr, "fork failed\n");

                } else if (pid == 0) {

                    printf("Child process\n");

                    // define pointer to a character array
                    char *argument_array[1024];
                        
                    // index for argument_array
                    int i = 0;

                    // variable for storing filepath for > command 
                    char filepath[128];

                    while(token != NULL){

                        if (exe == 0){

                            if(strcmp(execute, token) == 0){

                                printf("Execute command %s\n", token);
                                exe = 1;
                            }

                            
                        } else if (exe == 1){

                            if (strcmp(filemod,token) == 0){

                                sfile = 1;

                                token = strtok(NULL, delimiter);

                                strcpy(filepath, token);

                            }

                            else if (strcmp(background,token) == 0){

                                bg = 1;
                            

                            } else {

                                argument_array[i] = token;

                                i++;
                            
                            }

                        }

                        
                        token = strtok(NULL, delimiter);

                    }

                    
                    //execute program with > modifier
                    //run program in background with & modifier
                    //execute program without modifiers

                    if (exe == 1 && sfile == 1 && bg == 0){

                        printf("File modifier\n");

                        printf("Filepath %s\n", filepath);

                        int file = open(filepath, O_WRONLY | O_APPEND | O_CREAT, 0777);

                        dup2(file, STDOUT_FILENO);
                        close(file);

                        if (file == -1){
                            return 1;
                        } else {
                            execvp(argument_array[0], argument_array);
                        }


                    } else if (exe == 1 && sfile == 0 && bg == 1) {

                        printf("Background modifier\n");
                        break;

                    } else {

                        execvp(argument_array[0], argument_array);

                    }


                } else {

                    if (sfile == 1){

                        wait(NULL);
                        printf("Process finished with ID %d\n", pid);

                    } else if (bg == 1){

                        printf("No wait call needed\n");
                        printf("Process finished with ID %d\n", pid);
                            
                    } else {

                        wait(NULL);
                        printf("Process finished with ID %d\n", pid);

                    }

                    
                }       
                  
            } else {
                printf("Inserted %s\n", input);
            }

        }

                    
        
    };

    return 0;
}