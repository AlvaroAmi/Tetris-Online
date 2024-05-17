#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <ctype.h>
#include "menus.h"
#include "database.h"

extern void send_request(SOCKET sock, const char* command, const char* email, const char* password, const char* username);
extern int receive_response(SOCKET sock);
extern void clear_screen();
extern int has_uppercase(const char *str);
extern int has_special_char(const char *str);
extern int contains_at_symbol(const char *email);
extern void start_listener_thread(SOCKET sock);
extern void stop_listener_thread();
extern void send_game_start(SOCKET sock);
extern void send_game_finish(SOCKET sock);

int isLoggedIn = 0;

//////////////////////////////////////ASTHETICS///////////////////////////////////////

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
///////////////////////////////////COMMUNICATION METHODS///////////////////////////////////////

void login(SOCKET sock) {
    char email[100];
    char password[100];

    clear_screen();
    print_login();
    printf("Email: ");
    scanf("%99s", email);
    printf("Contrasena: ");
    scanf("%99s", password);

    send_request(sock, "LOGIN", email, password, NULL);
    isLoggedIn = receive_response(sock);

    if (isLoggedIn) {
        start_listener_thread(sock);
    }
    
    system("pause");
}

void register_user(SOCKET sock) {
    char email[100];
    char password[100];
    char confpassword[100];
    char username[100];
    int registration_successful = 0;

    do {
        clear_screen();
        print_register();

        printf("Correo (o escribe 'q' para salir): ");
        scanf("%99s", email);
        if (strcmp(email, "q") == 0) {
            return;
        }
        printf("Nombre de usuario: ");
        scanf("%99s", username);
        printf("Contrasena: ");
        scanf("%99s", password);
        printf("Repetir contrasena: ");
        scanf("%99s", confpassword);

        if (strcmp(password, confpassword) != 0) {
            printf("Las contrasenas no coinciden.\n");
            system("pause");
        } else if (!has_uppercase(password)) {
            printf("La contrasena debe contener al menos una letra mayuscula.\n");
            system("pause");
        } else if (!has_special_char(password)) {
            printf("La contrasena debe contener al menos un caracter especial.\n");
            system("pause");
        } else if (!contains_at_symbol(email)) {
            printf("El correo debe contener un simbolo '@'.\n");
            system("pause");
        } else {
            send_request(sock, "REGISTER", email, password, username);
            registration_successful = receive_response(sock); 
            system("pause");
            break; 
        }

    } while (!registration_successful);
}

void start_game(SOCKET sock) {
    send_game_start(sock);
    // Add game logic here
}

void finish_game(SOCKET sock) {
    send_game_finish(sock);
    // Add game finish logic here
}

///////////////////////////////////OTHER METHODS///////////////////////////////////////

void clear_screen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
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

///////////////////////////////////MENU DISPLAY///////////////////////////////////////

void print_logged_in_menu() {
    system("cls");
    print_tetris_logo(); 
    printf("1. Jugar online\n");
    printf("2. Jugar offline\n");
    printf("3. Cerrar Sesion\n");
    printf("*Pulsa 'q' para salir\n");
}

void display_menu(SOCKET sock) {
    char input[10];
    int option;

    do {
        clear_screen();
        if (isLoggedIn == 0) {
            print_tetris_logo();
            printf("1. Iniciar sesion\n");
            printf("2. Registrarse\n");
            printf("\n");
            printf("*Pulsa 'q' para salir\n");
            printf("\n");

            if (!fgets(input, sizeof(input), stdin)) {
                continue;
            }

            if (input[0] == 'q' || input[0] == 'Q') {
                break;
            }

            option = atoi(input);
            switch (option) {
                case 1:
                    login(sock);
                    break;
                case 2:
                    register_user(sock);
                    break;
            }
        } else {
            print_logged_in_menu();
            if (!fgets(input, sizeof(input), stdin)) {
                continue;
            }
            if (input[0] == 'q' || input[0] == 'Q') {
                isLoggedIn = 0;
                stop_listener_thread();
                finish_game(sock);
                continue;
            }

            option = atoi(input);
            switch (option) {
                case 1:
                    start_game(sock);
                    break;
                case 2:
                    //Initialize offline game
                    break;
                case 3:
                    isLoggedIn = 0; 
                    stop_listener_thread();
                    finish_game(sock);
                    break;

            }
        }
    } while (1);
}
