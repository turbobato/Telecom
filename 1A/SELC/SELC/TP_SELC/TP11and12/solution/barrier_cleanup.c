#include <stdio.h>
#include <semaphore.h>

#include "barrier_semaph.h"

/* This program cleans up semaphores used in the barrier exercice. */

int main(int argc, char *argv[]){
  sem_t *semaphore;
  
  
  /* access and destroy semaphores */
  if ((semaphore = sem_open(LOCK_FOR_SHARED_VARIABLES_NAME, 0, 0644, 0)) == SEM_FAILED) {
    perror("open lock for shared variables failed");
  }
  sem_close(semaphore);
  sem_unlink(LOCK_FOR_SHARED_VARIABLES_NAME);
  
  
  if ((semaphore = sem_open(SEM_FOR_BARRIER_NAME, 0, 0644, 0)) == SEM_FAILED) {
    perror("open lock for barrier failed");
  }
  
  sem_close(semaphore);
  sem_unlink(SEM_FOR_BARRIER_NAME);
  
  
  printf("cleanup: this is the end ... \n");
  return 0;
}
