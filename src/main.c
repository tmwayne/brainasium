//
// -----------------------------------------------------------------------------
// main.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2021
//

#include "dict.h" // dict_T, dict_new, dict_free
#include "registry.h" // load_plugins
#include "game.h" // game_T, game_init

int main(int argc, char** argv) {

  char *plugin_dir = "/home/tyler/.local/lib/gym/games";

  dict_T registry = dict_new();
  load_plugins(registry, plugin_dir);

  game_T game = game_init(registry, "sample");

  // TODO: free registry
  dict_free(&registry, (void (*)(void *)) entry_free);

  game->play();

  // TODO: free game

}

