/* Documentation----------------------------------------------------------------------
Name: PRASHANTH KB
Date: 15/04/2024
Description : Implement Communication between TWO related Processes using One Pipe
Sample execution: 

./pipe (No arguments)
    Error: No arguments passed Usage: ./pipe   <command1 > '|'   <command2>   

2. ./pipe ls  

    Error: Insufficient arguments passed Usage: ./pipe  <command1 > '|'   <command2>   

3. ./pipe ls '|' wc

    5 25 4. 

4. ./pipe ls ­ -l -­a '|' wc -­l ­-w 

    10 15
    ---------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char* argv [])
{
	if (argc == 1)							//if No command line arguments are passed
	{
		printf ("Error: No Command Line Argumenrs passed.\nUsage: ./pipe <command1> '|' <command2>\n");
		return 0;
	}
	else								
	{
		int pipe_pos = 1;

		/* To get the position of the Pipe '|' in the CLA */
		while ((pipe_pos < argc) && (strcmp (argv [pipe_pos], "|")) != 0)
		{
			pipe_pos += 1;
		}

		/* To validate the CLA. If the Pipe ("|") is missing or the Second Command is missing; display the Error message */
		if ((pipe_pos == argc) || (pipe_pos == (argc - 1)))
		{
			printf ("Error: Insufficient arguments passed.\nUsage: ./pipe <command1> '|' <command2>\n");
			return 0;
		}
		
		pid_t pid_c1, pid_c2;
		int pipefd [2];						//To store the Read and Write End of the Pipe.

		pipe (pipefd);						//To create the Pipe.

		pid_c1 = fork ();					//To create the First Child Process.
		if (pid_c1 > 0)						//If the 'pid_c1' is greater than 0; it is the Parent Process.
		{
			pid_c2 = fork ();				//To create the Second Child Process.
			if (pid_c2 > 0)					//If the 'pid_c2' is greater than 0; it is the Parent Process.
			{
				/* There is no usage of Pipe in the Parent */
				close (pipefd [0]);			//Close the Read End of the Pipe.
				close (pipefd [1]);			//Close the Write End of the Pipe.

				/* The Parent Process shall wait for it's 2 Child Process to complete the execution */
				wait (NULL);
				wait (NULL);
			}
			else if (pid_c2 == 0)			//If the 'pid_c2' is 0; it is the Second Child Process and will execute the Second Command.
			{
				/* The Second Command shall Read the Input from the Pipe and not from the Standard Input */
				dup2 (pipefd [0], 0);		//To Replace the Standard Input with the Read End of the Pipe.
				close (pipefd [1]);			//Close the Write End of the Pipe.

				/* To Execute the Second Command: File Name shall be Argument after "|" and Arguments shall be from after "|" to the End */
				execvp (argv [pipe_pos + 1], (argv + pipe_pos + 1));

				close (pipefd [0]);			//Close the Read End of the Pipe.
			}
			else							//Error Handling.
			{
				printf ("The Child Process is not created.\n");
			}
		}
		else if (pid_c1 == 0)			       	//If the 'pid_c1' is 0; it is the First Child Process and will execute the First Command.
		{
			/* The First Command shall Write the Output on the Pipe and not on the Standard Output */
			dup2 (pipefd [1], 1);		        	//To Replace the Standard Output with the Write End of the Pipe.
			close (pipefd [0]);			        	//Close the Read End of the Pipe.
			argv [pipe_pos] = NULL;		         	//To terminate the First Command in the CLA by replacing the "|" with NULL.

			/* To Execute the First Command */
			execvp (argv [1], (argv + 1));

			close (pipefd [1]);				//Close the Write End of the Pipe.
		}
		else								//Error Handling.
		{
			printf ("The Child Process is not created.\n");
		}
	}

	return 0;
}
