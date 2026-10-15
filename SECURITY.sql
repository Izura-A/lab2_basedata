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