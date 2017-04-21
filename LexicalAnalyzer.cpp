//
//  LexicalAnalyzer.cpp
//  lexical analyzer
//  THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
//  A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - Xuan Wang
//  Created by Xuan Wang on 17/1/13.
//  Copyright © 2017 Xuan Wang. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexanal.h"

using namespace std;

int read_command(char * input){
    *input='\0';
    char tmp[10000];
    int count=0;
    int sum=0;
    
    
    while(cin.getline(tmp,10000)){
        count=(int)strlen(tmp);
        if(count==2){
            if((tmp[0]=='g'||tmp[0]=='G')&&(tmp[1]=='o'||tmp[1]=='O'))
                break;
        }
        sum=sum+count;
        strcat(input,tmp);
        strcat(input," ");
        sum=sum+1;
    }
    
    strcat(input, "\0");
    return sum;
}


 char *lex_anal(char *current_pos, char *token, int *token_type){
    
    char *new_pos;
    while(*current_pos==' ')
         current_pos++;
   
     
    if(*current_pos=='\''){
        int slength=-1;
        
        do{
            slength++;
            *(token+slength)=current_pos[slength+1];
            
        }while(current_pos[slength+1]!='\''&& current_pos[1+slength] != '\0');
        
        if(current_pos[1+slength] == '\''){
            *(token+slength)='\0';
            *token_type=STRING;
            new_pos=current_pos+slength+2;
            return new_pos;
        }
        else{
            *token_type = '\'';
            *token = '\'';
            *(token+1)='\0';
            new_pos = current_pos+1;
            return new_pos;
        }
    }
    
    else{
        int token_length=0;
        
        for(int i=0;i<(int)strlen(current_pos);i++){
            if(current_pos[i]==' '){
                *(token+i)='\0';
                break;
            }
            else{
                
                *(token+i)=current_pos[i];
                token_length++;
            }
        }
        
        //token is a single character
        if(token_length==1){
            if(isdigit(*token))
                *token_type=INTEGER_NUMBER;
            else if(isalpha(*token))
                *token_type=IDENTIFIER;
            else
                *token_type=*token;// use ASCII code for single char token
            
            
            new_pos=current_pos+1;
            return new_pos;
        }
        //token is not a single character
        else{
            //use the type pf first chatacter as classification condition
            //if the first one is alpha, the token could be keyword or identifier
            if(isalpha(*token)){
                int allalpha=0;
                int alpha_num=0;
                int pos=0;
                //give checking conditions
                for(int i=0;i<token_length;i++){
                    if(isalpha(*(token+i)))
                        allalpha=allalpha+1;
                    else if(isdigit(*(token+i))||(*(token+i)=='_'))
                        alpha_num=alpha_num+1;
                    else{
                        pos=i;
                        *(token+i)='\0';
                        token_length=(int)strlen(token);
                        break;
                    }
                }
                //keyword or identifier
                if(allalpha==token_length){
                    char tmp[]=" ";
                    for(int i=0;i<token_length;i++){
                        tmp[i]=toupper(token[i]);
                    }
                    tmp[token_length]='\0';//
                    
                    if (strcmp( tmp, "ALL" ) == 0 )
                        *token_type=ALL;
                    
                    else if (strcmp( tmp, "AND" ) == 0 )
                        *token_type=AND;
                    
                    else if (strcmp( tmp, "ANY" ) == 0 )
                        *token_type=ANY;
                    
                    else if (strcmp( tmp, "AVG" ) == 0 )
                        *token_type=AVG;
                    
                    else if (strcmp( tmp, "AS" ) == 0 )
                        *token_type=AS;
                    
                    else if (strcmp( tmp, "BETWEEN" ) == 0 )
                        *token_type=BETWEEN;
                    
                    else if (strcmp( tmp, "BY" ) == 0 )
                        *token_type=BY;
                    
                    else if (strcmp( tmp, "CHAR" ) == 0 )
                        *token_type=CHAR;
                    
                    else if (strcmp( tmp, "CHECK" ) == 0 )
                        *token_type=CHECK;
                    
                    else if (strcmp( tmp, "CLOSE" ) == 0 )
                        *token_type=CLOSE;
                    
                    else if (strcmp( tmp, "COMMIT" ) == 0 )
                        *token_type=COMMIT;
                    
                    else if (strcmp( tmp, "COUNT" ) == 0 )
                        *token_type=COUNT;
                    
                    else if (strcmp( tmp, "CREATE" ) == 0 )
                        *token_type=CREATE;
                    
                    else if (strcmp( tmp, "DECIMAL" ) == 0 )
                        *token_type=DECIMAL;
                    
                    else if (strcmp( tmp, "DELETE" ) == 0 )
                        *token_type=DELETE;
                    
                    else if (strcmp( tmp, "DISTINCT" ) == 0 )
                        *token_type=DISTINCT;
                    
                    else if(strcmp( tmp, "DOUBLE" ) == 0 )
                        *token_type=DOUBLE;
                    
                    else if (strcmp( tmp, "DROP" ) == 0 )
                        *token_type=DROP;
                    
                    else if (strcmp( tmp, "EXISTS" ) == 0 )
                        *token_type=EXISTS;
                    
                    else if (strcmp( tmp, "FLOAT" ) == 0 )
                        *token_type=FLOAT;
                    
                    else if (strcmp( tmp, "FROM" ) == 0 )
                        *token_type=FROM;
                    
                    else if (strcmp( tmp, "GO" ) == 0 )
                        *token_type=GO;
                    
                    else if (strcmp( tmp, "GROUP" ) == 0 )
                        *token_type=GROUP;
                    
                    else if (strcmp( tmp, "HAVING" ) == 0 )
                        *token_type=HAVING;
                    
                    else if (strcmp( tmp, "IN" ) == 0 )
                        *token_type=IN;
                    
                    else if (strcmp( tmp, "INSERT" ) == 0 )
                        *token_type=INSERT;
                    
                    else if (strcmp( tmp, "INT" ) == 0 )
                        *token_type=INT;
                    
                    else if (strcmp( tmp, "INTO" ) == 0 )
                        *token_type=INTO;
                    
                    else if (strcmp( tmp, "IS" ) == 0 )
                        *token_type=IS;
                    
                    else if (strcmp( tmp, "LIKE" ) == 0 )
                        *token_type=LIKE;
                    
                    else if (strcmp( tmp, "MAX" ) == 0 )
                        *token_type=MAX;
                    
                    else if(strcmp( tmp, "MIN" ) == 0 )
                        *token_type=MIN;
                    
                    else if (strcmp( tmp, "NOT" ) == 0 )
                        *token_type=NOT;
                    
                    else if (strcmp( tmp, "NULL" ) == 0 )
                        *token_type=NULL0;
                    
                    else if (strcmp( tmp, "NUMERIC" ) == 0 )
                        *token_type=NUMERIC;
                    
                    else if (strcmp( tmp, "OF" ) == 0 )
                        *token_type=OF;
                    
                    else if (strcmp( tmp, "ON" ) == 0 )
                        *token_type=ON;
                    
                    else if (strcmp( tmp, "OR" ) == 0 )
                        *token_type=OR;
                    
                    else if (strcmp( tmp, "ORDER" ) == 0 )
                        *token_type=ORDER;
                    
                    else if (strcmp( tmp, "PRIMARY" ) == 0 )
                        *token_type=PRIMARY;
                    
                    else if (strcmp( tmp, "REAL" ) == 0 )
                        *token_type=REAL;
                    
                    else if (strcmp( tmp, "SCHEMA" ) == 0 )
                        *token_type=SCHEMA;
                    
                    else if (strcmp( tmp, "SELECT" ) == 0 )
                        *token_type=SELECT;
                    
                    else if (strcmp( tmp, "SET" ) == 0 )
                        *token_type=SET;
                    
                    else if (strcmp( tmp, "SOME" ) == 0 )
                        *token_type=SOME;
                    
                    else if (strcmp( tmp, "SUM" ) == 0 )
                        *token_type=SUM;
                    
                    else if (strcmp( tmp, "TABLE" ) == 0 )
                        *token_type=TABLE;
                    
                    else if (strcmp( tmp, "TO" ) == 0 )
                        *token_type=TO;
                    
                    else if (strcmp( tmp, "UNION" ) == 0 )
                        *token_type=UNION;
                    
                    else if (strcmp( tmp, "UNIQUE" ) == 0 )
                        *token_type=UNIQUE;
                    
                    else if (strcmp( tmp, "UPDATE" ) == 0 )
                        *token_type=UPDATE;
                    
                    else if (strcmp( tmp, "USER" ) == 0 )
                        *token_type=USER;
                    
                    else if (strcmp( tmp, "VALUES" ) == 0 )
                        *token_type=VALUES;
                    
                    else if (strcmp( tmp, "VIEW" ) == 0 )
                        *token_type=VIEW;
                    
                    else if (strcmp( tmp, "WHERE" ) == 0 )
                        *token_type=WHERE;
                    
                    else if (strcmp( tmp, "WITH" ) == 0 )
                        *token_type=WITH;
                    
                    else
                        *token_type=IDENTIFIER;
                    
                    if(*token_type!=IDENTIFIER){
                        for(int i=0;i<token_length;i++){
                            *(token+i)=toupper(token[i]);
                        }
                    }
                    
                    new_pos=current_pos+token_length;
                    return new_pos;
                    
                }
                
                //identifier(alpha num _)
                else if((allalpha+alpha_num)==token_length){
                    *token_type=IDENTIFIER;
                    new_pos=current_pos+token_length;
                    return new_pos;
                }
                //including other symble, and stop in position of the first other symble
                else{
                    *token_type=IDENTIFIER;
                    new_pos=current_pos+pos;
                    return new_pos;
                }
            }
            //if the first character is +, - or number, the token could be integer ot float
            else if(*token=='+'||*token=='-'||isdigit(*token)){
                int decimalPoint=0;
                int pos=1;
                for(int i=1;i<token_length;i++){
                    if(!isdigit(*(token+i))&&decimalPoint<1){
                        if(*(token+i)=='.'){
                            decimalPoint++;
                            pos++;
                        }
                        else{
                            *(token+i)='\0';
                            //token_length=1;
                            break;
                        }
                    }
                    else if(!isdigit(*(token+i))&&decimalPoint==1){
                        *(token+i)='\0';
                        break;
                    }
                    else
                        pos++;
                }
                if(pos==1)
                    *token_type=INTEGER_NUMBER;
                else if(decimalPoint!=0)
                    *token_type=FLOAT_NUMBER;
                else
                    *token_type=INTEGER_NUMBER;
                
                new_pos=current_pos+pos;
                return new_pos;
            }
            //if the first character is ., the token needs to be check the next character
            else if(*token=='.'){
                if(!isdigit(*(token+1))){
                    *token_type='.';
                    *(token+1)='\0';
                    new_pos=current_pos+1;
                    return new_pos;
                }
                else{
                    int pos=2;
                    for(int i=2;i<token_length;i++){
                        if(isdigit(*(token+i)))
                            pos++;
                        else{
                            *(token+i)='\0';
                            break;
                        }
                        
                    }
                    *token_type=FLOAT_NUMBER;
                    new_pos=current_pos+pos;
                    return new_pos;
                }
            }
            
            // if the first characte is <= >= =
            else if(*token=='>'||*token=='<'||*token=='='||*token=='!'){
                if(*token=='>'||*token=='<'||*token=='!'){
                    if(*(token+1)=='='){
                        *(token+2)='\0';
                        new_pos=current_pos+2;
                        if(*token=='<')
                            *token_type=LESSOREQ;
                        else if(*token=='>')
                            *token_type=GREATEROREQ;
                        else
                            *token_type=NOTEQ;
                        return new_pos;
                    }
                    else{
                        *(token+1)='\0';
                        new_pos=current_pos+1;
                        if(*token=='<')
                            *token_type='<';
                        else
                            *token_type='>';
                        return new_pos;
                    }
                }
                else{
                    *(token+1)='\0';
                    *token_type='=';
                    new_pos=current_pos+1;
                    return new_pos;
                }
            }
            else{
                *token_type=*token;
                *(token+1)='\0';
                new_pos=current_pos+1;
                return new_pos;
            }
            
            
        }
        
    }
}






    

    

    
