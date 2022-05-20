#ifndef table_h
#define table_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct item {
    char *key;
    int len_key;
    int offset_info;
    int len_info;
    int ind;
    int busy;
}item;
typedef struct table {
    item *items;
    char *name_of_info_file;
    int len_of_file;
    int msize;
    int csize;
}table;
long long hash_function (char *key);
int solve_index (char *key, int size);
int search (table table, char* key);
int insert (table *table, char *key, char *info);
int delete (table *table, char *key);
void free_table (table table);
void save_table_to_file (table table);
void load_table_from_file (table *table);
#endif
