#include "Game.h"
#include <ctime>
#include "Error.h"


struct Texturas {
	string file[4]{ "..\\Imagenes\\wall3.png", "..\\Imagenes\\characters1.png", "..\\Imagenes\\burguer1.png", "..\\Imagenes\\food2.png" };
	int fils[4]{1, 4, 1, 1};
	int cols[4]{1, 14, 1, 1};
};

Game::Game() {
	//Constructora, crea el window y el renderer de sdl y además las texturas

	srand(time(nullptr));
	// We first initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Juego con clases", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == nullptr || renderer == nullptr) throw string("Error loading SDL window or renderer");
	// We now create the textures
	Texturas texturas;
	for (uint i = 0; i < NUM_TEXTURES; i++) {
		textures[i] = new Texture(renderer);
		textures[i]->load(texturas.file[i], texturas.fils[i], texturas.cols[i]);
	}

	
	//valor inicial de las variables de la partida,
	//comida se rellena al construir el mapa en leeMapa
	comida = 0;
	vidas = 3;

	LeeMapa();
}
Game::~Game() {

	delete pacman;
	delete mapa;//invoca al destructor de mapa y todo lo que lleva consigo
	for (Ghost* g : ghosts) delete g;
	for (uint i = 0; i < NUM_TEXTURES; i++) delete textures[i];
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
}


void Game::run() {
	uint32_t startTime, frameTime;
	startTime = SDL_GetTicks();

	while (!exit) { 
		handleEvents();
		frameTime = SDL_GetTicks() - startTime;
		if (frameTime >= FRAME_RATE) {
			update();
			startTime = SDL_GetTicks();

		}
		
		render();

		//meter las vidas también
		if (comida <= 0) {
			exit = true;
			ganado = true;
		}
		//este último else se podria comprobar en el respawn del pacman 
		//pero por claridad se deja aquí y se ejcuta en cada vuelta
		else if (vidas <= 0) {
			exit = true;
		}
	}

	if (ganado)
		cout << "Has ganado!\n";
	else
		cout << "Has perdido :(\n";
}
void Game::update() {

	pacman->update();
	for(Ghost* g : ghosts)g->update();
	
}


void Game::render() const {

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	mapa->render();
	pacman->render();
	for (Ghost* g : ghosts) {
		g->render();
	}
	SDL_RenderPresent(renderer);

}

//Este método checkea si se ha hecho quit y si es otro evento se le pasa al pacman invocando el handleevent
void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !exit) {
		if (event.type == SDL_QUIT) exit = true;
		else pacman->handleEvents(event);
		
	}
}



void Game::LeeMapa() {
#ifndef DOMJUDGE
	std::ifstream in("..\\Mapas\\level01.dat");
	if (!in.is_open()) throw(Error("No se encuentra el fichero"));
	
	auto cinbuf = std::cin.rdbuf(in.rdbuf()); 
#endif 

	int fils, cols;

	cin >> fils >> cols;

	mapa = new GameMap(fils, cols,this, textures[0], textures[2], textures[3]);
	//para cada celda se lee su numero y se añade al array de GameMap
	for (int x = 0; x < fils; x++) {
		for (int y = 0; y < cols; y++) {
			int nCelda;
			cin >> nCelda;
			mapa->celdasMapa[x][y] = (MapCell)nCelda;

			//si se añade una comida se suma una al contador de la comida para llevar la cuenta en la partida
			// de cuandtas hay al principio y poder ir eliminando
			if ((MapCell)nCelda == Food)comida++;
			if (nCelda == 9) {
				//Creación del pacman
				pacman = new Pacman(Vector2D(y, x), this, textures[1]);
			}
			else if (nCelda >= 5 && nCelda<= 8) {
				ghosts[nCelda- 5] = new Ghost(Vector2D(y, x), this, textures[1], nCelda - 5);
				//Es la misma textura que el pacman ya que es una spritesheet
				//En el método render ya se seleccionara cual es
			}
			
		}
	}

	
    //Este bucle es de debug, para ver en la consola si se ha leido bien de fichero
	//estos x y van cambiados ya que en los bucles van del revés
	/*for (int y = 0; y < fils; y++) {
		for (int x = 0; x < cols; x++) {
			cout << mapa->celdasMapa[y][x] << " ";
		}
		cout << "\n";
	}
	*/
	

#ifndef DOMJUDGE // para dejar todo como estaba al principio
	std::cin.rdbuf(cinbuf);
#endif
}


//Estos dos metodos es para que el pacman y los fantasmas sepan el tamaño del mapa para renderizarlos y saber si se pueden mover
int Game::GetNFils() const {

	return mapa->fils;
}

int Game::GetNCols() const {

	return mapa->cols;
}


//para saber lo que hay en la siguinete casilla dependiendo de la dirección que se le pase y la posición actual 
bool Game::NextCell(const Vector2D& dir,const Vector2D& pos) const {
	bool celdaVacia = true;
	Vector2D  nextCell = pos;


	//Como el pacman o lo fantasmas no se pueden mover en diagonal, la direccion a la que se van a poder mover
	//va a ser un 1 o -1 en una dirección perpendicular
	if (dir.GetY() == -1) {
		nextCell.SetY((pos.GetY() - 1 + mapa->fils) % mapa->fils);
		if (mapa->celdasMapa[nextCell.GetY()][nextCell.GetX()] == Wall) celdaVacia = false;
	}
	else if (dir.GetY() == 1) {
		nextCell.SetY((pos.GetY() + 1 + mapa->fils) % mapa->fils);
		if (mapa->celdasMapa[nextCell.GetY()][nextCell.GetX()] == Wall) celdaVacia = false;
	}
	else if (dir.GetX() == - 1) {
		nextCell.SetX((pos.GetX() - 1 + mapa->fils) % mapa->fils);
		if (mapa->celdasMapa[nextCell.GetY()][nextCell.GetX()] == Wall) celdaVacia = false;
	}
	else if (dir.GetX() == + 1) {
		nextCell.SetX((pos.GetX() + 1 + mapa->fils) % mapa->fils);
		if (mapa->celdasMapa[nextCell.GetY()][nextCell.GetX()] == Wall) celdaVacia = false;
	}


	return celdaVacia;
}

void Game::pacManRespawn() {
	//se lleva al pacMan a la posicion original
	pacman->posAct = pacman->posIni;
	//Se reestablece las dos direcciones para que se mueva a la derecha al iniciar
	pacman->dir.SetY(0); pacman->dir.SetX(1);
	pacman->newDir.SetY(0); pacman->newDir.SetX(1);

	//Se resta las vidas y se le muestra en pantalla al jugador
	vidas--;
	cout << "Vidas: " << vidas << "\n";
}


