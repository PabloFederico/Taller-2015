/*
 * Sprite.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/Sprite.h"

/********************************************************************************/
Sprite::Sprite(int cant_Direcciones, Uint32 cant_Img_Distintas, Imagen* imagen,
				SDL_Rect posicion, Escenario* escen, Coordenada c_ceros){
	this->cant_Direcciones = cant_Direcciones;
	this->cant_Img_Distintas = cant_Img_Distintas;
	this->imagen = imagen;
	this->posicion = posicion;

	this->regPos.posX_player =(float)posicion.x;
	this->regPos.posY_player =(float)posicion.y;
	this->regPos.x_anterior = posicion.x;
	this->regPos.y_anterior = posicion.y;
	this->escenario = escen;
	this->coord_ceros = c_ceros;

	this->frames = new SDL_Rect*[cant_Direcciones];

	for (int i = 0; i < cant_Direcciones; i++){
		this->frames[i] = new SDL_Rect[cant_Img_Distintas];
	}
	this->fps = 30;
	this->delay = 0;
	this->miliseg_inicial = SDL_GetTicks();
	this->tiempoTranscurridoAlFinDeCiclo = 0;

	this->cargarFrames();

	this->direccion = 0;
	this->indexSpriteActual = 0;

	this->enMovimiento = false;

	this->frameActual = this->frames[this->direccion][this->indexSpriteActual];
}

/********************************************************************************/
void Sprite::cargarFrames(){
	int posX;
	int posY = 0;
	int ancho = this->imagen->getPixelsX() / this->cant_Img_Distintas;
	int alto = this->imagen->getPixelsY() / this->cant_Direcciones;
	for (int i = 0; i < this->cant_Direcciones; i++){
		posX = 0;
		for (int j = 0; j < this->cant_Img_Distintas; j++){
			SDL_Rect frame;
			frame.x = posX;
			frame.y = posY;
			frame.w = ancho;
			frame.h = alto;
			this->frames[i][j] = frame;
			posX = posX + ancho;
		}
		posY = posY + alto;
	}
}
/********************************************************************************/
Imagen* Sprite::getImagen(){
	return this->imagen;
}

/********************************************************************************/
SDL_Rect Sprite::getPosicion(){
	return this->posicion;
}

/********************************************************************************/
Coordenada Sprite::getPosPies() {
	return Coordenada( posicion.x + (posicion.w / 2), posicion.y + posicion.h );
}

/********************************************************************************/
void Sprite::setPosX(int x){
	int diferencia = rectangulos[0].x_ini - posicion.x;
	this->posicion.x = x;
	for (unsigned i = 0; i < rectangulos.size(); i++){
		rectangulos[i].x_ini = x + diferencia;
	}
}

void Sprite::setPosY(int y){
	int diferencia = rectangulos[0].y_ini - posicion.y;
	this->posicion.y = y;
	for (unsigned i = 0; i < rectangulos.size(); i++){
		rectangulos[i].y_ini = y + diferencia;
	}
}

/********************************************************************************/
void Sprite::mover(int cant_x, int cant_y){
	this->posicion.x += cant_x;
	this->posicion.y += cant_y;
	regPos.x_anterior += cant_x;
	regPos.y_anterior += cant_y;
	regPos.posX_player += cant_x;
	regPos.posY_player += cant_y;
	for (unsigned i = 0; i < rectangulos.size(); i++){
		rectangulos[i].x_ini += cant_x;
		rectangulos[i].y_ini += cant_y;
	}

	for(unsigned i = 0; i < caminoARecorrer.size(); i++){
		caminoARecorrer[i].x += cant_x;
		caminoARecorrer[i].y += cant_y;
	}
}

/********************************************************************************/
void Sprite::setDireccion(int direccion){
	if (this->direccion != direccion){
		this->direccion = direccion;
		this->indexSpriteActual = 0;
		this->frameActual = this->frames[direccion][0];
	}
}

/********************************************************************************/
bool Sprite::estaEnMovimiento(){
	return this->enMovimiento;
}

/********************************************************************************/
void Sprite::activarMovimiento(bool valor){
	this->enMovimiento = valor;
}

/********************************************************************************/
void Sprite::efectuarMovimiento(){
	if (this->delay && this->indexSpriteActual == 0){
		tiempoTranscurridoAlFinDeCiclo += currentTime();
		if (tiempoTranscurridoAlFinDeCiclo > this->delay){
			this->indexSpriteActual++;
			tiempoTranscurridoAlFinDeCiclo = 0;
		}
	}else{
		if (this->indexSpriteActual < this->cant_Img_Distintas)
			this->indexSpriteActual++;
		else this->indexSpriteActual = 0;
	}

	this->frameActual = this->frames[this->direccion][this->indexSpriteActual % cant_Img_Distintas];
	resetTime();
}

/********************************************************************************/
SDL_Rect Sprite::getFrameActual(){
	return this->frameActual;
}

/********************************************************************************/
void Sprite::setFps(int fps){
	this->fps = fps;
}

/********************************************************************************/
int Sprite::getFps(){
	return this->fps;
}

/********************************************************************************/
void Sprite::setDelay(int delay){
	this->delay = delay;
}

/********************************************************************************/
void Sprite::resetTime(){
	this->miliseg_inicial = SDL_GetTicks();
}

/********************************************************************************/
int Sprite::currentTime(){
	int currentTime = SDL_GetTicks();
	return currentTime - this->miliseg_inicial;
}

/********************************************************************************/
void Sprite::agregarRectangulo(Rectangulo rectangulo){
	this->rectangulos.push_back(rectangulo);
}

/********************************************************************************/
bool Sprite::checkColision(Sprite* otro){
	bool colision = false;
	/* Para cada rectángulo que conforma el sprite */
	for (unsigned i = 0; i < rectangulos.size() && !colision ; i++){
		/* Para cada rectángulo que conforma el sprite con el que se quiere comprobar la colisión */
		for (unsigned j = 0; j < otro->rectangulos.size() && ! colision; j++){
			if (rectangulos[i].existeColision(otro->rectangulos[j])){
				colision = true;
			}
		}
	}
	return colision;
}

/********************************************************************************/
Coordenada Sprite::coordPixelSprite() {
	int x = this->getPosicion().x + this->getPosicion().w / 2;
	int y = this->getPosicion().y + this->getPosicion().h / 2;
	return Coordenada(x,y);
}

/********************************************************************************/
void Sprite::setearNuevoCamino(Camino nuevoCamino, Coordenada coord_ceros){
	this->coord_ceros = coord_ceros;
	this->caminoARecorrer.clear();
	if (nuevoCamino.size() == 0) {
		this->caminoARecorrer = nuevoCamino.v;
		return;
	}

	try {
		nuevoCamino.convertirTilesAPixeles(coord_ceros);
		Coordenada c_prox_punto = nuevoCamino[0];
		// Coordenada para posicionar el sprite.
		c_prox_punto.x -= this->getPosicion().w / 2;
		c_prox_punto.y -= this->getPosicion().h;// / 2;

		Direccion direccion = Calculador::calcularDireccion(c_prox_punto, coordPixelSprite());
		this->setDireccion(direccion);
		this->activarMovimiento(true);

		this->caminoARecorrer = nuevoCamino.v;
	} catch ( FueraDeEscenario &e ) { /* Hacer algo!? */
	} catch ( TileEstaOcupado &e ) { }
}

/********************************************************************************/
vector<Coordenada> Sprite::getCaminoARecorrer(){
	return this->caminoARecorrer;
}

/********************************************************************************/
void Sprite::quitarPrimeraCoordenada(){
	this->caminoARecorrer.erase(this->caminoARecorrer.begin());
}

/********************************************************************************/
bool Sprite::quedaCaminoPorRecorrer(){
	return (caminoARecorrer.size() > 0);
}

/********************************************************************************/
// PRE: queda camino por recorrer.
bool Sprite::revisarCamino(Coordenada c_punto_actual) {
	Coordenada c_prox_punto = this->getCaminoARecorrer()[0];
	try {
		Coordenada   c_prox_tile = Calculador::tileParaPixel(  c_prox_punto, coord_ceros);
		if (!(this->escenario->tileEsOcupable(c_prox_tile))) {
			Coordenada c_pix_destino = getCaminoARecorrer().back();
			Coordenada c_tile_destino = Calculador::tileParaPixel(c_pix_destino, coord_ceros);
			// Si el tile que fue ocupado por otra entidad es el de destino, quedarse donde está.
			if (c_prox_tile == c_tile_destino) throw TileEstaOcupado();
			// Caso contrario, con el siguiente tile del camino ahora ocupado, crear nuevo camino para esquivarlo.
			else setearNuevoCamino(Calculador::obtenerCaminoMin(escenario, c_punto_actual, c_pix_destino, coord_ceros), coord_ceros);
		}
		return true;
	} catch ( FueraDeEscenario &e ) {} catch ( TileEstaOcupado &e ) {}
	setearNuevoCamino(Camino(), coord_ceros);
	return false;
}

/********************************************************************************/
void Sprite::acomodar(){
	this->frameActual = this->frames[this->direccion][cant_Direcciones-1];
}


/********************************************************************************/
void Sprite::update(int vel_personaje) {
	if (this->quedaCaminoPorRecorrer()) {

		/*	- calcular distancia al sig punto
		 *  - si es mayor a 1, seguir moviendo la posicion
		 *    sino cambiar al siguiente punto del camino (analizando la dirección)
		 *  - si ya no quedan camino a recorrer, cambiar estado de activar movimiento y acomodar
		 * */
		Coordenada c_prox_pixel = this->getCaminoARecorrer()[0];

		///* Esta condición es para que el chabón se ubique en el centro del tile,
		// * pero no se aplica para el último punto destino. */
		//if (this->getCaminoARecorrer().size() > 1){
		c_prox_pixel.x -= this->getPosicion().w / 2;
		c_prox_pixel.y -= this->getPosicion().h;// / 2;
		//}

		float distancia = Calculador::calcularDistanciaEntrePixeles(Coordenada(this->regPos.posX_player, this->regPos.posY_player), c_prox_pixel);

		if (distancia > 1){
            if (this->regPos.posX_player != c_prox_pixel.x) {
            	float x_result = (this->regPos.posX_player - ((this->regPos.posX_player - c_prox_pixel.x) / distancia) * vel_personaje  * 0.05);
            	this->setPosX(int(x_result));
            	this->regPos.posX_player = x_result;
            }
            if (this->regPos.posY_player != c_prox_pixel.y) {
                float y_result = (this->regPos.posY_player - ((this->regPos.posY_player - c_prox_pixel.y) / distancia) * vel_personaje * 0.05);
                this->setPosY(int(y_result));
                this->regPos.posY_player = y_result;
            }
		} else {
			/* guardar última coordenada para desocuparla y cambiar a la próxima */
			Coordenada c_ult_punto = this->getCaminoARecorrer()[0];
			this->quitarPrimeraCoordenada();

			/* Seteamos la dirección para el siguiente punto. */
			if (this->getCaminoARecorrer().size() > 0){
				try {
					//this->escenario->desocuparTile(Calculador::tileParaPixel(c_ult_punto, coord_ceros));
					if (this->revisarCamino(c_ult_punto)) {
						Coordenada c_prox_punto = this->getCaminoARecorrer()[0];
						c_prox_punto.x -= this->getPosicion().w / 2;
						c_prox_punto.y -= this->getPosicion().h / 2;
						Direccion direccion = Calculador::calcularDireccion(c_prox_punto, coordPixelSprite());
						this->setDireccion(direccion);
					}
				} catch ( FueraDeEscenario &e ) { setearNuevoCamino(Camino(), coord_ceros); }
			}
		}

	} else {
		/* Cuando se deja de mover, se debería quedar en
		 * una posición firme correspondiente a su dirección. */
		 this->activarMovimiento(false);
		 this->acomodar();
	}

           // bool hayColision = DetectorDeColisiones::verificarColisiones(this,juego->getSpritesEntidades());
           // if (hayColision) this->activarMovimiento(false);
}

/********************************************************************************/
Sprite::~Sprite() {
	for (int i = 0; i < this->cant_Direcciones; i++){
		delete[] this->frames[i];
	}
	delete[] this->frames;
}

