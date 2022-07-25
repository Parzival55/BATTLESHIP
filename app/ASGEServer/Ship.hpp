#pragma once
#include <tuple>
#include <memory>
#include <Engine/Sprite.hpp>

class Ship {
public:
	enum class Type {
		AIRCRAFT_CARRIER,
		BATTLESHIP,
		CRUISER,
		DESTROYER,
		SUBMARINES
	};

	enum class Orientation {
		VERTICAL,
		HORIZONTAL
	};

	Ship(std::unique_ptr<ASGE::Sprite> sprite, Type type, Orientation r );
	~Ship() = default;
	Ship(const Ship&) = delete;
	Ship& operator=(const Ship&) = delete;

	Type getType() const;
	Orientation getOrientation() const;
	int getSize() const;
	
	std::tuple<int, int> getPosition() const;
	void setPosition(std::tuple<int, int> param);
	
	const ASGE::Sprite& getSprite() const;

	bool isGrab() const;
	void setGrab(bool param);
	void shouldBeGrabbed(std::tuple<int, int> param);
	bool shouldBeReleased();

	bool isReady() const;
	void setReady(bool param);
private:
	Type type;
	Orientation r;
	int size;  // it depends on type, ex. (if AIRCRAFT_CARRIER then size = 5), and so on...	
	std::tuple<int, int> position;

	std::unique_ptr<ASGE::Sprite> sprite;

	bool grab = false;
	bool ready = false;
};