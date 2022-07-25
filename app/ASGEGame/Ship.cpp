#include "Ship.hpp"
#include <Engine/Sprite.hpp>
#include <iostream>

Ship::Ship(std::unique_ptr<ASGE::Sprite> sprite_, Type type, Orientation r) :size{1} {

	this->type = type;
	this->r = r;
	
	sprite = std::move(sprite_);

	switch (this->type)
	{
	case Type::AIRCRAFT_CARRIER:
		size = 5;
		
		sprite->loadTexture("/data/aircraft_carrier.png");
		break;
	case Type::BATTLESHIP:
		size = 4;
		sprite->loadTexture("/data/battleship.png");
		break;
	case Type::CRUISER:
		size = 3;
		sprite->loadTexture("/data/cruiser.png");
		break;
	case Type::DESTROYER:		
		size = 2;
		sprite->loadTexture("/data/destroyer.png");
		break;
	case Type::SUBMARINES:		
		size = 1;
		sprite->loadTexture("/data/submarine.png");
		break;
	default:		
		break;
	}	
	sprite->setGlobalZOrder(100);

	sprite->xPos(100);
	sprite->yPos(100);	
}


Ship::Type Ship::getType() const {
	return type;
}

Ship::Orientation Ship::getOrientation() const {
	return r;
}

int Ship::getSize() const {
	return size;
}




std::tuple<int, int> Ship::getPosition() const {
	return position;
}

void Ship::setPosition(std::tuple<int, int> param) {
	position = param;

	int x = std::get<0>(param);
	int y = std::get<1>(param);
	
	sprite->xPos(100 + 1 + x + x * 30 + 30);
	sprite->yPos(100 + 1 + y + y * 30 + 30);
}

const ASGE::Sprite& Ship::getSprite() const {
	return *sprite.get();
}


bool Ship::isGrab() const {
	return grab;
}

void Ship::setGrab(bool param) {
	this->grab = param;	
}

void Ship::shouldBeGrabbed(std::tuple<int, int> param) {
	
	int xCursor = std::get<0>(param);
	int yCursor = std::get<1>(param);

	int xShip = std::get<0>(position);
	int yShip = std::get<1>(position);

	switch (r) {
	case Orientation::HORIZONTAL:
		if (yCursor == yShip && xCursor >= xShip && xCursor < xShip + size) {
			setGrab(true);			
		}
			
		break;
	case Orientation::VERTICAL:
		if (xCursor == xShip && yCursor >= yShip && yCursor < yShip + size) {			
			setGrab(true);			
		}
			
		break;
	}

	
}
bool Ship::shouldBeReleased() {
	int xShip = std::get<0>(position);
	int yShip = std::get<1>(position);

	switch(r) {
	case Orientation::HORIZONTAL:
		if (xShip >= size && xShip < 8 && yShip >= 0 && yShip < 8 - size) {
			return true;
		}

		break;
	case Orientation::VERTICAL:
		if (xShip >= 0 && xShip < 8 && yShip >= 0 && yShip < 8 - size+1) {
			return true;
		}
		break;
	}
	return false;
}

bool Ship::isReady() const {
	return ready;
}

void Ship::setReady(bool param) {
	ready = param;
}