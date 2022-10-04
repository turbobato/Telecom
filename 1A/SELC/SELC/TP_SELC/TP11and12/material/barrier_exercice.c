#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#include <sys/wait.h>
#include <fcntl.h>


#include "barrier_semaph.h"

sem_t * lock_for_shared_variables;
sem_t * sem_for_barrier;

/* Name of the file containing the number of actor that reached the barrier */
#define ACTORS_REACHED_FILE_NAME "actors_reached.dat"
/* File descriptor for the file containing the number of actor
that reached the barrier */
FILE * actors_reached_file;

/* Functions prototype */
void actor_function(int actor_id);

/* global variables*/
int actors_number; // number of actors
int actors_reached; // number of actors that reached the barrier

/********************** main ***********************/
int main(int argc, char *argv[]) {
  int  i, etat;

  if(argc != 2){
    printf (" Wrong Usage: %s nbre-of-actors !\n", argv[0]);
    exit(1);
  }

  actors_number = atoi(argv[1]);

  /* Create and initialize semaphores */
  
  /*
   *  TO BE COMPLETED using identifiers of semaphores defined in
   *  barrier_semaph.h
   */
  lock_for_shared_variables = sem_open(LOCK_FOR_SHARED_VARIABLES_NAME, O_CREAT, 0644, 1);
  if (lock_for_shared_variables == SEM_FAILED) {
    perror("initialization of lock for shared variables failed");
    exit(1);
  }
  sem_for_barrier = sem_open(SEM_FOR_BARRIER_NAME, O_CREAT, 0644, 0);
  if (sem_for_barrier == SEM_FAILED) {
    perror("initialization of semaphore for barriers failed");
    exit(1);
  }
  actors_reached_file = fopen(ACTORS_REACHED_FILE_NAME, "w+");
  if (actors_reached_file == NULL) {
    printf("ERROR: could not open file containing the number of actors that reached the barrier\n");
    return 1;
  }
  fseek(actors_reached_file, 0, SEEK_SET);
  fprintf(actors_reached_file, "%d", 0); // initialize to 0 the number of actors that reached the barrier
  fflush(actors_reached_file);

  /*** Creation of actors ***/
  for (i = 0; i < actors_number ; i++)  {
    if (fork() == 0) actor_function(i);
  }

  /* Wait for the end of all the actors */
  while (wait(&etat) != -1)
    printf("main - end of child process with state: %04x (hexa)\n", etat);

  /* Destruction of semaphores */

  /* TO BE COMPLETED  */
  sem_close(lock_for_shared_variables);
  sem_unlink(LOCK_FOR_SHARED_VARIABLES_NAME);
  sem_close(sem_for_barrier);
  sem_unlink(SEM_FOR_BARRIER_NAME);

  printf("main - %s : this is the end ... \n", argv[0]);
  exit(0);
}

/********************** acteur  ***********************/
void actor_function(int actor_id){
  int ret_scan;
  int seconds, minutes;
  struct tm  * the_date;
  time_t the_clock;

  /* get the current date */
  the_clock = time(NULL);
  the_date = localtime(&the_clock);
  minutes = the_date->tm_min;
  seconds = the_date->tm_sec;

  /* display the current date */
  printf(" ******************** Actor %d starts at  %02d:%02d\n",  actor_id, minutes, seconds);

  /* Simulate time before reaching the barrier */
  sleep((actor_id)%3 +1);

  /*  TO BE COMPLETED */

  printf("========================= Actor %d in SC1\n", actor_id);
  sem_wait(lock_for_shared_variables);
  /* Increment the number of actors that reached the barrier */
  fseek(actors_reached_file, 0, SEEK_SET);
  ret_scan = fscanf(actors_reached_file, "%d", &actors_reached);
  actors_reached++;
  fseek(actors_reached_file, 0, SEEK_SET);
  fprintf(actors_reached_file, "%2d", actors_reached);
  fflush(actors_reached_file);
  sem_post(lock_for_shared_variables);

  if  (actors_reached == actors_number ) {
    printf("\n========================= ALL THE ACTORS ARRIVED \n\n");
    sleep(5);

    /* Decrement the number of actors that reached the barrier */
    fseek(actors_reached_file, 0, SEEK_SET);
    ret_scan = fscanf(actors_reached_file, "%d", &actors_reached);
    actors_reached = actors_reached-1;
    fseek(actors_reached_file, 0, SEEK_SET);
    fprintf(actors_reached_file, "%2d", actors_reached);
    fflush(actors_reached_file);

    while(actors_reached > 0){
      printf("========================= Actor %d is about to release another actor (one of %d blocked)\n", actor_id, actors_reached);


      /*  TO BE COMPLETED */
      sem_post(sem_for_barrier);

      /* Decrement the number of actors at the barrier */
      fseek(actors_reached_file, 0, SEEK_SET);
      ret_scan = fscanf(actors_reached_file, "%d", &actors_reached);
      actors_reached = actors_reached-1;
      fseek(actors_reached_file, 0, SEEK_SET);
      fprintf(actors_reached_file, "%2d", actors_reached);
      fflush(actors_reached_file);
    }
    printf("========================= Actor %d is about to exit SC1\n", actor_id);


    /*  TO BE COMPLETED */
    
    ;
  }
  else {
    printf("========================= Actor %d is about to exit SC1\n", actor_id);



    /*  TO BE COMPLETED */

    printf("************ Actor %d waits at the barrier\n", actor_id);
    sem_wait(sem_for_barrier);
    /*  TO BE COMPLETED */

    the_clock = time(NULL);
    the_date = localtime(&the_clock);
    minutes = the_date->tm_min;
    seconds = the_date->tm_sec;

    printf("************ Actor %d passed the barrier !   %02d:%02d\n",  actor_id, minutes, seconds);
    sleep((actor_id)%4 +1);
  }

  the_clock = time(NULL);
  the_date = localtime(&the_clock);
  minutes = the_date->tm_min;
  seconds = the_date->tm_sec;
  printf(" ******************** Actor %d  : end  %02d:%02d\n",  actor_id, minutes, seconds);

  exit(actor_id);

}
