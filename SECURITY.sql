USE OnlineStore;
GO

CREATE VIEW View_ClientOrders AS
SELECT o.ID_Order, o.OrderDate, o.OrderStatus, c.FullName, c.Phone
FROM Orders o
JOIN Clients c ON o.ID_Client = c.ID_Client;
GO

CREATE PROCEDURE sp_UpdateOrderStatus
    @OrderID INT,
    @NewStatus NVARCHAR(30)
AS
BEGIN
    IF @NewStatus IN ('Собран', 'Передан в доставку')
    BEGIN
        UPDATE Orders SET OrderStatus = @NewStatus WHERE ID_Order = @OrderID;
        PRINT 'Статус заказа обновлён.';
    END
    ELSE
        PRINT 'Ошибка: недопустимый статус для склада.';
END;
GO

CREATE TRIGGER trg_Orders_Insert
ON Orders
AFTER INSERT
AS
BEGIN
    INSERT INTO Audit (TableName, ActionType, UserName, Details)
    SELECT 
        'Orders', 
        'INSERT', 
        SUSER_SNAME(), 
        'Добавлен заказ ID: ' + CAST(i.ID_Order AS NVARCHAR(10)) + ', Статус: ' + i.OrderStatus
    FROM inserted i;
END;
GO

CREATE TRIGGER trg_Orders_Update
ON Orders
AFTER UPDATE
AS
BEGIN
    INSERT INTO Audit (TableName, ActionType, UserName, Details)
    SELECT 
        'Orders', 
        'UPDATE', 
        SUSER_SNAME(), 
        'Изменен статус заказа ID: ' + CAST(i.ID_Order AS NVARCHAR(10)) + ' с "' + d.OrderStatus + '" на "' + i.OrderStatus + '"'
    FROM inserted i
    JOIN deleted d ON i.ID_Order = d.ID_Order;
END;
GO

CREATE TRIGGER trg_Products_Delete
ON Products
AFTER DELETE
AS
BEGIN
    INSERT INTO Audit (TableName, ActionType, UserName, Details)
    SELECT 
        'Products', 
        'DELETE', 
        SUSER_SNAME(), 
        'Удален товар: ' + d.ProductName + ', Цена была: ' + CAST(d.Price AS NVARCHAR(20))
    FROM deleted d;
END;
GO
