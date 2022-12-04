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

public:
	City();
	virtual ~City();

	Organism *getOrganism( int x, int y );
	void setOrganism( Organism *organism, int x, int y );

	void move();
    void reset();

	friend ostream& operator<<( ostream &output, City &city );

};

#endif

