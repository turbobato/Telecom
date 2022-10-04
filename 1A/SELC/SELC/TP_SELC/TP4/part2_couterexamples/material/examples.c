#include <stdio.h>

void init(int * tab);
void swap(float f1, float f2);
int * alloc (int val);

int main(int argc, char *argv[]) {
  int * ptr = alloc(7);
  for (int i=0; i<20; i++){
    printf("%d \n",ptr[i]);
  }

  float f1=0.7;
  float f2=1.2;
  swap(f1,f2);
  printf("%f %f \n",f1,f2);
  swap(f1, f2);
  
  return 0;
}

int * alloc (int val){
   int tab[20];
   for (int i = 0; i < 20; i++) {
     tab[i]=val;
   }
   int * tmp = tab;
   return tmp;
}

void swap(float f1, float f2)
{
  static int compteur = 0;
  float tmp = f1;
  f1=f2;
  f2=tmp;
  compteur++;
  printf("%d \n",compteur);
  return;
}
