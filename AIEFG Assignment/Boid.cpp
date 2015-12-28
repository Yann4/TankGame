#include "Boid.h"
#include <iostream>


Boid::Boid()
{
	id = -1;
	isTarget = false;
	pos = position();
	velocity = position();

	rotation = 0;
	colour = Colour();

	width = 0;
	height = 0;
}

Boid::Boid(int id, position pos) : id(id), pos(pos)
{
	velocity = position();

	rotation = 0;
	colour = Colour(0, 255, 0);

	width = 0.25f;
	height = 0.5f;
}

void Boid::Update(float delta, const std::vector<BoidInfo>& others)
{
	delta = 0.03;

}

void Boid::Render()
{
	glPushMatrix();
		glTranslatef(pos.x, 0.5f, pos.z);
		glRotatef(rotation, 0, 1, 0);
		glColor3f(colour.r, colour.g, colour.b);
		glutSolidTeapot(0.25);
	glPopMatrix();
}

void Boid::UpdateLocation(position steeringForce, float delta)
{
	//Update position based on old velocity and accelleration (mass is 1)
	pos += (velocity * delta) + ((steeringForce * (delta * delta) * 0.5f));

	//Update velocity
	velocity += steeringForce * delta;
	velocity = truncate(velocity, maxVelocity);

	//Update rotation
	position heading = normalise(velocity);
	float deg = atan2(-heading.z, heading.x) * 180 / M_PI;
	rotation = deg;
}

void Boid::resolveCollision(position moveBy)
{
	pos += moveBy;
}
