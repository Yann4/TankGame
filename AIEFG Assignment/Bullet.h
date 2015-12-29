#pragma once
#include <vector>

#include "Boid.h"
#include "Obstacle.h"
#include "Collision.h"
#include "ObjectTypes.h"

class Boid;

class Bullet
{
private:
	position pos;
	position velocity;
	
	position size;
	Colour colour;

	float rotation;
public:
	Bullet();
	Bullet(position pos, position velocity);

	bool Update(float delta, std::vector<Boid*>& players, std::vector<Obstacle*>& walls);
	void Render();
};
