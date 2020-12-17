
#include "SDL.h"
#include "SDL_image.h"
#include "checkML.h"
#include <iostream>
#include "Game.h"
#include "checkML.h"
#include "Pacman.h"
#include "PacmanError.h"


using namespace std;

using uint = unsigned int;



int main2() {
	
	try {
		Game game = Game();
		game.run();
	}
	catch (PacmanError& e) {
		cout << e.what() << "\n";
	}
	
	
	//para poder leer la consola al final
	system("pause");
	return 0;
}



int main(int argc, char* argv[]){
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	main2();
	return 0;
}