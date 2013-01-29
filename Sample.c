#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<assert.h>
#include<limits.h>

int startSize = 160;

typedef struct {
  unsigned int key;
  unsigned int length;
  char* line;
} line_t;


line_t* emptyLine () {
  line_t* l = malloc(sizeof(line_t));
  l->key = UINT_MAX;
  l->length = 0;
  l->line = 0;
  return l;
}

int cmpLine (line_t a, line_t b) {
  return a.key - b.key;
}

// consider re-using memory.  After profiling.
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

/* heap_t* empty(int n, cmp_t* cmp) {
  heap_t* h = malloc(sizeof(heap_t));
  h->items = calloc(n, sizeof(void*));
  h->last = h->items + n;
  h->next = h->items;
  h->cmp = cmp;
  return h;
  } */

heap_t* full(int n) {
  fprintf (stderr, ".\n");
  heap_t* h = malloc(sizeof(heap_t));
  h->size = n;
  h->items = calloc(n, sizeof(line_t));
  for (int i = 0; i < n; i++)
    h->items = emptyLine();
  return h;
}

int up    (int i) { return i ? (i-1) / 2 : i; }
int left  (int i) { return 2*i + 1; }
int right (int i) { return 2*i + 2; }

/* int add (void* item, heap_t* h) {
  // unsigned int k, char* v) {
  if (h->next - h->last >= 0) return -1;
  
  *(h->next) = item;
  int i = h->next - h->last;
  h->next ++;

  // repair by bubbling up:
  while (h->cmp (h->items[up(i)], h->items[i]) < 0) {
    void* temp = h->items[i];
    h->items[i] = h->items[up(i)];
    h->items[up(i)] = temp;
    i = up(i); }
  // return # of free spaces remaining.
  return h->last - h->next;
  } */

void bubbleDown (heap_t *h) {
  line_t* items = h->items;
  int size = h->size;

  int i = 0;
  while (1) {
    line_t ci = items[i];
    int l = left (i);  line_t cl = items[l];
    int r = right (i); line_t cr = items[r];
    
    int new;
    if ((size <= l) || (cmpLine (cl, ci) <= 0))
      if ((size <= r) || (cmpLine (cr, ci) <=0))
        break;
      else new = r;
    else if (cmpLine(cr, cl) < 0)
      new = l;
    else new = r;
    
    line_t temp = items[i];
    items[i] = items[new];
    items[new] = temp;
     
    i = new; } }

/* void replaceTop (void* item, heap_t h) {
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
*/

heap_t* sample (int n) {
  fprintf (stderr,"+\n");
  heap_t* h = full(n);

  // Consider alias method.
  // fprintf(stderr,"%i: %i\n", mI, corpusR[mI]);
  while (1) {
    unsigned int c = rand ();
    if (c < h->items[0].key) {
      fprintf(stderr, "-\n");
      fprintf(stderr, "%i\n", c);

      if (! readline (h->items)) return h;
      (h->items[0]).key = c;      
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
  int n = 1;
  if (argc >= 2)
    n = atoi(argv[1]);
  fprintf(stderr,"%i\n", n);
  printLines (sample (n));
  //    free (*lines); }
  exit(0); }
