// Name: David Weir
// Student Number: 19433086
// I acknowledge all terms of DCU's Academic Ibtegrity Policy

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "shell_functions.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators

int main(int argc, char ** argv, char *envp[])
{
    char buf[MAX_BUFFER];
    char ** args;
    char shell[1024]; // this will hold a new shell environment variable

    strcat(shell, getenv("PWD")); // get the current PWD environment variable and store it in shell
    strcat(shell, "/myshell"); // concatanate the shell and "/myshell"
    setenv("SHELL", shell, 1); // add this to the environment variables

    welcome(); // calls the welcome command to welcome the user to the shell

    if(argc == 2)
    {
        FILE *batchfile;
        char c[100];

        batchfile = fopen(argv[1], "r"); // open the batchfile

        if(batchfile == NULL) // if it is empty inform the user no commands are inputted
        {
            printf("Error: no commands supplied.");
        }
        else
        {
            while(fgets(c, sizeof(c), batchfile) != NULL) // while not at the end of the file
            {
                char ** command = split_line(c); // parse the line using the split_line function as if the command was taken from the shell prompt
                execute(command); // run the execute function to run the commands on each line
                printf("\n");
            }
        }

        fclose(batchfile); // close the batchfile containing the commands
    }

    // loop prints out the shell prompt and takes user input until the user inputs "quit", activating the quit command which exits the shell program
    while(!feof(stdin))
    {

        // prints out prompt with each loop
       prompt();

       if(fgets(buf, MAX_BUFFER, stdin)) // reads user input
       {
        // parse input into components

            args = split_line(buf); // tokenizes user input using the split_line command

            // if the command line was not empty use execute to find the user given command and execute it
            if(args[0] != NULL)
            {
                if(hasampersand(args) == 1) // hasampersand() returns 1 if an ampersand is found at the end of the input
                {
                    backgr_exe(args); // invokes background execution function
                }
                else
                {
                    // if the user inputs pause call the pause function
                    if(!strcmp(args[0], "pause"))
                    {
                        pauseShell();
                    }
                    // if the user uses the environ command use the enviro function else use execute to find the user's given command
                    else if(!strcmp(args[0], "environ"))
                    {
                        enviro(envp);
                    }
                    else
                    {
                        execute(args);
                    }
                }
            }  
        }
    }

    return 0;
}

