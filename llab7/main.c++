#include "func.h"
#include <chrono>
#include <thread>

int main() {
    std::cout << "=== DEMONSTRATION ===\n";
    std::vector<std::shared_ptr<NPC>> demo_npcs;
    demo_npcs.push_back(std::make_shared<Knight>(50, 50, "Arthur"));
    demo_npcs.push_back(std::make_shared<Dragon>(55, 55, "Smaug"));
    demo_npcs.push_back(std::make_shared<Elf>(20, 20, "Legolas")); 
    std::cout << "\nInitial NPCs:\n";
    NPCFactory::printAll(demo_npcs);
    std::cout << "\nMap view:\n";
    NPCFactory::printMap(demo_npcs);
    NPCFactory::saveToFile("dungeon.txt", demo_npcs);
    auto loaded = NPCFactory::loadFromFile("dungeon.txt");
    std::cout << "\nLoaded from file:\n";
    NPCFactory::printAll(loaded);
    std::cout << "\n\n=== LAB 7: MULTI-THREADED GAME ===\n";
    std::cout << "Starting game with 50 NPCs for " << GAME_DURATION << " seconds...\n";
    std::cout << "Press Ctrl+C to stop early.\n\n";
    try {
        Game game;
        game.start();
        std::this_thread::sleep_for(std::chrono::seconds(GAME_DURATION + 1));
        std::cout << "\n=== TIME'S UP - STOPPING GAME ===\n";
        game.stop();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\n=== GAME COMPLETE ===\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}