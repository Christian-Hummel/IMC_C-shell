#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

void handle_sigchld(){

    int pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        printf("Process finished with ID %d\n", pid);
    }

}

int main(){
    
    // set up signal handler
    
    signal(SIGCHLD, &handle_sigchld);

    // initialize variables for comparison

    const char version[] = "globalusage";
    const char execute[] = "exec";
    const char filemod[] = ">";
    const char background[] = "&";
    const char quit[] = "quit";
    const char delimiter[] = " ";
    const char yes[] = "y";
    const char Yes[] = "Y";
    const char no[] = "n";
    const char No[] = "N";

    // initialize flags for modifiers and commands

    int exe = 0;
    int sfile = 0;
    int bg = 0;
    
    // initialize input buffer
    char input[128];

    // initialize array for running background processes

    pid_t bg_processes[128];
    int bg_count = 0;

    char response[64];


    
    while (1){

        if (fgets(input, sizeof(input), stdin) == 0){

            printf("Error reading input");
            break;

        } else {

            // remove newline character at the end
            input[strcspn(input, "\n")] = 0;

            // define pointer to a character array
            char *argument_array[1024];
                        
            // index for argument_array
            int i = 0;

            // variable for storing filepath for > command 
            char filepath[128];

            char *token = strtok(input, delimiter);

            if (strcmp(quit,token) == 0 && strtok(NULL, delimiter) == NULL){

                if (bg_count > 0){

                    printf("The following processes are running, are you sure you want to quit? [Y/n]\n");

                    int j;

                    for (j = 0; j < bg_count; j++){

                        printf("Background process with pid: %d\n", bg_processes[j]);

                    }
                

                    

                    if (fgets(response, sizeof(response), stdin) == 0){

                        printf("Error reading input");
                        break;

                    } else {

                        response[strcspn(response, "\n")] = 0;

                        // If the response is empty, treat it as 'n' for no
                        if (strlen(response) == 0) {

                            strcpy(response, "n");

                        }

                        if (strcmp(yes,response) == 0 || strcmp(Yes,response) == 0){

                            printf("Exiting...\n");

                            for (int j = 0; j < bg_count; j++) {
                            kill(bg_processes[j], SIGKILL);
                            }

                            break;

                        } else if (strcmp(no,response) == 0 || strcmp(No, response) == 0){

                            printf("Continue execution\n");
                            
                        } else {

                            printf("Wrong input, continue execution\n");
                            
                        }
                    }

                } else {
                    printf("Exiting...\n");
                    break;
                }
                
                

            } else if (strcmp(version,token) ==0 && strtok(NULL, delimiter) == NULL) {

                printf("IMCSH Version 1.0 created by Alexander Berger and Christian Hummel\n");

            } else if (strcmp(execute,token) == 0) {

            // parse input string

            while(token != NULL){

                if (exe == 0){

                    if(strcmp(execute, token) == 0){

                        // printf("Execute command %s\n", token);
                        exe = 1;
                        }

                            
                } else if (exe == 1){

                    if (strcmp(filemod,token) == 0){

                        sfile = 1;

                        token = strtok(NULL, delimiter);

                        strcpy(filepath, token);

                        }

                    else if (strcmp(background,token) == 0 && strtok(NULL, delimiter) == NULL){

                        bg = 1;                           

                    } else {

                        argument_array[i] = token;

                        i++;
                            
                        }

                    }

                    token = strtok(NULL, delimiter);

                }

                argument_array[i] = NULL;

                // create temporary flags

                int current_sfile = sfile;
                int current_bg = bg;


                // reset flags

                exe = 0;
                sfile = 0;
                bg = 0;


                // call fork to start child process 

                pid_t pid;

                pid = fork();

                if (pid < 0) {

                    fprintf(stderr, "fork failed\n");

                } else if (pid == 0) {

                    // Child Process
            
                    //execute command with > modifier

                    if (current_sfile == 1 && current_bg == 0){

                        int file = open(filepath, O_WRONLY | O_APPEND | O_CREAT, 0777);

                        dup2(file, STDOUT_FILENO);
                        close(file);

                        if (file == -1){
                            return 1;
                        } else {
                            execvp(argument_array[0], argument_array);
                        }

                    //run command in background with & modifier

                    } else if (current_sfile == 0 && current_bg == 1) {

                        // change process group to background

                        setpgid(0, 0);

                        execvp(argument_array[0], argument_array);

                    //run command in background, stored in file

                    } else if (current_sfile == 1 && current_bg == 1){

                        // change process group to background

                        setpgid(0, 0);

                        int file = open(filepath, O_WRONLY | O_APPEND | O_CREAT, 0777);

                        dup2(file, STDOUT_FILENO);
                        close(file);

                        if (file == -1){
                            return 1;
                        } else {
                            execvp(argument_array[0], argument_array);
                        }

                    //execute command without modifiers
                    
                    } else {

                        execvp(argument_array[0], argument_array);

                    }


                } else {

                    //Parent Process


                    if (current_sfile == 1 && current_bg == 0){

                        wait(NULL);

                        printf("Process finished with ID %d\n", pid);
                        

                    } else if (current_sfile == 0 && current_bg == 1){

                        
                        // printf("Background Process\n");
                        bg_processes[bg_count] = pid;
                        bg_count ++;
                        
                        

                    } else if (current_sfile == 1 && current_bg == 1){

                        // printf("Background Process stored in file\n");
                        bg_processes[bg_count] = pid;
                        bg_count ++;
                        

                    } else {

                        wait(NULL);

                        printf("Process finished with ID %d\n", pid);

                    }

                }       
                  
            } else {
                printf("Error: command %s not found\n", input);
            }

        }
        
    };

    return 0;
}