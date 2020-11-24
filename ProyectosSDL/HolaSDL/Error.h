#pragma once
#include <string>
using namespace std;
	class Error {
	protected:
		string mensaje;
	public:
		Error(string const& m) : mensaje(m) {};
		const string& getMensaje() const {
			return mensaje;
		};
	};

