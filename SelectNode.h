//
//  SeleteNode.h
//  SQL parser and interpreter
//
//  Created by 王璇 on 17/4/8.
//  Copyright © 2017年 Xuan Wang. All rights reserved.
//

#include "ScanTable.h"
#include "MyNode.h"


class SelectNode{
public:
    int  NAttrsExp;             // Number of attrs
    char AttrExp[50][100];  // Attribute name
    struct MyNode *selectExp[100];// Buffer in ScanTable to get the attr value
    
    int  NRels;              // Number of relations
    char RelName[20][32];    // Relation name (optional, you can use
    // the TableName[ ] field in the ScanTable
    // to store the Relation Name...
    ScanTable *Rel[10];      // ScanTable object for the relation
    
    struct MyNode *where;             // Where clause
    
    /* =========================================
     The following 2 variables are used by
     the optional part of this project
     ========================================= */
    //SELECT *parent;       // Parent and Child are used to
    //SELECT *child;	    // implement nested queries
};


