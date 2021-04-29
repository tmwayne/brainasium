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

#define EXERCISE_NAME "sample"

double play(int argc, char **argv) {

  printf("Press Enter: ");
  while (getchar()) break;
  printf("You win!\n");

  return 1;

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
