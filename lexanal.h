

/* ===================================================================
   These are the SYMBOLIC CONSTANTS of the token codes 
   for the tokens return by the lexical analyzer
   =================================================================== */

enum Token
{
   IDENTIFIER = 128,
   INTEGER_NUMBER,
   FLOAT_NUMBER,
   STRING,

   // Use the ASCII code for the single char tokens:
   //    , ( ) [ ] . : ; * + - / < > = "

   LESSOREQ, GREATEROREQ, NOTEQ,   // <=   >=  !=

   ALL, AND, ANY, AS, AVG, BETWEEN, BY, CHAR,
   CHECK, CLOSE, COMMIT, COUNT, CREATE, DECIMAL, DELETE, DISTINCT,
   DOUBLE, DROP, EXISTS, FLOAT, FROM, GO, GROUP, HAVING,
   IN, INSERT, INT, INTO, IS, LIKE, MAX, MIN,
   NOT, 

   NULL0,      // Because NULL is already defined in C, we use NULL0 

   NUMERIC, OF, ON, OR, ORDER, PRIMARY,
   REAL, SCHEMA, SELECT, SET, SOME, SUM, TABLE, TO,
   UNION, UNIQUE, UPDATE, USER, VALUES, VIEW, WHERE, WITH
};


int read_command(char *input);
char *lex_anal(char *current_pos, char *token, int *token_type);




