-- Создание таблицы Users
CREATE TABLE IF NOT EXISTS Users (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Name TEXT NOT NULL UNIQUE,
    Password TEXT NOT NULL
);

-- Создание таблицы Orders
CREATE TABLE IF NOT EXISTS Orders (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Customer_ID INTEGER,
    Product_Name TEXT NOT NULL,
    Order_Date TEXT NOT NULL,
    Delivery_Date TEXT,
    Status TEXT,
    FOREIGN KEY (Customer_ID) REFERENCES Users(ID)
);

-- Создание таблицы Customer_Orders
CREATE TABLE IF NOT EXISTS Customer_Orders (
    ID INTEGER PRIMARY KEY AUTOINCREMENT,
    Customer_ID INTEGER,
    Order_ID INTEGER,
    FOREIGN KEY (Customer_ID) REFERENCES Users(ID),
    FOREIGN KEY (Order_ID) REFERENCES Orders(ID)
);
