#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <direct.h>
#include <cstdlib>
#include <windows.h>
#include "Table.h"

// Funkcja otwieraj¹ca Notatnik w ma³ym formacie
void openNotepadWithSize(const std::string& filePath) {
    // Konwersja std::string na szeroki ci¹g znaków (wchar_t*)
    std::wstring wideFilePath = std::wstring(filePath.begin(), filePath.end());

    // Otwórz plik w Notatniku (u¿ycie ShellExecuteW dla szerokich ci¹gów znaków)
    ShellExecuteW(0, L"open", L"notepad", wideFilePath.c_str(), 0, SW_SHOWNORMAL);

    // Odczekaj chwilê, aby Notatnik zd¹¿y³ siê uruchomiæ
    Sleep(500);

    // ZnajdŸ okno Notatnika
    HWND hwnd = FindWindowW(L"Notepad", NULL);
    if (hwnd) {
        // Ustaw nowe wymiary okna: (pozycja_x, pozycja_y, szerokosc, wysokosc)
        MoveWindow(hwnd, 100, 100, 300, 300, TRUE);
    }
}

void generateReceiptTXT(const Table& table) {
    if (table.getNumber() == 0) return; // Pomijamy niewa¿ne stoliki

    _mkdir("Paragony"); // Tworzenie folderu, jeœli nie istnieje

    // Œcie¿ka do pliku paragonu
    std::string filePath = "Paragony/receipt_table_" + std::to_string(table.getNumber()) + ".txt";

    std::ofstream receipt(filePath);
    if (!receipt.is_open()) {
        std::cerr << "Failed to create receipt file!\n";
        return;
    }

    // Pobranie aktualnej daty i godziny
    std::time_t now = std::time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);

    char buffer[80];
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &timeInfo);

    // Zawartoœæ paragonu
    receipt << "=============================\n";
    receipt << "          RESTAURACJA        \n";
    receipt << "=============================\n";
    receipt << "Data: " << buffer << "\n";
    receipt << "Stolik nr: " << table.getNumber() << "\n";
    receipt << "Kwota zamowienia: " << std::fixed << std::setprecision(2) << table.getOrderAmount() << " PLN\n";
    receipt << "Typ platnosci: " << table.getPaymentType() << "\n";
    receipt << "Status zamowienia: " << table.getOrderStatus() << "\n";
    receipt << "=============================\n";
    receipt << "Dziekujemy za wizyte!\n";
    receipt << "=============================\n";

    receipt.close();

    // Otwórz Notatnik w ma³ym formacie
    openNotepadWithSize(filePath);

    std::cout << "Receipt for table " << table.getNumber() << " has been generated and opened in a small Notepad window.\n";
}

sf::Text createText(const std::string& str, const sf::Font& font, unsigned int size, sf::Color color, sf::Vector2f position) {
    sf::Text text;
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position);
    return text;
}//tworzenie tekstu

int main() {//brak konsoli na release int APIENTRY wWinMain
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "Restauracja");
    window.setFramerateLimit(60);

    const int rows = 15;
    const int cols = 20;
    const float padding = 5.0f;
    const float tableSize = (window.getSize().x - (cols + 1) * padding) / cols;
    const float startX = padding;
    const float startY = (window.getSize().y - rows * (tableSize + padding)) / 2;
    //okno i inicjalizacja tabelki

    std::vector<Table> tables(rows * cols);
    //inicjalizacja teraz 300 stolow

    sf::Font font;
    if (!font.loadFromFile("D:\\projekt-cpp\\Restauracja\\x64\\Debug\\arial.ttf")) {
        std::cerr << "Failed to load font!\n";
        return -1;
    }

    std::vector<std::string> paymentOptions = { "Cash", "Card" };
    std::vector<std::string> statusOptions = { "Pending", "In Progress", "Clean Up" };

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int index = i * cols + j;
            tables[index].setSize(tableSize);
            tables[index].setPosition(startX + j * (tableSize + padding), startY + i * (tableSize + padding));
        }//tabelka
    }

    int selectedTableIndex = -1;
    bool enterPressed = false;
    int tableCounter = 1;
    std::string inputBuffer;
    bool editingOrderAmount = false;
    //enum kolor nr na status stolika 
    //obiad za cene

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    for (int i = 0; i < tables.size(); ++i) {
                        if (tables[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            if (!enterPressed) {
                                tables[i].setSelected(!tables[i].isSelected());
                            }
                            selectedTableIndex = i;
                            break;
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && !enterPressed) {
                    enterPressed = true;
                    tableCounter = 1;
                    for (auto& table : tables) {
                        if (table.isSelected()) {
                            table.setNumber(tableCounter++);
                        }
                        else {
                            table.setVisible(false); // Ukryj niewybrane sto³y
                        }
                    }
                }
                if (selectedTableIndex != -1 && enterPressed) {
                    if (event.key.code == sf::Keyboard::Up) {
                        int currentIndex = std::find(statusOptions.begin(), statusOptions.end(), tables[selectedTableIndex].getOrderStatus()) - statusOptions.begin();
                        tables[selectedTableIndex].setOrderStatus(statusOptions[(currentIndex + 1) % statusOptions.size()]);
                    }
                    if (event.key.code == sf::Keyboard::Down) {
                        int currentIndex = std::find(statusOptions.begin(), statusOptions.end(), tables[selectedTableIndex].getOrderStatus()) - statusOptions.begin();
                        tables[selectedTableIndex].setOrderStatus(statusOptions[(currentIndex - 1 + statusOptions.size()) % statusOptions.size()]);
                    }
                    if (event.key.code == sf::Keyboard::Right) {
                        int currentIndex = std::find(paymentOptions.begin(), paymentOptions.end(), tables[selectedTableIndex].getPaymentType()) - paymentOptions.begin();
                        tables[selectedTableIndex].setPaymentType(paymentOptions[(currentIndex + 1) % paymentOptions.size()]);
                    }
                    if (event.key.code == sf::Keyboard::Left) {
                        int currentIndex = std::find(paymentOptions.begin(), paymentOptions.end(), tables[selectedTableIndex].getPaymentType()) - paymentOptions.begin();
                        tables[selectedTableIndex].setPaymentType(paymentOptions[(currentIndex - 1 + paymentOptions.size()) % paymentOptions.size()]);
                    }
                    if (event.key.code == sf::Keyboard::O) {
                        editingOrderAmount = true;
                        inputBuffer.clear();
                    }
                    if (event.key.code == sf::Keyboard::C) {
                        generateReceiptTXT(tables[selectedTableIndex]);
                        tables[selectedTableIndex].setPaymentType("");
                        tables[selectedTableIndex].setOrderAmount(0.0f);
                        tables[selectedTableIndex].setOrderStatus("");
                    }
                }
            }

            if (editingOrderAmount && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8 && !inputBuffer.empty()) { // Backspace
                    inputBuffer.pop_back();
                }
                else if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == '.') {
                    if (event.text.unicode == '.' && inputBuffer.find('.') != std::string::npos) {
                        continue; // kropki
                    }
                    inputBuffer += static_cast<char>(event.text.unicode);
                }
            }

            if (editingOrderAmount && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                if (selectedTableIndex != -1 && !inputBuffer.empty()) {
                    tables[selectedTableIndex].setOrderAmount(std::stof(inputBuffer));
                    editingOrderAmount = false;
                    inputBuffer.clear();
                }
            }
        }
        //edycja kwoty

        window.clear(sf::Color::Black);

        for (const auto& table : tables) {
            if (table.isVisible()) {
                table.draw(window, font);
            }
        }
        //cyfry na stolik

        if (selectedTableIndex != -1) {
            Table& selectedTable = tables[selectedTableIndex];
            sf::RectangleShape uiPanel(sf::Vector2f(400, 300));
            uiPanel.setFillColor(sf::Color(200, 200, 200));
            uiPanel.setPosition(780, 100);
            window.draw(uiPanel);

            sf::Text uiTitle = createText("Table " + std::to_string(selectedTable.getNumber()) + " Details", font, 20, sf::Color::Black, sf::Vector2f(800, 110));
            window.draw(uiTitle);

            sf::Text uiPaymentType = createText("Payment Type: " + selectedTable.getPaymentType(), font, 16, sf::Color::Black, sf::Vector2f(800, 150));
            window.draw(uiPaymentType);

            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << selectedTable.getOrderAmount();
            sf::Text uiOrderAmount = createText("Order Amount: " + oss.str() + " PLN", font, 16, sf::Color::Black, sf::Vector2f(800, 190));
            window.draw(uiOrderAmount);

            sf::Text uiOrderStatus = createText("Order Status: " + selectedTable.getOrderStatus(), font, 16, sf::Color::Black, sf::Vector2f(800, 230));
            window.draw(uiOrderStatus);

            sf::Text instructions = createText("Use arrow keys: Up/Down for status, Left/Right for payment", font, 14, sf::Color::Red, sf::Vector2f(800, 270));
            window.draw(instructions);

            if (editingOrderAmount) {
                sf::Text inputText = createText("Enter Order Amount: " + inputBuffer, font, 16, sf::Color::Blue, sf::Vector2f(800, 310));
                window.draw(inputText);
            }
            else {
                sf::Text inputHint = createText("Press 'O' to enter order amount", font, 14, sf::Color::Red, sf::Vector2f(800, 310));
                window.draw(inputHint);
            }

            sf::Text clearHint = createText("Press 'C' to clear table data", font, 14, sf::Color::Red, sf::Vector2f(800, 340));
            window.draw(clearHint);
        }

        window.display();
    }

    return 0;
}
