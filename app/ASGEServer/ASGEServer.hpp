//
// Created by james on 28/01/2021.
//

#ifndef ASGENETGAME_ASGESERVER_HPP
#define ASGENETGAME_ASGESERVER_HPP
#include <Engine/OGLGame.hpp>
#include "Ship.hpp"
#include "Tile.hpp"
#include "Packet.hpp"
#include <memory>

class GCNetServer;
class ASGEServer : public ASGE::OGLGame
{	
public:
	ASGEServer(const ASGE::GameSettings& settings);
	~ASGEServer() override;
	ASGEServer(const ASGEServer&) = delete;
	ASGEServer& operator=(const ASGEServer&) = delete;

	bool init();
	void update(const ASGE::GameTime& us) override;
	void render(const ASGE::GameTime& us) override;

	void keyHandler(ASGE::SharedEventData data);
	void mouseClickHandler(ASGE::SharedEventData data);
	void mouseMoveHandler(ASGE::SharedEventData data);

	void addShip(const Ship& ship);
	std::tuple<int, int> calculateTileIndex();
	bool isGameStarted();

	void handlePacket(Packet packet);

private:
	std::unique_ptr<GCNetServer> net_server{};

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
	bool startTheGame = false;			//the game will begin when you place all the ships into the map
	bool shipSelected = false;			//this var to test if you select the ship so you can place it into the map
	bool clientReady  = false;
	
	bool gameEnd      = false;
	bool win		  = false;

	bool currentTurn = true; //if false then client turn else server turn

	int score = 0;
	//---- camera -----//
	ASGE::Camera camera{};
};

#endif // ASGENETGAME_ASGESERVER_HPP
