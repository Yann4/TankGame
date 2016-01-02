#include "Bullet.h"

Bullet::Bullet()
{
	pos = position();
	velocity = position();
	size = position();
	colour = Colour();
	rotation = 0;
	lifetime = 0;
}

Bullet::Bullet(int id, position pos, position vel) : pos(pos), velocity(vel), id(id)
{
	size = position(1, 0.5);
	colour = Colour();

	position heading = normalise(velocity);
	rotation = atan2(-heading.z, heading.x) * 180 / M_PI;
	lifetime = 0;
}

void Bullet::UpdateState(position p, float rot)
{
	pos = p;
	rotation = rot;
}

//Returns true if the bullet should be removed
bool Bullet::Update(float delta, std::vector<Boid*>& players, std::vector<Obstacle*>& walls)
{
	pos += velocity / delta;

	Collision::MTV mtv;
	Collision::BoundingBox bb(pos.x, pos.z, size.x / 2, size.z / 2);

	for (auto player : players)
	{
		if (Collision::collision(bb, player->getBoundingBox(), mtv))
		{
			player->hitByBullet();
			return true;
		}
	}

	for (auto wall : walls)
	{
		if (Collision::collision(bb, wall->getBoundingBox(), mtv))
		{
			return true;
		}
	}

	if (++lifetime >= 200)
	{
		return true;
	}

	return false;
}

void Bullet::Render()
{
	glPushMatrix();
		glTranslatef(pos.x, 0.5f, pos.z);
		glRotatef(rotation, 0, 1, 0);
		glColor3f(colour.r, colour.g, colour.b);
		glutSolidCube(0.25);
	glPopMatrix();
}