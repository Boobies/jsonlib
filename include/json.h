/*
 * Copyright 2017 Bogdan Barbu
 *
 * This file is part of JSONlib.
 *
 * JSONlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JSONlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with JSONlib.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JSON_H
#define JSON_H

#include <stdbool.h>
#include <stdio.h>

enum json_type {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_NUMBER,
    JSON_STRING,
    JSON_BOOL,
    JSON_NULL
};

typedef struct json_value {
    enum json_type type;
    union {
        struct json_object *object;
        struct json_array *array;
        double number;
        char *string;
        bool boolean;
    } value;
} json_value;

struct json_object {
    char *name;
    struct json_value *value;
    struct json_object *next;
};

struct json_array {
    size_t length;
    struct json_value *value[];
};

json_value *json_new(enum json_type, ...);
json_value *json_parse(FILE *f);
enum json_type json_type(json_value *);
void json_print(json_value *, FILE *);

void json_object_set(json_value *, char *, json_value *);
json_value *json_object_get(json_value *, char *);

size_t json_array_length(json_value *);
void json_array_set(json_value *, size_t, json_value *);
json_value *json_array_get(json_value *, size_t);

void json_number_set(json_value *, double);
double json_number_get(json_value *);

void json_string_set(json_value *, char *);
char *json_string_get(json_value *);

void json_bool_set(json_value *, bool);
bool json_bool_get(json_value *);

#endif
