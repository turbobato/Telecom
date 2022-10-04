#include <stdio.h>


int main(int argc, char *argv[]) {

  // display a message to indicate what users should write
  // use printf to do that
  printf("Enter an first integer value:\n");
  // wait until users have provided you information
  // use scanf to do that
  int first_integer;
  char ret = scanf("%d", &first_integer);
  // check that information provided is an integer
  if (ret != 1)
  {
    printf ("Error when reading input, make sure you write an integer value\n");
    return -1;
  }
  // repeat previous steps to get a second integer
  printf("Enter an second integer value:\n");
  int second_integer;
  ret = scanf("%d", &second_integer);
  if (ret != 1)
  {
    printf ("Error when reading input, make sure you write an integer value\n");
    return -2;
  }

  // compute the sum of integer values and display it
  printf("Result of (%d)+(%d): %d\n", first_integer, second_integer, first_integer+second_integer);

  return 0;
}
