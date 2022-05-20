#include "table.h"
#include "readline/readline.h"
#include <string.h>
long long hash_function (char *key) {
    int multiply = 59;
    long long hash = 0;
    int multiply_pow = 1;
    for (int i = 0; i < strlen(key); ++i ) {
        hash += key[i] * multiply_pow;
        multiply_pow *= multiply;
    }
    return hash;
}
int solve_index (char *key, int size) {
    int index = (hash_function(key)) % size;
    return index;
}
int search (table table, char *key) {
    int cur = solve_index(key, table.msize);
    for (int checked = 0; (table.items[cur].busy != 0) && (checked <= (table.msize-1)); ++checked) {
        if (strcmp (table.items[cur].key, key) == 0) {
            return cur;
        }
        cur = ((cur+1) % table.msize);
    }
    return -1;
}
int insert (table *table, char *key, char *info) {
    int checked = 0;
    int cur = solve_index(key, table->msize);
    while ((table->items[cur].busy == 1) && (checked < table->msize)) {
        if (strcmp (table->items[cur].key, key) == 0) {
            return -1;
        }
        cur = ((cur+1) % table->msize);
        ++checked;
    }
    if (checked < table->msize) {
        table->items[cur].key = key;
        table->items[cur].busy = 1;
        table->items[cur].ind = cur;
        (table->csize++);
        if (table->name_of_info_file != NULL) {
            FILE *fd = NULL;
            fd = fopen(table->name_of_info_file, "r+b");
            fseek(fd, 0L, SEEK_END);
            table->items[cur].offset_info = (int) ftell(fd);
            table->items[cur].len_info = (int)(strlen(info)+1);
            fwrite(info, sizeof(char), table->items[cur].len_info, fd);
            fclose(fd);
        } else {
            FILE *fd = NULL;
            printf("Please, write a info file name\n");
            table->name_of_info_file = readline(NULL);
            table->len_of_file = (int) (strlen(table->name_of_info_file)+1);
            fd = fopen(table->name_of_info_file, "w+b");
            fseek(fd, 0L, SEEK_END);
            table->items[cur].offset_info = (int) ftell(fd);
            table->items[cur].len_info = (int)(strlen(info)+1);
            fwrite(info, sizeof(char), table->items[cur].len_info, fd);
            fclose(fd);
        }
        return 0;
    }
    return -2;
}
int delete (table *table, char *key) {
    int searched = search (*table, key);
    if (searched != -1) {
        table->items[searched].busy = 0;
        free(table->items[searched].key);
        (table->csize--);
        return 0;
    }
    return -1;
}
void free_table (table table) {
    for (int i = 0; i < table.msize; ++i) {
        if (table.items[i].busy == 1) {
            free(table.items[i].key);
            if (table.name_of_info_file != NULL) {
                free(table.name_of_info_file);
            }
        }
    }
}
void load_table_from_file (table *table) {
    FILE *fd = NULL;
    char *name = NULL;
    printf("Please, write name of the file for loading\n");
    name = readline(NULL);
    int ind = 0;
    fd = fopen(name, "r+b");
    if (fd != NULL) {
        fread(&table->msize, sizeof(int), 1, fd);
        table->items = calloc(table->msize, sizeof(item));
        fread(&table->csize, sizeof(int), 1, fd);
        if (table->csize != 0) {
            fread(&table->len_of_file, sizeof(int), 1, fd);
            table->name_of_info_file = calloc(table->len_of_file, sizeof(char));
            fread(table->name_of_info_file, sizeof(char), table->len_of_file, fd);
        }
        for (int i = 0; i < table->csize; ++i) {
            fread(&table->items[i].len_key, sizeof(int), 1, fd);
            fread(&ind, sizeof(int), 1, fd);
            fread(&table->items[ind].len_info, sizeof(int), 1, fd);
            fread(&table->items[ind].offset_info, sizeof(int), 1, fd);
            table->items[ind].key = (char*) calloc(table->items[i].len_key, sizeof(char));
            fread(table->items[ind].key, sizeof(char), table->items[i].len_key, fd);
            table->items[ind].busy = 1;
        }
    } else {
        table->msize = 1;
        fd = fopen(name, "w+b");
        fwrite(&table->msize, sizeof(int), 1, fd);
        table->items = calloc(table->msize, sizeof(item));
        table->csize = 0;
    }
    fclose(fd);
    free(name);
}
void save_table_to_file (table table) {
    FILE *fd = NULL;
    char *name;
    printf("Please, write name of the file for saving\n");
    name = readline(NULL);
    fd = fopen(name, "w+b");
    fwrite(&table.msize, sizeof(int), 1, fd);
    fwrite(&table.csize, sizeof(int), 1, fd);
    if (table.csize != 0) {
        fwrite(&table.len_of_file, sizeof(int), 1, fd);
        fwrite(table.name_of_info_file, sizeof(char), table.len_of_file, fd);
    }
    for (int i = 0; i< table.msize; ++i) {
        if (table.items[i].busy == 1) {
            table.items[i].len_key =(int) (strlen(table.items[i].key)+1);
            fwrite(&table.items[i].len_key, sizeof(int), 1, fd);
            fwrite(&table.items[i].ind, sizeof(int), 1, fd);
            fwrite(&table.items[i].len_info, sizeof(int), 1, fd);
            fwrite(&table.items[i].offset_info, sizeof(int), 1, fd);
            fwrite(table.items[i].key, sizeof(char), (strlen(table.items[i].key)+1), fd);
        }
    }

    fclose(fd);
    free(name);
}
