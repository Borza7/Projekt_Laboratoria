// Table.h - Plik nag³ówkowy klasy Table
#ifndef TABLE_H
#define TABLE_H

#include <SFML/Graphics.hpp>
#include <string>

class Table {
private:
    sf::RectangleShape shape;
    bool selected;
    int number; // stol
    std::string paymentType;
    float orderAmount;
    std::string orderStatus;
    bool visible; // widocznosc

public:
    // Konstruktor
    Table();

    // Settery i gettery
    void setPosition(float x, float y);
    void setSize(float size);
    void setSelected(bool isSelected);
    void setNumber(int num);
    void setPaymentType(const std::string& type);
    void setOrderAmount(float amount);
    void setOrderStatus(const std::string& status);
    void setVisible(bool isVisible);

    bool isSelected() const;
    int getNumber() const;
    std::string getPaymentType() const;
    float getOrderAmount() const;
    std::string getOrderStatus() const;
    bool isVisible() const;

    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;

    void draw(sf::RenderWindow& window, const sf::Font& font) const;
};

#endif // TABLE_H
#pragma once
