#pragma once

class Tile {
public:
	enum class TileType {
		SEA,
		HIT,
		MISSED,
		SHIP_SUBMARINES,
		SHIP_DESTROYER,
		SHIP_CRUISER,
		SHIP_BATTLESHIP,
		SHIP_AIRCRAFT_CARRIER,
		NONE
	};
};