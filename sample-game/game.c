//
// -----------------------------------------------------------------------------
// game.c
// -----------------------------------------------------------------------------
//
// Sample game for building out the backend for game
//

#include <stdio.h> // printf, getchar

int game() {

  printf("Press Enter: ");
  while (getchar()) break;
  printf("You win!\n");

}
