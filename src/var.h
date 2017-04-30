/*
 * var.h
 *
 * Simple linked list to hold a list of variables containing of an identifier
 * and a value (both on long).
 *
 * Tom René Hennig
 */

#ifndef VAR_H
#define VAR_H


/******************************************************************************
 *                             TYPE DECLARATIONS
 */

/*
 * Variable as an element of a single linked list.
 */
typedef struct sVariable{
    long id;
    long value;
    struct sVariable *next;
} Variable;

/*
 * Definition of a list as an pointer to its first elelement
 */
typedef struct sVariable *VariableList;


/******************************************************************************
 *                            FUNCTION DECLARATIONS
 */

/*
 * Add a variable to the list returning the list (necessary to allocate a new
 * listin case it was emtpy). If the identifier is already allocated nothing
 * is modified.
 * ARGS     list - list handle to add variable to
 *          id   - identifier of new variable
 * RETURN   possibly new handle of the list
 */
VariableList addVariable(VariableList list, long id);

/*
 * Look up value of given identifier, add new variable to the list and
 * initialize it to zero, if it is not yet allocated.
 * WARNING: cannot add variables to an empty list!
 * ARGS     list - list to search for identifier
 *          id   - identifier of variable to be searched for
 * RETURN   value of the searched variable (0 if it has not been allocated yet)
 */
long getValue(VariableList list, long id);

/*
 * Set the value of an variable in the given list, allocates a new variable
 * if necessary.
 * WARNING: cannot add variables to an empty list!
 * ARGS     list  - list to modify the value of variable identifier
 *          id    - identifier to search for
 *          value - new value of variable identifier
 */
void setValue(VariableList list, long id, long value);

#endif
