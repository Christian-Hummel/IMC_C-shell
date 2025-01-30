#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>



int main(){

    // initialize variables for comparison
    const char version[128] = "IMCSH Version 1.0 created by Alexander Berger and Christian Hummel";
    const char execute[] = "exec";
    const char filemod[] = ">";
    const char background[] = "&";
    const char quit[] = "quit";
    const char delimiter[] = " ";
    
    // initialize input buffer
    char input[128];
    
    
    while (1){

        if (fgets(input, sizeof(input), stdin) == 0){
            printf("Error reading input");
            break;
        } else {

            // remove newline character at the end
            input[strcspn(input, "\n")] = 0;

            // set up array for storing options 

            char* argument_list[256] = {"ls","-a",NULL};

            char * token = strtok(input, delimiter);

            if (strcmp(quit,token) == 0){
                printf("Exiting...\n");
                break;
            } else if (strcmp(execute,token) == 0) {
                
                // call strtok function again to skip exec
                token = strtok(NULL, delimiter);

                // define pointer to a character array
                char * argument_array[1024];
                int i = 0;

                while(token != NULL){

                    if (strcmp(filemod,token) == 0){
                        printf("filemodifier %s\n",token);
                    } else if (strcmp(background,token) == 0){
                        printf("background process modifier %s\n",token);
                    }

                    // printf("Portion %s\n", token);
                    
                    argument_array[i] = token;

                    token = strtok(NULL, delimiter);

                    i++;
                    
                    
                    // leads to segmentation fault because > is maybe not in the ascii table, that gets used for strcmp()
                    // printf("Comparing elements: %d %s %s\n", strcmp(portion, argument_list[0]), portion, argument_list[0]);

                }

                // execute command, first element, and options provided by the user
                execvp(argument_array[0], argument_array);           
                  
            } else {
                printf("Inserted %s\n", input);
            }

        }
            
        
    };


    // if (strcmp(portion, argument_list[0]) == 0){
    //     execvp(portion, argument_list);
    //     continue;
    // }


    //     int id = fork();

    // if (id < 0) {
    //     fprintf(stderr, "fork failed\n");
    // } else if (id == 0) {

    //     printf("Child process\n");

    // } else {
    //     wait(NULL);
    //     printf("Parent procees\n");
        

    // }

    return 0;
}
