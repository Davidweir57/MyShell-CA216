// Name: David Weir
// Student Number: 19433086
// I acknowledge all terms of DCU's Academic Integrity Policy

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

// exits shell program
int quit()
{
    exit(0);
}

// clears shell terminal
int clear()
{
    char *clear[2];
    clear[0] = "clear", clear[1] = NULL;

    pid_t pid = fork();

    if(pid == -1)
    {
        printf("Error fork failed");
    }

    else if (pid == 0)
    {
        execvp("clear", clear);
    }
    wait(NULL);
}

// function takes a dirname from the user and creates a new directory with that name
int makedir(char *args[])
{

    // if no dirname given print an error message
    if(args[1] == NULL)
    {
        printf("Error: No directory name given.");
    }
    // else create a directory in the cwd with the given name
    else
    {
        // use inbuilt mkdir function to create a new directory. Command defaults to the creating the directory in the cwd.
        // result is stored in check. if successful it returns 0 else it returns -1
        int check = mkdir(args[1], 0777);

        // checking if the directory has been created
        if(check == 0)
        {
            printf("Directory created successfully!");
        }
        else
        {
            printf("Error in creating directory.");
            exit(1);
        }

    }
}

// lists contents of current directory
// TO ADD: list contents of given dir
int dir()
{
    DIR *dir;
    struct dirent *files;
    struct stat mystats;

    dir = opendir(".");

    if (dir == NULL)
    {
        printf("Directory cannot be opened!" );
    }
    else
    {
        while ((files = readdir(dir)))
        {
            stat(files->d_name, &mystats);
            printf("%d ", mystats.st_size);
            printf("%s\n", files->d_name);
        }
    }

    closedir(dir);
}

// print environment variables
int enviro(char *envp[])
{
    extern char **environ;
    // loop through array of environments variables, printing them until end
    for(int i=0; envp[i] != NULL; i++)
    {
        printf("\n%s", envp[i]);
    }

    getchar();
}

// pauses the program until the user presses enter
void pauseShell()
{
    printf("Press Enter to unpause the shell...");
    getchar(); // wait for user to input enter
}

int cd(char *args[])
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd)); // gets cwd

    // if no directory provided print the cwd
    if(args[1] == NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        chdir(args[1]); // change the directory to the user given target
        getcwd(cwd, sizeof(cwd)); // get the new cwd
        // setenv("PWD", cwd, 1);
        putenv("PWD=cwd"); // set the PWD environment varable to the new cwd
    }
}

// prints all arguments after the command echo e.g. echo hello world outputs "hello world"
int echo(char *args[])
{

    // loops through the array of the parsed input line starting at index 1 and outputs them to the terminal.
    for(int i=1; args[i]; i++)
    {
        printf("%s ", args[i]);
    }
}

int help()
{
    FILE *file;
    char c[1000];
    int numlines=0;

    file = fopen("readme.md", "r"); // opens the manual under readme.md

    // error handling if file cant be found or reached
    if(file == NULL)
    {
        printf("Error: cannot opening file.");
    }

    // while the file is not at the end get take a line from the file
    while(fgets(c, sizeof(c), file) != NULL)
    {
        // if statement implements more filter
        // we output lines to stdout until we have read 20 lines after this we reset our count of lines to 0 and wait for the user to input Enter to continue the loop and read more of the file
        if(numlines < 19)
        {
            fputs(c, stdout);
            numlines++;
        }
        else
        {
            fputs(c, stdout); // outputs the line for count of 21 which is skipped otherwise
            numlines = 0; // reset our count
            getchar(); // wait for the user
        }
    }

    fclose(file); // closes readme file

}