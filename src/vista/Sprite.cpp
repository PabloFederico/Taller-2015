/*
 * Sprite.cpp
 *
 *  Created on: 16 de sept. de 2015
 *      Author: emanuel
 */

#include "../vista/Sprite.h"
#include "../utils/ContenedorDeSonidos.h"

/********************************************************************************/
Sprite::Sprite(int cant_Direcciones, Uint32 cant_Img_Distintas, Imagen* imagen,
			SDL_Rect posicion, Escenario* escen, Coordenada c_ceros, Entidad* ent): entidad(ent) {
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

	this->cargarFrames(cant_Direcciones, cant_Img_Distintas, imagen, frames);
	imagenPetrificada = NULL;

	this->direccion = 0;
	this->indexSpriteActual = 0;

	this->enMovimiento = false;

	this->frameActual = this->frames[this->direccion][this->indexSpriteActual];
}

/********************************************************************************/
void Sprite::cargarFrames(int dir, int dif_frames, Imagen* image, SDL_Rect** frames){
	int posX;
	int posY = 0;
	int ancho = image->getPixelsX() / dif_frames;
	int alto = image->getPixelsY() / dir;
	for (int i = 0; i < dir; i++){
		posX = 0;
		for (int j = 0; j < dif_frames; j++){
			SDL_Rect frame;
			frame.x = posX;
			frame.y = posY;
			frame.w = ancho;
			frame.h = alto;
			frames[i][j] = frame;
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
void Sprite::agregarImagenPetrificada(Imagen* image){
	this->imagenPetrificada = image;
}

/********************************************************************************/
void Sprite::cambiarTexture(){
	Imagen* image_aux = this->imagen;
	imagen = imagenPetrificada;
	imagenPetrificada = image_aux;
}

/********************************************************************************/
Entidad* Sprite::getEntidad(){
	return this->entidad;
}

/********************************************************************************/
Coordenada Sprite::getPosPies() {
	//if (entidad->getTipo() == ARQUERO)
	//	return Coordenada( posicion.x + posicion.w/2, posicion.y + posicion.h * 0.75 );
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
Direccion Sprite::getDireccion(){
	return (Direccion)direccion;
}

/********************************************************************************/
bool Sprite::estaEnMovimiento(){
	return (this->enMovimiento || entidad->getEstado() != QUIETO);
}

/********************************************************************************/
void Sprite::activarMovimiento(bool valor){
	this->enMovimiento = valor;
	if (valor)
		this->entidad->cambioEstado(CAMINANDO);
	else this->entidad->finalizaAccion();
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
	if (nuevoCamino.empty()) {
		this->caminoARecorrer = nuevoCamino.v;
		//this->activarMovimiento(false);
		return;
	}

	try {
		nuevoCamino.convertirTilesAPixeles(coord_ceros);
		Coordenada c_prox_punto = nuevoCamino[0];
		// Coordenada para posicionar el sprite.
		c_prox_punto.x -= this->getPosicion().w / 2;
		c_prox_punto.y -= this->getPosicion().h;

		Coordenada c_tile_actual = Calculador::tileParaCualquierPixel(coordPixelSprite(),coord_ceros);
		Coordenada c_tile_prox = Calculador::tileParaCualquierPixel(c_prox_punto,coord_ceros);
		if (c_tile_actual != c_tile_prox){
			Direccion direccion = Calculador::calcularDireccionEntrePuntosAdyacentes(c_tile_actual,c_tile_prox);
			this->setDireccion(direccion);
		}
		//Direccion direccion = Calculador::calcularDireccion(c_prox_punto, coordPixelSprite());
		//this->setDireccion(direccion);
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
		Coordenada c_prox_tile = Calculador::tileParaCualquierPixel(c_prox_punto, coord_ceros);
		c_prox_tile.x = Calculador::ChequeoDeBorde(escenario->getDimension().first,  c_prox_tile.x);
		c_prox_tile.y = Calculador::ChequeoDeBorde(escenario->getDimension().second, c_prox_tile.y);
		if (!(this->escenario->tileEsOcupable(c_prox_tile))) {
			Coordenada c_pix_destino = getCaminoARecorrer().back();
			Coordenada c_tile_destino = Calculador::tileParaCualquierPixel(c_pix_destino, coord_ceros);
			c_tile_destino.x = Calculador::ChequeoDeBorde(escenario->getDimension().first,  c_tile_destino.x);
			c_tile_destino.y = Calculador::ChequeoDeBorde(escenario->getDimension().second, c_tile_destino.y);
			// Si el tile que fue ocupado por otra entidad es el de destino, quedarse donde está.
			if (c_prox_tile == c_tile_destino) throw TileEstaOcupado();
			// Caso contrario, con el siguiente tile del camino ahora ocupado, crear nuevo camino para esquivarlo.
			else {
				setearNuevoCamino(Calculador::obtenerCaminoMin(escenario, c_punto_actual, c_pix_destino, coord_ceros), coord_ceros);
			}
		}
		if (quedaCaminoPorRecorrer())
			return true;
	} catch ( FueraDeEscenario &e ) {} catch ( TileEstaOcupado &e ) {}
	setearNuevoCamino(Camino(), coord_ceros);
	return false;
}

/********************************************************************************/
void Sprite::acomodar(){
	this->frameActual = this->frames[this->direccion][0];
}


/********************************************************************************/
void Sprite::update(int vel_personaje, Mix_Chunk* sonido_caminar) {
	if (this->quedaCaminoPorRecorrer()) {

		// Activo sonido del movimiento:
		Mix_PlayChannel(1, sonido_caminar, 0);
		/*	- calcular distancia al sig punto
		 *  - si es mayor a 1, seguir moviendo la posicion
		 *    sino cambiar al siguiente punto del camino (analizando la dirección)
		 *  - si ya no quedan camino a recorrer, cambiar estado de activar movimiento y acomodar
		 * */
		Coordenada c_prox_pixel = this->getCaminoARecorrer()[0];

		c_prox_pixel.x -= this->getPosicion().w / 2;
		c_prox_pixel.y -= this->getPosicion().h;

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
						Coordenada c_tile_actual = Calculador::tileParaCualquierPixel(coordPixelSprite(),coord_ceros);
						Coordenada c_tile_prox = Calculador::tileParaCualquierPixel(c_prox_punto,coord_ceros);
						if (c_tile_actual != c_tile_prox){
							Direccion direccion = Calculador::calcularDireccionEntrePuntosAdyacentes(c_tile_actual,c_tile_prox);
							this->setDireccion(direccion);
						}
						//Direccion direccion = Calculador::calcularDireccion(c_prox_punto, coordPixelSprite());
						//this->setDireccion(direccion);
					}
				} catch ( FueraDeEscenario &e ) { setearNuevoCamino(Camino(), coord_ceros); }
			} else // Se acaba de terminar el camino//, que es de un solo paso.
				throw PasoCompletado(entidad->getIDJug());
		}
	} else {
		/* Cuando se deja de mover, se debería quedar en
		 * una posición firme correspondiente a su dirección. */
		 this->activarMovimiento(false);
		 this->acomodar();
		 // Paro el sonido:
		 Mix_HaltChannel(1);
	}

           // bool hayColision = DetectorDeColisiones::verificarColisiones(this,juego->getSpritesEntidades());
           // if (hayColision) this->activarMovimiento(false); todo?
}

/********************************************************************************/
Sprite::~Sprite() {
	if (!entidad->esUnidad()){
		for (int i = 0; i < this->cant_Direcciones; i++){
			delete[] this->frames[i];
		}
		delete[] this->frames;
	}
}

