CREATE DATABASE OnlineStore
ON PRIMARY (
    NAME = 'OnlineStore_data',
    FILENAME = 'C:\SQLData\OnlineStore.mdf',
    SIZE = 10MB,
    MAXSIZE = 90MB, 
    FILEGROWTH = 5MB
),
FILEGROUP Fg_goods (    
    NAME = 'OnlineStore_Goods',
    FILENAME = 'C:\SQLData\OnlineStore_Goods.ndf',
    SIZE = 5MB,
    MAXSIZE = 50MB,
    FILEGROWTH = 5MB
),
FILEGROUP Fg_orders ( 
    NAME = 'OnlineStore_Orders',
    FILENAME = 'C:\SQLData\OnlineStore_Orders.ndf',
    SIZE = 5MB,
    MAXSIZE = 50MB,
    FILEGROWTH = 5MB
)
LOG ON (
    NAME = 'OnlineStore_Log',
    FILENAME = 'C:\SQLData\OnlineStore_Log.ldf',
    SIZE = 5MB,
    MAXSIZE = 50MB,
    FILEGROWTH = 5MB
);
GO