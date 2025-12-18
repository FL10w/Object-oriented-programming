#include "func.h"
#include <random>
#include <chrono>
#include <thread>
#include <fstream>
#include <algorithm>
#include <numeric>

Knight::Knight(int x, int y, const std::string& name) {
    setPosition(x, y);
    {
        std::unique_lock lock(mtx);
        this->name = name;
        this->alive = true;
    }
}

double Knight::distanceTo(const std::shared_ptr<NPC>& other) const {
    int dx = getX() - other->getX();
    int dy = getY() - other->getY();
    return std::sqrt(dx*dx + dy*dy);
}

void Knight::moveRandom() {
    if (!isAlive()) return;
    thread_local static std::mt19937 gen(std::random_device{}());
    thread_local static std::uniform_int_distribution<> dir(-1, 1);
    int d = getMoveDistance();
    int nx = getX() + dir(gen) * d;
    int ny = getY() + dir(gen) * d;
    nx = std::clamp(nx, 0, MAP_WIDTH - 1);
    ny = std::clamp(ny, 0, MAP_HEIGHT - 1);
    setPosition(nx, ny);
}

void Knight::accept(Visitor& v, std::shared_ptr<NPC> o) {
    v.visitKnight(std::static_pointer_cast<Knight>(shared_from_this()), o);
}

Elf::Elf(int x, int y, const std::string& name) {
    setPosition(x, y);
    {
        std::unique_lock lock(mtx);
        this->name = name;
        this->alive = true;
    }
}
double Elf::distanceTo(const std::shared_ptr<NPC>& other) const {
    int dx = getX() - other->getX();
    int dy = getY() - other->getY();
    return std::sqrt(dx*dx + dy*dy);
}

void Elf::moveRandom() {
    if (!isAlive()) return;
    thread_local static std::mt19937 gen(std::random_device{}());
    thread_local static std::uniform_int_distribution<> dir(-1, 1);
    int d = getMoveDistance();
    int nx = getX() + dir(gen) * d;
    int ny = getY() + dir(gen) * d;
    nx = std::clamp(nx, 0, MAP_WIDTH - 1);
    ny = std::clamp(ny, 0, MAP_HEIGHT - 1);
    setPosition(nx, ny);
}

void Elf::accept(Visitor& v, std::shared_ptr<NPC> o) {
    v.visitElf(std::static_pointer_cast<Elf>(shared_from_this()), o);
}

Dragon::Dragon(int x, int y, const std::string& name) {
    setPosition(x, y);
    {
        std::unique_lock lock(mtx);
        this->name = name;
        this->alive = true;
    }
}

double Dragon::distanceTo(const std::shared_ptr<NPC>& other) const {
    int dx = getX() - other->getX();
    int dy = getY() - other->getY();
    return std::sqrt(dx*dx + dy*dy);
}
void Dragon::moveRandom() {
    if (!isAlive()) return;
    thread_local static std::mt19937 gen(std::random_device{}());
    thread_local static std::uniform_int_distribution<> dir(-1, 1);
    int d = getMoveDistance();
    int nx = getX() + dir(gen) * d;
    int ny = getY() + dir(gen) * d;
    nx = std::clamp(nx, 0, MAP_WIDTH - 1);
    ny = std::clamp(ny, 0, MAP_HEIGHT - 1);
    setPosition(nx, ny);
}

void Dragon::accept(Visitor& v, std::shared_ptr<NPC> o) {
    v.visitDragon(std::static_pointer_cast<Dragon>(shared_from_this()), o);
}
BattleVisitor::BattleVisitor(std::queue<BattleTask>& q, std::mutex& m, std::condition_variable& cv)
    : battleQueue(q), queueMutex(m), queueCV(cv) {}

void BattleVisitor::addObserver(std::shared_ptr<Observer> obs) {
    observers.push_back(obs);
}

void BattleVisitor::notifyKill(const std::string& k, const std::string& v) {
    for (auto& obs : observers) obs->onKill(k, v);
}

void BattleVisitor::visitKnight(std::shared_ptr<Knight> self, std::shared_ptr<NPC> other) {
    if (!self->isAlive() || !other->isAlive()) return;
    int dx = self->getX() - other->getX();
    int dy = self->getY() - other->getY();
    if (dx*dx + dy*dy <= self->getKillDistance()*self->getKillDistance() && other->getType() == "Dragon") {
        {
            std::lock_guard lock(queueMutex);
            battleQueue.emplace(self, other);  
        }
        queueCV.notify_one();
    }
}

void BattleVisitor::visitElf(std::shared_ptr<Elf> self, std::shared_ptr<NPC> other) {
    if (!self->isAlive() || !other->isAlive()) return;
    int dx = self->getX() - other->getX();
    int dy = self->getY() - other->getY();
    if (dx*dx + dy*dy <= self->getKillDistance()*self->getKillDistance() && other->getType() == "Knight") {
        {
            std::lock_guard lock(queueMutex);
            battleQueue.emplace(self, other);
        }
        queueCV.notify_one();
    }
}

void BattleVisitor::visitDragon(std::shared_ptr<Dragon> self, std::shared_ptr<NPC> other) {
    if (!self->isAlive() || !other->isAlive()) return;
    int dx = self->getX() - other->getX();
    int dy = self->getY() - other->getY();
    if (dx*dx + dy*dy <= self->getKillDistance()*self->getKillDistance() && other->getType() == "Elf") {
        {
            std::lock_guard lock(queueMutex);
            battleQueue.emplace(self, other);
        }
        queueCV.notify_one();
    }
}

void ConsoleObserver::onKill(const std::string& k, const std::string& v) {
    std::lock_guard lock(OutputMutex::getCoutMutex());
    std::cout << "[KILL] " << k << " killed " << v << '\n';
}

FileObserver::FileObserver(const std::string& fname) : filename(fname) {
    file.open(fname, std::ios::app);  // ✅ открываем
}

void FileObserver::onKill(const std::string& k, const std::string& v) {
    std::lock_guard lock(fileMutex);
    if (file.is_open()) {
        file << "[KILL] " << k << " killed " << v << '\n';
        file.flush();
    }
}
std::shared_ptr<NPC> NPCFactory::create(const std::string& type, int x, int y, const std::string& name) {
    if (type == "Knight") return std::make_shared<Knight>(x, y, name);
    if (type == "Elf") return std::make_shared<Elf>(x, y, name);
    if (type == "Dragon") return std::make_shared<Dragon>(x, y, name);
    return nullptr;
}

std::vector<std::string> NPCFactory::getAvailableTypes() {
    return {"Knight", "Elf", "Dragon"};
}
std::vector<std::shared_ptr<NPC>> NPCFactory::loadFromFile(const std::string& fname) {
    std::vector<std::shared_ptr<NPC>> npcs;
    std::ifstream f(fname);
    std::string line;
    while (std::getline(f, line)) {
        std::istringstream iss(line);
        std::string type, name;
        int x, y;
        if (iss >> type >> x >> y >> name) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                if (auto npc = create(type, x, y, name)) npcs.push_back(npc);
            }
        }
    }
    return npcs;
}

void NPCFactory::saveToFile(const std::string& fname, const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::ofstream f(fname);
    for (auto& npc : npcs) {
        if (npc->isAlive()) {
            f << npc->getType() << ' ' << npc->getX() << ' ' << npc->getY() << ' ' << npc->getName() << '\n';
        }
    }
}
void NPCFactory::printAll(const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::lock_guard lock(OutputMutex::getCoutMutex());
    for (auto& npc : npcs) {
        if (npc->isAlive()) {
            std::cout << npc->getType() << " (" << npc->getName() 
                      << ") at (" << npc->getX() << ", " << npc->getY() << ")\n";
        }
    }
}

std::vector<std::shared_ptr<NPC>> NPCFactory::createRandomNPCs(int n) {
    thread_local static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> coord(0, MAP_WIDTH - 1);
    auto types = getAvailableTypes();
    std::uniform_int_distribution<> tdist(0, 2);

    std::vector<std::shared_ptr<NPC>> npcs;
    for (int i = 0; i < n; ++i) {
        int x = coord(gen), y = coord(gen);
        std::string type = types[tdist(gen)];
        std::string name = type + "_" + std::to_string(i);
        if (auto npc = create(type, x, y, name)) npcs.push_back(npc);
    }
    return npcs;
}

void NPCFactory::printMap(const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::lock_guard lock(OutputMutex::getCoutMutex());
    int sw = MAP_WIDTH / MAP_SCALE, sh = MAP_HEIGHT / MAP_SCALE;
    std::vector<std::vector<char>> map(sh, std::vector<char>(sw, '.'));
    std::vector<std::vector<int>> cnt(sh, std::vector<int>(sw, 0));
    for (auto& npc : npcs) {
        if (!npc->isAlive()) continue;
        int x = npc->getX() / MAP_SCALE;
        int y = npc->getY() / MAP_SCALE;
        if (x < 0 || x >= sw || y < 0 || y >= sh) continue;
        cnt[y][x]++;
        if (cnt[y][x] == 1) map[y][x] = npc->getMapSymbol();
        else if (cnt[y][x] == 2) map[y][x] = '2';
        else if (cnt[y][x] <= 9) map[y][x] = '0' + cnt[y][x];
        else map[y][x] = '+';
    }

    std::cout << "\n=== MAP (" << MAP_WIDTH << "x" << MAP_HEIGHT << " scaled to " 
              << sw << "x" << sh << ") ===\n";
    for (int i = 0; i < sh; ++i) {
        std::cout << '|';
        for (int j = 0; j < sw; ++j) std::cout << map[i][j];
        std::cout << "|\n";
    }

    int alive = 0, cells = 0, maxc = 0;
    std::map<std::string, int> types;
    for (auto& npc : npcs) {
        if (npc->isAlive()) {
            alive++;
            types[npc->getType()]++;
        }
    }
    for (int i = 0; i < sh; ++i)
        for (int j = 0; j < sw; ++j) {
            if (cnt[i][j] > 0) { cells++; maxc = std::max(maxc, cnt[i][j]); }
        }

    std::cout << "NPCs: " << alive << " | Occupied cells: " << cells 
              << "/" << (sw * sh) << " | Max in cell: " << maxc << "\n";
    std::cout << "Legend: K-Knight, E-Elf, D-Dragon, 2-9-count, +-10+\n";
}

void NPCFactory::printDetailedStats(const std::vector<std::shared_ptr<NPC>>& npcs) {
    std::lock_guard lock(OutputMutex::getCoutMutex());
    std::map<std::string, int> cnt;
    for (auto& npc : npcs) if (npc->isAlive()) cnt[npc->getType()]++;
    std::cout << "\n=== NPC DETAILS ===\n";
    std::cout << "Total alive: " << std::accumulate(cnt.begin(), cnt.end(), 0,
        [](int s, auto& p) { return s + p.second; }) << "/" << npcs.size() << "\n";
    for (auto& [t, c] : cnt) std::cout << t << ": " << c << "\n";
}
Game::Game() {
    npcs = NPCFactory::createRandomNPCs(50);
    {
        std::lock_guard lock(OutputMutex::getCoutMutex());
        std::cout << "[GAME] Created " << npcs.size() << " NPCs\n";
        std::map<std::string, int> cnt;
        for (auto& n : npcs) cnt[n->getType()]++;
        for (auto& [t, c] : cnt) std::cout << "[GAME] " << t << ": " << c << "\n";
    }
}

Game::~Game() {
    stop();
}

void Game::start() {
    running = true;
    movementThreadObj = std::thread(&Game::movementThread, this);
    battleThreadObj = std::thread(&Game::battleThread, this);
    mainThreadObj = std::thread(&Game::mainThread, this);

    std::lock_guard lock(OutputMutex::getCoutMutex());
    std::cout << "\n=== GAME STARTED ===\n";
    std::cout << "Duration: " << GAME_DURATION << " seconds\n";
    std::cout << "NPCs: Knight (move:30/kill:10), Elf (10/50), Dragon (50/30)\n";
    std::cout << "Main thread will update every second...\n";
}

void Game::stop() {
    running = false;
    queueCV.notify_all();
    
    if (movementThreadObj.joinable()) movementThreadObj.join();
    if (battleThreadObj.joinable()) battleThreadObj.join();
    if (mainThreadObj.joinable()) mainThreadObj.join();
}
void Game::mainThread() {
    auto end = std::chrono::steady_clock::now() + std::chrono::seconds(GAME_DURATION);
    int sec = 0;

    while (running) {
        auto now = std::chrono::steady_clock::now();
        if (now >= end) break;

        auto remain = std::chrono::duration_cast<std::chrono::milliseconds>(end - now);
        auto wait = std::min(remain, std::chrono::milliseconds(1000));
        std::this_thread::sleep_for(wait);

        if (!running) break;
        sec++;

        std::lock_guard lock(OutputMutex::getCoutMutex());
        std::cout << "\n=== TIME: " << sec << "s / " << GAME_DURATION << "s ===\n";
        NPCFactory::printMap(npcs);

        int alive = 0;
        std::map<std::string, int> types;
        for (auto& n : npcs) if (n->isAlive()) { alive++; types[n->getType()]++; }
        std::cout << "Alive: " << alive << "/" << npcs.size() << " | ";
        for (auto& [t, c] : types) std::cout << t << ":" << c << " ";
        std::cout << "\n";
    }
    running = false;
    queueCV.notify_all();
    printFinalResults();  
}

void Game::movementThread() {
    auto consoleObs = std::make_shared<ConsoleObserver>();
    auto fileObs = std::make_shared<FileObserver>("log.txt");
    int iter = 0;

    while (running) {
        iter++;
        BattleVisitor visitor(battleQueue, queueMutex, queueCV);
        visitor.addObserver(consoleObs);
        visitor.addObserver(fileObs);

        for (auto& npc : npcs)
            if (npc->isAlive()) {
                npc->moveRandom();
                for (auto& other : npcs)
                    if (npc != other && other->isAlive())
                        npc->accept(visitor, other);
            }

        if (iter % 20 == 0) {
            std::lock_guard lock(OutputMutex::getCoutMutex());
            std::cout << "[MOVEMENT] Iteration " << iter << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
void Game::battleThread() {
    thread_local static std::mt19937 gen(std::random_device{}());
    thread_local static std::uniform_int_distribution<> dice(1, 6);
    auto consoleObs = std::make_shared<ConsoleObserver>();
    auto fileObs = std::make_shared<FileObserver>("log.txt");
    int n = 0;

    while (running) {
        BattleTask task(nullptr, nullptr);
        bool got = false;

        {
            std::unique_lock lock(queueMutex);
            if (queueCV.wait_for(lock, std::chrono::milliseconds(100),
                [this] { return !battleQueue.empty() || !running; }) && running) {
                if (!battleQueue.empty()) {
                    task = std::move(battleQueue.front());
                    battleQueue.pop();
                    got = true;
                }
            }
        }
        if (got && task.attacker && task.defender && task.attacker->isAlive() && task.defender->isAlive()) {
            n++;
            int a = dice(gen), d = dice(gen);
            std::lock_guard lock(OutputMutex::getCoutMutex());
            std::cout << "[BATTLE #" << n << "] "
                      << task.attacker->getName() << " vs " << task.defender->getName()
                      << " | " << a << " vs " << d << "\n";
            if (a > d) {
                task.defender->kill();
                consoleObs->onKill(task.attacker->getName(), task.defender->getName());
                std::cout << "[KILLED] " << task.defender->getName() << "\n";
            }
        }
    }
}

void Game::printFinalResults() {
    std::lock_guard lock(OutputMutex::getCoutMutex());
    std::cout << "\n=== GAME OVER ===\n";
    NPCFactory::printMap(npcs);
    std::vector<std::shared_ptr<NPC>> survivors;
    std::copy_if(npcs.begin(), npcs.end(), std::back_inserter(survivors),
        [](auto& n) { return n->isAlive(); });

    std::cout << "\nSurvivors (" << survivors.size() << "):\n";
    for (auto& s : survivors)
        std::cout << "  " << s->getName() << " (" << s->getType() 
                  << ") @ (" << s->getX() << ", " << s->getY() << ")\n";

    NPCFactory::saveToFile("survivors.txt", survivors);
    std::cout << "\n Results saved to 'survivors.txt' and 'log.txt'\n";
}