all:
	yacc -d sql.y
	flex sql.l
	g++ -o dbms y.tab.c                     
