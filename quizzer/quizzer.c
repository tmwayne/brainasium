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

#include <cstrings.h>          // strmatch, get_line
#include <error.h>             // assert
#include "exercise.h"          // exercise_T, exercise_new
#include "registry.h"          // entry_T, entry_new

#define EXERCISE_NAME "quizzer"

#define NLINES 128
#define MAX_BUF 128

#define config_get(key) dict_get(configs, (key))

struct pair {
  char *a;
  char *b;
};

static void swap(struct pair **a, struct pair **b) {

  struct pair *tmp = *a;
  *a = *b;
  *b = tmp;

}

void shuffle_quiz(struct pair **quiz, int n) {

  srand(time(NULL));

  for(size_t i=n-1; i>0; i--) {
    size_t j = rand() / (RAND_MAX / n);
    struct pair *t = quiz[j];
    quiz[j] = quiz[i];
    quiz[i] = t;
  }

}

int load_quiz(struct pair **quiz, FILE *fd) {

  char line[LINE_MAX+1];
  int n = 0;

  // TODO: prevent segfaults when >2 fields in input
  for ( ; n<NLINES && get_line(line, LINE_MAX, fd)>0; n++ ) {
    if (!(quiz[n] = calloc(1, sizeof(struct pair)))) return -1;
    char *saveptr = NULL;
    quiz[n]->a = strdup(get_tok_r(line, '|', &saveptr));
    quiz[n]->b = strdup(get_tok_r(NULL, '|', &saveptr));
  }

  return n;

}

int give_quiz(struct pair **quiz, int len, int nguess) {

  char guess[MAX_BUF];
  int tries = 0;
  int nright = 0;
  int nwrong = 0;

  for (int i=0; i<len; i++) {
    printf("%s : ", quiz[i]->a);
    for ( tries=0; tries < nguess; tries++) {
      get_line(guess, MAX_BUF, stdin); 
      if (strcasematch(guess, quiz[i]->b)) break;
      else if (tries < nguess-1) printf("Nope, try again : ");
      else {
        printf("The answer is %s...\n", quiz[i]->b);
        swap(&quiz[nwrong], &quiz[i]);
        nwrong++;
      }
    }
  }

  return len - nwrong;

}

int write_misses(struct pair **quiz, int len, FILE *fd) {

  // Write headers
  fprintf(fd, "%s|%s\n", quiz[0]->a, quiz[0]->b);
  quiz++;

  for (int i=0; i<len; i++) 
    fprintf(fd, "%s|%s\n", quiz[i]->a, quiz[i]->b);

  return 0;

}

double play(int argc, char **argv) {

  dict_T configs = dict_new();

  // Set defaults
  dict_set(configs, "nguess", 3);


  // Load command-line arguments
  argp_parse(&argp, argc, argv, 0, 0, configs);
  
  // Load quiz
  FILE *fin = fopen(dict_get(configs, "file"), "r");
  struct pair *quiz[NLINES];

  int nlines;
  if ((nlines = load_quiz(quiz, fin)) <= 0) {
    fprintf(stderr, "Failed to allocate memory for quiz...\n");
    exit(EXIT_FAILURE);
  }

  // Prepare quiz
  nlines--; // skip header
  shuffle_quiz(quiz+1, nlines); // ignore header

  int nlines_config = 0;
  if ((nlines_config = (long) config_get("nlines")))
    nlines = nlines_config < nlines ? nlines_config : nlines;

  // Give quiz
  int nright = give_quiz(quiz+1, nlines, (int) (long) config_get("nguess"));
  double score = (double) nright / nlines;

  printf("You scored %.0f%!\n", 100*score);

  // Save misses
  char *fmisses = NULL;
  if ((fmisses = dict_get(configs, "fmisses"))) {
    FILE *fout = fopen(fmisses, "w");
    if (!fout) {
      fprintf(stderr, "Unable to open misses file...\n");
      exit(EXIT_FAILURE);
    }
    write_misses(quiz, nlines - nright, fout);
  }

  // Cleanup
  // TODO: free configs dictionary
  // dict_free(&configs);
  for (int i=0; i<nlines; i++) free(quiz[i]);

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
