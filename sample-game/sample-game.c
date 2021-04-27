//
// -----------------------------------------------------------------------------
// sample-game.c
// -----------------------------------------------------------------------------
//
// Sample game for building out the backend for game
//

#include <stdio.h>     // printf, getchar
#include "game.h"      // game_T, game_new, 
#include "registry.h"  // register_game, entry_T, entry_new
#include <error.h> // assert

void play(int argc, char **argv) {

  printf("Press Enter: ");
  while (getchar()) break;
  printf("You win!\n");

}

void sample_game_free(){};

game_T sample_game_init() {

  game_T game = game_new();

  game->play = play;
  game->free = sample_game_free;
  game->args = NULL;

  return game;

}

void register_game(dict_T registry, char *plugin_path) {

  assert(registry && plugin_path);

  entry_T entry = entry_new(plugin_path, (void *(*)()) sample_game_init);
  dict_set(registry, "sample", entry);

}  
