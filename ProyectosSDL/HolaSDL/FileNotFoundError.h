#pragma once
#include "PacmanError.h"
class FileNotFoundError: public PacmanError
{
private:
	string filePath;

public:
	//Este error además devuelve la direccion de la carpeta a la que se no se puede acceder
	FileNotFoundError(const string& m, const string& path): PacmanError(m), filePath(path)  {}
	virtual string what() {
		return PacmanError::what() + filePath;
	}
};

