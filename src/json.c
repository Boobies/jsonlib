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

#include <assert.h>
#include <json.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

json_value *json_new(enum json_type type, ...) {
    json_value *val;
    static json_value null = { JSON_NULL };
    size_t arraylen, arraysz;
    va_list arg;

    va_start(arg, type);

    if ((val = malloc(sizeof *val)) != NULL)
        switch (val->type = type) {
        case JSON_OBJECT:
            val->value.object = NULL;
            break;
        case JSON_ARRAY:
            arraylen = va_arg(arg, size_t);
            arraysz = arraylen * sizeof val->value.array->value[0];
            if ((val->value.array = malloc(sizeof *val->value.array + arraysz)) != NULL)
                for (size_t i = 0; i < (val->value.array->length = arraylen); ++i)
                    json_array_set(val, i, &null);
            else {
                free(val);
                val = NULL;
            }
            break;
        case JSON_NUMBER:
            val->value.number = 0;
            break;
        case JSON_STRING:
            val->value.string = "";
            break;
        case JSON_BOOL:
            val->value.boolean = false;
            break;
        }

    va_end(arg);

    return val;
}

enum json_type json_type(json_value *src) {
    return src->type;
}

void json_print(json_value *src, FILE *f) {
    switch (json_type(src)) {
    case JSON_OBJECT:
        fputs("{ ", f);
        for (struct json_object *p = src->value.object; p != NULL; (p = p->next) && fputs(", ", f)) {
            fprintf(f, "\"%s\": ", p->name);
            json_print(p->value, f);
        }
        fputs(" }", f);
        break;
    case JSON_ARRAY:
        fputc('[', f);
        for (size_t i = 0; i < json_array_length(src); ++i) {
            json_print(json_array_get(src, i), f);
            if (i < json_array_length(src) - 1)
                fputs(", ", f);
        }
        fputc(']', f);
        break;
    case JSON_NUMBER:
        fprintf(f, "%f", json_number_get(src));
        break;
    case JSON_STRING:
        fprintf(f, "\"%s\"", json_string_get(src));
        break;
    case JSON_BOOL:
        fputs(json_bool_get(src) ? "true" : "false", f);
        break;
    case JSON_NULL:
        fputs("null", f);
        break;
    }
}

void json_object_set(json_value *src, char *name, json_value *val) {
    struct json_object *node;

    assert(json_type(src) == JSON_OBJECT);
    assert((node = malloc(sizeof *node)) != NULL);

    node->name = name;
    node->value = val;
    node->next = src->value.object;
    src->value.object = node;
}

json_value *json_object_get(json_value *src, char *name) {
    assert(json_type(src) == JSON_OBJECT);

    for (struct json_object *p = src->value.object; p != NULL; p = p->next)
        if (strcmp(name, p->name) == 0)
            return p->value;

    return NULL;
}

size_t json_array_length(json_value *src) {
    assert(json_type(src) == JSON_ARRAY);

    return src->value.array->length;
}

void json_array_set(json_value *src, size_t idx, json_value *val) {
    assert(idx < json_array_length(src));

    src->value.array->value[idx] = val;
}

json_value *json_array_get(json_value *src, size_t idx) {
    assert(idx < json_array_length(src));

    return src->value.array->value[idx];
}

void json_number_set(json_value *src, double n) {
    assert(json_type(src) == JSON_NUMBER);

    src->value.number = n;
}

double json_number_get(json_value *src) {
    assert(json_type(src) == JSON_NUMBER);

    return src->value.number;
}

void json_string_set(json_value *src, char *str) {
    assert(json_type(src) == JSON_STRING);

    src->value.string = str;
}

char *json_string_get(json_value *src) {
    assert(json_type(src) == JSON_STRING);

    return src->value.string;
}

void json_bool_set(json_value *src, bool b) {
    assert(json_type(src) == JSON_BOOL);

    src->value.boolean = b;
}

bool json_bool_get(json_value *src) {
    assert(json_type(src) == JSON_BOOL);

    return src->value.boolean;
}
