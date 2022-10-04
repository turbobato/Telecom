/**********************************************
In this program, a parent process creates a
child: parent and child processes communicate
via a pipe.
The parent process waits for characters from
the standard input (keyboard), and forward each
of them to the pipe.
The child process reads charactes from the pipe
and simply displays them.
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_MSG_LENGTH 100

int main (int argc, char *argv[]){
  int char_index, read_ret, fork_ret,
  the_pipe[2], child_exit_state;
  char received_char, should_continue;
  char received_msg[MAX_MSG_LENGTH];

  pipe(the_pipe);
  printf("Apres pipe : the_pipe[0] = %d, the_pipe[1] = %d\n", the_pipe[0], the_pipe[1]);

  /*****************************************
  *       CREATION OF A CHILD PROCESS
  *****************************************/
  fork_ret = fork();

  if (fork_ret  == -1)
  {
    printf("ERROR: fork just failed.\n");
    exit(1);
  }

  if (fork_ret != 0 )
  {
    /*****************************************
    *   branch executed by the parent process
    *****************************************/
    printf ("****************************\n");
    printf ("Parent process, num=%d\n", (int)getpid());
    printf ("****************************\n");

    printf("As a sender, first close the readable part of the pipe.\n");
    close(the_pipe[0]);

    printf("Write characters on the keyboard, they will be transferd to the pipe\n");
    printf("\tNote: CTRL+D closes the pipe.");
    while (1)
    {
      received_char = getchar();
      if (received_char == EOF)   /* <CTRL D>  = EOF sur stdin */
      {
        printf("Do you really want to close the pipe (y/n) :\n");
        scanf("%c", &should_continue);
        if (should_continue == 'y')
        {
          /* When the pipe is closed for writing by the producer,
           * new calls to the read function will return 0.
          */
          printf("Closing the writable part of the pipe.\n");
          close(the_pipe[1]);
          break;
        }
      }
      write(the_pipe[1], &received_char, sizeof(char));
    }

    /* When the child process observes that the writable part of the pipe was
     * closed, it will terminate its execution (calling exit).
     * Here, we wait for this termaination.
    */
    printf("Parent process: waiting for the termination of the child process\n");
    wait(&child_exit_state);
    printf("Parent process : after wait, child_exit_state = (hexa)%0x\n", child_exit_state);
  }
  else
  {
    /*****************************************
    *    branch executed by the child process
    *****************************************/
    const char left_margin[] = "\t\t\t\t";  /* constant margin to display
                                             * messages */

    printf(left_margin);
    printf("**************************\n");
    printf(left_margin);
    printf("Child process, num=%d\n", (int)getpid());
    printf(left_margin);
    printf("**************************\n");

    close(the_pipe[1]);
    char_index = 0;
    printf(left_margin);
    printf("Ready to read content from the pipe:\n");

    /* Characters are read one by one from the pipe, and displayed.
    When the pipe is closed by the producer, the value returned by a call
    to the read function is 0.
    */
    read_ret = 1;
    while ( read_ret != 0 )
    {
      read_ret = read(the_pipe[0], &received_char, sizeof(char));
      {
        printf(left_margin);
        printf("%d - received character %d: %c(%x)\n",
        read_ret, char_index, received_char, received_char);
        received_msg[char_index] = received_char;
        if (char_index < MAX_MSG_LENGTH) char_index = char_index+1;
        else char_index = 0;
      }
    }

    printf(left_margin);
    printf("The pipe was closed by the producer\n");

    received_msg[char_index] = '\0';

    printf(left_margin);
    printf("received message: %s\n", received_msg);

    /* end of the child process */
    exit (2);
  }

  return (0);
}
