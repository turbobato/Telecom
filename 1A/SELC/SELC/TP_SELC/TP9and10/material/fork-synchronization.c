#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main () {
  int fork_ret, wait_ret, state ;
  printf ("First process is executing, pid %d \n", (int)getpid());
  fork_ret=fork();
  switch (fork_ret)
    {
    case 0 :
      printf ("\t\t\t\t********\n\t\t\t\t* CHILD *	\n\t\t\t\t********\n");
      printf ("\t\t\t\tCHILD, PID: %d \n\t\t\t\tPARENT, PID: %d \n",
	      (int) getpid(),(int) getppid() );
      printf("\t\t\t\tGoing for a 30 secondes sleep...\n");
      sleep (30);
      printf
           ("\t\t\t\tJust wok up,\n\t\t\t\tI finish executing EXIT(7)\n");
      exit (7);
    case -1:
      printf ("Fork just failed !!!");
      exit(2);
    default:
      printf("********\n* PARENT *\n********\n");
      printf ("Parent process, pid: %d \nChild process, pid: %d \n",
	      (int) getpid(),fork_ret );
      printf ("Parent waiting for completion of the child process...\n");
      wait_ret = wait (&state);
      printf("Child finished its exectuion, pid: %d,\nIts state was : %04x\n",
        wait_ret,state);
    }
 return 0;
}
