//
//  ParseDrop.cpp
//  SQL parser and interpreter
//
//  Created by 王璇 on 17/2/5.
//  Copyright © 2017年 Xuan Wang. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexanal.h"

char ParseDrop(char check, char* tname, char* p, char* token, int token_code){
    int i;
    //get next token--TABLE
    p=lex_anal( p, token, &token_code );//watch out for space
    
    if(token_code!=TABLE){
        printf("Error: keyword TABLE excepted");
        check='N';
        return check;
    }
    
    //get next token--table name
    p=lex_anal( p, token, &token_code );
    if(token_code!=IDENTIFIER){
        printf("Error: table name expected");
        check='N';
        return check;
    }
    else{
        //store table name
        for(i=0;i<strlen(token);i++){
            tname[i]=token[i];
        }
        tname[i]='\0';
        return check='Y';
    }
    
}
