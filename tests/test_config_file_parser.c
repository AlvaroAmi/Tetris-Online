#include "config_file_parser.h"
#include <stdio.h>

void print_config_parameter(const struct config_parameter *parameter) {
    switch (parameter->type) {
    case INTEGER:
        printf("INTEGER: %s = %lld\n", parameter->key, parameter->value.integer);
        break;

    case REAL:
        printf("REAL: %s = %f\n", parameter->key, parameter->value.real);
        break;

    case STRING:
        printf("STRING: %s = %s\n", parameter->key, parameter->value.string);
        break;

    case BOOLEAN:
        printf("BOOLEAN: %s = %s\n", parameter->key, parameter->value.boolean ? "true" : "false");
        break;
    }
}

void print_config_parameters(struct config_parameter *head) {
    struct config_parameter *cur_node = head;
    while (cur_node != NULL) {
        print_config_parameter(cur_node);
        cur_node = cur_node->next;
    }
}

int main(int argc, char const *argv[]) {
    struct config_parameter *config_parameter_head = read_config_file("example_config_file.txt");
    print_config_parameters(config_parameter_head);
    save_config_parameters(config_parameter_head, "example_config_file_saved.txt");
    free_config_parameters(config_parameter_head);
    return 0;
}