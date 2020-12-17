#pragma once
#include "PacmanError.h"
class FileFormatError: public PacmanError
{

public:
	FileFormatError(const string& m) : PacmanError(m) {};
	virtual string what() {
		return PacmanError::what();
	}
};

