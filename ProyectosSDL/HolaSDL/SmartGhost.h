#pragma once
#include "Ghost.h"
class SmartGhost: public Ghost
{
private:
	int edad;

public:

	virtual void update();
	virtual void saveToFile(std::ofstream& file);



};

