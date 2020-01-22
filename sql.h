#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <list> 
#include <algorithm>
#include <string>
#include <sstream>
#include<string.h>
#include "string"
#include <vector>
#include <iomanip>
#include<map>
# define YYSTYPE char*
using namespace std;
ofstream output_write;
ofstream myfile;
std::ostringstream sconv;
ifstream openfile("catalog.txt");
string name;
extern char* yytext;
string insert_values;
string main_table="";
string alias="";
string table_attr="";
string table_name1,table_name2;
struct tableDetails{
string tname;
string tcolumn;
string bytesize;
string tkey;
list<string> column;
list <string> cols;
int col_count;
int rsize=0;
int tsize=0;
int records=0;
};

struct insert_values_strct{

vector<string> eachValue;

}insert;
string nest_join="";int join_count=0;
int nested=0;int join=0;string init_table;string next_table;string init_ret_col;string next_ret_col;int t_attr=0;

int insertSel = 0;
int order_by=0;
int from_order_by=0;
string ins_tem="";
string sel_col;
int sel_count;
string insert_tvalues;
int recordsize;
int totalsize;
string columns;
string tablename;
string store_tablename;
string p_key="";
int records;
int insert_failed=0;
int check_table;
int ins_count;
int create_table=0;
int where_con=0;
int startPrint=-1;
int endPrint=-1;
string ret_col="";string ret_value="";string oper="";int asciiVal=-1;
string nest_col="";
int nest_count=0;
string nest_tab1="";
string nest_tab2=""; 
int check_nest_tab=0;;
list<tableDetails> result;
struct tableDetails tableDetailsStr;
int getRecordSize(string);
void writetofile();
bool insertValidate();
void updateCatalog();
void showTables();
void select();
void join_tables();
string returnCommandJoin(string);
string executeGrep(char *command);
void writeVectorToFile(vector<string> ,string );
string getCommand(string,int);
bool where(int);
fstream binaryFile;
map<int,int> charSizeMap;
map<string,vector<int> > sizeTypeMap;
map<string,int> cumMap;
map<string,int> cumMapMain;
vector<vector<string> > orderVector;
int orderByColInd=-1;
int	orderByIgnore = -1;
string orderByColName="";
string orderByColType = "";
int from_where=0;
fstream binSrchFile,binSrchOutFile;
int match=0;
static vector<int> visited;
string sumAttr = "";
string groupByColName = "";
int groupByCase = -1;
int from_groupBy = 0;
int group_by=0;
string orderByColName1="";
int join_grpBy = 0;
int star_present=0;
string join_where_tbl="",join_where_col = "";
string sumAttrTable = "";


void clear(){
	tablename="";
	startPrint=-1;
    endPrint=-1;
	insertSel = 0;
	orderVector.clear();
	orderByColType = "";
	orderByColName="";
	from_where=0;
	visited.clear();
	star_present=0;
	join_grpBy = 0;
	from_groupBy = 0;
	groupByCase = -1;
	order_by=0;
	orderByColName="";
	orderByColName1="";
	groupByColName="";
	sizeTypeMap.clear();
	cumMap.clear();
	sumAttr="";
	sel_col="";
	//cout<<"sel_col"<<sel_col<<endl;
	where_con=0;
	ret_col="";
	group_by=0;
	ret_value="";
	table_name1 = "";
	table_name2 = "";
}
void checkSumAttr(){
	if(sumAttr.length()!=0)
		cout<<"Attribute is "<<sumAttr<<endl;
	sumAttr="";
}
bool sortcol( const vector<string>& v1,	const vector<string>& v2 ) {
	//if(orderByColInd>=0){
		//cout<<"orderByColInd : "<<orderByColInd<<"  "<<v1[orderByColInd]<<"  "<<v2[orderByColInd]<<endl;
		if(orderByColType.compare("char")==0)
			return (v1[orderByColInd].compare(v2[orderByColInd])) < 0;
		else{
			
			int x,y;
			x=atoi(v1[orderByColInd].c_str());
			y=atoi(v2[orderByColInd].c_str());			
			return x < y;
		}
	//} 
	//else
		//return false;
}

inline void createTable(){
    string temp_table=tablename;
     ifstream f((temp_table.append("\.tbl")).c_str());
      if(f.good()){
          cout<<"table already exist";
		  clear();
          return ;
      }
    tableDetailsStr.tname = tablename;
    tableDetailsStr.tcolumn = columns;
    if(p_key!="")
      tableDetailsStr.tkey = p_key;
    else
      tableDetailsStr.tkey = "NULL";
    tableDetailsStr.rsize = getRecordSize(tableDetailsStr.tcolumn);
    tableDetailsStr.tsize = 0;
    tableDetailsStr.records = 0;
    writetofile();
    cout<<"table created successfully"<<endl;
    
     //access the 3rd line of catalog.txt to get the total number of tables count
    char *command = "head -3 catalog.txt|tail -1| grep total";
    
    //execute grep command
    string str = executeGrep(command);
    
    //take from the string str the total number of tables from the file catalog.txt
    str = str.substr(0,str.length()-1);
    //cout<<"str is "<<str<<endl;
    int findNbrOfTables = str.find(":");
    stringstream convert;
    
    size_t number_of_tables;
    convert<<str.substr(findNbrOfTables+1,(str.length()-findNbrOfTables-1));
    convert>>number_of_tables;
    
    
    //increment the count of toal number of tables by one and place it back in the catalog.txt
    //cout<<"number_of_tables are "<<number_of_tables<<endl;
    number_of_tables=number_of_tables+1;
    ostringstream oss;
    oss<<number_of_tables;
    string finalNbrOfTables = str.substr(0,findNbrOfTables+1).append(oss.str());
    //cout<<"finalNbrOfTables string is  "<<finalNbrOfTables<<endl;
    string sedHead = "sed -i '3s/^.*";
    string sedTail = ".*$/";
    sedTail.append(finalNbrOfTables).append("'/ catalog.txt");
    string sed = sedHead.append(str).append(sedTail);
    //cout<<"sed is : "<<sed<<endl;
    popen(sed.c_str(),"w");
    
    //store in the file the #table in the respective .tbl file 
    fstream file1;
     string temp1=tablename;
     string temp="\.tbl";
     string name=temp1.append(temp);
     file1.open(name.c_str(),ios::out|ios::app|ios::binary);
     
     if(!file1)
     {
      cout<<"Error in creating the file:"<<endl;
     }
     //cout<<"file write = "<<number_of_tables<<endl;
      file1.write(reinterpret_cast<const char *>(&number_of_tables), sizeof(number_of_tables));
      file1.close();
      
     //read from the binary file the stored #table 
      file1.open(name.c_str(),ios::in|ios::binary);
      size_t count_tbls1;
      file1.read(( char *)&count_tbls1, sizeof(count_tbls1));
      //cout<<"count is "<<count_tbls1<<endl;
      file1.close();
      columns="";
    
}
inline string executeGrep(char *command){
  string str=""; 
   char c = 0;
    FILE *fpipe;
    if (0 == (fpipe = (FILE*)popen(command, "r")))
    {
        perror("popen() failed.");
        exit(1);
    }
    //get the stream output into a string str
    while (fread(&c, sizeof c, 1, fpipe))
    {
        printf("%c", c);
        str+=c;
    }
return str;
}
inline int getRecordSize(string columns){
    size_t rsize=0;
    stringstream convert;
    string columnsTemp = columns;
    int index;
    while((index=columnsTemp.find(","))>=0) {
            int indexType;
        int strtIndex = columnsTemp.substr(0,index).find(":");
        string type = columnsTemp.substr(strtIndex,index-strtIndex);
        if((indexType=type.find("INT"))>=0){
            rsize = rsize+4;
        }
        else if( (indexType=type.find("CHAR"))>=0){
            int temp=0;
            convert<<type.substr(type.find("(")+1,(type.find(")")-type.find("(")-1));
            convert>>temp;
            convert.str("");
            convert.clear();
            rsize = rsize + temp;
        }
        columnsTemp = columnsTemp.substr(index+1,columnsTemp.length());
    }
    size_t commaInst = columnsTemp.find(",");
    if(commaInst==-1){

        int INTFind = columnsTemp.find("INT");
        int CHARFind = columnsTemp.find("CHAR");
        if(INTFind>=0){
            return rsize+4;
        }
        else if(CHARFind>=0){
            string charLength = columnsTemp.substr(columnsTemp.find("(")+1,(columnsTemp.find(")")-columnsTemp.find("(")-1));
            size_t temp;
            stringstream convert;
            convert<<charLength;
            convert>>temp;
            convert.str("");
            convert.clear();
            return rsize+temp;
        }
    }
}
void writetofile()
{
   
   ofstream file;
   file.open("catalog.txt",ios::app);
   
    
   if(!file)
   {
    cout<<"Error in creating the file:"<<endl;
   }
   else{
   file<<"Tablename="<<tableDetailsStr.tname<<endl;
   file<<"Columns="<<tableDetailsStr.tcolumn<<endl;
   file<<"Primary Key="<<tableDetailsStr.tkey<<endl;
   file<<"Record size="<<tableDetailsStr.rsize<<endl;
   file<<"Total size="<<tableDetailsStr.tsize<<endl;
   file<<"Records="<<tableDetailsStr.records<<endl;
   file.close();
   }
   
   
}

char * getWritable(string command){
    
    char * writable = new char[command.size() + 1];
      std::copy(command.begin(), command.end(), writable);
      writable[command.size()] = '\0';
      //cout<<writable<<endl;
    return writable;
}
int getLineOfFile(string tblName,int add){

      fstream file1;
      string temp1=tblName;
      string temp="\.tbl";
      string name=temp1.append(temp);
	  //cout<<name<<endl;
      file1.open(name.c_str(),ios::in|ios::binary);
	  if(!file1.is_open())
		  cout<<"main table file not opened"<<endl;
      size_t count_tbls1;
      file1.read(( char *)&count_tbls1, sizeof(count_tbls1));
      //cout<<"count is "<<count_tbls1<<endl;
      count_tbls1 = count_tbls1-1;
      size_t lineOfColumns = count_tbls1*6+add;
      file1.close();
return lineOfColumns;
}
string getCommand(string tblName,int add){
    
    
      size_t lineOfColumns = getLineOfFile(tblName, add);
      //cout<<"lineOfColums is "<<lineOfColumns<<endl;
      ostringstream oss;
      oss<<lineOfColumns;
      string grepHead = "head -";
      string grepTail = " catalog.txt|tail -1| grep =";
      string command = grepHead.append(oss.str()).append(grepTail);
    
return command;
}
vector<string> getVector(string str){
    size_t splitColsLine = str.find("=");
    string colsDef = str.substr(splitColsLine+1, (str.length()-splitColsLine-1));
    int lenCols,i=0;vector<string> vec;
    while((lenCols = colsDef.find(","))>0 && lenCols<colsDef.length()){          
          
          vec.push_back(colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1));   
    }
    
return vec;
}



//"char",strPrim,s_insert_value,byteIgnore,size_of_column
bool checkPrimaryKey(string type, string primKey,string primKeyVal, int byteIgnore, int size_of_column,int size_table1 ){
//"char","",string(strColVal),ignoreMain,sizeTypeMap[primKeyCol][1],recordSizeMain
	string tblName;
	if(insertSel==1){
		tblName = main_table;
	}
	else{
		tblName = tablename;
	}
	
	
	cout<<"#############"<<endl;
	ifstream file_table1;
	file_table1.open((tblName.append("\.tbl")).c_str(),ios::binary|ios::in|ios::ate);
	if(!file_table1){
	cout<<"file not found"<<endl;
	}
	int size=file_table1.tellg();
	file_table1.seekg(8,ios::beg);
	int pos_t1=file_table1.tellg();
	vector<string> table1_k1;
 
   //cout<<"type : "<<type<<endl;
   /*cout<<"primKey : "<<primKey<<endl;
   //cout<<"primKeyVal : "<<primKeyVal<<endl;
   //cout<<"byteIgnore : "<<byteIgnore<<endl;
   //cout<<"size_of_column : "<<size_of_column<<endl;
   //cout<<"size_table1: "<<size_table1<<endl;*/
 
 
	while(pos_t1<size){
		if(type.compare("char")==0){
   //"char",strPrim,s_insert_value,byteIgnore,size_of_column,recordSiz
   			char *strcolval=new char[100]();
			//char strcolval[size_of_column];
			file_table1.seekg(byteIgnore+pos_t1);
			file_table1.read(strcolval,size_of_column);
			table1_k1.push_back(string(strcolval));
			delete[] strcolval;
		}
		else{
    
			int intcolval;	
      
			file_table1.seekg(byteIgnore+pos_t1);
    
			file_table1.read(reinterpret_cast<char *>(&intcolval),size_of_column);
    
      ostringstream ss;
      ss<<intcolval;
    
			table1_k1.push_back(ss.str());
		}
		pos_t1=pos_t1+size_table1;
		
	}
	vector<string>::iterator it;
	it = find (table1_k1.begin(), table1_k1.end(), primKeyVal);
	if (it != table1_k1.end())
		return false;
	else
		return true;
	
	
	
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
inline bool insertSelectValidate(){
     int star;
     if(sel_col.compare("")==0)
        star = 1;
     else 
         star=0;
     
    string command = getCommand(main_table,5);
    string to_col = executeGrep(getWritable(command));
    to_col = to_col.substr(0,to_col.length()-1);
    //cout<<"string from catalog is "<<to_col<<endl;
    vector<string> to_type_vec ;
    size_t splitColsLine = -1;
    splitColsLine = to_col.find("=");
    string colsDef = "";
    colsDef = to_col.substr(splitColsLine+1, (to_col.length()-splitColsLine-1));
    int lenCols,i=0;
	//cout<<"colsdef"<<colsDef<<endl;
    while((lenCols = colsDef.find(","))>0 && lenCols<colsDef.length()){          
          to_type_vec.push_back(colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1));   
          
          colsDef = colsDef.substr(lenCols+1,colsDef.length());
		 // cout<<"colsdef:"<<colsDef<<endl;
    }
    to_type_vec.push_back(colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1));   
	//cout<<"**********"<<endl;
    for(int i=0;i<to_type_vec.size();i++){
                  //cout<<to_type_vec[i]<<endl;            
    }
	
    
    command = getCommand(tablename,5);
    string from_col = executeGrep(getWritable(command));
    from_col = from_col.substr(0,from_col.length()-1);
    //cout<<"string from catalog is "<<from_col<<endl;
    vector<string> from_type_vec;
    splitColsLine = from_col.find("=");
    colsDef = from_col.substr(splitColsLine+1, (from_col.length()-splitColsLine-1));
    string temp_sel_col ="";
    string temp_cols=colsDef;
    int test_value;
    if(star==1){
    while((test_value=temp_cols.find(","))>0){
        //cout<<"Entered while after star:"<<endl;
        string temp_colsdef=temp_cols.substr(0,colsDef.find(":"));
        temp_cols=temp_cols.substr(temp_cols.find(",")+1,colsDef.length()-colsDef.find(",")-1);
        //cout<<temp_cols<<endl;
        sel_col=sel_col.append(temp_colsdef).append(",");
    }
    sel_col=sel_col.append(temp_cols.substr(0,temp_cols.find(":")));
   // cout<<"star:"<<sel_col<<endl;
    }
    //cout<<"$$$$$$$$$$$$$$$$"<<sel_col<<endl;
     temp_sel_col = sel_col;
    int lenSelCol = -1;
    lenSelCol = temp_sel_col.find(",");
    string sel_colDef = "";
    sel_colDef = temp_sel_col.substr(0,lenSelCol);
	//cout<<colsDef<<endl;
	//cout<<"sel_colDef : "<<sel_colDef<<"  "<<(colsDef.find(sel_colDef))<<endl;
    while((lenCols = colsDef.find(sel_colDef))>=0 && lenCols<colsDef.length()){  
			//cout<<"inside while for checking "<<endl;
              string actualCol = "";
              actualCol = colsDef.substr(lenCols,colsDef.length()-lenCols);
			  //cout<<"actualCol :"<<actualCol<<endl;
			  int xy = actualCol.find(":");
             // cout<<"actualCol : "<<actualCol<<endl;
              //cout<<"sel_colDef :"<<sel_colDef<<endl;
              //if(actualCol.compare(sel_colDef)==0){
				  if(actualCol.find(",")!=string::npos)
					from_type_vec.push_back(actualCol.substr(xy+1, actualCol.find(",")-xy-1));
				else{
					from_type_vec.push_back(actualCol.substr(xy+1,actualCol.length()-xy));
				}
				  if(lenSelCol==-1 || lenSelCol>=temp_sel_col.length())
					  break;
                  temp_sel_col = temp_sel_col.substr(lenSelCol+1,temp_sel_col.length());
                  lenSelCol = temp_sel_col.find(",");
                  sel_colDef = temp_sel_col.substr(0,lenSelCol);
              //}
              //colsDef = colsDef.substr(lenCols+1,colsDef.length());
    }
/*	if((lenCols = colsDef.find(sel_colDef))>0 && lenCols<colsDef.length()){
		string actualCol = "";
		actualCol = colsDef.substr(0,colsDef.find(":"));
		if(actualCol.compare(sel_colDef)==0){
			from_type_vec.push_back(colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1));   
		}
	}*/
	//cout<<")))))))))))))))))))))))))))"<<endl;
	for(int i=0;i<from_type_vec.size();i++){
                  //cout<<from_type_vec[i]<<"\t"<<endl;            
    }
    if(to_type_vec.size()==from_type_vec.size()){
    
            //cout<<"length same and check"<<endl;
            for(int i=0;i<from_type_vec.size();i++){
                  //cout<<to_type_vec[i]<<"\t"<<from_type_vec[i]<<"\t"<<endl;    
                  if(from_type_vec[i].compare(to_type_vec[i])!=0){
                      cout<<"validation failed due to different column definitions"<<endl;
                      from_type_vec.clear();
                      to_type_vec.clear();
                      sel_col="";
                      return false;
                  }
                          
            }
            cout<<"validation successful"<<endl;
            from_type_vec.clear();
            to_type_vec.clear();
            insertSel=1;
            select();
            if(insert_failed==0)
            cout<<"Insertion Successful:"<<endl;
            else
            cout<<"Insertion Failed:"<<endl;
            insertSel=0;
			insert_failed=0;
            sel_col="";
            return true;
    }
    else{
        cout<<"validation failed. Number of columns mismatch"<<endl;
        from_type_vec.clear();
        to_type_vec.clear();
        sel_col="";
        return false;
    }
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
inline bool insertValidate(){
      //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&7start
      string temp_tbl = tablename;
      ifstream f((temp_tbl.append("\.tbl")).c_str());
      if(!f.good()){
          cout<<"table doesn't exist";
          return false;
      }
      
      //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&end
      string command = getCommand(tablename,5);
      string str = executeGrep(getWritable(command));
	  
	  command = getCommand(tablename,6);
      string strPrim = executeGrep(getWritable(command));
      int findEq = strPrim.find("=");
	  strPrim = strPrim.substr(findEq+1,strPrim.length()-2-findEq);
     
     
      command = getCommand(tablename,7);
      string recSize = executeGrep(getWritable(command));
	  recSize = recSize.substr(recSize.find("=")+1,recSize.length());
     //cout<<"rec size str : "<<recSize<<endl;
     int recordSize = atoi(recSize.c_str());
     //cout<<"rec size int : "<<recordSize<<endl;
	  
      //cout<<"string from catalog is "<<str<<endl;

      //parsing the columns= line from catalog.txt file
      //cout<<"insert_values is "<<insert_values<<endl;
      size_t splitColsLine = str.find("=");
      string colsDef = str.substr(splitColsLine+1, (str.length()-splitColsLine-1));
      //cout<<"colsDef is "<<colsDef<<endl;
      string temp_insert_values = insert_values;
      int lenCols,lenColsIns,i=0,byteIgnore = 0;
      while((lenCols = colsDef.find(","))>0 && (lenColsIns = temp_insert_values.find(","))>0 && lenCols<colsDef.length() && lenColsIns < temp_insert_values.length()){
        //cout<<"loop"<<i<<endl;
        int findType = colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1).find("(");
        //cout<<"findType is "<<findType<<endl;
        string temp_substring=temp_insert_values.substr(0,lenColsIns);
        string col_name = colsDef.substr(0,colsDef.find(":"));
        //cout<<"strPrim : "<<strPrim<<"tfhfhfhhfhf"<<endl;
        //cout<<"col_name : "<<col_name<<endl;
        if(findType>=0){
        //type is char. So, check for the length of the string with the value within ()
            //cout<<"character"<<endl;
            //cout<<temp_substring<<endl;
            string s_insert_value=temp_substring.substr(1,temp_substring.length()-2);
            //cout<<s_insert_value<<endl;
            int temp_length=s_insert_value.length();
            string s_colsDef_length=colsDef.substr(colsDef.find("(")+1,colsDef.find(")")-colsDef.find("(")-1);
            int size_of_column=atoi(s_colsDef_length.c_str());
            //cout<<"size of char column:"<<size_of_column<<endl;
            if(size_of_column>=temp_length){
                //cout<<"error char"<<endl;
				
        
        
				if(col_name.compare(strPrim)==0){
					
					if(!checkPrimaryKey("char",strPrim,s_insert_value,byteIgnore,size_of_column,recordSize)){
								return false;
					}
					
					
				}
                insert.eachValue.push_back(s_insert_value);
                charSizeMap[i] = size_of_column;
				
				byteIgnore+=size_of_column;
            
            }
            else
            return false;
        }
        else{
			
          //cout<<"integer"<<endl;
          //cout<<temp_substring<<endl;
          bool has_only_digits = (temp_substring.find_first_not_of( "0123456789" ) == string::npos);
          //cout<<has_only_digits<<endl;
          if(has_only_digits==true){
             //cout<<"error integer"<<endl;
			 if(col_name.compare(strPrim)==0){
					
					if(!checkPrimaryKey("int",strPrim,temp_substring,byteIgnore,4,recordSize)){
								return false;
					}
					
					
				}
              insert.eachValue.push_back(temp_substring);
			  byteIgnore+=4;
              
          }
          else
          return false;
        }
        colsDef = colsDef.substr(lenCols+1,colsDef.length());
        //cout<<"colsDef is "<<colsDef;
        temp_insert_values = temp_insert_values.substr(lenColsIns+1,temp_insert_values.length());
        //cout<<"temp_insert_values is "<<temp_insert_values<<endl;  
        i++;
      }
      int x;
      //cout<<"colsDef:"<<colsDef<<endl;
      //cout<<"temp_insert_values:"<<temp_insert_values<<endl;
      string col_name = colsDef.substr(0,colsDef.find(":"));
      if((x=colsDef.find("("))>0 && x<colsDef.length())
      {
          //cout<<"character"<<endl;
          string s_insert_value=temp_insert_values.substr(temp_insert_values.find("(")+1,temp_insert_values.find(")")-temp_insert_values.find("(")-1);
          //cout<<"s_insert : "<<s_insert_value<<endl;
          int temp_length=s_insert_value.length();
          //cout<<temp_length<<endl;
          string s_colsDef_length=colsDef.substr(colsDef.find("(")+1,colsDef.find(")")-colsDef.find("(")-1);
          
          int size_of_column=atoi(s_colsDef_length.c_str());
          //cout<<"size"<<size_of_column<<endl;
          if(size_of_column>=temp_length){
              
              
              if(col_name.compare(strPrim)==0){
					
        					if(!checkPrimaryKey("char",strPrim,s_insert_value,byteIgnore,size_of_column,recordSize)){
        								return false;
        					}
        					
        					
 				    }
              
              insert.eachValue.push_back(s_insert_value);
              charSizeMap[i++] = size_of_column;
          }
          else
          return false;
      }
      else{
          //cout<<"Integer"<<endl;
          //cout<<temp_insert_values<<endl;
          bool has_only_digits = (temp_insert_values.find_first_not_of( "0123456789" ) == string::npos);
          if(has_only_digits==true){
          
              if(col_name.compare(strPrim)==0){
					
        					if(!checkPrimaryKey("int",strPrim,temp_insert_values,byteIgnore,4,recordSize)){
        								return false;
        					}
        					
        					
				    }
              insert.eachValue.push_back(temp_insert_values.c_str());
          
          }
          else
            return false;
     }
        lenColsIns=temp_insert_values.find(",");
        int temp_test=colsDef.find(",");
        if((lenColsIns>=0)||(temp_test>=0)){

            return false; 
         }
     //cout << "myvector contains:";
    for (unsigned i=0; i<insert.eachValue.size(); i++)
        //std::cout << ' ' << insert.eachValue[i];
     
     return true;
}
void executeSed(int line,string oldVal,string newVal){
    
    string str_line;
    {
        ostringstream oss;
        oss<<line;
        str_line = oss.str();
    }
    string sedHead = "sed -i '";
    sedHead = sedHead.append(str_line);
    sedHead = sedHead.append("s/^.*");
    string sedTail = ".*$/";
    sedTail.append(newVal).append("'/ catalog.txt");
    string sed = sedHead.append(oldVal).append(sedTail);
    //cout<<"sed is : "<<sed<<endl;
    system(sed.c_str() );

    
}
void updateCatalog(string tblName){

    string command = getCommand(tblName,7);
    string stri_rsize = executeGrep(getWritable(command));
    int findEq = stri_rsize.find("=");
    string str_rsize = stri_rsize.substr(stri_rsize.find("=")+1,stri_rsize.length()-1-findEq);
    int rsize = atoi(str_rsize.c_str());
    
    
    command = getCommand(tblName,8);
    string stri_tsize = executeGrep(getWritable(command));
    findEq = stri_tsize.find("=");
    string str_tsize = stri_tsize.substr(findEq+1,stri_tsize.length()-1-findEq);
    int tsize = atoi(str_tsize.c_str());
    int newTSize = tsize+rsize;
    string newTsize;
    {  
        ostringstream oss;
        oss<<newTSize;
        string newV = oss.str();
        newTsize = stri_tsize.substr(0,stri_tsize.find("=")+1).append(newV);
    }
    
    
    command = getCommand(tblName,9);
    string stri_tRecs = executeGrep(getWritable(command));
    findEq = stri_tRecs.find("=");
    string str_tRecs = stri_tRecs.substr(findEq+1,stri_tRecs.length()-1-findEq);
    int trecs = atoi(str_tRecs.c_str());
    int newTRecs = trecs+1;
    string newTrecs;
    {  
        ostringstream oss;
        oss<<newTRecs;
        string newV = oss.str();
        newTrecs = stri_tRecs.substr(0,stri_tRecs.find("=")+1).append(newV);
    }
    
    
    
    int line;
    line = getLineOfFile(tblName,8);
    stri_tsize = stri_tsize.substr(0,stri_tsize.length()-1);
    executeSed(line,stri_tsize,newTsize);
    
    line = getLineOfFile(tblName,9);
    stri_tRecs = stri_tRecs.substr(0,stri_tRecs.length()-1);
    executeSed(line,stri_tRecs,newTrecs);    


}
void insertValues(){

    //cout << "myvector contains:";
    //for (unsigned i=0; i<insert.eachValue.size(); i++)
        //std::cout << ' ' << insert.eachValue[i];
    //std::cout <<tablename<< '\n';

    string temp=tablename;
    
    writeVectorToFile(insert.eachValue,temp);
    cout<<"insertion successful"<<endl;
    
    charSizeMap.clear();
    insert.eachValue.clear();
    //#############################################################################################
    updateCatalog(tablename);
    insert_values="";
    
    //###########################################################################################################
}

void writeVectorToFile(vector<string> vec,string temp){
    //cout<<"*****************************************************"<<endl;
    fstream binFileWrite;
    binFileWrite.open(temp.append("\.tbl").c_str(),ios::out|ios::binary|ios::app);
    if(!binFileWrite){
    cout<<"file not opened"<<endl;}
    vector <string> :: iterator i;
    size_t pos;
    //cout<<"printing vector"<<endl;
    for(i=vec.begin();i!=vec.end();++i){
        
       //cout<<*i<<"printing vector"<<"\t";
        //binFileWrite.write((char *)&(*i),sizeof(vec[0]));
        
    
    }
    for(map<int,int>::iterator it = charSizeMap.begin(); it != charSizeMap.end(); ++it) {
          cout << it->first <<" ->  "<<it->second<<"\n";
      }
      //cout<<"vec size "<<vec.size()<<endl;
    for(int j=0;j<vec.size();j++){
        
        if(charSizeMap.find(j)==charSizeMap.end()){
            //cout<<"inside integer printing"<<endl;
            int tempInsInt =  atoi(vec[j].c_str());
            //cout<<"sizeiof int print"<<sizeof(tempInsInt)<<endl;
            binFileWrite.write(reinterpret_cast<char *>(&tempInsInt),sizeof(tempInsInt));
        }
        else{
            //cout<<"inside character printing"<<endl;
            //cout<<"sizeiof char print"<<charSizeMap.find(j)->second<<endl;
            string store = vec[j];
            
            //cout<<"vector of j"<<store<<endl;
            char vector1[store.length()];
            for(int i=0;i<store.length();i++)
            {
              vector1[i]=store[i];
            }
            for(int i=0;i<sizeof(vector1);i++)
            {
               //cout<<"character:"<<vector1[i]<<endl;
            } 
            binFileWrite.write(store.c_str(),charSizeMap.find(j)->second);
        }
    
    }
    //cout<<"outside priting"<<endl;
    binFileWrite.close();
    
    
    //reading from the bin file
    binFileWrite.open(temp.c_str(),ios::in|ios::binary|ios::ate);
    if(!binFileWrite){
    cout<<"file not opened"<<endl;}
    
    size_t size = binFileWrite.tellg();
    binFileWrite.seekp(0);
    size_t a;int b;
    
    binFileWrite.read((char *)&a,sizeof(a));
    //cout<<"a is : "<<a<<endl;
   // binFileWrite.read((char *)&b,sizeof(b));
   // cout<<"b is : "<<b<<endl;
    //binFileWrite.read((char *)&b,sizeof(b));
    //binFileWrite.read((char *)&b,sizeof(b));
    //cout<<"b is : "<<b<<endl;
    //pos = binFileWrite.tellg();
    //cout<<"pos is : "<<pos<<endl;
    vec.clear();
    binFileWrite.close();
}
bool where(int pos){

    fstream binFile;
    string table=tablename;
    size_t count_tbls;
        binFile.open((table.append("\.tbl")).c_str(),ios::binary|ios::in|ios::ate);
        if(!binFile){
        cout<<"file not found"<<endl;
        }
    int size=binFile.tellg();
    binFile.seekg(pos,ios::beg);
    //cout<<"file pointer is at: "<<binFile.tellg()<<endl;




    if(!(cumMap.find(ret_col)==cumMap.end())){
        //cout<<"ignoring "<<cumMap.find(ret_col)->second<<" bytes \n";
        binFile.ignore(cumMap.find(ret_col)->second);
        //char* strColVal;
		char *strColVal=new char[100]();
		int intColVal;
        if(sizeTypeMap[ret_col][0]==0){
            //cout<<"getting char where"<<endl;
            binFile.read(strColVal,sizeTypeMap[ret_col][1]);
            if(asciiVal==61){
                if(string(strColVal).compare(ret_value)==0){
                return true;
                }
            }
            else if(asciiVal==60){
                if(string(strColVal).compare(ret_value)<0){
                return true;
                }
            }
            else if(asciiVal==62){
                if(string(strColVal).compare(ret_value)>0){
                return true;
                }
            }
            else
            return false;
        }
        else{
            //cout<<"getting int select"<<endl;
            binFile.read(reinterpret_cast<char *>(&intColVal),sizeTypeMap[ret_col][1]);
            if(asciiVal==61){
                if(intColVal==atoi(ret_value.c_str())){
                return true;
                }
            }
            else if(asciiVal==60){
                if(intColVal<atoi(ret_value.c_str())){
                return true;
                }
            }
            else if(asciiVal==62){
                if(intColVal>atoi(ret_value.c_str())){
                return true;
                }
            }
            else
            return false;
        }
		delete[] strColVal;
        
    }
	else
		return false;
}
void merge(int fileNumber,int recordSize,int numOfCols,vector<string> colNames){
	fstream orderedFile;
	string tempTable = tablename;
	tempTable = tempTable.append("ordered");
	//cout<<"tempTable : "<<tempTable<<endl;
	orderedFile.open(tempTable.c_str(),ios::binary|ios::out|ios::trunc);
	if(!orderedFile.good()){
		cout<<"output file not created"<<endl;
		clear();
		return;		
	}
	fstream file[fileNumber];
	int pos[fileNumber];
	int size[fileNumber];
	int minSize=0,minSizePos=-1;
	//cout<<"*****************************************************************"<<endl;
	for(int i=1;i<=fileNumber;i++){
		string temp = tablename;
		int x;
		ostringstream os ;
		os<<i;
		temp = temp.append("_temp").append(os.str());
		file[i-1].open(temp.c_str(),ios::in|ios::binary|ios::ate);
		size[i-1] = file[i-1].tellg();
		if(minSize>size[i-1]){
			minSize=size[i-1];
			minSizePos = i-1;
		}
		pos[i-1]=0;
		file[i-1].seekg(pos[i-1]);
		temp="";
	}
	int i=0;
	vector<int> skip;
	int prevIndex=-1;
	int skipFile;
	if(from_order_by==1){
		cout<<"\n\n";
		for(int g=0;g<colNames.size();g++){
			cout<<colNames[g]<<"\t";
		}
		cout<<"\n---------------------------------------------------------------------------"<<endl;
	}
	while(true){
		string minValue;int minValue1=-65536,minValueIndex=-1;
		skipFile = 0;
		//cout<<"merge while"<<endl;
		for(int j=0;j<fileNumber;j++){
			//cout<<"skipFile :"<<skipFile<<endl;
			if( (find(skip.begin(), skip.end(),j))!=skip.end()){
				//cout<<j<<" file skipped"<<endl;
				continue;
			}
			//cout<<"file["<<j<<"].tellg() "<<file[j].tellg()<<endl;
			file[j].ignore(orderByIgnore);
			map<int,int>::iterator ite = charSizeMap.find(orderByColInd);
			if (ite != charSizeMap.end()){
				char *strColVal=new char[charSizeMap[orderByColInd]+1]();
				file[j].read(strColVal,charSizeMap[orderByColInd]);
				if(skipFile==0){
				  minValue = string(strColVal);
				  minValueIndex=j;
				  skipFile=1;
				}
				else{

					if(minValue.compare(string(strColVal))>0){
						minValue="";
						minValue = string(strColVal);			
						minValueIndex = j;
					}

				}
				delete[] strColVal;	
			}
			else{
				int intColVal;
				file[j].read(reinterpret_cast<char *>(&intColVal),4);
				//cout<<"*********************"<<intColVal<<endl;
				if(skipFile==0){
				  //cout<<"*********************"<<intColVal<<endl;
				  minValue1 = intColVal;
				  minValueIndex=j;
				  skipFile=1;
				}
				else{

					if(minValue1>intColVal){
						minValue1 = intColVal;		
						minValueIndex = j;
					}

				}
				
			}
			file[j].seekg(pos[j]);	
		}
		//cout<<"#############################################"<<minValue1<<"      "<<minValueIndex<<endl;
		for(int k=0;k<numOfCols;k++){
			
			map<int,int>::iterator ite = charSizeMap.find(k);
			if (ite != charSizeMap.end()){
				char *strColVal=new char[charSizeMap[k]+1]();
				file[minValueIndex].read(strColVal,charSizeMap[k]);
				orderedFile.write(strColVal,charSizeMap[k]);
				if(from_order_by==1)
					cout<<string(strColVal)<<"\t";
			}
			else{
				int intColVal;
				file[minValueIndex].read(reinterpret_cast<char *>(&intColVal),4);
				orderedFile.write(reinterpret_cast<char *>(&intColVal),4);	
				if(from_order_by==1)
					cout<<(intColVal)<<"\t";
			}
		
		
		}
		if(from_order_by==1)
			cout<<"\n";
		pos[minValueIndex]=file[minValueIndex].tellg();
		if(pos[minValueIndex]==size[minValueIndex]){
			//cout<<"pushing file "<<minValueIndex<<endl;
			skip.push_back(minValueIndex);
			file[minValueIndex].close();
		}
			
		if(skip.size()>=(fileNumber-1))
			break;
		minValue="";
		minValue1=-65536;
		prevIndex = minValueIndex;
		minValueIndex=-1;
	}
	
	for(int l=0;l<fileNumber;l++){
	if( (find(skip.begin(), skip.end(),l))!=skip.end()){
			continue;
	}
	prevIndex = l;
	}
	//cout<<"prevIndex :"<<prevIndex<<endl;
	int minValueIndex = prevIndex;
	if(fileNumber==1 && (!file[0].is_open())){
		skip.clear();
		orderedFile.close();
		clear();
		return;
	}
	while(true){
		for(int k=0;k<numOfCols;k++){
			
			map<int,int>::iterator ite = charSizeMap.find(k);
			if (ite != charSizeMap.end()){
				char *strColVal=new char[charSizeMap[k]+1]();
				file[minValueIndex].read(strColVal,charSizeMap[k]);
				orderedFile.write(strColVal,charSizeMap[k]);
				if(from_order_by==1)
					cout<<string(strColVal)<<"\t";
			}
			else{
				int intColVal;
				file[minValueIndex].read(reinterpret_cast<char *>(&intColVal),4);
				orderedFile.write(reinterpret_cast<char *>(&intColVal),4);	
				if(from_order_by==1)
					cout<<(intColVal)<<"\t";
			}
		
		
		}
		if(from_order_by==1)
			cout<<"\n";
		pos[minValueIndex]=file[minValueIndex].tellg();
		if(pos[minValueIndex]==size[minValueIndex]){
			//skip.push_back(minValueIndex);
			file[minValueIndex].close();
			break;
		}
	}
	/*for(int i=1;i<=fileNumber;i++){
		file[i-1].close();
	}*/
	//cout<<"*****************************************************************"<<endl;
	skip.clear();
	orderedFile.close();
}

void order(string tblName,vector<string> colNames){
		//cout<<"joinTbl in order "<<tblName<<endl;
		for(int c=0;c<colNames.size();c++){
			//cout<<colNames[c]<<endl;
			//cout<<sizeTypeMap[colNames[c]][0]<<endl;
		}
		//cout<<"printed colNames"<<endl;
		
		
		int recordSize=0;int index=-1;
		//int orderColInd = sel_col.find_last_of(",");
		//string orderByCol = sel_col.substr(orderColInd+1,sel_col.length()-(orderColInd+1));
		//cout<<"orderByCol : "<<orderByCol<<endl;
		string orderByCol = orderByColName;
		//cout<<"orderByCol : "<<orderByCol<<endl;
		for(int c=0;c<colNames.size();c++){
			if(sizeTypeMap[colNames[c]][0]==0){
				charSizeMap[c]=sizeTypeMap[colNames[c]][1];
				//cout<<"sizeTypeMap[colNames[c]][1]; :"<<sizeTypeMap[colNames[c]][1]<<endl;
			}
			if(orderByCol.compare(colNames[c])==0){
				orderByColInd = c;
				orderByIgnore = recordSize;
				if(sizeTypeMap[colNames[c]][0]==0){
					orderByColType="char";
				}
				else{
					orderByColType = "int";
				}
			}
			recordSize+=sizeTypeMap[colNames[c]][1];
		}
		//cout<<"recordSize : "<<recordSize<<"  "<<orderByColType<<endl;
		index = colNames.size();
		
		
		
		
		/*string command = getCommand(tablename,7);
		string recSize = executeGrep(getWritable(command));
		recSize = recSize.substr(recSize.find("=")+1,recSize.length());
		//cout<<"rec size str : "<<recSize<<endl;
		int recordSize = atoi(recSize.c_str());
		//cout<<"&&&&&&&&&&& recordSize : "<<recordSize<<endl;

		command = getCommand(tablename,5);
		string str = executeGrep(getWritable(command));
		//str = str.substr(str.find("=")+1,str.length()-str.find("=")-2);
		
		size_t splitColsLine = str.find("=");
		string colsDef = str.substr(splitColsLine+1, (str.length()-splitColsLine-1));
		string temp_colsDef = colsDef;
		int val=0,lenCols,index=0;
		while((lenCols = colsDef.find(","))>0 && lenCols<colsDef.length()){
			int findType = colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1).find("(");
			string colName = colsDef.substr(0,colsDef.find(":"));
			if(colName.compare(orderByCol)==0){
				orderByColInd = index;
				orderByIgnore = val;
			}
			if(findType>=0){
				
				string s_colsDef_length=colsDef.substr(colsDef.find("(")+1,colsDef.find(")")-colsDef.find("(")-1);
				int size_of_column=atoi(s_colsDef_length.c_str());
				cumMap[colName] = val;
				sizeTypeMap[colName].push_back(0);
				sizeTypeMap[colName].push_back( size_of_column) ;
				val = val+size_of_column;     
				charSizeMap[index]=size_of_column;
			}
			else{
				
				cumMap[colName] = val;
				sizeTypeMap[colName].push_back(1);
				sizeTypeMap[colName].push_back(4) ;
				val = val+4; 
				
			}
			colsDef = colsDef.substr(lenCols+1,colsDef.length());
			//cout<<"colsDef is "<<colsDef<<endl;
			index++;
		}
		int findType = colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1).find("(");
		string colName = colsDef.substr(0,colsDef.find(":"));
		if(colName.compare(orderByCol)==0){
				orderByColInd = index;
				orderByIgnore = val;
			}
		if(findType>=0){
			string s_colsDef_length=colsDef.substr(colsDef.find("(")+1,colsDef.find(")")-colsDef.find("(")-1);
			int size_of_column=atoi(s_colsDef_length.c_str());
			cumMap[colName] = val;
			sizeTypeMap[colName].push_back(0);
			sizeTypeMap[colName].push_back( size_of_column) ;
			val = val+size_of_column;              
			charSizeMap[index]=size_of_column;
		}
		else{
			
			cumMap[colName] = val;
			sizeTypeMap[colName].push_back(1);
			sizeTypeMap[colName].push_back(4) ;
			val = val+4; 
			
		}
		index++;*/
		//cout<<"orderByColInd "<<orderByColInd<<endl;
		//cout<<"index : " <<index<<endl;
		map<int,int> ::const_iterator it;
		for(it = charSizeMap.begin(); it != charSizeMap.end(); it++){
			//cout << it->first << " -> " << it->second <<"\n";
		}
		
		fstream binFile;
		string table=tblName;
		//cout<<"from_where :"<<from_where<<"  from_groupBy:"<<from_groupBy<<endl;
		if(from_where==1||from_groupBy==1){
			table=table.append("\.tbl");
		}
		cout<<"table "<<table<<endl;
		size_t count_tbls;
		binFile.open((table).c_str(),ios::binary|ios::in|ios::ate);
		if(!binFile){
			cout<<"order file not found"<<endl;
			clear();
			return;
		}
		int size=binFile.tellg();
		int pos,fileStart;
		if(from_where==1||from_groupBy==1){
			fileStart=8;
		}
		else{
			fileStart=0;
		}
		//cout<<"fileStart : "<<fileStart<<endl;
		pos = fileStart;
		binFile.seekg(pos);
		//cout<<"size:"<<size<<endl;
		//binFile.seekg(sizeof(count_tbls),ios::beg);
		//cout<<"pos:"<<pos<<endl;
		pos=binFile.tellg();
		int fileNumber=0,end;
		while(size>fileStart){
			//cout<<"in while1"<<endl;
			int temp_size=100/recordSize;
			//cout<<"temp_size*recordSize "<<temp_size*recordSize<<endl;
			if(size > temp_size*recordSize){
				end = temp_size*recordSize;
			}
			else{
				end = size;
			}
			size = size - end;
			//cout<<"size left "<<size<<endl;
			int rows = 0;
			while(end > fileStart /*|| (end==0 && size>0)*/){
				vector<string> vect;
				//cout<<"in while2"<<endl;
				//cout<<"end is "<<end<<endl;
				int columns=0;
				for (int it = 0;it<index;it++){
					//cout<<"in for"<<it<<endl;
					
					int intColVal;
					//cout<<"iter strt"<<endl;
					map<int,int>::iterator ite = charSizeMap.find(columns);
					//cout<<"ite != charSizeMap.end() "<< (ite != charSizeMap.end())<<endl;
					  if (ite != charSizeMap.end()){
							//cout<<"in Char "<<endl;
							char *strColVal=new char[charSizeMap[it]+1]();
							//cout<<"charSizeMap[it] "<<charSizeMap[it]<<endl;
							//cout<<"strColVal before :"<<string(strColVal)<<"   pos : "<<(binFile.tellg())<<endl;
						  binFile.read(strColVal,charSizeMap[it]);
						  //cout<<"strColVal after :"<<string(strColVal)<<"   pos : "<<(binFile.tellg())<<endl;
						  vect.push_back(string(strColVal));
							delete[] strColVal;
						  //cout<<"strColVal end :"<<string(strColVal)<<endl;
						  //orderVector[rows][columns] = string(strColVal);
						  //cout<<" orderVector[rows][columns] "<<vect[columns]<<endl;
					  }
					  else{
						  //cout<<"in int "<<endl;
						  binFile.read(reinterpret_cast<char *>(&intColVal),4);
						  //cout<<"intColVal : "<<intColVal<<endl;
						  ostringstream oss;
						  oss<<intColVal;
						  //cout<<"oss.str() : "<<oss.str()<<endl;
						  //cout<<"rows : "<<rows<<"  columns : "<<columns<<endl;
						  vect.push_back(oss.str());
						  //orderVector[rows][columns].push_back(oss.str());
						  //cout<<" orderVector[rows][columns] "<<vect[columns]<<endl;
					  }
						  
					//std::cout << it->first << " => " << it->second << '\n';
					columns++;
					
				}
				orderVector.push_back(vect);
				rows++;
				vect.clear();
				end = end - recordSize;
			}
			//cout<<"Before sort"<<endl;
			for (int i = 0; i < orderVector.size(); i++)
			{
				for (int j = 0; j < orderVector[i].size(); j++)
				{
					//cout << orderVector[i][j]<<"\t";
				}
				//cout<<"\n"<<endl;
			}
			//cout<<"order by col  "<<orderByColName<<endl;
			sort(orderVector.begin(), orderVector.end(),sortcol);
			
			//cout<<"After sort"<<endl;
			fileNumber++;
			ostringstream os;
			os<<fileNumber;
			string tempTable = tablename;
			tempTable = tempTable.append("_temp").append(os.str());
			fstream outFile;
			outFile.open(tempTable.c_str(),ios::binary|ios::out|ios::trunc);
			for (int i = 0; i < orderVector.size(); i++)
			{
				for (int j = 0; j < orderVector[i].size(); j++)
				{
					map<int,int>::iterator ite = charSizeMap.find(j);
					//cout<<"ite != charSizeMap.end() "<< (ite != charSizeMap.end())<<endl;
					  if (ite != charSizeMap.end()){
						  
						outFile.write(orderVector[i][j].c_str(),charSizeMap[j]);
					  }
					  else{
						  stringstream num(orderVector[i][j]);
						  int x=0;
						  num>>x;
						  outFile.write(reinterpret_cast<char *>(&x),4);
					  }
					//cout << orderVector[i][j]<<"\t";
				}
			 //cout<<"\n"<<endl;
			}
			outFile.close();
			tempTable="";
			
			orderVector.clear();

		}
		/*cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;
		//cout<<fileNumber<<endl;
		//cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;*/
		merge(fileNumber,recordSize,index,colNames);
		charSizeMap.clear();
		orderByColInd=-1;
		orderByIgnore = -1;
		orderByCol="";
		//sel_col="";
		binFile.close();
		fileNumber=0;
		table="";
		tblName="";
}
void search(int start,int end,int mid,int recordSize,vector<string> colNames){
	
	
	binSrchFile.seekg(mid);
	int pos = binSrchFile.tellg();
	//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<"  "<<pos<<"    "<<cumMap[ret_col]<<"   "<<ret_col<<endl;
	binSrchFile.ignore(cumMap[ret_col]);
	//cout<<binSrchFile.tellg()<<endl;
	if(sizeTypeMap[ret_col][0]==0){
		//cout<<"in char"<<endl;
		char *strColVal=new char[sizeTypeMap[ret_col][1]+1]();
		binSrchFile.read(strColVal,sizeTypeMap[ret_col][1]);
		if(string(strColVal).compare(ret_value)==0){
			match=1;
			
			vector<int>::iterator it;
			it = find (visited.begin(), visited.end(), pos);
			if (it == visited.end()){
				visited.push_back(pos);
				if(asciiVal==61||asciiVal==63||asciiVal==64){
					binSrchFile.seekg(pos);
					for(int h=0;h<colNames.size();h++){
						if(sizeTypeMap[colNames[h]][0]==0){
							char *strColVal1=new char[sizeTypeMap[colNames[h]][1]+1]();
							//char *strColVal1=new char[100]();
							binSrchFile.read(strColVal1,sizeTypeMap[colNames[h]][1]);
							binSrchOutFile.write(strColVal1,sizeTypeMap[colNames[h]][1]);
							//cout<<string(strColVal1)<<"\t";
							delete[] strColVal1;
						}
						else{
							int intColVal1;
							binSrchFile.read(reinterpret_cast<char *>(&intColVal1),4);
							binSrchOutFile.write(reinterpret_cast<char *>(&intColVal1),4);
							//cout<<intColVal1<<"\t";
						}
					}
				//cout<<"\n"<<endl;
				}
			}
			//upPos = checkDupUpperHalf(tblName,pos,recordSize);
			//downPos = checkDupLowerHalf(tblName,pos,recordSize);
			
			
			int end1 = end;
			int mid1 = mid;
			end = mid;
			mid = (((start+end)/2)/recordSize)*recordSize;
			if( end==mid){
				//cout<<"breaking"<<endl;
				//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
				return;
			}
			search(start,end,mid,recordSize,colNames);
			//cout<<"equal 2nd part"<<endl;
			start = mid1;
			end = end1;
			mid = (((start+end)/2)/recordSize)*recordSize;
			if(start==mid ){
				//cout<<"breaking"<<endl;
				//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
				return;
			}

			search(start,end,mid,recordSize,colNames);
		}
		else if(string(strColVal).compare(ret_value) > 0){
			
			
				end = mid;
				mid = (((start+end)/2)/recordSize)*recordSize;
			if( end==mid){
				//cout<<"breaking"<<endl;
				//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
				return;
			}

				search(start,end,mid,recordSize,colNames);
			
				
		}
		else{
			
				start = mid;
				mid = (((start+end)/2)/recordSize)*recordSize;
			if(start==mid ){
				//cout<<"breaking"<<endl;
				//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
				return;
			}

				search(start,end,mid,recordSize,colNames);
			
			
		}
		delete[] strColVal;
	}
	else{
		int intColVal;
		//cout<<"in int "<<binSrchFile.tellg()<<endl;
		binSrchFile.read(reinterpret_cast<char *>(&intColVal),4);
		//cout<<"intcolval  "<<intColVal<<"   "<<ret_value<<endl;
		if(intColVal==atoi(ret_value.c_str())){
			match=1;
			//cout<<intColVal<<endl;
			//cout<<"equal"<<endl;
			//upPos = checkDupUpperHalf(tblName,pos,recordSize);
			//downPos = checkDupLowerHalf(tblName,pos,recordSize);
			vector<int>::iterator it;
			it = find (visited.begin(), visited.end(), pos);
			if (it == visited.end()){
				visited.push_back(pos);
				//cout<<"\n\n"<<endl;
				
				for(int l=0;l<visited.size();l++){
					//cout<<visited[l]<<"\t";
				}
				//cout<<"\n\n"<<endl;
				if(asciiVal==61||asciiVal==63||asciiVal==64){
					//cout<<"position:"<<pos<<endl;
					binSrchFile.seekg(pos);
					for(int h=0;h<colNames.size();h++){
						//cout<<"valu:"<<sizeTypeMap[colNames[h]][0]<<endl;
						//cout<<"enter value:"<<sizeTypeMap[colNames[h]][1]<<endl;
						//cout<<"filepos:"<<binSrchFile.tellg()<<endl;
						if(sizeTypeMap[colNames[h]][0]==0){
							char *strColVal1=new char[sizeTypeMap[colNames[h]][1]+1]();
							//char *strColVal1=new char[100]();
							binSrchFile.read(strColVal1,sizeTypeMap[colNames[h]][1]);
							binSrchOutFile.write(strColVal1,sizeTypeMap[colNames[h]][1]);
							//cout<<string(strColVal1)<<"\t";
							delete[] strColVal1;
						}
						else{
							int intColVal1;
							binSrchFile.read(reinterpret_cast<char *>(&intColVal1),4);
							binSrchOutFile.write(reinterpret_cast<char *>(&intColVal1),4);
							//cout<<intColVal1<<"\t";
						}
					}
				}
				//cout<<"\n"<<endl;
			}
			//binSrchOutFile.write(reinterpret_cast<char *>(&intColVal),4);
			int end1 = end;
			int mid1 = mid;
			end = mid;
			
			mid = (((start+end)/2)/recordSize)*recordSize;
			if( end==mid){
				//cout<<"breaking"<<endl;
				//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
				return;
			}

			search(start,end,mid,recordSize,colNames);
			//cout<<"2nd equal"<<endl;
			start = mid1;
			end = end1;
			mid = (((start+end)/2)/recordSize)*recordSize;
			if(start==mid ){
				//cout<<"breaking"<<endl;
				//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
				return;
			}
			search(start,end,mid,recordSize,colNames);
			//cout<<"leaving equal"<<endl;
			//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
		}
		else if(intColVal > atoi(ret_value.c_str())){
			//cout<<"greater than"<<endl;
			end = mid;
			mid = (((start+end)/2)/recordSize)*recordSize;
			if( end==mid){
				//cout<<"breaking"<<endl;
				//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
				return;
			}
			search(start,end,mid,recordSize,colNames);
			
			//cout<<"leaving greater than"<<endl;
			//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
		}
		else{
							//cout<<"less than"<<endl;
			start = mid;
			mid = (((start+end)/2)/recordSize)*recordSize;
			if(start==mid ){
				//cout<<"breaking"<<endl;
				//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
				return;
			}

			search(start,end,mid,recordSize,colNames);
			//cout<<"leaving less than"<<endl;
			//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
		}
	}
	return;
}

void binarySearch(string tblName,vector<string> colNames){
	int recordSize=0;
	//cout<<"colNames:"<<endl;
	for(int c=0;c<colNames.size();c++){
			if(sizeTypeMap[colNames[c]][0]==0){
				charSizeMap[c]=sizeTypeMap[colNames[c]][1];
				
			}
			//cout<<colNames[c]<<endl;
			recordSize+=sizeTypeMap[colNames[c]][1];
		}
		//cout<<"\n";
	//cout<<"recordSize  "<<recordSize<<endl;
	//fstream binSrchFile;
	string table=tblName;
	cout<<"table :"<<table<<endl;
	size_t count_tbls;
	binSrchFile.open((table).c_str(),ios::binary|ios::in|ios::ate);
	binSrchOutFile.open((table.append("Where")).c_str(),ios::binary|ios::out|ios::trunc);
	if(!binSrchOutFile.good()){
		cout<<" binSrchOutFile file not found"<<endl;
		clear();
		return;
	}
	if(!binSrchFile){
		cout<<"binSrchFile file not found"<<endl;
		clear();
		return;
	}
	int size=binSrchFile.tellg();
	int pos=0;
	binSrchFile.seekg(pos);
	pos=binSrchFile.tellg();
	int start = 0, end = size, mid = (((start+end)/2)/recordSize)*recordSize;
	//cout<<" ######################   "<<start<<"    "<<end<<"    "<<mid <<endl;
	int upPos=-1,downPos=-1;
	search(start,end,mid,recordSize,colNames);
	/*while(true){
		binSrchFile.seekg(mid);
		binSrchFile.ignore(cumMap[ret_col]);
		pos = binSrchFile.tellg();
		if(sizeTypeMap[ret_col][0]==0){
			char *strColVal=new char[sizeTypeMap[ret_col][1]+1]();
			binSrchFile.read(strColVal,charSizeMap[it]);
			if(string(strColVal).compare(ret_value)==0){
				match=1;
				upPos = checkDupUpperHalf(tblName,pos,recordSize);
				downPos = checkDupLowerHalf(tblName,pos,recordSize);
				break;
			}
			else if(string(strColVal).compare(ret_value) > 0){
				end = mid;
				mid = (((start+end)/2)/recordSize)*recordSize;
				if(end==mid)
					break;
			}
			else{
				start = mid;
				mid = (((start+end)/2)/recordSize)*recordSize;
				if(start==mid)
					break;
			}
			delete[] strColVal;
		}
		else{
			int intColVal;
			binSrchFile.read(reinterpret_cast<char *>(&intColVal),4);
			if(intColVal==ret_value){
				match=1;
				upPos = checkDupUpperHalf(tblName,pos,recordSize);
				downPos = checkDupLowerHalf(tblName,pos,recordSize);
				break;
			}
			else if(intColVal > ret_value){
				end = mid;
				mid = (((start+end)/2)/recordSize)*recordSize;
				if(end==mid)
					break;
			}
			else{
				start = mid;
				mid = (((start+end)/2)/recordSize)*recordSize;
				if(start==mid)
					break;
			}
		}
	}
	fstream outFile;
	string tempTable = tablename;
	tempTable = tempTable.append("_where");
	outFile.open(tempTable.c_str(),ios::binary|ios::out|ios::trunc);
	if(match==1){
		if(upPos >= 0 && (downPos > 0 && downPos < size)){
			
			//loop and get all the values from upPos to downPos and write to outFile
			
		}
		else if(upPos < 0 && (downPos > 0 && downPos < size)){
			//loop and get all the values from pos to downPos and write to outFile
		}
		else if(upPos > 0 && downPos < 0 ){
			//loop and get all the values from upPos to pos and write to outFile
			
		}
		else{
			//read only that record at pos and write to outFile
		}
	}*/
	
	sort(visited.begin(),visited.end());
	//cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"<<endl;
	
	if(asciiVal==60||asciiVal==63){
		startPrint = 0;
		endPrint = visited[0];
	}
	else if(asciiVal == 62||asciiVal==64){
		startPrint = visited[visited.size()-1]+recordSize;
		endPrint = size;
	}
	//cout<<"startPrint :"<<startPrint<<"  endPrint:"<<endPrint<<endl;
	int iterations = (endPrint-startPrint)/recordSize;
	if(endPrint!=0&&endPrint!=(size-recordSize)){
		binSrchFile.seekg(startPrint);
		for(int iter = 0;iter<iterations;iter++){		
			
			for(int h=0;h<colNames.size();h++){
				//cout<<"sizeTypeMap[colNames[h]][0]: "<<sizeTypeMap[colNames[h]][0]<<"\t";
				if(sizeTypeMap[colNames[h]][0]==0){
					char *strColVal1=new char[sizeTypeMap[colNames[h]][1]+1]();
					//char *strColVal1=new char[100]();
					binSrchFile.read(strColVal1,sizeTypeMap[colNames[h]][1]);
					binSrchOutFile.write(strColVal1,sizeTypeMap[colNames[h]][1]);
					//cout<<string(strColVal1)<<"\t";
					delete[] strColVal1;
				}
				else{
					int intColVal1;
					binSrchFile.read(reinterpret_cast<char *>(&intColVal1),4);
					binSrchOutFile.write(reinterpret_cast<char *>(&intColVal1),4);
					//cout<<intColVal1<<"\t";
				}
			}
			startPrint = startPrint+recordSize;
			if(startPrint == size)
				break;
			//cout<<"\n"<<endl;
		}
	}
	binSrchFile.close();
	binSrchOutFile.close();
	//tempTable = "";
	tblName = "";
	table = "";
	//cout<<"end binSearch"<<endl;
	visited.clear();
	return;
}

void groupBy(string tableName,int recSize){
	
	string addSumAttr = "";
	orderByColName = orderByColName1;
	string sel_col_t = sel_col;
	if(sel_col.compare("")!=0)
		addSumAttr = addSumAttr.append(",sum(").append(sumAttr).append(")");
	else
		addSumAttr = addSumAttr.append("sum(").append(sumAttr).append(")");
	sel_col = sel_col.append(addSumAttr);
	addSumAttr="";
	addSumAttr = "sum(";
	addSumAttr = addSumAttr.append(sumAttr).append(")");
	//cout<<sel_col_t<<"    *"<<orderByColName<<"   *"<<groupByColName<<"   *"<<sel_col<<"   *"<<addSumAttr<<endl;
	sizeTypeMap[addSumAttr].push_back(1);
	sizeTypeMap[addSumAttr].push_back(4);
	if(groupByCase==1){
		//cout<<"sumAttrTable : "<<sumAttrTable<<"  next_table : "<<next_table<<endl;
		if(sumAttrTable.compare(next_table)==0){
			if(join_grpBy==1){
				cumMap[sumAttr] = sizeTypeMap[groupByColName][1];
				cumMap[groupByColName] = 0;
			}
		}
		else{
			if(join_grpBy==1){
				cumMap[sumAttr]=0;
				cumMap[groupByColName]=4;
			}
		}
	}
	//cout<<"!!!!!!!!!"<<addSumAttr<<sizeTypeMap["sum(my.id)"][0]<<endl;
	//cout<<" sizeTypeMap[sel_col_t][1] : "<<sizeTypeMap[sel_col_t][1]<<endl;
/*	if(sel_col_t.compare("")!=0){
		//cout<<"in compare"<<endl;
		if(join_grpBy==1)
			cumMap[sumAttr] = sizeTypeMap[sel_col_t][1];
		//cout<<"cumMap[addSumAttr] "<<cumMap[sumAttr]<<endl;
	}
	else
		if(join_grpBy==1)
			cumMap[sumAttr] = 0;
	*/
	//cout<<"!!!!!!!!!"<<sizeTypeMap["sum(id)"][0]<<endl;
	fstream groupByInput,groupByOutput;
	groupByInput.open(tableName.c_str(),ios::binary|ios::in|ios::ate);
	groupByOutput.open((tableName.append("GroupBy")).c_str(),ios::binary|ios::out|ios::trunc);
	int inFileSize = groupByInput.tellg();
	//cout<<"in sum "<<tableName<<"    "<<recSize<<"   "<<inFileSize<<endl;
	int pos,fileStart;
	pos = 0;
	int prev,sum ;string prevStr;
	if(groupByCase==0){
		if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
			cout<<"\n\nsum("<<sumAttr<<")\n--------------------------"<<endl;
	}
	else if(groupByCase==1){
		if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
			cout<<"\n\n"<<groupByColName<<"\tsum("<<sumAttr<<")\n--------------------------"<<endl;
	}
	//cout<<"cumMap[groupByColName] : "<<cumMap[groupByColName]<<endl;
	while(pos < inFileSize && (pos+recSize) <= inFileSize){
		//cout<<pos<<endl;
		groupByInput.seekg(pos);
		if(groupByCase==0){
			
			//cout<<"in groupByCase 0"<<endl;
			int intColVal;
			
			groupByInput.ignore(cumMap[groupByColName]);
			groupByInput.read(reinterpret_cast<char *>(&intColVal),4);
			if(pos==0){
				prev = intColVal;
				sum = intColVal;
				if((pos+recSize)==inFileSize){
					if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
						cout<<sum<<"\n";
					groupByOutput.write(reinterpret_cast<char*>(&sum),4);
				}
					
			}
			else{
				if(intColVal==prev){
					sum+=intColVal;
					if((pos+recSize)==inFileSize){
						if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
							cout<<sum<<"\n";
						groupByOutput.write(reinterpret_cast<char*>(&sum),4);
					}
				}
				else{
					if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
						cout<<sum<<"\n";
					groupByOutput.write(reinterpret_cast<char*>(&sum),4);
					sum=0;
					sum=intColVal;
					prev = intColVal;
					if((pos+recSize)==inFileSize){
						if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
							cout<<sum<<"\n";
						groupByOutput.write(reinterpret_cast<char*>(&sum),4);
					}
				}
			}
		}
		else if(groupByCase==1){
			//cout<<"in groupByCase 1"<<endl;
			
			int grpByColIndex=-1;
			string temp_sel_col = sel_col;
			int findComma = temp_sel_col.find(",");
			string temp = temp_sel_col.substr(0,findComma);
			if(temp.compare(groupByColName)==0)
				grpByColIndex = 0;
			else
				grpByColIndex = 1;
			int intColVal1,intColVal2;
			//cout<<" cumMap[sumAttr] : "<<cumMap[sumAttr]<<endl;
			groupByInput.ignore(cumMap[sumAttr]);
			groupByInput.read(reinterpret_cast<char *>(&intColVal1),4);
			//cout<<"intColVal1 = "<<intColVal1<<endl;
			groupByInput.seekg(pos);
			//cout<<"pos after read "<<groupByInput.tellg()<<"    "<<intColVal1<<endl;
			groupByInput.ignore(cumMap[groupByColName]);
			//cout<<"pos after ignore "<<groupByInput.tellg()<<endl;
			if(sizeTypeMap[groupByColName][0]==0){
				char *strColVal1=new char[sizeTypeMap[groupByColName][1]+1]();
				groupByInput.read(strColVal1,sizeTypeMap[groupByColName][1]);
				if(pos==0){
					prevStr = string(strColVal1);
					sum = intColVal1;
					if((pos+recSize)==inFileSize){
						if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
							cout<<prevStr<<"\t"<<sum<<endl;
						if(grpByColIndex==0){
							groupByOutput.write(prevStr.c_str(),sizeTypeMap[groupByColName][1]);
							groupByOutput.write(reinterpret_cast<char*>(&sum),4);
						}
						else{
							groupByOutput.write(reinterpret_cast<char*>(&sum),4);
							groupByOutput.write(prevStr.c_str(),sizeTypeMap[groupByColName][1]);
						}
					}
				}
				else {
					if(prevStr.compare(string(strColVal1))==0){
						sum+=intColVal1;
						if((pos+recSize)==inFileSize){
							if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
								cout<<prevStr<<"\t"<<sum<<endl;
							if(grpByColIndex==0){
								groupByOutput.write(prevStr.c_str(),sizeTypeMap[groupByColName][1]);
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
							}
							else{
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
								groupByOutput.write(prevStr.c_str(),sizeTypeMap[groupByColName][1]);
							}
						}
					}
					else{
						if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
							cout<<prevStr<<"\t"<<sum<<endl;
						if(grpByColIndex==0){
								groupByOutput.write(prevStr.c_str(),sizeTypeMap[groupByColName][1]);
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
							}
							else{
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
								groupByOutput.write(prevStr.c_str(),sizeTypeMap[groupByColName][1]);
							}
						sum=0;
						sum = intColVal1;
						prevStr = "";
						prevStr = string(strColVal1);
						if((pos+recSize)==inFileSize){
							if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
								cout<<prevStr<<"\t"<<sum<<endl;
							if(grpByColIndex==0){
								groupByOutput.write(prevStr.c_str(),sizeTypeMap[groupByColName][1]);
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
							}
							else{
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
								groupByOutput.write(prevStr.c_str(),sizeTypeMap[groupByColName][1]);
							}
						}
					}
				}
				delete[] strColVal1;
			}
			else{
				//cout<<"reading integer "<<endl;
				groupByInput.read(reinterpret_cast<char *>(&intColVal2),4);
				//cout<<"intColVal2 = "<<intColVal2<<endl;
				if(pos==0){
					prev = intColVal2;
					sum = intColVal1;
					if((pos+recSize)==inFileSize){
							if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
								cout<<prev<<"\t"<<sum<<endl;
							if(grpByColIndex==0){
								groupByOutput.write(reinterpret_cast<char*>(&prev),4);
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
							}
							else{
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
								groupByOutput.write(reinterpret_cast<char*>(&prev),4);
							}
						}
				}
				else {
					if(intColVal2==prev){
						sum+=intColVal1;
						if((pos+recSize)==inFileSize){
							if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
								cout<<prev<<"\t"<<sum<<endl;
							if(grpByColIndex==0){
								groupByOutput.write(reinterpret_cast<char*>(&prev),4);
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
							}
							else{
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
								groupByOutput.write(reinterpret_cast<char*>(&prev),4);
							}
						}
					}
					else{
						if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
							cout<<prev<<"\t"<<sum<<"\n";
						if(grpByColIndex==0){
								groupByOutput.write(reinterpret_cast<char*>(&prev),4);
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
							}
							else{
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
								groupByOutput.write(reinterpret_cast<char*>(&prev),4);
							}
						sum=0;
						sum=intColVal1;
						prev = intColVal2;
						if((pos+recSize)==inFileSize){
							if(order_by==0 /*|| (orderByColName.compare(groupByColName)==0)*/)
								cout<<prev<<"\t"<<sum<<endl;
							if(grpByColIndex==0){
								groupByOutput.write(reinterpret_cast<char*>(&prev),4);
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
							}
							else{
								
								groupByOutput.write(reinterpret_cast<char*>(&sum),4);
								groupByOutput.write(reinterpret_cast<char*>(&prev),4);
							}
						}
					}
				}
			}
		}
		//cout<<"changing pos"<<endl;
		pos = pos + recSize;
	}
	groupByOutput.close();
	addSumAttr = "";
}

void select(){
	vector<string> grpBySel;
      //if(where_con!=1){
              //cout<<"select attr list"<<sel_col<<endl;
              
        string selTbl = tablename;
		selTbl = selTbl.append("_select\.tbl");
		fstream selOut;
		selOut.open(selTbl.c_str(),ios::binary|ios::out|ios::trunc);
      //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
      int ignoreMain = 0,count=0;int recordSizeMain=-1;string strPrim="",primKeyCol="";
      if(insertSel==1){
          string  command = getCommand(tablename,6);
          strPrim += executeGrep(getWritable(command));
          int findEq = strPrim.find("=");
          strPrim = strPrim.substr(findEq+1,strPrim.length()-2-findEq);
          //cout<<"&&&&&&&&&&& strPrim : "<<strPrim<<endl;
          
          command = getCommand(tablename,7);
          string recSize = executeGrep(getWritable(command));
          recSize = recSize.substr(recSize.find("=")+1,recSize.length());
          //cout<<"rec size str : "<<recSize<<endl;
          int recordSize = atoi(recSize.c_str());
          //cout<<"&&&&&&&&&&& recordSize : "<<recordSize<<endl;
          
          command = getCommand(main_table,6);
          string strPrimMain = executeGrep(getWritable(command));
          findEq = strPrimMain.find("=");
          strPrimMain = strPrimMain.substr(findEq+1,strPrimMain.length()-2-findEq);
          //cout<<"&&&&&&&&&&& strPrimMain : "<<strPrimMain<<endl;
          
          
          command = getCommand(main_table,5);
          string str = executeGrep(getWritable(command));
          str = str.substr(str.find("=")+1,str.length()-str.find("=")-2);
          //cout<<"&&&&&&&&&&& str : "<<str<<endl;
          
          
          int brk=0;
          int findOB;
          while(str.find(",")!=string::npos){
                
             //   cout<<"&&&&&&&&&&& in while : "<<count<<endl;
              //  cout<<"&&&&&&&&&&& str at strt : "<<str<<endl;
                string column = str.substr(0,str.find(":"));
                //cout<<"&&&&&&&&&&& column : "<<column<<endl;
                if(column.compare(strPrimMain)==0){
					  //cout<<"compare:"<<brk<<endl;
                      brk=1;
                      break;
                }
              if((findOB = str.substr(0,str.find(",")).find("("))!=string::npos){
                      int sizeOfCol = atoi((str.substr(findOB+1, str.find(")")-findOB-1)).c_str());
                     
                      ignoreMain+=sizeOfCol;
                      
              }
              else{
              
                      ignoreMain+=4;
              }
              count++;
              str=str.substr(str.find(",")+1,str.length()-str.find(",")-1);
              //cout<<"&&&&&&&&&&& str at end : "<<str<<endl;
          }
		 
      //cout<<"&&&&&&&&&&& out while : "<<count<<endl;
          // cout<<"sizedhwwfh"<<ignoreMain<<endl;
	        /*if(brk!=1){
                 
             if((findOB = str.find("("))!=string::npos){
                      int sizeOfCol = atoi((str.substr(findOB+1, str.find(")")-findOB-1)).c_str());
                      
                      ignoreMain+=sizeOfCol;
              }
              else{
              
                      ignoreMain+=4;
              }
                 
          }*/
          
          
          command = getCommand(main_table,7);
          recSize = executeGrep(getWritable(command));
          recSize = recSize.substr(recSize.find("=")+1,recSize.length());
          //cout<<"rec size str : "<<recSize<<endl;
          recordSizeMain = atoi(recSize.c_str());
          //cout<<"&&&&&&&&&&& recordSizeMain : "<<recordSizeMain<<endl;
          
          int count_t = count;string sel_col_temp = sel_col;
		  //cout<<"sel_col_temp:"<<sel_col_temp<<endl;
          while(count_t!=0){
            //      cout<<"&&&&&&&&&&& in while : "<<count_t<<endl;
             //     cout<<"******"<<sel_col_temp<<endl;
                  sel_col_temp=sel_col_temp.substr(sel_col_temp.find(",")+1,sel_col_temp.length()-sel_col_temp.find(","));
                 // cout<<"&*&*&*"<<sel_col_temp<<endl;
                  count_t--;
                  
          }
          //cout<<"&&&&&&&&&&& out while : "<<count_t<<endl;
          int temp1;
          if((temp1=sel_col_temp.find(","))==string::npos){
          
                  primKeyCol += sel_col_temp;
				  //cout<<"primary key column:"<<primKeyCol<<endl;
          }
          else{
          
                  primKeyCol += sel_col_temp.substr(0,temp1);
				  //cout<<"primry key else:"<<primKeyCol<<endl;
          }
          
          //cout<<"primKeyCol : "<<primKeyCol<<"&"<<endl;
      } 

     //NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNn
     
     
              int star=0;			  
			if(star_present == 1 ){
				if( group_by==1 ){
					cout<<"Invalid query for Group by"<<endl;
						clear();
					return;
				}
				else{
					star=1;
				}
			}
			star_present=0;
			  
			  
			  
			  
			  
			  
			  
			  
			  
			  
              //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^start
              string temp_tbl = tablename;
              ifstream f((temp_tbl.append("\.tbl")).c_str());
              
              if(!f.good()){
                  cout<<"table doesn't exist";
				  clear();
                  return;
              }
              //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^end
                    string command = getCommand(tablename,5);
                    string str = executeGrep(getWritable(command));
                   // cout<<"string from catalog is "<<str<<endl;
                    
              //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^start
              string sel_col_t = sel_col,str_t = str;
              string colNamePrint = "";
			  vector<string> colNamePrintArr;
              if(star!=1){
                  int fin;
                  while((fin = sel_col_t.find(","))!=string::npos){
                      
                      if(str_t.find(sel_col_t.substr(0,fin))==string::npos){
                          cout<<(sel_col_t.substr(0,fin))<<" column not found"<<endl;
						  clear();
                          return;
                      }
					  colNamePrintArr.push_back(sel_col_t.substr(0,fin));
                      colNamePrint = colNamePrint.append(sel_col_t.substr(0,fin)).append("\t");
                     sel_col_t = sel_col_t.substr(fin+1, sel_col_t.length());
                 }
                 if(str_t.find(sel_col_t)==string::npos){
                     cout<<sel_col_t<<" column not found"<<endl;
					 clear();
                     return;
                 }
				 colNamePrintArr.push_back(sel_col_t);
                 colNamePrint = colNamePrint.append(sel_col_t).append("\t");
             }
              //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^end
              vector<string> allColumns;
                    size_t splitColsLine = str.find("=");
                    string colsDef = str.substr(splitColsLine+1, (str.length()-splitColsLine-1));
                    string temp_colsDef = colsDef;
                    int val=0,lenCols;
                    while((lenCols = colsDef.find(","))>0 && lenCols<colsDef.length()){
                        int findType = colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1).find("(");
                        string colName = colsDef.substr(0,colsDef.find(":"));
                        if(star==1){
							colNamePrintArr.push_back(colName);
                            colNamePrint = colNamePrint.append(colName).append("\t");
							if(insertSel!=1){
                            sel_col+=colName;
                            sel_col+=",";
							}
                        }
                        if(findType>=0){
                            
                            string s_colsDef_length=colsDef.substr(colsDef.find("(")+1,colsDef.find(")")-colsDef.find("(")-1);
                            int size_of_column=atoi(s_colsDef_length.c_str());
                            cumMap[colName] = val;
                            sizeTypeMap[colName].push_back(0);
                            sizeTypeMap[colName].push_back( size_of_column) ;
							allColumns.push_back(colName);
                            val = val+size_of_column;              
                        }
                        else{
                            
                            cumMap[colName] = val;
                            sizeTypeMap[colName].push_back(1);
                            sizeTypeMap[colName].push_back(4) ;
							allColumns.push_back(colName);
                            val = val+4; 
                            
                        }
                        colsDef = colsDef.substr(lenCols+1,colsDef.length());
                        //cout<<"colsDef is "<<colsDef<<endl;
                        
                    }
                    
                    int findType = colsDef.substr(colsDef.find(":")+1, lenCols-colsDef.find(":")-1).find("(");
                        string colName = colsDef.substr(0,colsDef.find(":"));
                        if(star==1){
							colNamePrintArr.push_back(colName);
                            colNamePrint = colNamePrint.append(colName).append("\t");
								if(insertSel!=1){
                                  sel_col+=colName;
								}
                        }
                        if(findType>=0){
                            
                            string s_colsDef_length=colsDef.substr(colsDef.find("(")+1,colsDef.find(")")-colsDef.find("(")-1);
                            int size_of_column=atoi(s_colsDef_length.c_str());
                            cumMap[colName] = val;
                            sizeTypeMap[colName].push_back(0);
                            sizeTypeMap[colName].push_back( size_of_column) ;
							allColumns.push_back(colName);
                            val = val+size_of_column;              
                        }
                        else{
                            
                            cumMap[colName] = val;
                            sizeTypeMap[colName].push_back(1);
                            sizeTypeMap[colName].push_back(4) ;
							allColumns.push_back(colName);
                            val = val+4; 
                            
                        }
                    
                    //cout<<"select attr list"<<sel_col<<endl;
                    //cout<<"\n\n"<<colNamePrint<<"\n"<<"------------------------------------------------------------"<<endl;
					
                    for(map<string,int>::iterator it = cumMap.begin(); it != cumMap.end(); ++it) {
                        //cout << it->first <<" ->  "<<it->second<<"\n"; 
                    }
					
					
                    string table;
					string result="";
					fstream binFile;
					int size,pos;
					//cout<<"select attr list"<<sel_col<<endl;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//cout<<"orderByColName cpy*"<<orderByColName<<endl;
		orderByColName1=orderByColName;
		
		int seekPos=0;
		if(where_con==1){
			from_where=1;
			table=tablename;
			orderByColName = ret_col;
			//cout<<"\n\n All Columns "<<endl;
			for(int g=0;g<allColumns.size();g++){
				//cout<<allColumns[g]<<endl;
			}	
			order(table,allColumns);
			string tempOrdered = tablename;
			tempOrdered = tempOrdered.append("ordered");
			binarySearch(tempOrdered,allColumns);
			from_where=0;
			table = tempOrdered.append("Where");
			if(groupByColName!=""){
				orderByColName = groupByColName;
				order(table,allColumns);
				table = tablename;
				table = table.append("ordered");
				orderByColName="";
				orderByColName = orderByColName1;
				if(sizeTypeMap[sumAttr][0]==0){
					cout<<"sum column cannot be a character type"<<endl;
					clear();
					return ;
				}
				//cout<<" where"<<endl;
				if(sumAttr.compare(groupByColName)==0){
					//cout<<"same"<<endl;
					if(sel_col!=""){
						cout<<" Invalid query for group  by"<<endl;
						clear();
						return;
					}
					else{
						groupByCase = 0;
						//cout<<"groupByCase : "<<groupByCase<<endl;
						
						groupBy(table,val);
						selTbl = "";
						selTbl.append(table).append("GroupBy");
						grpBySel.push_back(sel_col);
					}
				}
				else{
					if(sel_col.find(",")!=string::npos){
						cout<<" Invalid query for group  by"<<endl;
						clear();
						return;
					}
					else{
						groupByCase = 1;
						//cout<<"groupByCase : "<<groupByCase<<endl;
					groupBy(table,val);
					selTbl = "";
					selTbl.append(table).append("GroupBy");
					
					//cout<<"sel_col:"<<sel_col;
					int col = sel_col.find(",");
					string frstCol = sel_col.substr(0,col);
					
					grpBySel.push_back(frstCol);
					grpBySel.push_back(sel_col.substr(col+1,sel_col.length()-col-1));
					}
				}
			}
			//groupByColName="";
			//sumAttr="";
			binFile.open((table).c_str(),ios::binary|ios::in|ios::ate);
			if(!binFile){
				cout<<"file not found"<<endl;
			}
			size=binFile.tellg();
			pos=0;
			from_where=0;
			tempOrdered="";
			orderByColName="";
			orderByColName=orderByColName1;
			//cout<<"orderByColName "<<orderByColName<<endl;
			//cout<<"select attr list"<<sel_col<<endl;
		}
		else{
			table = tablename;
			from_groupBy = 1;
			if(groupByColName!=""){
				//cout<<"no where"<<endl;
				//cout<<"orderByColName cpy*"<<orderByColName<<endl;
				//orderByColName1 = orderByColName;
				orderByColName = groupByColName;
				order(table,allColumns);
				table = table.append("ordered");
				orderByColName="";
				orderByColName = orderByColName1;
				//cout<<"orderByColName "<<orderByColName<<endl;
				if(sizeTypeMap[sumAttr][0]==0){
					cout<<"sum column cannot be a character type"<<endl;
					clear();
					return ;
				}
				//cout<<sumAttr<<"   *"<<groupByColName<<"   *"<<sel_col<<endl;
				if(sumAttr.compare(groupByColName)==0){
					//cout<<"same"<<endl;
					if(sel_col!=""){
						cout<<" Invalid query for group  by"<<endl;
						clear();
						return;
					}
					else{
						groupByCase = 0;
						//cout<<"groupByCase : "<<groupByCase<<endl;
						groupBy(table,val);
						selTbl = "";
						selTbl.append(table).append("GroupBy");
						grpBySel.push_back(sel_col);
					}
				}
				else{
					if(sel_col.find(",")!=string::npos){
						cout<<" Invalid query for group  by"<<endl;
						clear();
						return;
					}
					else{
						groupByCase = 1;
						//cout<<"groupByCase : "<<groupByCase<<endl;
						groupBy(table,val);
						selTbl = "";
						selTbl.append(table).append("GroupBy");
						int col = sel_col.find(",");
						string frstCol = sel_col.substr(0,col);
						
						grpBySel.push_back(frstCol);
						grpBySel.push_back(sel_col.substr(col+1,sel_col.length()-col-1));
						//cout<<grpBySel[0]<<"  "<<grpBySel[1]<<endl;
					}
				}
			}
			else{
				table = table.append("\.tbl");
				seekPos=8;
			}
			//groupByColName="";
			orderByColName="";
			orderByColName=orderByColName1;
			//cout<<"orderByColName "<<orderByColName<<endl;
			//sumAttr="";
			//cout<<"main "<<table<<endl;
			binFile.open((table).c_str(),ios::binary|ios::in|ios::ate);
			if(!binFile){
				cout<<"file not found"<<endl;
			}
			size=binFile.tellg();
			pos=0;
			binFile.seekg(pos);
			//cout<<"size:"<<size<<endl;
			binFile.seekg(seekPos,ios::beg);
			//cout<<"pos:"<<pos<<endl;
			pos=binFile.tellg();
			from_groupBy = 0;
		}

		//cout<<"back in select"<<endl;
		//cout<<"select attr list"<<sel_col<<endl;

//cout<<grpBySel[0]<<"  "<<grpBySel[1]<<endl;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
			
	if(group_by!=1){			
				//cout<<"pos:"<<pos<<endl;
				string temp_sel_col = sel_col;
                     

				//cout<<"\n\n";
					for(int i=0;i<colNamePrintArr.size();i++){
						//cout<<colNamePrintArr[i]<<"\t";
					}
					//cout<<"\n------------------------------------------------------------"<<endl;
					//cout<<"\n";

				//cout<<"@@@@@@@@@@@@" <<val<<endl;
                     while(pos<size && (pos+val)<=size){
                         //cout<<"outer while"<<endl;
                         result="";
                         sel_col = temp_sel_col;
                         
                         //cout<<"@@@@@@@@@@@@" <<val<<endl;
                         //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
                         /*if(where_con==1){
                             bool doSelect=where(pos);
                             if(!doSelect){
                                 pos=pos+val;
                                 continue;
                             }
                          }*/
                         
                         //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                         //cout<<"pos:"<<pos<<endl;
                         fstream insSelbin;
                          int pos1 = pos;
                         if(main_table.compare("")!=0){
							 //cout<<"@@@@@@@@@main_tablecompae"<<endl;
                             string temp_main_table =  main_table;
                             temp_main_table = temp_main_table.append("\.tbl");
                             insSelbin.open(temp_main_table.c_str(),ios::binary|ios::out|ios::app);
                             
                             if(!insSelbin.is_open())
								 //cout<<"Main file not opened:"<<endl;
                         //cout<<"%%%%%%%%%%%"<<primKeyCol<<endl;
                         
                        //cout<<"******&"<<primKeyCol<<"     "<<sizeTypeMap[primKeyCol][0]<<endl;
                         if(sizeTypeMap[primKeyCol][0]==0){
                          //cout<<"@@@@@@@@@"<<endl;
                               //char strColVal[100];
                                char *strColVal=new char[100]();
								//cout<<"!!!!!!!!!!!" <<pos<<endl;
								binFile.seekg(pos);
                               //cout<<"ignoring "<<cumMap.find(primKeyCol)->second<<" bytes at "<<(binFile.tellg())<<"\n";
                              binFile.ignore(cumMap.find(primKeyCol)->second);
                                binFile.read(strColVal,sizeTypeMap[primKeyCol][1]);
                                //cout<<"!!!!!!!!!!  :"<<(string(strColVal))<<endl;
                               if( !checkPrimaryKey("char","",string(strColVal),ignoreMain,sizeTypeMap[primKeyCol][1],recordSizeMain) ){
                                       
                                       cout<<"duplicate value"<<endl;
                                       insert_failed=1;
                                       return;
                               
                               }
							    //cout<<"@@@@@@@@@"<<endl;
                               
                               delete[] strColVal;
                         
                         }
                         else{
                         
                                   int intColVal;
								   //cout<<"$$$$$$"<<pos<<endl;
								   binFile.seekg(pos);
								   
								  // cout<<"ignoring "<<cumMap.find(primKeyCol)->second<<" bytes at "<<(binFile.tellg())<<"\n";
								   
                              binFile.ignore(cumMap.find(primKeyCol)->second);
                                   binFile.read(reinterpret_cast<char *>(&intColVal),sizeTypeMap[primKeyCol][1]);
                                   //cout<<"!!!!!!!!!! :"<<intColVal<<endl;
                                
                                   ostringstream oss;
                                   oss<<intColVal;
                                   if( !checkPrimaryKey("int","",oss.str(),ignoreMain,sizeTypeMap[primKeyCol][1],recordSizeMain) ){
                                       
                                       cout<<"duplicate value"<<endl;
                                       insert_failed=1;
                                       return;
                               
                               }
                                //cout<<"@@@@@@@@@"<<endl;
                         }
                         
                     }
                          //cout<<"@@@@@@@@@"<<endl;
                         pos=pos1;
						 //cout<<"pos out inner while "<<pos<<endl;
                         binFile.seekg(pos,ios::beg);
                         //cout<<"sel_col outer while "<<sel_col<<endl;
                          while((lenCols = sel_col.find(","))>0 && lenCols<sel_col.length()){
                           //cout<<"inner while"<<endl;
                              binFile.seekg(pos);
                              //cout<<"inner loop tellg:"<<binFile.tellg()<<endl;
                              string col = sel_col.substr(0,lenCols);
                              //cout<<"@@@@@@@@@@@@" <<val<<endl;
                              //cout<<"col:"<<col<<endl;
                              string table=tablename;
                              char *strColVal=new char[100]();
                              int intColVal;
                              //char strColVal[sizeTypeMap[col][1]];
                              // cout<<"ignoring "<<cumMap.find(col)->second<<" bytes \n";
                              binFile.ignore(cumMap.find(col)->second);
                              //cout<<"pos at print:"<<binFile.tellg()<<endl;
                             // cout<<"sizeTypeMap[col][0] :"<<sizeTypeMap[col][0]<<endl;
                             // cout<<"sizeTypeMap[col][1] : "<<sizeTypeMap[col][1]<<endl;
                             
                              if(sizeTypeMap[col][0]==0){
                                  //cout<<"printing char select"<<endl;
                                  //cout<<"sizeTypeMap[col][1] : "<<sizeTypeMap[col][1];
                                  binFile.read(strColVal,sizeTypeMap[col][1]);
                                  //cout<<"read string is "<<string(strColVal)<<endl;
                                  if(insertSel==1){
                                        
                                        insSelbin.write(strColVal,sizeTypeMap[col][1]);
                                  
                                  }
								  selOut.write(strColVal,sizeTypeMap[col][1]);
                                  result+=string(strColVal);
                                  result+="\t";
								  delete[] strColVal;
                              }
                              else{
                                  //cout<<"printing int select"<<endl;
                                  //cout<<"sizeTypeMap[col][1] : "<<sizeTypeMap[col][1];
                                  binFile.read(reinterpret_cast<char *>(&intColVal),sizeTypeMap[col][1]);
                                  
                                  if(insertSel==1){
                                      insSelbin.write(reinterpret_cast<char *>(&intColVal),sizeTypeMap[col][1]);
                                  }
                                  selOut.write(reinterpret_cast<char *>(&intColVal),sizeTypeMap[col][1]);
                                  ostringstream os;
                                  os<<intColVal;
                                  result+=os.str();
                                  result+="\t";
                              }
                             sel_col=sel_col.substr(lenCols+1,sel_col.length()-lenCols-1);
                          }
                         // cout<<"##################################################"<<endl;
                          binFile.seekg(pos);
                              string col = sel_col.substr(0,lenCols-1);
                              string table=tablename;
                              char *strColVal=new char[100]();
                              int intColVal;
                              //char strColVal[sizeTypeMap[col][1]];
							  //cout<<"pos out while"<<(binFile.tellg())<<endl;
                              //cout<<"ignoring "<<cumMap.find(col)->second<<" bytes \n";
                              binFile.ignore(cumMap.find(col)->second);
                              //cout<<"pos at print:"<<binFile.tellg()<<endl;
                              if(sizeTypeMap[col][0]==0){
                                  //cout<<"printing char select"<<endl;
                                  //cout<<"sizeTypeMap[col][1] : "<<sizeTypeMap[col][1];
                                  binFile.read(strColVal,sizeTypeMap[col][1]);
                                  
                                  if(insertSel==1){
                                      insSelbin.write(strColVal,sizeTypeMap[col][1]);
                                  }
                                  selOut.write(strColVal,sizeTypeMap[col][1]);
                                  result+=string(strColVal);
                                  result+="\t";
                              }
                              else{
                                  //cout<<"printing int select"<<endl;
                                  //cout<<"sizeTypeMap[col][1] : "<<sizeTypeMap[col][1];
                                  binFile.read(reinterpret_cast<char *>(&intColVal),sizeTypeMap[col][1]);
                                  if(insertSel==1){
                                      insSelbin.write(reinterpret_cast<char *>(&intColVal),sizeTypeMap[col][1]);
                                  }
                                  selOut.write(reinterpret_cast<char *>(&intColVal),sizeTypeMap[col][1]);
                                  ostringstream os;
                                  os<<intColVal;
                                  result+=os.str();
                                  result+="\t";
                              }
							  delete[] strColVal;
                              if(insertSel==1){
                                  //cout<<"update"<<endl;
                                  updateCatalog(main_table);
                              }
							  if(order_by==0)
								cout<<"\n\n"<<result<<"\n\n"<<endl;
                          pos=pos+val;
                     // cout<<"pos:"<<pos<<endl;
                     }
					 selOut.close();
					 sel_col = temp_sel_col;
	}
	//cout<<"order by sec begin"<<endl;
	//cout<<grpBySel[0]<<"  "<<grpBySel[1]<<endl;
					 if(order_by ==1 /*&& (orderByColName.compare(groupByColName)!=0)*/){
						 //cout<<"@@@@@@@@@@@@@@@@"<<sel_col<<endl;
						 //cout<<(sel_col.find(orderByColName)==string::npos)<<endl;
						 if(sel_col.find(orderByColName)==string::npos){
							 cout<<"Invalid input for order by"<<endl;
							 clear();
							 return;
						 }
						 if(group_by==1){
							string tempSum = "sum(";
							tempSum = tempSum.append(sumAttr).append(")");
							 //cout<<"tempSum  *"<<tempSum<<"    *"<<orderByColName<<endl;
							 //cout<<"orderByColName.compare(tempSum)==0"<<(orderByColName.compare(tempSum)==0)<<endl;
							// if(orderByColName.compare(tempSum)==0){
								 //orderByColName="";
								//orderByColName = tempSum;
									from_order_by=1;
									//cout<<"selTbl "<<selTbl<<endl;
									//cout<<grpBySel[0]<<endl;
									for(int u=0;u<grpBySel.size();u++)
									{
										//cout<<grpBySel[u]<<endl;
									}
									//cout<<"\n";
									//if(sizeTypeMap[grpBySel[1]][0]==1)
									//	cout<<"same "<<sizeTypeMap["sum(id)"][1]<<endl;
									order(selTbl,grpBySel);
									from_order_by=0;
									grpBySel.clear();
							// }
						 }
						 else{
							 from_order_by=1;
							 //cout<<"selTbl "<<selTbl<<endl;
							 order(selTbl,colNamePrintArr);	
							 from_order_by=0;
						 }
						 //cout<<" back from order"<<endl;
					 }
					 
					 
	clear();
}
void show_table(){
 fstream file;
    string temp_tbl = tablename;
    ifstream f((temp_tbl.append("\.tbl")).c_str());
    
    if(!f.good()){
        cout<<"table doesn't exist";
		clear();
        return;
    }
     string tblName = tablename;
    tblName=tblName.append("\.tbl");
    //cout<<"Binary Table NAme:"<<tblName<<endl;
    file.open(tblName.c_str(),ios::in|ios::binary);
    size_t table_count;
    file.read(( char *)&table_count, sizeof(table_count));
    //cout<<"count of table:"<<table_count<<endl;
    table_count = table_count-1;
    int line_number=table_count*6+4;
    ifstream file_catalog;
    file_catalog.open("catalog.txt");
    int line_count=1;
    string line;
    for( line; getline(file_catalog, line ); ) {
        line_count = line_count + 1;
        if (line_count > line_number && line_count <=line_number + 6) {
            cout << line << endl;
        } else if (line_count > line_number + 6) {
            break;
        }
    }


}
void showTables(){

	
    ifstream file_catalog;
    file_catalog.open("catalog.txt");
    //int line_count=1;
    string line;
    for( line; getline(file_catalog, line ); ) {
        //line_count = line_count + 1;
        //cout << line << endl;
    }


}
string returnCommandJoin(string tablename){
    
    fstream file1;
    string temp1=tablename;
     string temp="\.tbl";
     string name=temp1.append(temp);
    file1.open(name.c_str(),ios::in|ios::binary);
      size_t count_tbls1;
      file1.read(( char *)&count_tbls1, sizeof(count_tbls1));
      //cout<<"count is "<<count_tbls1<<endl;
      count_tbls1 = count_tbls1-1;
      size_t column_lines = count_tbls1*6+5;
      //cout<<"lineOfColums is "<<column_lines<<endl;
      ostringstream oss;
      oss<<column_lines;
      string grep_head = "head -";
      string grep_tail = " catalog.txt|tail -1| grep =";
      string command = grep_head.append(oss.str()).append(grep_tail);
    file1.close();
return command;
}



void join_tables(){
	//cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	vector<string> grpBySel;
	int star=0;
	if(star_present == 1 ){
		if( group_by==1 ){
			cout<<"Invalid query for Group by"<<endl;
			clear();
			return;
		}
		else{
			star=1;
		}
	}
	else{
		if(group_by==1 && !(sel_col.compare("")==0)){
			sel_col = sel_col.append(",").append(sumAttr);
			//cout<<"sel_col append "<<sel_col<<endl;
		}
	}
	star_present=0;
	//cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	
	vector<string> finalOrderByColNames;
	int colCount=0;
	string joinTbl = next_table;
	joinTbl = joinTbl.append("_").append(init_table).append("_Join\.tbl");
	fstream joinOut;
	joinOut.open(joinTbl.c_str(),ios::binary|ios::out|ios::trunc);
	
	if (init_table!= table_name1){
		string temp;
		temp= init_ret_col;
		init_ret_col=next_ret_col;
		next_ret_col=temp;
	}
   
   string temp_tbl = init_table;
    ifstream f((temp_tbl.append("\.tbl")).c_str());
    
    if(!f.good()){
        cout<<"table doesn't exist";
		clear();
        return;
    }
    temp_tbl = "";
    temp_tbl = next_table;
    ifstream f1((temp_tbl.append("\.tbl")).c_str());
    
    if(!f1.good()){
        cout<<"table doesn't exist";
		clear();
        return;
    }
	string command = returnCommandJoin(init_table);
    string str = executeGrep(getWritable(command));
    string str_t = str,init_ret_col_t = init_ret_col;
    if(str_t.find(init_ret_col_t)==string::npos){
         cout<<"column not found"<<endl;
		 clear();
         return;
     }
    vector<string> orderByColNames;
    ////cout<<"join:"<<str<<endl;
	size_t splitColsLine = str.find("=");
    string temp_cols = str.substr(splitColsLine+1, (str.length()-splitColsLine-1));
	int size_table1= getRecordSize(temp_cols);
	vector<int> typevector_table1;
	vector<int> sizevector_table1;
	int lenCols,column_count=0;
	int column_number_k1,ignore_value_k1,cum_ignore_value_k1=0;
	int cumVal = 0;vector<string> where_table;
while((lenCols = temp_cols.find(","))>0 && lenCols<temp_cols.length()){
		 //cout<<"inside table1 while"<<endl;
		string findType = temp_cols.substr(temp_cols.find(":")+1, lenCols-temp_cols.find(":")-1);
		string colName = temp_cols.substr(0,temp_cols.find(":"));
		string tempTbl = init_table;
		orderByColNames.push_back(tempTbl.append("\.").append(colName));
		if(star==1){
			if(sel_col.compare("")==0)
				sel_col=sel_col+orderByColNames[orderByColNames.size()-1];
			else
				sel_col=sel_col+","+orderByColNames[orderByColNames.size()-1];
		}
		//cout<<" tempTbl : "<<tempTbl<<endl;
		tempTbl="";
		column_count++;
		if(colName.compare(init_ret_col)==0){
			column_number_k1=column_count;
			////cout<<"datatype:"<<findType;
			//typevector_table1.push_back(findType);
			int findval=findType.find("(");
			if(findval>0){
        ////cout<<"in char"<<endl;
				typevector_table1.push_back(0);
				string size=findType.substr(findType.find("(")+1,findType.find(")")-findType.find("(")-1);
				sizevector_table1.push_back(atoi(size.c_str()));
				ignore_value_k1=atoi(size.c_str());
				if(init_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				////cout<<"char if modify start"<<endl;
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(0);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(atoi(size.c_str()));
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += atoi(size.c_str());
				////cout<<"char if modify end"<<sizeTypeMap[orderByColNames[orderByColNames.size()-1]][0]<<"   "<<sizeTypeMap[orderByColNames[orderByColNames.size()-1]][1]<<endl;
			}
			else{
        ////cout<<"in int"<<endl;
				typevector_table1.push_back(1);
				sizevector_table1.push_back(4);
				////cout<<"int if modify start"<<endl;
				////cout<<"orderByColNames[orderByColNames.size()-1"<<(orderByColNames[orderByColNames.size()-1])<<endl;
				if(init_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(1);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(4);
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += 4;
				////cout<<"int if modify end"<<sizeTypeMap[orderByColNames[orderByColNames.size()-1]][0]<<"   "<<sizeTypeMap[orderByColNames[orderByColNames.size()-1]][1]<<endl;
        ignore_value_k1=4;
			}
			
		
		}
		else{
			int findval=findType.find("(");
			if(findval>0){
        ////cout<<"in char"<<endl;
				typevector_table1.push_back(0);
				string size=findType.substr(findType.find("(")+1,findType.find(")")-findType.find("(")-1);
				sizevector_table1.push_back(atoi(size.c_str()));
				if(init_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(0);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(atoi(size.c_str()));
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += atoi(size.c_str());
			}
			else{
      ////cout<<"in int"<<endl;
				typevector_table1.push_back(1);
				sizevector_table1.push_back(4);
				if(init_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(1);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(4);
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += 4;
			}
			
		}
   
			
		temp_cols = temp_cols.substr(lenCols+1,temp_cols.length()-lenCols-1);	
		}
		//cout<<"outside table1 while"<<endl;
   	string findType = temp_cols.substr(temp_cols.find(":")+1, lenCols-temp_cols.find(":")-1);
		string colName = temp_cols.substr(0,temp_cols.find(":"));
		string tempTbl = "";
		tempTbl = init_table;
		orderByColNames.push_back(tempTbl.append("\.").append(colName));
		if(star==1){
			if(sel_col.compare("")==0)
				sel_col=sel_col+orderByColNames[orderByColNames.size()-1];
			else
				sel_col=sel_col+","+orderByColNames[orderByColNames.size()-1];
		}
		tempTbl="";
		column_count++;
		if(colName.compare(init_ret_col)==0){
			column_number_k1=column_count;
			////cout<<"datatype:"<<findType;
			//typevector_table1.push_back(findType);
      int findval=findType.find("(");
			if(findval>0){
        ////cout<<"in char"<<endl;
				typevector_table1.push_back(0);
				string size=findType.substr(findType.find("(")+1,findType.find(")")-findType.find("(")-1);
				sizevector_table1.push_back(atoi(size.c_str()));
				ignore_value_k1=atoi(size.c_str());
				if(init_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(0);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(atoi(size.c_str()));
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += atoi(size.c_str());
			}
			else{
      ////cout<<"in int"<<endl;
				typevector_table1.push_back(1);
				sizevector_table1.push_back(4);
				if(init_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(1);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(4);
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += 4;
        ignore_value_k1=4;
			}
			
		
		}
		else{
     
			int findval=findType.find("(");
			if(findval>0){
      ////cout<<"in char"<<endl;
				typevector_table1.push_back(0);
				string size=findType.substr(findType.find("(")+1,findType.find(")")-findType.find("(")-1);
				sizevector_table1.push_back(atoi(size.c_str()));
				if(init_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(0);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(atoi(size.c_str()));
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += atoi(size.c_str());
				
			}
			else{
      ////cout<<"in int"<<endl;
				typevector_table1.push_back(1);
				sizevector_table1.push_back(4);
				if(init_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(1);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(4);
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += 4;
			}
			
		}
		int i=1;
		while(i<column_number_k1){
			cum_ignore_value_k1=cum_ignore_value_k1+sizevector_table1[i-1];
			i++;
		}
	vector<string> table1_k1;	
	fstream file_table1;
	string tempt1=init_table;
	//file_table1.open(tempt1.append("\.tbl").c_str(),ios::in|ios::binary);
	
	
	
	
	//cout<<"___________________________________________init"<<init_table<<"______________"<<next_table<<endl;
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	int pos1,tableNumber=-1;string fileName="";
	if(where_table.size()>0){
		 orderByColName1 = orderByColName;
		 string tApp = join_where_tbl;
		 string whereTableOrder = "";
		 orderByColName = tApp.append("\.").append(join_where_col);
		 from_where=1;
		 order(join_where_tbl,where_table);
		 whereTableOrder = whereTableOrder.append("ordered");
		 binarySearch(whereTableOrder,where_table);
		 whereTableOrder = whereTableOrder.append("Where");
		 from_where=0;		 
		 orderByColName = orderByColName1;
		 fileName = whereTableOrder;
		 tApp="";
		 file_table1.open((whereTableOrder).c_str(),ios::binary|ios::in|ios::ate);
		 pos1=0;
		 tableNumber=1;
		 whereTableOrder="";
	}
	else{
		file_table1.open((tempt1.append("\.tbl")).c_str(),ios::binary|ios::in|ios::ate);
		pos1=8;
	}
	where_table.clear();
	
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	
	
	 //file_table1.open((tempt1.append("\.tbl")).c_str(),ios::binary|ios::in|ios::ate);
	if(!file_table1){
		cout<<"file not found"<<endl;
	}
	int size=file_table1.tellg();
	file_table1.seekg(pos1,ios::beg);
	int pos_t1=file_table1.tellg();
	while(pos_t1<size){
   ////cout<<"pos_t1:"<<pos_t1<<endl;
		if(typevector_table1[column_number_k1-1]==0){
			char *strcolval=new char[100]();
			//char strcolval[sizevector_table1[column_number_k1-1]];
			file_table1.seekg(cum_ignore_value_k1+pos_t1);
			file_table1.read(strcolval,sizevector_table1[column_number_k1-1]);
			table1_k1.push_back(string(strcolval));
			delete[] strcolval;
		}
		else{
     ////cout<<"int:"<<endl;
			int intcolval;	
      ////cout<<"cum_ignore_value_k1:"<<cum_ignore_value_k1<<endl;
			file_table1.seekg(cum_ignore_value_k1+pos_t1);
      ////cout<<"fp position:"<<file_table1.tellg()<<endl;
     // //cout<<"sizevector_table1[column_count] "<<sizevector_table1[column_number_k1-1]<<endl;
			file_table1.read(reinterpret_cast<char *>(&intcolval),sizevector_table1[column_number_k1-1]);
    //  //cout<<"fp position:"<<file_table1.tellg()<<endl;
      //file_table1.seekg(pos_t1);
      ostringstream ss;
      ss<<intcolval;
     // //cout<<ss.str()<<endl;
			table1_k1.push_back(ss.str());
		}
	pos_t1=pos_t1+size_table1;
		
		
		
	}
	for(size_t i=0;i<table1_k1.size();i++){
		//cout<<"values:  "<<table1_k1[i]<<"\t";	
	}
	file_table1.close();
	//cout<<"file_table1 closed"<<endl;
 
 
 
 
 
 	cumVal=0;
	string command1 = returnCommandJoin(next_table);
    string str1 = executeGrep(getWritable(command1));
    
    string str_t1 = str1,next_ret_col_t = next_ret_col;
    if(str_t1.find(next_ret_col_t)==string::npos){
         //cout<<"column not found"<<endl;
         return;
     }
     
     
    ////cout<<"join:"<<str1<<endl;
	size_t splitColsLine1 = str1.find("=");
    string temp_cols1 = str1.substr(splitColsLine1+1, (str1.length()-splitColsLine1-1));
   // //cout<<"tempcols1"<<temp_cols1<<endl;
	int size_table2= getRecordSize(temp_cols1);
	vector<int> typevector_table2;
	vector<int> sizevector_table2;
	int lenCols1,column_count1=0,cum_ignore_value_k2=0;
	int column_number_k2,ignore_value_k2;
   lenCols1 = temp_cols1.find(",");
   ////cout<<"lencols1:"<<lenCols1 <<endl;
	 while((lenCols1 = temp_cols1.find(","))>0 && lenCols1<temp_cols1.length()){
   ////cout<<"in while"<<endl;
		string findType1 = temp_cols1.substr(temp_cols1.find(":")+1, lenCols1-temp_cols1.find(":")-1);
		string colName1 = temp_cols1.substr(0,temp_cols1.find(":"));
		string tempTbl = next_table;
		orderByColNames.push_back(tempTbl.append("\.").append(colName1));
		if(star==1){
			if(sel_col.compare("")==0)
				sel_col=sel_col+orderByColNames[orderByColNames.size()-1];
			else
				sel_col=sel_col+","+orderByColNames[orderByColNames.size()-1];
		}
		
		tempTbl="";
  // //cout<<"column name:"<<colName1<<endl;
		column_count1++;
		if(colName1.compare(next_ret_col)==0){
			column_number_k2=column_count1;
			
			//typevector_table1.push_back(findType);
			int findval=findType1.find("(");
			if(findval>0){
				typevector_table2.push_back(0);
				string size1=findType1.substr(findType1.find("(")+1,findType1.find(")")-findType1.find("(")-1);
				sizevector_table2.push_back(atoi(size1.c_str()));
				ignore_value_k1=atoi(size1.c_str());
				if(next_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(0);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(atoi(size1.c_str()));
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += atoi(size1.c_str());
			}
			else{
				typevector_table2.push_back(1);
				sizevector_table2.push_back(4);
				if(next_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(1);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(4);
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += 4;
        ignore_value_k1=4;
			}
			
		
		}
		else{
			int findval=findType1.find("(");
			if(findval>0){
				typevector_table2.push_back(0);
				string size1=findType1.substr(findType1.find("(")+1,findType1.find(")")-findType1.find("(")-1);
				sizevector_table2.push_back(atoi(size1.c_str()));
				if(next_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(0);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(atoi(size1.c_str()));
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += atoi(size1.c_str());
			}
			else{
				typevector_table2.push_back(1);
				sizevector_table2.push_back(4);
				if(next_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(1);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(4);
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += 4;
			}
			
		}
		
		temp_cols1 = temp_cols1.substr(lenCols1+1,temp_cols1.length()-lenCols1-1);	
			
		}
   string findType1 = temp_cols1.substr(temp_cols1.find(":")+1, lenCols1-temp_cols1.find(":")-1);
		string colName1 = temp_cols1.substr(0,temp_cols1.find(":"));
		tempTbl = "";
		tempTbl = next_table;
		orderByColNames.push_back(tempTbl.append("\.").append(colName1));
		if(star==1){
			if(sel_col.compare("")==0)
				sel_col=sel_col+orderByColNames[orderByColNames.size()-1];
			else
				sel_col=sel_col+","+orderByColNames[orderByColNames.size()-1];
		}
		tempTbl="";
		column_count1++;
		if(colName1.compare(next_ret_col)==0){
			column_number_k2=column_count1;
			
			//typevector_table1.push_back(findType);
			int findval=findType1.find("(");
			if(findval>0){
				typevector_table2.push_back(0);
				string size1=findType1.substr(findType1.find("(")+1,findType1.find(")")-findType1.find("(")-1);
				sizevector_table2.push_back(atoi(size1.c_str()));
				ignore_value_k1=atoi(size1.c_str());
				if(next_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(0);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(atoi(size1.c_str()));
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += atoi(size1.c_str());
			}
			else{
				typevector_table2.push_back(1);
				sizevector_table2.push_back(4);
				if(next_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(1);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(4);
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += 4;
        ignore_value_k1=4;
			}
			
		
		}
		else{
			int findval=findType1.find("(");
			if(findval>0){
				typevector_table2.push_back(0);
				string size1=findType1.substr(findType1.find("(")+1,findType1.find(")")-findType1.find("(")-1);
				sizevector_table2.push_back(atoi(size1.c_str()));
				if(next_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(0);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(atoi(size1.c_str()));
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += atoi(size1.c_str());
			}
			else{
				typevector_table2.push_back(1);
				sizevector_table2.push_back(4);
				if(next_table.compare(join_where_tbl)==0){
					where_table.push_back(orderByColNames[orderByColNames.size()-1]);
				}
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(1);
				sizeTypeMap[orderByColNames[orderByColNames.size()-1]].push_back(4);
				
				cumMap[orderByColNames[orderByColNames.size()-1]] = cumVal;
				cumVal += 4;
			}
			
		}
   ////cout<<"column_count"<<column_number_k2<<endl;
   //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
   string temp_sel_col = sel_col;
   int j=1;
   	while(j<column_number_k2){
			cum_ignore_value_k2=cum_ignore_value_k2+sizevector_table2[j-1];
			j++;
		}
	
	vector<string> table2_k2;	
	fstream file_table2;
	string tempt2=next_table;
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	
	
	//cout<<"started where"<<endl;
		int pos2;
	if(where_table.size()>0){
		cout<<"+++++++++++++++++++++++++"<<endl;
		 orderByColName1 = orderByColName;
		 string tApp = join_where_tbl;
		 string whereTableOrder = "";
		 orderByColName = tApp.append("\.").append(join_where_col);
		 from_where=1;
		 cout<<"+++++++++++++++++++++++++"<<endl;
		 order(join_where_tbl,where_table);
		 cout<<"+++++++++++++++++++++++++"<<endl;
		 whereTableOrder = whereTableOrder.append("ordered");
		 binarySearch(whereTableOrder,where_table);
		 cout<<"+++++++++++++++++++++++++"<<endl;
		 whereTableOrder = whereTableOrder.append("Where");
		 from_where=0;
		 fileName = whereTableOrder;
		 orderByColName = orderByColName1;
		 
		 tApp="";
		 file_table2.open((whereTableOrder).c_str(),ios::binary|ios::in|ios::ate);
		 pos2=0;
		 tableNumber=2;
		 whereTableOrder="";
	}
	else{
		file_table2.open((tempt2.append("\.tbl")).c_str(),ios::binary|ios::in|ios::ate);
		pos2=8;
	}
	where_table.clear();
	//cout<<"cleared where_table"<<where_table.size()<<endl;
	
	
	
	
	
	
	
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	 //file_table2.open((tempt2.append("\.tbl")).c_str(),ios::binary|ios::in|ios::ate);
	if(!file_table2){
	cout<<"file not found"<<endl;
	}
	int size1=file_table2.tellg();
	file_table2.seekg(pos2,ios::beg);
	int pos_t2= file_table2.tellg();
	while(pos_t2<size1){
		if(typevector_table2[column_number_k2-1]==0){
			char *strcolval=new char[100]();
			//char strcolval[sizevector_table2[column_number_k2-1]];
			file_table2.seekg(cum_ignore_value_k2+pos_t2);
			file_table2.read(strcolval,sizevector_table2[column_number_k2-1]);
			table2_k2.push_back(string(strcolval));
			delete[] strcolval;
		}
		else{
			int intcolval;	
      /////cout<<"cum_ignore_value_k2:"<<cum_ignore_value_k2<<endl;
			file_table2.seekg(cum_ignore_value_k2+pos_t2);
			////cout<<"fp position:"<<file_table2.tellg()<<endl;
      ////cout<<"sizevector_table1[column_count] "<<sizevector_table2[column_number_k2-1]<<endl;
			file_table2.read(reinterpret_cast<char *>(&intcolval),sizevector_table2[column_number_k2-1]);
      ////cout<<"fp position:"<<file_table2.tellg()<<endl;
      ostringstream ss;
      ss<<intcolval;
			table2_k2.push_back(ss.str());
		}
	pos_t2=pos_t2+size_table2;
		
	}
	for(size_t i=0;i<sizevector_table1.size();i++){
		////cout<<"sizevector:"<<sizevector_table1[i]<<endl;	
	}
 for(size_t i=0;i<table2_k2.size();i++){
		//cout<<"values:    "<<table2_k2[i]<<"\t";	
	}
	
 for(size_t i=0;i<sizevector_table2.size();i++){
		////cout<<"sizevector:"<<sizevector_table2[i]<<endl;	
	}

 file_table2.close();
	file_table1.close();
	//cout<<"file_table2 closed"<<endl;
 //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
 for(int b=0;b<orderByColNames.size();b++){
	 //cout<<orderByColNames[b]<<endl;
 }
 colCount=0;
 file_table1.close();
file_table2.close();


//cout<<"------------------------------------------------------------------------------------------------------------------------------------------------\n";


if ((column_count==1)&&(column_count1==1)){
		for(i=0;i<table1_k1.size();i++){
			for(j=0;j<table2_k2.size();j++){
					if(table1_k1[i]==table2_k2[j]){
						//cout<<table1_k1[i]<<"\n";
					
					
					
					
					}
			
			
			
			}
		}
	}
	
	else{

 ////cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1"<<endl;
 int count=0;
 
 
 //cout<<"tableNumber : "<<tableNumber<<endl;
 if(tableNumber==1){
	  file_table1.open((fileName).c_str(),ios::binary|ios::in);
	  file_table2.open((tempt2).c_str(),ios::binary|ios::in);
	  pos1=0;pos2=8;
 }
 else if(tableNumber==2){
	 file_table1.open((tempt1).c_str(),ios::binary|ios::in);
	 file_table2.open((fileName).c_str(),ios::binary|ios::in);
	 pos1=8;pos2=0;
 }
 else{
	file_table1.open((tempt1).c_str(),ios::binary|ios::in);
	file_table2.open((tempt2).c_str(),ios::binary|ios::in);
	pos1=8;pos2=8;
 }
 
 
 
 
 file_table2.seekg(pos2);
 file_table1.seekg(pos1);
 pos_t1=file_table1.tellg();
 pos_t2=file_table2.tellg();
//cout<<"pos_t1: "<<pos_t1<<endl;
//cout<<"pos_t2: "<<pos_t2<<endl;
 //cout<<"size_table1 : "<<size_table1<<endl;
//cout<<"size_table2 : "<<size_table2<<endl;
  //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
  if(order_by==0 && group_by==0)
	  cout<<"\n\nJOIN OUTPUT\n-----------------------------------------------------------------------------------\n";
int flag=0;
   for (int i=0 ; i<table1_k1.size();i++){
   //cout<<"in first for"<<endl;
		
       for (j=0; j<table2_k2.size();j++){
		   colCount=0;
       ////cout<<"in secoond for"<<endl;
       file_table1.seekg(pos_t1);
           if (table1_k1[i]==table2_k2[j]){
           count=0;
		   ////cout<<"count : "<<count<<endl;
             while(file_table1.tellg()<pos_t1+size_table1) {
             ////cout<<"inside while"<<endl;
               count++;
			   string colNme = orderByColNames[colCount];
			   //cout<<"colNmeOut : "<<colNme<<endl;
			   if(temp_sel_col.find(colNme)==string::npos){
				   //cout<<"colNme if : "<<colNme<<endl;
				   colNme="";
				   colCount++;
				   //cout<<"colCount  "<<colCount<<endl;
				   if(file_table1.tellg()+sizevector_table1[count-1]==size)
						break;
				   file_table1.ignore(sizevector_table1[count-1]);
				   //cout<<"pos_t2 if"<< pos_t2<<endl;
				   //cout<<"pos_t1 if"<< pos_t1<<endl;
				   continue;
				   
			   }
			   //cout<<"colNme else : "<<colNme<<endl;
			   colCount++;
			   if(flag<colCount){
					finalOrderByColNames.push_back(colNme);
					flag=colCount;
			   }
			   
			   //cout<<"colCount  "<<colCount<<endl;
			   
				if (count==column_number_k1){
				   
				   string init = init_table;
					 string next = next_table;
					 init.append("\.").append(init_ret_col);
					 next.append("\.").append(next_ret_col);
					 if (star==1 || ((sel_col.find(init)!=string::npos) && (sel_col.find(next)!=string::npos))){
							file_table1.ignore(sizevector_table1[count-1]);
							continue;
					 }
				}
					if (typevector_table1[count-1]==0){
                     ////cout<<"inside char if "<<endl;
					 	//char strcolval[100];
						char *strcolval=new char[100]();
                     //char strcolval[sizevector_table1[count-1]];
                     ////cout<<"sizevector_table1[count-1]:"<<sizevector_table1[count-1]<<endl;
                     file_table1.read(strcolval,sizevector_table1[count-1]);
					 joinOut.write(strcolval,sizevector_table1[count-1]);
					 if(order_by==0&& group_by==0)
						cout<<string(strcolval).append("\t");
						delete []strcolval;
                   }
                   else{
                   ////cout<<"inside int if "<<count<<endl;
                     int intcolval;
                     ////cout<<"sizevector_table1[count-1]:"<<sizevector_table1[count-1]<<endl;
                     file_table1.read(reinterpret_cast<char*> (&intcolval),sizevector_table1[count-1]);
                     joinOut.write(reinterpret_cast<char*> (&intcolval),sizevector_table1[count-1]);
                     ostringstream ss;
                     ss<<intcolval;
					 if(order_by==0&& group_by==0)
						cout<<ss.str().append("\t");
                   
                   }
			 }
               
             count=0;
			 ////cout<<"count : "<<count<<endl;
		//cout<<"pos_t2    :"<<pos_t2<<endl;
        file_table2.seekg(pos_t2);
        //cout<<"fp is :"<<file_table2.tellg()<<endl;
		////cout<<"$$$$$$$$$$$$$$$$$   "<<size1<<endl;
		//cout<<"########################"<<endl;
             while((file_table2.tellg()<pos_t2+size_table2) &&(file_table2.tellg()>0) ){
				 
             //cout<<"inside while"<<(file_table2.tellg())<<endl;
               count++ ;
			   string colNme = orderByColNames[colCount];
				   //cout<<"colNmeOut : "<<colNme<<endl;
			   if(temp_sel_col.find(colNme)==string::npos){
				   //cout<<"colNme if : "<<colNme<<endl;
				   colNme="";
				   colCount++;
				   //cout<<"colCount if"<< colCount<<endl;
				   ////cout<<"(pos_t2+sizevector_table2[count-1]  "<<(pos_t2+sizevector_table2[count-1])<<endl;
					if(file_table2.tellg()+sizevector_table2[count-1]==size1)
						break;
						file_table2.ignore(sizevector_table2[count-1]);
				    //cout<<"inside while"<<(file_table2.tellg())<<endl;
				   continue;
				   
			   }
			   //cout<<"colNme else : "<<colNme<<endl;
			   colCount++;
			   if(flag<colCount){
					finalOrderByColNames.push_back(colNme);
					flag=colCount;
			   }
			    //cout<<"colCount else"<< colCount<<endl;
               ////cout<<"count : "<<count<<endl;
               ////cout<<"typevector_table2[count-1] :"<<typevector_table2[count-1]<<endl;
               if (typevector_table2[count-1]==0){
				   	//char strcolval[100];
					char *strcolval=new char[100]();
					//char strcolval[sizevector_table2[count-1]];
                     file_table2.read(strcolval,sizevector_table2[count-1]);
					 joinOut.write(strcolval,sizevector_table2[count-1]);
					 if(order_by==0&& group_by==0)
						cout<<string(strcolval).append("\t");
                   delete []strcolval;
                   }
                   else{
                  // ////cout<<"in int 2"<<endl;
                     int intcolval;
                    // ////cout<<"sizevector_table2[count-1] : "<<sizevector_table2[count-1]<<endl;
                     file_table2.read(reinterpret_cast<char*> (&intcolval),sizevector_table2[count-1]);
					 joinOut.write(reinterpret_cast<char*> (&intcolval),sizevector_table2[count-1]);
                     ostringstream ss;
                     ss<<intcolval;
					 if(order_by==0&& group_by==0)
						cout<<ss.str().append("\t");
                   
                   }
               ////cout<<"fp is :"<<file_table2.tellg()<<endl;
           
           }
       
      if(order_by==0 && group_by==0)
		cout<<"\n";
       }
       else{
       ////cout<<"ignoring ft2"<<file_table2.tellg();
       if(file_table2.tellg()<pos_t2+size_table2 && (file_table2.tellg()+size_table2 )<pos_t2+size_table2)
	         file_table2.ignore(size_table2);
		 
       
       //cout<<"ignored ft2: "<<file_table2.tellg()<<endl;
	   }
	   count=0;
       pos_t2=pos_t2+size_table2;
        
         
 
 }
	pos_t1=pos_t1+size_table1;
   file_table1.seekg(pos_t1);
   pos_t2=8;
   //cout<<"after 1st loop: ***************"<<pos_t2 <<"    "<<file_table2.tellg()<<endl;
   file_table2.seekg(pos_t2);
   //cout<<"after 1st loop: "<<file_table2.tellg()<<endl;
   
   
}	

 
   
}
int finalSize=0;
for (int i=0;i<finalOrderByColNames.size();i++){
	//cout<< finalOrderByColNames[i]<<"\t";
	finalSize += sizeTypeMap[finalOrderByColNames[i]][1]; 
}
//cout<<"\n";
file_table1.close();
file_table2.close();
joinOut.close();
string selTbl="";
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
orderByColName1="";
orderByColName1 = orderByColName;
			if(groupByColName!=""){
				//cout<<"no where"<<endl;
				//cout<<"orderByColName cpy*"<<orderByColName<<endl;
				//orderByColName1 = orderByColName;
				//from_groupBy=1;
				orderByColName="";
				orderByColName = groupByColName;
				//cout<<"joinTbl in join "<<joinTbl<<endl;
				order(joinTbl,finalOrderByColNames);
				string table = "";
				table = table.append("ordered");
				//cout<<"table in join "<<table<<endl;
				orderByColName="";
				//orderByColName = orderByColName1;
				//cout<<"orderByColName "<<orderByColName<<endl;
				if(sizeTypeMap[sumAttr][0]==0){
					cout<<"sum column cannot be a character type"<<endl;
					clear();
					return ;
				}
				//cout<<sumAttr<<"   *"<<groupByColName<<"   *"<<sel_col<<endl;
				if(sumAttr.compare(groupByColName)==0){
					//cout<<"same"<<endl;
					if(sel_col!=""){
						clear();
						return;
					}
					else{
						groupByCase = 0;
						//cout<<"groupByCase : "<<groupByCase<<endl;
						join_grpBy = 1;
						groupBy(table,finalSize);
						join_grpBy = 0;
						selTbl = "";
						selTbl.append(table).append("GroupBy");
						grpBySel.push_back(sel_col);
					}
				}
				else{
					string temp1 = sel_col;
					temp1=temp1.substr(sel_col.find(",")+1,(sel_col.length()-sel_col.find(",")-1));
					//cout<<"temp1 : "<<temp1<<endl;
					if(temp1.find(",")!=string::npos){
						cout<<" Invalid query for group  by"<<endl;
						clear();
						return;
					}
					else{
						groupByCase = 1;
						sel_col = sel_col.substr(0,sel_col.find(","));
						//cout<<"groupByCase : "<<groupByCase<<"   "<<sel_col<<"    "<<finalSize<<" "<<table<<endl;
						join_grpBy = 1;
						if(orderByColName.compare(groupByColName)==0)
							from_order_by=1;
						groupBy(table,finalSize);
						from_order_by=0;
						join_grpBy = 0;
						selTbl = "";
						selTbl.append(table).append("GroupBy");
						int col = sel_col.find(",");
						string frstCol = sel_col.substr(0,col);
						grpBySel.push_back(frstCol);
						grpBySel.push_back(sel_col.substr(col+1,sel_col.length()-col-1));
					}
					temp1="";
				}
			}
			//groupByColName="";
			orderByColName="";
			orderByColName=orderByColName1;
			//from_groupBy = 0;
			
			
			
			 if(order_by ==1 /*&&  (orderByColName.compare(groupByColName)!=0)*/){
						 if(group_by==1){
							 string tempSum = "sum(";
							tempSum = tempSum.append(sumAttr).append(")");
							 //cout<<"tempSum  *"<<tempSum<<"    *"<<orderByColName<<endl;
							 //if(orderByColName.compare(tempSum)==0){
								 //orderByColName="";
								//orderByColName = tempSum;
									from_order_by=1;
									//cout<<"selTbl "<<selTbl<<endl;
									for(int u=0;u<grpBySel.size();u++)
									{
										//cout<<grpBySel[u]<<endl;
									}
									//cout<<"\n";
									//if(sizeTypeMap[grpBySel[1]][0]==1)
										//cout<<"same "<<sizeTypeMap["sum(id)"][1]<<endl;
									
									order(selTbl,grpBySel);
									from_order_by=0;
									grpBySel.clear();
									group_by=0;
							 //}
						 }
						 else{
							 from_order_by=1;
							 //cout<<"selTbl "<<selTbl<<endl;
							 //cout<<"**************************************************************"<<endl;
							// cout<<"sel_col = "<<sel_col<<endl;
							 std::vector<int>::iterator it1,it2;
							 string init = init_table;
							 string next = next_table;
							 init.append("\.").append(init_ret_col);
							 next.append("\.").append(next_ret_col);
							 if (star==1 || ((sel_col.find(init)!=string::npos) && (sel_col.find(next)!=string::npos)))
								 finalOrderByColNames.erase(std::remove(finalOrderByColNames.begin(), finalOrderByColNames.end(), init), finalOrderByColNames.end());
							 for(int t=0;t<finalOrderByColNames.size();t++){
								 cout<<finalOrderByColNames[t]<<"\t";
							 }
							 
							 cout<<"\n";
							 if(orderByColName.compare(init)==0){
								 orderByColName = "";
								 orderByColName = next;
							 }
							 order(joinTbl,finalOrderByColNames);	
							 from_order_by=0;
						 }
						 //cout<<" back from order"<<endl;
					 }
					 
					 clear();
			
			
			
			
			
			
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!			

}

void showTable(){
 fstream file;
    string temp_tbl = tablename;
    ifstream f((temp_tbl.append("\.tbl")).c_str());
    
    if(!f.good()){
        cout<<"table doesn't exist";
		clear();
        return;
    }
     string tblName = tablename;
    tblName=tblName.append("\.tbl");
    cout<<"Binary Table NAme:"<<tblName<<endl;
    file.open(tblName.c_str(),ios::in|ios::binary);
    size_t table_count;
    file.read(( char *)&table_count, sizeof(table_count));
    cout<<"count of table:"<<table_count<<endl;
    table_count = table_count-1;
    int line_number=table_count*6+4;
    ifstream file_catalog;
    file_catalog.open("catalog.txt");
    int line_count=1;
    string line;
    for( line; getline(file_catalog, line ); ) {
        line_count = line_count + 1;
        if (line_count > line_number && line_count <=line_number + 6) {
            cout << line << endl;
        } else if (line_count > line_number + 6) {
            break;
        }
    }


}


