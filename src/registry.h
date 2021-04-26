// 
// -----------------------------------------------------------------------------
// registry.h
// -----------------------------------------------------------------------------
//
// TODO: rewrite this summary
// Registry for maintain list of plugins available. Plugins in the proper
// directory with the proper file extension will be loaded and its
// register_plugin function will be run. This will cause an entry to be added
// to a dictionary. The entry will hold the name of the interface type, 
// the path the plugin, and the initialization function to run if that
// interface is selected in the configurations.
//
// Tyler Wayne © 2020
//

#ifndef REGISTRY_INCLUDED
#define REGISTRY_INCLUDED

#include <dict.h>

#define E entry_T
typedef struct E {
  char *plugin_path;
  void *(*init)();
} *E;

extern E      entry_new(char *plugin_dir, void *(*init)());
extern void   entry_free(E);
extern void   load_plugins(dict_T registry, char *plugin_dir);
extern void   register_plugin(dict_T registry, char *plugin_path);

#undef E
#endif
