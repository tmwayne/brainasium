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
#include "game.h"        // game_T, game_init

#define DEFAULT_USER_RC_PATH "/home/tyler/.config/gymrc"
#define DEFAULT_GAMES_DIR "/home/tyler/.local/lib/gym/games"
#define DEFAULT_GAME "sample"

static void set_defaults(dict_T);

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

int main(int argc, char** argv) {

  // Load configurations
  dict_T configs = dict_new();
  set_defaults(configs);

  FILE *userrc = fopen(DEFAULT_USER_RC_PATH, "r");
  if (userrc) configparse(configs, userrc);

  // TODO: Load command-line arguments

  // Load game
  dict_T registry = dict_new();
  load_plugins(registry, dict_get(configs, "games_dir"));

  game_T game = game_init(registry, DEFAULT_GAME);
  dict_free(&registry, (void (*)(void *)) entry_free);

  // Play
  time_t start = time(NULL);

  game->play();

  time_t elapsed = time(NULL) - start;

  // Save output
  FILE *fout = fopen(dict_get(configs, "output_file"), "a");
  if (fout) {
    // TODO: write timestamp
    char now[20];
    fprintf(fout, "%s|", timestamp(now, 20));
    fprintf(fout, "%s|", DEFAULT_GAME);
    fprintf(fout, "%ld\n", elapsed);
    // TODO: write score
    fclose(fout);
  }

  // Cleanup
  game_free(&game);

}

static void set_defaults(dict_T configs) {
  
  dict_set(configs, "games_dir", DEFAULT_GAMES_DIR);

}
