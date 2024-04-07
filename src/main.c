#include "menu.h"
#include <stdio.h>

#define MAX_LINE 10
#define MAX_LEN 50

int is_logged_in = 0; 

int main(int argc, char const *argv[]) {
    char option;
    do {
        if (!is_logged_in) {
            option = main_menu(); 
            switch (option) {
            case '1':
                is_logged_in = log_in(); 
                break;
            case '2':
                register_user(); 
                break;
            case 'q':
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida\n");
                break;
            }
        } else {
            print_logged_in_menu(); 
            option = getchar(); 
            getchar(); 
            switch (option) {
            case '1':
                play_offline(); 
                break;
            case '2':
                is_logged_in = 0; 
                printf("Sesión cerrada exitosamente.\n");
                break;
            case 'q':
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida\n");
                break;
            }
        }
    } while (option != 'q');

    return 0;
}
