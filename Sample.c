#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<assert.h>

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

// heap!

char** sample (int n) {
  char** corpus = calloc(n+1, sizeof(char*));
  unsigned int* corpusR = calloc(n, sizeof(unsigned int));
  int last = 0;
  
  // maximum at root.
  int up    (int i) { return i ? (n-1) / 2 : i; }
  int left  (int i) { return 2*n + 1; }
  int right (int i) { return 2*n + 2; }
  void add (unsigned int k, char* v) {
    corpusR[last] = k;
    corpus[last] = v;
    int i = last++;
    while (corpusR[up(i)] < corpusR[i]) {
      // repair by bubbling up:
      char* temp = corpus[i];
      unsigned int tempR = corpusR[i];
      corpus[i] = corpus[up(i)];
      corpusR[i] = corpusR[up(i)];
      corpus[up(i)] = temp;
      corpusR[up(i)] = tempR;
      i = up(i); } }
  void replaceTop (unsigned int k, char* v) {
    free (corpus[0]);
    corpusR[0] = k;
    corpus[0] = v;
    // bubble down:
    int i = 0;
    while (1) {
      unsigned int ci = corpusR[i];
      int l = left (i); unsigned int cl = corpusR[l];
      int r = right (i); unsigned int cr = corpusR[r];

      int new;
      if ((last <= l) || (cl <= ci))
        if ((last <= r) || (cr <= ci))
          break;
        else new = r;
      else if (cr < cl)
        new = l;
      else new = r;

      unsigned int tempR = corpusR[i];
      char* temp         = corpus[i];
      corpusR[i] = corpusR[new];
      corpus[i]  = corpus[new];
      corpusR[new] = tempR;
      corpus[new]  = temp;

      i = new; } }

  int i;
  for (i = 0; i < n; i++) {
    char* line = readline();
    if (!line) {
      fprintf(stderr, "Early exodus.\n");
      fprintf(stderr, "%p\n", line);
      return corpus;
    }
    add (rand(), line);
  }
  assert (last == i);
    
  // Consider alias method.
  // fprintf(stderr,"%i: %i\n", mI, corpusR[mI]);
  while (1) {
    unsigned int c = rand ();
    if (c < corpusR[0]) {
      fprintf(stderr, "%i\n", c);
      char* line = readline ();
      if (!line) return corpus;
      replaceTop (c, line); }
    else if (!dropLine())
      return corpus; }}

int main (int argc, char** argv) {
  srand(time(0));
  int n = 1;
  if (argc >= 2)
    n = atoi(argv[1]);
  // fprintf(stderr,"%i\n", n);
  for (char** lines = sample (n); *lines; lines++) {
    printf("%s", *lines);
    free (*lines); }
  exit(0); }
