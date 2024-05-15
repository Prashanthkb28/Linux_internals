/* Documentation----------------------------------------------------------------------
Name: PRASHANTH KB
Date: 20/04/2024
Description :   WAP to block certain signals from being received from command line.
Sample execution: 

 1. ./block_signal
      Process 2345 waiting for signal.. press ctrl + c from terminal. 
      SIGINT received  
Inside handler 
Inside handler 
. .
.....

2. Now open another terminal and send signal to process using kill command. 
Bash$ kill ­SIGUSR1 2345 
Bash$ kill ­SIGTERM 2345 
Bash$ kill ­SIGABRT 2345 

3. After exiting from handler will respond to blocked signal.
--------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

/* Signal Handler for SIGINT */
void sigint_handler (int sig)
{
	printf ("SIGINT received\n");
   i=10;
   while(i--)
	{
		printf ("Inside the Handler...\n");
		sleep (1);
	}
}

int main ()
{
	printf ("Process %d waiting for a signal...\n", getpid ());

	struct sigaction act;
	memset (&act, 0, sizeof (act));

	act.sa_handler = sigint_handler;		//Update the Signal Handler to 'signal_handler' from the default function.

	sigemptyset (&(act.sa_mask));			//To empty the Signal set.
	/* To include the below Signals to the Mask */
	sigaddset (&(act.sa_mask), SIGINT);		//To block the same Signal from disrupting the Handler execution.
	sigaddset (&(act.sa_mask), SIGUSR1);
	sigaddset (&(act.sa_mask), SIGUSR2);
	sigaddset (&(act.sa_mask), SIGABRT);

	sigaction (SIGINT, &act, NULL);			//To change the Action taken if SIGINT is detected.

	while (1);

	return 0;
}
