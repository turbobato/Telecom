#include <stdio.h>
#include <string.h>

void display_message(char * message)
{
  printf("%s", message);
}

// declare functions before using them
char isCharInteger(char c);
char charToInteger(char c);
char isStringInteger(char * c);
char stringToInteger(char * c);
char stringLength(char * c);

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
    char * arg = argv[i];
    if(!isStringInteger(arg))
    {
      display_message("Wrong usage, parameters param1 and param2 should be integers when executing ./sum param1 param2\n");
      return -2;
    }

  }

  

  /* arguments have been checked, we can proceed to the sum:
     1 - convert string into an integer value
     2 - sum obtained values
  */

  char first_integer = stringToInteger(argv[1]);
  char second_integer = stringToInteger(argv[2]);

  printf("Result: %d\n", first_integer+second_integer);

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

char charToInteger(char c)
{
  switch (c) {
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
  }
  return -1;
}

char isStringInteger(char * s)
{
    int j=0;
    if(s[j]=='-')
      j=1;
    while(s[j]!='\0')
    {
      char c = s[j];
      if(isCharInteger(c)==0)
      {
	return 0;
      }
      j++;
    }
    return 1;
}

char stringToInteger(char * c)
{
  char sign = 1;
  char result=0;
  char i;

  if(c[0]=='-')
  {
    sign = -1;
  }

  char length = stringLength(c);

  for(i=length-1; i>=0; i--)
  {
    if(isCharInteger(c[i]))
    {
      char current_integer = charToInteger(c[i]);
      char factor = 1;
      int j;
      for(j=0;j<length-i-1;j++)
      {
        factor = factor*10;
      }
      current_integer = current_integer * factor;
      result = result+current_integer;
    }
  }

  return sign*result;
}

char stringLength(char * c)
{
  char j=0;
  while(c[j]!='\0')
  {
    j++;
  }
  return j;
}
