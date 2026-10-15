USE OnlineStore;
GO

CREATE TABLE Categories (
    ID_Category INT PRIMARY KEY IDENTITY(1,1),
    CategoryName NVARCHAR(100) NOT NULL
) ON Fg_goods;

CREATE TABLE Products (
    ID_Product INT PRIMARY KEY IDENTITY(1,1),
    ID_Category INT FOREIGN KEY REFERENCES Categories(ID_Category),
    ProductName NVARCHAR(200) NOT NULL,
    Price MONEY NOT NULL CHECK (Price > 0),
    StockQuantity INT NOT NULL DEFAULT 0 CHECK (StockQuantity >= 0)
) ON Fg_goods;

CREATE TABLE Clients (
    ID_Client INT PRIMARY KEY IDENTITY(1,1),
    FullName NVARCHAR(150) NOT NULL,
    Email NVARCHAR(100),         
    Phone NVARCHAR(20)
) ON Fg_orders;

CREATE TABLE Orders (
    ID_Order INT PRIMARY KEY IDENTITY(1,1),
    ID_Client INT FOREIGN KEY REFERENCES Clients(ID_Client),
    OrderDate DATE DEFAULT GETDATE(),
    OrderStatus NVARCHAR(30) DEFAULT 'Новый' 
        CHECK (OrderStatus IN ('Новый', 'Собран', 'Передан в доставку', 'Отменен', 'Доставлен'))
) ON Fg_orders;

CREATE TABLE OrderItems (
    ID_Order INT FOREIGN KEY REFERENCES Orders(ID_Order),
    ID_Product INT FOREIGN KEY REFERENCES Products(ID_Product),
    Quantity INT NOT NULL CHECK (Quantity > 0),
    PRIMARY KEY (ID_Order, ID_Product)
) ON Fg_orders;

CREATE TABLE OrderComments (
    ID_Comment INT PRIMARY KEY IDENTITY(1,1),
    ID_Order INT FOREIGN KEY REFERENCES Orders(ID_Order),
    CommentText NVARCHAR(500),
    CommentDate DATETIME DEFAULT GETDATE()
) ON Fg_orders;
GO