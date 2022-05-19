#include "table.h"
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
        table->items[cur].info = info;
        table->items[cur].busy = 1;
        table->items[cur].ind = cur;
        (table->csize++);
        return 0;
    }
    return -2;
}
int delete (table *table, char *key) {
    int searched = search (*table, key);
    if (searched != -1) {
        table->items[searched].busy = 0;
        (table->csize--);
        return 0;
    }
    return -1;
}
void free_table (table table) {
    for (int i = 0; i < table.msize; ++i) {
        free(table.items[i].key);
        free(table.items[i].info);
    }
}
/*int load_table_from_file (table *table) {
    FILE *fd = NULL;
    char *name;
    char t;
    scanf("%c", &t);
    name = get_str();
    int size_of_vector = 0;
    int size_of_table = 0;
    fd = fopen(name, "r+b");
    if (fd != NULL) {
        fread(&size_of_vector, sizeof(int), 1, fd);
        printf("%d", size_of_vector);
        printf("%d", size_of_table);
        table->items = calloc(table->msize, sizeof(item));
        fread(&size_of_table, sizeof(int), 1, fd);
        for (int i = 0; i < table->csize; ++i) {
            fread(table->items[i].key, sizeof(item), 1, fd);
        }
    } else {
        fd = fopen(name, "w+b");
        fwrite(&table->msize, sizeof(int), 1, fd);
    }
    fclose(fd);
    return 0;
}*/
