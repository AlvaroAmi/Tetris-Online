#include "menu.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "database.h"
#include <winsock2.h>

#define MAX_LINE 10
#define MAX_LEN 50

//OLD VERSION OF MENU 
//NEW VERSION --> MENUS.C

void print_tetris_logo() {
  printf("  _____ _____ _____ ____  ___ ____  \n");
  printf(" |_   _| ____|_   _|  _ \\|_ _/ ___| \n");
  printf("   | | |  _|   | | | |_) || |\\___ \\ \n");
  printf("   | | | |___  | | |  _ < | | ___) |\n");
  printf("   |_| |_____| |_| |_| \\_\\___|____/ \n");
  printf("   ___  _   _ _     ___ _   _ _____ \n");
  printf("  / _ \\| \\ | | |   |_ _| \\ | | ____|\n");
  printf(" | | | |  \\| | |    | ||  \\| |  _|  \n");
  printf(" | |_| | |\\  | |___ | || |\\  | |___ \n");
  printf("  \\___/|_| \\_|_____|___|_| \\_|_____|\n");
  printf("                                    \n");
  printf("\n");
}

void print_login() {
  printf("   ____     _     _                    _         \n");
  printf("  /  _/__  (_)___(_)__    ___ ___ ___ (_)__  ___ \n");
  printf(" _/ // _ \\/ / __/ / _ \\  (_-</ -_|_-</ / _ \\/ _ \\\n");
  printf("/___/_//_/_/\\__/_/\\___/ /___/\\__/___/_/\\___/_//_/\n");
  printf("                                                 \n");
  printf("\n");
}

void print_register() {
  printf("   ___           _     __                      \n");
  printf("  / _ \\___ ___ _(_)__ / /________ ________ ___ \n");
  printf(" / , _/ -_) _ `/ (_-</ __/ __/ _ `/ __(_-</ -_)\n");
  printf("/_/|_|\\__/\\_, /_/___/\\__/_/  \\_,_/_/ /___/\\__/ \n");
  printf("         /___/                                 \n");
  printf("\n");
}

void print_play() {
  printf("     __                  \n");
  printf(" __ / /_ _____ ____ _____\n");
  printf("/ // / // / _ `/ _ `/ __/\n");
  printf("\\___/\\_,_/\\_, /\\_,_/_/   \n");
  printf("         /___/           \n");
  printf("\n");
  printf("\n");
}

void clear_if_needed(char *str, int max_line) {
  // Limpia los caracteres de más introducidos
  if ((strlen(str) == max_line - 1) && (str[max_line - 2] != '\n'))
    while (getchar() != '\n')
      ;
}

char main_menu() {
  system("cls");
  print_tetris_logo();
  printf("1. Iniciar sesion\n");
  printf("2. Registrarse\n");
  printf("\n");
  printf("*Pulsa 'q' para salir\n");
  printf("\n");
  fflush(stdout);
  char linea[MAX_LINE];
  fgets(linea, MAX_LINE, stdin);
  clear_if_needed(linea, MAX_LINE);
  return *linea;
}

char **log_in_menu(char *option) {
  system("cls");
  system("clear");
  print_login();

  char **result = (char **)malloc(2 * sizeof(char *));
  for (int i = 0; i < 2; ++i) {
    result[i] = (char *)malloc(MAX_LEN * sizeof(char));
  }

  printf("Correo: ");
  fgets(result[0], MAX_LEN, stdin);
  result[0][strcspn(result[0], "\n")] = 0;

  printf("Contrasena: ");
  fgets(result[1], MAX_LEN, stdin);
  result[1][strcspn(result[1], "\n")] = 0;

  printf("Presione Enter para continuar o 'q' para salir: \n");
  fgets(option, MAX_LEN, stdin); 
  

  return result;
}


char offline_menu() {
  system("cls");
  print_play();
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

void init_menus(int is_logged_in){
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
}

int log_in() {
    system("cls");
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("TetrisOnline.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    char option;
    do {
        char *control_option = &option;
        char **result = log_in_menu(control_option); 
        if (option == '\n') {
            rc = show_user(db, result[0], result[1]);
            if (rc == 1) {
                printf("Inicio de sesion exitoso\n");
                system("pause");
                
                for (int i = 0; i < 2; i++) free(result[i]);
                free(result);
                return 1;
            } else {
                printf("Correo o contrasena incorrectos\n");
                system("pause");
            }
        } else if (option == 'q') {
          
            break;
        } else {
            printf("Opcion no valida\n");
            system("pause");
        }

        for (int i = 0; i < 2; i++) free(result[i]);
        free(result);
    } while (option != 'q');

    
    return 0;
}

void play_offline() {
  char option;
  do {
    option = offline_menu();
    switch (option) {
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
  } while (option != 'q');
}

int is_valid(char *mail, char *password) {
  // Comprobar si el correo y la contraseña son validos
  return 1;
}

int has_uppercase(const char *str) {
  while (*str) {
    if (isupper((unsigned char)*str))
      return 1;
    str++;
  }
  return 0;
}

int has_special_char(const char *str) {
  const char *special_chars = "!@#$%%^&*()-_=+[]{};:'\",.<>/?\\|`~";

  while (*str) {
    if (strchr(special_chars, *str))
      return 1;
    str++;
  }
  return 0;
}

int contains_at_symbol(const char *email) {
  return strchr(email, '@') != NULL;
}


void register_user() {
    
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("TetrisOnline.db", &db); 
    if (rc != SQLITE_OK) {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char email[MAX_LEN];
    char username[MAX_LEN];
    char password[MAX_LEN];
    char confirm_password[MAX_LEN];

    while (1) {
      system("cls");
        print_register(); 
        printf("Correo (o escribe 'q' para salir): ");
        fgets(email, MAX_LEN, stdin);
        email[strcspn(email, "\n")] = 0; 

        if (strcmp(email, "q") == 0) {
            break; 
        }

        if (!contains_at_symbol(email)) { 
            printf("Correo invalido. Debe contener un '@'.\n");
            system("pause");
            continue;
        }

        printf("Nombre de usuario: ");
        fgets(username, MAX_LEN, stdin);
        username[strcspn(username, "\n")] = 0;

        printf("Contrasena: ");
        fgets(password, MAX_LEN, stdin);
        password[strcspn(password, "\n")] = 0;

        printf("Confirmar contrasena: ");
        fgets(confirm_password, MAX_LEN, stdin);
        confirm_password[strcspn(confirm_password, "\n")] = 0;

        if (strcmp(password, confirm_password) != 0) {
            printf("Las contrasenas no coinciden.\n");
            system("pause");
            continue;
        }

        if (!has_uppercase(password) || !has_special_char(password) || strlen(password) < 8) {
            printf("La contrasena debe contener al menos una mayuscula, un caracter especial y tener 8 caracteres de longitud.\n");
            system("pause");
            continue;
        }

        
        rc = insert_user(db, username, email, password, 0); 
        if (rc != SQLITE_OK) {
            printf("Error al registrar el usuario: %s\n", sqlite3_errmsg(db));
            system("pause");
        } else {
            printf("Usuario registrado exitosamente.\n");
            system("pause");
            break; 
        }
    }

    sqlite3_close(db);
}


void print_logged_in_menu() {
    system("cls");
    print_tetris_logo(); 
    printf("1. Jugar\n");
    printf("2. Cerrar Sesion\n");
    printf("*Pulsa 'q' para salir\n");
}
