/*------------------------------------------
Communications between a parent process and
its 4 children processes via a pipe and signals
---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define CLILDREN_NB 4

int the_pipe [2];	/* global variable for the pipe */

/*-------------------------------------
            main function
--------------------------------------*/
int main (int argc, char *argv[]){
  int child_pid, child_iter ;
  void child_function (void);
  void parent_function (void);

  if( pipe (the_pipe) ) {
    perror("Erreur when opening the pipe");
    exit(2);
  }

  /*	creation of chlildren processes  */
  for ( child_iter =1; child_iter <=CLILDREN_NB  ; child_iter++ ){

    /*  Create children and execute child_function
     *  in each of them.
     */
     child_pid = fork();
     if(child_pid==0)
      child_function();
  }
  parent_function();
  return (0);
}

/*-------------------------------------
function executed by the parent process
--------------------------------------*/
void parent_function(void)
{
  int received_pid  ;
  int children_counter = 0  ;
  int child_pid ;	/* Returned by the wait function */
  int child_exit_state;	/* Mot dans lequel le fils renvoie son child_exit_state de fin */
  char received_msg [80];

  printf("Executing the parent process, nb %d\n", (int)getpid());

  /*	Waiting for messages from children   */

  while ( children_counter < CLILDREN_NB )  {

    /*   read characters one by one
     *   from the pipe and store them in
     *   received_msg; note that the end of the received
     *   message can be detected using the NULL character ('\0').
     */
    char received_char, char_iter=0;
    do
    {
      read (the_pipe[0], &received_char, 1);
      received_msg[char_iter] = received_char;
      char_iter++;
    }
    while(received_char!='\0');
    received_msg[char_iter] = received_char;
    printf("Executing pid %d, message received: %s\n", (int)getpid(), received_msg);

    sscanf (received_msg, "%*s%*s%*s%*s%d", &received_pid  );

    /* Send SIGUSR1 to the child using the PID that was just received
     * from the pipe (see line of code above).
     */
    kill(received_pid, SIGUSR1);

    /*	attente de la fin du fils    */
    child_pid = wait(&child_exit_state);
    children_counter++ ;
    printf("Received exit from child %d with child_exit_state: (hexa)%x\n", child_pid, child_exit_state);
  }
}
/*-------------------------------------
    function executed by children
--------------------------------------*/

void child_function(void)
{
  void  on_sigusr1(int);
  static char msg[] = "Hello, here is Pid      "; /* blank spaces are
                                                  * left to store the
                                                  * PID
                                                  */


  /* Make sure on_sigusr1 is executed when receiving SIGUSR1 */
  signal(SIGUSR1, on_sigusr1);

  printf("Executing process number %d\n", (int)getpid());
  sprintf(msg+19, "%d", (int)getpid());

  sleep (2);   /* to introduce offsets between processes */

  /* Write the text in the pipe and close it */

  /*   Execute WRITE only ONCE to send the message msg
   *   in the pipe. Use strlen(msg) to perform this. Warning :
   *   make sure the NULL character ('\0') is present at the end of the text in the pipe
   */

  write(the_pipe[1], msg, strlen(msg)+1);

  printf("Message sent to the parent process : %s\n", msg);
  close(the_pipe[1]);
  pause(); /* Pause the process, waiting for a signal from its
            * parent process.
            */
}


/*	Fonction de traitement du signal         */

void on_sigusr1 (int sig_nb)
{
  printf("Executing on_sigusr1 for process %d, received signal: %d\n",
	 (int)getpid(),
   sig_nb);
  exit (sig_nb);
}
