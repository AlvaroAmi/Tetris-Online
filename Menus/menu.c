#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

#define MAX_LINE 10
#define MAX_LEN 50

void clear_if_needed(char *str, int max_line)
{
	// Limpia los caracteres de más introducidos
	if ((strlen(str) == max_line-1) && (str[max_line-2] != '\n'))
		while (getchar() != '\n');
}

char main_menu(){
    printf("\t\t Tetris Online\n");
    printf("1. Iniciar sesion\n");
    printf("2. Registrarse\n");
    printf("3. Jugar offline\n");
    printf("\n");
    printf("*Pulsa 'q' para salir\n");
    printf("\n");
    fflush(stdout);
	char linea[MAX_LINE];
	fgets(linea, MAX_LINE, stdin);
	clear_if_needed(linea, MAX_LINE);
	return *linea;
}

char** log_in_menu(char *option){
    printf("\t\t Inicio sesion\n");

    char **result = (char**)malloc(2 * sizeof(char*)); 
    for (int i = 0; i < 2; ++i) {
        result[i] = (char*)malloc(MAX_LEN * sizeof(char));
    }

    printf("Correo: ");
    fgets(result[0], MAX_LEN, stdin);
    result[0][strcspn(result[0], "\n")] = 0;

    printf("Contraseña: ");
    scanf("%49s", result[1]); 

    while (getchar() != '\n');

    printf("Presione Enter para continuar o 'q' para salir: ");
    scanf("%c", option); 
    while (getchar() != '\n');

    return result;
}

char offline_menu(){
    printf("\t\t Juego offline\n");
    printf("1. Un Jugador\n");
    printf("2. Multijugador\n");
    printf("\n");
    printf("*Pulsa 'q' para salir\n");
    printf("\n");
    fflush(stdout);
    char linea[MAX_LINE];
    fgets(linea, MAX_LINE, stdin);
    clear_if_needed(linea, MAX_LINE);
    return *linea;
}

void log_in(){
    char option;
    do{
        char *control_option = &option;
        char **result = log_in_menu(control_option);
        switch (option){
        case '\n':
            if(is_valid(result[0], result[1])){
                printf("Inicio de sesion exitoso\n");
                
                for (int i = 0; i < 2; i++) free(result[i]);
                free(result);
                break;
            }else{
                printf("\t\t Inicio Incorrecto\n");
                printf("Correo o contraseña incorrectos\n");
                
                for (int i = 0; i < 2; i++) free(result[i]);
                free(result);
                break;
            }
        case 'q':
            
            for (int i = 0; i < 2; i++) free(result[i]);
            free(result);
            return; 
        default:
            printf("Opcion no valida\n");
            
            for (int i = 0; i < 2; i++) free(result[i]);
            free(result);
            break;
        }
    }while(option != 'q');

}


void play_offline(){
    char option;
    do{
        option = offline_menu();
        switch(option){
            case '1':
                printf("Jugando...\n");
                break;
            case '2':
                printf("Multijugador...\n");
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }
    }while(option != 'q');
}

bool is_valid(char *mail, char *password){
    //Comprobar si el correo y la contraseña son validos
    return true;
}