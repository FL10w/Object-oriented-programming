#include "func.h"
#include <gtest/gtest.h>
#include <filesystem>

class NPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove("test_dungeon.txt");
        std::filesystem::remove("log.txt");
    }
    void TearDown() override {
        std::filesystem::remove("test_dungeon.txt");
        std::filesystem::remove("log.txt");
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
TEST_F(NPCTest, KnightKillsDragon) {
    auto knight = std::make_shared<Knight>(0, 0, "Lancelot");
    auto dragon = std::make_shared<Dragon>(2, 2, "Drogon");
    std::vector<std::shared_ptr<NPC>> npcs = {knight, dragon};

    NPCFactory::battle(npcs, 5.0);

    EXPECT_TRUE(knight->isAlive());
    EXPECT_FALSE(dragon->isAlive());

    std::ifstream log("log.txt");
    std::string line;
    ASSERT_TRUE(std::getline(log, line));
    EXPECT_NE(line.find("Lancelot killed Drogon"), std::string::npos);
}
TEST_F(NPCTest, ElfKillsKnight) {
    auto elf = std::make_shared<Elf>(0, 0, "Thranduil");
    auto knight = std::make_shared<Knight>(1, 1, "Arthur");
    std::vector<std::shared_ptr<NPC>> npcs = {elf, knight};

    NPCFactory::battle(npcs, 5.0);

    EXPECT_TRUE(elf->isAlive());
    EXPECT_FALSE(knight->isAlive());

    std::ifstream log("log.txt");
    std::string line;
    ASSERT_TRUE(std::getline(log, line));
    EXPECT_NE(line.find("Thranduil killed Arthur"), std::string::npos);
}