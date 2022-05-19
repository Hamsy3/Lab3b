#include "menu.h"
#include "table.h"
#include "input_func.h"
#include "readline/readline.h"
int choice_input() {
    int choice_input = 0;
    printf("Do you want to load table from file?\n");
    printf ("1. Yes\n");
    printf ("2. No\n");
    scanf ("%d", &choice_input);
    return choice_input;
}
int menu_input() {
  int choice = 0;
    choice = choice_input();
  if (choice<1 || choice>2) {
      do {
          choice = choice_input();
      } while (choice<1 || choice>2);
  }
  return choice;
}
int choice_action() {
    int choice_action = 0;
    printf("Please, choose action\n");
    printf ("0. Exit\n");
    printf ("1. Insert element\n");
    printf ("2. Delete element\n");
    printf ("3. Search element\n");
    printf ("4. Print table\n");
    scanf ("%d", &choice_action);
    return choice_action;
}
int menu_action() {
  int choice = 0;
    choice = choice_action();
  if (choice<0 || choice>4) {
      do {
          choice = choice_action();
      } while (choice<0 || choice>4);
  }
  return choice;
}
int menu() {
    int choice = 0;
    char *key = NULL;
    char *info = NULL;
    table table;
    table.msize = 0;
    table.csize = 0;
    do {
        printf("Please write size of vector\n");
        if (input_int(&table.msize) == 0) {
            printf("EOF\n");
            return 1;
        }
    } while (table.msize <=0);
    table.items = calloc(table.msize, sizeof(item));
    do {
    choice = menu_action();
    switch (choice) {
        case 0:
            break;
        case 1:
            printf("Write a key\n");
            key = readline(NULL);
            printf("Write info\n");
            info = readline(NULL);
            int inserted = insert(&table, key, info);
            if (inserted == -1) {
                printf("Duplicate keys\n");
            } else if (inserted == -2) {
                printf ("All positions are busy\n");
            }
            break;
        case 2:
            printf("Write a key\n");
            key = readline(NULL);
            if (delete(&table, key) == -1) {
                printf("Element not found\n");
            }
            break;
        case 3:
            printf("Write a key\n");
            key = readline(NULL);
            int searched = search(table, key);
            if (searched == -1) {
                printf("Key not found\n");
            } else {
                printf ("The index of element: %d\n", searched);
                printf("Info: %s\n",table.items[searched].info);
            }
            break;
        case 4:
            for (int i = 0; i<table.msize; ++i) {
                printf("%d %s %s", table.items[i].busy, table.items[i].key, table.items[i].info);
                printf("\n");
            }
            break;

        default:
            break;
    }
    } while (choice != 0);
    free_table(table);
    free(table.items);
    return 0;
}
