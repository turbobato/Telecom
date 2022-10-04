#include <stdio.h>
#include <string.h>

char isCharInteger(char c);
char charToInteger(char c);
char stringToInteger(char * c);

int main(int argc, char *argv[]) {
  /* To be completed */

  // check that the user gave a correct number of parameters
  // Usage: ./sum_fscanf file_name
  if (argc!=3){
    printf("mauvais nombre de paramètres");
    return 0;
  }
  // open file_name and retreive integer values it contains
  // use fopen and fscanf to do that
  FILE * f = fopen(argv[1], "r");
  if (f == NULL){
    printf("Error: file %s does not exist\n", argv[1]);;
    return 0;
  }

  char arg1[10];
  char arg2[10];
  int ret;

  ret = fscanf(f, "%s", arg1);
  if (ret != 1) printf ("error when reading input stream\n");
  ret = fscanf(f, "%s", arg2);
  if (ret != 1) printf ("error when reading input stream\n");


  // check for the occurence of errors when using fopen and fscanf,
  // display an error message and quit in case of errors.

  int ok1;
  int ok2;
  int i = 0;
  
  while (arg1[i] != '\0') {
    ok1= isCharInteger(arg1[i]);
    if (!ok1) break;
    i++;
  }

  i = 0;

  while (arg2[i] != '\0') {
    ok2 = isCharInteger(arg2[i]);
    if (!ok2) break;
    i++;
  }

  if (!ok1 || !ok2) {
    printf("Wrong usage, parameters param1 and param2 should be integers when executing");
    return 0;
  }
  // compute the sum of integer values and display it
  printf("Le résultat de la somme est %d", stringToInteger(arg1)+stringToInteger(arg2));
  FILE * out = fopen(argv[2], "w+");
  if (out == NULL){
    printf("Error: file %s does not exist\n", argv[2]);;
    return 0;
  }
  fprintf(out,"%d",stringToInteger(arg1)+stringToInteger(arg2));

  return 0;
}

char isCharInteger(char c)
{
  switch (c) {
    case '0' : 
      return 1;
    case '1' :
      return 1;
    case '2' :
      return 1;
    case '3' :
      return 1;
    case '4' :
      return 1;
    case '5' :
      return 1;
    case '6' :
      return 1;
    case '7' :
      return 1;
    case '8' :
      return 1;
    case '9' :
      return 1;
  } 
  return 0;
}

char charToInteger(char c){
  switch (c) {
    case '0' : 
      return 0;
    case '1' :
      return 1;
    case '2' :
      return 2;
    case '3' :
      return 3;
    case '4' :
      return 4;
    case '5' :
      return 5;
    case '6' :
      return 6;
    case '7' :
      return 7;
    case '8' :
      return 8;
    case '9' :
      return 9;
  }
  return 0;
}

char stringToInteger(char * c)
{
  int i = 0;
  while(c[i]!='\0') {
    i++;
  }
  char res=0;
  char pow = 1;
  for (int j = i-1; j >= 0 ; j--) {
    res = res + pow*charToInteger(c[j]);
    pow = 10*pow;
  }
  return res;
}
