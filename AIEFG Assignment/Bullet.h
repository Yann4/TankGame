#pragma once
#include <vector>

#include "Boid.h"
#include "Obstacle.h"
#include "Collision.h"
#include "ObjectTypes.h"
#include "Actions.h"

class Boid;

class Bullet
{
private:
	int id;
	position pos;
	position velocity;
	
	position size;
	Colour colour;

	float rotation;
	int lifetime;
public:
	Bullet();
	Bullet(int id, position pos, position velocity);

	bool Update(float delta, std::vector<Boid*>& players, std::vector<Obstacle*>& walls);
	void UpdateState(position p, float rot);
	void Render();

	//"B:[ID],[PosX],[PosZ],[Rotation]"
	//B:[2chars],[6chars],[6chars],[6chars]
	std::string getInfoString() {
		return "B:" + Actions::intToFLString(id, 2) + "," + std::to_string(pos.x).substr(0, 6) + "," + std::to_string(pos.z).substr(0, 6)
			+ "," + std::to_string(rotation).substr(0, 6);
	}

	int getID() { return id; }
};
