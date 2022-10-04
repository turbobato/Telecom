#include <stdio.h>
#include <string.h>

void display_message(char * message)
{
  printf("%s", message);
}

// declare function before using it
char isCharInteger(char c);

int main(int argc, char *argv[]) {

  // first, check that the number of parameters is correct
  if(argc!=3)
  {
    display_message("Wrong usage, 2 parameters expected: ./sum param1 param2");
    return -1;
  }

  // now, check that parameters represent integer values
  // declare, implement, and use function isCharInteger
  int i;
  for(i=1; i<argc;i++)
  {
    const char * arg = argv[i];
    int j=0;
    if(arg[j]=='-')
      j=1;
    while(arg[j]!='\0')
    {
      char c = arg[j];
      if(isCharInteger(c)==0)
      {
        display_message("Wrong usage, parameters param1 and param2 should be integers when executing ./sum param1 param2\n");
        return -2;
      }
      j++;
    }

  }

  return 0;
}

char isCharInteger(char c)
{
  switch (c) {
    case '0':
      return 1;
    case '1':
      return 1;
    case '2':
      return 1;
    case '3':
      return 1;
    case '4':
      return 1;
    case '5':
      return 1;
    case '6':
      return 1;
    case '7':
      return 1;
    case '8':
      return 1;
    case '9':
      return 1;
  }
  return 0;
}
