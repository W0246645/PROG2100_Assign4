#include "GameSpecs.h"
#include "Organism.h"

Organism::Organism() {

}

Organism::Organism(City *city, int width, int height) {
    this->city = city;
    this->width = width;
    this->height = height;
}

Organism::~Organism() {

}

void Organism::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void Organism::endTurn() {
    this->moved = true;
}

void Organism::reset() {
    this->moved = false;
    this->targets.clear();
}

bool Organism::isTurn() {
    return !this->moved;
}

char Organism::getSpecies() {
    return this->species;
}

ostream &operator<<(ostream &output, Organism *organism) {
    return output << organism->species;
}
