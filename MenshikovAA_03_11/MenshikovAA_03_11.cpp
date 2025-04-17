#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits> // для очистки буфера ввода

class Clock {
public:
    std::string model;
    std::string brand;
    std::string dialType;
    double price;

    Clock(const std::string& model, const std::string& brand, const std::string& dialType, double price)
        : model(model), brand(brand), dialType(dialType), price(price) {}

    void display() const {
        std::cout << "Модель: " << model << ", Бренд: " << brand
            << ", Тип набора: " << dialType << ", Цена: " << price << std::endl;
    }
};

// Функция для записи вектора часов в файл (добавление в конец)
void writeToFile(const std::vector<Clock>& clocks, const std::string& filename, bool append = true) {
    std::ofstream outFile(filename, append ? std::ios::app : std::ios::out);
    if (!outFile) {
        std::cerr << "Невозможно открыть файл для записи." << std::endl;
        return;
    }
    for (const auto& clock : clocks) {
        outFile << clock.model << "," << clock.brand << "," << clock.dialType << "," << clock.price << "\n";
    }
    outFile.close();
}

// Функция для чтения вектора часов из файла
std::vector<Clock> readFromFile(const std::string& filename) {
    std::vector<Clock> clocks;
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Невозможно открыть файл для чтения. Будет создан новый файл." << std::endl;
        return clocks;
    }
    std::string line;
    while (std::getline(inFile, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);

        if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
            std::string model = line.substr(0, pos1);
            std::string brand = line.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string dialType = line.substr(pos2 + 1, pos3 - pos2 - 1);
            double price = std::stod(line.substr(pos3 + 1));

            clocks.emplace_back(model, brand, dialType, price);
        }
    }
    inFile.close();
    return clocks;
}

// Функция для добавления часов через консоль
void addClock(std::vector<Clock>& clocks) {
    std::string model, brand, dialType;
    double price;

    std::cout << "Введите модель: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, model);

    std::cout << "Введите бренд: ";
    std::getline(std::cin, brand);

    std::cout << "Введите тип набора: ";
    std::getline(std::cin, dialType);

    std::cout << "Введите цену: ";
    while (!(std::cin >> price)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод. Пожалуйста, введите число: ";
    }

    clocks.emplace_back(model, brand, dialType, price);
}

// Пузырьковая сортировка
void bubbleSort(std::vector<Clock>& clocks, bool ascending = true) {
    size_t n = clocks.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if ((ascending && clocks[j].price > clocks[j + 1].price) ||
                (!ascending && clocks[j].price < clocks[j + 1].price)) {
                std::swap(clocks[j], clocks[j + 1]);
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "russian");
    std::vector<Clock> clocks;
    std::string filename = "clocks.txt";

    // Чтение данных из файла
    clocks = readFromFile(filename);

    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Добавить часы\n2. Сортировать часы\n3. Отобразить часы\n4. Выход\n";
        std::cout << "Введите ваш выбор: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            addClock(clocks);
            // Записываем только последние добавленные часы (добавляем в конец файла)
            if (!clocks.empty()) {
                std::vector<Clock> lastClock = { clocks.back() };
                writeToFile(lastClock, filename);
            }
            std::cout << "Часы успешно добавлены!\n";
            break;
        }
        case 2: {
            int sortChoice;
            std::cout << "Сортировать по:\n1. По возрастанию\n2. По убыванию\n";
            std::cout << "Введите ваш выбор: ";
            std::cin >> sortChoice;

            if (sortChoice == 1 || sortChoice == 2) {
                bubbleSort(clocks, sortChoice == 1);
                // Перезаписываем весь файл после сортировки
                writeToFile(clocks, filename, false);
                std::cout << "Часы успешно отсортированы!\n";
            }
            else {
                std::cout << "Неверный выбор сортировки.\n";
            }
            break;
        }
        case 3:
            if (clocks.empty()) {
                std::cout << "Список часов пуст.\n";
            }
            else {
                std::cout << "\nСписок часов:\n";
                for (const auto& clock : clocks) {
                    clock.display();
                }
            }
            break;
        case 4:
            std::cout << "Выход...\n";
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (choice != 4);

    return 0;
}