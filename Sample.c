#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<assert.h>
#include<limits.h>
#include<math.h>

int startSize = 160;

typedef struct {
  int key;
  unsigned int length;
  char* line;
} line_t;


line_t emptyLine () {
  line_t l = { key : INT_MAX,
               length : 0,
               line : 0};
  return l;
}

int cmpLine (line_t a, line_t b) { return a.key - b.key; }

int readline (line_t *l) {
  if (feof(stdin)) return 0;
  if (l->length < startSize) {
    l->line = realloc(l->line, startSize);
    l->length = startSize; }
  int c = 0; int i = 0;
  while (('\n' != c) && (EOF != (c = getc(stdin))) && c) {
    while (i + 2 >= l->length)
      if (!(l->line = realloc(l->line, l->length *= 2)))
        exit (1);
    l->line[i++] = c; }
  l->line[i++] = 0;
  return 1; }

int dropLines (double skip) {
  if ((skip > 0) && feof(stdin)) return 0;
  int c = 0;
  while (('\n' != c) && (skip-- > 0) && (EOF != (c = getchar())) && c) ;
  return 1; }

typedef int cmp_t (void*, void*);
typedef void* make_t ();

typedef struct {
  int size;
  line_t* items;
} heap_t;

heap_t* full(int n) {
  // fprintf (stderr, ".\n");
  heap_t* h = malloc(sizeof(heap_t));
  h->size = n;
  h->items = calloc(n, sizeof(line_t));
  for (int i = 0; i < n; i++)
    h->items[i] = emptyLine();
  return h;
}

int up    (int i) { return i ? (i-1) / 2 : i; }
int left  (int i) { return 2*i + 1; }
int right (int i) { return 2*i + 2; }

void bubbleDown (heap_t *h) {
  line_t* items = h->items;
  int size = h->size;

  int i = 0;
  while (1) {
    line_t ci = items[i];
    
    int l = left (i);
    int r = right (i);

    int new = ((size <= r) || (cmpLine(items[l], items[r]) > 0)) ? l : r;
    new = ((size <= new) || (cmpLine (ci, items[new]) > 0)) ? i : new;

    if (i == new) break;
    
    line_t temp = items[i];
    items[i] = items[new];
    items[new] = temp;
     
    i = new; } }

/* How about measuring time between picks instead?
   Would need less randoms.

   c = corpusSize
   s = # items seen (including current)

   prob. to pick item:
   p = c / s
   (really? no. Isn't that the answer for putting the items back?)
   p = 1 - sum (1 / (s-i)) [0..i-1]
   actually, yes: p = c / s
   (It's the probability of being in the first c places
   in a random shuffle of s items.)

   percentiles!
   inverted distribution function. (or so.)

   There's Vitter's algorithm Z (see paper)
   but we can cheat!

   What's the distribution of waiting times until getting a number
   smaller than the current maximum?  It's an exponential
   distribution!

   All numbers normalized to [0,1]:
   max \elem [0,1]
   
   prob. we haven't found a new `min-max' until at least step s+i:
   (1-max) ** i


   F^{-1}(x) = - log(x) / a
   for x in [0,1].

   for x in [0,RAND_MAX]

   - log (x / RAND_MAX) / a
   gives
   (log RAND_MAX - log x) / a

   needs floor for exponential -> geometric.

   p = 1 - e**lambda
   
   ln (U) / ln (1-p)

   floor $ (log x - log RAND_MAX) / log (1-cmax/RAND_MAX)
   
  
 */

// floating point troubles..
heap_t* sample (int n) {
  // fprintf (stderr,"+\n");
  heap_t* h = full(n);

  while (1) {
    unsigned int c = rand ();
    if (c) {
      int key = h->items[0].key;
      fprintf(stderr, "key: %i\t", key);
      if (key < RAND_MAX) {
        double p = key / (double) RAND_MAX;
        double cc = c / (double) RAND_MAX;
        fprintf(stderr, "p: %f", p);
        double skip =  ( log (cc) / log (1 - p));
        fprintf (stderr, "\tskip: %f\t", skip);
        if (!dropLines(skip)) {
          // fprintf(stderr, "-\n");
          return h; }}
        
      int rand_max = RAND_MAX - (RAND_MAX % key);
      do { c = rand ();
      } while (c > rand_max);
      c %= key;
      fprintf (stderr, "\tc: %i\t", c);
      fprintf (stderr, "rand_max: %i", rand_max);

      // fprintf(stderr, "-\n");
      // fprintf(stderr, "%i\n", c);
      fprintf(stderr,"\n");
      if (! readline (h->items)) return h;
      h->items[0].key = c;      
      bubbleDown (h); }}}


void printLines (heap_t* h) {
  for(int i = 0; i < h->size; i++) {
    char* line = h->items[i].line;
    if (line) printf("%s", line); }}

int main (int argc, char** argv) {
  srand(time(0));
  int n = argc < 2 ? 1 : atoi(argv[1]);
  if (0>n) n=0;
  fprintf(stderr,"%i\n", n);
  // Note: This would leak memory, if the programme would run for longer.
  printLines (sample (n));
  exit(0); }
