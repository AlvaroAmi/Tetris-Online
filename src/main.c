#include "menu.h"
#include <stdio.h>

#define MAX_LINE 10
#define MAX_LEN 50

int is_logged_in = 0; 

int main(int argc, char const *argv[]) {
    init_menus(is_logged_in);

    return 0;
}
