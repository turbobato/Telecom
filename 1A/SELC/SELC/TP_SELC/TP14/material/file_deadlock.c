
/******************************************************
On UNIX, usage of locks when accessing a file verrou
is not mandatory (advisory locking). To make it
mandatory (mandatory locking), it is necessary
(if you have the wright to do so) to modify the access
wrights on the file (cf. man lockf et man chmod).
Depending on the operating systems, commands that follow
should do the job:
chmod 2644
chmod +l
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char *argv[]){
  int i=0, sentence_size, sleep_index_in_sentence,
  deadlock_nb=0,  /* counter for the number of detected deadlocks */
  correct_iteration_nb =0,  /* counter for the number of correct iteration
                             * of the critical section */
  lockf_ret =0, /* value returned by call to lockf */
  file_1, file_2; /* shared resources */
  char sentence[256];

  if (argc != 3){
    printf(" Wrong usage : %s file_1_name file_2_name ! \n", argv[0]);
    exit(2);
  }

  sprintf(sentence, " -- Writen by pid %d -- \n", (int)getpid() );
  sentence_size=strlen(sentence);

  /* Open the two files */

  if ((file_1 = open (argv[1], O_RDWR)) == -1) {
    perror ("open file_1");
    exit (1);
  }
  if ((file_2 = open (argv[2], O_RDWR)) == -1) {
    perror ("open file_2");
    exit (1);
  }

  while(1){
    /* TO BE COMPLETED: take the first lock */

    if (lockf_ret != 0)  perror ("lockf file_1");
    printf ("Pid %d : entered critical section 1 (%s), lockf_ret=%d\n", (int)getpid(), argv[1], lockf_ret );

    // simulate execution
    sleep(random() %4);

    /* TO BE COMPLETED: take the second lock */

    /* TO BE COMPLETED: If a deadlock is detected, release the first lock
       execute sleep, increment deadlock_nb, and try to
       take it again by restarting the loop */

    correct_iteration_nb++;
    printf ("Pid %d : entered critical section 2 (%s), lockf_ret=%d, correct_iteration_nb =%d\n",
    (int)getpid(), argv[2], lockf_ret, correct_iteration_nb);

    /*----------------------------------------------
    Begining of the critical section
    ----------------------------------------------*/

    sleep_index_in_sentence=random()%sentence_size;

    for (i=0; i <sentence_size ;i++){
      if (i == sleep_index_in_sentence)sleep(random()%4);
      if (write (file_1, sentence+i, sizeof (char)) < 0){
        printf ("ERROR: writing to %s failed\n", argv[1]);
        break;
      }
    }

    for (i=0; i <sentence_size;i++){
      if (i == sleep_index_in_sentence)sleep(random()%4);
      if (write (file_2, sentence+i, sizeof (char) ) < 0){
        printf ("ERROR: writing to %s failed\n", argv[2]);
        break;
      }
    }

    /*----------------------------------------------
    End of the critical section
    ----------------------------------------------*/

    lseek(file_2, 0, SEEK_SET);
    /* TO BE COMPLETED: release the second lock */
    if (lockf_ret != 0)  perror ("ulockf file_2");
    printf ("Pid %d : exit critical section 2 (%s), lockf_ret=%d\n", (int)getpid(), argv[2], lockf_ret );

    lseek(file_1, 0, SEEK_SET);
    /* TO BE COMPLETED: release the first lock */
    if (lockf_ret != 0)  perror ("ulockf file_1");
    printf ("Pid %d : exit critical section 1 (%s), lockf_ret=%d\n", (int)getpid(), argv[1], lockf_ret );

  }  /* end of for */

  return 0;

}
