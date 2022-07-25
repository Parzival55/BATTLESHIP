//
// Created by huxy on 23/02/2020.
//

#pragma once

#include <Engine/OGLGame.hpp>
#include "Ship.hpp"
#include "Tile.hpp"
#include "Packet.hpp"

class GCNetClient;

class ASGENetGame : public ASGE::OGLGame
{
public:
	explicit ASGENetGame(const ASGE::GameSettings& settings);
	~ASGENetGame() override;

	ASGENetGame(const ASGENetGame&) = delete;
	ASGENetGame& operator=(const ASGENetGame&) = delete;

	
	void update(const ASGE::GameTime& us) override;
	void render(const ASGE::GameTime& us) override;
	//void fixedUpdate(const ASGE::GameTime& us) override;


	void keyHandler(ASGE::SharedEventData data);
	void mouseClickHandler(ASGE::SharedEventData data);
	void mouseMoveHandler(ASGE::SharedEventData data);

	void addShip(const Ship& ship);
	std::tuple<int, int> calculateTileIndex();
	bool isGameStarted();

	void handlePacket(Packet packet);
private:
	std::unique_ptr<GCNetClient> client{};
	std::string key_callback_id{}; /**< Key Input Callback ID. */
	std::map<int, bool> mouse_buttons;

	//------ texture ---//
	std::unique_ptr<ASGE::Sprite> map = nullptr;
	std::unique_ptr<ASGE::Sprite> cursor = nullptr;

	std::unique_ptr<ASGE::Sprite> map_op = nullptr; //opponent map

	std::unique_ptr<ASGE::Sprite> missed = nullptr; //hit failure 
	std::unique_ptr<ASGE::Sprite> hit = nullptr; //hit success
	//------ gameplay variables ---//	
	
	std::vector< std::unique_ptr< Ship > > ships;
	Tile::TileType tiles_op[8][8];
	Tile::TileType tiles[8][8];
	std::tuple<double, double> cursorPos;     //cursor postion
	std::tuple<int, int> cursorIdx;		//cursor index
	bool startTheGame = false;			//the game will begin when you place all the ships into the map ...
	bool serverReady  = false;			//and the server is ready also 
	bool shipSelected = false;			//this var to test if you select the ship so you can place it into the map

	bool gameEnd = false;
	bool win = false;

	bool currentTurn = false; //if false then server turn else client turn

	int score = 0;
	//---- camera -----//
	ASGE::Camera camera{};
};
