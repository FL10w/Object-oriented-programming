#include "func.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <thread>
#include <map>

class NPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove("test_dungeon.txt");
        std::filesystem::remove("log.txt");
        std::filesystem::remove("survivors.txt");
        std::filesystem::remove("thread_test_log.txt");
    }
    void TearDown() override {
        std::filesystem::remove("test_dungeon.txt");
        std::filesystem::remove("log.txt");
        std::filesystem::remove("survivors.txt");
        std::filesystem::remove("thread_test_log.txt");
    }
};
TEST_F(NPCTest, SaveLoad) {
    auto k = std::make_shared<Knight>(10, 20, "Galahad");
    auto d = std::make_shared<Dragon>(30, 40, "Toothless");
    auto e = std::make_shared<Elf>(50, 60, "Arwen");

    std::vector<std::shared_ptr<NPC>> npcs = {k, d, e};
    NPCFactory::saveToFile("test_dungeon.txt", npcs);
    auto loaded = NPCFactory::loadFromFile("test_dungeon.txt");

    ASSERT_EQ(loaded.size(), 3);
    EXPECT_EQ(loaded[0]->getType(), "Knight");
    EXPECT_EQ(loaded[1]->getType(), "Dragon");
    EXPECT_EQ(loaded[2]->getType(), "Elf");
}
TEST_F(NPCTest, MovementWithinBounds) {
    auto knight = std::make_shared<Knight>(0, 0, "TestKnight");
    
    EXPECT_EQ(knight->getX(), 0);
    EXPECT_EQ(knight->getY(), 0);
    
    for (int i = 0; i < 20; ++i) {
        knight->moveRandom();
        
        EXPECT_GE(knight->getX(), 0);
        EXPECT_LT(knight->getX(), MAP_WIDTH);
        EXPECT_GE(knight->getY(), 0);
        EXPECT_LT(knight->getY(), MAP_HEIGHT);
    }
}
TEST_F(NPCTest, DeadNPCDoesNotMove) {
    auto knight = std::make_shared<Knight>(50, 50, "DeadKnight");
    int initialX = knight->getX();
    int initialY = knight->getY();
    
    knight->kill();
    EXPECT_FALSE(knight->isAlive());
    
    knight->moveRandom();
    
    EXPECT_EQ(knight->getX(), initialX);
    EXPECT_EQ(knight->getY(), initialY);
}
TEST_F(NPCTest, DistanceCalculation) {
    auto npc1 = std::make_shared<Knight>(0, 0, "N1");
    auto npc2 = std::make_shared<Knight>(3, 4, "N2");
    
    EXPECT_DOUBLE_EQ(npc1->distanceTo(npc2), 5.0);
    EXPECT_DOUBLE_EQ(npc2->distanceTo(npc1), 5.0);
}
TEST_F(NPCTest, CreateRandomNPCs) {
    auto npcs = NPCFactory::createRandomNPCs(10);
    
    EXPECT_EQ(npcs.size(), 10);
    
    for (const auto& npc : npcs) {
        EXPECT_GE(npc->getX(), 0);
        EXPECT_LT(npc->getX(), MAP_WIDTH);
        EXPECT_GE(npc->getY(), 0);
        EXPECT_LT(npc->getY(), MAP_HEIGHT);
        EXPECT_TRUE(npc->isAlive());
    }
}
TEST_F(NPCTest, NPCTypeSpecificDistances) {
    auto knight = std::make_shared<Knight>(0, 0, "K");
    auto elf = std::make_shared<Elf>(0, 0, "E");
    auto dragon = std::make_shared<Dragon>(0, 0, "D");
    
    EXPECT_EQ(knight->getMoveDistance(), 30);
    EXPECT_EQ(knight->getKillDistance(), 10);
    
    EXPECT_EQ(elf->getMoveDistance(), 10);
    EXPECT_EQ(elf->getKillDistance(), 50);
    
    EXPECT_EQ(dragon->getMoveDistance(), 50);
    EXPECT_EQ(dragon->getKillDistance(), 30);
}
TEST_F(NPCTest, GameCreation) {
    Game game;
    SUCCEED();
}
TEST_F(NPCTest, MapSymbols) {
    auto knight = std::make_shared<Knight>(0, 0, "K");
    auto elf = std::make_shared<Elf>(0, 0, "E");
    auto dragon = std::make_shared<Dragon>(0, 0, "D");
    
    EXPECT_EQ(knight->getMapSymbol(), 'K');
    EXPECT_EQ(elf->getMapSymbol(), 'E');
    EXPECT_EQ(dragon->getMapSymbol(), 'D');
}
TEST_F(NPCTest, MapPrinting) {
    std::vector<std::shared_ptr<NPC>> npcs;
    npcs.push_back(std::make_shared<Knight>(5, 5, "K1"));
    npcs.push_back(std::make_shared<Elf>(15, 15, "E1"));
    npcs.push_back(std::make_shared<Dragon>(25, 25, "D1"));
    
    EXPECT_NO_THROW(NPCFactory::printMap(npcs));
}
TEST_F(NPCTest, ThreadSafety) {
    auto npc1 = std::make_shared<Knight>(10, 10, "ThreadTest1");
    auto npc2 = std::make_shared<Elf>(20, 20, "ThreadTest2");
    
    std::thread t1([&]() {
        for (int i = 0; i < 100; ++i) {
            int x = npc1->getX();
            int y = npc1->getY();
            EXPECT_GE(x, 0);
            EXPECT_GE(y, 0);
        }
    });
    
    std::thread t2([&]() {
        for (int i = 0; i < 100; ++i) {
            int x = npc2->getX();
            int y = npc2->getY();
            EXPECT_GE(x, 0);
            EXPECT_GE(y, 0);
        }
    });
    
    std::thread t3([&]() {
        for (int i = 0; i < 100; ++i) {
            npc1->setPosition(i % MAP_WIDTH, i % MAP_HEIGHT);
        }
    });
    
    t1.join();
    t2.join();
    t3.join();
    
    EXPECT_TRUE(npc1->isAlive());
    EXPECT_TRUE(npc2->isAlive());
}
TEST_F(NPCTest, ObserverThreadSafety) {
    FileObserver observer("thread_test_log.txt");
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&observer, i]() {
            for (int j = 0; j < 5; ++j) {
                observer.onKill("Killer_" + std::to_string(i), 
                               "Victim_" + std::to_string(j));
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::ifstream log("thread_test_log.txt");
    ASSERT_TRUE(log.good());
    
    int lineCount = 0;
    std::string line;
    while (std::getline(log, line)) {
        lineCount++;
        EXPECT_TRUE(line.find("[KILL]") != std::string::npos);
    }
    
    EXPECT_EQ(lineCount, 25);
}
TEST_F(NPCTest, FactoryAvailableTypes) {
    auto types = NPCFactory::getAvailableTypes();
    
    EXPECT_EQ(types.size(), 3);
    
    bool hasKnight = false;
    bool hasElf = false;
    bool hasDragon = false;
    
    for (const auto& type : types) {
        if (type == "Knight") hasKnight = true;
        if (type == "Elf") hasElf = true;
        if (type == "Dragon") hasDragon = true;
    }
    
    EXPECT_TRUE(hasKnight);
    EXPECT_TRUE(hasElf);
    EXPECT_TRUE(hasDragon);
}
TEST_F(NPCTest, BattleLogic) {
    std::queue<BattleTask> battleQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    
    BattleVisitor visitor(battleQueue, queueMutex, queueCV);
    
    auto knight = std::make_shared<Knight>(0, 0, "Knight");
    auto dragon = std::make_shared<Dragon>(5, 0, "Dragon");
    auto elf = std::make_shared<Elf>(10, 0, "Elf");
    
    knight->accept(visitor, dragon);
    EXPECT_FALSE(battleQueue.empty());
    
    while (!battleQueue.empty()) battleQueue.pop();
    
    elf->accept(visitor, knight);
    EXPECT_FALSE(battleQueue.empty());
}
TEST_F(NPCTest, KillMethod) {
    auto npc = std::make_shared<Knight>(50, 50, "Test");
    
    EXPECT_TRUE(npc->isAlive());
    npc->kill();
    EXPECT_FALSE(npc->isAlive());
}
TEST_F(NPCTest, FactoryCreateValidTypes) {
    auto knight = NPCFactory::create("Knight", 0, 0, "Test");
    auto elf = NPCFactory::create("Elf", 0, 0, "Test");
    auto dragon = NPCFactory::create("Dragon", 0, 0, "Test");
    
    EXPECT_NE(knight, nullptr);
    EXPECT_NE(elf, nullptr);
    EXPECT_NE(dragon, nullptr);
    
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(elf->getType(), "Elf");
    EXPECT_EQ(dragon->getType(), "Dragon");
}
TEST_F(NPCTest, FactoryCreateInvalidType) {
    auto invalid = NPCFactory::create("Invalid", 0, 0, "Test");
    EXPECT_EQ(invalid, nullptr);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}