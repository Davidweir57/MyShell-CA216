// Name: David Weir
// Student Number: 19433086
// I acknowledge all terms of DCU's Academic Ibtegrity Policys

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "commands.h"

#define MAX_BUFFER 1024                        // max line buffer
#define MAX_ARGS 64                            // max # args
#define SEPARATORS " \t\n"                     // token sparators

// builds intro to my shell
void welcome() 
{  
    printf("******************************************"); 
    printf("\n\n\n\t****  MY SHELL  ****"); 
    printf("\n\n\t-CA216 Assignment 1-");
    printf("\nName: David Weir");
    printf("\nStudent Number: 19433086");
    printf("\n\n\n******************************************"); 
    char* username = getenv("USER"); 
    printf("\n\n\nUSER is: @%s", username); 
    printf("\nUse \"help\" to access the shell's user manual.");
    printf("\n"); 
    sleep(1); 
}

int hasampersand(char *args[])
{
    int last=0;

    // loops through the tokenized input incrementing the last variable to find the index of the last element
    for(int i=1; args[i]; i++)
    {
        last++;
    }

    // if the last token is an & symbol return 1 else return 0
    if(!strcmp(args[last], "&"))
    {
    	return 1;
    }
    else
    {
        return 0;
    }
}

// function tokenizes the user given command line seperated by whitespace delimiters
// function was built using instructions provided in a guide for a simple shell found here: https://brennan.io/2015/01/16/write-a-shell-in-c/
char **split_line(char *line)
{
  int bufsize = MAX_ARGS, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens)
  {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SEPARATORS);
  while (token != NULL)
  {
    tokens[position] = token;
    position++;

    if (position >= bufsize)
    {
      bufsize += MAX_ARGS;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens)
      {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, SEPARATORS);
  }
  tokens[position] = NULL;
  return tokens;
}

// prints our current working directory as part of the prompt
void prompt()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd)); // gets the current working dir
    printf("\n%s\n >>> ", cwd); // prints the cwd then the prompt ">>>"
}

// function forks to create a child process and runs execvp on the given command
// here we wait for the child process to finish
int external_exe(char *args[])
{
    char *command = args[0]; // user command e.g. ls, mkdir
    char *cmd[3]; // array of the commands arguments
    cmd[0]=args[1], cmd[1]=args[2], cmd[2]=NULL;

    pid_t pid = fork(); // create child process

    // fork error handling
    if(pid == -1)
    {
        printf("Error: fork failed.");
    }
    else if(pid == 0)
    {
        printf("%s", command);
        execvp(command, cmd);
    }

    wait(NULL); // wait for child process to end
}

// function to find and execute command provided by user
int execute(char **args)
{
    // a char array of the possible internal commands a user can give
    char *internal_cmds[] = {
        "quit",
        "clr",
        "mkdir",
        "help",
        "dir",
        "echo",
        "environ",
        "cd"
    };
    

    // array of command's functions
    int (*commands[]) (char **) = {
        &quit,
        &clear,
        &makedir,
        &help,
        &dir,
        &echo,
        &enviro,
        &cd,
    };

    int foundInt=0;

    if(args[0] == NULL) // check if user has given a command
    {
        // user entered no command i.e. an empty command input
        return 1;
    }
    else
    {
        for(int i=0; i<(sizeof(internal_cmds) / sizeof(char *)); i++) // loops through array of internal commands
        {
            if(!strcmp(args[0], internal_cmds[i])) // finds the command that matches the user's input
            {
                foundInt++; // if the command is found to be an internal command increment foundInt(ernal)
                return (*commands[i])(args); // executes the given command
            }
        }

        // if we could not find an internal command matching the users input we run the external command function to handle the non-built-in
        // command, so long as it is a valid command
        if(foundInt==0)
        {
            external_exe(args);
        }

    }
}

// // function to find and execute command provided by user in the background
// we do not wait for the child process to finish before returning to parent as we want the child process to run in the background of the parent process
int backgr_exe(char **args)
{
    // a char array of the possible internal commands a user can give
    char *internal_cmds[] = {
        "quit",
        "clr",
        "mkdir",
        "help",
        "dir",
        "echo",
        "environ",
        "cd"
    };
    

    // array of command's functions
    int (*commands[]) (char **) = {
        &quit,
        &clear,
        &makedir,
        &help,
        &dir,
        &echo,
        &enviro,
        &cd,
    };

    int foundInt=0;

    if(args[0] == NULL) // check if user has given a command
    {
        // user entered no command i.e. an empty command input
        return 1;
    }
    else
    {

        pid_t pid = fork(); // creates a child process

        for(int i=0; i<(sizeof(internal_cmds) / sizeof(char *)); i++) // loops through array of internal commands
        {
            if(!strcmp(args[0], internal_cmds[i])) // finds the command that matches the user's input
            {
                if(pid == -1)
                {
                    printf("Error: fork failed.");
                }
                else if(pid == 0)
                {
                    return (*commands[i])(args); // executes the given command
                }
            }
        }

    }
}
