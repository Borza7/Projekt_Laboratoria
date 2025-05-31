#include "Table.h"

// Konstruktor domyœlny
Table::Table() : selected(false), number(0), paymentType(""), orderAmount(0.0f), orderStatus(""), visible(true) {
    shape.setOutlineThickness(1.0f);
    shape.setOutlineColor(sf::Color::Black);
}

// Settery
void Table::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

void Table::setSize(float size) {
    shape.setSize(sf::Vector2f(size, size));
}

void Table::setSelected(bool isSelected) {
    selected = isSelected;
    shape.setFillColor(selected ? sf::Color(100, 100, 100) : sf::Color::White);
}

void Table::setNumber(int num) {
    number = num;
}

void Table::setPaymentType(const std::string& type) {
    paymentType = type;
}

void Table::setOrderAmount(float amount) {
    orderAmount = amount;
}

void Table::setOrderStatus(const std::string& status) {
    orderStatus = status;
}

void Table::setVisible(bool isVisible) {
    visible = isVisible;
}

// Gettery
bool Table::isSelected() const {
    return selected;
}

int Table::getNumber() const {
    return number;
}

std::string Table::getPaymentType() const {
    return paymentType;
}

float Table::getOrderAmount() const {
    return orderAmount;
}

std::string Table::getOrderStatus() const {
    return orderStatus;
}

bool Table::isVisible() const {
    return visible;
}

sf::Vector2f Table::getPosition() const {
    return shape.getPosition();
}

sf::FloatRect Table::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

// Funkcja rysuj¹ca stolik
void Table::draw(sf::RenderWindow& window, const sf::Font& font) const {
    window.draw(shape);
    if (number > 0) {
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(number));
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::Red);
        text.setPosition(shape.getPosition().x + shape.getSize().x / 2 - 5, shape.getPosition().y + shape.getSize().y / 2 - 10);
        window.draw(text);
    }
}

