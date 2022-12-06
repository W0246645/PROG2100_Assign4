#ifndef _Organism_H
#define _Organism_H

#include <iostream>
#include <vector>
#include "GameSpecs.h"
#include "City.h"

using namespace std;

class Organism
{
protected:
	int x;
	int y;
	int width;
	int height;
	bool moved;
    int steps;
    char species;
    vector<int> targets;
	City *city;

	enum { WEST, NORTH, EAST, SOUTH, NUM_DIRECTIONS };

public:
	Organism();
	Organism( City *city, int width, int height );
	virtual ~Organism();

	virtual void move() = 0;
	//virtual void spawn() = 0;
	//virtual int getSpecies() = 0; //this could also be coded concrete here
	//virtual void getPosition() = 0;

    virtual void setTargets() = 0;

	void setPosition( int x, int y );
	void endTurn();
    void reset();
	bool isTurn();
    char getSpecies();

	friend ostream& operator<<( ostream &output, Organism *organism );
};

#endif
