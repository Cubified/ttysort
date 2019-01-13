/*
 * ttysort.c: a terminal-based sorting algorithm visualizer
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <termbox.h>

#include "config.h"
#include "ttysort.h"
#include "algos.h"
#include "menu.h"

menu *m;

int main(int argc, char **argv){
  gradient = 0;

  init();

  menu_init(&m);

  menu_main(m);

  menu_free(m);

  return 0;
}
