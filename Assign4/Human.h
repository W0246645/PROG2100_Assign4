#ifndef _Human_H
#define _Human_H

#include "Organism.h"
#include "GameSpecs.h"

class Human : public Organism
{
public:
	Human();
	virtual ~Human();

	void move();
    void setTargets();
};

#endif
