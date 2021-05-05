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

const char *argp_program_version = "0.0.1";
const char *argp_program_bug_address = "<tylerwayne3@gmail.com>";

static struct argp_option options[] = {
  {"misses", 'm', "file", 0, "File to write misses to"},
  {"num-lines", 'n', "n", 0, "Number of questions to give"},
  {"num-guesses", 'g', "n", 0, "Number of guesses allowed"},
  {"reverse", 'r', 0, 0, "Prompt the reverse side of the cards"},
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

  dict_T configs = state->input;

  switch (key) {
    case 'r': dict_set(configs, "reverse", (void *) 1); break;
    case 'm': dict_set(configs, "fmisses", arg); break;

    // Note dict_set expects key to be (void *), we explicitly
    // cast from int to prevent compiler warnings
    case 'n': dict_set(configs, "nlines", (void *) atol(arg)); break;
    case 'g': dict_set(configs, "nguess", (void *) atol(arg)); break;

    case ARGP_KEY_ARG:
      if (state->arg_num <= 1) dict_set(configs, "file", arg);
      break;

    // TODO: allow for multiple input files to be given
    case ARGP_KEY_END: if (state->arg_num < 1) argp_usage(state); break;
    default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static char args_doc[] = "card-file";
static char doc[] = "flashcards -- improve memory and learn new topics";
static struct argp argp = { options, parse_opt, args_doc, doc };
