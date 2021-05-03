//
// -----------------------------------------------------------------------------
// abacus.c
// -----------------------------------------------------------------------------
//
// Generate random numbers for practicing abacus calculations
//
// Tyler Wayne © 2021
//

// TODO: add argopts
// TODO: have digits be an optional flag

#include <stdio.h>
#include <stdlib.h>       // srand, rand, abs, EXIT_FAILURE
#include <time.h>         // time
#include <math.h>         // pow, fmax, fmin
#include <string.h>       // strcmp

#include <error.h>        // assert
#include <configparse.h>  // configparse
#include <dict.h>         // dict_T, dict_new, dict_get
#include "exercise.h"
#include "registry.h"

#define EXERCISE_GAME "abacus"

enum ops { ADD, SUB, MUL, DIV };

int select_op(char *input) {
  
  if (strcmp(input, "add") == 0) return ADD;
  else if (strcmp(input, "sub") == 0) return SUB;
  else if (strcmp(input, "mul") == 0) return MUL;
  else if (strcmp(input, "div") == 0) return DIV;
  else return -1;

}

int ndigits(int x) {

  return floor(log10(x)) + 1;

}

char *pretty_int(char *str, size_t n, int x) {

  int len = ndigits(x); 
  len += (len-1)/3; // add space for commas

  if (!str) {
    str = calloc(len+1, sizeof(char));
    n = len;
  }

  if (n < len) {
    fprintf(stderr, "%s:%d:%s Buffer overflow...\n",
      __FILE__, __LINE__, __func__);
    exit(EXIT_FAILURE);
  }

  for (int i=len-1, j=1; i>=0; i--, j++) {
    str[i] = (x % 10) + '0';
    x /= 10;
    if (j % 3 == 0) str[--i] = ',';
  }

  str[len] = '\0';

  return str;

}

double add(int digits, int n) {

  int answer = 0;

  printf("(+)\n\n");

  for (int i=0; i<n; i++) {
    int tmp = rand() % (int) pow(10, digits);
    answer += tmp;

    char buf[14] = {0};
    pretty_int(buf, 13, tmp);
    printf("%*s\n\n", 13, buf);
  }

  return answer;

}

double sub(int digits, int n) {

  if (digits < 2) digits = 2;
  char buf[14];

  int answer = rand() % (int) pow(10, digits+1);
  pretty_int(buf, 13, answer);
  printf("(-)\n\n");
  printf("%*s\n\n", 13, buf);

  for (int i=1; i<n; i++) {
    int tmp = rand() % (int) pow(10, digits);
    answer -= tmp;
    pretty_int(buf, 13, tmp);
    printf("%*s\n\n", 13, buf);
  }

  return answer;

}

double mul(int digits) {

  int x = rand() % (int) pow(10, digits);
  int y = rand() % (int) pow(10, digits);

  printf("%d * %d ?\n", x, y);

  return x * y;

}

double divide(int digits) {

  double x = rand() % (int) pow(10, digits);
  double y = rand() % (int) pow(10, digits);

  printf("%g / %g ?\n", x, y);

  return x / y;

}

double play(int argc, char **argv) {

  // Default configurations
  int add_digits = 7;
  int add_nums = 4;
  int sub_digits = 7;
  int sub_nums = 4;

  // User-configurations
  dict_T configs = dict_new();
  FILE *fd = fopen("/home/tyler/.config/abacusrc", "r");
  if (fd) {
    configparse(configs, fd);
    char *tmp;
    if ((tmp = dict_get(configs, "add_digits"))) add_digits = atoi(tmp);
    if ((tmp = dict_get(configs, "add_nums"))) add_nums = atoi(tmp);
    if ((tmp = dict_get(configs, "sub_digits"))) sub_digits = atoi(tmp);
    if ((tmp = dict_get(configs, "sub_nums"))) sub_nums = atoi(tmp);
  }

  // Command-line arguments
  int op;
  unsigned int seed = time(NULL);
  srand(seed);

  if (argc == 1) {
    int rnd = rand() % 4;
    switch (rnd) {
      case 0: op = ADD; break;
      case 1: op = SUB; break;
      case 2: op = MUL; break;
      case 3: op = DIV; break;
    }
  }

  else if (argc > 2) {
    fprintf(stderr, "Usage: %s op\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  else if ((op = select_op(argv[1])) < 0) {
    fprintf(stderr, "Invalid selection. Options are add, sub, mul, div\n");
    exit(EXIT_FAILURE);
  }
  
  // TODO: refactor this and turn into a function
  double answer;
  switch (op) {
    case ADD: answer = add(add_digits, add_nums); break;
    case SUB: answer = sub(sub_digits, sub_nums); break;
    case MUL: answer = mul(3);    break;
    case DIV: answer = divide(2); break;
  }

  
  double guess = 0;

  // TODO: read in whole line then use sscanf to parse number
  scanf("%lf", &guess);

  if (guess == answer) {
    printf("You got it!\n");
    return 1;
  }  else {
    printf("Nope, it's %.*f\n", op == DIV ? 4 : 0, answer);
    return 0;
  }

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
  dict_set(registry, EXERCISE_GAME, entry);

}  
