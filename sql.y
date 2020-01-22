%{

int yylex();

#include "sql.h"
#include<stdio.h>
#include "y.tab.h"
#include"lex.yy.c"

#include<ctype.h>
#include<stdio.h>
#include<string.h>
#include <iostream>
using namespace std;
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <list>
#include <ctype.h>


typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yylex();
extern int yyparse();
extern FILE *yyin;
//extern YY_BUFFER_STATE yy_scan_buffer(char *str, int);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);


int main(int argc, char *argv[] )
{
  if(argc==2){
   string f=argv[1];
   size_t find_f=f.find("=");
   string f_open=f.substr(find_f+1);
   if(!openfile.is_open())
	{
	ofstream newfile;
	newfile.open ("catalog.txt");
	newfile << "File: catalog.txt\n";
	newfile << "----------------------------------------------------------\n";
  cout<<"total number of tables:0\n";
  newfile << "total number of tables:0\n";
  cout<<"total number of tables:0\n";
	newfile.close();

	}

  //readTables();
  output_write.open("output.txt");
 FILE *myfile = fopen(f_open.c_str(), "r");
	yyin = myfile;
	// parse through the input until there is no more:
	do {
	yyparse();
	} while (!feof(yyin)); output_write.close();


}else{
  printf("Enter \"quit\"; to exit\n");
   if(!openfile.is_open())
	{
	ofstream newfile;
	newfile.open ("catalog.txt");
	newfile << "File: catalog.txt\n";
	newfile << "----------------------------------------------------------\n";
  cout<<"total number of tables:0\n";
  newfile << "total number of tables:0\n";
  cout<<"total number of tables:0\n";
	newfile.close();

	}

  //readTables();
  output_write.open("output.txt");char sql[256];
   while(strcmp(sql, "quit;")!=0)
	{
	printf("SQL>");

                gets(sql);
	if(strcmp(sql, "quit")!=0)
	{YY_BUFFER_STATE buffer = yy_scan_buffer(sql,sizeof(sql));
	ins_count=0;
	recordsize=0;columns="";records=0;totalsize=0;
	insert_values="";
	check_table=0;
    	yyparse();
   	yy_delete_buffer(buffer);}

	}

output_write.close();
}



return 0;


}
void yyerror(const char* m)
{
  //cout << m << endl;
}


%}

%token SELECT
%token DISTINCT
%token FROM
%token WHERE
%token LE
%token GE
%token EQ
%token sum
%token NE
%token OR
%token AND
%token LIKE
%token GROUP
%token HAVING
%token ORDER
%token ASC
%token DESC
%token VARCHAR
%token ID ID_INSERT
%token NUM
%token TABLE CREATE IN SPACE INT CHAR into INTO VALUES INSERT PRIMARY_KEY QUIT ID_N TABLES SHOW DROP JOIN ON
%right '='
%left AND OR
%left '<' '>' LE GE EQ NE

%%

    S       : ST1';' {printf("----SQL----\n");}
	    | error S{ };
    ST1     : SELECT attributeList FROM tableList ST2
	{	select();
		//order();
		//checkSumAttr();
	}
               | SELECT DISTINCT attributeList FROM tableList ST2
	       
	       |{}	CREATE TABLE table_name'('column_list')'
	{createTable(); columns="";}
	       |INSERT INTO table_name VALUES'('column_values')'
	 {if(insertValidate()==true){
        printf("validate");
        insertValues();
        insert_values="";
    }
      else
      { printf("Not validated");
      insert_values="";
      }
      }
	       | INSERT INTO table_name'('columns')' VALUES'('column_values')'
	 { }
	       |INSERT INTO nest_insert insert_select { insertSelectValidate(); insert_values="";sel_col="";main_table="";tablename="";}
	       |SELECT attributeList FROM new_table JOIN new_table ON join_cond ST2{join_tables();}

	       | DROP TABLE table_name { }

	| SHOW TABLE table_name {show_table(); }
	
	

	| SHOW TABLES { showTables(); }

	       |QUIT {}


	       ;

    nest_insert: ID {main_table=$1;};
    new_table:ID{if(join==0){join=1;init_table="";init_table=$1;}else{next_table="";next_table=$1;}};

    join_cond:ID'.'ID'='ID'.'ID{table_name1=$1;table_name2=$5;init_ret_col=$3; next_ret_col=$7;};

    insert_select:SELECT attributeList FROM tableList ST2 {};
    ST2     : WHERE COND ST3{where_con=1;}
               | ST3
               ;
    ST3     : GROUP groupByList ST4
               | ST4
               ;
    ST4     : HAVING COND ST5
               | ST5
               ;
    ST5     : ORDER orderByList ST6{ order_by=1;}
               |
               ;
    ST6     : DESC
               | ASC
               |
               ;
  alias	    :ID{alias=$1;};
  attributeList :attributeList','ID {sel_col=sel_col+","+$3;sel_count=sel_count+1;}
               | '*' {/*cout<<"heradr"<<endl;*/ star_present=1;}
               | ID{sel_col=sel_col+$1;sel_count=sel_count+1;}
	       |attributeList','ID'.'ID{ sel_col=sel_col+","+$3+"."+$5;sel_count=sel_count+1;}
	       |ID'.'ID{sel_col=sel_col+$1+"."+$3;sel_count=sel_count+1;}
		   |attributeList','sum'('ID'.'ID')'{sumAttr = sumAttr+$5+"."+$7;sumAttrTable = "";sumAttrTable = $5;}
		   |sum'('ID'.'ID')'','attributeList{sumAttr = sumAttr+$3+"."+$5;sumAttrTable = "";sumAttrTable = $3;}
		   |attributeList','sum'('ID')'{sumAttr = sumAttr+$5;}
		   |sum'('ID')'','attributeList{sumAttr = sumAttr+$3;}
		   |sum'('ID'.'ID')'{sumAttr = sumAttr+$3+"."+$5;}
		   |sum'('ID')'{sumAttr = sumAttr+$3;}
               ;
	orderByList: ID{orderByColName = "";orderByColName = $1;}
				|ID'.'ID{orderByColName = "";orderByColName=orderByColName+$1+"."+$3;}
				|sum'('ID'.'ID')'{orderByColName = "";orderByColName = orderByColName+"sum"+'('+$3+"."+$5+')';}
				|sum'('ID')'{orderByColName = "";orderByColName = orderByColName+"sum"+'('+$3+')';/*cout<<"orderByColName in y "<<orderByColName<<endl;*/}
	groupByList:ID{groupByColName = $1;group_by = 1;}
				|ID'.'ID{groupByColName = "";groupByColName=groupByColName+$1+"."+$3;group_by = 1;}
  tableList    : table_name',' tableList{}
               | table_name
               ;
    COND    : COND OR COND
               | COND AND COND
               | E
               ;
    E         : F '=' G {/*cout<<ret_col<<"->"<<ret_value<<endl;*/asciiVal = 61;}
               | F '<' G {asciiVal = 60;}
               | F '>' G  {asciiVal = 62;}               
               | F LE G {asciiVal=63;}
               | F GE G{asciiVal=64;}
               | F EQ F
               | F NE G{asciiVal=65;}
               | F OR F 
               | F AND F 
               | F LIKE F 
               ;
    F         : ID {ret_col=$1;} 
				|ID'.'ID{join_where_tbl="";join_where_tbl = $1;join_where_col="";join_where_col = $3;ret_col="";ret_col = ret_col + $1+"."+$3;}
               ;
    G	      : '\''ID'\''{ret_value=$2;}
	      | NUM{ret_value=$1;}
	      ;
  table_name  : ID { list<tableDetails>::iterator Iterator;
		 for(Iterator = result.begin(); 
		   Iterator != result.end();
		   Iterator++)
		 {
		  if($1==(*Iterator).tname)
			{ check_table=1;}
		  }
		tablename=$1;store_tablename=$1;
		}
	       ;
  column_list : column_list','ID INT {string m=$4; transform(m.begin(), m.end(), m.begin(), ::toupper);
					columns=columns+","+$3+":"+m;recordsize=recordsize+4;}
		
	       |column_list','ID CHAR'('NUM')' {string m=$4; transform(m.begin(), m.end(), m.begin(), ::toupper);
						string a=$6;columns=columns+","+$3+":"+ m +"("+$6+")";recordsize=recordsize+atoi(a.c_str());}
	       |ID INT {string m=$2; transform(m.begin(), m.end(), m.begin(), ::toupper);
			columns=columns+$1+":"+m ;recordsize=recordsize+4;}
	       |ID CHAR'('NUM')' {string m=$2; transform(m.begin(), m.end(), m.begin(), ::toupper);
					string a=$4;columns=columns+$1+":"+ m +"("+$4+")";recordsize=recordsize+atoi(a.c_str());}
	       |column_list ',' PRIMARY_KEY'('ID')' {p_key=$5;}
               ;
  
  column_values: column_values',' '\''ID'\'' {string a=$4; insert_values=insert_values+",("+$4+")";ins_count=ins_count+1;}
		| '\''ID'\'' {string a=$2; insert_values=insert_values+"("+$2+")";ins_count=ins_count+1;}
		|column_values','NUM { string a=$3;int n=atoi(a.c_str());printf("%d\n",n);insert_values=insert_values+","+$3;ins_count=ins_count+1;}
		|NUM {string a=$1;int n=atoi(a.c_str());printf("%d\n",n);insert_values=insert_values+$1;ins_count=ins_count+1;}
		;
  columns	: columns','ID
		|ID 
		;
%%




