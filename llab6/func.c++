#include "func.h"

Knight::Knight(int x, int y, const std::string& name) {
    this->x = x;
    this->y = y;
    this->name = name;
}
double Knight::distanceTo(const std::shared_ptr<NPC>& other) const {
    int dx = this->x - other->getX();
    int dy = this->y - other->getY();
    return std::sqrt(dx * dx + dy * dy);
}
void Knight::accept(Visitor& visitor, std::shared_ptr<NPC> other) {
    visitor.visitKnight(std::static_pointer_cast<Knight>(shared_from_this()), other);
}


Elf::Elf(int x, int y, const std::string& name) {
    this->x = x;
    this->y = y;
    this->name = name;
}
double Elf::distanceTo(const std::shared_ptr<NPC>& other) const {
    int dx = this->x - other->getX();
    int dy = this->y - other->getY();
    return std::sqrt(dx * dx + dy * dy);
}
void Elf::accept(Visitor& visitor, std::shared_ptr<NPC> other) {
    visitor.visitElf(std::static_pointer_cast<Elf>(shared_from_this()), other);
}


Dragon::Dragon(int x, int y, const std::string& name) {
    this->x = x;
    this->y = y;
    this->name = name;
}
double Dragon::distanceTo(const std::shared_ptr<NPC>& other) const {
    int dx = this->x - other->getX();
    int dy = this->y - other->getY();
    return std::sqrt(dx * dx + dy * dy);
}
void Dragon::accept(Visitor& visitor, std::shared_ptr<NPC> other) {
    visitor.visitDragon(std::static_pointer_cast<Dragon>(shared_from_this()), other);
}


void BattleVisitor::addObserver(std::shared_ptr<Observer> obs) {
    observers.push_back(obs);
}
void BattleVisitor::notifyKill(const std::string& killer, const std::string& victim) {
    for (auto& obs : observers) {
        obs->onKill(killer, victim);
    }
}
void BattleVisitor::visitKnight(std::shared_ptr<Knight> self, std::shared_ptr<NPC> other) {
    if (!self->isAlive() || !other->isAlive()) return;
    if (other->getType() == "Dragon") {
        other->kill();
        notifyKill(self->getName(), other->getName());
    }
}
void BattleVisitor::visitElf(std::shared_ptr<Elf> self, std::shared_ptr<NPC> other) {
    if (!self->isAlive() || !other->isAlive()) return;
    if (other->getType() == "Knight") {
        other->kill();
        notifyKill(self->getName(), other->getName());
    }
}

void BattleVisitor::visitDragon(std::shared_ptr<Dragon> self, std::shared_ptr<NPC> other) {
}

void ConsoleObserver::onKill(const std::string& killer, const std::string& victim) {
    std::cout << "[KILL] " << killer << " killed " << victim << std::endl;
}

FileObserver::FileObserver(const std::string& filename) : filename(filename) {}
void FileObserver::onKill(const std::string& killer, const std::string& victim) {
    std::ofstream file(filename, std::ios::app);
    file << "[KILL] " << killer << " killed " << victim << std::endl;
    file.close();
}

std::shared_ptr<NPC> NPCFactory::create(const std::string& type, int x, int y, const std::string& name) {
    if (type == "Knight") return std::make_shared<Knight>(x, y, name);
    if (type == "Elf") return std::make_shared<Elf>(x, y, name);
    if (type == "Dragon") return std::make_shared<Dragon>(x, y, name);
    return nullptr;
}
std::vector<std::shared_ptr<NPC>> NPCFactory::loadFromFile(const std::string& filename) {
    std::vector<std::shared_ptr<NPC>> npcs;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type, name;
        int x, y;
        if (iss >> type >> x >> y >> name) {
            if (x >= 0 && x <= 500 && y >= 0 && y <= 500) {
                auto npc = create(type, x, y, name);
                if (npc) npcs.push_back(npc);
            }
        }
    }
    return npcs;
}
void NPCFactory::saveToFile(const std::string& filename, const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::ofstream file(filename);
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            file << npc->getType() << " " << npc->getX() << " " << npc->getY()
                 << " " << npc->getName() << "\n";
        }
    }
    file.close();
}
void NPCFactory::printAll(const std::vector<std::shared_ptr<NPC>>& npcs) {
    for (const auto& npc : npcs) {
        if (npc->isAlive()) {
            std::cout << npc->getType() << " (" << npc->getName()
                      << ") at (" << npc->getX() << ", " << npc->getY() << ")\n";
        }
    }
}
void NPCFactory::battle(std::vector<std::shared_ptr<NPC>>& npcs, double range) {
    BattleVisitor visitor;
    visitor.addObserver(std::make_shared<ConsoleObserver>());
    visitor.addObserver(std::make_shared<FileObserver>());

    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = 0; j < npcs.size(); ++j) {
            if (i == j) continue;
            if (npcs[i]->isAlive() && npcs[j]->isAlive() &&
                npcs[i]->distanceTo(npcs[j]) <= range) {
                npcs[i]->accept(visitor, npcs[j]);
            }
        }
    }
}