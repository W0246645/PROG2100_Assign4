#include "GameSpecs.h"
#include "Human.h"

Human::Human() {
    this->species = 'H';
    this->moved = true;
}

Human::Human(City *city, int width, int height) {
    this->species = 'H';
    this->moved = true;
    this->city = city;
    this->width = width;
    this->height = height;
}

Human::~Human() = default;

void Human::move() {
    this->setTargets();
    if (this->targets.size() != 0) {
        int pos = rand() % this->targets.size();
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
