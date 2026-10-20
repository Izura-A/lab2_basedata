USE master;
GO
IF EXISTS (SELECT * FROM sys.server_principals WHERE name = 'Admin') DROP LOGIN Admin;
IF EXISTS (SELECT * FROM sys.server_principals WHERE name = 'Customer') DROP LOGIN Customer;
IF EXISTS (SELECT * FROM sys.server_principals WHERE name = 'Warehousestaff') DROP LOGIN Warehousestaff;
IF EXISTS (SELECT * FROM sys.server_principals WHERE name = 'Supportmanager') DROP LOGIN Supportmanager;

IF EXISTS (SELECT * FROM sys.server_principals WHERE name LIKE 'Cust%' AND name != 'Customer') DROP LOGIN Cust1, Cust2, Cust3;
IF EXISTS (SELECT * FROM sys.server_principals WHERE name LIKE 'Ware%' AND name != 'Warehousestaff') DROP LOGIN Ware1, Ware2, Ware3;
IF EXISTS (SELECT * FROM sys.server_principals WHERE name LIKE 'Supp%' AND name != 'Supportmanager') DROP LOGIN Supp1, Supp2, Supp3;
GO
  
CREATE LOGIN Admin WITH PASSWORD = 'Admin123!';
USE OnlineStore;
CREATE USER Admin FOR LOGIN Admin;
ALTER ROLE db_owner ADD MEMBER Admin;
GO

CREATE LOGIN Customer WITH PASSWORD = 'Cust123!';
USE OnlineStore;
CREATE USER Customer FOR LOGIN Customer;
GRANT INSERT ON Orders TO Customer;
GRANT SELECT ON Orders TO Customer;
CREATE LOGIN Cust1 WITH PASSWORD = 'Cust123!'; CREATE USER Cust1 FOR LOGIN Cust1; ALTER ROLE Customer ADD MEMBER Cust1;
CREATE LOGIN Cust2 WITH PASSWORD = 'Cust123!'; CREATE USER Cust2 FOR LOGIN Cust2; ALTER ROLE Customer ADD MEMBER Cust2;
CREATE LOGIN Cust3 WITH PASSWORD = 'Cust123!'; CREATE USER Cust3 FOR LOGIN Cust3; ALTER ROLE Customer ADD MEMBER Cust3;
GO

CREATE LOGIN Warehousestaff WITH PASSWORD = 'Ware123!';
USE OnlineStore;
CREATE USER Warehousestaff FOR LOGIN Warehousestaff;
GRANT UPDATE ON Orders TO Warehousestaff;
GRANT UPDATE ON Products TO Warehousestaff;
CREATE LOGIN Ware1 WITH PASSWORD = 'Ware123!'; CREATE USER Ware1 FOR LOGIN Ware1; ALTER ROLE Warehousestaff ADD MEMBER Ware1;
CREATE LOGIN Ware2 WITH PASSWORD = 'Ware123!'; CREATE USER Ware2 FOR LOGIN Ware2; ALTER ROLE Warehousestaff ADD MEMBER Ware2;
CREATE LOGIN Ware3 WITH PASSWORD = 'Ware123!'; CREATE USER Ware3 FOR LOGIN Ware3; ALTER ROLE Warehousestaff ADD MEMBER Ware3;
GO

CREATE LOGIN Supportmanager WITH PASSWORD = 'Supp123!';
USE OnlineStore;
CREATE USER Supportmanager FOR LOGIN Supportmanager;
GRANT UPDATE ON Orders TO Supportmanager;
GRANT INSERT ON OrderComments TO Supportmanager;
DENY DELETE ON Products TO Supportmanager;
DENY UPDATE ON Products TO Supportmanager;
CREATE LOGIN Supp1 WITH PASSWORD = 'Supp123!'; CREATE USER Supp1 FOR LOGIN Supp1; ALTER ROLE Supportmanager ADD MEMBER Supp1;
CREATE LOGIN Supp2 WITH PASSWORD = 'Supp123!'; CREATE USER Supp2 FOR LOGIN Supp2; ALTER ROLE Supportmanager ADD MEMBER Supp2;
CREATE LOGIN Supp3 WITH PASSWORD = 'Supp123!'; CREATE USER Supp3 FOR LOGIN Supp3; ALTER ROLE Supportmanager ADD MEMBER Supp3;
GO

DENY SELECT, INSERT, UPDATE, DELETE ON Audit TO Customer;
DENY SELECT, INSERT, UPDATE, DELETE ON Audit TO Warehousestaff;
DENY SELECT, INSERT, UPDATE, DELETE ON Audit TO Supportmanager;
GO
