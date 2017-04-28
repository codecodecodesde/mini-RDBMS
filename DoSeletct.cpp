//
//  DoSeletct.cpp
//  SQL parser and interpreter
//
//  Created by 王璇 on 17/4/8.
//  Copyright © 2017年 Xuan Wang. All rights reserved.
//

#include <stdio.h>
#include "MyNode.h"
#include "SelectNode.h"
#include <cmath>


void DoSelect(struct SelectNode *env, int currRel, int hasWhere){
    if (currRel < env->NRels)
    { /* -----------------------------------------------------
       We have not yet reached the last relation in the list
       ----------------------------------------------------- */
        //reset file relation[currRel].fd to beginning of file;
        env->Rel[currRel]->Open(env->Rel[currRel]->true_Relname);
        
        while ( env->Rel[currRel]->GetNext() != 0 )
        {
            //read one tuple from relation[currRel].fd
            if(env->Rel[currRel]->buf[env->Rel[currRel]->record_size] == 'N')
                continue;
            
            DoSelect(env, currRel+1, hasWhere);   // Recurse...
        }
        env->Rel[currRel]->Close();
    }
    else
    {
        if ( hasWhere == 1 && env->where->eval_bool() == true )
        {
            /* ================================================
             Print the SELECT clause
             ================================================ */
            for (int i = 0; i < env->NAttrsExp; i++)
            {
                //print result of Expression i in SELECT-clause;
                if(env->selectExp[i]->node_type == STRING_CONST || env->selectExp[i]->node_type == CHAR_ATTR){
                    printf("%10s ",env->selectExp[i]->eval_string());
                }
                else{
                    if(env->selectExp[i]->isFloat == true)
                        printf("%10.4f ",env->selectExp[i]->eval_num());
                    else{
                        printf("%10d  ",(int)env->selectExp[i]->eval_num());
                    }
                }
                //(Note: how you print depends on the type of result.
                //If it is INT, use printf("%10d".., for FLOAT, use printf("%10.4f", for CHAR, use printf("%20s"...) )
            }
            printf("\n");
            
        }
        else if(hasWhere == 0){
            
            for (int i = 0; i < env->NAttrsExp; i++)
            {
                //print result of Expression i in SELECT-clause;
                if(env->selectExp[i]->node_type == STRING_CONST || env->selectExp[i]->node_type == CHAR_ATTR){
                    printf("%10s ",env->selectExp[i]->eval_string());
                }
                else{
                    if(env->selectExp[i]->isFloat == true)
                        printf("%10.4f ",env->selectExp[i]->eval_num());
                    else{
                        printf("%10d  ",(int)env->selectExp[i]->eval_num());
                    }
                }
                //(Note: how you print depends on the type of result.
                //If it is INT, use printf("%10d".., for FLOAT, use printf("%10.4f", for CHAR, use printf("%20s"...) )
            }
            printf("\n");
 
        }
    }
}







