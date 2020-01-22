%{
void yyerror(char *);
#include "y.tab.h"
extern YYSTYPE yylval;


%}
comment       [/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]
%%

[ \n\r]			{};
[ ]			{};
[\t]			{};
{comment}         { /* DO NOTHING */ ;}
sum|SUM				return sum;
select|SELECT        return SELECT;
create|CREATE        return CREATE;
insert|INSERT		return INSERT;
into|INTO		return INTO;
values|VALUES		return VALUES;
TABLES|tables		return TABLES;
show|SHOW		return SHOW;
drop|DROP		return DROP;
TABLE|table	return TABLE;
JOIN|join	return JOIN;
ON|on		return ON;
distinct        return DISTINCT;
from|FROM            return FROM;
where|WHERE           return WHERE;
like           	return LIKE;
desc         	return DESC;
"varchar" 	return VARCHAR;
asc           	return ASC;
"group by"|"GROUP BY"    	return GROUP;
having          return HAVING;
"order by"|"ORDER BY"     	return ORDER;
or              return OR;
and             return AND;
in              return IN;
int|INT		{yylval=strdup(yytext);return INT;}
char|CHAR	{yylval=strdup(yytext);return CHAR;}
"<="            return LE;
">="            return GE;
"=="            return EQ;
"!="            return NE;
quit		return QUIT;
[A-Za-z][A-Za-z0-9_#-]*           {yylval=strdup(yytext);return ID;}
[0-9]+		{yylval=strdup(yytext);return NUM;}
"primary key"|"PRIMARY KEY"	      {yylval=strdup(yytext);return PRIMARY_KEY;}
.               return yytext[0];

%%

int yywrap(void)
{

	return 1;
}
