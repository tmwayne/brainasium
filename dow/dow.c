//
// -----------------------------------------------------------------------------
// doomsday.c
// -----------------------------------------------------------------------------
//
// Generate values for practicing the Day of the Week calculations
// Doomsday algorithm courtesy of John Conway.
//
// Tyler Wayne © 2021
//

#include <stdio.h>      // printf
#include <stdlib.h>     // rand, srand, RAND_MAX
#include <time.h>       // time, clock, CLOCKS_PER_SEC
#include <error.h>      // assert

#include "registry.h"   // entry_new
#include "game.h"       // game_new

#define YEAR_MIN 1700
#define YEAR_MAX 2100

static int anchor_day[4] = { 2, 0, 5, 3 };

int random_year() {

  int year = YEAR_MIN + rand() / (RAND_MAX / (YEAR_MAX-YEAR_MIN));
  return year;

}

int doomsday(int year) {

  int century = year / 100;
  year %= 100;

  int a = year / 12;
  int b = year % 12;
  int c = (year % 12) / 4;
  int d = anchor_day[century % 4];

  return (a + b + c + d) % 7;

}

double play(int argc, char **argv) {

  srand(time(NULL)); // set random seed

  int year = random_year();
  int answer = doomsday(year);

  char *prompt = "What is the day of the week for July 4th, %d? ";
  printf(prompt, year);

  char c;
  while (c = getchar()) break;

  int score = (c == answer + '0');

  if (score) printf("You got it!\n");
  else printf("Nope, it's %d\n", answer);

  return score;

}

// interface to Gym routine ----------------------------------------------------

game_T dow_init() {

  game_T game = game_new();

  game->play = play;
  game->args = NULL;

  return game;

}

void register_game(dict_T registry, char *plugin_path) {

  assert(registry && plugin_path);

  entry_T entry = entry_new(plugin_path, (void *(*)()) dow_init);
  dict_set(registry, "dow", entry);

}  
