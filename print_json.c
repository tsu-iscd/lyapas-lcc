#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include "print_json.h"

void print_json(json_t *root) {
    print_json_aux(root, 0);
    printf("\n");
}

void print_json_aux(json_t *element, int indent) {
    switch (json_typeof(element)) {
    case JSON_OBJECT:
        print_json_object(element, indent);
        break;
    case JSON_ARRAY:
        print_json_array(element, indent);
        break;
    case JSON_STRING:
        print_json_string(element, indent);
        break;
    case JSON_INTEGER:
        print_json_integer(element, indent);
        break;
    case JSON_REAL:
        print_json_real(element, indent);
        break;
    case JSON_TRUE:
        print_json_true(element, indent);
        break;
    case JSON_FALSE:
        print_json_false(element, indent);
        break;
    case JSON_NULL:
        print_json_null(element, indent);
        break;
    default:
        fprintf(stderr, "unrecognized JSON type %d\n", json_typeof(element));
    }
}

void print_json_indent(int indent) {
    int i;
    for (i = 0; i < indent; i++) { putchar(' '); }
}

const char *json_plural(int count) {
    return count == 1 ? "" : "s";
}

void print_json_object(json_t *element, int indent) {
    const char *key;
    json_t *value;

    print_json_indent(indent);
    printf("{");
    int first = 1;
    json_object_foreach(element, key, value) {
        if (first)
            first = 0;
        else
            printf(", ");
        printf("\"%s\": ", key);
        if (json_typeof(value) == JSON_OBJECT || json_typeof(value) == JSON_ARRAY)
            printf("\n");
        print_json_aux(value, indent + 2);
        //if (json_typeof(value) == JSON_OBJECT || json_typeof(value) == JSON_ARRAY)
            //print_json_indent(indent);
    }
    //printf("\n");
    //print_json_indent(indent);
    printf("}");

}

void print_json_array(json_t *element, int indent) {
    size_t i;
    size_t size = json_array_size(element);
    print_json_indent(indent);
    printf("[\n");
    for (i = 0; i < size; i++) {
        if (i)
            printf(",\n");
        print_json_aux(json_array_get(element, i), indent + 2);
    }
    printf("\n");
    print_json_indent(indent);
    printf("]");
}

void print_json_string(json_t *element, int indent) {
    printf("\"%s\"", json_string_value(element));
}

void print_json_integer(json_t *element, int indent) {
    printf("%" JSON_INTEGER_FORMAT, json_integer_value(element));
}

void print_json_real(json_t *element, int indent) {
    printf("%f", json_real_value(element));
}

void print_json_true(json_t *element, int indent) {
    (void)element;
    printf("true");
}

void print_json_false(json_t *element, int indent) {
    (void)element;
    printf("false");
}

void print_json_null(json_t *element, int indent) {
    (void)element;
    printf("null");
}
