#include<stdlib.h>
#include<stdio.h>

int main (int argc, char** argv) {
  while (1) {
    for(int i = 0; i < 100000; i++) {
      int c = getchar ();
      switch (c) {
      case 0: printf ("\nZERO after %i!\n", i); exit(0); break;
      case EOF: printf ("\nEOF\n"); exit(0); break; } }
    printf ("."); } }
