#include <stdio.h>
#include <stdbool.h>
#include "menu.h"
#define MAX_LINE 10
#define MAX_LEN 50

int main(int argc, char const *argv[])
{
    char option;
    do{
        option = main_menu();
        switch(option){
            case '1':
                log_in();
                break;
            case '2':
                printf("Registrar nuevo usuario...\n");
                break;
            case '3':
                play_offline();
                break;
            case 'q':
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }
    }while(option != 'q');
    
    return 0;
}
