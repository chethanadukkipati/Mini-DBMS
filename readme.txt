This project runs only on Linux and equivalent.
Run the command make to make the project
Execute "./dbms" for console input.
Execute "./dbms script='[filename].sql" for file input, where [filename] is the input sql filename.
The outputs are given on the console itself.
In the join query, the projecion columns should be given in the same order as that of the tables given in the join query. And also, if you give "where" in "join", then the table on which you give the where command should come first in the join.
Example1:
select T1.a,T1.b, T2.c from T1 join T2 on T1.a = T2.a where T1.a = value;
 Example2:
 select T2.a,T2.b, T1.c from T2 join T1 on T2.a = T1.a where T2.a = value;
