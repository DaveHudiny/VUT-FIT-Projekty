#ifndef PRECEDENT_TABLE_H
#define PRECEDENT_TABLE_H

#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include "auxilary_functions.h"
#include "symtable.h"
#include "code_generator.h"

#define TABLE_SIZE 15


bool isOperatorPrecedent(int token);

// Translates token to precedentTable coordinates.
int precedentTableTranslator(int token);

int getValuePrecedentTable(int tokenStack, int tokenInput);

typedef struct StackItem // Item of stack
{
    struct StackItem *next;
    int ranking;
    int dataType;
    bool stillCons;
    t_TokenAttribute *attributes;
    int data; // Expected token value (for example NONTERMINAL, PRECEDENT_LESS, TOKEN_NUMBER...)
}t_StackItem;

typedef struct Stack // Stack structure
{
    t_StackItem *first;
}t_Stack;

void stackInit(t_Stack *s); // Initialize stack
int topTerminal(t_Stack *s); // Returns first terminal value from top of stack
t_StackItem popStack(t_Stack *s); // Returns data from top of stack and delete it in stack
t_StackItem topStack(t_Stack *s); // Return top from stack without deleting top
int pushStack(t_Stack *s, int data, int dataType, t_TokenAttribute *attributes, bool isConst, int ranking); // Pushing new top to stack with new token value
int pushAfterTopTerminal(t_Stack *s); // Pushing PRECEDENT_LESS value after first terminal in stack
bool isInStack(t_Stack *s, int data); // Returns true, when data value is in stack, false in opposite situation
void deleteStack(t_Stack *s); // Deletes stack
void printStack(t_Stack *s); // Prints all data from stack


bool isEndingTerminal(int token); // Returns true, if input value is EOL, COMMA, SEMICOLON or LEFT_BRACE
int precedentAnalysis(var_type_t expected, var_type_t *returned, bool generateCode); // Does precedent analysis - please, turn GV_returnLast to true if you have already read token from expression
// returns -1 if it wasn't successful and 2 if there were allocation problem. Returns 0 if correct.

#endif // end of precedent_table.h
