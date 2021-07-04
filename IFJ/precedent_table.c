#include "precedent_table.h"
#include "file_loader.h"
#include "auxilary_functions.h"

static t_Token *aux = NULL;
static char GV_precedentTable[TABLE_SIZE*TABLE_SIZE+1]={
         //  +     -     *     /     (     )     i     <     >     ==    <=    >=    !=    EOL/, num
    /* + */ '>',  '>',  '<',  '<',  '<',  '>',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '>',   '<',
    /* - */ '<',  '>',  '<',  '<',  '<',  '>',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '>',   '<',
    /* * */ '>',  '>',  '>',  '>',  '<',  '>',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '>',   '<',
    /* / */ '>',  '>',  '<',  '>',  '<',  '>',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '>',   '<',
    /* ( */ '<',  '<',  '<',  '<',  '<',  '=',  '<',  '<',  '<',  '<',  '<',  '<',  '<',   0 ,   '<',
    /* ) */ '>',  '>',  '>',  '>',   0 ,  '>',   0 ,  '>',  '>',  '>',  '>',  '>',  '>',  '>',    0 ,
    /* i */ '>',  '>',  '>',  '>',   0 ,  '>',   0 ,  '>',  '>',  '>',  '>',  '>',  '>',  '>',    0 ,
    /* < */ '<',  '<',  '<',  '<',  '<',  '>',  '<',   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,  '>',   '<',
    /* > */ '<',  '<',  '<',  '<',  '<',  '>',  '<',   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,  '>',   '<',
    /*== */ '<',  '<',  '<',  '<',  '<',  '>',  '<',   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,  '>',   '<',
    /*!= */ '<',  '<',  '<',  '<',  '<',  '>',  '<',   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,  '>',   '<',
    /*<= */ '<',  '<',  '<',  '<',  '<',  '>',  '<',   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,  '>',   '<',
    /*>= */ '<',  '<',  '<',  '<',  '<',  '>',  '<',   0 ,   0 ,   0 ,   0 ,   0 ,   0 ,  '>',   '<',
    /*EOL*/ '<',  '<',  '<',  '<',  '<',   0 ,  '<',  '<',  '<',  '<',  '<',  '<',  '<',   0 ,   '<',
    /*num*/ '>',  '>',  '>',  '>',   0 ,  '>',   0 ,  '>',  '>',  '>',  '>',  '>',  '>',  '>',    0  
};
//_________________________________________________________________________
//________________________Stack functions__________________________________
//_________________________________________________________________________
void stackInit(t_Stack *s) // Init stack
{
    s->first=NULL;
}

t_StackItem topStack(t_Stack *s) // Returns top element from stack. If stack is empty, creates empty element
{
    if(s->first==NULL)
    {
        t_StackItem pom;
        pom.data=TOKEN_ERROR;
        pom.attributes=NULL;
        pom.dataType=ILLEGAL;
        pom.next=NULL;
        return pom;
    }
    return *s->first;
}

t_StackItem *pointerToTop(t_Stack *s)
{
    return s->first;
}

int topTerminal(t_Stack *s) // Returns type of first terminal in stack. If there isn't any, returns -1
{
    if(s->first==NULL)
    {
        return -1;
    }
    t_StackItem *pom=s->first;
    while(pom!=NULL && (pom->data==NONTERMINAL || pom->data==PRECEDENT_LESS))
    {
        pom=pom->next;
    }
    if(pom!=NULL)
    {
        return pom->data;
    }
    else
    {
        return -1;
    }
    
}

void freeItAll(t_StackItem *freeman) // Free attributes from stack element. Important for no memory leaks
{
    if(freeman->attributes!=NULL)
    {
        if(freeman->attributes->dynString!=NULL)
        {
            free(freeman->attributes->dynString);
            freeman->attributes->dynString=NULL;
        }
        free(freeman->attributes);
        freeman->attributes=NULL;
    }
}
t_StackItem popStack(t_Stack *s) // Pops element from stack. WARNING - call freeItAll on popped item for no memleaks
{
    if(s->first==NULL)
    {
        t_StackItem pom;
        pom.data=TOKEN_ERROR;
        pom.attributes=NULL;
        pom.dataType=ILLEGAL;
        pom.next=NULL;
        return pom;
    }
    t_StackItem *pom;
    t_StackItem returnor;
    pom=s->first;
    s->first=s->first->next;
    returnor.dataType=pom->dataType;
    returnor.data=pom->data;
    returnor.attributes=pom->attributes;
    returnor.next=NULL;
    returnor.ranking=pom->ranking;
    returnor.stillCons=pom->stillCons;
    free(pom);
    pom=NULL;
    return returnor;
}

t_TokenAttribute *createAttributes(t_TokenAttribute *attributes) // Creates stack element attributes. Dont call function if attributes is NULL
{
    t_TokenAttribute *newItem=malloc(sizeof(t_TokenAttribute));
    if(newItem==NULL)
    {
        return NULL;
    }
    
    newItem->decimal=attributes->decimal;
    newItem->dynStringCurrentIndex=attributes->dynStringCurrentIndex;
    newItem->dynStringSize=attributes->dynStringSize;
    newItem->number=attributes->number;
    int n=100;
    if(attributes->dynString!=NULL)
    {
        newItem->dynString=malloc(n*sizeof(htab_key_t));
        if(newItem->dynString==NULL)
        {
            free(newItem);
            return NULL;
        }
        strcpy(newItem->dynString, attributes->dynString);
    }
    else
    {
        newItem->dynString=NULL;
    }
    return newItem;
}

int pushStack(t_Stack *s, int data, int dataType, t_TokenAttribute *attributes, bool isConst, int ranking) // Pushes element to stack, allocates new attribute for it. Don't forget to avoid memleaks.
{
    t_StackItem *newItem = malloc(sizeof(t_StackItem));
    if(newItem==NULL)
    {
        return -1;
    }
    newItem->ranking=ranking;
    newItem->next=s->first;
    newItem->data=data;
    newItem->dataType=dataType;
    newItem->stillCons=isConst;
    s->first=newItem;
    if(attributes==NULL)
    {
        newItem->attributes=NULL;
        return 0;
    }
    newItem->attributes=createAttributes(attributes); // Create attributes for stack element - IMPORTANT - call freeItAll after pop, or memory leaks occur
    if(newItem->attributes==NULL)
    {
        return -1;
    }
    
    return 0;
}

int pushAfterTopTerminal(t_Stack *s) // Pushes < after first terminal in stack
{
    if(s->first==NULL)
    {
        return -1;
    }
    t_StackItem *pom=s->first;
    if(pom->data!=NONTERMINAL && pom->data !=PRECEDENT_LESS)
    {
        return pushStack(s, PRECEDENT_LESS, ILLEGAL, NULL, true, 0);
    }

    while(pom->next!=NULL && (pom->next->data==NONTERMINAL || pom->next->data==PRECEDENT_LESS))
    {
        pom=pom->next;
    }
    if(pom->next!=NULL) // Terminal found, allocate new element.
    {
        t_StackItem *newItem = malloc(sizeof(struct StackItem));
        if(newItem==NULL)
        {
            return -1;
        }
        newItem->next=pom->next;
        newItem->data=PRECEDENT_LESS;
        newItem->dataType=ILLEGAL;
        newItem->attributes=NULL;
        pom->next=newItem;
        return 0;
    }
    else
    {
        return -1;
    }
}

bool isInStack(t_Stack *s, int data) // Finds wheter type of element is in stack or not.
{
    if(s->first==NULL)
    {return false;}
    t_StackItem *pom = s->first;
    while(pom!=NULL && pom->data!=data)
    {
        pom=pom->next;
    }
    return pom!=NULL;
}

void deleteStack(t_Stack *s) // Deletes stack - only option where you dont need to call freeItAll separatedly. Still, dont forget call freeItAll on your popStacked elements.
{
    t_StackItem pom;
    while((pom=popStack(s)).data!=TOKEN_ERROR)
    {
        freeItAll(&pom);
    }
    s->first=NULL;
}

void printStack(t_Stack *s) // Prints stack 
{
    t_StackItem *pom = s->first;
    while(pom!=NULL)
    {
        fprintf(stderr, "%d\n", pom->data);
        pom=pom->next;
    }
    fprintf(stderr,"DNO\n");
}

//_________________________________________________________________________
//________________________Precedent table functions________________________
//_________________________________________________________________________

int precedentTableTranslator(int token) // Returns position of element in precedent table
{
    switch(token) //  +     -     *     /     (     )     i     <     >     ==    <=    >=    !=    EOL/,
    {
        case TOKEN_PLUS:
            return 0;
        case TOKEN_MINUS:
            return 1;
        case TOKEN_MULTIPLICATION:
            return 2;
        case TOKEN_DIVISION:
            return 3;
        case TOKEN_LEFT_PARENTHESIS:
            return 4;
        case TOKEN_RIGHT_PARENTHESIS:
            return 5;
        case TOKEN_IDENTIFIER:
            return 6;
        case TOKEN_LESS:
            return 7;
        case TOKEN_MORE:
            return 8;
        case TOKEN_EQUAL:
            return 9;
        case TOKEN_LESS_EQUAL:
            return 10;
        case TOKEN_MORE_EQUAL:
            return 11;
        case TOKEN_UNEQUAL:
            return 12;
        case TOKEN_EOL:
            return 13;
        case TOKEN_COMMA:
            return 13;
        case TOKEN_LEFT_BRACE:
            return 13;
        case TOKEN_SEMICOLON:
            return 13;
        case TOKEN_NUMBER:
            return 14;
        case TOKEN_DECIMAL:
            return 14;
        case TOKEN_STRING:
            return 14;
        default:
            fprintf(stderr, "\nInvalid input token value\n");
            return -1;
    }
    return 0;
}

int getValuePrecedentTable(int tokenStack, int tokenInput) // Translates your token values to >, <, = or 0.
{
    int i=precedentTableTranslator(tokenStack);
    if(i==-1) // Token is not in stack
    {
        return -1;
    }
    int j=precedentTableTranslator(tokenInput);
    if(j==-1)
    {
        return -1;
    }
    return (int)GV_precedentTable[(TABLE_SIZE*i)+j];
}

//_________________________________________________________________________
//________________________Grammar definition_______________________________
//_________________________________________________________________________

//Our grammar
/*
* 1: E->E+E
* 2: E->E*E
* 3: E->(E) yes
* 4: E->id yes
* 5: E->int yes
* 6: E->float yes
* 7: E->string yes
* 8: E->E-E
* 9: E->E/E
* 10: E->E>E
* 11: E->E<E
* 12: E->E!=E
* 13: E->E>=E
* 14: E->E<=E
* 15: E->E==E
* 15: E->-E
*/

//_________________________________________________________________________
//________________________Precedent analysis_______________________________
//_________________________________________________________________________

bool isEndingTerminal(int token) // Returns true if token is ending terminal. Miracle.
{
    return token==TOKEN_EOL || token==TOKEN_COMMA || token==TOKEN_LEFT_BRACE || token==TOKEN_SEMICOLON; 
}

bool isOperatorPrecedent(int token) // Returns true, if the operator is supported in our precedent table.
{
    if(token==TOKEN_PLUS || token==TOKEN_MINUS || token==TOKEN_DIVISION || token==TOKEN_MULTIPLICATION
        || token==TOKEN_EQUAL || token == TOKEN_UNEQUAL || token == TOKEN_LESS_EQUAL || token==TOKEN_LESS
        || token==TOKEN_MORE || token==TOKEN_MORE_EQUAL) return true;
        return false;
}

void initMinusToken(t_Token *token) // Initalizes TOKEN_MINUS
{
    token->type=TOKEN_MINUS;
    token->attribute.decimal=0;
    token->attribute.dynString=NULL;
    token->attribute.dynStringCurrentIndex=0;
    token->attribute.dynStringSize=0;
    token->attribute.number=0;
}

t_Token *fakeGetToken(bool getLine, t_Token *previousToken) // Returns current token OR breaks TOKEN_NUMBER || TOKEN_DECIMAL to TOKEN_MINUS and numerical token.
{   

    if(previousToken!=NULL && previousToken->type==TOKEN_MINUS && aux!=NULL){ // If last token was minus, you expect that after minus is number. If not, it doesnt matter
        destroyToken(previousToken);
        t_Token *token;
        token=aux;
        aux=NULL;
        return token;
    }

    else
    {
        t_Token *token=getToken(getLine); // Main function of this facade.
        if(token->type==TOKEN_NUMBER && token->attribute.number<0) // If number is <0, you should break it to TOKEN_MINUS and TOKEN_NUMBER/TOKEN_DECIMAL.
        {
            aux=token;
            aux->attribute.number=-1*aux->attribute.number;
            token=malloc(sizeof(struct Token));
            initMinusToken(token);
            return token;
        }
        if(token->type==TOKEN_DECIMAL && token->attribute.decimal<0)
        {
            aux=token;
            aux->attribute.decimal=-1*aux->attribute.decimal;
            token=malloc(sizeof(struct Token));
            initMinusToken(token);
            return token;
        }
        return token;
    }
    
}

t_StackItem emptyItem() // Initializes empty item.
{
    t_StackItem pom;
    pom.attributes=NULL;
    pom.data=1000;
    pom.dataType=ILLEGAL;
    pom.next=NULL;
    return pom;
}

float decimalOperation(float data1, float data2, int operation) // TODO - generate code
{
    switch(operation)
    {
        case TOKEN_PLUS:
            return data1+data2;
        case TOKEN_MINUS:
            return data1-data2;
        case TOKEN_MULTIPLICATION:
            return data1*data2;
        case TOKEN_DIVISION:
            return data1/data2;
        default:
            fprintf(stderr, "I cannot count this!");
            return 0;
    }
}

int intOperation(int data1, int data2, int operation) // TODO - generate code
{
    switch(operation)
    {
        case TOKEN_PLUS:
            return data1+data2;
        case TOKEN_MINUS:
            return data1-data2;
        case TOKEN_MULTIPLICATION:
            return data1*data2;
        case TOKEN_DIVISION:
            return data1/data2;
        default:
            fprintf(stderr, "I cannot count this!");
            return 0;
    }
}

// nt stringOperation(char) TODO - generate code

bool isCorrectToken(int token) //It should be named isIncorrectToken. My bad :)
{
    return token==TOKEN_ERROR || token==TOKEN_ERROR_MEM || token==TOKEN_EOF || token==TOKEN_RIGHT_BRACE || token==TOKEN_KEYWORD || 
        token==TOKEN_ASSIGN || token==TOKEN_DEFINE || token==TOKEN_ERROR || token==TOKEN_ERROR_MEM;
}

int precedentAnalysis(var_type_t expected, var_type_t *returned, bool generateCode) // expected contains expected type of expression, returned should contain result type of operationen
{
    if(generateCode)
      printf("CREATEFRAME\n");
    bool canEOL=true;
    bool semanticSucces=true;
    int comp=0;
    int localRanking=1;
    t_Token *token=NULL;
    token=fakeGetToken(false, token);
    if(isEndingTerminal(token->type) || isCorrectToken(token->type)) // if the token is ending token, you should read it until end of precedent analysis - opposite reads something you dont want
    {
        GV_returnLast=true;
        return 2;
    }
    t_Stack s;
    bool success=true; // Input is gramatically correct
    t_StackItem topFromStack = emptyItem();
    bool correctOperation;
    stackInit(&s);
    t_StackItem top = emptyItem();
    t_StackItem pom = emptyItem();
    t_StackItem deletor = emptyItem();

    pushStack(&s, TOKEN_EOL, ILLEGAL, NULL, true, 0); // Pushes bottom of stack
    do{ // Start of do-while loop of precedent analysis - it reads tokens from input and does the stuff.
        freeItAll(&top);
        freeItAll(&topFromStack);
        top.data=topTerminal(&s); // Reads information about first terminal in stack
        int tableValue=getValuePrecedentTable(top.data, token->type); // Returns value of precedent table from position of top and token->type
        switch(tableValue)
        {
            case '=': // Tokens have same priorty, pushes it to stack - only )
                if(pushStack(&s, token->type, ILLEGAL, &token->attribute, true, 0)==-1) // Push token to
                {
                    return 2; // ERROR 99
                }
                
                token=fakeGetToken(canEOL, token);
                if(token->type==TOKEN_ERROR || token->type==TOKEN_ERROR_MEM)
                {
                    freeItAll(&top);
                    freeItAll(&topFromStack);
                    freeItAll(&pom);
                    return 1;
                }
                if(isOperatorPrecedent(token->type) || token->type == TOKEN_LEFT_PARENTHESIS)
                {
                    canEOL=false;
                }
                else
                {
                    canEOL=true;
                } 
                if(isEndingTerminal(token->type))
                {
                    GV_returnLast=true;
                }         
                break;
            case '<': // Situation when your top terminal has lower priority than first input terminal (what a shame!)

                if(pushAfterTopTerminal(&s)==-1) // pushes PRECEDENT_LESS after first terminal, for example E+E -> E+<E (E are nonterminals)
                {
                    return 99; // ERROR 99
                }
                if(pushStack(&s, token->type, ILLEGAL, &token->attribute, true, 0)==-1)
                {
                    return 99; // ERROR 99
                }
                
                token=fakeGetToken(canEOL, token);
                if(token->type==TOKEN_ERROR || token->type==TOKEN_ERROR_MEM) // Analysis reads token - if it is error token, return error 1
                {
                    freeItAll(&top);
                    freeItAll(&topFromStack);
                    freeItAll(&pom);
                    return 1;
                }
                if(isOperatorPrecedent(token->type) || token->type == TOKEN_LEFT_PARENTHESIS) // After operators can be end of lines
                {
                    canEOL=false;
                }
                else
                {
                    canEOL=true;
                }
                
                if(isEndingTerminal(token->type))
                {
                    GV_returnLast=true;
                }
                break;
            case '>': // Your top terminal has higher priority than next terminal - start reduction!
                if(isInStack(&s, PRECEDENT_LESS)) // If there is PRECEDENT_LESS (<) in stack, you can do all the stuff, if not, it's error
                {
                    freeItAll(&top);
                    top=popStack(&s);
                    switch(top.data)
                    {
                        case TOKEN_NUMBER: // Top terminal is number and has higher priority than next token, apply E->int
                            
                            if((deletor=popStack(&s)).data==PRECEDENT_LESS) // pops < from stack
                            {   
                                if (generateCode)
                                {
                                  defvarTF(localRanking);
                                  movIntTF(localRanking, top.attributes->number);
                                }
                                int alert=pushStack(&s, NONTERMINAL, INT, top.attributes, true, localRanking++); // Creates new nonterminal in stack
                                if(alert==-1)
                                {
                                    return 99;
                                }
                                if(returned!=NULL)
                                {
                                    *returned=INT;
                                }
                                if(expected!=ILLEGAL && expected!=INT && expected!=BOOL)
                                {
                                    freeItAll(&deletor); // Cleaning
                                    freeItAll(&top);
                                    freeItAll(&topFromStack);
                                    return 5;
                                }
                                
                                
                                fprintf(stderr, "5 ");
                            }
                            else
                            {
                                success=false;
                            }
                            freeItAll(&deletor);
                            break;
                        case TOKEN_DECIMAL: // Top terminal is float and has higher priority than next token, apply E->float
                            if((deletor=popStack(&s)).data==PRECEDENT_LESS)
                            {
                                if (generateCode)
                                {
                                  defvarTF(localRanking);
                                  movFloatTF(localRanking, top.attributes->decimal);
                                }
                                int alert=pushStack(&s, NONTERMINAL, FLOAT64, top.attributes, true, localRanking++);
                                if(alert==-1)
                                {
                                    return 99;
                                }
                                if(returned!=NULL)
                                {
                                    *returned=FLOAT64; // configure return type
                                }
                                if(expected!=ILLEGAL && expected!=FLOAT64 && expected!=BOOL)
                                {
                                    freeItAll(&deletor);
                                    freeItAll(&top);
                                    freeItAll(&topFromStack);
                                    return 5;
                                }
                                fprintf(stderr, "6 ");
                            }
                            else
                            {
                                success=false;
                            }
                            freeItAll(&deletor);
                            break;
                        case TOKEN_STRING: // Top terminal is string and... E->string
                            if((deletor=popStack(&s)).data==PRECEDENT_LESS)
                            { 
                                if (generateCode)
                                {
                                  defvarTF(localRanking);
                                  movStringTF(localRanking, top.attributes->dynString);
                                }
                                int alert=pushStack(&s, NONTERMINAL, STRING, top.attributes, true, localRanking++);
                                if(alert==-1)
                                {
                                    return 99;
                                }
                                if(returned!=NULL)
                                {
                                    *returned=STRING; // configure return type
                                }
                                if(expected!=ILLEGAL && expected!=STRING && expected!=BOOL)
                                {
                                    freeItAll(&deletor);
                                    freeItAll(&top);
                                    freeItAll(&topFromStack);
                                    return 5;
                                }
                                fprintf(stderr,"7 ");
                            }
                            else
                            {
                                success=false;
                            }
                            freeItAll(&deletor);
                            break;
                        case TOKEN_IDENTIFIER: // Apply E->id
                            if((deletor=popStack(&s)).data==PRECEDENT_LESS)
                            {
                                htab_iterator_t pom=search_all_frames(top.attributes->dynString); // Search in hash table
                                if(!htab_iterator_valid(pom)) 
                                {
                                    semanticSucces=false; // Variable is not defined!
                                }
                                else if (pom.ptr->type==FUNC)
                                {
                                    deleteStack(&s); // Some cleaning
                                    freeItAll(&deletor);
                                    freeItAll(&topFromStack);
                                    freeItAll(&top);
                                    return 3;
                                }
                                else
                                {
                                
                                    if (generateCode)
                                    {
                                      defvarTF(localRanking);
                                      movIdentifTF(localRanking, pom.ptr->key, pom.ptr->frame_number);
                                    }
                                    int alert=pushStack(&s, NONTERMINAL, pom.ptr->type, top.attributes, false, localRanking++);
                                    if(alert==-1)
                                    {
                                        return 99;
                                    }
                                }
                                if(returned!=NULL && semanticSucces == true)
                                {
                                    *returned=pom.ptr->type; // Configure return type
                                }
                                
                                fprintf(stderr, "4 ");
                            }
                            else
                            {
                                deleteStack(&s); // Some cleaning
                                freeItAll(&deletor);
                                freeItAll(&pom);
                                freeItAll(&topFromStack);
                                freeItAll(&top);
                                return 2;
                            }
                            freeItAll(&deletor);
                            break;
                        case TOKEN_RIGHT_PARENTHESIS: // Apply E->(E)
                            freeItAll(&pom);
                            pom=popStack(&s);
                            if(pom.data==NONTERMINAL)
                            {
                                if((deletor=popStack(&s)).data==TOKEN_LEFT_PARENTHESIS)
                                {
                                    freeItAll(&deletor);
                                    if((deletor=popStack(&s)).data==PRECEDENT_LESS)
                                    {

                                        int alert=pushStack(&s, NONTERMINAL, pom.dataType, pom.attributes, pom.stillCons, localRanking);
                                        if(alert==-1)
                                        {
                                            return 99;
                                        }
                                        freeItAll(&pom);
                                        fprintf(stderr, "3 ");
                                    }
                                    else
                                    {
                                        success=false;
                                    }
                                    freeItAll(&deletor);
                                }
                                else
                                {
                                    success=false;
                                }
                                freeItAll(&deletor);
                                
                            }
                            else
                            {
                                success=false;
                            }
                            freeItAll(&pom);
                            freeItAll(&top);
                            freeItAll(&topFromStack);
                            break;
                        case NONTERMINAL: // Apply some E->E op E rule
                            freeItAll(&topFromStack);
                            topFromStack=popStack(&s);
                            correctOperation=false;
                            int operation=topFromStack.data;
                            if(comp>0) // If program have already done some comparing, return error 
                            {
                                freeItAll(&deletor);
                                freeItAll(&topFromStack);
                                freeItAll(&top);
                                deleteStack(&s);
                                return 5;
                            }
                            switch(topFromStack.data) // Possible rules for E->E op E
                            {
                                case TOKEN_PLUS:
                                    fprintf(stderr, "1 ");
                                    correctOperation=true;
                                    break;
                                case TOKEN_MINUS:
                                    fprintf(stderr, "8 ");
                                    if(top.dataType==STRING) // String can use only + operation, nothing else (and of course they can be compared, you curious)
                                    {
                                        freeItAll(&top);
                                        freeItAll(&topFromStack);
                                        deleteStack(&s);
                                        return 5;
                                    }
                                    correctOperation=true;
                                    break;
                                case TOKEN_MULTIPLICATION:
                                    fprintf(stderr,"2 ");
                                    if(top.dataType==STRING)
                                    {
                                        freeItAll(&top);
                                        freeItAll(&topFromStack);
                                        deleteStack(&s);
                                        return 5;
                                    }
                                    correctOperation=true;
                                    break;
                                case TOKEN_DIVISION:
                                    fprintf(stderr,"9 ");
                                    if(top.stillCons==true && top.attributes!=NULL && ((top.dataType==INT && top.attributes->number==0)|| 
                                        (top.dataType==FLOAT64 && top.attributes==0))) //
                                    { // Division by zero
                                        freeItAll(&top);
                                        freeItAll(&topFromStack);
                                        deleteStack(&s);
                                        return 9;
                                    }
                                    if(top.dataType==STRING)
                                    {
                                        freeItAll(&top);
                                        freeItAll(&topFromStack);
                                        deleteStack(&s);
                                        return 5;
                                    }
                                    correctOperation=true;
                                    break;
                                case TOKEN_MORE:
                                    fprintf(stderr,"10 ");
                                    if(expected!=BOOL && expected!=ILLEGAL) // You shouldn't compare anything, total bullshit
                                    {
                                        comp--;
                                        semanticSucces=false;
                                    }
                                    else
                                    {
                                        comp++;
                                    }
                                    correctOperation=true;
                                    
                                    
                                    correctOperation=true;
                                    break;
                                case TOKEN_LESS:
                                    fprintf(stderr,"11 ");
                                    if(expected!=BOOL && expected!=ILLEGAL)
                                    {
                                        comp--;
                                        semanticSucces=false;
                                    }
                                    else
                                    {
                                        comp++;
                                    }
                                    correctOperation=true;
                                    break;
                                case TOKEN_UNEQUAL:
                                    fprintf(stderr,"12 ");
                                    if(expected!=BOOL && expected!=ILLEGAL)
                                    {
                                        comp--;
                                        semanticSucces=false;
                                    }
                                    else
                                    {
                                        comp++;
                                    }
                                    correctOperation=true;
                                    break;
                                case TOKEN_MORE_EQUAL:
                                    fprintf(stderr,"13 ");
                                    if(expected!=BOOL && expected!=ILLEGAL)
                                    {
                                        comp--;
                                        semanticSucces=false;
                                    }
                                    else
                                    {
                                        comp++;
                                    }
                                    correctOperation=true;
                                    break;
                                case TOKEN_LESS_EQUAL:
                                    fprintf(stderr,"14 ");
                                    if(expected!=BOOL && expected!=ILLEGAL)
                                    {
                                        comp--;
                                        semanticSucces=false;
                                    }
                                    else
                                    {
                                        comp++;
                                    }
                                    correctOperation=true;
                                    break;
                                case TOKEN_EQUAL:
                                    fprintf(stderr,"15 ");
                                    if(expected!=BOOL && expected!=ILLEGAL)
                                    {
                                        comp--;
                                        semanticSucces=false;
                                    }
                                    else
                                    {
                                        comp++;
                                    }
                                    correctOperation=true;
                                    break;
                                default:
                                    fprintf(stderr,"\nIncorrect operator\n");
                                    correctOperation=false;
                                    break;
                            }
                            if(correctOperation)
                            {
                                
                                freeItAll(&topFromStack);
                                topFromStack=popStack(&s);
                                if(topFromStack.data==NONTERMINAL) // Rules are E-> E op E, no E -> bullshit op E
                                {
                                    if((deletor=popStack(&s)).data==PRECEDENT_LESS)
                                    {
                                        if(topFromStack.dataType==ILLEGAL || top.dataType==ILLEGAL) // Expressions like _ + _ are illegal, sorry
                                        {
                                            freeItAll(&deletor);
                                            freeItAll(&topFromStack);
                                            freeItAll(&top);
                                            return 7;
                                        }
                                        if(topFromStack.dataType==top.dataType)
                                        {
                                            if(topFromStack.dataType==FLOAT64 && topFromStack.stillCons && top.stillCons) // Totally useless, because variables exist and will break all the stuff
                                            {
                                                topFromStack.attributes->decimal=decimalOperation(topFromStack.attributes->decimal, top.attributes->decimal, operation);
                                            }
                                            else if(topFromStack.dataType==INT && topFromStack.stillCons && top.stillCons)
                                            {
                                                topFromStack.attributes->number=decimalOperation(topFromStack.attributes->number, top.attributes->number, operation);
                                            }
                                            if (generateCode)
                                            {
                                              defvarTF(localRanking);
                                            }
                                            switch(top.dataType)
                                            {
                                                case FLOAT64:
                                                    if(generateCode)
                                                      generateFloatOperation(topFromStack.ranking, top.ranking, operation, localRanking);
                                                    break;
                                                case INT:
                                                    if(generateCode)
                                                      generateIntOperation(topFromStack.ranking, top.ranking, operation, localRanking);
                                                    break;
                                                case STRING:
                                                    if(generateCode)
                                                      generateStringOperation(topFromStack.ranking, top.ranking, operation, localRanking);
                                                    break;
                                                default:
                                                    semanticSucces=false;
                                            }
                                            int alert=pushStack(&s, NONTERMINAL, topFromStack.dataType, topFromStack.attributes, (topFromStack.stillCons && top.stillCons), localRanking++);
                                            if(alert==-1)
                                            {
                                                return 99;
                                            }
                                            // Creates new nonterminal with some E -> E op E rule
                                            freeItAll(&topFromStack);
                                            freeItAll(&top);
                                        }
                                        else
                                        {
                                            freeItAll(&deletor);
                                            freeItAll(&topFromStack);
                                            freeItAll(&top);
                                            return 5;
                                        } 

                                    }
                                    else
                                    {
                                        success=false;
                                    }
                                    freeItAll(&deletor);
                                }
                                /*else if(topFromStack.data==PRECEDENT_LESS && negation)
                                {
                                    top.attributes->number=-top.attributes->number;
                                    top.attributes->decimal=-top.attributes->decimal;
                                    pushStack(&s, NONTERMINAL, top.dataType, top.attributes, top.stillCons);
                                    //freeItAll(&topFromStack);
                                    //freeItAll(&top);
                                }*/
                                else
                                {
                                    success=false;
                                }
                                freeItAll(&topFromStack);
                                freeItAll(&top);
                            }
                            else
                            {
                                success=false;
                            }
                            freeItAll(&topFromStack);
                            freeItAll(&top);
                            freeItAll(&deletor);
                            break;
                        default:
                            success=false;
                            break;
                        
                    }
                }// End of correct situation, when < (PRECEDENT_LESS) is in Stack
                else
                {
                    success=false;
                }     
                break;
            case -1: // Some token is invalit
                success=false;
                break;
            case 0: // Wait, that's illegal! (for example id id is typical syntax error)
                freeItAll(&deletor);
                freeItAll(&topFromStack);
                freeItAll(&top);
                deleteStack(&s);
                return 2;
                break;
            default:
                success=false;
                break;
        }

        if(!success || !semanticSucces) // End all the stuff
            break;
        
        if(token->type==TOKEN_EOF) // WTH? That has to be some error
            break;

        
        top.data=topTerminal(&s);

    }while((!isEndingTerminal(top.data)) || 
            (!isEndingTerminal(token->type))); // While loop running 'til the top term and the last term are not ending tokens
    

    deleteStack(&s);
    freeItAll(&topFromStack);
    freeItAll(&top);

    if((expected==BOOL && comp!=1) || comp>1) // More comparing in one expression? You lil bastard.
    {
        return 5;
    }
    
    if(token->type==EOF) // WTH?
    {
        return -1;
    }

    if(comp<0) // Comparing were illegal and that's the result
    {
        return 7;
    }

    if(success && semanticSucces) // That was brilliant, you did it! You wrote correct expression. I am proud of you!
    {
        fprintf(stderr,"\n");
        if (generateCode)
        {
          printf("DEFVAR TF@exprRes\n");
          printf("MOVE TF@exprRes TF@%%%d\n", localRanking-1);
        }
        GV_returnLast=true;
        return 0;
    }
    else
    {
        if(semanticSucces) // Semantic control was successfull, but syntax wasnt
        {
            return 2;
        }
        return 3;   
    }
}
