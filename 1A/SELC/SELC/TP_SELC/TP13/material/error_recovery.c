/*************************************************
Program for recovering from memory access errors:
specific handlers for signals SIGSEGV and SIGBUS
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

/* Declare an array and an iterator on this array */
long array_iterator, array[100];

/* Functions declaration */
void on_memory_access_error(int Numero);

/* Declare the data structure used to save/restore an
 * execution context and re-execute a program from a given point
 * (using sigsetjmp/siglongjmp) */
jmp_buf ctxt;

/**************************************/
/* main function                      */
/**************************************/
int main(int argc, char *argv[]) {
	long sig_set_jmp_ret, scanf_ret; /* variable to store values returned
                                    * by system calls: TO BE COMPLETED
                                    * if necessary */

	for (array_iterator=0 ; array_iterator<100; array_iterator++)
    array[array_iterator]= array_iterator;

	/* Display adresses range of the array */
	printf ("array = (hexa)%p, adresse array[0] = %p, adresse array[99] = %p\n",array , &array[0], &array[99]);

	/*  TO BE COMPLETED: associate a signal handler to SIGSEGV and SIGBUS
   *  in order to recover from memory access errors: this is the
   *  recovery point */

	/* TO BE COMPLETED: set here a recovery point to re-execute the program
  * from here in case of error */

	printf ("sigsetjmp, returned value: %ld\n", sig_set_jmp_ret);

  /* TO BE COMPLETED: if next instructions are executed after
   * recovering from an erroneous memory access, display a
   * warning message: "an error just occured, make sure you respect the
   * usage condition of this program" */

  /* Ask a user to give an index for reading the content of the
   * array: large enough values, or negative values should
   * produce an error */
	while (1) {
		printf ("Provide an index value to retreive a value from the array: \n");
		scanf_ret = scanf ("%ld", &array_iterator);
		if (scanf_ret != 1) break;
		printf ("About to execute array[%ld] = 0 (adress = %p)\n", array_iterator, &array[array_iterator]);
		array[array_iterator] = 0;
		fflush (stdin);
	}
	exit(0);
}
/**************************************/
/* traitement des signaux             */
/**************************************/
void  on_memory_access_error (int Num_Sig ){

	printf("-- on_memory_access_error -- Pid %d received signal %d\n", (int)getpid(), Num_Sig);
	printf("-- on_memory_access_error -- Error accessing adress: %p, array index = %ld (hexa : %lx)\n", &array[array_iterator], array_iterator, array_iterator);

	// TO BE COMPLETED: jump to the recovery point now
}
