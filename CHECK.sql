USE OnlineStore;
GO
SELECT * FROM Products;
SELECT * FROM Clients;
SELECT * FROM Orders;
GO


SELECT 'Categories' AS Таблица, COUNT(*) AS Строк FROM Categories
UNION ALL
SELECT 'Products', COUNT(*) FROM Products
UNION ALL
SELECT 'Clients', COUNT(*) FROM Clients
UNION ALL
SELECT 'Orders', COUNT(*) FROM Orders
UNION ALL
SELECT 'OrderItems', COUNT(*) FROM OrderItems
UNION ALL
SELECT 'OrderComments', COUNT(*) FROM OrderComments;
GO