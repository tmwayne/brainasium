//
// -----------------------------------------------------------------------------
// dow.c
// -----------------------------------------------------------------------------
//
// Generate values for practicing the Day of the Week calculations
// Doomsday algorithm courtesy of John Conway.
//
// Tyler Wayne © 2021
//

#include <stdio.h>      // printf
#include <stdlib.h>     // rand, srand, RAND_MAX
#include <time.h>       // mktime, localtime
#include <strings.h>    // bzero

#include "argparse.h"   // argp_parse
#include <error.h>      // assert
#include "registry.h"   // entry_new
#include "exercise.h"   // exercise_new

#define EXERCISE_NAME "dow"

// TODO: set this is a configuration parameter
#define YEAR_MIN 1700
#define YEAR_MAX 2100

#define TM_YEAR_BASE 1900

static int anchor_day[4] = { 2, 0, 5, 3 };

static int mod(int a, int b) {
  
  int r = a % b;
  return r < 0 ? r + b : r;

}

static struct tm *rand_date() {

  srand(time(NULL));

  // TODO: confirm that this is the best way to randomly pick a year
  int day = rand() % 365; 
  int year = rand() % (YEAR_MAX-YEAR_MIN) + (YEAR_MIN-TM_YEAR_BASE);

  struct tm timeinfo;
  time_t loctime;

  bzero(&timeinfo, sizeof(struct tm));
  timeinfo.tm_isdst = 1; // allow mktime to determine DST setting
  timeinfo.tm_isdst = -1; // allow mktime to determine DST setting
  timeinfo.tm_mon = 0;
  timeinfo.tm_mday = day;
  timeinfo.tm_year = year;

  loctime = mktime(&timeinfo);

  return localtime(&loctime);

}

int from_year(int year) {

  int century = year / 100;
  year %= 100;

  int a = year / 12;
  int b = year % 12;
  int c = (year % 12) / 4;
  int d = anchor_day[century % 4];

  return (a + b + c + d) % 7;

}

double play(int argc, char **argv) {

  dict_T configs = dict_new();

  // Load command-line arguments
  argp_parse(&argp, argc, argv, 0, 0, configs);

  srand(time(NULL)); // set random seed

  struct tm *rnddate = rand_date();
  char date[100];
  strftime(date, 100, "%B %d %Y", rnddate); // date in format : March 03 1978

  int year_offset = from_year(rnddate->tm_year + TM_YEAR_BASE);

  int day_offset = mod(rnddate->tm_wday - year_offset, 7);
  int answer;

  switch((long) dict_get(configs, "format")) {
    
    case FORMAT_DAY:
      answer = day_offset;
      printf("What is the day offset for %s ? ", date);
      break;

    case FORMAT_YEAR:
      answer = year_offset;
      printf("What is the year offset for %d ? ", rnddate->tm_year + TM_YEAR_BASE);
      break;

    default:
      answer = rnddate->tm_wday;
      printf("What is the day of the week of %s ? ", date);
      break;
      
  };

  // char wday[10];
  // strftime(wday, 10, "%A", rnddate); // extract name of weekday

  char c;
  while (c = getchar()) break;

  int score = (c == answer + '0');

  if (score) printf("You got it!\n");
  // else printf("Nope, it's %s (%d)\n", wday, rnddate->tm_wday);
  else printf("Nope, it's %d\n", answer);

  return score;

}

// interface to Gym routine ----------------------------------------------------

exercise_T init() {

  exercise_T exercise = exercise_new();

  exercise->play = play;
  exercise->args = NULL;

  return exercise;

}

void add_to_registry(dict_T registry, char *plugin_path) {

  assert(registry && plugin_path);

  entry_T entry = entry_new(plugin_path, (void *(*)()) init);
  dict_set(registry, EXERCISE_NAME, entry);

}  
