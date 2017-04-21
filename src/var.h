/*
 * var.h
 *
 * Tom René Hennig
 */

#ifndef VAR_H
#define VAR_H

typedef struct sVariable{
    long id;
    long value;
    struct sVariable *next;
} Variable;

typedef struct sVariable *VariableList;


VariableList addVariable(VariableList list, long id);
long getValue(VariableList list, long id);
void setValue(VariableList list, long id, long value);

#endif
