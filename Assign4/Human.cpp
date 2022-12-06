#include <random>
#include "GameSpecs.h"
#include "Human.h"

Human::Human() {
    this->species = 'H';
    this->moved = true;
    this->steps = 0;
}

Human::Human(City *city, int width, int height) {
    this->species = 'H';
    this->moved = true;
    this->city = city;
    this->width = width;
    this->height = height;
    this->steps = 0;
}

Human::~Human() = default;

void Human::move() {
    this->setTargets();
    if (this->targets.size() != 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, this->targets.size() - 1);
        int pos = dis(gen);
        switch (this->targets[pos]) {
            case 2:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->y -= 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 4:
                this->city->setOrganism(nullptr, this->x, this->y);
                this->x += 1;
                this->city->setOrganism(this, this->x, this->y);
                break;
            case 6:
                this->city->setOrganism(nullptr, this->x, this->y);
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
    if (this->steps == 3) {
        this->recruitTargets();
        if (!this->targets.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, this->targets.size() - 1);
            int pos = dis(gen);
            switch (this->targets[pos]) {
                case 2:
                    this->city->setOrganism((new Human(this->city, GRIDSIZE, GRIDSIZE)), this->x, this->y-1);
                    this->city->getOrganism(this->x, this->y-1)->setPosition(this->x, this->y-1);
                    break;
                case 4:
                    this->city->setOrganism((new Human(this->city, GRIDSIZE, GRIDSIZE)), this->x+1, this->y);
                    this->city->getOrganism(this->x+1, this->y)->setPosition(this->x+1, this->y);
                    break;
                case 6:
                    this->city->setOrganism((new Human(this->city, GRIDSIZE, GRIDSIZE)), this->x, this->y+1);
                    this->city->getOrganism(this->x, this->y+1)->setPosition(this->x, this->y+1);
                    break;
                case 8:
                    this->city->setOrganism((new Human(this->city, GRIDSIZE, GRIDSIZE)), this->x-1, this->y);
                    this->city->getOrganism(this->x-1, this->y)->setPosition(this->x-1, this->y);
                    break;
            }
            this->city->moreHuman();
        }
        this->steps = -1;
    }
    this->targets.clear();
    this->steps += 1;
    this->moved = true;
}

void Human::setTargets() {
    if (this->y != 0 && this->city->getOrganism(this->x, this->y - 1) == nullptr) {
        this->targets.push_back(2);
    }
    if (this->x != this->width-1 && this->city->getOrganism(this->x + 1, this->y) == nullptr) {
        this->targets.push_back(4);
    }
    if (this->y != this->height-1 && this->city->getOrganism(this->x, this->y + 1) == nullptr) {
        this->targets.push_back(6);
    }
    if (this->x != 0 && this->city->getOrganism(this->x - 1, this->y) == nullptr) {
        this->targets.push_back(8);
    }
}

void Human::recruitTargets() {
    if (this->y != 0) {
        if (this->city->getOrganism(this->x, this->y - 1) == nullptr) {
            this->targets.push_back(2);
        }
    }
    if (this->x != this->width-1) {
        if (this->city->getOrganism(this->x + 1, this->y) == nullptr) {
            this->targets.push_back(4);
        }
    }
    if (this->y != this->width-1) {
        if (this->city->getOrganism(this->x, this->y + 1) == nullptr)  {
            this->targets.push_back(6);
        }
    }
    if (this->x != 0) {
        if (this->city->getOrganism(this->x - 1, this->y) == nullptr) {
            this->targets.push_back(8);
        }
    }
}
