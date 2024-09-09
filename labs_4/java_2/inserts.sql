DROP TABLE people;

CREATE TABLE people(
  id INTEGER,
  lastname varchar(64),
  firstname varchar(64),
  secondname varchar(64),
  age integer,
  oid INTEGER,
  PRIMARY KEY(lastname, firstname, oid)
);

INSERT into people (id, lastname, firstname, secondname, age, oid)
VALUES (1, 'Ivanov', 'Ivan', 'Ivanovich', 30, 1);
INSERT into people (id, lastname, firstname, secondname, age, oid)
VALUES (2, 'Petrov', 'Petr', 'Petrovich', 40, 2);
INSERT into people (id, lastname, firstname, secondname, age, oid)
VALUES (3, 'Sidorov', 'Sidor', 'Sidorovich', 30, 3);
INSERT into people (id, lastname, firstname, secondname, age, oid)
VALUES (4, 'Vasilev', 'Petr', 'Vasilevich', 60, 4);
INSERT into people (id, lastname, firstname, secondname, age, oid)
VALUES (5, 'Mihailov', 'Mihail', 'Ivanovich', 30, 5);

SELECT * FROM people;