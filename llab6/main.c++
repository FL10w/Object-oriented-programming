#include "func.h"

int main() {
    std::vector<std::shared_ptr<NPC>> npcs;

    npcs.push_back(std::make_shared<Knight>(100, 100, "Arthur"));
    npcs.push_back(std::make_shared<Dragon>(105, 105, "Smaug"));
    npcs.push_back(std::make_shared<Elf>(200, 200, "Legolas"));

    std::cout << "Initial state:\n";
    NPCFactory::printAll(npcs);

    NPCFactory::saveToFile("dungeon.txt", npcs);
    auto loaded = NPCFactory::loadFromFile("dungeon.txt");

    std::cout << "\nLoaded from file:\n";
    NPCFactory::printAll(loaded);

    std::cout << "\n--- BATTLE MODE (range = 10) ---\n";
    NPCFactory::battle(loaded, 10.0);

    std::cout << "\nAfter battle:\n";
    NPCFactory::printAll(loaded);

    return 0;
}