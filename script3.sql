SELECT * FROM person ORDER BY yob;

SELECT yob, name FROM person ORDER BY name;

SELECT personid, SUM(balance)
FROM account WHERE personid >= 8
GROUP BY personid;

SELECT personid, SUM(balance)
FROM account
GROUP BY personid
ORDER BY personid;

SELECT *
FROM person JOIN account ON account.personid = person.personid
WHERE person.name = 'Peter';

SELECT person.name, SUM(account.balance)
FROM person JOIN account ON person.personid = account.personid
WHERE person.yob >= 1987 group by person.name;

