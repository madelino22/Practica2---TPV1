#include "Game.h"
#include <ctime>
#include "Error.h"




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




	string cargar;
	cout << "jugar(p) o cargar partida(c)?\n";
	cin >> cargar;
	while (cargar != "p" && cargar != "c") {
		cout << "Introducir valor correcto (p) o (c)\n";
		cin >> cargar;
	}

	objetosCharacter = 0;

	if (cargar == "p") {
		nivel = 1;
		//valor inicial de las variables de la partida,
		//comida se rellena al construir el mapa en leeMapa
		comida = 0;
		vidas = 3;
		loadLevelFile("..\\Mapas\\level01.dat");
	}
	else if (cargar == "c") {
		//Cargar partida
		loadSavedGame();
	}


	pausado = false;

	
}


Game::~Game() {

	for (GameObject* o : objetos) delete o;

	objetos.clear();
	ghosts.clear();
	
	for (uint i = 0; i < NUM_TEXTURES; i++) delete textures[i];
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
}

void Game::storeGhost(Ghost* g) {
	list<GameObject*>::iterator it = objetos.insert(objetos.end(), g);
	g->setItList(it);
	ghosts.push_back(g);

}

void Game::save() {
	SDL_RenderClear(renderer);//limpiar pantalla

	/*
	cout << "Introducir número de fichero guardado\n";
	std::string numGuardado;
	getline(cin, numGuardado);
	cin >> numGuardado;
	cout << numGuardado;
	*/
	

	std::ofstream archivoDeGuardado;
	//archivoDeGuardado.open("..\\partidasGuardadas\\partidaGuardada" + numGuardado + ".txt");
	archivoDeGuardado.open("..\\partidasGuardadas\\partidaGuardadaPrueba.txt");
	archivoDeGuardado << objetosCharacter << "\n";

	for (GameObject* o : objetos) {
		GameCharacter* c = dynamic_cast<GameCharacter*>(o);
		if (c != nullptr) c->saveToFile(archivoDeGuardado);
	}

	//ahora los datos de game
	archivoDeGuardado << nivel << " " << comida << " " << vidas << "\n";

	//guardar los datos del mapa
	mapa->saveToFile(archivoDeGuardado);

	archivoDeGuardado.close();

}

void Game::loadSavedGame() {
	
	/*
	* cout << "introducir numero de la partida guardada";
	int num;
	cin >> num;
	*/
	
	
	ifstream archivoLeer;
	//archivoLeer.open("..\\partidasGuardadas\\partidaGuardada" + num + ".txt");
	archivoLeer.open("..\\partidasGuardadas\\partidaGuardadaPrueba.txt");
	
	

	//varaible para saber cuantos objetos de tipo character hay en la partida guardada 
	int objetosCh;
	archivoLeer >> objetosCh;
	//Primero están los 4 fantasmas y el pacman
	for (int x = 0; x < objetosCh; x++) {
		int tipo;
		archivoLeer >> tipo;
		
		if(tipo == 1)
		{//si es algun fantasma
			objetosCharacter++;
			Ghost* g = new Ghost(archivoLeer, this);
			storeGhost(g);
		}
		else if (tipo == 0) {
			//si es el pacman
			objetosCharacter++;
			pacman = new Pacman(archivoLeer, this);
			objetos.push_back(pacman);
		}
		else if (tipo == 2) {
			objetosCharacter++;
			Ghost* g = new SmartGhost(archivoLeer, this);
			storeGhost(g);
		}
		

	}

	

	//ahora los datos de game
	archivoLeer >> nivel >> comida >> vidas;

	//por último el mapa
	mapa = new GameMap(archivoLeer, this);
	objetos.push_back(mapa);
}

void Game::run() {
	uint32_t startTime, frameTime;
	startTime = SDL_GetTicks();

	while (!exit) { 
		if (!pausado) {
			handleEvents();
			frameTime = SDL_GetTicks() - startTime;
			if (frameTime >= FRAME_RATE) {
				update();
				startTime = SDL_GetTicks();
			}

			render();
		}
		else {
			save();
			pausado = false;
		}
		
	}

	
}
void Game::update() {
	
	for (GameObject* o : objetos) o->update();
	
	//fantasmasChocan();
	
	for (auto g : objectsToErase) {
		
		cout << "hola";
		delete *g;
		objetos.erase(g);
		
	}
	
	objectsToErase.clear();

	//meter las vidas también
	if (comida <= 0) {
		ganado = true;
		
		if (ganado && nivel == 4) {
			exit = true;
			cout << "Has ganado!\n";

		}

		else if (ganado && nivel < 4) {
			ganado = false;
			nivel++;
			string nuevoNivel = "..\\Mapas\\level0" + to_string(nivel) + ".dat";
			destruccionesCambioNivel();
			loadLevelFile(nuevoNivel);
			cout << "salio";
		}
	}
	//este último else se podria comprobar en el respawn del pacman 
	//pero por claridad se deja aquí y se ejcuta en cada vuelta
	else if (vidas <= 0) {
		cout << "Has perdido :(\n";
		exit = true;
	}

}


void Game::render() const {

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	for (GameObject* o : objetos) o->render();
	SDL_RenderPresent(renderer);

}

//Este método checkea si se ha hecho quit y si es otro evento se le pasa al pacman invocando el handleevent
void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !exit) {
		if (event.type == SDL_QUIT) exit = true;
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s) {
			pausado = true;
		}
		else pacman->handleEvents(event);
		
	}
}

void Game::destruccionesCambioNivel() {
	//En este método hay que hacer tambien la destruccion de los nodos de la lsita de objetos
	for (GameObject* o : objetos) o->~GameObject();

	pacman = nullptr;
	mapa = nullptr;

	objetos.clear();
	ghosts.clear();

}
	void Game::loadLevelFile(string file)
	{
		
		cout << "Cargado con nuevo metodo \n";
		std::ifstream in(file);
		if (!in.is_open()) throw(Error("No se encuentra el fichero"));

		auto cinbuf = std::cin.rdbuf(in.rdbuf());


		int fils, cols;

		cin >> fils >> cols;

		mapa = new GameMap(Point2D(0,0), WIN_WIDTH / cols, WIN_HEIGHT / fils, this, textures[0], textures[2], textures[3], fils, cols);
		objetos.push_back(mapa);
		//para cada celda se lee su numero y se añade al array de GameMap
		for (int y = 0; y < fils; y++) {
			for (int x = 0; x < cols; x++) {
				int nCelda;
				cin >> nCelda;
				mapa->celdasMapa[y][x] = (MapCell)nCelda;

				//si se añade una comida se suma una al contador de la comida para llevar la cuenta en la partida
				// de cuandtas hay al principio y poder ir eliminando
				if ((MapCell)nCelda == Food)comida++;
				if (nCelda == 9) {
					//Creación del pacman

					objetosCharacter++;
					//pacman = new Pacman(mapCordsToSDLPoint(Point2D(y, x)), this, textures[1]);
					pacman = new Pacman(mapCordsToSDLPoint(Point2D(y, x)), mapa->width, mapa->height, this, mapCordsToSDLPoint(Point2D(y, x)), Vector2D(0, 1), textures[1], Point2D(0,10));
					objetos.push_back(pacman);
				}
				else if (nCelda >= 5 && nCelda <= 8) {

					objetosCharacter++;
					Point2D posIni = mapCordsToSDLPoint(Point2D(y, x));
                    Ghost* g = new Ghost(posIni, mapa->width, mapa->height, this, posIni, Point2D(0, 1), textures[1], Point2D(0, (nCelda - 5) * 2));
					storeGhost(g);
				}
				 else if (nCelda == 4) {
					objetosCharacter++;
					Point2D posIni = mapCordsToSDLPoint(Point2D(y, x));
					Ghost* g = new SmartGhost(posIni, mapa->width, mapa->height, this, posIni, Point2D(0, 1), textures[1], Point2D(0, 8), 0);
					storeGhost(g);

				}
				 
				

			}
		}
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





bool Game::tryMove(const SDL_Rect& rect, Vector2D dir, Point2D& newPos) {
	SDL_Rect mapRect = mapa->getDestRect();


	
	//dire va a ser 1 en la dirección en la que se vaya, por eso se le multiplica por 10, para que avance 10 pixeles en esa dir
	newPos.SetX(rect.x + dir.GetX() * avanceEnX);
	newPos.SetY(rect.y + dir.GetY() * avanceEnY);



	if (dir.GetX() > 0 && (newPos.GetX() + rect.w) >= mapRect.x + mapRect.w) {
		newPos.SetX(mapRect.x + avanceEnX);
	}
	else if (dir.GetX() < 0 && newPos.GetX() <= mapRect.x) {
		newPos.SetX(mapRect.x + mapRect.w - avanceEnX);
	}
	else if (dir.GetY() > 0 && (newPos.GetY() + rect.h) >= mapRect.y + mapRect.h) {
		newPos.SetY(mapRect.y + avanceEnY);
	}
	else if (dir.GetY() < 0 && newPos.GetY() <= mapRect.y) {
		newPos.SetY(mapRect.y + mapRect.h - avanceEnY);
	}

	SDL_Rect newRect = { newPos.GetX(), newPos.GetY(), rect.w, rect.h };

	return !(mapa->IntersectsWall(newRect));

}




void Game::pacManRespawn() {
	//se lleva al pacMan a la posicion original
	pacman->SetPosAct( pacman->GetPosIni());
	//Se reestablece las dos direcciones para que se mueva a la derecha al iniciar
	pacman->SetDir(0, 1);
	pacman->SetNewDir(0, 1);

	//Se resta las vidas y se le muestra en pantalla al jugador
	vidas--;
	cout << "Vidas: " << vidas << "\n";
}


Texture* Game::getTexture(string name) {
	if (name == "character")
		return textures[1];
	else if (name == "wall")
		return textures[0];
	else if (name == "vitamin")
		return textures[2];
	else if (name == "food")
		return textures[3];
}



void Game::fantasmasChocan() {

	//Se comprueba las colisiones de los fonatasmas entre ellos
	for (Ghost* g1 : ghosts) {
		//si es uno inteligente puede qeu se reproduzca
		SmartGhost* c1 = dynamic_cast<SmartGhost*>(g1);
		if (c1 != nullptr) {
			for (Ghost* g2 : ghosts) {
				//para evitar comprobar la colision con él mismo
				if (g1 != g2) {
					SDL_Rect rect1 = g1->getDestRect();
					SDL_Rect rect2 = g2->getDestRect();

					//si ha colisionado con otro fantasma, y además los dos tienene el cooldown a menos de 0, ed decir pueden rerpoducirse los dos, se reproducen
					//Además el cooldown coincide con el timpo que tardan en hacaerse adultos por lo que uno pequeño no podrá reproducirse
					if (SDL_HasIntersection(&rect1, &rect2) && g1->getCooldown() <= 0 && g2->getCooldown() <= 0) {
						g1->setCooldown(500);
						g2->setCooldown(500);

						objetosCharacter++;
						SmartGhost* c2 = dynamic_cast<SmartGhost*>(g2);
						//si choca con un smartghost
						if (c2 != nullptr) {
							Point2D posIni = Point2D(rect1.y, rect1.x);
							ghosts.push_back(new SmartGhost(posIni, mapa->width, mapa->height, this, posIni, Point2D(0, 1), textures[1], Point2D(0, 8), 0));
							objetos.push_back(ghosts.back());
							ghosts.front()->EscribePosicion();
						}
						//si el fantasma con el que choca era uno normal
						else {
							Point2D posIni = Point2D(rect1.y, rect1.x);
							//mete al final de la lista de fantasmas el fantasma que toca
							ghosts.push_back(new Ghost(posIni, mapa->width, mapa->height, this, posIni, Point2D(0, 1), textures[1], Point2D(0, 0)));
							//busca al final de la lista de fantasmas el último introducido, que justo es el que se acab de introducir y lo añade ala lista objetos, como son punteros los ods no hay problema
							objetos.push_back(ghosts.back());
							ghosts.front()->EscribePosicion();
						}

					}
				}
				
			
			}
		}
	}

}



void Game::eraseGhost(list<GameObject*>::iterator itList) {
	
	objectsToErase.push_back(itList);
	
}
