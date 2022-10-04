#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void child_function(int child_number);
void father_function();

int  main (int argc, char *argv[]) {
  int nb_of_processes, i_child, pid_child;

  /* Check that one parameter was provided */
  if (argc != 2) {
    printf(" Wrong usage, expected : %s number_of_processes ! \n", argv[0]);
    exit(2);
  }

  nb_of_processes = atoi (argv [1]);  /* conversion ascii -> integer */

  /* ----------------------------------------
  *  creation of children processes
  * ----------------------------------------
  */
  for ( i_child =1; i_child <= nb_of_processes  ; i_child++ ) {
    pid_child = fork();
    switch (pid_child) {
      case 0 :
      child_function(i_child); /* You have to write this function ... */
      break;
      case -1 :
      perror("Fork just failed !!!");
      exit(33);  /* exit in case an error occured */
    }
  }

  father_function();      /* You have to write this function ... */

  return 0;

}

void child_function(int child_number)
{
  printf("This is my PID : %d \n This is the PID of my father : %d\n", getpid(),getppid());
  printf("Now going to sleep\n");
  sleep(2*child_number);
  printf("%d just woke up !\n", getpid());
  if (child_number == 2){
    int x = 2/0;
  }
  exit(child_number);
}

void father_function()
{
  int wait_ret = 0;
  int state;
  while (wait_ret != -1){
    wait_ret = wait(&state);
    if (wait_ret != -1){
      printf("The child process of PID : %d just finished with state : %04x\n", wait_ret, state);
    }
  }
}
