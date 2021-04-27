//
// -----------------------------------------------------------------------------
// game.c
// -----------------------------------------------------------------------------
//
// Create and initialize Games for the Gym
//

#include <stdio.h>    // fprintf
#include <stdlib.h>
#include <dlfcn.h>    // dlopen, dlclose

#include <mem.h>      // NEW0
#include <error.h>    // assert
#include "registry.h"
#include "game.h"

game_T game_new() {
  
  game_T game;
  NEW0(game);
  return game;

}

game_T game_init(dict_T registry, char *type) {

  void *dlhandle;
  game_T (*init)();
  game_T game;

  assert(registry && type);

  entry_T entry;
  if (!(entry = dict_get(registry, type))) {
    fprintf(stderr, "Failed to find game of type %s\n", type);
    exit(EXIT_FAILURE);
  }
    
  // Open plugin dynamic library if one has been provided
  dlhandle = entry->plugin_path ? dlopen(entry->plugin_path, RTLD_NOW) : NULL;

  // TODO: check if a valid entry can be made without a plugin_path
  // if (!dlhandle) {
    // fprintf(stderr, "%s\n", dlerror());
    // exit(exit_failure);
  // }
  
  init = (game_T (*)()) entry->init;
  game = init();
  game->plugin_handle = dlhandle;
  
  return game;

}

void game_free(game_T *game) {
  
  assert(game && *game);

  // (*game)->free((*game)->args);
  if ((*game)->plugin_handle) dlclose((*game)->plugin_handle);

  FREE(*game);

}
