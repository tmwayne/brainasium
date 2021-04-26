//
// -----------------------------------------------------------------------------
// main.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2021
//

#include <dict.h> // dict_T, dict_new, dict_free
#include <configparse.h> // configparse
#include "registry.h" // load_plugins
#include "game.h" // game_T, game_init

#define DEFAULT_USER_RC_PATH "/home/tyler/.config/gymrc"
#define DEFAULT_GAMES_DIR "/home/tyler/.local/lib/gym/games"

static void set_defaults(dict_T);

int main(int argc, char** argv) {

  // Load configurations
  dict_T configs = dict_new();
  set_defaults(configs);

  FILE *userrc = fopen(DEFAULT_USER_RC_PATH, "r");
  if (userrc) configparse(configs, userrc);

  dict_T registry = dict_new();
  load_plugins(registry, dict_get(configs, "games_dir"));

  game_T game = game_init(registry, "sample");
  dict_free(&registry, (void (*)(void *)) entry_free);

  game->play();

  game_free(&game);

}

static void set_defaults(dict_T configs) {
  
  dict_set(configs, "games_dir", DEFAULT_GAMES_DIR);

}
