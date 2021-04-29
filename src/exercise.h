// 
// -----------------------------------------------------------------------------
// exercise.h
// -----------------------------------------------------------------------------
//
// Plugin data structure for registering new exercises
//

#ifndef EXERCISE_INCLUDED
#define EXERCISE_INCLUDED

#include <dict.h>

#define E exercise_T
typedef struct E *E;

struct E {
  double (*play)(int argc, char **argv);
  void *plugin_handle;
  void *args;
};

extern E    exercise_new();
extern E    exercise_init(dict_T, char *type);
extern void exercise_free(E *);

#undef E
#endif
