# DatabaseUni
This project is simulating a database. 


## Available commands:
- import ${path} - Adds a table to the catalog ("import ./tables/salary.txt")
- showtables - Show all available tables from the catalog
- describe ${tableName} - Finds the table and prints the col names and types
- print ${tableName} - Enters "PRINT MODE" <br/>
Available commands in this mode: "next","previous" and "exit"
- export ${tableName} ${file name} - exports the table to file ("export salary ..//salaries.txt")
- select ${col-number} ${value} ${tableName} - prints all rows from table where on ${col-number} we have the value-${value} ("select 2 700.00 salary"")
- addcolumn ${tableName} ${col-name} ${col-type} - adds new col with name in table and sets a type for the col. For every existing row we add the NULL value. ("addcolumn salary egn string")
- update ${tableName} ${search-col-number} ${search-val} ${target-col-number} ${targetValue} - changes the value of rows which have search-val ("update salary 1 1 3 123456789")
- delete ${tableName} ${search-col-number} ${search-val} - deletes all rows which contains searched value ("delete salary 1 13")
- insert ${tableName} ${column-1} … ${column-n> - inserts new row ("insert salary 14 -714.00")
- innerjoin ${firstTableName} ${col-1} ${secondTableName} ${col-2} - creates new table made by joining the two tables. ("innerjoin users 1 salary 1")
- rename ${oldName} ${newName} - renames the table ("rename users usesrrs")
- count ${tableName} ${search-col-number} ${search-val} - prints the number of rows containing the value ("count salary 1 5")
- aggregate ${tableName} ${search-col-number} ${search-val} ${target-col-number} ${operation} - finds all the rows containing the value and executes an operation ("aggregate salary 1 3 2 sum") <br/>
Available operations: "sum", "product", "maximum", "minimum"


## Table format:
The first string on the row is the name of the table. Then we have the data types on the same row.
On the next row we have the columns names and on every new line we have the table rows.

Example: ("./tables/users.txt") <br/>
users,int,string,int  <br/>
id,name,age  <br/>
1,"moni",20  <br/>
2,"viki",22  <br/>

Table name: "users", Types: "int,string,int", Col names: "id,name,age", Rows: "1,"moni",20" and "2,"viki",22"

## Catalog format:
The catalog contains all imported tables with their names and paths.

Example:<br/>
${tableName} - ${tablePath} <br/>
salary - ..//tables//salary.txt  <br/>
users - ..//tables//users.txt <br/>
users_JOIN_salary_COL_1_AND_1 - ..//tables//users_JOIN_salary_COL_1_AND_1.txt <br/>

