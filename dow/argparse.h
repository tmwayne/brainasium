// 
// -----------------------------------------------------------------------------
// argparse.h
// -----------------------------------------------------------------------------
//
// Argument parser for Day-Of-Week
//

#include <argp.h>
#include <string.h> // strcmp

#include <dict.h>   // dict_T, dict_set

#define CONFIG_SET(key, val) dict_set(configs, (key), (void *) (val))

enum dow_config {
  FORMAT_DEFAULT = 0,
  FORMAT_DAY = 1,
  FORMAT_YEAR = 2
};

const char *argp_program_version = "0.0.1";
const char *argp_program_bug_address = "<tylerwayne3@gmail.com>";

static struct argp_option options[] = {
  {"format", 'f', "format", 0, "Format of exercise: day, year, default"},
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {

  dict_T configs = state->input;

  switch (key) {

    case 'f': 
      if (!strcmp(arg, "day")) CONFIG_SET("format", FORMAT_DAY);
      else if (!strcmp(arg, "year")) CONFIG_SET("format", FORMAT_YEAR);
      else fprintf(stderr, "Format not recognized. Using default...\n");
      break;

    // Don't accept key word arguments
    
    default: return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

// static char args_doc[] = "";
static char doc[] = "dow -- figure out day-of-week for a given date";
static struct argp argp = { options, parse_opt, NULL, doc };

#undef CONFIG_SET
