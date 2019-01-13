/*
 * config.h: algorithm configuration
 */

/*
 * TODO?: Take user args
 * or menu option
 */
#define RUN_SPEED 100000
#define VER "0.1.0"

typedef void (*func)();

static void bubblesort();
static void quicksort_wrapper();
static void selectionsort();
static void shellsort();
static void cocktailshakersort();
static void gnomesort();
static void combsort();
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
  "Bogosort",
  "--------",
  "Quit"
};
