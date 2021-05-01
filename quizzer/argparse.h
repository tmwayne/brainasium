// 
// -----------------------------------------------------------------------------
// argparse.h
// -----------------------------------------------------------------------------
//
// Argument parser for quizzer
//

#include <argp.h>
#include <stdlib.h> // atol
#include <dict.h>   // dict_T, dict_set

const char *argp_program_version = "0.0.1";
const char *argp_program_bug_address = "<tylerwayne3@gmail.com>";

static struct argp_option options[] = {
  {"misses", 'm', "file", 0, "File to write misses to"},
  {"nlines", 'n', "num", 0, "Number of questions to give"},
  // {"without-value", 'b', 0, 0, "Optional argument without value"},
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

  dict_T configs = state->input;

  switch (key) {
    case 'm': dict_set(configs, "misses", arg); break;
    case 'n': dict_set(configs, "nlines", atol(arg)); break;

    // stop at the first positional arg and capture the remaining ones
    case ARGP_KEY_ARG:
      if (state->arg_num <= 1) dict_set(configs, "file", arg);
      break;

    // TODO: allow for multiple input files to be given
    case ARGP_KEY_END: if (state->arg_num < 1) argp_usage(state); break;
    default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static char args_doc[] = "quiz-file";
static char doc[] = "quizzer -- strengthen memory with flashcard style exercise";
static struct argp argp = { options, parse_opt, args_doc, doc };
