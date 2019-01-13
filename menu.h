/*
 * menu.h: simple termbox menu
 *         (works, but feels
 *          imperfect/messy)
 */

#ifndef __MENU_H
#define __MENU_H

#define VALID_CHOICE(c) (m->items[c].action != NULL)

typedef struct menu_item {
  char *text;
  func action;
} menu_item;

typedef struct menu_toggle {
  char *text;
  char bind;
  int state;
  func action;
} menu_toggle;

typedef struct menu_input {
  char *text;
  char *val;
  char bind;
  int pos_x;
  int pos_y;
  int max_len;
  func action;
} menu_input;

typedef struct menu {
  char *title;
  int choice;
  int items_count;
  menu_item *items;
  int toggles_count;
  menu_toggle *toggles;
  int inputs_count;
  menu_input *inputs;
} menu;

/*
 * Menu "library" (almost
 * entirely standalone,
 * BASE_X/Y as well as
 * len() are defined
 * in ttysort.h
 */

void menu_disp(menu *m){
  char *s;
  int i, menu_height, c,
      pos_x, pos_y;

  draw_rect(8, TB_BLUE);
  
  tb_printf(center(m->title), BASE_Y - 2, TB_WHITE|TB_BOLD, TB_DEFAULT, m->title);

  for(i=0;i<m->items_count;i++){
    if(m->choice == i){
      tb_printf(center(m->items[i].text), BASE_Y+i, TB_BLACK, TB_WHITE, m->items[i].text);
    } else {
      tb_printf(center(m->items[i].text), BASE_Y+i, TB_WHITE, TB_DEFAULT, m->items[i].text);
    }
  }

  menu_height = i + 1;

  for(i=0;i<m->toggles_count;i++){
    s = malloc(len(m->toggles[i].text) + 5);
    sprintf(s, "%s (%c)", m->toggles[i].text, m->toggles[i].bind);
    if(m->toggles[i].state){
      tb_printf(center(s), BASE_Y+menu_height+i, TB_BLACK, TB_WHITE, s);
    } else {
      tb_printf(center(s), BASE_Y+menu_height+i, TB_WHITE, TB_DEFAULT, s);
    }
    free(s);
  }

  menu_height += i + 1;

  for(i=0;i<m->inputs_count;i++){
    pos_x = ((tb_width() - m->inputs[i].max_len) / 2);
    pos_y = BASE_Y+menu_height+(i*3)+1;

    for(c=0;c<=m->inputs[i].max_len;c++){
      tb_putchar(pos_x+c, pos_y, TB_BLACK, TB_WHITE, ' ');
    }

    s = malloc(len(m->inputs[i].text) + 6);
    sprintf(s, "%s (%c):", m->inputs[i].text, m->inputs[i].bind);
    tb_printf(center(s), pos_y-1, TB_WHITE, TB_DEFAULT, s);
    tb_printf(pos_x, pos_y, TB_BLACK, TB_WHITE, m->inputs[i].val); 

    free(s);

    m->inputs[i].pos_x = pos_x;
    m->inputs[i].pos_y = pos_y;
  }

  tb_present();
}

void menu_input_focus(menu_input inp, menu *m){
  struct tb_event evt;

  tb_set_cursor(inp.pos_x+len(inp.val), inp.pos_y);
  tb_present();

  tb_poll_event(&evt);

  if((char)evt.ch >= '0' &&
     (char)evt.ch <= 'z' &&
     len(inp.val) < inp.max_len){
    append(inp.val, (char)evt.ch);
    menu_disp(m);
    menu_input_focus(inp, m);
  } else if(evt.key == TB_KEY_ARROW_LEFT){ /* TODO: Fix backspace */
    delchar(inp.val);
    menu_disp(m);
    menu_input_focus(inp, m);
  } else if(evt.key == TB_KEY_ENTER){
    tb_set_cursor(TB_HIDE_CURSOR, TB_HIDE_CURSOR);
    inp.action(inp.val);
  } else {
    printf("\a"); /* Bell */
    menu_input_focus(inp, m);
  }
}

void menu_loop(menu *m){
  int i;
  struct tb_event evt;
  tb_poll_event(&evt);

  switch(evt.key){
    case TB_KEY_ARROW_UP:
      if(m->choice > 0){
        m->choice--;
        while(!VALID_CHOICE(m->choice)){
          m->choice--;
        }
      }
      break;
    case TB_KEY_ARROW_DOWN:
      if(m->choice < m->items_count-1){
        m->choice++;
        while(!VALID_CHOICE(m->choice)){
          m->choice++;
        }
      }
      break;
    case TB_KEY_ENTER:
      run(m->items[m->choice].action, m->items[m->choice].text);
      break;
  }
  for(i=0;i<m->toggles_count;i++){
    if(evt.ch == (uint32_t)m->toggles[i].bind){
      m->toggles[i].state = !m->toggles[i].state;
      m->toggles[i].action(m->toggles[i].state);
    }
  }
  for(i=0;i<m->inputs_count;i++){
    if(evt.ch == (uint32_t)m->inputs[i].bind){
      menu_input_focus(m->inputs[i], m);
    }
  }

  if(running){
    menu_disp(m);
    menu_loop(m);
  }
}

void menu_add_item(char *text, func action, menu *m){
  m->items = realloc(m->items, (m->items_count+1) * sizeof(menu_item));
  m->items[m->items_count].text   = text;
  m->items[m->items_count].action = action;
  m->items_count++;
}

void menu_add_toggle(char *text, char bind, func action, menu *m){
  m->toggles = realloc(m->toggles, (m->toggles_count+1) * sizeof(menu_toggle));
  m->toggles[m->toggles_count].text   = text;
  m->toggles[m->toggles_count].bind   = bind;
  m->toggles[m->toggles_count].state  = 0;
  m->toggles[m->toggles_count].action = action;
  m->toggles_count++;
}

void menu_add_input(char *text, char *default_value, char bind, int max_len, func action, menu *m){
  m->inputs = realloc(m->inputs, (m->inputs_count+1) * sizeof(menu_input));
  m->inputs[m->inputs_count].text    = text;
  m->inputs[m->inputs_count].val     = default_value;
  m->inputs[m->inputs_count].bind    = bind;
  m->inputs[m->inputs_count].max_len = max_len;
  m->inputs[m->inputs_count].action  = action;
  m->inputs_count++;
}

void menu_init(menu **m){
  *m = malloc(sizeof(menu));
  (*m)->choice = 0;
  (*m)->items_count = 0;
  (*m)->items = malloc(sizeof(menu_item));
  (*m)->toggles_count = 0;
  (*m)->toggles = malloc(sizeof(menu_toggle));
  (*m)->inputs_count = 0;
  (*m)->inputs = malloc(sizeof(menu_input));
}

void menu_free(menu *m){
  int i;
  free(m->items);
  free(m->toggles);
  for(i=0;i<m->inputs_count;i++){
    free(m->inputs[i].val);
  }
  free(m->inputs);
  free(m);
}

/*
 * User-facing menu
 */

void menu_main(menu *m){
  m->title = "ttysort v" VER;

  FOREACH(options, 0){
    menu_add_item(option_names_pretty[i], options[i], m);
  }

  menu_add_toggle("Gradient", 'g', set_gradient, m);

  menu_add_input("Run Speed", stralloc("10"), 'r', 3, set_run_speed, m);

  /* menu_add_input("Dataset Size", stralloc("20"), 'd', 2, set_dataset_size, m); */

  menu_disp(m);
  
  menu_loop(m);
}

#endif
