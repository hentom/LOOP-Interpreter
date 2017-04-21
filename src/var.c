/*
 * var.c
 *
 * Tom René Hennig
 */

#include <stdio.h>
#include <stdlib.h>

#include "var.h"


VariableList addVariable(VariableList list, long id)
{
    Variable *tmp = list;
    if (list == NULL) {
        tmp = malloc(sizeof(Variable));
        if (tmp == NULL) {
            fprintf(stderr, "ERROR: unable to allocate memory\n");
            exit(EXIT_FAILURE);
        }
        tmp->id = id;
        tmp->value = 0;
        tmp->next = NULL;
        return tmp;
    }
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = malloc(sizeof(Variable));
    if (tmp->next == NULL) {
        fprintf(stderr, "ERROR: unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    tmp = tmp->next;
    tmp->id = id;
    tmp->value = 0;
    tmp->next = NULL;
    return list;
}

long getValue(VariableList list, long id)
{
    Variable *tmp = list;
    if (list == NULL) {
        fprintf(stderr, "ERROR: unable to obtain value from empty variable list\n");
        exit(EXIT_FAILURE);
    }
    do {
        if (tmp->id == id)
            return tmp->value;
        tmp = tmp->next;
    } while (tmp != NULL);
    addVariable(list, id);
    return 0;
}

void setValue(VariableList list, long id, long value)
{
    Variable *tmp = list;
    if (list == NULL) {
        fprintf(stderr, "ERROR: unable to set value of an empty variable list\n");
        exit(EXIT_FAILURE);
    }
    do {
        if (tmp->id == id) {
            tmp->value = value;
            return;
        }
        tmp = tmp->next;
    } while (tmp != NULL);
    list = addVariable(list, id);
    setValue(list, id, value);
}
