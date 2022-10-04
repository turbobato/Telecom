#include <stdio.h>
#include <string.h>

void display_message(char * message)
{
  printf(message);
}

char isCharInteger(char c);

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
