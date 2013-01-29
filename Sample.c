#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<assert.h>
#include<limits.h>

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

int dropLine () {
  if (feof(stdin)) return 0;
  int c = 0;
  while (('\n' != c) && (EOF != (c = getchar())) && c) ;
  return 1; }

typedef int cmp_t (void*, void*);
typedef void* make_t ();

typedef struct {
  int size;
  line_t* items;
} heap_t;

heap_t* full(int n) {
  fprintf (stderr, ".\n");
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
    
    int l = left (i);  line_t cl = items[l];
    int r = right (i); line_t cr = items[r];

    int new;
    new = ((size <= r) || (cmpLine(cl, cr) > 0)) ? l : r;
    new = ((size <= new) || (cmpLine (ci, items[new]) > 0)) ? i : new;

    if (i == new) break;
    
    line_t temp = items[i];
    items[i] = items[new];
    items[new] = temp;
     
    i = new; } }

heap_t* sample (int n) {
  fprintf (stderr,"+\n");
  heap_t* h = full(n);

  while (1) {
    unsigned int c = rand ();
    if (c < h->items[0].key) {
      // fprintf(stderr, "-\n");
      fprintf(stderr, "%i\n", c);

      if (! readline (h->items)) return h;
      h->items[0].key = c;      
      bubbleDown (h); }
    else if (!dropLine()) {
      fprintf(stderr, "-\n");
      return h; }}}

void printLines (heap_t* h) {
  for(int i = 0; i < h->size; i++) {
    char* line = h->items[i].line;
    if (line) printf("%s", line); }}

int main (int argc, char** argv) {
  srand(time(0));
  int n = argc < 2 ? 1 : atoi(argv[1]);
  fprintf(stderr,"%i\n", n);
  // Note: This would leak memory, if the programme would run for longer.
  printLines (sample (n));
  exit(0); }
