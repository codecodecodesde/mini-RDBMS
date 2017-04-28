#include <iostream>
#include <fstream> //ifstream ofstream
#include <sstream> //istringstream
#include "SelectNode.h"
#include "ScanTable.h"
#include "lexanal.h"
#include "MyNode.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <map>
//#include "SelectNode.h"

using namespace std;

int ParseCreate(char* tname, char *tb_info, int *size,char *p);
void DoCreate(FILE *cFile, char *tb_info,char *tname, int * size);
char ParseDrop(char check, char* tname, char* p, char* token, int token_code);
char *lex_anal(char *current_pos, char *token, int *token_type);
struct MyNode *BE(char *& current, map<string,ScanTable*> &tablelist);
void printToken( int code, char *token );
//int ParseSelect(struct SelectNode *env, char *p, map<string, ScanTable*> tablelist);
void DoSelect(struct MyNode *where, struct SelectNode *env, int currRel);
int ParseSelect(struct SelectNode *env, char* &p, map<string,ScanTable*> &tablelist, char* inp, int num_of_char);
void DoSelect(struct SelectNode *env, int currRel, int hasWhere);



int main(int argc, const char * argv[]){
    
    struct Data
    {
        char relname[30];
        char fieldname[30];
        char fieldtype;
        int  startpos;
        int  fieldsize;
    };
    

    char inp[10000];    //for user input command
    char token[100];    // Output for lexanal( )
    int  token_code;    // Output for lexanal( )
    int  num_of_char;   //number of character in the whole input command
    char *p = inp;      //moving pointer
    
    while (1){
        int result = 0;
        char check='O';
        char tname[100]={'\0'};
        map<string, ScanTable*> tablelist;
       
        cout << "Please input your SQL command:";

        num_of_char = read_command(inp);
        p = inp;
        
        //get the first token--CREATE|DROP|INSERT....
        p = lex_anal(p, token, &token_code);
       


        switch (token_code){
            
            case CREATE:{
                char *tb_info = new char[10000];
                int *size = new int[100];
                result=ParseCreate(tname, tb_info, size, p);
                
                if(result == 1){
                    FILE *cFile;
                    FILE *eFile;
                    cFile=fopen("catalog","a+");
                 
                    //to check if the table "tname" exist or not
                    ifstream infield(tname);
                    if(infield.good())
                        printf("Error: this table '%s' has already exist!\n",tname);
                    else{
                        //write new table to catalog and create a new empty table
                        DoCreate(cFile,tb_info,tname, size);
                        fclose(cFile);
                        eFile=fopen(tname, "w");
                        printf("The empty file '%s' has been created successfully!\n",tname);
                        fclose(eFile);
                    }
                }
                else
                    printf("Error detected during parsing!");
                delete[] tb_info;
                delete[] size;
                tb_info=NULL;
                size=NULL;
                break;
            }

            case DROP:{
                result=ParseDrop(check, tname, p, token, token_code);
              
                // no grammar error, do drop table command
                if(result=='Y'){
                  //rename the old catalog file, and open a new file named catalog to store new catalog
                  rename("catalog","catalog2");
                  ifstream origfile("catalog2");
                  ofstream newfile("catalog");
                  string line;

                  if(origfile.is_open()){
                      //read the original file line by line, check the first word in each line
                      //firstword !=tname: write this line into the new catalog file
                      while(getline(origfile, line)){
                          string firstword;
                          istringstream first(line);
                          first>>firstword;
                          if(firstword.compare(tname))
                              newfile<<line<<endl;
                      }
                      newfile.close();
                  }
                  origfile.close();
                    
                  //remove origial catalog file and remove the "tname" table
                  remove("catalog2");
                  if(remove(tname)!=0)
                      printf("Error: '%s' do not exist!\n",tname);
                  else
                      printf("The file '%s' is DELETED!\n",tname);
                }
                break;
            }
                
            case  INSERT:{
                Data CatelogInfo[10];
                char *buf=new char[1000];
                
                
                p=lex_anal( p, token, &token_code );
                if(token_code!=INTO){
                    printf("Error: keyword INTO expected\n");
                    check='N';
                    break;
                }
                
                p=lex_anal( p, token, &token_code );
                if(token_code!=TABLE){
                    printf("Error: keyword TABLE expected\n");
                    check='N';
                    break;
                }
                
                p=lex_anal( p, token, &token_code );
                if(token_code!=IDENTIFIER){
                    printf("Error: tablename expected\n");
                    check='N';
                    break;
                }
                else{
                    int i;
                    for(i=0;i<strlen(token);i++){
                        tname[i]=token[i];
                    }
                    tname[i]='\0';
                }
                
                p=lex_anal( p, token, &token_code );
                if(token_code!=VALUES){
                    printf("Error: keyword VALUES expected\n");
                    check='N';
                    break;
                }
                
                p=lex_anal( p, token, &token_code );
                if(token_code!='('){
                    printf("Error: ( expected\n");
                    check='N';
                    break;
                }
                
                //store the this relation datainfo from catelog
                FILE *fp;
                fp = fopen("catalog", "r");
                int n_fields=0;
                int record_size=0;
                while ( fscanf(fp, "%s %s %c %d %d",
                               CatelogInfo[n_fields].relname,CatelogInfo[n_fields].fieldname,
                               &CatelogInfo[n_fields].fieldtype, &CatelogInfo[n_fields].startpos,&CatelogInfo[n_fields].fieldsize) > 0){
                    
                    if(strcmp(CatelogInfo[n_fields].relname, tname)==0){
                        
                        if ( CatelogInfo[n_fields].startpos + CatelogInfo[n_fields].fieldsize >record_size ){
                            record_size = CatelogInfo[n_fields].startpos+ CatelogInfo[n_fields].fieldsize;
                        }
                        
                        n_fields++;
                    }
                }
                
                if(n_fields==0){
                    printf("Error:cannot find table '%s' info from catelog\n",tname);
                    break;
                }
             
                
                for(int i=0;i<n_fields;i++){
                    //get the first filed in insert command
                    p=lex_anal( p, token, &token_code );
                    if (CatelogInfo[i].fieldtype=='I') {
                        if(token_code==INTEGER_NUMBER||token_code==FLOAT_NUMBER){
                            int tmp=atoi(token);
                            memcpy(&buf[CatelogInfo[i].startpos], &tmp,sizeof(int));
                        }
                        else if(token_code==STRING){
                            printf("Error: semantical errors, %s cannot be converted to INT_NUMBER\n",token);
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                        else{
                            printf("Error: illegal input field value: %s\n",token);
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                       
                        
                    }
                    else if(CatelogInfo[i].fieldtype=='F'){
                        if(token_code==FLOAT_NUMBER||token_code==INTEGER_NUMBER){
                            double tmp=atof(token);
                            memcpy(&buf[CatelogInfo[i].startpos],&tmp ,sizeof(double));
                        }
                        else if(token_code==STRING){
                            printf("Error: semantical errors, string '%s' cannot be converted to FLOAT_NUMBER\n",token);
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                        else{
                            printf("Error: illegal input field value: %s\n",token);
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                    }
                    
                    else{
                        if(token_code==STRING){
                            string str=token;
                            if(str.size()<CatelogInfo[i].fieldsize){
                                memcpy(&buf[CatelogInfo[i].startpos], token,str.size()+1);
                            }
                            else
                                memcpy(&buf[CatelogInfo[i].startpos], token,CatelogInfo[i].fieldsize);
                        }
                        else if(token_code==INTEGER_NUMBER||token_code==FLOAT_NUMBER){
                            printf("Error: semantical errors, %s cannot be converted to String\n",token);
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                        else{
                            printf("Error: illegal input field value: %s\n",token);
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                    }
                    //to get , or ) after attribute values
                    p=lex_anal( p, token, &token_code );
                    if(i<n_fields-1){
                        if(token_code==')'){
                            printf("Error: lack of fields!\n");
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                        if(token_code!=','){
                            printf("Error: Syntactically Incorrect , expected\n");
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                    }
                    else{
                        if(token_code==','){
                            printf("Error: feilds redundance\n");
                            delete[] buf;
                            buf=NULL;;
                            break;
                        }
                        if(token_code!=')'){
                            printf("Error: Syntactically Incorrect ) expected\n");
                            delete[] buf;
                            buf=NULL;
                            break;
                        }
                        else{
                            buf[record_size]='Y';
                            continue;
                        }
                    }
                    
                }
             
                
                //write data in buf to file "tname"
                if(buf!=NULL){
                    ofstream fd(tname,ios::out|ios::binary|ios::app);
                    fd.write(buf,record_size+1);
                    fd.close();
                }
                
//                /*-------------------------------------------------------
//                 use print-data.cc to show the content of the binary file
//                 -------------------------------------------------------*/
//                ScanTable *f=new ScanTable();
//                f->Open( tname );
//               // f->PrintRelationInfo();
//                while ( f->GetNext() != 0 )
//                {
//                    /* ------------------
//                     Print it...
//                     ------------------ */
//                    f->PrintTuple();
//                }
//                
//                f->Close();

                
                
            break;
            }
            
            case DELETE:{
                
                //next token:TABLE
                p=lex_anal( p, token, &token_code );
                if(token_code != TABLE){
                    printf("Error: keyword: TABLE expected!");
                    break;
                }
                
                //next token:table name. store table name in tname[]
                p=lex_anal( p, token, &token_code );
                if(token_code!=IDENTIFIER){
                    printf("Error: tablename expected\n");
                    break;
                }
                else{
                    int i;
                    for(i=0;i<strlen(token);i++){
                        tname[i]=token[i];
                    }
                    tname[i]='\0';
                }
                
                //next token: WHERE
                int endMark=0;
                p=lex_anal( p, token, &token_code );
                if(token_code != WHERE){
                    if(p == inp + num_of_char)
                        endMark=1;
                    else {
                        printf("Error: keyword: WHERE expected!");
                        break;
                    }
                }
                
                //to parse where clause
                struct MyNode *where= new MyNode;
                string name(tname, strlen(tname));
                //map<string, ScanTable*> tablelist;
                ScanTable *R = new ScanTable();
                strcpy(R->true_Relname,tname);
                R->Open(R->true_Relname);
                tablelist.insert(pair<string, ScanTable*>(name,R));
                R->Close();
                if(endMark==0){//where clause exists
                    where = BE(p, tablelist);
                    if(where == NULL){
                        printf("error in parsing where clause\n");
                        break;
                    }
                }

                
                R->Open( tname );
                bool r = false;
                while(R->GetNext() != 0){
                    if(endMark==0){
                        r = where->eval_bool();
                        if(r == true){
                            R->buf[R->record_size]='N';
                            R->Write(R->fd, R->buf[R->record_size]);
                        }
                    }
                    else{
                        R->buf[R->record_size]='N';
                        R->Write(R->fd, R->buf[R->record_size]);
                    }
                    
                    R->PrintTuple();
                }
                R->Close();
                delete where;
                tablelist.clear();
            break;
                
            }
            case SELECT:{
                int resultMarker = -1;
                struct SelectNode *root = new SelectNode;
                tablelist = map<string, ScanTable*>();
                resultMarker = ParseSelect(root, p, tablelist, inp, num_of_char);
                if(resultMarker == -1){
                    printf("Error in parsing SELECT and FROM clause\n");
                    break;
                }
                //check where clause exists or not
                if(resultMarker == 1){
                    root->where = BE(p, tablelist);
                    if(root->where == NULL){
                        printf("Error in parsing parsing where clause\n");
                        break;
                    }
                }
                else if(resultMarker == 0)
                    root->where = NULL;
                else{
                    printf("Error: exist error in parsing From clause procession\n");
                    break;
                }
                
                for(int t = 0;  t < root->NAttrsExp; t++){
                    cout<<"       ";
                    for(int l = 0; l < strlen(root->AttrExp[t]); l++){
                        if(root->AttrExp[t][l] != ' ')
                            cout << root->AttrExp[t][l];
                    }
                }
                printf("\n---------------------------------------------------\n");
                DoSelect(root, 0, resultMarker);
                
                
            break;
            
            }


            default:
                cout << "Illegal command: " << token << endl;
                cout << "  if you can not see the illegal command, " << endl;
                cout << "  it means it has unvisible characters like blank\n";
        }

    }
}

