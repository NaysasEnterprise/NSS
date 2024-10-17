#include <string>

// Структура описания заказа
class Order {
public:
    int order_id;
    std::string product_name;
    std::string client_name;
    std::string order_date;
    std::string status;
    int quantity;
    bool is_ready;

    // Конструктор заказа
    Order(int order_id, const std::string& product, const std::string& client,
        const std::string& date, const std::string& order_status, int qty);

    // Функция для изменения статуса заказа
    void update_status(const std::string& new_status);

    // Функция для проверки готовности заказа
    bool is_order_ready() const;

    // Функция для проверки количества товаров в заказе
    int get_quantity() const;

    // Получить данные о заказе в формате строки
    std::string get_order_info() const;
};
//пока всё еще не точно, функции точно будут убраны или заменены на какие то другие, либо же добавлены