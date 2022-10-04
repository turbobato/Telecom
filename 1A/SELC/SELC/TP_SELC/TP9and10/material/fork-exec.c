#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
  int fork_ret,wait_ret,state;


  if (argc != 2){
    printf(" Wrong usage, expected: %s executable_file_path\n", argv[0]);
    exit(1);
  }

  printf (" Executing process fork_ret: %d, about to fork.\n",(int)getpid());
  fork_ret=fork();
  switch (fork_ret){
    case  0 :
      printf (" Hi ! I am Lucas, the new born child! My PID is:%d\n", (int)getpid());
      printf (" I (pid: %d) just decided to replace my program by %s\n", (int)getpid(), argv[1]);
      execl(argv[1], argv[1], NULL);
      printf (" %d : Ooops, I did not manage to change my program (exit 2!) \n", (int)getpid());
      exit (2);
    case   -1 :
      printf ("Fork just failed! arghhhh... ");
      exit (3);
    default :
      /* Parent waiting for a Child... */
      printf (" Parent (pid: %d) is waiting\n ",(int) getpid());
      wait_ret=wait(&state);
      printf ( " Child finished, its pid was: %d ", wait_ret);
      printf (" ... its state was :%04x (hexa) \n",state);
      exit(0);
    }
  return 0;
}
