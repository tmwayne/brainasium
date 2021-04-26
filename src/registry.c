//
// -----------------------------------------------------------------------------
// registry.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2021
//

#include <stdio.h> // fprintf
#include <stdlib.h>
#include <dirent.h> // opendir
#include <limits.h> // PATH_MAX
#include <unistd.h> // getcwd, chdir
#include <dlfcn.h>  // dlopen, dlclose
#include <string.h> // strdup
#include <c-strings.h>
#include <dict.h>
#include <mem.h> // NEW, FREE
#include <error.h> // assert
#include "registry.h"

entry_T entry_new(char *plugin_path, void *(*init)()) {
  
  assert(plugin_path && init);

  entry_T entry;
  NEW(entry);
  entry->plugin_path = strdup(plugin_path);
  entry->init = init;

  return entry;

}

void entry_free(entry_T entry) {
  
    // assert(entry && *entry);
    assert(entry);

    FREE(entry->plugin_path);
    FREE(entry);

}

void load_plugins(dict_T registry, char *plugin_dir) {

  assert(registry && plugin_dir);

  // Save current working directory
  char cwd[PATH_MAX];
  getcwd(cwd, PATH_MAX);

  // Change to plugin directory
  if (0 != chdir(plugin_dir)) {
    fprintf(stderr, "Failed to find plugin directory\n");
    exit(EXIT_FAILURE);
  }

  DIR *d;
  struct dirent *dir;
  d = opendir(".");

  if (d) {

    while ((dir = readdir(d)) != NULL) {
      if (extmatch(dir->d_name, ".so")) {
        char *path = realpath(dir->d_name, NULL);
        register_plugin(registry, path);
      }
    }
    closedir(d);

  }
  chdir(cwd);

}

void register_plugin(dict_T registry, char *plugin_path) {

  assert(registry && plugin_path);
  
  void *dlhandle;
  char *error;

  dlhandle = dlopen(plugin_path, RTLD_NOW);
  if (!dlhandle) {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  dlerror(); // clear any existing error

  void (*register_game)(dict_T, char *) = dlsym(dlhandle, "register_game");

  if ((error = dlerror()) != NULL) {
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
  }

  register_game(registry, plugin_path);

  dlclose(dlhandle);

}
