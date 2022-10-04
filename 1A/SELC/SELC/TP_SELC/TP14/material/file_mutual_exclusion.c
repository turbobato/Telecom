/******************************************************

In this file, we implement a simple mutual exclusion
between processes accessing a shared file.

********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int open_ret;

int main (int argc, char *argv[])
{
  int lock_ret, write_ret ;
  char taped_word[256];
  if (argc != 2)
  {
    printf("Wrong usage : %s file_name ! \n", argv[0]);
    exit(2);
  }
  /*-----------------------------------------------------
  To check that the program works correctly, display
  outputs on shell windows for which we first obtained
  the name using the tty command. Pass the file name
  as an argument to the program.
  ----------------------------------------------------- */

  if ((open_ret = open (argv[1], O_RDWR)) == -1)
  {
    perror ("open 2");
    exit (1);
  }

  while (1)
  {
    /*  TO BE COMPLETED: implement a mutual exclusion  */

    printf ("Pid %d : entered the critical section\n", (int)getpid() );


    /*----------------------------------------------
    Begining of the critical section
    ----------------------------------------------*/
    while ( fgets(taped_word, sizeof(taped_word), stdin) )
    {
      /*----------------------------------------------
      The program stays in the read/write loop until
      the user tape "end".
      ----------------------------------------------*/
      if (!strncmp (taped_word, "end", 3)) break;
      write_ret = write (open_ret, taped_word, strlen(taped_word));
      if (write_ret < 0)
      {
        printf ("Error when writing in %s\n", argv[1]);
        break;
      }
    }

    /*----------------------------------------------
    TO BE COMPLETED: end of the critical section
    ----------------------------------------------*/
    printf ("Pid %d : arret temporaire d'utilisation de %s\n",
    (int)getpid(), argv[1]);

    /*  TO BE COMPLETED to avoid starvation  */

    printf ("Finished critical section, lock returned =  %d\n", lock_ret);
  }  /* fin de la boucle while */
}
