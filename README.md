USE OnlineStore;
GO
EXECUTE AS USER = 'Customer';
GO

SELECT * FROM Orders;
INSERT INTO Orders (ID_Client, OrderStatus) VALUES (1, 'Новый');

REVERT;
GO 



USE OnlineStore;
GO
EXECUTE AS USER = 'Warehousestaff';
GO

UPDATE Orders SET OrderStatus = 'Собран' WHERE ID_Order = 1;
UPDATE Products SET StockQuantity = 100 WHERE ID_Product = 1;

REVERT;
GO




USE OnlineStore;
GO
EXECUTE AS USER = 'Supportmanager';
GO

UPDATE Orders SET OrderStatus = 'Отменен' WHERE ID_Order = 1;
INSERT INTO OrderComments (ID_Order, CommentText) VALUES (1, 'Клиент попросил отменить');

REVERT;
GO
