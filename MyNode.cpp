//
//  ExprDelete.cpp
//  SQL parser and interpreter
//
//  Created by 王璇 on 17/4/3.
//  Copyright © 2017年 Xuan Wang. All rights reserved.
//

#include <stdio.h>
#include "lexanal.h"
#include <cstdlib>
#include <cstring>
#include "ScanTable.h"
#include "MyNode.h"
#include <map>
#include <string>


using namespace std;

char *lex_anal(char *current_pos, char *token, int *token_type);
struct MyNode *E(char *&p, map<string, ScanTable*> &tablelist);
struct MyNode *BE(char *& current,map<string, ScanTable*> &tablelist);
bool isCompatible(int i, int j);


/*==========================================================
            MyNode member: evaluate string
 ========================================================== */
char* MyNode::eval_string(){
    if(node_type == STRING_CONST)
        return value.c;
    else if(node_type == CHAR_ATTR)
        return value.c_attr;
    else{
        printf("no matching node_type! in eval_string()\n");
        return(NULL);
    }
            
}
/*==========================================================
            MyNode member: evaluate number
 ========================================================== */
double MyNode::eval_num(){
    if(node_type == INT_CONST)
        return value.i;
    else if(node_type == FLOAT_CONST)
        return value.f;
    else if(node_type == INT_ATTR)
        return *value.i_attr;
    else if(node_type == FLOAT_ATTR)
        return *value.f_attr;
    else if(node_type == PLUS)
        return value.p[0]->eval_num() + value.p[1]->eval_num();
    else if(node_type == MINUS)
        return value.p[0]->eval_num() - value.p[1]->eval_num();
    else if(node_type == MULT)
        return value.p[0]->eval_num() * value.p[1]->eval_num();
    else if(node_type == DIV)
        return value.p[0]->eval_num() / value.p[1]->eval_num();
    else{
        printf("no matching node_type! in eval_num()\n");
        return 0;
    }
}
/*==========================================================
            MyNode member: evaluate bool expression
 ========================================================== */
bool MyNode::eval_bool(){
    
    /*call eval_num()*/
    if(node_type == LS)
        return value.p[0]->eval_num() < value.p[1]->eval_num();
    else if(node_type == LSE)
        return value.p[0]->eval_num() <= value.p[1]->eval_num();
    else if(node_type == LA)
        return value.p[0]->eval_num() > value.p[1]->eval_num();
    else if(node_type == LAE)
        return value.p[0]->eval_num() >= value.p[1]->eval_num();
    else if(node_type == EQ)
        return value.p[0]->eval_num() == value.p[1]->eval_num();
    else if(node_type == UEQ)
        return value.p[0]->eval_num() != value.p[1]->eval_num();
    else if(node_type == PLUS ||node_type == MINUS || node_type == MULT || node_type == DIV || node_type == INT_ATTR || node_type == FLOAT_ATTR || node_type == INT_CONST || node_type == FLOAT_CONST)
        return eval_num();
    
    /*call eval_bool()*/
    else if(node_type == AND_)
        return value.p[0]->eval_bool() && value.p[1]->eval_bool();
    else if(node_type == OR_)
        return value.p[0]->eval_bool() || value.p[1]->eval_bool();
    else if(node_type == NOT_)
        return !value.q->eval_bool();
    
    /*call eval_string()*/
    else if(node_type == LSSS)
        return strcmp(value.p[0]->eval_string(), value.p[1]->eval_string()) < 0;
    else if(node_type == LASS)
        return strcmp(value.p[0]->eval_string(), value.p[1]->eval_string()) > 0;
    else if(node_type == LSESS)
        return strcmp(value.p[0]->eval_string(), value.p[1]->eval_string()) <= 0;
    else if(node_type == LAESS)
        return strcmp(value.p[0]->eval_string(), value.p[1]->eval_string()) >= 0;
    else if(node_type == EQSS)
        return strcmp(value.p[0]->eval_string(), value.p[1]->eval_string()) == 0;
    else if(node_type == UEQSS)
        return strcmp(value.p[0]->eval_string(), value.p[1]->eval_string()) != 0;
    
    
    else{
        printf("no matching node_type in eval_bool()\n");
        return false;
    }
}



/*==========================================================
 INT_CONST | FLOAT_CONST | STRING_CONST | ID | ID.ID | ( E )
 ========================================================== */
struct MyNode *F(char * & current, map<string, ScanTable*> &tablelist){//?
    struct MyNode *pr;
    char token[100] ={'\0'};
    int token_code;
    char tmp_token[100]={'\0'};
    int tmp_code;
    char* p;
    ScanTable *table = new ScanTable;
    
    current=lex_anal( current, token, &token_code );
    
    if(token_code == INTEGER_NUMBER){
        pr = new MyNode;
        pr->node_type = INT_CONST;
        pr->value.i = atoi(token);
        return pr;
    }
    
    else if(token_code == FLOAT_NUMBER){
        pr = new MyNode;
        pr->node_type = FLOAT_CONST;
        pr->value.f = atof(token);
        pr->isFloat = true;
        return pr;
    }
    
    else if(token_code == STRING){
        pr = new MyNode;
        pr->node_type = STRING_CONST;
        strcpy(pr->value.c, token);
        return pr;
    }
    
    //need to handle test.name and name two type attribute name
    else if(token_code == IDENTIFIER){
        bool hasPrefix = false;
        
        //if the token is tablename or alias, need to check the next pos
        string name(token, strlen(token));//store the possible tablename or alias or attr_name at first
        //peek the next position
        p = lex_anal(current, tmp_token, &tmp_code);
        //has prefix
        if(tmp_code == '.'){
            current=lex_anal(current, token, &token_code);//read away .
            current=lex_anal(current, token, &token_code);//this token is the token next to .
            hasPrefix = true;
            
        }
        
        if(hasPrefix == true){
            if(tablelist.find(name) != tablelist.end())
                table = tablelist.at(name);
            else{
                printf("Error in attribute perfix!\n");
                return(NULL);
            }
            table->Open(table->true_Relname);//open with real name
        }
        else{
            //find the attribute from every table
            int count = 0;
            map<string, ScanTable*>::iterator it,pos;
            for(it = tablelist.begin(); it != tablelist.end(); ++it){
                table = it->second;
                table->Open(table->true_Relname);
                if(table->findAttr(token)){
                    count++;
                    pos = it;
                }
                table->Close();
            }
            //it = tablelist.begin();
            
            if(count == 1){
                table = pos->second;
                table->Open(table->true_Relname);
            }
            else if(count > 1){
                printf("Error: attribute name confliction\n");
                return(NULL);
            }
            else{
                printf("Error: cannot find this attribute\n");
                return(NULL);
            }
        }
        
        if(table->getAttrType(token) == 'C'){
            pr = new MyNode;
            pr->node_type = CHAR_ATTR;
            pr->value.c_attr =(char *) table->getAttrPtr(token);
            table->Close();
            return pr;
        }
        
        else if(table->getAttrType(token) == 'F'){
            pr = new MyNode;
            pr->node_type = FLOAT_ATTR;
            pr->isFloat = true;
            pr->value.f_attr =(double *) table->getAttrPtr(token);
            table->Close();
            return pr;
        }
        
        else if(table->getAttrType(token) == 'I'){
            pr = new MyNode;
            pr->node_type = INT_ATTR;
            pr->value.i_attr = (int *) table->getAttrPtr(token);
            table->Close();
            return pr;
        }
        
        else{
            printf("cannot find attribute!\n");
            table->Close();
            return NULL;
        }
        
    }
    
    else if(token_code == '('){
        
        pr = E(current, tablelist);
        if(pr == NULL)
            return NULL;
        
        current = lex_anal(current, token, &token_code);
        
        if(token_code == ')')
            return pr;
        else{
            printf("Syntax error: ')' was expected\n");
            return NULL;
        }
    }
    
    else{
        printf("Syntax Error: only expect INT_CONST | FLOAT_CONST | STRING_CONST | ID | ID.ID | ( E )\n");
        return NULL;
    }
}



/*==========================================================
 Term ::= Factor [ * Factor | / Factor ] ....
 ========================================================== */
struct MyNode *T(char *& current,map<string, ScanTable*> &tablelist){
    struct MyNode *pr1, *pr2;
    char *p;
    char token[100]={'\0'};
    int token_code = 0;
    char tmp_token[100] = {'\0'};
    int tmp_code;
    
    pr1 = F(current, tablelist);
    
    if(pr1 == NULL)
        return NULL;
    
    p = lex_anal(current, token, &token_code);//peek the next token, while current is still in the previous position
    while(token_code == '*'||token_code == '/'){
        //read and pass this token
        current = lex_anal(current, token, &token_code);
        
        pr2 = new MyNode;
        pr2->node_type = (token_code == '*')? MULT:DIV;
        pr2->value.p[0] = pr1;
        
        //compatible check: pr2 parent node type -- pr2 child(p[0]) node type
        if(isCompatible(pr2->node_type, pr1->node_type))
            pr2->value.p[1] = F(current, tablelist);
        else
            return NULL;
        
        if(pr2->value.p[1] == NULL)
            return(NULL);
        
        if(pr2->value.p[0]->isFloat == true || pr2->value.p[1]->isFloat == true)
            pr2->isFloat = true;
        //compatible check: pr2 parent node type -- pr2 child(p[1]) node type
        if(isCompatible(pr2->node_type, pr2->value.p[1]->node_type))
            pr1 = pr2;
        else
            return(NULL);
        
        
        //peek the next token
        p = lex_anal(current, tmp_token, &tmp_code);//to peek the next position
        if(tmp_code == '*'|| tmp_code == '/'){
            p = lex_anal(current, token, &token_code);
            continue;
        }
        else
            break;
    }
   
    return pr1;
}


/*==========================================================
        Expr ::= Term [ + Term | - Term ] ...
 ========================================================== */
struct MyNode *E(char *& current, map<string, ScanTable*> &tablelist){
    struct MyNode *pr1, *pr2;
    char *p;
    char token[100]={'\0'};
    int token_code;
    char tmp_token[100] = {'\0'};
    int tmp_code;
    
    pr1 = T(current, tablelist);
    if(pr1 == NULL)
        return(NULL);
    
    //similar to *T()
    p = lex_anal(current, token, &token_code);
    while(token_code == '+'||token_code == '-'){
        current = lex_anal(current, token, &token_code);
        pr2 = new MyNode;
        pr2->node_type = (token_code == '+')? PLUS:MINUS;
        pr2->value.p[0] = pr1;
        
        if(isCompatible(pr2->node_type, pr1->node_type))
            pr2->value.p[1] = T(current, tablelist);
        else
            return NULL;
        
        if(pr2->value.p[1] == NULL)
            return NULL;
        if(pr2->value.p[0]->isFloat == true ||pr2->value.p[1]->isFloat == true)
            pr2->isFloat = true;
        if(isCompatible(pr2->node_type, pr2->value.p[1]->node_type))
            pr1 = pr2;
        else
            return NULL;

        p = lex_anal(current, tmp_token, &tmp_code);//to peek the next position
        if(tmp_code == '+'|| tmp_code == '-'){
            p = lex_anal(current, token, &token_code);
            continue;
        }
        else
            break;
    }
    return pr1;
}

/*==========================================================
    BF::= E RELOP E | NOT BF | ( BE )????
 ========================================================== */
struct MyNode *BF(char *& current, map<string, ScanTable*> &tablelist){
    struct MyNode *pr1, *pr2;
    char *p;
    char token[100]={'\0'};
    int token_code;
    
    //peek the next token
    p=lex_anal(current, token, &token_code);
    
    if(token_code == NOT){
        //read and pass this token
        current =lex_anal(current, token, &token_code);
        
        pr2 = new MyNode;
        pr2->node_type = NOT_;
        pr2->value.q = BF(current, tablelist);
        if(pr2 == NULL)
            return NULL;
        if(isCompatible(pr2->node_type, pr2->value.q->node_type))
            return pr2;
        else
            return NULL;
    }
    //try (BE) at first. if it failed, call *E() (PS:start from '(')
    else if(token_code == '('){
        //store postion of '('
        char tmp_token[1000];
        strcpy(tmp_token, current);
        
        current = lex_anal(current, token, &token_code);
        pr2 = BE(current, tablelist);
        if(pr2 != NULL){//if BE() success
            current = lex_anal(current, token, &token_code);
            if(token_code == ')')
                return pr2;
            else
                return NULL;
        }
        
        //arrived here if call BE() failed
        current=tmp_token;//back to '(' position
        pr1 = E(current, tablelist);//*E() can handle (). no need to wait for the accompanying ')'
        if(pr1 == NULL)
            return NULL;
        
        current = lex_anal(current, token, &token_code);
        pr2 = new MyNode;
        pr2->value.p[0] = pr1;
        
        if(pr1->node_type == STRING_CONST || pr1->node_type == CHAR_ATTR){
            if(token_code == '=' )
                pr2->node_type = EQSS;
            else if(token_code == '<')
                pr2->node_type =LSSS;
            else if(token_code == '>')
                pr2->node_type = LASS;
            else if(token_code == LESSOREQ)
                pr2->node_type = LSESS;
            else if(token_code == GREATEROREQ)
                pr2->node_type = LAESS;
            else if(token_code == NOTEQ)
                pr2->node_type = UEQSS;
            else
                return NULL;
        }
        else{
            if(token_code == '+')
                pr2->node_type = PLUS;
            else if(token_code == '-')
                pr2->node_type = MINUS;
            else if(token_code == '*')
                pr2->node_type = MULT;
            else if(token_code == '/')
                pr2->node_type = DIV;
            else if(token_code == '=' )
                pr2->node_type = EQ;
            else if(token_code == '<')
                pr2->node_type =LS;
            else if(token_code == '>')
                pr2->node_type = LA;
            else if(token_code == LESSOREQ)
                pr2->node_type = LSE;
            else if(token_code == GREATEROREQ)
                pr2->node_type = LAE;
            else if(token_code == NOTEQ)
                pr2->node_type = UEQ;
            else
                return NULL;
        }

        pr2->value.p[1] = E(current, tablelist);
        if(pr2->value.p[1] == NULL)
            return NULL;
        
        if(isCompatible(pr2->node_type, pr2->value.p[1]->node_type))
            pr1 = pr2;
        else
            return NULL;
        
        return pr1;
    }
    
    /*E RELOP E*/
    else{
        pr1 = E(current, tablelist);
        if(pr1 == NULL)
            return NULL;
        
        current = lex_anal(current, token, &token_code);
        pr2 = new MyNode;
        pr2->value.p[0]=pr1;
        if(pr1->node_type == STRING_CONST || pr1->node_type == CHAR_ATTR){
            if(token_code == '=' )
                pr2->node_type = EQSS;
            else if(token_code == '<')
                pr2->node_type =LSSS;
            else if(token_code == '>')
                pr2->node_type = LASS;
            else if(token_code == LESSOREQ)
                pr2->node_type = LSESS;
            else if(token_code == GREATEROREQ)
                pr2->node_type = LAESS;
            else if(token_code == NOTEQ)
                pr2->node_type = UEQSS;
            else
                return(NULL);
        }
        else{
            if(token_code == '+')
                pr2->node_type = PLUS;
            else if(token_code == '-')
                pr2->node_type = MINUS;
            else if(token_code == '*')
                pr2->node_type = MULT;
            else if(token_code == '/')
                pr2->node_type = DIV;
            else if(token_code == '=' )
                pr2->node_type = EQ;
            else if(token_code == '<')
                pr2->node_type =LS;
            else if(token_code == '>')
                pr2->node_type = LA;
            else if(token_code == LESSOREQ)
                pr2->node_type = LSE;
            else if(token_code == GREATEROREQ)
                pr2->node_type = LAE;
            else if(token_code == NOTEQ)
                pr2->node_type = UEQ;
            else
                return(NULL);
        }
        
        pr2->value.p[1] = E(current, tablelist);
        if(pr2->value.p[1] == NULL)
            return NULL;
        
        if(isCompatible(pr2->node_type, pr2->value.p[1]->node_type))
            pr1 = pr2;
        else
            return(NULL);
        
        return pr1;
    }
}

/*==========================================================
            BT ::= BF | BF [AND BF]...
 ========================================================== */
struct MyNode *BT(char *& current, map<string, ScanTable*> &tablelist){
    struct MyNode *pr1, *pr2;
    char *p;
    char token[100] = {'\0'};
    int token_code;
    
    pr1 = BF(current, tablelist);
    if(pr1 == NULL)
        return(NULL);
    
    p=lex_anal(current, token, &token_code);
    while(token_code == AND){
        current = lex_anal(current, token, &token_code);
        pr2 = new MyNode;
        pr2->node_type = AND_;
        pr2->value.p[0] = pr1;
        if(isCompatible(AND_, pr1->node_type))
            pr2->value.p[1] = BF(current, tablelist);
        else
            return NULL;
    
        if(pr2->value.p[1] == NULL)
            return NULL;
        
        if(isCompatible(AND_, pr2->value.p[1]->node_type))
            pr1 = pr2;
        else
            return(NULL);
        
        p = lex_anal(current, token, &token_code);
    }
     return pr1;
}

/*==========================================================
                BE ::= BT | BT [OR BT]
 ========================================================== */
struct MyNode *BE(char *& current, map<string, ScanTable*>& tablelist){
    struct MyNode *pr1, *pr2;
    char *p;
    char token[100];
    int token_code;
    
    pr1 = BT(current, tablelist);
    if(pr1 == NULL)
        return(NULL);
    
    p=lex_anal(current, token, &token_code);
    while(token_code == OR){
        current = lex_anal(current, token, &token_code);
        pr2 = new MyNode;
        pr2->node_type = OR_;
        pr2->value.p[0] = pr1;
        if(isCompatible(OR_, pr1->node_type))
            pr2->value.p[1] = BT(current, tablelist);
        else
            return(NULL);
        
        if(pr2->value.p[1] == NULL)
            return(NULL);
        
        if(isCompatible(OR_, pr2->value.p[1]->node_type))
            pr1 = pr2;
        else
            return(NULL);

        p = lex_anal(current, token, &token_code);
    }
    return pr1;
}

/*==================================================
    check whether the datatype is compatible or not
 ===================================================*/
bool isCompatible(int i, int j){
    /*i: arithmetic expr; j: arithmetic expr*/
    if(i == MULT||i == DIV || i == PLUS || i == MINUS || i == LS || i == LA || i == LSE || i == LAE || i == EQ || i == UEQ){
        if(j == MULT || j == DIV || j == PLUS || j == MINUS || j == INT_CONST || j == FLOAT_CONST || j == INT_ATTR || j == FLOAT_ATTR )
            return true;
        else{
            printf("Semantical Error: data type not compatible\n");
            return false;
        }
    }
    /*i: string; j: string*/
    if(i == EQSS || i == UEQSS || i == LSSS || i == LASS || i == LSESS || i == LAESS){
        if(j == STRING_CONST || j == CHAR_ATTR)
            return true;
        else{
            printf("Semantical Error: data type not compatible\n");
            return false;
        }
    
    }
    /*i:bool expr; j: bool expr*/
    if(i == AND_ || i == OR_ || i == NOT_){
        if(j == AND_ || j == OR_ || j == NOT_ || j == LS || j == LA || j == LSE || j == LAE || j == EQ || j == UEQ || j == LSSS || j == LASS || j == LSESS || j == LAESS || j == EQSS || j == UEQSS)
            return true;
        else{
            printf("Semantical Error: data type not compatible\n");
            return false;
        }
    }
    else{
        printf("Syntax error in isCompatible()\n");
        return false;
    }
}








