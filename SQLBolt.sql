
/*
this is the answer to the 

https://sqlbolt.com/

*/

Exercise1
Find the title of each film 
SELECT Title FROM movies;

Find the director of each film 
SELECT Director FROM movies;

Find the title and director of each film
SELECT Title,Director FROM movies;

Find the title and year of each film 
SELECT * FROM movies;


Exercise2

Find the movie with a row id of 6 
SELECT * FROM movies where id=6;

Find the movies released in the years between 2000 and 2010 
SELECT * FROM movies where 2000 <= year and year<= 2010;

Find the movies not released in the years between 2000 and 2010 
SELECT * FROM movies where not(2000 <= year and year<= 2010);

Find the first 5 Pixar movies and their release year 
SELECT Title,year FROM movies LIMIT 5


Exercise3

Find all the Toy Story movies 
SELECT * from Movies where Title LIKE "Toy Story%" 

Find all the movies directed by John Lasseter
SELECT * from Movies where Director="John Lasseter" 

Find all the movies (and director) not directed by John Lasseter 
SELECT * from Movies where  not Director="John Lasseter" 

Find all the WALL-* movies 
SELECT * from Movies where Title LIKE "WALL%"


Exercise4

List all directors of Pixar movies (alphabetically), without duplicates 
SELECT DISTINCT Director FROM movies ORDER BY Director ASC;


List the last four Pixar movies released (ordered from most recent to least) 
SELECT * FROM movies ORDER BY Year DESC LIMIT 4;

List the first five Pixar movies sorted alphabetically 
SELECT * FROM movies ORDER BY Title ASC LIMIT 5;
 
List the next five Pixar movies sorted alphabetically 
SELECT * FROM movies ORDER BY Title ASC LIMIT 5 OFFSET 5;
--OFFSET=5番目から数え始め, LIMIT=5個　かぞえる



Exercise5

List all the Canadian cities and their populations
SELECT * FROM north_american_cities where Country=="Canada";

Order all the cities in the United States by their latitude from north to south 
SELECT * FROM north_american_cities where Country=="United States" ORDER BY Latitude DESC;


List all the cities west of Chicago, ordered from west to east 
-- sqlのなかのsql で副文
SELECT * FROM north_american_cities where Longitude<(SELECT Longitude FROM north_american_cities where City="Chicago") ORDER BY Longitude ASC;


List the two largest cities in Mexico (by population) 
SELECT City FROM north_american_cities where Country="Mexico" ORDER BY Population DESC LIMIT 2;

List the third and fourth largest cities (by population) in the United States and their population
SELECT City FROM north_american_cities where Country="United States" ORDER BY Population DESC LIMIT 2 OFFSET 2;



Exercise6

Find the domestic and international sales for each movie
SELECT m.Title,b.International_sales,b.Domestic_sales FROM Movies m,BoxOffice b where m.id=b.Movie_id;
もしくは以下でも正解
SELECT m.Title, b.International_sales, b.Domestic_sales FROM Movies m Inner JOIN Boxoffice b ON m.Id=b.Movie_id;

Show the sales numbers for each movie that did better internationally rather than domestically
SELECT m.Title, b.International_sales, b.Domestic_sales FROM Movies m Inner JOIN Boxoffice b ON m.Id=b.Movie_id where b.International_sales>b.Domestic_sales;
もしくは以下でも正解
SELECT m.Title,b.International_sales,b.Domestic_sales FROM Movies m,BoxOffice b where m.id=b.Movie_id and b.International_sales>b.Domestic_sales;


List all the movies by their ratings in descending order 
SELECT m.Title FROM Movies m,BoxOffice b where m.id=b.Movie_id ORDER BY b.Rating DESC
もしくは以下でも正解
SELECT m.Title,b.International_sales,b.Domestic_sales FROM Movies m,BoxOffice b where m.id=b.Movie_id ORDER BY b.Rating DESC



Exercise7

Find the list of all buildings that have employees 
SELECT DISTINCT b.* FROM Employees e INNER JOIN Buildings b ON e.Building = b.Building_name 

Find the list of all buildings and their capacity 
SELECT b.*,b.Capacity FROM Buildings b 


List all buildings and the distinct employee roles in each building (including empty buildings) 
SELECT DISTINCT b.*, e.Role FROM Buildings b LEFT JOIN Employees e ON e.Building = b.Building_name 


Exercise8
Find the name and role of all employees who have not been assigned to a building
SELECT Name,Role FROM employees  WHERE Building IS NULL;


Find the names of the buildings that hold no employees
SELECT b.Building_name FROM Buildings b LEFT JOIN employees e on b.Building_name=e.Building WHERE e.Building IS NULL;


Exercise9

List all movies and their combined sales in millions of dollars
1 million=1e6
SELECT m.Title,(b.Domestic_sales+International_sales)/1000000 FROM Movies m INNER JOIN BoxOffice b ON m.Id=b.Movie_id;

--Ratingが何を表しているのか不明
List all movies and their ratings in percent 
SELECT m.Title, b.Rating*10 AS percent FROM Movies m INNER JOIN BoxOffice b ON m.Id=b.Movie_id;

List all movies that were released on even number years 
SELECT Title FROM Movies m where m.Year%2==0


Exercise10

--The GROUP BY clause works by grouping rows that have the same value in the column specified. 
Find the longest time that an employee has been at the studio
SELECT Max(Years_employed) FROM employees;

For each role, find the average number of years employed by employees in that role 
SELECT Role,AVG(Years_employed) FROM Employees GROUP BY Role;

Find the total number of employee years worked in each building 
SELECT Building,SUM(Years_employed) FROM Employees GROUP BY Building;


Exercise11
/*
Our queries are getting fairly complex, 
but we have nearly introduced all the important parts of a SELECT query. 
One thing that you might have noticed is that if the GROUP BY clause is 
executed after the WHERE clause (which filters the rows which are to be grouped), 
then how exactly do we filter the grouped rows?

Luckily, SQL allows us to do this by adding an additional HAVING clause
which is used specifically with the GROUP BY clause 
to allow us to filter grouped rows from the result set.

SELECT group_by_column, AGG_FUNC(column_expression) AS aggregate_result_alias, …
FROM mytable
WHERE condition
GROUP BY column
HAVING group_condition;


The HAVING clause constraints are written the same way as the WHERE clause constraints, and are applied to the grouped rows. With our examples, this might not seem like a particularly useful construct, but if you imagine data with millions of rows with different properties, being able to apply additional constraints is often necessary to quickly make sense of the data.
Did you know?

If you aren't using the `GROUP BY` clause, a simple `WHERE` clause will suffice.
Exercise
*/

Find the number of Artists in the studio (without a HAVING clause) 
SELECT count(Name) FROM employees where Role="Artist";

Find the number of Employees of each role in the studio
SELECT Role,count(Role) FROM Employees Group BY Role;

Find the total number of years employed by all Engineers 
SELECT SUM(Years_employed) FROM employees where Role="Engineer";


Exercise12
/*


Complete SELECT query
SELECT DISTINCT column, AGG_FUNC(column_or_expression), …
FROM mytable
    JOIN another_table
      ON mytable.column = another_table.column
    WHERE constraint_expression
    GROUP BY column
    HAVING constraint_expression
    ORDER BY column ASC/DESC
    LIMIT count OFFSET COUNT;

Each query begins with finding the data that we need in a database, 
and then filtering that data down into something that can be processed 
and understood as quickly as possible. Because each part of the query 
is executed sequentially, 
it's important to understand the order of execution so that you know
 what results are accessible where.
Query order of execution
1. FROM and JOINs

The FROM clause, and subsequent JOINs are first executed to determine the total working set of data that is being queried. This includes subqueries in this clause, and can cause temporary tables to be created under the hood containing all the columns and rows of the tables being joined.
2. WHERE

Once we have the total working set of data, the first-pass WHERE constraints are applied to the individual rows, and rows that do not satisfy the constraint are discarded. Each of the constraints can only access columns directly from the tables requested in the FROM clause. Aliases in the SELECT part of the query are not accessible in most databases since they may include expressions dependent on parts of the query that have not yet executed.
3. GROUP BY

The remaining rows after the WHERE constraints are applied are then grouped based on common values in the column specified in the GROUP BY clause. As a result of the grouping, there will only be as many rows as there are unique values in that column. Implicitly, this means that you should only need to use this when you have aggregate functions in your query.
4. HAVING

If the query has a GROUP BY clause, then the constraints in the HAVING clause are then applied to the grouped rows, discard the grouped rows that don't satisfy the constraint. Like the WHERE clause, aliases are also not accessible from this step in most databases.
5. SELECT

Any expressions in the SELECT part of the query are finally computed.
6. DISTINCT

Of the remaining rows, rows with duplicate values in the column marked as DISTINCT will be discarded.
7. ORDER BY

If an order is specified by the ORDER BY clause, the rows are then sorted by the specified data in either ascending or descending order. Since all the expressions in the SELECT part of the query have been computed, you can reference aliases in this clause.
8. LIMIT / OFFSET

Finally, the rows that fall outside the range specified by the LIMIT and OFFSET are discarded, leaving the final set of rows to be returned from the query.

*/
Find the number of movies each director has directed
SELECT Director,COUNT(Title) FROM Movies GROUP BY Director;

Find the total domestic and international sales that can be attributed to each director 
SELECT m.Director,SUM(b.Domestic_sales+b.International_sales) FROM Movies m JOIN Boxoffice b on m.id=b.Movie_id  GROUP BY m.Director;



Exercise13

--Add the studio's new production, Toy Story 4 to the list of movies (you can use any director)

INSERT INTO Movies VALUES(15,"Toy Story 4", "Josh Cooley",2019,100)
/*
Toy Story 4 has been released to critical acclaim!
It had a rating of 8.7, and made 340 million domestically and 270 million internationally.
Add the record to the BoxOffice table. 
*/
INSERT INTO Boxoffice VALUES(15,8.7,340*1e6,270*1e6)



Exercise14
/*
The director for A Bug's Life is incorrect, 
it was actually directed by John Lasseter 

*/
UPDATE Movies SET Director="John Lasseter" Where Title="A Bug's Life";

/*
The year that Toy Story 2 was released is incorrect, 
it was actually released in 1999
*/
UPDATE Movies SET Year="1999" Where Title="Toy Story 2";
/*
Both the title and director for Toy Story 8 is incorrect! 
The title should be "Toy Story 3" and it was directed by Lee Unkrich
*/
UPDATE Movies SET Title="Toy Story 3",Director="Lee Unkrich" Where Title="Toy Story 8";



/*
This database is getting too big, 
lets remove all movies that were released before 2005. 
*/
DELETE FROM Movies where Year<2005

/*
Andrew Stanton has also left the studio, 
so please remove all movies directed by him.
*/
DELETE from Movies Where Director="Andrew Stanton"



Exercise16
/*
When you have new entities and relationships to store in your database,
you can create a new database table using the CREATE TABLE statement.
Create table statement w/ optional table constraint and default value
CREATE TABLE IF NOT EXISTS mytable (
    column DataType TableConstraint DEFAULT default_value,
    another_column DataType TableConstraint DEFAULT default_value,
    …
);

The structure of the new table is defined by its table schema,
 which defines a series of columns. Each column has a name, 
 the type of data allowed in that column, 
 an optional table constraint on values being inserted, 
 and an optional default value.

If there already exists a table with the same name, 
the SQL implementation will usually throw an error, 
so to suppress the error and skip creating a table if one exists,
 you can use the IF NOT EXISTS clause.



CREATE TABLE movies (
    id INTEGER PRIMARY KEY,
    title TEXT,
    director TEXT,
    year INTEGER, 
    length_minutes INTEGER
);

*/

/*
Create a new table named Database with the following columns:

– Name A string (text) describing the name of the database
– Version A number (floating point) of the latest version of this database
– Download_count An integer count of the number of times this database was downloaded
This table has no constraints. 
*/

CREATE TABLE IF NOT EXISTS Database(
    Name TEXT,
    Version FLOAT,
    Download_count INTEGER
);

--今回はこれでもよいが,うまく実行されない
CREATE TABLE IF NOT EXISTS Database(
    id INTEGER PRIMARY KEY,
    Name TEXT,
    Version FLOAT,
    Download_count INTEGER
);


Exercise17
/*
Add a column named Aspect_ratio with a FLOAT data type 
to store the aspect-ratio each movie was released in.
*/

ALTER TABLE Movies ADD Aspect_ratio FLOAT
/*
Add another column named Language with a TEXT data type 
to store the language that the movie was released in.
Ensure that the default for this language is English. 
*/
ALTER TABLE Movies ADD Language TEXT DEFAULT English


Exercise18
/*
We've sadly reached the end of our lessons, 
lets clean up by removing the Movies table
*/
DROP TABLE IF EXISTS Movies;
/*
And drop the BoxOffice table as well 
*/
DROP TABLE IF EXISTS Boxoffice;

