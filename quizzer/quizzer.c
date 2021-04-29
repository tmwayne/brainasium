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
#include <readline/readline.h> // readline

#include <c-strings.h>         // strmatch
#include <error.h>             // assert
#include "game.h"              // game_T, game_new
#include "registry.h"          // entry_T, entry_new

#define EXERCISE_NAME "quizzer"

#define NLINES 128
#define MAX_BUF 128

struct pair {
  char *a;
  char *b;
};

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

int give_quiz(struct pair **quiz, int len) {

  char *guess;
  int tries = 0;
  int nright = 0;

  for (int i=0; i<len; i++) {
    printf("%s : ", quiz[i]->a);
    for ( tries=0; tries < 3; tries++) {
      // TODO: use the stack instead of the allocating on the heap every time
      char *guess = readline(NULL);
      if (strmatch(guess, quiz[i]->b)) {
        nright++;
        break;
      }
      else if (tries < 2) printf("Nope, try again : ");
      else printf("The answer is %s...\n", quiz[i]->b);
      free(guess);
    }
  }

  return nright;

}

double play(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  // Load quiz
  FILE *fin = fopen(argv[1], "r");
  struct pair *quiz[NLINES];

  int nlines = load_quiz(quiz, fin);
  if (nlines <= 0) {
    fprintf(stderr, "Failed to allocate memory for quiz...\n");
    exit(EXIT_FAILURE);
  }

  // Prepare quiz
  shuffle_quiz(quiz+1, nlines-1); // ignore header

  // Give quiz
  double score = (double) give_quiz(quiz+1, nlines-1) / (nlines-1);

  printf("You scored %.0f%!\n", 100*score);

  // Cleanup
  for (int i=0; i<nlines; i++) free(quiz[i]);

  return score;

}

// interface to Gym routine ----------------------------------------------------

game_T init() {

  game_T game = game_new();

  game->play = play;
  game->args = NULL;

  return game;

}

void add_to_registry(dict_T registry, char *plugin_path) {

  assert(registry && plugin_path);

  entry_T entry = entry_new(plugin_path, (void *(*)()) init);
  dict_set(registry, EXERCISE_NAME, entry);

}  
