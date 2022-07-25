#pragma once


struct Packet {
	int x;
	int y;
	int hit;
	/*

	   5: AIRCRAFT_CARRIER,
	   4: BATTLESHIP,
	   3: CRUISER,
	   2: DESTROYER,
	   1: SUBMARINES
	   0: missed the hit
	   -1
   */
	bool ready;
	bool win;
};
