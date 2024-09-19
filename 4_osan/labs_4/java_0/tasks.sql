-- Мориков Алексей Михайлович 4 вариант
-- Morikov Alexey Mikhailovich 4 variant

-- 1st query

SELECT
    First_name || ' ' || Last_name AS "ФИО",
    Salary AS "ОКЛАД",
    ROUND(Salary * 0.87, 0) AS "Оклад минус подоходный"
FROM employees;

-- 2nd query 

SELECT
    First_name AS "Имя",
    Last_name AS "Фамилия",
    Job_id AS "Должность",
    Hire_date AS "Дата приема на работу"
FROM employees
-- В созданной таблице нет других значений, условие всегда выполнено
WHERE Hire_date NOT BETWEEN '1995-01-01' AND '1999-12-31'
    OR Job_id IN ('AD_PRES', 'AD_VP', 'AD_ASST')
LIMIT 5;

-- 3rd query

SELECT
    First_name AS "Имя",
    Last_name AS "Фамилия",
    LOWER( 
        CONCAT(
            SUBSTRING(First_name, LENGTH(First_name) - 1, 2),
            SUBSTRING(Last_name, 0, 4)
            )
        ) AS "Идентификатор"
FROM employees;

-- 4th query

SELECT
    job_id AS "Должность",
    ROUND( MAX(Salary), 0 ) AS "Максимальная зарплата",
    ROUND( MIN(Salary), 0 ) AS "Минимальная зарплата",
    ROUND( AVG(Salary), 2 ) AS "Средняя зарплата"
FROM employees
GROUP BY job_id;
