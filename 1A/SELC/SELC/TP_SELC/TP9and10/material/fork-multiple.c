#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
  pid_t fork_ret1, fork_ret2 ;
  printf (" print 1 - Executin the first process, pid: %d \n",
	  (int)getpid() );
  fork_ret1 = fork();
  printf (" print 2 - fork returned: %d - executing proces number= %d - parent pid:%d \n",
	  (int)fork_ret1, (int)getpid(), (int)getppid() );
  fork_ret2 = fork();
  printf (" print 3 - fork returned: %d - executing proces number= %d - parent pid:%d \n",
	  (int)fork_ret2, (int)getpid(), (int)getppid() );
  return 0;
}
