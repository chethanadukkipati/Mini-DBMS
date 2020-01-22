SELECT person.personid,account.t2
FROM person JOIN account ON account.personid = person.personid
where person.yob>1991;