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

	lives = 20;
	spawnPoint = pos;
}

Boid::Boid(int id, position pos) : id(id), pos(pos)
{
	velocity = position();
	spawnPoint = pos;
	rotation = 0;
	colour = Colour(0, 255, 0);

	width = 0.25f;
	height = 0.5f;
	lives = 20;
}

void Boid::Update(double delta)
{
	if (delta != 0)
	{
		velocity /= delta;
	}

	pos += velocity;
	velocity = position(0, 0);
}

void Boid::UpdateState(position p, float rot)
{
	pos = p;
	rotation = rot;
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

void Boid::giveUpdateString(std::string actions, std::vector<Bullet*>& bullets)
{
	if (actions.find("A") != std::string::npos)
	{
		//Rotate left
		rotation += turnSpeed;
	}
	if (actions.find("D") != std::string::npos)
	{
		//Rotate right
		rotation -= turnSpeed;
	}

	if (rotation >= 360)
	{
		rotation -= 360;
	}
	if (rotation <= -360)
	{
		rotation += 360;
	}

	if (actions.find("W") != std::string::npos)
	{
		//Move forward
		float rot = -(rotation * M_PI / 180);
		velocity = position(cos(rot), sin(rot));
		if (fabs(velocity.x) < 0.000001f)
		{
			velocity.x = 0;
		}
		if (fabs(velocity.z) < 0.000001f)
		{
			velocity.z = 0;
		}
		velocity = normalise(velocity) * maxVelocity;
	}
	if (actions.find("S") != std::string::npos)
	{
		//Move backwards
		float rot = -(rotation * M_PI / 180) + M_PI;
		
		velocity = position(cos(rot), sin(rot));
		if (fabs(velocity.x) < 0.000001f)
		{
			velocity.x = 0;
		}
		if (fabs(velocity.z) < 0.000001f)
		{
			velocity.z = 0;
		}
		velocity = normalise(velocity) * maxVelocity;
	}

	if (actions.find("F") != std::string::npos)
	{
		//Fire
		float rot = -(rotation * M_PI / 180);
		position bVel = position(cos(rot), sin(rot));
		bVel = normalise(bVel) * maxVelocity * 2;
		bullets.push_back(new Bullet(id * 10 + bulletIndex++, pos + normalise(bVel), bVel));

		if (bulletIndex > 9)
		{
			bulletIndex = 0;
		}
	}
}

void Boid::hitByBullet()
{
	lives--;
	pos = spawnPoint;
}

void Boid::resolveCollision(position moveBy)
{
	pos += moveBy;
}
