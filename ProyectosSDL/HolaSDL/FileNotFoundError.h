#pragma once
#include "PacmanError.h"
class FileNotFoundError: public PacmanError
{
private:
	string filePath;

public:
	FileNotFoundError(const string& m, const string& path): PacmanError(m), filePath(path)  {}
	virtual string what() {
		return PacmanError::what() + filePath;
	}
};

