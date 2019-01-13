/*
 * config.h: algorithm configuration
 */

#define VER "0.1.0"

int RUN_SPEED = 100000;

typedef void (*func)();

static void bubblesort();
static void quicksort_wrapper();
static void selectionsort();
static void shellsort();
static void cocktailshakersort();
static void gnomesort();
static void combsort();
static void countingsort();
static void gravitysort();
static void bogosort();

static void shutdown();

func options[] = {
  bubblesort,
  quicksort_wrapper,
  selectionsort,
  shellsort,
  cocktailshakersort,
  gnomesort,
  combsort,
  countingsort,
/*  gravitysort, */
  bogosort,
  NULL,
  shutdown
};

char *option_names_pretty[] = {
  "Bubblesort",
  "Quicksort",
  "Selection Sort",
  "Shellsort",
  "Cocktail Shaker Sort",
  "Gnome Sort",
  "Comb Sort",
  "Counting Sort",
/*  "Gravity Sort", */
  "Bogosort",
  "--------",
  "Quit"
};
