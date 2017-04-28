//
//  ParseCreate.cpp
//  SQL parser
//
//  Created by 王璇 on 17/2/1.
//  Copyright © 2017年 Xuan Wang. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexanal.h"
#include <stdlib.h>

//pay attention to the parameter list
int ParseCreate(char* tname,char *tb_info, int *size, char* p){
    
    int index=0;//to identify the position of new info
    int i=0;
    int attri_num=0;
    char token[100];    // Output for lexanal( )
    int  token_code=0;
    p=lex_anal( p, token, &token_code );//watch out for space
    
    if(token_code!=TABLE){
        printf("Error: keyword TABLE excepted");
        return 0;
    }
    
    //get next token--table name
    p=lex_anal( p, token, &token_code );
    if(token_code!=IDENTIFIER){
        printf("Error: table name expected");
        return 0;
    }
    else{
        //store table name
        for(i=0;i<strlen(token);i++){
            tname[i]=token[i];
        }
        tname[i]='\0';
        
    }
    
    //get next token--(
    p=lex_anal( p, token, &token_code );
    if(token_code!='('){
        printf("Error: ( expected");
        return 0;
    }
    
    do{
        //get next token--attribute name
        p=lex_anal( p, token, &token_code );
        if(token_code!=IDENTIFIER){
            printf("Error '%s': attribute name expected\n", token);
            return 0;
        }
        else{
            //store attribute name
            for(i=0;i<strlen(token);i++){
                tb_info[index+i]=token[i];
            }
            tb_info[index+i]=' ';
            index=index+(int)strlen(token)+1;
        }
        
        //get next token--data type(int|float|char)
        p=lex_anal( p, token, &token_code );
        if(strcasecmp(token, "INT")==0){
            token_code='I';
            size[attri_num]=sizeof(int);
            attri_num++;
            tb_info[index]='I';
            tb_info[index+1]=' ';
            index=index+2;
        }
        else if(strcasecmp(token, "FLOAT")==0){
            token_code='F';
            size[attri_num]=8;
            attri_num++;
            tb_info[index]='F';
            tb_info[index+1]=' ';
            index=index+2;
        }
        else if(strcasecmp(token, "CHAR")==0){
            //get next token--(
            p=lex_anal(p, token, &token_code);
            if(token_code!='('){
                printf("Error '%s': ( expected", token);
                return 0;
            }
            else{
                //get next token--number
                p=lex_anal(p, token, &token_code);
                if(token_code!=INTEGER_NUMBER){
                    printf("Error '%s': integer size expected", token);
                    return 0;
                }
                else{
                    token_code='C';
                    size[attri_num]=atoi(token)+1;
                    attri_num++;
                    tb_info[index]='C';
                    tb_info[index+1]=' ';
                    index=index+2;
                    
                    //get next token
                    p=lex_anal(p, token, &token_code);
                    if(token_code!=')'){
                        printf("Error '%s': ) expected",token);
                        return 0;
                    }
                }
            
            
            }
            
        }
        else{
            printf("Error: data type expected!");
            return 0;
        }
        
        //get next token
        p=lex_anal(p, token, &token_code);
        if(token_code !=','&& token_code !=')')
            return 0;
        
    }while(token_code==',');
    return 1;
    
}
