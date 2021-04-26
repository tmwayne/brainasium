// 
// -----------------------------------------------------------------------------
// game.h
// -----------------------------------------------------------------------------
//
// Backend abstract base class for Biblio
//

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "dict.h"

#define G game_T
typedef struct G *G;

struct G {
  void (*play)();
  void (*free)(void *);
  void *plugin_handle;
  void *args;
};

extern G    game_new();
extern G    game_init(dict_T, char *type);
extern void game_free(G *);

#undef B
#endif
