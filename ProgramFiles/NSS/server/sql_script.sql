CREATE TABLE IF NOT EXISTS Users (
    UserID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name TEXT NOT NULL UNIQUE,
    Password TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS Orders (
    OrderID INTEGER PRIMARY KEY AUTOINCREMENT,
    UserID INTEGER NOT NULL,
    ProductName TEXT NOT NULL,
    OrderDate TEXT NOT NULL,
    DeliveryDate TEXT,
    Status TEXT NOT NULL,
    PickupPointID INTEGER NOT NULL,
    FOREIGN KEY (UserID) REFERENCES Users(UserID) // Задача 1 реализовать функцию просмотра заказов конкретного пользователя
);

CREATE TABLE IF NOT EXISTS PickupPoints (
    PickupPointID INTEGER PRIMARY KEY AUTOINCREMENT,
    Address TEXT NOT NULL                            
);
