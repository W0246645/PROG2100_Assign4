#include <random>
#include "GameSpecs.h"
#include "Zombie.h"
#include "Human.h"

Zombie::Zombie() {
    this->species = 'Z';
    this->moved = true;
    this->humanFound = false;
    this->steps = 0;
    this->stepsSinceFood = 0;
    this->humanFound = false;
}

Zombie::Zombie(City *city, int width, int height) {
    this->species = 'Z';
    this->moved = true;
    this->city = city;
    this->width = width;
    this->height = height;
    this->humanFound = false;
    this->steps = 0;
    this->stepsSinceFood = 0;
}

Zombie::~Zombie() = default;

void Zombie::move() {
    this->setTargets();
    if (!this->targets.empty()) {
        if (this->humanFound) {
            this->city->lessHuman();
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, this->targets.size() - 1);
        int pos = dis(gen);
        //1 is NW, 2 is N, 3 is NE, 4 is E, etc etc.
        switch (this->targets[pos]) {
            case 1:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->x -= 1;
                this->y -= 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 2:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->y -= 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 3:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->x += 1;
                this->y -= 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 4:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->x += 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 5:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->x += 1;
                this->y += 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 6:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->y += 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 7:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->x -= 1;
                this->y += 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 8:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->x -= 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
        }
    }
    this->targets.clear();

    if (this->steps == 8) {
        this->breedTargets();
        if (!this->targets.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, this->targets.size() - 1);
            int pos = dis(gen);
            //1 is NW, 2 is N, 3 is NE, 4 is E, etc etc.
            switch (this->targets[pos]) {
                case 1:
                    this->city->setOrganism((new Zombie(this->city, GRIDSIZE, GRIDSIZE)), this->x-1, this->y-1);
                    this->city->getOrganism(this->x-1, this->y-1)->setPosition(this->x-1, this->y-1);
                    break;
                case 2:
                    this->city->setOrganism((new Zombie(this->city, GRIDSIZE, GRIDSIZE)), this->x, this->y-1);
                    this->city->getOrganism(this->x, this->y-1)->setPosition(this->x, this->y-1);
                    break;
                case 3:
                    this->city->setOrganism((new Zombie(this->city, GRIDSIZE, GRIDSIZE)), this->x+1, this->y-1);
                    this->city->getOrganism(this->x+1, this->y-1)->setPosition(this->x+1, this->y-1);
                    break;
                case 4:
                    this->city->setOrganism((new Zombie(this->city, GRIDSIZE, GRIDSIZE)), this->x+1, this->y);
                    this->city->getOrganism(this->x+1, this->y)->setPosition(this->x+1, this->y);
                    break;
                case 5:
                    this->city->setOrganism((new Zombie(this->city, GRIDSIZE, GRIDSIZE)), this->x+1, this->y+1);
                    this->city->getOrganism(this->x+1, this->y+1)->setPosition(this->x+1, this->y+1);
                    break;
                case 6:
                    this->city->setOrganism((new Zombie(this->city, GRIDSIZE, GRIDSIZE)), this->x, this->y+1);
                    this->city->getOrganism(this->x, this->y+1)->setPosition(this->x, this->y+1);
                    break;
                case 7:
                    this->city->setOrganism((new Zombie(this->city, GRIDSIZE, GRIDSIZE)), this->x-1, this->y+1);
                    this->city->getOrganism(this->x-1, this->y+1)->setPosition(this->x-1, this->y+1);
                    break;
                case 8:
                    this->city->setOrganism((new Zombie(this->city, GRIDSIZE, GRIDSIZE)), this->x-1, this->y);
                    this->city->getOrganism(this->x-1, this->y)->setPosition(this->x-1, this->y);
                    break;
            }
            this->steps = -1;
            this->city->lessHuman();
            this->city->moreZombie();
        } else {
            this->steps -= 1;
        }
    }
    this->steps += 1;
    this->targets.clear();
    if (this->stepsSinceFood == 3 && !this->humanFound) {
        this->city->lessZombie();
        this->city->moreHuman();
        int x = this->x;
        int y = this->y;
        this->city->setOrganism((new Human(this->city, GRIDSIZE, GRIDSIZE)), x, y);
        this->city->getOrganism(x, y)->setPosition(x, y);
        return;
    } else if(!this->humanFound) {
        this->stepsSinceFood +=1;
    } else {
        this->stepsSinceFood = 0;
    }
    this->humanFound = false;
    this->moved = true;
}

//Will push an approved position to the target vector. 1 is NW, 2 is N, 3 is NE, 4 is E, etc etc.
//Makes sure that the possible position is not going to push it off the edge of the grid and that it's null.
//If it finds a human in an acceptable position it'll remove any of the nullptr positions in the target vector
//and only look for humans after one human is found.
void Zombie::setTargets() {
    if (this->x != 0 && this->y != 0) {
        if (this->city->getOrganism(this->x - 1, this->y - 1) != nullptr) {
            if (this->city->getOrganism(this->x - 1, this->y - 1)->getSpecies() == 'H') {
                if (this->humanFound) {
                    this->targets.push_back(1);
                } else {
                    this->targets.clear();
                    this->targets.push_back(1);
                    this->humanFound = true;
                }
            }
        } else if (!this->humanFound) {
            this->targets.push_back(1);
        }
    }
    if (this->y != 0) {
        if (this->city->getOrganism(this->x, this->y - 1) != nullptr) {
            if (this->city->getOrganism(this->x, this->y - 1)->getSpecies() == 'H') {
                if (this->humanFound) {
                    this->targets.push_back(2);
                } else {
                    this->targets.clear();
                    this->targets.push_back(2);
                    this->humanFound = true;
                }
            }
        } else if (!this->humanFound) {
            this->targets.push_back(2);
        }
    }
    if (this->x != this->width-1 && this->y != 0) {
        if (this->city->getOrganism(this->x + 1, this->y - 1) != nullptr) {
            if (this->city->getOrganism(this->x + 1, this->y - 1)->getSpecies() == 'H') {
                if (this->humanFound) {
                    this->targets.push_back(3);
                } else {
                    this->targets.clear();
                    this->targets.push_back(3);
                    this->humanFound = true;
                }
            }
        } else if (!this->humanFound) {
            this->targets.push_back(3);
        }
    }
    if (this->x != this->width-1) {
        if (this->city->getOrganism(this->x + 1, this->y) != nullptr) {
            if (this->city->getOrganism(this->x + 1, this->y)->getSpecies() == 'H') {
                if (this->humanFound) {
                    this->targets.push_back(4);
                } else {
                    this->targets.clear();
                    this->targets.push_back(4);
                    this->humanFound = true;
                }
            }
        } else if (!this->humanFound) {
            this->targets.push_back(4);
        }
    }
    if (this->x != this->width-1 && this->y != this->height-1) {
        if (this->city->getOrganism(this->x + 1, this->y + 1) != nullptr) {
            if (this->city->getOrganism(this->x + 1, this->y + 1)->getSpecies() == 'H') {
                if (this->humanFound) {
                    this->targets.push_back(5);
                } else {
                    this->targets.clear();
                    this->targets.push_back(5);
                    this->humanFound = true;
                }
            }
        } else if (!this->humanFound) {
            this->targets.push_back(5);
        }
    }
    if (this->y != this->width-1) {
        if (this->city->getOrganism(this->x, this->y + 1) != nullptr) {
            if (this->city->getOrganism(this->x, this->y + 1)->getSpecies() == 'H') {
                if (this->humanFound) {
                    this->targets.push_back(6);
                } else {
                    this->targets.clear();
                    this->targets.push_back(6);
                    this->humanFound = true;
                }
            }
        } else if (!this->humanFound) {
            this->targets.push_back(6);
        }
    }
    if (this->x != 0 && this->y != this->width-1) {
        if (this->city->getOrganism(this->x - 1, this->y + 1) != nullptr) {
            if (this->city->getOrganism(this->x - 1, this->y + 1)->getSpecies() == 'H') {
                if (this->humanFound) {
                    this->targets.push_back(7);
                } else {
                    this->targets.clear();
                    this->targets.push_back(7);
                    this->humanFound = true;
                }
            }
        } else if (!this->humanFound) {
            this->targets.push_back(7);
        }
    }
    if (this->x != 0) {
        if (this->city->getOrganism(this->x - 1, this->y) != nullptr) {
            if (this->city->getOrganism(this->x - 1, this->y)->getSpecies() == 'H') {
                if (this->humanFound) {
                    this->targets.push_back(8);
                } else {
                    this->targets.clear();
                    this->targets.push_back(8);
                    this->humanFound = true;
                }
            }
        } else if (!this->humanFound) {
            this->targets.push_back(8);
        }
    }
}

//Will push an approved position to the target vector. 1 is NW, 2 is N, 3 is NE, 4 is E, etc etc.
//Makes sure that the possible position is not going to push it off the edge of the grid and that it's a human.
void Zombie::breedTargets() {
    if (this->x != 0 && this->y != 0) {
        if (this->city->getOrganism(this->x - 1, this->y - 1) != nullptr && this->city->getOrganism(this->x - 1, this->y - 1)->getSpecies() == 'H') {
            this->targets.push_back(1);
        }
    }
    if (this->y != 0) {
        if (this->city->getOrganism(this->x, this->y - 1) != nullptr && this->city->getOrganism(this->x, this->y - 1)->getSpecies() == 'H') {
            this->targets.push_back(2);
        }
    }
    if (this->x != this->width-1 && this->y != 0) {
        if (this->city->getOrganism(this->x + 1, this->y - 1) != nullptr && this->city->getOrganism(this->x + 1, this->y - 1)->getSpecies() == 'H') {
            this->targets.push_back(3);
        }
    }
    if (this->x != this->width-1) {
        if (this->city->getOrganism(this->x + 1, this->y) != nullptr && this->city->getOrganism(this->x + 1, this->y)->getSpecies() == 'H') {
            this->targets.push_back(4);
        }
    }
    if (this->x != this->width-1 && this->y != this->height-1) {
        if (this->city->getOrganism(this->x + 1, this->y + 1) != nullptr && this->city->getOrganism(this->x + 1, this->y + 1)->getSpecies() == 'H') {
            this->targets.push_back(5);
        }
    }
    if (this->y != this->width-1) {
        if (this->city->getOrganism(this->x, this->y + 1) != nullptr && this->city->getOrganism(this->x, this->y + 1)->getSpecies() == 'H')  {
            this->targets.push_back(6);
        }
    }
    if (this->x != 0 && this->y != this->width-1) {
        if (this->city->getOrganism(this->x - 1, this->y + 1) != nullptr && this->city->getOrganism(this->x - 1, this->y + 1)->getSpecies() == 'H') {
            this->targets.push_back(7);
        }
    }
    if (this->x != 0) {
        if (this->city->getOrganism(this->x - 1, this->y) != nullptr && this->city->getOrganism(this->x - 1, this->y)->getSpecies() == 'H') {
            this->targets.push_back(8);
        }
    }
}


