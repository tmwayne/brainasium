// 
// -----------------------------------------------------------------------------
// argparse.h
// -----------------------------------------------------------------------------
//
// Argument parser for Day-Of-Week
//

#include <argp.h>
#include <dict.h>   // dict_T, dict_set

const char *argp_program_version = "0.0.1";
const char *argp_program_bug_address = "<tylerwayne3@gmail.com>";

static struct argp_option options[] = {
  {"format", 'f', "format", 0, "Format of exercise: day, year, default"},
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

  dict_T configs = state->input;

  switch (key) {
    case 'f': dict_set(configs, "format", arg); break;

    // case ARGP_KEY_ARG:
      // if (state->arg_num <= 1) dict_set(configs, "file", arg);
      // break;

    // case ARGP_KEY_END: if (state->arg_num > 0) argp_usage(state); break;
    default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

// static char args_doc[] = "";
static char doc[] = "dow -- figure out the day-of-week for a given date";
// static struct argp argp = { options, parse_opt, args_doc, doc };
static struct argp argp = { options, parse_opt, NULL, doc };
