/* Documentation----------------------------------------------------------------------
Name: PRASHANTH KB
Date: 18/04/2024
Description :  Implement communication between three related processes using two pipe
Sample execution:

 ./three_pipes (No arguments)
 Error: No arguments passed Usage: ./three_pipes  <command1 > '|'   <command2> '|'  <command3 >       

2. ./pipe ls ­l '|' grep 

 Error: Insufficient arguments passed Usage: ./pipe  <command1 > '|'   <command2> '|'  <command3 >

3. ./pipe ls -­l '|' grep “pattern” '|' wc -­l 

5 5 25
--------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char* argv [])
{
	if (argc == 1)								//If no Commands line arguments are passed
	{
		printf ("Error: No Command Line Argumenrs passed.\nUsage: ./pipe <command1> '|' <command2> '|' <command3>\n");
		return 0;
	}
	else										
	{
		int pipe_pos [2] = {1, 0};				//To store the Pipe positions in the CLA.

		/* To get the First position of the Pipe '|' in the CLA */
		while ((pipe_pos [0] < argc) && (strcmp (argv [pipe_pos [0]], "|")) != 0)
		{
			pipe_pos [0] += 1;
		}

		/* To validate the CLA. If the 1st Pipe ("|") is missing or the Second Command is missing; display the Error message */
		if ((pipe_pos [0] == argc - 1) || (pipe_pos [0] == argc))
		{
			printf ("Error: Insufficient arguments passed.\nUsage: ./pipe <command1> '|' <command2> '|' <command3>\n");
			return 0;
		}

		/* To get the Second position of the Pipe '|' in the CLA */
		pipe_pos [1] = pipe_pos [0] + 1;		//To start the Search of 2nd "|" in CLA from the position after the 1st "|" is encountered.
		while ((pipe_pos [1] < argc) && (strcmp (argv [pipe_pos [1]], "|")) != 0)
		{
			pipe_pos [1] += 1;
		}

		/* To validate the CLA. If the 2nd Pipe ("|") is missing or the Third Command is missing; display the Error message */
		if ((pipe_pos [1] == argc - 1) || (pipe_pos [1] == argc))
		{
			printf ("Error: Insufficient arguments passed.\nUsage: ./pipe <command1> '|' <command2> '|' <command3>\n");
			return 0;
		}

		pid_t pid_c1, pid_c2, pid_c3;
		int pfd1 [2], pfd2 [2];

		pipe (pfd1);							//To create the Pipe-1.

		pid_c1 = fork ();						//To create the First Child Process.
		if (pid_c1 > 0)							//If the 'pid_c1' is greater than 0; it is the Parent Process.
		{
			pipe (pfd2);						//To create the Pipe-2 before Second Child creation as Second Child shall write on the Pipe-2.

			pid_c2 = fork ();					//To create the Second Child Process.
			if (pid_c2 > 0)						//If the 'pid_c2' is greater than 0; it is the Parent Process.
			{
				pid_c3 = fork ();
				if (pid_c3 > 0)
				{
					/* There is no usage of Pipe in the Parent */
					close (pfd1 [0]);			//Close the Read End of the Pipe-1.
					close (pfd1 [1]);			//Close the Write End of the Pipe-1.
					close (pfd2 [0]);			//Close the Read End of the Pipe-2.
					close (pfd2 [1]);			//Close the Write End of the Pipe-2.

					/* The Parent Process shall wait for it's 3 Child Process to complete the execution */
					wait (NULL);
					wait (NULL);
					wait (NULL);
				}
				else if (pid_c3 == 0)			//If the 'pid_c3' is 0; it is the Third Child Process and will execute the Third Command.
				{
					/* The Third Command shall Read the Input from the Pipe-2 and not from the Standard Input */
					close (pfd1 [0]);			//Close the Read End of the Pipe-1.
					close (pfd1 [1]);			//Close the Write End of the Pipe-1.
					close (pfd2 [1]);			//Close the Write End of the Pipe-2.
					dup2 (pfd2 [0], 0);			//To Replace the Standard Input with the Read End of the Pipe-2.

					/* To Execute the Third Command: File Name shall be Argument after 2nd "|" and Arguments shall be from after 2nd "|" to the End */
					execvp (argv [(pipe_pos [1]) + 1], (argv + pipe_pos [1] + 1));
					
					close (pfd2 [0]);			//Close the Read End of the Pipe-2.
				}
				else							//Error Handling.
				{
					printf ("The Child Process is not created.\n");
				}
			}
			else if (pid_c2 == 0)				//If the 'pid_c2' is 0; it is the Second Child Process and will execute the Second Command.
			{
				/* The Second Command shall Read the Input from the Pipe-1 and Write the Output on the Pipe-2 */
				close (pfd1 [1]);				//Close the Write End of the Pipe-1.
				close (pfd2 [0]);				//Close the Read End of the Pipe-2.
				dup2 (pfd1 [0], 0);				//To Replace the Standard Input with the Read End of the Pipe-1.
				dup2 (pfd2 [1], 1);				//To Replace the Standard Output with the Write End of the Pipe-2.
				argv [pipe_pos [1]] = NULL;		//To terminate the Second Command in the CLA by replacing the 2nd "|" with NULL.

				/* To Execute the Second Command: File Name shall be Argument after 1st "|" and Arguments shall be from after 1st "|" till before 2nd "|" */
				execvp (argv [(pipe_pos [0]) + 1], (argv + pipe_pos [0] + 1));

				close (pfd1 [0]);				//Close the Read End of the Pipe-1.
				close (pfd2 [1]);				//Close the Write End of the Pipe-2.
			}
			else								//Error Handling.
			{
				printf ("The Child Process is not created.\n");
			}
		}
		else if (pid_c1 == 0)					//If the 'pid_c1' is 0; it is the First Child Process and will execute the First Command.
		{
			/* The First Command shall Write the Output on the Pipe-1 and not on the Standard Output */
			close (pfd1 [0]);					//Close the Read End of the Pipe.
			close (pfd2 [0]);					//Close the Read End of the Pipe-2.
			close (pfd2 [1]);					//Close the Write End of the Pipe-2.
			dup2 (pfd1 [1], 1);					//To Replace the Standard Output with the Write End of the Pipe-1.
			argv [pipe_pos [0]] = NULL;			//To terminate the First Command in the CLA by replacing the 1st "|" with NULL.

			/* To Execute the First Command: File Name shall be Argument after the Executable Name and Arguments shall start from after Executable Name to the 'pipe_pos' */
			execvp (argv [1], (argv + 1));

			close (pfd1 [1]);					//Close the Write End of the Pipe.
		}
		else									//Error Handling.
		{
			printf ("The Child Process is not created.\n");
		}
	}

	return 0;
}
