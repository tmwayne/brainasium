// 
// -----------------------------------------------------------------------------
// argparse.h
// -----------------------------------------------------------------------------
//
// Argument parser for Flashcards
//

#include <argp.h>
#include <stdlib.h> // atol
#include <dict.h>   // dict_T, dict_set

#define config_set(key,val) dict_set(configs, (key), (void *) (val))

enum ops {
  DEFAULT = 0,
  ADD = 1,
  SUB = 2,
  MUL = 3,
  DIV = 4
};

const char *argp_program_version = "0.0.1";
const char *argp_program_bug_address = "<tylerwayne3@gmail.com>";

static struct argp_option options[] = {
  {"digits", 'd', "d", 0, "Number of digits in each number"},
  {"nums", 'n', "n", 0, "Number of numbers on which to operate"},
  {"reverse", 'r', 0, 0, "Prompt the reverse side of the cards"},
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

  dict_T configs = state->input;
  long op;

  switch (key) {

    case 'd': config_set("digits", atol(arg));  break;
    case 'n': config_set("nums", atol(arg));    break;

    case ARGP_KEY_ARG:
      if (state->arg_num <= 1) {
        if      (!strcmp(arg, "add")) op = ADD;
        else if (!strcmp(arg, "sub")) op = SUB;
        else if (!strcmp(arg, "mul")) op = MUL;
        else if (!strcmp(arg, "div")) op = DIV;
        else {
          fprintf(stderr, "Operation not recognized...\n"
            "You can try add, sub, mul, or div\n\n");
          argp_usage(state);
        }
        config_set("op", op);
      }
      break;

    case ARGP_KEY_END: 
      // if (state->arg_num < 1) config_set("op", DEFAULT);
      if (state->arg_num > 1) argp_usage(state);
    default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static char args_doc[] = "operation";
static char doc[] = "abacus -- calculate your heart out";
static struct argp argp = { options, parse_opt, args_doc, doc };
