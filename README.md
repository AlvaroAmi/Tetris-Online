# Tetris-Online

### First compile the database creation 


### To compile use:
#### On Linux:
```
gcc -o ./bin/main $(find ./src -type f -name "*.c" ! -name "create_database.c") -I ./include -L ./lib -lsqlite3
```
#### On Windows:
```

gcc -o bin/create_database src/database/create_database.c src/database/database.c lib/sqlite-amalgamation-3080704/sqlite3.c  -Iinclude -Ilib/sqlite-amalgamation-3080704 -Ilib -lpthread

gcc -o bin/query_database tests/query_database.c src/database/database.c lib/sqlite-amalgamation-3080704/sqlite3.c -Iinclude -Ilib/sqlite-amalgamation-3080704 -Ilib -lpthread


gcc -c src/database/database.c src/files/*.c src/menus/*.c lib/sqlite-amalgamation-3080704/sqlite3.c -Iinclude -Ilib/sqlite-amalgamation-3080704
g++ -c src/database/database.c -Iinclude -o database.o

Cliente:
g++ -c src/client/*.cpp src/tetris/*.cpp -Iinclude
g++ *.o -o bin/client.exe -lws2_32 -lncurses  

Servidor:
g++ -c src/server/server.cpp -Iinclude
g++ database.o sqlite3.o config_file_parser.o server.o -o bin/server.exe -lws2_32
```

#### On macOS:
```
gcc -o bin/tetris-online src/database/database.c src/files/*.c src/menus/*.c src/main.c lib/sqlite-amalgamation-3080704/sqlite3.c -Iinclude -Ilib/sqlite-amalgamation-3080704
```


### Compile the singleplayer game
```
g++ -o ./bin/tetris ./tests/test_tetris_singleplayer.cpp ./src/tetris/*.cpp -I ./include -lncurses
```

#### Execute the game
```
./bin/tetris
```

##### Test config file parser
```
gcc -o bin/test_config_file_parser src/files/config_file_parser.c tests/test_config_file_parser.c -I include
```

```
./bin/test_config_file_parser
```

##### Test sockets

```
g++ .\test_client1.cpp -o Client1.exe -lws2_32
g++ .\test_client2.cpp -o Client2.exe -lws2_32
```
