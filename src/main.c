//
// -----------------------------------------------------------------------------
// main.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2021
//

#include <stdlib.h>      // calloc
#include <dict.h>        // dict_T, dict_new, dict_free
#include <configparse.h> // configparse
#include <time.h>        // time_t,
#include "registry.h"    // load_plugins
#include "argparse.h"

// TODO: change "game" to exercise
#include "game.h"        // game_T, game_init

#define DEFAULT_USER_RC_PATH "/home/tyler/.config/gymrc"
#define DEFAULT_EXERCISE_DIR "/home/tyler/.local/lib/gym/games"
#define DEFAULT_EXERCISE "sample"

static void set_defaults(dict_T);
static char *timestamp(char *, size_t);

int main(int argc, char** argv) {

  // Set defaults
  dict_T configs = dict_new();
  set_defaults(configs);

  // Load configurations
  FILE *userrc = fopen(DEFAULT_USER_RC_PATH, "r");
  if (userrc) configparse(configs, userrc);

  // Load command-line arguments

  // ARGP_IN_ORDER : force arguments to be parsed in order 
  // so that flags aren't parsed early
  argp_parse(&argp, argc, argv, ARGP_IN_ORDER, 0, configs);
  // TODO: free any structures related to argp

  // Remaining arguments are passed to the exercise program
  argc = (int) (long) dict_get(configs, "argc");
  argv = dict_get(configs, "argv");

  // Load game
  dict_T registry = dict_new();
  load_plugins(registry, dict_get(configs, "exercise_dir"));

  char *exercise = dict_get(configs, "exercise");
  if (!exercise) {
    fprintf(stderr, "Exercise not recognized...\n");
    exit(EXIT_FAILURE);
  }
  game_T game = game_init(registry, exercise);
  dict_free(&registry, (void (*)(void *)) entry_free);

  // Play
  time_t start = time(NULL);
  game->play(argc, argv);

  time_t elapsed = time(NULL) - start;
  printf("It took you %ld seconds...\n", elapsed);

  // Save output
  FILE *fout = fopen(dict_get(configs, "output_file"), "a");
  if (fout) {
    char now[20];
    fprintf(fout, "%s|", timestamp(now, 20));
    fprintf(fout, "%s|", exercise);
    fprintf(fout, "%ld\n", elapsed);
    // TODO: write score
    fclose(fout);
  }

  // Cleanup
  game_free(&game);

}

static void set_defaults(dict_T configs) {
  
  dict_set(configs, "exercise_dir", DEFAULT_EXERCISE_DIR);
  dict_set(configs, "exercise", DEFAULT_EXERCISE);

}

static char *timestamp(char *buf, size_t len) {

  if (buf && len < 20) return NULL;
  else if (!buf) buf = calloc(20, sizeof(char));

  time_t now = time(NULL);
  struct tm *tm = localtime(&now);

  snprintf(buf, 20, "%04d-%02d-%02d %02d:%02d:%02d",
    tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, 
    tm->tm_hour, tm->tm_min, tm->tm_sec);

  return buf;

}
