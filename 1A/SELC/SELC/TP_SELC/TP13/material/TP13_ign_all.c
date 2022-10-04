#include <stdio.h>
#include <signal.h>
#include <unistd.h>
int main(int argc, char *argv[]){
  int Nb_Sig;
  printf("NSIG = %d", NSIG);
  sleep(3);
  for(Nb_Sig = 1; Nb_Sig < NSIG ; Nb_Sig ++)
  {
   
  } 
  while(1)
  {
    sleep(5);
  } /* Wait for receiving signals */
  return 0;
}
