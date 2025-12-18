#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <mutex>
#include <shared_mutex>
#include <random>
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <map>

const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 100;
const int GAME_DURATION = 30;
const int MAP_SCALE = 10;
class Visitor;
class Observer;
class OutputMutex {
public:
    static std::mutex& getCoutMutex() {
        static std::mutex coutMutex;
        return coutMutex;
    }
};
class NPC : public std::enable_shared_from_this<NPC> {
public:
    virtual ~NPC() = default;
    virtual void accept(Visitor& visitor, std::shared_ptr<NPC> other) = 0;
    virtual bool isAlive() const = 0;
    virtual std::string getType() const = 0;
    virtual void kill() = 0;
    virtual double distanceTo(const std::shared_ptr<NPC>& other) const = 0;
    virtual void moveRandom() = 0;
    virtual int getMoveDistance() const = 0;
    virtual int getKillDistance() const = 0;
    virtual char getMapSymbol() const = 0;
    int getX() const { 
        std::shared_lock lock(mtx);
        return x; 
    }
    int getY() const { 
        std::shared_lock lock(mtx);
        return y; 
    }
    const std::string& getName() const { 
        std::shared_lock lock(mtx);
        return name; 
    }
    
    void setPosition(int newX, int newY) {
        std::unique_lock lock(mtx);
        x = newX;
        y = newY;
    }
protected:
    mutable std::shared_mutex mtx;
    int x = 0;
    int y = 0;
    std::string name;
    bool alive = true;
};
class Knight : public NPC {
public:
    Knight(int x, int y, const std::string& name);
    void accept(Visitor& visitor, std::shared_ptr<NPC> other) override;
    bool isAlive() const override { return alive; }
    std::string getType() const override { return "Knight"; }
    char getMapSymbol() const override { return 'K'; }
    void kill() override { alive = false; }
    double distanceTo(const std::shared_ptr<NPC>& other) const override;
    void moveRandom() override;
    int getMoveDistance() const override { return 30; }
    int getKillDistance() const override { return 10; }
};
class Elf : public NPC {
public:
    Elf(int x, int y, const std::string& name);
    void accept(Visitor& visitor, std::shared_ptr<NPC> other) override;
    bool isAlive() const override { return alive; }
    std::string getType() const override { return "Elf"; }
    char getMapSymbol() const override { return 'E'; }
    void kill() override { alive = false; }
    double distanceTo(const std::shared_ptr<NPC>& other) const override;
    void moveRandom() override;
    int getMoveDistance() const override { return 10; }
    int getKillDistance() const override { return 50; }
};
class Dragon : public NPC {
public:
    Dragon(int x, int y, const std::string& name);
    void accept(Visitor& visitor, std::shared_ptr<NPC> other) override;
    bool isAlive() const override { return alive; }
    std::string getType() const override { return "Dragon"; }
    char getMapSymbol() const override { return 'D'; }
    void kill() override { alive = false; }
    double distanceTo(const std::shared_ptr<NPC>& other) const override;
    void moveRandom() override;
    int getMoveDistance() const override { return 50; }
    int getKillDistance() const override { return 30; }
};
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visitKnight(std::shared_ptr<Knight> self, std::shared_ptr<NPC> other) = 0;
    virtual void visitElf(std::shared_ptr<Elf> self, std::shared_ptr<NPC> other) = 0;
    virtual void visitDragon(std::shared_ptr<Dragon> self, std::shared_ptr<NPC> other) = 0;
};
struct BattleTask {
    std::shared_ptr<NPC> attacker;
    std::shared_ptr<NPC> defender;
    BattleTask(std::shared_ptr<NPC> a, std::shared_ptr<NPC> d)
        : attacker(std::move(a)), defender(std::move(d)) {}
};
class BattleVisitor : public Visitor {
public:
    BattleVisitor(std::queue<BattleTask>& battleQueue, 
                  std::mutex& queueMutex,
                  std::condition_variable& queueCV);
    void visitKnight(std::shared_ptr<Knight> self, std::shared_ptr<NPC> other) override;
    void visitElf(std::shared_ptr<Elf> self, std::shared_ptr<NPC> other) override;
    void visitDragon(std::shared_ptr<Dragon> self, std::shared_ptr<NPC> other) override;

    void notifyKill(const std::string& killer, const std::string& victim);
    void addObserver(std::shared_ptr<Observer> obs);
private:
    std::vector<std::shared_ptr<Observer>> observers;
    std::queue<BattleTask>& battleQueue;
    std::mutex& queueMutex;
    std::condition_variable& queueCV;
};
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onKill(const std::string& killer, const std::string& victim) = 0;
};
class ConsoleObserver : public Observer {
public:
    void onKill(const std::string& killer, const std::string& victim) override;
};
class FileObserver : public Observer {
public:
    FileObserver(const std::string& filename = "log.txt");
    void onKill(const std::string& killer, const std::string& victim) override;
private:
    std::string filename;
    mutable std::mutex fileMutex;
    std::ofstream file;  
};
class NPCFactory {
public:
    static std::shared_ptr<NPC> create(const std::string& type, int x, int y, const std::string& name);
    static std::vector<std::shared_ptr<NPC>> loadFromFile(const std::string& filename);
    static void saveToFile(const std::string& filename, const std::vector<std::shared_ptr<NPC>>& npcs);
    static void printAll(const std::vector<std::shared_ptr<NPC>>& npcs);
    
    static std::vector<std::shared_ptr<NPC>> createRandomNPCs(int count);
    static void printMap(const std::vector<std::shared_ptr<NPC>>& npcs);
    static std::vector<std::string> getAvailableTypes();
    static void printDetailedStats(const std::vector<std::shared_ptr<NPC>>& npcs);
};
class Game {
public:
    Game();
    ~Game();
    
    void start();
    void stop();
private:
    void movementThread();
    void battleThread();
    void mainThread();
    void printFinalResults(); 
    std::vector<std::shared_ptr<NPC>> npcs;
    std::thread movementThreadObj;
    std::thread battleThreadObj;
    std::thread mainThreadObj;
    std::queue<BattleTask> battleQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::atomic<bool> running{false};
    std::atomic<bool> gameOver{false};
};