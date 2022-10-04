#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#include <sys/wait.h>
#include <fcntl.h>

#include "semaph.h"

/* Les deux verrous */
sem_t * lock_for_shared_variables;
sem_t * lock_for_readers_writers;
sem_t * lock_against_starvation;

/* System file descriptor containing le number 
 * of readers accessing the shared ressource */
FILE* number_of_readers_file;

/* Macro: name of the file containing the number of readers accessint the shared resource */
#define FILE_NB_READERS_NAME "readers.dat"

/* Prototypes of functions */
void readers_function(int reader_id);
void writers_function(int writer_id);
char is_first();
char is_last();


/********************** main ***********************/
int main(int argc, char *argv[]) {
  int  i, max_writers, max_readers, child_state;	
  int initial_sem_counter = 1; // initial value for semaphores counter  
  

  if(argc != 3){
    printf ("Wrong usage : argv[0] number-of-readers number-of-writers!\n");
    exit(1);
  }
  
  max_readers = atoi(argv[1]);
  max_writers = atoi(argv[2]);
  
  /* Creation of semaphores, which is equivalent 
   * to theoretical operations seen during lectures:
   * Init(sem, initial_counter_value)
   */
  lock_for_shared_variables = sem_open(LOCK_FOR_SHARED_VARIABLES_NAME, O_CREAT, 0644, initial_sem_counter);
  if (lock_for_shared_variables == SEM_FAILED) {
    perror("initialization of lock for shared variables failed");
    exit(1);
  }
  
  lock_for_readers_writers = sem_open(LOCK_FOR_READERS_WRITERS_NAME, O_CREAT, 0644, initial_sem_counter);
  if (lock_for_readers_writers == SEM_FAILED) {
    perror("initialization of lock for readers writers synchronization failed");
    exit(1);
  }
  lock_against_starvation = sem_open(LOCK_AGAINST_STARVATION_NAME, O_CREAT, 0644, initial_sem_counter);
  if (lock_against_starvation == SEM_FAILED) {
    perror("initialization of lock to avaoid starvation failed");
    exit(1);
  }	

  /* Open and initialise the file containing the number
   * of readers accessing the shared resource.
   */

  number_of_readers_file = fopen(FILE_NB_READERS_NAME, "w+");
  if (number_of_readers_file == NULL) {
    printf("ERROR: could not open the file to count the number of readers accessing the shared resource");
    return 1;       
  }
  fseek(number_of_readers_file, 0, SEEK_SET); // move to the beginning of the file
			       // before writing in it
  fprintf(number_of_readers_file, "%d", 0); // init the content of the file with 0
  fflush(number_of_readers_file); // force writing in the file before continuing
	
  /*** Creation of writer processes ***/
  for (i = 0; i < max_writers ; i++)  {
    if (fork() == 0) writers_function(i);
  }
	
  /*** Creation of reader processes ***/
  for (i = 0; i < max_readers ; i++)  {
    if (fork() == 0) readers_function(i);
  }	
	
  /* Main function waits for all readers and writers to finish their
     execution */
  while (wait(&child_state) != -1) 
    printf("main : end of child process, number %04x (state in hexa)\n", child_state);
  
  /* Destruction of semaphores */
  sem_close(lock_for_shared_variables);
  sem_unlink(LOCK_FOR_SHARED_VARIABLES_NAME);
  sem_close(lock_for_readers_writers);
  sem_unlink(LOCK_FOR_READERS_WRITERS_NAME);
  
  printf("main : %s : this is the end... \n", argv[0]);	
  exit(0);
}

/********************** writers function  ***********************/
/* Each writer executes a loop of three writings    */
void writers_function(int writer_id){
  int i=0;
  int seconds, minutes;
  struct tm  * the_date;
  time_t the_clock;
  
  /* initialisation of the clock */ 
  the_clock = time(NULL);
  the_date = localtime(&the_clock);
  minutes = the_date->tm_min;
  seconds = the_date->tm_sec; 
  
  printf(" ******************** Writer %d starts -- Date: %02d:%02d\n",  writer_id, minutes, seconds);
  
  /* Loop of three writings */
  for (i = 0; i < 3; i++){
    
    /*****  Begining of the critical section for the readers/writers mechanism *****/
    sem_wait(lock_against_starvation);
    sem_wait(lock_for_readers_writers);

        /* get the current date */
    the_clock=time(NULL);
    the_date=localtime(&the_clock);
    minutes= the_date->tm_min;
    seconds = the_date->tm_sec;

    /* display the current date */
    printf(" ******************** Writer %d enters the critical section -- Date :  %02d:%02d (iteration : %d)\n",  writer_id, minutes, seconds, i);


    /* Simulate time used by the writer while having access to the shared resource */
    sleep(3+writer_id%3);
    
    /* get the current date */
    the_clock = time(NULL);
    the_date = localtime(&the_clock);
    minutes = the_date->tm_min;
    seconds = the_date->tm_sec;
    /* display the current date */ 
    printf(" ******************** Writer %d about to exit the critical section -- Date :  %02d:%02d (iteration : %d)\n", writer_id, minutes, seconds,  i);
    
    
    /*****  End of the critical section for the readers/writers mechanism *****/
    sem_post(lock_against_starvation);
    sem_post(lock_for_readers_writers);
    
    sleep(2+i);
  }
  
  printf("Writer %d about to finish\n", writer_id);
  exit(writer_id);
}

/********************** readers function  ***********************/
void readers_function(int reader_id) {
  int seconds, minutes;
  time_t the_clock;
  struct tm  * the_date;

  sem_wait(lock_against_starvation);
  sem_post(lock_against_starvation);

  sem_wait(lock_for_shared_variables);
  if (is_first()) {
    printf("========================= Reader %d is the first one entering the critical section (i.e. accessing the shared resource) \n", reader_id);
    /*****  Begining of the critical section for the readers/writers mechanism *****/
    sem_wait(lock_for_readers_writers);
  }
  sem_post(lock_for_shared_variables);

  /* get the current date */
  the_clock = time(NULL);
  the_date = localtime(&the_clock);
  minutes = the_date->tm_min;
  seconds = the_date->tm_sec;

  /* display the current date */  
  printf("========================= Reader %d enters the critical section -- Date :  %02d:%02d \n", reader_id,  minutes, seconds);
  
  /* Simulation du temps passe en section critique */
  sleep(10+reader_id%4);
  
  /* get the current date */
  the_clock = time(NULL);
  the_date = localtime(&the_clock);
  minutes = the_date->tm_min;
  seconds = the_date->tm_sec;

  /* display the current date */
  printf("========================= Reader %d about to exit the critical section -- Date :  %02d:%02d \n", reader_id,  minutes, seconds);
    

  sem_wait(lock_for_shared_variables);
  if (is_last()) {
    printf("========================= Reader %d is last\n", reader_id);
    /*****  End of the critical section for the readers/writers mechanism *****/
    sem_post(lock_for_readers_writers);
  }
  sem_post(lock_for_shared_variables);
  exit(reader_id);
}

 
/********************** is_first function ***********************/
char is_first(){
  int number_of_readers;
  char result = 0;
  
  /* Read the file containing the number of readers accessing the shared resource */
  fseek(number_of_readers_file, 0, SEEK_SET);
  fscanf(number_of_readers_file, "%d", &number_of_readers);
  
  if (number_of_readers == 0 ) result = 1;
  number_of_readers++;
  
  /* Update the file containing the number of readers accessing the shared resource */
  fseek(number_of_readers_file, 0, SEEK_SET);
  fprintf(number_of_readers_file, "%2d", number_of_readers);
  printf("In is_first, number of readers : %2d\n", number_of_readers);
  fflush(number_of_readers_file);
  
  return result;	
}


/********************** is_last function ***********************/
char is_last(){
  int number_of_readers;
  int result = 0;
  
  /* Read the file containing the number of readers accessing the shared resource */
  fseek(number_of_readers_file, 0, SEEK_SET);
  fscanf(number_of_readers_file, "%d", &number_of_readers);
  
  number_of_readers = number_of_readers-1;
  if (number_of_readers == 0 ) result = 1;
  
  /* Update the file containing the number of readers accessing the shared resource */
  fseek(number_of_readers_file, 0, SEEK_SET);
  fprintf(number_of_readers_file, "%2d", number_of_readers);
  printf("In is_last, number of readers: %2d\n", number_of_readers);
  fflush(number_of_readers_file);
  
  return result;
}
