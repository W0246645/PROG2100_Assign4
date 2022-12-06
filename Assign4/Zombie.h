#ifndef _Zombie_H
#define _Zombie_H

#include "Organism.h"
#include "GameSpecs.h"

class Zombie : public Organism
{
protected:
    bool humanFound;
    int stepsSinceFood;
public:
	Zombie();
    Zombie( City *city, int width, int height );
	virtual ~Zombie();

    void setTargets();
	void move();
    void breedTargets();
};

#endif
