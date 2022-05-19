#include "input_func.h"
int input_int (int *num) {
    int a = 0;
    do {
        a = scanf ("%d", num);
        if (a < 0) {
            return 0;
        }
        
        if (a == 0) {
            scanf("%*c");
        }
    } while (a == 0);
    return 1;
}
