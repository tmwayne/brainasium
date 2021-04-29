//
// -----------------------------------------------------------------------------
// exercise.c
// -----------------------------------------------------------------------------
//
// Create and initialize Exercise for the Gym
//

#include <stdio.h>    // fprintf
#include <stdlib.h>
#include <dlfcn.h>    // dlopen, dlclose

#include <mem.h>      // NEW0
#include <error.h>    // assert
#include "registry.h"
#include "exercise.h"

exercise_T exercise_new() {
  
  exercise_T exercise;
  NEW0(exercise);
  return exercise;

}

exercise_T exercise_init(dict_T registry, char *type) {

  void *dlhandle;
  exercise_T (*init)();
  exercise_T exercise;

  assert(registry && type);

  entry_T entry;
  if (!(entry = dict_get(registry, type))) {
    fprintf(stderr, "Failed to find exercise of type %s\n", type);
    exit(EXIT_FAILURE);
  }
    
  // Open plugin dynamic library if one has been provided
  dlhandle = entry->plugin_path ? dlopen(entry->plugin_path, RTLD_NOW) : NULL;

  // TODO: check if a valid entry can be made without a plugin_path
  // if (!dlhandle) {
    // fprintf(stderr, "%s\n", dlerror());
    // exit(exit_failure);
  // }
  
  init = (exercise_T (*)()) entry->init;
  exercise = init();
  exercise->plugin_handle = dlhandle;
  
  return exercise;

}

void exercise_free(exercise_T *exercise) {
  
  assert(exercise && *exercise);

  // (*exercise)->free((*exercise)->args);
  if ((*exercise)->plugin_handle) dlclose((*exercise)->plugin_handle);

  FREE(*exercise);

}
