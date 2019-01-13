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

typedef struct menu {
  char *title;
  int choice;
  int items_count;
  menu_item *items;
  int toggles_count;
  menu_toggle *toggles;
} menu;

/*
 * Menu "library" (almost
 * entirely standalone,
 * BASE_X/Y are defined
 * in ttysort.h as well
 * as len()
 */

void menu_disp(menu *m){
  char *s;
  int i, menu_height;

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
      tb_printf(center(s), BASE_Y+menu_height, TB_BLACK, TB_WHITE, s);
    } else {
      tb_printf(center(s), BASE_Y+menu_height, TB_WHITE, TB_DEFAULT, s);
    }
    free(s);
  }

  tb_present();
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

  if(running){
    menu_disp(m);
    menu_loop(m);
  }
}

void menu_add_item(char *text, func action, menu *m){
  m->items = realloc(m->items, (m->items_count+1) * sizeof(menu_item));
  m->items[m->items_count].text = text;
  m->items[m->items_count].action = action;
  m->items_count++;
}

void menu_add_toggle(char *text, char bind, func action, menu *m){
  m->toggles = realloc(m->toggles, (m->toggles_count+1) * sizeof(menu_toggle));
  m->toggles[m->toggles_count].text = text;
  m->toggles[m->toggles_count].bind = bind;
  m->toggles[m->toggles_count].state = 0;
  m->toggles[m->toggles_count].action = action;
  m->toggles_count++;
}

void menu_init(menu **m){
  *m = malloc(sizeof(menu));
  (*m)->choice = 0;
  (*m)->items_count = 0;
  (*m)->items = malloc(sizeof(menu_item));
  (*m)->toggles_count = 0;
  (*m)->toggles = malloc(sizeof(menu_toggle));
}

void menu_free(menu *m){
  free(m->items);
  free(m->toggles);
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

  menu_disp(m);
  
  menu_loop(m);
}

#endif
