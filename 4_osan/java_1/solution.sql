-- Мориков Алексей Михайлович 4 вариант
-- Morikov Alexey Mikhailovich 4 variant

-- 1st query

SELECT * FROM bd_employees
WHERE phone_number SIMILAR TO '8\-\(49(5|9)\)\-[0-9][0-9][0-9]\-[0-9][0-9]\-[0-9][0-9]'

-- 2nd query

SELECT * FROM bd_employees
WHERE phone_number SIMILAR TO '8\-\([0-9][0-9][0-9]\)\-[0-9][0-9][0-9][0-9]\-[0-9][0-9][0-9]'

-- 3rd query

-- Если всего два символа и сразу собака, то надо ли вставлять '_'?
SELECT last_name, 
       SUBSTRING(email, 0, 3) || '_' || SUBSTRING(email, POSITION('@' IN email)) AS email
FROM bd_employees;

-- 4th query

SELECT * FROM bd_employees
WHERE LENGTH(first_name) - LENGTH(REGEXP_REPLACE(first_name, '[aeiou]', '', 'g')) <= 3;
