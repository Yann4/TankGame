#include "Bullet.h"

Bullet::Bullet()
{
	pos = position();
	velocity = position();
	size = position();
	colour = Colour();
	rotation = 0;
}

Bullet::Bullet(position pos, position vel) : pos(pos), velocity(vel)
{
	size = position(1, 0.5);
	colour = Colour();

	position heading = normalise(velocity);
	rotation = atan2(-heading.z, heading.x) * 180 / M_PI;
}

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