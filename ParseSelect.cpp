//
//  ParseFrom.cpp
//  SQL parser and interpreter
//
//  Created by 王璇 on 17/4/22.
//  Copyright © 2017年 Xuan Wang. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <cstring>
#include <map>
#include "lexanal.h"
#include "ScanTable.h"
#include "SelectNode.h"
using namespace std;

char *lex_anal(char *current_pos, char *token, int *token_type);
struct MyNode *E(char *&p, map<string, ScanTable*> &tablelist);

int ParseSelect(struct SelectNode *env, char* &p, map<string,ScanTable*> &tablelist, char* inp, int num_of_char){
    char token[100] = {'\0'};
    int token_code;
    int tmp_code;
    char* start;
    char* tmp;
    bool findFrom = false;
    bool findWhere = false;
    int m = 0;
    //char expr[1000] = {'\0'};
    env->NAttrsExp = 0;
    env->NRels = 0;
    
    start = p;
    while(p < inp + num_of_char){
        p = lex_anal(p, token, &token_code);
        if(token_code == FROM || token_code == ','){

            if(token_code == ','){
                strncpy(env->AttrExp[env->NAttrsExp], start, p - start - 1);
                env->NAttrsExp++;
                start = p;
            }
            else{
                findFrom = 1;
                strncpy(env->AttrExp[env->NAttrsExp], start, p - start - 4);
                m = p - start - 4;
                while(env->AttrExp[env->NAttrsExp][m-1] == ' '){
                    env->AttrExp[env->NAttrsExp][m - 1] = '\0';
                    m--;
                }
                env->NAttrsExp++;
                break;
            }
        }
    }
    
    if(findFrom == 0){
        printf("Error: FROM expected in SELECT command\n");
        return -1;
    }
    
    while(p < inp + num_of_char){
        
        p = lex_anal(p, token, &token_code);
        char tmp_token[100] = {'\0'};
        if(token_code == IDENTIFIER){
            env->Rel[env->NRels] = new ScanTable;
            strcpy(env->Rel[env->NRels]->true_Relname, token);//store ScanTable->true_Relname
            (env->Rel[env->NRels])->Open((env->Rel[env->NRels])->true_Relname);
            (env->Rel[env->NRels])->Close();
            strcpy(env->RelName[env->NRels],token);//store Relname:???optional
            
            //peek the next token: 1)alias 2) , 3)where 4) others
            tmp = lex_anal(p, tmp_token, &tmp_code);
            
            // 1)alias
            if(tmp_code == IDENTIFIER){
                p = lex_anal(p, token, &token_code);//read away alias
                string name(token, strlen(token));
                tablelist.insert(pair<string, ScanTable*>(name, env->Rel[env->NRels]));
                env->NRels++;
                
                //peek the next token: 1), 2)where 3)others
                tmp = lex_anal(p, tmp_token, &tmp_code);
                
                if(tmp_code == ','){
                    p = lex_anal(p, token, &token_code);//read away ','
                    continue;
                }
                else if(tmp_code == WHERE){
                    p = lex_anal(p, token, &token_code);//read away where
                    findWhere = 1;
                    break;
                }
                else{
                    printf("Syntex error: relname alias XX\n");
                    return -1;
                }
            }
            
            //2) ,
            else if(tmp_code == ','){
                string name(token, strlen(token));
                tablelist.insert(pair<string, ScanTable*>(name, env->Rel[env->NRels]));
                p = lex_anal(p, token, &token_code);//read away ','
                env->NRels++;
                continue;
            }
            //3) where
            else if(tmp_code == WHERE){
                string name(token, strlen(token));
                tablelist.insert(pair<string, ScanTable*>(name, env->Rel[env->NRels]));
                env->NRels++;
                p = lex_anal(p, token, &token_code);//read away where
                findWhere = 1;
                break;
            }
            //4)others
            else{
                p = lex_anal(p, token, &token_code);
                if(p == inp + num_of_char){
                    string name(token, strlen(token));
                    tablelist.insert(pair<string, ScanTable*>(name, env->Rel[env->NRels]));
                    env->NRels++;
                    break;
                }
                else{
                    printf("Syntex error in From clause\n");
                    return -1;
                }
            }
        }
        
        else{
            p = lex_anal(p, token, &token_code);
            if(p == inp + num_of_char){
                string name(token, strlen(token));
                tablelist.insert(pair<string, ScanTable*>(name, env->Rel[env->NRels]));
                env->NRels++;
                break;
            }
            else{
                printf("Syntex error in From clause\n");
                return -1;
            }
            
        }
    }
    
    //create attribute trees
    char* pr1 = env->AttrExp[0];
    pr1 = lex_anal(pr1, token, &token_code);
    if(token_code == '*'){
        if(env->NAttrsExp != 1){
            printf("Syntex error in SELECT clause: * XX\n");
            return -1;
        }
        else{
            env->NAttrsExp = 0;
            int num_AttrExpr = 0;
            for(int j = 0; j < env->NRels; j++){
                env->NAttrsExp += env->Rel[j]->n_fields;
                for(int t = 0; t < env->Rel[j]->n_fields; t++){
                    strcpy(env->AttrExp[num_AttrExpr], env->Rel[j]->DataDes[t].fieldname);
                    num_AttrExpr++;
                }
            }
        }
    }
    
    for(int i = 0; i < env->NAttrsExp; i++){
        env->selectExp[i] = new MyNode;
        char* pr2 = env->AttrExp[i];
        env->selectExp[i] = E(pr2, tablelist);
        if(env->selectExp[i] == NULL){
            printf("Error in attribute expression\n");
            return -1;
        }
    }
   
    
    if(findWhere == 1)
        return 1;
    else
        return 0;
    
}
    
    
