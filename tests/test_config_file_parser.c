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
    case CONFIG_BOOLEAN:  
        printf("BOOLEAN: %s = %s\n", parameter->key, parameter->value.config_boolean ? "true" : "false");
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
    if (argc < 2) {
        printf("Usage: %s <config file>\n", argv[0]);
        return 1;
    }

    const char* config_file = argv[1];
    struct config_parameter *config_parameter_head = read_config_file(config_file);
    if (config_parameter_head == NULL) {
        printf("Failed to read configuration.\n");
        return 1;
    }
    
    print_config_parameters(config_parameter_head);
    printf("\nSearching for 'port' parameter...\n");

    struct config_parameter *port_param = get_config_parameter(config_parameter_head, "port");
    if (port_param != NULL) {
        print_config_parameter(port_param);
    } else {
        printf("Parameter 'port' not found.\n");
    }
    
    save_config_parameters(config_parameter_head, "example_config_file_saved.txt");
    free_config_parameters(config_parameter_head);

    return 0;
}
