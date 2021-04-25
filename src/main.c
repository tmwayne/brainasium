//
// -----------------------------------------------------------------------------
// main.c
// -----------------------------------------------------------------------------
//
// Tyler Wayne © 2021
//

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "configparse.h"
#include "argparse.h"
#include "assert.h"

int main(int argc, char** argv) {

  /* ARGUMENTS */
  //char* ext = ".txt";
  // Configurations
  //struct configs configs;
  //config_parse("/home/tyler/.cheatsheetsrc", &configs);

  // Defaults
  struct arguments arguments;
  arguments.with = 0;
  arguments.without = 0;
  // arguments.cs_dir = getenv("CHEATSHEETS_DIR");
  // Don't know how long arguments.cs_dir is
  //arguments.cs_dir = configs.cs_dir;

  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  /* PROGRAM LOGIC */
  if (arguments.without)
    printf("Optional arg without value: %d\n", arguments.without); 
  
  if (arguments.with)
    printf("Optional arg with value: %s\n", arguments.with);

  printf("Positional arg: %s\n", arguments.args[0]);

}

