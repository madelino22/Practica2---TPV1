#include "Game.h"
#include <ctime>




Game::Game() {
	//Constructora, crea el window y el renderer de sdl y adem�s las texturas
	srand(time(nullptr));
	//inicializacion de SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Juego con clases", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//si window es nullptr da mensaje de error pero el renderer no
	if (window == nullptr || renderer == nullptr) throw SDLError(SDL_GetError());
	// Creacion de las texturas
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
		try {
			loadSavedGame();
		}
		catch (PacmanError& e) {
			//SI hay error al cargar el nivel se carga el nivel 1 para continuar con la ejecuci�n
			cout << e.what() << "\n Se va a cargar el nivel 1 \n";
			loadLevelFile("..\\Mapas\\level01.dat");
		}
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

//como hay muchos siteios en los qeu se vaa necesitar crear un fantasma y hay c�digo repetido se hace ese m�todo
void Game::storeGhost(Ghost* g) {
	list<GameObject*>::iterator it = objetos.insert(objetos.end(), g);
	g->setItList(it);
	ghosts.push_back(g);

}

void Game::save() {
	SDL_RenderClear(renderer);//limpiar pantalla


	//No funciona el cargado y guardado a varias partidas guardadas, solo funciona a una(error cin)
	/*
	cout << "Introducir n�mero de fichero guardado\n";
	std::string numGuardado;
	getline(cin, numGuardado);
	cin >> numGuardado;
	cout << numGuardado;
	*/
	

	std::ofstream archivoDeGuardado;

	//lo comentado de abajo es lo que deber�a ir al cargar una partida introduciendo su n�mero
	//archivoDeGuardado.open("..\\partidasGuardadas\\partidaGuardada" + to_string(5) + ".txt");
	archivoDeGuardado.open("..\\partidasGuardadas\\partidaGuardadaPrueba.txt");


	//ahora los datos de game
	archivoDeGuardado << nivel << " " << comida << " " << vidas << "\n";

	//guardar los datos del mapa
	mapa->saveToFile(archivoDeGuardado);


	//mira entodos los gameObjects, y si alguno es un gmaeCharacter lo guarda

	archivoDeGuardado << objetosCharacter << "\n";
	for (GameObject* o : objetos) {
		GameCharacter* c = dynamic_cast<GameCharacter*>(o);
		if (c != nullptr) c->saveToFile(archivoDeGuardado);
	}

	

	archivoDeGuardado.close();

}

void Game::loadSavedGame() {
	
	/*
	cout << "introducir numero de la partida guardada";
	int num;
	cin >> num;
	*/
	
	
	string file = "..\\partidasGuardadas\\partidaGuardadaPrueba.txt";
	
	ifstream archivoLeer;
	//archivoLeer.open("..\\partidasGuardadas\\partidaGuardada" + to_string(num) + ".txt");
	archivoLeer.open(file);
	if (!archivoLeer.is_open()) throw(FileNotFoundError("No se ha podido abrir el archivo con la ruta: ", file));
	//cargar los datos de game
	archivoLeer >> nivel >> comida >> vidas;
	if (nivel < 0 || comida < 0 || vidas < 0) {
		throw(FileFormatError("Al intentar cargar de la partida(" + file + ") el nivel, la comida o las vidas tiene un formato erroneo(menor que 0)"));
	}
	//cargar el mapa
	mapa = new GameMap(archivoLeer, this);
	objetos.push_back(mapa);

	//varaible para saber cuantos objetos de tipo character hay en la partida guardada 
	int objetosCh;
	archivoLeer >> objetosCh;
	if (objetosCh < 5) {
		throw(FileFormatError("Al intentar cargar de la partida(" + file + ") se han encontrado menos de 5 objetos de tipo character, lo que es imposible ya que siempre hay al menos cinco(4 ghost + pacman)"));
	}
	//Por �ltimo est�n los fantasmas y el pacman
	loadGameCharacters(archivoLeer, objetosCh);
}

void Game::loadGameCharacters(ifstream& file, int objetosCh) {
	for (int x = 0; x < objetosCh; x++) {
		int tipo;
		file >> tipo;

		if (tipo == 1)
		{//si es algun fantasma
			objetosCharacter++;
			Ghost* g = new Ghost(file, this);
			storeGhost(g);
		}
		else if (tipo == 0) {
			//si es el pacman
			objetosCharacter++;
			pacman = new Pacman(file, this);
			objetos.push_back(pacman);
		}
		else if (tipo == 2) {
			//si es un smartGhost
			//tipoGhost no hace nada, pero como el saveToFile de Ghost tmb guarda un n�mreo para saber su tipo hay que tenerle en cuenta al leer
			int tipoGhost;
			file >> tipoGhost;
			objetosCharacter++;
			Ghost* g = new SmartGhost(file, this);
			storeGhost(g);
		}
	}
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
			//Esto era para la lectura de la partida que se quiere guardar, pero no funciona por el cin
			save();
			pausado = false;
		}
		
	}

	
}
void Game::update() {
	
	for (GameObject* o : objetos) o->update();
	//el borrado de los smartghost en tiempo de ejecuci�n se hace al final del update, ucando todos han hecho sus update,
	//para que no de problemas
	for (auto g : objectsToErase) {
		Ghost* c = dynamic_cast<Ghost*>(*g);
		delete *g;
		objetos.erase(g);
		ghosts.remove(c);
		objetosCharacter--;
	}
	
	objectsToErase.clear();


	//Contral de condiciones de vicotria y paso de nivel
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
	//este �ltimo else se podria comprobar en el respawn del pacman 
	//pero por claridad se deja aqu� y se ejcuta en cada vuelta
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

//Este m�todo checkea si se ha hecho quit y si es otro evento se le pasa al pacman invocando el handleevent
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
	//En este m�todo hay que hacer tambien la destruccion de los nodos de la lsita de objetos
	for (GameObject* o : objetos) o->~GameObject();

	objetosCharacter = 0;
	pacman = nullptr;
	mapa = nullptr;
	
	objetos.clear();
	ghosts.clear();

}

void Game::loadLevelFile(string file)
{
		cout << "Cargado con nuevo metodo \n";
		std::ifstream in;
		in.open(file);
		if (!in.is_open()) throw(FileNotFoundError("No se ha podido abrir el archivo con la ruta: " , file));
		auto cinbuf = std::cin.rdbuf(in.rdbuf());
		int fils, cols; cin >> fils >> cols;

		if (cin.fail()) {
			throw(FileFormatError("Al leer el mapa del archivo(" + file + ") las filas o las columnas no son del tipo esperado(int)"));
		}
		else if (fils < 0 || cols < 0) {
			throw(FileFormatError("Al leer el mapa del archivo(" + file + ") las filas o las columnas no son positivas"));
		}

		mapa = new GameMap(Point2D(0,0), WIN_WIDTH, WIN_HEIGHT, this, textures[0], textures[2], textures[3], fils, cols);
		objetos.push_back(mapa);
		//para cada celda se lee su numero y se a�ade al array de GameMap
		for (int y = 0; y < fils; y++) {
			for (int x = 0; x < cols; x++) {
				int nCelda;
				cin >> nCelda;
				mapa->celdasMapa[y][x] = (MapCell)nCelda;

				//si se a�ade una comida se suma una al contador de la comida para llevar la cuenta en la partida
				// de cuandtas hay al principio y poder ir eliminando
				if ((MapCell)nCelda == Food)comida++;
				if (nCelda == 9) {
					//Creaci�n del pacman

					objetosCharacter++;
					//pacman = new Pacman(mapCordsToSDLPoint(Point2D(y, x)), this, textures[1]);
					pacman = new Pacman(mapCordsToSDLPoint(Point2D(y, x)), mapa->casillaW, mapa->casillaH, this, mapCordsToSDLPoint(Point2D(y, x)), Vector2D(0, 1), textures[1], Point2D(0,10));
					objetos.push_back(pacman);
				}
				else if (nCelda >= 5 && nCelda <= 8) {

					objetosCharacter++;
					Point2D posIni = mapCordsToSDLPoint(Point2D(y, x));
                    Ghost* g = new Ghost(posIni, mapa->casillaW, mapa->casillaH, this, posIni, Point2D(0, 1), textures[1], Point2D(0, (nCelda - 5) * 2));
					storeGhost(g);
				}
				 else if (nCelda == 4) {
					objetosCharacter++;
					Point2D posIni = mapCordsToSDLPoint(Point2D(y, x));
					Ghost* g = new SmartGhost(posIni, mapa->casillaW, mapa->casillaH, this, posIni, Point2D(0, 1), textures[1], Point2D(0, 8), 0);
					storeGhost(g);
				}
			}
		}
}


	
	




//Estos dos metodos es para que el pacman y los fantasmas sepan el tama�o del mapa para renderizarlos y saber si se pueden mover
int Game::GetNFils() const {

	return mapa->fils;
                      }

int Game::GetNCols() const {

	return mapa->cols;
}


//para saber lo que hay en la siguinete casilla dependiendo de la direcci�n que se le pase y la posici�n actual 
bool Game::NextCell(const Vector2D& dir,const Vector2D& pos) const {
	bool celdaVacia = true;
	Vector2D  nextCell = pos;


	//Como el pacman o lo fantasmas no se pueden mover en diagonal, la direccion a la que se van a poder mover
	//va a ser un 1 o -1 en una direcci�n perpendicular
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


	
	//dir va a ser 1 en la direcci�n en la que se vaya, por eso se le multiplica por 10, para que avance 10 pixeles en esa dir
	newPos.SetX(rect.x + dir.GetX() * avanceEnX);
	newPos.SetY(rect.y + dir.GetY() * avanceEnY);

	//en vez de poner que cuando se salgan del limite de la pantalla se muevan al otro l�mite, est� hehco que cuando lleguen
	//al limite del mapa que foman las casillas (mapRect.x + mapa->casillaW * mapa->cols) en este caso el eje x, se muevan, ya que si no se hace as�
	//surge un error
	if (dir.GetX() > 0 && (newPos.GetX() + rect.w) >= mapRect.x + mapa->casillaW * mapa->cols) {
		newPos.SetX(mapRect.x + avanceEnX + rect.w);
	}
	else if (dir.GetX() < 0 && newPos.GetX() <= mapRect.x) {
		newPos.SetX(mapRect.x + mapRect.w - avanceEnX - rect.w);
	}
	else if (dir.GetY() > 0 && (newPos.GetY() + rect.h) >= mapRect.y + mapa->casillaH * mapa->fils) {
		newPos.SetY(mapRect.y + avanceEnY +rect.h);
	}
	else if (dir.GetY() < 0 && newPos.GetY() <= mapRect.y) {
		newPos.SetY(mapRect.y + mapRect.h - avanceEnY - rect.h - 30 );
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


//Este m�todo checkea si el fantasma g1 colisiona con otros y adem�s si lo hace y se deben reproducir se encarga de la reproducci�n
void Game::checkColisionFantasmas(Ghost* g1) {
	
		//si es uno inteligente puede qeu se reproduzca
		SmartGhost* c1 = dynamic_cast<SmartGhost*>(g1);
		if (c1 != nullptr) {
			for (Ghost* g2 : ghosts) {
				//para evitar comprobar la colision con �l mismo
				if (g1 != g2) {
					SDL_Rect rect1 = g1->getDestRect();
					SDL_Rect rect2 = g2->getDestRect();

					//si ha colisionado con otro fantasma, y adem�s los dos tienene el cooldown a menos de 0, ed decir pueden rerpoducirse los dos, se reproducen
					//Adem�s el cooldown coincide con el timpo que tardan en hacaerse adultos por lo que uno peque�o no podr� reproducirse
					if (SDL_HasIntersection(&rect1, &rect2) && g1->getCooldown() <= 0 && g2->getCooldown() <= 0) {
						g1->setCooldown(500);
						g2->setCooldown(500);

						objetosCharacter++;
						SmartGhost* c2 = dynamic_cast<SmartGhost*>(g2);
						//si choca con un smartghost
						if (c2 != nullptr) {
							Point2D posIni = Point2D(rect1.y, rect1.x);
							Ghost* g = new SmartGhost(posIni, mapa->casillaW, mapa->casillaH, this, posIni, Point2D(0, 1), textures[1], Point2D(0, 8), 0);
							storeGhost(g);
						}
						//si el fantasma con el que choca era uno normal
						else {
							Point2D posIni = Point2D(rect1.y, rect1.x);
							//mete al final de la lista de fantasmas el fantasma que toca
							Ghost* g = new Ghost(posIni, mapa->casillaW, mapa->casillaH, this, posIni, Point2D(0, 1), textures[1], Point2D(0, 0));
							storeGhost(g);
						}

					}
				}


			}
		}
	}






void Game::eraseGhost(list<GameObject*>::iterator itList) {
	
	objectsToErase.push_back(itList);
	
}




int Game::distanciaAlPacman(Point2D posFantasma, int& x, int& y) {


	int distancia;
	//el rect es para coneguir facil, con un m�tdoo que ya tenemos la posici�n del pacman
	SDL_Rect rect = pacman->getDestRect();
	int a = rect.x - posFantasma.GetX();
	int b = rect.y - posFantasma.GetY();
	distancia = sqrt(pow(a, 2) + pow(b, 2));

	//estas variables son para que sepa leugo en qu� direcci�n ir, dependiendo de si son positivas o negativas, est� arriba, abajo, derecha o izq
	x = a;
	y = b;

	return distancia;

}