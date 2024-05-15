/* Documentation---------------------------------------------------------------------------
Name: PRASHANTH KB
Date: 31/03/2024
Description :   WAP to create child process to execute command passed through command line
Requirements :
Requirements:
Create child and execute a command passed from command-line arguments.  
If no arguments passed print a usage message.
In case any wrong command passed, print an error message.
After child terminates print the exit status of child process

sample Input:  ./a.out date
Sample Output:  
This is the CHILD process, with id 11612
Wed Apr  4 13:27:19 IST 2012
Child exited with status 0------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

int main (int argc, char* argv [])
{
    /* validating the command line arguments */
	if (argc == 1)											
	{
		printf ("Insufficient arguments.\nUsage: ./exe_child <command> [options]\n");
		return 0;
	}
	else													
	{
		pid_t pid = fork ();								//To create the Child Process.
		int status;											//To store the Child's Exit status.

		if (pid > 0)										//If the 'pid' is greater than 0; it is the Parent Process.
		{
			printf ("Child Process created with PID %d.\n", pid);

			wait (&status);									//To wait for the Child Process to terminate.
			printf ("Child terminated with Exit status %d.\n", status);
		}
		else if (pid == 0)									//If the 'pid' is 0; it is the Child Process.
		{
			int ret = execvp (argv [1], (argv + 1));		//To execute the Command passed through the CLA in the Child Process.
			if (ret == -1)									//Error Handling.
			{
				perror ("execvp");
			}
		}
		else												//Error Handling.
		{
			printf ("The Child Process is not created.\n");
		}
	}

	return 0;
}
