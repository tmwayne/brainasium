//
// -----------------------------------------------------------------------------
// Quizzer
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2021
//

#include <stdio.h>             // printf, fopen
#include <stdlib.h>            // srand, rand, RAND_MAX, EXIT_FAILURE
#include <time.h>              // time
#include <limits.h>            // LINE_MAX
#include <string.h>            // strdup
#include "argparse.h"

#include <configparse.h>       // configparse
#include <cstrings.h>          // strmatch, get_line
#include <error.h>             // assert
#include "exercise.h"          // exercise_T, exercise_new
#include "registry.h"          // entry_T, entry_new

#define EXERCISE_NAME "flashcards"
#define USERRC_PATH "/home/tyler/.config/flashcardsrc"

#define NLINES 128
#define MAX_BUF 128

#define config_get(key) dict_get(configs, (key))
#define config_set(key, val) dict_set(configs, (key), (void *) (val))

struct pair {
  char *a;
  char *b;
};

static void swap(struct pair **a, struct pair **b) {

  struct pair *tmp = *a;
  *a = *b;
  *b = tmp;

}

void shuffle_cards(struct pair **cards, int n) {

  srand(time(NULL));

  for(size_t i=n-1; i>0; i--) {
    size_t j = rand() / (RAND_MAX / n);
    struct pair *t = cards[j];
    cards[j] = cards[i];
    cards[i] = t;
  }

}

int load_cards(struct pair **cards, FILE *fd) {

  char line[LINE_MAX+1];
  int n = 0;

  // TODO: prevent segfaults when >2 fields in input
  for ( ; n<NLINES && get_line(line, LINE_MAX, fd)>0; n++ ) {
    if (!(cards[n] = calloc(1, sizeof(struct pair)))) return -1;
    char *saveptr = NULL;
    cards[n]->a = strdup(get_tok_r(line, '|', &saveptr));
    cards[n]->b = strdup(get_tok_r(NULL, '|', &saveptr));
  }

  return n;

}

int give_cards(struct pair **cards, int len, int nguess) {

  char guess[MAX_BUF];
  int tries = 0;
  int nright = 0;
  int nwrong = 0;

  for (int i=0; i<len; i++) {
    printf("%d. %s : ", i+1, cards[i]->a);
    // TODO: check that nguess is always greater than 0
    for ( tries=0; tries < nguess; tries++) {
      get_line(guess, MAX_BUF, stdin); 
      if (strcasematch(guess, cards[i]->b)) break;
      else if (tries < nguess-1) printf("Nope, try again : ");
      else {
        printf("The answer is %s...\n", cards[i]->b);
        swap(&cards[nwrong], &cards[i]);
        nwrong++;
      }
    }
  }

  return len - nwrong;

}

int write_misses(struct pair **cards, int len, FILE *fd) {

  // Write headers
  fprintf(fd, "%s|%s\n", cards[0]->a, cards[0]->b);
  cards++;

  for (int i=0; i<len; i++) 
    fprintf(fd, "%s|%s\n", cards[i]->a, cards[i]->b);

  return 0;

}

double play(int argc, char **argv) {

  dict_T configs = dict_new();

  // Set defaults
  config_set("nguess", 2);

  // TODO: rename configuration key names
  // TODO: rename userrc (seems forced here)
  // Load configurations
  FILE *userrc = fopen(USERRC_PATH, "r");
  if (userrc) configparse(configs, userrc);

  // Load command-line arguments
  argp_parse(&argp, argc, argv, 0, 0, configs);
  
  // Load flashcards
  FILE *fin = fopen(config_get("file"), "r");
  struct pair *cards[NLINES];

  int nlines;
  if ((nlines = load_cards(cards, fin)) <= 0) {
    fprintf(stderr, "Failed to allocate memory for flashcards...\n");
    exit(EXIT_FAILURE);
  }

  // Prepare flashcards
  nlines--; // skip header
  shuffle_cards(cards+1, nlines); // ignore header

  int nlines_config = 0;
  if ((nlines_config = (long) config_get("nlines")))
    nlines = nlines_config < nlines ? nlines_config : nlines;

  // Give flashcards
  int nright = give_cards(cards+1, nlines, (int) (long) config_get("nguess"));
  double score = (double) nright / nlines;

  printf("You scored %.0f%!\n", 100*score);

  // Save misses
  char *fmisses = NULL;
  if ((fmisses = config_get("fmisses"))) {
    FILE *fout = fopen(fmisses, "w");
    if (!fout) {
      fprintf(stderr, "Unable to open misses file...\n");
      exit(EXIT_FAILURE);
    }
    write_misses(cards, nlines - nright, fout);
  }

  // Cleanup
  // TODO: free configs dictionary
  // dict_free(&configs);
  for (int i=0; i<nlines; i++) free(cards[i]);

  return score;

}

// interface to Gym routine ----------------------------------------------------

exercise_T init() {

  exercise_T exercise = exercise_new();

  exercise->play = play;
  exercise->args = NULL;

  return exercise;

}

void add_to_registry(dict_T registry, char *plugin_path) {

  assert(registry && plugin_path);

  entry_T entry = entry_new(plugin_path, (void *(*)()) init);
  dict_set(registry, EXERCISE_NAME, entry);

}  
