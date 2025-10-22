#include "figure.h"
#include <iostream>
#include <memory>
#include <limits>


int main() {
    FigureArray figures;
    int choice;
    
do {
    showMenu();
    std::cin >> choice;
    
    switch (choice) {
        case 1: {
            auto pentagon = std::make_shared<Pentagon>();
            std::cin.ignore(); 
            std::cin >> *pentagon;
            
            if (std::cin.fail()) {
                std::cout << "Ошибка ввода. Фигура не добавлена" << std::endl;
                std::cin.clear();
            } else {
                figures.addFigure(pentagon);
                std::cout << "Пятиугольник успешно добавлен" << std::endl;
            }
            break;
        }
            case 2: {
                auto hexagon = std::make_shared<Hexagon>();
                std::cin.ignore();
                std::cin >> *hexagon;
                if (std::cin.fail()) {
                    std::cout << "Ошибка ввода. Фигура не добавлена" << std::endl;
                    std::cin.clear();
                } else {
                    figures.addFigure(hexagon);
                    std::cout << "Шестиугольник успешно добавлен" << std::endl;
                }
                break;
            }
            case 3: {
                auto octagon = std::make_shared<Octagon>();
                std::cin.ignore();
                std::cin >> *octagon;
                if (std::cin.fail()) {
                    std::cout << "Ошибка ввода. Фигура не добавлена" << std::endl;
                    std::cin.clear(); 
                } else {
                    figures.addFigure(octagon);
                    std::cout << "Восьмиугольник успешно добавлен" << std::endl;
                }
                break;
            }
            case 4: {
                if (figures.size() == 0) {
                    std::cout << "В массиве нет фигур." << std::endl;
                } else {
                    std::cout << "\n~~~ Все фигуры ~~~" << std::endl;
                    figures.printAll();
                }
                break;
            }
            case 5: {
                double total = figures.totalArea();
                std::cout << "Общая площадь всех фигур: " << total << std::endl;
                break;
            }
            case 6: {
                if (figures.size() == 0) {
                    std::cout << "Нет фигур для удаления" << std::endl;
                } else {
                    std::cout << "Введите индекс для удаления (0-" << figures.size()-1 << "): ";
                    int index;
                    std::cin >> index;
                    if (index >= 0 && index < static_cast<int>(figures.size())) {
                        figures.removeFigure(index);
                        std::cout << "Фигура удалена" << std::endl;
                    } else {
                        std::cout << "Ошибка: неверный индекс" << std::endl;
                    }
                }
                break;
            }
            case 0: {
                break;
            }
        }
        
        
    } while (choice != 0);
    
    return 0;
}