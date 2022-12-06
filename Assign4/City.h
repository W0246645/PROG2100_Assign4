#ifndef _CITY_H
#define _CITY_H

#include <iostream>
#include "GameSpecs.h"

using namespace std;

class Organism;

class City
{
protected:
	Organism *grid[GRIDSIZE][GRIDSIZE];
    int generation;
    int numHumans;
    int numZombies;

public:
	City();
	virtual ~City();

	Organism *getOrganism( int x, int y );
	void setOrganism( Organism *organism, int x, int y );

	void move();
    void reset();
    bool hasDiversity();
    void lessZombie();
    void lessHuman();
    void moreHuman();
    void moreZombie();

	friend ostream& operator<<( ostream &output, City &city );

};

#endif

