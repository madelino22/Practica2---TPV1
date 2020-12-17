#pragma once
#include <stdexcept>
using namespace std;

class PacmanError: public logic_error
{

public:
    PacmanError(const string& m) : logic_error(m) {};
	virtual string what() {

		return logic_error::what();
	}

};

