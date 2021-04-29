//
// -----------------------------------------------------------------------------
// sample-exercise.c
// -----------------------------------------------------------------------------
//
// Sample exercise for building out the backend for exercise
//

#include <stdio.h>         // printf, getchar

#include <error.h>         // assert
#include "exercise.h"      // exercise_T, exercise_new, 
#include "registry.h"      // add_to_registry, entry_T, entry_new

#define EXERCISE_NAME "sample"

double play(int argc, char **argv) {

  printf("Press Enter: ");
  while (getchar()) break;
  printf("You win!\n");

  return 1;

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
