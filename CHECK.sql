USE OnlineStore;
GO

SELECT 'Categories' AS TableName, COUNT(*) AS RowCount FROM Categories
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

INSERT INTO Orders (ID_Client, OrderStatus) VALUES (1, 'Новый');
GO

SELECT * FROM Audit;
GO

UPDATE Orders SET OrderStatus = 'Собран' WHERE ID_Order = (SELECT MAX(ID_Order) FROM Orders);
GO

SELECT * FROM Audit;
GO
