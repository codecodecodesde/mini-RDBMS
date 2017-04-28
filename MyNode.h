//
//  MyNode.h
//  SQL parser and interpreter
//
//  Created by 王璇 on 17/4/4.
//  Copyright © 2017年 Xuan Wang. All rights reserved.
//
#ifndef MYNODE_H
#define MYNODE_H

class MyNode{
    
public:
    int node_type;
    bool isFloat;
    union{
        int i;
        double f;
        char c[100];
        
        int *i_attr;
        double *f_attr;
        char *c_attr;
        
        struct MyNode *p[2];
        
        struct MyNode *q;
    }value;
    
    
    
    char *eval_string();
    
    double eval_num();
    
    bool eval_bool();

};

enum TYPE
{INT_CONST, FLOAT_CONST, STRING_CONST,
    INT_ATTR, FLOAT_ATTR, CHAR_ATTR,
    AND_, OR_, NOT_,//_ to avoid confliction with token_code
    EQ, EQSS,// = for num; = for ss
    LS, LSSS,// < for num; < for ss
    LA, LASS,// > for num; > for ss
    UEQ, UEQSS,// != for num; != for ss
    LSE, LSESS,// <= for num; <= for ss
    LAE, LAESS,// >= for num; >= for ss
    PLUS, MINUS,
    MULT, DIV
};

#endif
