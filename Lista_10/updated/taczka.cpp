#include <iostream>
#include <fstream>
#include <sstream>

struct Stone {
    std::string type;
    int weight;
    int quantity;
};

struct Wheelbarrow {
    int number;
    Stone* stones;
    int stoneCount;
};

int main() {
    std::ifstream inputFile("dane.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku Dane1.txt." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        
        int minute;
        iss >> minute;

        Wheelbarrow wheelbarrow;
        wheelbarrow.number = minute;
        wheelbarrow.stones = nullptr;
        wheelbarrow.stoneCount = 0;

        while (iss) {
            int number;
            std::string type;
            int weight;
            int quantity;

            iss >> number >> type >> weight >> quantity;

            if (!iss) {
                break;
            }

            Stone* newStones = new Stone[wheelbarrow.stoneCount + 1];
            std::copy(wheelbarrow.stones, wheelbarrow.stones + wheelbarrow.stoneCount, newStones);

            Stone stone;
            stone.type = type;
            stone.weight = weight;
            stone.quantity = quantity;

            newStones[wheelbarrow.stoneCount] = stone;

            delete[] wheelbarrow.stones;

            wheelbarrow.stones = newStones;
            wheelbarrow.stoneCount++;
        }

        // Wyświetlanie danych na ekranie
        std::cout << "Minuta: " << minute << std::endl;
        for (int i = 0; i < wheelbarrow.stoneCount; ++i) {
            const Stone& stone = wheelbarrow.stones[i];
            std::cout << "Taczka nr " << wheelbarrow.number << ": "
                      << stone.quantity << " " << stone.type << "(s) o wadze "
                      << stone.weight << " - Potrzeba " << (stone.weight * stone.quantity)
                      << " minut, aby przeładować tę taczkę." << std::endl;
        }

        delete[] wheelbarrow.stones;
        std::cout << std::endl;
    }

    inputFile.close();

    return 0;
}
