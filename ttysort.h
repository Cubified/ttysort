/*
 * ttysort.h: core functions
 */

#ifndef __TTYSORT_H
#define __TTYSORT_H

#define LENGTH(x) (sizeof(x)/sizeof(x[0]))
#define FOREACH(arr, start) int i;for(i=start;i<LENGTH(arr);i++)

#define ARR_MAX 10

#define BASE_X ((tb_width() - ARR_MAX) / 2)
#define BASE_Y ((tb_height() - ARR_LEN) / 2)

#define center(s) ((tb_width() - len(s)) / 2)
#define MSG "Press any key to exit."

enum {
  state_normal = TB_WHITE,
  state_swapped = TB_BLUE,
  state_pivot = TB_RED,
  state_sorted = TB_GREEN
};

typedef struct elem {
  int val;
  int state;
} elem;

int running;
int gradient;
elem *arr;
elem *arr_sorted;

/*
 * Miscellany
 */

int len(char *s){
  int o;
  char *c;
  for(c=s,o=0;*c;c++,o++){}
  return o;
}

void append(char *s, char c){
  s = realloc(s, len(s) + 2);
  sprintf(s, "%s%c", s, c);
}

char *stralloc(char *s){
  char *o = malloc(len(s) + 1);
  sprintf(o, "%s", s);
  return o;
}

void delchar(char *s){
  s[len(s)-1] = '\0';
}

/*
 * Array utilities
 */

void generate_arr(elem **out, int n, int m){
  int i;

  *out = malloc(n * sizeof(elem));

  srand(time(NULL));

  for(i=0;i<n;i++){
    out[0][i].val = (rand() % m) + 1;
    out[0][i].state = state_normal;
  }
}

/*
 * Out and in are in the wrong order, not
 * fixing because I'm just amazed at the
 * fact I never caught this
 */
void copy_arr(elem **out, elem **in, int n){
  int i;

  *in = malloc(n * sizeof(elem));

  for(i=0;i<n;i++){
    in[0][i] = out[0][i];
  }
}

/*
 * (Bubblesort due to ease of
 * implementation)
 */
void sort_arr(elem *a, int n){
  int s = 1,
      j = 0,
      i, t;
  while(s){
    s = 0;
    j++;
    for(i=0;i<n-j;i++){
      if(a[i].val > a[i+1].val){
        t = a[i].val;
        a[i].val = a[i+1].val;
        a[i+1].val = t;
        s = 1;
      }
    }
  }
}

/*
 * Drawing utilities
 */

void tb_putchar(int x, int y, int fg, int bg, char ch){
  struct tb_cell c;
  c.ch = (uint32_t)ch;
  c.fg = (uint32_t)fg;
  c.bg = (uint32_t)bg;

  tb_put_cell(x, y, &c);
}

void tb_printf(int x, int y, int fg, int bg, char *str){
  char *c;
  int i = 0;
  for(c=str;*c;c++){
    tb_putchar(x+i, y, fg, bg, *c);
    i++;
  }
}

void display_arr(elem *arr, int n, int p1, int p2, int y_off){
  int i, c, bg;
  for(i=0;i<n;i++){
    if(gradient){
      bg = (int)((((float)arr[i].val / (float)ARR_MAX) * 7));
      if(bg < 2){
        bg = TB_RED;
      }
    } else {
      if(arr[i].val == arr_sorted[i].val &&
         arr[i].state == state_normal){
        arr[i].state = state_sorted;
      }
      bg = arr[i].state;
    }
    for(c=0;c<arr[i].val;c++){
      tb_putchar(
        BASE_X + c, BASE_Y + i + y_off,
        TB_DEFAULT,
        bg,
        ' '
      );
    }
    for(;c<10;c++){
      tb_putchar(
        BASE_X + c, BASE_Y + i + y_off,
        TB_WHITE,
        TB_DEFAULT,
        ' '
      );
    }

    if(i == p1 ||
       i == p2){
      tb_putchar(
        BASE_X - 2, BASE_Y + i + y_off,
        TB_WHITE,
        TB_YELLOW,
        ' '
      );
    } else {
      tb_putchar(
        BASE_X - 2, BASE_Y + i + y_off,
        TB_WHITE,
        TB_DEFAULT,
        ' '
      );
    }

    arr[i].state = state_normal;
  }

  tb_present();
}

void draw_rect(int padding, int color, int base_y, int h){
  int halfpadding = (padding / 2),
      x, y;

  for(y=base_y-halfpadding;
      y<base_y+h+halfpadding;
      y++){
    if(y == base_y-halfpadding || y == base_y+h+halfpadding-1){
      for(x=BASE_X-padding;
          x<BASE_X+ARR_MAX+padding;
          x++){
        tb_putchar(x, y, TB_DEFAULT, color, ' ');
      }
    } else {
      tb_putchar(BASE_X-padding, y, TB_DEFAULT, color, ' ');
      tb_putchar(BASE_X+ARR_MAX+padding-1, y, TB_DEFAULT, color, ' ');
    }
  }
}

/*
 * Core/misc utilities
 */

void init_arr(){
  generate_arr(&arr, ARR_LEN, ARR_MAX);
  copy_arr(&arr, &arr_sorted, ARR_LEN);
  sort_arr(arr_sorted, ARR_LEN);
}

void init(){
  running = 1;
  tb_init();
  init_arr();
}

void shutdown(){
  free(arr);
  free(arr_sorted);
  tb_shutdown();
}

void wait_for_key(){
  struct tb_event evt;
  tb_poll_event(&evt);
}

void set_gradient(int g){
  gradient = g;
}

void set_run_speed(char *s){
  char *end;
  RUN_SPEED = ((float)(10/(float)strtol(s, &end, 10)) * 100000);
  if(*end){
    RUN_SPEED = 100000;
  }
}

void set_dataset_size(char *s){
  char *end;
  ARR_LEN = strtol(s, &end, 10);
  if(*end){
    ARR_LEN = 20;
  }

  free(arr);
  free(arr_sorted);

  init_arr();
}

void run(func algo, char *name_pretty){
  if(algo != shutdown){
    tb_clear();

    draw_rect(8, TB_RED, BASE_Y, ARR_LEN);

    tb_printf(center(name_pretty), BASE_Y - 2, TB_WHITE|TB_BOLD, TB_DEFAULT, name_pretty);

    algo(arr, ARR_LEN, display_arr);

    draw_rect(8, TB_GREEN, BASE_Y, ARR_LEN);

    tb_printf(center(MSG), BASE_Y + ARR_LEN + 1, TB_WHITE, TB_DEFAULT, MSG);
    tb_present();

    free(arr);
    free(arr_sorted);
    init_arr();

    wait_for_key();

    tb_clear();
  } else {
    running = 0;
    shutdown();
  }
}

#endif
