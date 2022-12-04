#include "GameSpecs.h"
#include "Human.h"
#include "Zombie.h"
#include "City.h"


City::City() {
    vector<int> humanPositions;
    for (int i = 0; i < HUMAN_STARTCOUNT; ++i) {
        int pos = rand() % (GRIDSIZE * GRIDSIZE);
        if (count(humanPositions.begin(), humanPositions.end(), pos)) {
            i--;
        } else {
            humanPositions.push_back(pos);
        }
    }

    vector<int>zombiePositions;
    for (int i = 0; i < ZOMBIE_STARTCOUNT; ++i) {
        int pos = rand() % (GRIDSIZE * GRIDSIZE);
        if (count(zombiePositions.begin(), zombiePositions.end(), pos) || count(humanPositions.begin(), humanPositions.end(), pos)) {
            i--;
        } else {
            zombiePositions.push_back(pos);
        }
    }

    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (count(humanPositions.begin(), humanPositions.end(), (i * GRIDSIZE) + j)) {
                this->grid[i][j] = new Human(this, GRIDSIZE, GRIDSIZE);
                this->grid[i][j]->setPosition(i, j);
            } else if (count(zombiePositions.begin(), zombiePositions.end(), (i * GRIDSIZE) + j)) {
                this->grid[i][j] = new Zombie(this, GRIDSIZE, GRIDSIZE);
                this->grid[i][j]->setPosition(i, j);
            } else {
                this->grid[i][j] = nullptr;
            }
        }
    }
}

City::~City() = default;

Organism *City::getOrganism(int x, int y) {
    return this->grid[x][y];
}

void City::setOrganism(Organism *organism, int x, int y) {
    this->grid[x][y] = organism;
}

void City::move() {
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

ostream &operator<<(ostream &output, City &city) {
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (city.grid[i][j] == nullptr) {
                output << " - ";
            } else {
                output << " " << city.grid[i][j]->getSpecies() << " ";
            }
        }
        output << endl;
    }
    return output;
}
