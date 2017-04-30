/*
 * var.c
 *
 * Simple linked list to hold a list of variables containing of an identifier
 * and a value (both on long).
 *
 * Tom René Hennig
 */


/******************************************************************************
 *                              INCLUDE SECTION
 */

#include <stdio.h>
#include <stdlib.h>

#include "var.h"


/******************************************************************************
 *                           FUNCTION DEFINITIONS
 */

/*
 * Add a variable to the list returning the list (necessary to allocate a new
 * listin case it was emtpy). If the identifier is already allocated nothing
 * is modified.
 * ARGS     list - list handle to add variable to
 *          id   - identifier of new variable
 * RETURN   possibly new handle of the list
 */
VariableList addVariable(VariableList list, long id)
{
    Variable *tmp = list;
    
    // Allocate new list if necessary
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
    
    // Walk till the end of the list and check whether the new identifier is
    // already in the list
    while (tmp->next != NULL) {
        if (tmp->id == id) {
            fprintf(stderr, "WARNING: tried to double allocate variable");
            return list;
        }
        tmp = tmp->next;
    }
    
    // Allocate new element at the end of the list
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

/*
 * Look up value of given identifier, add new variable to the list and
 * initialize it to zero, if it is not yet allocated.
 * WARNING: cannot add variables to an empty list!
 * ARGS     list - list to search for identifier
 *          id   - identifier of variable to be searched for
 * RETURN   value of the searched variable (0 if it has not been allocated yet)
 */
long getValue(VariableList list, long id)
{
    Variable *tmp = list;
    
    // input check
    if (list == NULL) {
        fprintf(stderr, "ERROR: unable to obtain value from empty variable list\n");
        exit(EXIT_FAILURE);
    }
    
    // walk through the list and search identifier
    do {
        if (tmp->id == id)
            return tmp->value;
        tmp = tmp->next;
    } while (tmp != NULL);
    
    // add identifier to list if is not already there
    addVariable(list, id);
    return 0;
}

/*
 * Set the value of an variable in the given list, allocates a new variable
 * if necessary.
 * WARNING: cannot add variables to an empty list!
 * ARGS     list  - list to modify the value of variable identifier
 *          id    - identifier to search for
 *          value - new value of variable identifier
 */
void setValue(VariableList list, long id, long value)
{
    Variable *tmp = list;
    
    // input check
    if (list == NULL) {
        fprintf(stderr, "ERROR: unable to set value of an empty variable list\n");
        exit(EXIT_FAILURE);
    }
    
    // walk through the list and search identifier
    do {
        if (tmp->id == id) {
            tmp->value = value;
            return;
        }
        tmp = tmp->next;
    } while (tmp != NULL);
    
    // add identifier to list if is not already there
    list = addVariable(list, id);
    setValue(list, id, value);
}
