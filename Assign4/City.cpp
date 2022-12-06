#include <windows.h>
#include <conio.h>
#include <random>
#include "GameSpecs.h"
#include "Human.h"
#include "Zombie.h"
#include "City.h"

void Col(int);

City::City() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, GRIDSIZE * GRIDSIZE - 1);

    vector<int> humanPositions;
    for (int i = 0; i < HUMAN_STARTCOUNT; ++i) {
        int pos = dis(gen);
        if (count(humanPositions.begin(), humanPositions.end(), pos)) {
            i--;
        } else {
            humanPositions.push_back(pos);
        }
    }

    vector<int>zombiePositions;
    for (int i = 0; i < ZOMBIE_STARTCOUNT; ++i) {
        int pos =dis(gen);
        if (count(zombiePositions.begin(), zombiePositions.end(), pos) || count(humanPositions.begin(), humanPositions.end(), pos)) {
            i--;
        } else {
            zombiePositions.push_back(pos);
        }
    }

    //flip i and j on x and y since for i and for j is rows then columns but x comes first which is columns.
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (count(humanPositions.begin(), humanPositions.end(), (i * GRIDSIZE) + j)) {
                this->grid[i][j] = new Human(this, GRIDSIZE, GRIDSIZE);
                this->grid[i][j]->setPosition(j, i);
            } else if (count(zombiePositions.begin(), zombiePositions.end(), (i * GRIDSIZE) + j)) {
                this->grid[i][j] = new Zombie(this, GRIDSIZE, GRIDSIZE);
                this->grid[i][j]->setPosition(j, i);
            } else {
                this->grid[i][j] = nullptr;
            }
        }
    }
    this->generation = 0;
    this->numHumans = HUMAN_STARTCOUNT;
    this->numZombies = ZOMBIE_STARTCOUNT;
}

City::~City() = default;

//flip i and j on x and y since for i and for j is rows then columns but x comes first which is columns.
Organism *City::getOrganism(int x, int y) {
    return this->grid[y][x];
}

//flip i and j on x and y since for i and for j is rows then columns but x comes first which is columns.
void City::setOrganism(Organism *organism, int x, int y) {
    this->grid[y][x] = organism;
}

void City::move() {
    this->generation += 1;
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (this->grid[i][j] != nullptr && this->grid[i][j]->isTurn()) {
                this->grid[i][j]->move();
            }
        }
    }
}

void City::reset() {
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (this->grid[i][j] != nullptr) {
                this->grid[i][j]->reset();
            }
        }
    }
}

bool City::hasDiversity() {
    return (this->numZombies != 0 && this->numHumans != 0 && this->generation != ITERATIONS);
}

void City::lessHuman() {
    this->numHumans -= 1;
}

void City::lessZombie() {
    this->numZombies -= 1;
}

void City::moreHuman() {
    this->numHumans += 1;
}

void City::moreZombie() {
    this->numZombies += 1;
}

ostream &operator<<(ostream &output, City &city) {
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (city.grid[i][j] == nullptr) {
                output << " - ";
            } else if (city.grid[i][j]->getSpecies() == 'H') {
                Col(HUMAN_COLOR);
                output << " H ";
                Col(WHITE_COLOR);
            } else {
                Col(ZOMBIE_COLOR);
                output << " Z ";
                Col(WHITE_COLOR);
            }
        }
        output << endl;
    }

    output << "Generation: " << city.generation << endl;
    output << "Humans: " << city.numHumans << endl;
    output << "Zombies: " << city.numZombies << endl;

    return output;
}

void Col (int c) {
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c);
}
