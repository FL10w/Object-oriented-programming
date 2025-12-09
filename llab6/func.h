// func.h
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

class Visitor;
class Observer;
class NPC : public std::enable_shared_from_this<NPC> {
public:
    virtual ~NPC() = default;

    virtual void accept(Visitor& visitor, std::shared_ptr<NPC> other) = 0;
    virtual bool isAlive() const = 0;
    virtual std::string getType() const = 0;
    virtual void kill() = 0;
    virtual double distanceTo(const std::shared_ptr<NPC>& other) const = 0;

    // Публичные геттеры — безопасный доступ
    int getX() const { return x; }
    int getY() const { return y; }
    const std::string& getName() const { return name; }

protected:
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
    void kill() override { alive = false; }
    double distanceTo(const std::shared_ptr<NPC>& other) const override;
};
class Elf : public NPC {
public:
    Elf(int x, int y, const std::string& name);
    void accept(Visitor& visitor, std::shared_ptr<NPC> other) override;
    bool isAlive() const override { return alive; }
    std::string getType() const override { return "Elf"; }
    void kill() override { alive = false; }
    double distanceTo(const std::shared_ptr<NPC>& other) const override;
};
class Dragon : public NPC {
public:
    Dragon(int x, int y, const std::string& name);
    void accept(Visitor& visitor, std::shared_ptr<NPC> other) override;
    bool isAlive() const override { return alive; }
    std::string getType() const override { return "Dragon"; }
    void kill() override { alive = false; }
    double distanceTo(const std::shared_ptr<NPC>& other) const override;
};
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visitKnight(std::shared_ptr<Knight> self, std::shared_ptr<NPC> other) = 0;
    virtual void visitElf(std::shared_ptr<Elf> self, std::shared_ptr<NPC> other) = 0;
    virtual void visitDragon(std::shared_ptr<Dragon> self, std::shared_ptr<NPC> other) = 0;
};
class BattleVisitor : public Visitor {
public:
    void visitKnight(std::shared_ptr<Knight> self, std::shared_ptr<NPC> other) override;
    void visitElf(std::shared_ptr<Elf> self, std::shared_ptr<NPC> other) override;
    void visitDragon(std::shared_ptr<Dragon> self, std::shared_ptr<NPC> other) override;

    void notifyKill(const std::string& killer, const std::string& victim);
    void addObserver(std::shared_ptr<Observer> obs);
private:
    std::vector<std::shared_ptr<Observer>> observers;
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
};
class NPCFactory {
public:
    static std::shared_ptr<NPC> create(const std::string& type, int x, int y, const std::string& name);
    static std::vector<std::shared_ptr<NPC>> loadFromFile(const std::string& filename);
    static void saveToFile(const std::string& filename, const std::vector<std::shared_ptr<NPC>>& npcs);
    static void printAll(const std::vector<std::shared_ptr<NPC>>& npcs);
    static void battle(std::vector<std::shared_ptr<NPC>>& npcs, double range);
};