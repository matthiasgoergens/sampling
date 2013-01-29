#include<stdlib.h>
#include<stdio.h>
#include<time.h>

int startSize = 160;

// consider re-using memory.  After profiling.
char* readline () {
  if (feof(stdin)) return 0;
  int size = startSize;
  char* line = calloc(size, sizeof(char));
  int c = 0; int i = 0;
  while (('\n' != c) && (EOF != (c = getc(stdin))) && c) {
    line[i++] = c;
    while (i + 1 >= size)
      if (!(line = realloc(line, size *= 2)))
        exit (1); }
  line[i++] = 0;
  return line; }
int dropLine () {
  if (feof(stdin)) return 0;
  int c = 0;
  while (('\n' != c) && (EOF != (c = getc(stdin))) && c) ;
  return 1; }

int maximumIndex (int n, unsigned int* a) {
  int mI = n;
  while(n--)
    if (a[n] > a[mI]) mI = n;
  return mI; }

char** sample (int n) {
  char** corpus = calloc(n+1, sizeof(char*));
  unsigned int* corpusR = calloc(n, sizeof(unsigned int));
  for (int i = 0; i < n; i++)
    if (! (corpus[i] = readline())) return corpus;
  // Consider alias method.
  for (int i = 0; i < n; i++) {
    corpusR[i] = rand (); }
  int mI = maximumIndex (n, corpusR);
  fprintf(stderr,"%i: %i\n", mI, corpusR[mI]);
  while (1) {
    unsigned int c = rand ();
    if (corpusR[mI] <= c) {
      if (!dropLine()) { fprintf(stderr,"\n"); return corpus; }}
    else {
      char* line = readline ();
      if (!line) {fprintf (stderr, "\n"); return corpus;}
      corpusR[mI] = c;
      corpus[mI] = line;
      mI = maximumIndex(n, corpusR);
      fprintf(stderr,"\n%i: %i\t", mI, corpusR[mI]); } } }

int main (int argc, char** argv) {
  srand(time(0));
  int n = 100;
  if (argc >= 2)
    n = atoi(argv[1]);
  fprintf(stderr,"%i\n", n);
  for (char** lines = sample (n); *lines; lines++) {
    printf("%s", *lines);
    free (*lines); }
  exit(0); }
