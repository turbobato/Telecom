#include <stdio.h>
#include <string.h>

void display_message(char * message)
{
  printf(message);
}

char isCharInteger(char c);
char charToInteger(char c);
char stringToInteger(char * c);

int main(int argc, char *argv[]) {

  if (argc != 3) {
    display_message("Wrong usage, 2 parameters expected : ./sum param1 param2 \n");
    return 0;
  }

  int ok1;
  int ok2;
  int i = 0;
  
  while (argv[1][i] != '\0') {
    ok1= isCharInteger(argv[1][i]);
    if (!ok1) break;
    i++;
  }

  i = 0;

  while (argv[2][i] != '\0') {
    ok2 = isCharInteger(argv[2][i]);
    if (!ok2) break;
    i++;
  }

  if (!ok1 || !ok2) {
    display_message("Wrong usage, parameters param1 and param2 should be integers when executing ./sum param1 param2");
    return 0;
  }

  printf("Le résultat de la somme est %d", stringToInteger(argv[1])+stringToInteger(argv[2]));
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


char charToInteger(char c)
{
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
