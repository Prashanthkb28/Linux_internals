/* Documentation----------------------------------------------------------------------------
Name: PRASHANTH KB
Date: 10/04/2024
Description :WAP to make zombie process become orphan, print status of each state
sample execution :
 ./zomb_orph
A child created with pid 1234
Name:  ./zomb_orph
State: S (sleeping)
Name:  /zomb_orph   (After some time)
State: Z (zombie)
Process 1234 cleared by init  (After some time)
 ---------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void print_status (pid_t pid, int option)
{
	int fd, i = 0, j = 0;
	char path [20] = {'\0'}, str [3][30] = {'\0'}, ch;

	sprintf (path, "/proc/%d/status", pid);					//To Open the File containing the Status of the Child Process.
	fd = open (path, O_RDONLY);

	if (fd != -1)											//If the File is Opened successfully, read the Name and Status of the Child Process and store them as a String.
	{
		do
		{
			read (fd, &ch, 1);								//To read one Character from the Status File and store in the Field Array.
			str [j][i++] = ch;

			if (ch == '\n')									//If a New-line is encountered, one Field is read and the value of 'i' and 'j' shall be updated.
			{
				i = 0;
				j += 1;
			}
		} while (j != 3);									//The loop shall run till the Top 3 Fields are read.

		if (option == 1)									//To print the Name and Status of the Child Process in Sleeping State ignoring the Umask.
			printf ("%s%s", str [0], str [2]);
		else if (option == 2)								//To print the Name and Status of the Child Process in Zombie State.
			printf ("%s%s", str [0], str [1]);

		close (fd);											//To Close the File.
	}
	else													//Error Handling.
	{
		printf ("The file doesn't exists. The Process has been Cleared.\n");
	}
}

int main ()
{
	pid_t pid_c1, pid_c2;

	pid_c1 = fork ();										//To Create the First Child Process.

	if (pid_c1 > 0)											//If the 'pid_c1' Value is greater than 0; it is a Parent Process.
	{
		printf ("A Child is created with PID %d.\n", pid_c1);

		/* To print the Status of the Sleeping (First) Child Process */
		sleep (2);
		print_status (pid_c1, 1);

		/* To print the Status of the Zombie (First) Child Process */
		sleep (5);
		print_status (pid_c1, 2);

		pid_c2 = fork ();									//To Create the Second Child Process.

		if (pid_c2 > 0)										//If the 'pid_c2' Value is greater than 0; it is a Parent Process.
		{
			//printf ("A Child is created with PID %d.\n", pid_c2);
		}
		else if (pid_c2 == 0)								//If the 'pid_c2' Value is 0; it is a Child Process.
		{
			/* To print the Clearing of the Zombie (First) Child Process by Init */
			sleep (5);
			printf ("Process %d cleared by init.\n", pid_c1);
		}
		else												//Error Handling.
		{
			printf ("Child Process is not created.\n");
		}
	}
	else if (pid_c1 == 0)									//If the 'pid_c1' Value is 0; it is a Child Process.
	{
		sleep (5);
	}
	else													//Error Handling.
	{
		printf ("Child Process is not created.\n");
	}

	return 0;
}
