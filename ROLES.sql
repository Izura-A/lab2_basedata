USE master;
GO
IF EXISTS (SELECT * FROM sys.server_principals WHERE name = 'Admin') DROP LOGIN Admin;
IF EXISTS (SELECT * FROM sys.server_principals WHERE name = 'Customer') DROP LOGIN Customer;
IF EXISTS (SELECT * FROM sys.server_principals WHERE name = 'Warehousestaff') DROP LOGIN Warehousestaff;
IF EXISTS (SELECT * FROM sys.server_principals WHERE name = 'Supportmanager') DROP LOGIN Supportmanager;
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
GO

CREATE LOGIN Warehousestaff WITH PASSWORD = 'Ware123!';
USE OnlineStore;
CREATE USER Warehousestaff FOR LOGIN Warehousestaff;
GRANT UPDATE ON Orders TO Warehousestaff;
GRANT UPDATE ON Products TO Warehousestaff;
GO

CREATE LOGIN Supportmanager WITH PASSWORD = 'Supp123!';
USE OnlineStore;
CREATE USER Supportmanager FOR LOGIN Supportmanager;
GRANT UPDATE ON Orders TO Supportmanager;
GRANT INSERT ON OrderComments TO Supportmanager;
DENY DELETE ON Products TO Supportmanager;
DENY UPDATE ON Products TO Supportmanager;
GO