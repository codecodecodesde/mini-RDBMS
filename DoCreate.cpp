//
//  DoCreate.cpp
//  SQL parser and interpreter
//
//  Created by 王璇 on 17/2/3.
//  Copyright © 2017年 Xuan Wang. All rights reserved.
//
#include <stdio.h>
#include <cstdio>
#include <string>
using namespace std;

void DoCreate(FILE *cFile, char *tb_info, char *tname, int * size){
    
    int attr_num=0;
    int start_pos=0;
    for(int i=0;i<100;i++){
        if(size[i]!=0)
            attr_num++;
        else
            break;
    }
    char *current_pos=tb_info;
    char attr_name[100];
    char attr_type;
   
   
    for(int num=0;num<attr_num;num++){
        int j=0;
        while(*current_pos!=' '){
            attr_name[j]=*current_pos;
            current_pos++;
            j++;
        }
        attr_name[j]='\0';
        current_pos++;
        attr_type=*current_pos;
        current_pos=current_pos+2;
        if(start_pos==0){
            fprintf(cFile,"%s %s %c %d %d \n",tname,attr_name,attr_type,start_pos,size[num]);
            start_pos=start_pos+size[num];
        }
        else{
            if(attr_type=='I'){
                if(start_pos%4!=0)
                    start_pos=start_pos+(4-start_pos%4);
            }
            else if(attr_type=='F'){
                if(start_pos%8!=0)
                    start_pos=start_pos+(8-start_pos%8);
            }
            fprintf(cFile,"%s %s %c %d %d \n",tname,attr_name,attr_type,start_pos,size[num]);
            start_pos=start_pos+size[num];
        }
    }
   
}



