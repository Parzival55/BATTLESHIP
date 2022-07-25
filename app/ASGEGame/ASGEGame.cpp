#include "ASGEGame.hpp"
#include "GCNetClient.hpp"


ASGENetGame::ASGENetGame(const ASGE::GameSettings& settings) : OGLGame(settings)
{			
	//========= Input ==========//
	inputs->use_threads = false;
	inputs->addCallbackFnc(ASGE::EventType::E_KEY, &ASGENetGame::keyHandler, this);
	inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_MOVE, &ASGENetGame::mouseMoveHandler, this);
	inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_CLICK, &ASGENetGame::mouseClickHandler, this);

	//========= Init Entities =====//			
	std::unique_ptr<Ship> ship1 = std::make_unique<Ship>(std::move(renderer->createUniqueSprite()),
		Ship::Type::AIRCRAFT_CARRIER, Ship::Orientation::VERTICAL);
	ship1->setPosition(std::make_tuple<int, int>(0, 10));

	std::unique_ptr<Ship> ship2 = std::make_unique<Ship>(std::move(renderer->createUniqueSprite()),
		Ship::Type::BATTLESHIP, Ship::Orientation::VERTICAL);
	ship2->setPosition(std::make_tuple<int, int>(2, 10));

	std::unique_ptr<Ship> ship3 = std::make_unique<Ship>(std::move(renderer->createUniqueSprite()),
		Ship::Type::CRUISER, Ship::Orientation::VERTICAL);
	ship3->setPosition(std::make_tuple<int, int>(4, 10));

	std::unique_ptr<Ship> ship4 = std::make_unique<Ship>(std::move(renderer->createUniqueSprite()),
		Ship::Type::DESTROYER, Ship::Orientation::VERTICAL);
	ship4->setPosition(std::make_tuple<int, int>(6, 10));

	std::unique_ptr<Ship> ship5 = std::make_unique<Ship>(std::move(renderer->createUniqueSprite()),
		Ship::Type::SUBMARINES, Ship::Orientation::VERTICAL);	
	ship5->setPosition(std::make_tuple<int, int>(8, 10));

	ships.push_back(std::move(ship1));
	ships.push_back(std::move(ship2));
	ships.push_back(std::move(ship3));
	ships.push_back(std::move(ship4));
	ships.push_back(std::move(ship5));

	cursor = renderer->createUniqueSprite();
	cursor->loadTexture("/data/cursor.png");
	cursor->setGlobalZOrder(100);
	cursor->xPos(100);
	cursor->yPos(100);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			tiles[i][j] = Tile::TileType::SEA;
			tiles_op[i][j] = Tile::TileType::SEA;
		}
	}

	

	//========= Map ==========//
	map = renderer->createUniqueSprite();
	map->loadTexture("/data/mod.png");
	map->xPos(100);    //idx: 0 --> 7
	map->yPos(100);    //idx: 0 ---> 7

	map_op = renderer->createUniqueSprite();
	map_op->loadTexture("/data/mod.png");
	map_op->xPos(566);  //idx: 15 --> 22
	map_op->yPos(100);  //idx: 0 ---> 7

	missed = renderer->createUniqueSprite();
	missed->loadTexture("/data/missed.png");
	missed->setGlobalZOrder(100);

	hit = renderer->createUniqueSprite();
	hit->loadTexture("/data/hit.png");
	hit->setGlobalZOrder(100);
	
	//========= Camera ==========//
	camera.resize(1024, 768);
	camera.lookAt({ 1024 / 2, 768 / 2 });

	//======= Init Socket =======//
	client = std::make_unique<GCNetClient>(this);
	client->connect("127.0.0.1", 31276);

	//======= toggleFPS =======//
	toggleFPS();
}


ASGENetGame::~ASGENetGame()
{
	//this->inputs->unregisterCallback(key_callback_id);
}

void ASGENetGame::update(const ASGE::GameTime& us)
{
	if (gameEnd) {
		return;
	}

	client->update(us.deltaInSecs());
	auto CursorPos = calculateTileIndex();
	int xCursor = std::get<0>(CursorPos);
	int yCursor = std::get<1>(CursorPos);

	
	if (isGameStarted() && serverReady) {
		if (!currentTurn) return;

		if (score >= 55) {
			gameEnd = true;
			win = true;
			client->send(Packet{ -1, -1, -1,false,true });
		}
	}

	if (mouse_buttons[ASGE::MOUSE::MOUSE_BTN1]) {
		

		if (xCursor >= 15 && xCursor < 23 && yCursor >= 0 && yCursor <= 8) {
			client->send(Packet{ (xCursor - 15), yCursor, -1,true,false});
		}
		

		
		if (!shipSelected) {
			for (const auto& param : ships) {
				if (!param->isGrab() && !param->isReady() && !shipSelected) {
					param->shouldBeGrabbed(CursorPos);
					shipSelected = param->isGrab();
					if (shipSelected)
						break;
				}
			}
		}
		else {

			if (xCursor >= 0 && xCursor < 8 && yCursor >= 0 && yCursor <= 8) {
				for (const auto& param : ships) {
					if (param->isGrab() && param->shouldBeReleased()) {
						param->setGrab(false);
						param->setReady(true);
						shipSelected = false;

						addShip(*param);

						break;
					}
				}
			}
		}//else

		mouse_buttons[ASGE::MOUSE::MOUSE_BTN1] = false;
	}
	
	
}

void ASGENetGame::render(const ASGE::GameTime& /*us*/) {
	auto view = camera.getView();
	renderer->setViewport({ 0, 0, 1024, 768 });
	renderer->setProjectionMatrix(view);

	//render the ships
	for (const auto& param : ships) {
		renderer->render(param->getSprite());
	}

	// render cursor
	cursor->xPos(100 + 1 + 31 * std::get<0>(cursorIdx) + 31);
	cursor->yPos(100 + 1 + 31 * std::get<1>(cursorIdx) + 31);
	if (!shipSelected)
		renderer->render(*cursor);

	//render the map
	renderer->render(*map);
	renderer->render(*map_op);
	



	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tiles_op[i][j] == Tile::TileType::MISSED) {
				missed->xPos(566 + 1 + j + j * 30 + 30);
				missed->yPos(100 + 1 + i + i * 30 + 30);
				renderer->render(*missed);
			}
			else if (tiles_op[i][j] == Tile::TileType::HIT) {
				hit->xPos(566 + 1 + j + j * 30 + 30);
				hit->yPos(100 + 1 + i + i * 30 + 30);
				renderer->render(*hit);
			}

			if (tiles[i][j] == Tile::TileType::MISSED) {
				missed->xPos(100 + 1 + j + j * 30 + 30);
				missed->yPos(100 + 1 + i + i * 30 + 30);
				renderer->render(*missed);
			}
			else if (tiles[i][j] == Tile::TileType::HIT) {
				hit->xPos(100 + 1 + j + j * 30 + 30);
				hit->yPos(100 + 1 + i + i * 30 + 30);
				renderer->render(*hit);
			}
		}
	}
	if (serverReady && isGameStarted()) {
		if (currentTurn) {
			ASGE::Text test = ASGE::Text{ renderer->getDefaultFont(), "Your Turn" };
			test.setPositionX(380);
			test.setPositionY(70);
			test.setColour(ASGE::COLOURS::ALICEBLUE);
			renderer->render(test);
		}
		else {
			ASGE::Text test = ASGE::Text{ renderer->getDefaultFont(), "Enemy Turn" };
			test.setPositionX(380);
			test.setPositionY(70);
			test.setColour(ASGE::COLOURS::BISQUE);
			renderer->render(test);
		}
	}
	



	if (!gameEnd) {
		if (!startTheGame) {
			ASGE::Text test = ASGE::Text{ renderer->getDefaultFont(), "Set Ships On It's Board..." };
			test.setPositionX(150);
			test.setPositionY(90);
			renderer->render(test);
		}
		else {
			ASGE::Text test = ASGE::Text{ renderer->getDefaultFont(), "Battle..." };
			test.setPositionX(150);
			test.setPositionY(90);
			renderer->render(test);
		}
	}
	else {
		if (win) {
			ASGE::Text test = ASGE::Text{ renderer->getDefaultFont(), "... You Win ..." };
			test.setPositionX(250);
			test.setPositionY(90);
			renderer->render(test);
		}
		else {
			ASGE::Text test = ASGE::Text{ renderer->getDefaultFont(), "... You Lose" };
			test.setPositionX(250);
			test.setPositionY(90);
			renderer->render(test);
		}
	}
	
}

/*
void ASGENetGame::fixedUpdate(const ASGE::GameTime& us)
{
  Game::fixedUpdate(us);
}
*/


//=================================================//
//				keyHandler 
//=================================================//

void ASGENetGame::keyHandler(ASGE::SharedEventData data)
{
	const auto* key = dynamic_cast<const ASGE::KeyEvent*>(data.get());

	if (key->key == ASGE::KEYS::KEY_ESCAPE)
	{
		signalExit();
	}
}

//=================================================//
//				mouseClickHandler 
//=================================================//

void ASGENetGame::mouseClickHandler(ASGE::SharedEventData data) {

	const auto* mouse_event = dynamic_cast<const ASGE::ClickEvent*>(data.get());
	int button = mouse_event->button;
	int action = mouse_event->action;

	if (button == ASGE::MOUSE::MOUSE_BTN1 &&
		action == ASGE::MOUSE::BUTTON_PRESSED)
	{
		mouse_buttons[button] = true;
	}

	if (button == ASGE::MOUSE::MOUSE_BTN1 &&
		action == ASGE::MOUSE::BUTTON_RELEASED)
	{
		mouse_buttons[button] = false;
	}

	/*
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tiles[i][j] == Tile::TileType::SEA)
				std::cout << "S";
		}
	}*/
}

//=================================================//
//				mouseMoveHandler 
//=================================================//
#include <iostream>
void ASGENetGame::mouseMoveHandler(ASGE::SharedEventData data) {

	const auto* mouse_event = dynamic_cast<const ASGE::MoveEvent*>(data.get());
	
	cursorPos = std::tuple<double, double>(mouse_event->xpos, mouse_event->ypos);
	cursorIdx = calculateTileIndex();
	if (shipSelected) {
		for (const auto& param : ships) {
			if (param->isGrab()) {
				param->setPosition(calculateTileIndex());
			}
		}
	}

}

//=================================================//
//				addShip 
//=================================================//
// add ship to the map
void ASGENetGame::addShip(const Ship& ship) {

	auto pos = ship.getPosition();
	Ship::Orientation r = ship.getOrientation();

	Tile::TileType tiletype = Tile::TileType::NONE;
	Ship::Type temp = ship.getType();
	switch (temp) {
	case Ship::Type::AIRCRAFT_CARRIER:
		tiletype = Tile::TileType::SHIP_AIRCRAFT_CARRIER;
		break;
	case Ship::Type::BATTLESHIP:
		tiletype = Tile::TileType::SHIP_BATTLESHIP;
		break;
	case Ship::Type::CRUISER:
		tiletype = Tile::TileType::SHIP_CRUISER;
		break;
	case Ship::Type::DESTROYER:
		tiletype = Tile::TileType::SHIP_DESTROYER;
		break;
	case Ship::Type::SUBMARINES:
		tiletype = Tile::TileType::SHIP_SUBMARINES;
		break;
	}

	switch (r) {
	case Ship::Orientation::HORIZONTAL:

		for (int i = 0; i < ship.getSize(); i++) {
			tiles[std::get<1>(pos)][std::get<0>(pos) + i] = tiletype;
		}
		break;
	case Ship::Orientation::VERTICAL:
		for (int i = 0; i < ship.getSize(); i++) {
			tiles[std::get<1>(pos) + i][std::get<0>(pos)] = tiletype;
		}
		break;
	}

}


//=================================================//
//				calculateTileIndex 
//=================================================//
// calculate tile index from cursor position
std::tuple<int, int> ASGENetGame::calculateTileIndex() {
	int x = std::get<0>(cursorPos) - 100;
	int index_x = x / 31 - 1;

	int y = std::get<1>(cursorPos) - 100;
	int index_y = y / 31 - 1;

	return std::tuple<int, int>(index_x, index_y);
}

//=================================================//
//				isGameStarted 
//=================================================//

bool ASGENetGame::isGameStarted() {
	if (!startTheGame) {
		for (const auto& param : ships) {
			if (!param->isReady()) {
				return false;
			}
		}
		
		startTheGame = true;
		client->send(Packet{ -1, -1, -1, true, false});
	}
	return startTheGame;
}


//=================================================//
//				handlePacket 
//=================================================//

void ASGENetGame::handlePacket(Packet packet) {

	
	if (!serverReady) {
		if (packet.ready) {
			serverReady = true;			
		}
	}
	else {
		if (packet.win) {
			gameEnd = true;
			return;
		}
	
		int hit = 0;
		if (packet.hit == -1) {			

			currentTurn = true;

			switch (tiles[packet.y][packet.x]) {			
			case Tile::TileType::SHIP_AIRCRAFT_CARRIER:										
				tiles[packet.y][packet.x] = Tile::TileType::HIT;
				hit = 5;
				break;
			case Tile::TileType::SHIP_BATTLESHIP:												
				tiles[packet.y][packet.x] = Tile::TileType::HIT;
				hit = 4;
				break;
			case Tile::TileType::SHIP_CRUISER:												
				tiles[packet.y][packet.x] = Tile::TileType::HIT;
				hit = 3;
				break;
			case Tile::TileType::SHIP_DESTROYER:												
				tiles[packet.y][packet.x] = Tile::TileType::HIT;
				hit = 2;
				break;
			case Tile::TileType::SHIP_SUBMARINES:												
				tiles[packet.y][packet.x] = Tile::TileType::HIT;
				hit = 1;
				break;
			case Tile::TileType::SEA:				
				tiles[packet.y][packet.x] = Tile::TileType::MISSED;
				hit = 0;
				break;
			default:				
				return;				
			}
			client->send(Packet{ packet.x, packet.y, hit, true, false });
		}
		else {	
			currentTurn = false;

			if (packet.hit == 0) {
				tiles_op[packet.y][packet.x] = Tile::TileType::MISSED;
			} else if (packet.hit > 0) {

				tiles_op[packet.y][packet.x] = Tile::TileType::HIT;				
				score += packet.hit;
			}
		}

	}
}