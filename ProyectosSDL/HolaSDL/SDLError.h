#pragma once
#include "PacmanError.h"
class SDLError: public PacmanError
{
public:
	SDLError(const string& m) : PacmanError(m) {};
	virtual string what() {
		return PacmanError::what();
	}
};

