#include <stdio.h>

int integers_to_sum[50];

int main(int argc, char *argv[]) {

  // check that the user gave a correct number of parameters
  // Usage: ./sum_fscanf file_name
  if(argc!=2)
  {
    printf("Wrong usage: expected ./sum_fscanf file_name");
    return -1;
  }

  // open file_name and retreive integer values it contains
  // use fopen and fscanf to do that

  // check for the occurence of errors when using fopen and fscanf,
  // display an error message and quit in case of errors.
  FILE * f = fopen(argv[1], "r");
  if (f == NULL)
  {
    printf("Error: could not open file %s, make sure it exists\n", argv[1]);
    return -2;
  }

  int idx = 0;
  int ret = fscanf(f, "integer: %d", integers_to_sum);
  while(ret==1)
  {
    idx++;
    ret = fscanf(f, ", integer: %d", &(integers_to_sum[idx]));
  }
  if(ret==-1)
  {
    printf("Error: could not match integer values in file %s\n", argv[1]);
    return -3;
  }

  int result = 0;
  int iter = 0;
  for(iter=0; iter<idx;iter++)
  {
    result = result + integers_to_sum[iter];
  }

  // first variant: compute the sum of integer values and display it
  printf("Result: %d\n", result);

  // second variant: compute the sum of integer values and print it in
  // an output file

  return 0;
}
