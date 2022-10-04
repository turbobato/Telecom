#include <stdio.h>
#include <semaphore.h>

#include "semaph.h"

/* Fermeture de semaphores deja existants */

int main(int argc, char *argv[]){
  sem_t * mutex;
  printf("cleanup : starting ... \n");

  mutex = sem_open(LOCK_FOR_SHARED_VARIABLES_NAME,0,0644,0);
  if(mutex == SEM_FAILED){
    perror("pb open LOCK_FOR_SHARED_VARIABLES_NAME");
  }
  sem_close(mutex);
  sem_unlink(LOCK_FOR_SHARED_VARIABLES_NAME);
  
  mutex = sem_open(LOCK_FOR_READERS_WRITERS_NAME,0,0644,0);
  if(mutex == SEM_FAILED){
    perror("pb open LOCK_FOR_READERS_WRITER_NAME");
  }
  sem_close(mutex);
  sem_unlink(LOCK_FOR_READERS_WRITERS_NAME);
  
  mutex = sem_open(LOCK_AGAINST_STARVATION_NAME,0,0644,0);
  if(mutex == SEM_FAILED){
    perror("pb open LOCK_AGAINST_STARVATION_NAME");
  }
  sem_close(mutex);
  sem_unlink(LOCK_AGAINST_STARVATION_NAME);
  
  printf("cleanup : this is the end... \n");
  return 0;
}
