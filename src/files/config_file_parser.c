#include "config_file_parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum parser_state {
    EXPECTING_IDENTIFIER,
    EXPECTING_EQUAL,
    EXPECTING_VALUE
};

struct parser {
    char *src;
    size_t cursor;
    size_t src_length;
    size_t line;
    size_t beginning_of_line; // Used to calculate the column
    enum parser_state state;
};

static int is_eof(struct parser *parser) {
    return parser->cursor >= parser->src_length;
}

static void skip_while_not(struct parser *parser, int (*predicate)(int)) {
    while (!is_eof(parser) && !predicate(parser->src[parser->cursor])) {
        parser->cursor++;
    }
}

static void skip_line(struct parser *parser) {
    while (!is_eof(parser) && !(parser->src[parser->cursor] == '\n')) {
        parser->cursor++;
    }
}

static int is_identifier_start(const char c) {
    return isalpha(c) || c == '_';
}

static int is_identifier(const char c) {
    return isalnum(c) || c == '_';
}

static int is_boolean(const char *s) {
    return strcmp(s, "true") == 0 || strcmp(s, "false") == 0;
}

static void parser_error(struct parser *parser, const char *message) {
    fprintf(stderr, "Error at line %ld, column %ld: %s\n", parser->line, parser->cursor - parser->beginning_of_line + 1, message);
    exit(1);
}

static char *get_string_from_range(const char *src, size_t start, size_t end) {
    size_t size = end - start;
    char *str = (char *)malloc(size + 1);
    memcpy(str, src + start, size);
    str[size] = '\0';
    return str;
}

static char *parse_identifier(struct parser *parser) {
    size_t first_char_position = parser->cursor - 1;

    if (!is_identifier_start(parser->src[parser->cursor - 1])) {
        parser_error(parser, "Invalid identifier");
    }

    while (!is_eof(parser) && is_identifier(parser->src[parser->cursor]))
        parser->cursor++;

    char *identifier = get_string_from_range(parser->src, first_char_position, parser->cursor);

    return identifier;
}

static void parse_string(struct parser *parser, struct config_parameter *config_parameter) {
    size_t first_char_position = parser->cursor;
    char c;
    while ((c = parser->src[parser->cursor++]) != '"') {
        if (c == '\0' || c == '\n') {
            parser_error(parser, "Expected '\"'");
        }
    }

    char *string = get_string_from_range(parser->src, first_char_position, parser->cursor - 1);

    config_parameter->type = STRING;
    config_parameter->value.string = string;
}

static void parse_number(struct parser *parser, struct config_parameter *config_parameter) {
    size_t first_char_position = parser->cursor - 1;

    while (!is_eof(parser) && (isdigit(parser->src[parser->cursor]) || parser->src[parser->cursor] == '.' || parser->src[parser->cursor] == '+' || parser->src[parser->cursor] == '-')) {
        parser->cursor++;
    }

    char *number_str = get_string_from_range(parser->src, first_char_position, parser->cursor);

    char *endptr;
    config_parameter->value.integer = strtoll(number_str, &endptr, 10);

    // Check if the conversion to integer was successful
    if (*endptr == '\0') {
        config_parameter->type = INTEGER;
        free(number_str);
        return;
    }

    // Try to convert the string to a double (real number)
    config_parameter->value.real = strtod(number_str, &endptr);

    // Check if the conversion to double was successful
    if (*endptr == '\0') {
        config_parameter->type = REAL;
        free(number_str);
        return;
    }

    free(number_str);
    parser_error(parser, "Invalid number");
}

static struct config_parameter *next_config_parameter(struct parser *parser) {
    parser->state = EXPECTING_IDENTIFIER;
    char *key = NULL;

    char c;
    while ((c = parser->src[parser->cursor++])) {
        switch (c) {
        case ' ':
        case '\t':
        case '\r':
            break;

        case '\0':
        case '\n':
            if (parser->state == EXPECTING_VALUE)
                parser_error(parser, "Expected value");
            else if (parser->state == EXPECTING_EQUAL)
                parser_error(parser, "Expected '='");

            parser->line++;
            parser->beginning_of_line = parser->cursor;

            if (c == '\0')
                return NULL;
            break;

        case '#':
            skip_line(parser);
            break;

        default:
            switch (parser->state) {
            case EXPECTING_IDENTIFIER:
                key = parse_identifier(parser);
                parser->state = EXPECTING_EQUAL;
                break;

            case EXPECTING_EQUAL:
                if (c != '=') {
                    parser_error(parser, "Expected '='");
                }
                parser->state = EXPECTING_VALUE;
                break;

            case EXPECTING_VALUE:;
                struct config_parameter *parameter = (struct config_parameter *)malloc(sizeof(struct config_parameter));
                parameter->key = key;

                if (c == '"') {
                    parse_string(parser, parameter);
                    return parameter;
                } else if (isdigit(c) || c == '+' || c == '-') {
                    parse_number(parser, parameter);
                    return parameter;
                }

                size_t first_char_position = parser->cursor - 1;
                skip_while_not(parser, isspace);
                char *value_str = get_string_from_range(parser->src, first_char_position, parser->cursor);

                if (is_boolean(value_str)) {
                    parameter->type = CONFIG_BOOLEAN;
                    parameter->value.config_boolean = strcmp(value_str, "true") == 0;
                    free(value_str);
                    return parameter;
                }

                char msg[24 + strlen(key) + 1];
                sprintf(msg, "Invalid value for key \"%s\"", key);
                free(key);
                free(parameter);
                parser_error(parser, msg);
            }
            break;
        }
    }
}

struct config_parameter *read_config_file(const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    long fpsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    struct parser parser = {
        .src = (char *)malloc(fpsize + 1),
        .cursor = 0,
        .src_length = (size_t)fpsize,
        .line = 1,
        .beginning_of_line = 0,
        .state = EXPECTING_IDENTIFIER};

    fread(parser.src, 1, fpsize, fp);
    parser.src[fpsize] = '\0';
    fclose(fp);

    struct config_parameter *head = next_config_parameter(&parser);
    struct config_parameter *cur_node = head;

    while (1) {
        struct config_parameter *parameter = next_config_parameter(&parser);
        if (parameter == NULL)
            break;

        cur_node->next = parameter;
        cur_node = parameter;
    }

    free(parser.src);

    return head;
}

void free_config_parameters(struct config_parameter *head) {
    struct config_parameter *cur_node = head;
    while (cur_node != NULL) {
        struct config_parameter *next = cur_node->next;
        free(cur_node->key);
        if (cur_node->type == STRING)
            free(cur_node->value.string);

        free(cur_node);
        cur_node = next;
    }
}

void save_config_parameters(struct config_parameter *head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
        exit(1);
    }

    struct config_parameter *cur_node = head;
    while (cur_node != NULL) {
        switch (cur_node->type) {
        case INTEGER:
            fprintf(fp, "%s = %lld\n", cur_node->key, cur_node->value.integer);
            break;

        case REAL:
            fprintf(fp, "%s = %f\n", cur_node->key, cur_node->value.real);
            break;

        case STRING:
            fprintf(fp, "%s = \"%s\"\n", cur_node->key, cur_node->value.string);
            break;

        case CONFIG_BOOLEAN:
            fprintf(fp, "%s = %s\n", cur_node->key, cur_node->value.config_boolean ? "true" : "false");
            break;
        }

        cur_node = cur_node->next;
    }

    fclose(fp);
}

struct config_parameter *get_config_parameter(struct config_parameter *head, const char *key) {
    struct config_parameter *cur_node = head;
    while (cur_node != NULL) {
        if (strcmp(cur_node->key, key) == 0)
            return cur_node;

        cur_node = cur_node->next;
    }

    return NULL;
}
