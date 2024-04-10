# Tetris-Online

### First compile the database creation 
#### On macOS:
```
gcc -I./include -o bin/create_database src/database/create_database.c -lsqlite3
``` 

### To compile use:
#### On Linux:
```
gcc -o ./bin/main $(find ./src -type f -name "*.c" ! -name "create_database.c") -I ./include -L ./lib -lsqlite3
```
#### On Windows:
```
gcc -o tetris-online src/database/database.c src/ficheros/*.c src/menus/*.c src/main.c lib/sqlite-amalgamation-3080704/sqlite3.c -Iinclude -Ilib/sqlite-amalgamation-3080704

```
### On macOS:
```

##### Test config file parser
```
gcc -o bin/test_config_file_parser src/ficheros/config_file_parser.c tests/test_config_file_parser.c -I include
```

```
./bin/test_config_file_parser
```