#ifndef CONFIG_FILE_PARSER_H
#define CONFIG_FILE_PARSER_H

#include <stdlib.h>

enum config_parameter_type {
    INTEGER,
    REAL,
    STRING,
    BOOLEAN
};

union config_parameter_value {
    long long integer;
    double real;
    char *string;
    int boolean;
};

struct config_parameter {
    enum config_parameter_type type;
    char *key;
    union config_parameter_value value;
    struct config_parameter *next;
};

struct config_parameter *read_config_file(const char *filename);
void free_config_parameters(struct config_parameter *head);
void save_config_parameters(struct config_parameter *head, const char *filename);
struct config_parameter *get_config_parameter(struct config_parameter *head, const char *key);

#endif