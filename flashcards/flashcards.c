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

// TODO: have NLINES be dynamic
#define NLINES 512
#define MAX_BUF 128

#define config_get(key) dict_get(configs, (key))
#define config_set(key, val) dict_set(configs, (key), (void *) (val))

struct card {
  char *a;
  char *b;
};

static void swap(struct card **a, struct card **b) {

  struct card *tmp = *a;
  *a = *b;
  *b = tmp;

}

void shuffle_cards(struct card **cards, int n) {

  srand(time(NULL));

  for(size_t i=n-1; i>0; i--) {
    size_t j = rand() / (RAND_MAX / n);
    struct card *t = cards[j];
    cards[j] = cards[i];
    cards[i] = t;
  }

}

int load_cards(struct card **cards, FILE *fd) {

  char line[LINE_MAX+1];
  int n = 0;

  // TODO: prevent segfaults when >2 fields in input
  for ( ; n<NLINES && get_line(NULL, line, LINE_MAX, fd)==E_OK; n++ ) {
    if (!(cards[n] = calloc(1, sizeof(struct card)))) return -1;
    char *saveptr = NULL;
    cards[n]->a = strdup(get_tok_r(line, '|', &saveptr));

    // TODO: return error on malformed input
    cards[n]->b = strdup(get_tok_r(NULL, '|', &saveptr));
  }

  return n;

}

int give_cards(struct card **cards, int len, int nguess, int reverse) {

  char guess[MAX_BUF];
  int tries = 0;
  int nright = 0;
  int nwrong = 0;

  for (int i=0; i<len; i++) {
    char *prompt = reverse ? cards[i]->b : cards[i]->a;
    char *answer = reverse ? cards[i]->a : cards[i]->b;

    printf("%d. %s : ", i+1, prompt);
    // TODO: check that nguess is always greater than 0
    for ( tries=0; tries < nguess; tries++) {
      get_line(NULL, guess, MAX_BUF, stdin); 
      if (strcasematch(guess, answer)) break;
      else if (tries < nguess-1) printf("Nope, try again : ");
      else {
        printf("The answer is %s...\n", answer);
        swap(&cards[nwrong], &cards[i]);
        nwrong++;
      }
    }
  }

  return len - nwrong;

}

int write_misses(struct card **cards, int len, FILE *fd) {

  // Write headers
  fprintf(fd, "%s|%s\n", cards[0]->a, cards[0]->b);
  cards++;

  for (int i=0; i<len; i++) 
    fprintf(fd, "%s|%s\n", cards[i]->a, cards[i]->b);

  return 0;

}

double play(int argc, char **argv) {

  // TODO: check that the quiz and misses file aren't the same

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

  // Convert integer configurations
  int nlines_config = (int) (long) config_get("nlines");
  int nguess = (int) (long) config_get("nguess");
  
  // TODO: this potentially converts a NULL pointer to zero, make this simpler
  int reverse = (int) (long) config_get("reverse");
  
  // Load flashcards
  FILE *fin = fopen(config_get("file"), "r");
  struct card *cards[NLINES];

  int ncards;
  if ((ncards = load_cards(cards, fin)) <= 0) {
    fprintf(stderr, "Failed to allocate memory for flashcards...\n");
    exit(EXIT_FAILURE);
  }

  // Prepare flashcards
  ncards--; // skip header
  shuffle_cards(cards+1, ncards); // ignore header

  if (nlines_config) ncards = nlines_config < ncards ? nlines_config : ncards;

  // Give flashcards
  int nright = give_cards(cards+1, ncards, nguess, reverse);
  double score = (double) nright / ncards;

  printf("You scored %.0f%!\n", 100*score);

  // Save misses
  char *fmisses = NULL;
  if ((fmisses = config_get("fmisses")) && nright < ncards) {
    FILE *fout = fopen(fmisses, "w");
    if (!fout) {
      fprintf(stderr, "Unable to open misses file...\n");
      exit(EXIT_FAILURE);
    }
    write_misses(cards, ncards - nright, fout);
  }

  // Cleanup
  // TODO: free configs dictionary
  // dict_free(&configs);
  for (int i=0; i<ncards; i++) free(cards[i]);

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
