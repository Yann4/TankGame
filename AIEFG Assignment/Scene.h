#ifndef _SCENE_H_
#define _SCENE_H_

#include <windows.h>
#include <cstdio>
#include <GL\gl.h>
#include <GL\glu.h>
#include "gl/glut.h"
#include "Obstacle.h"
#include "ObjectTypes.h"
#include "Boid.h"
#include "MathHelper.h"
#include "Bullet.h"

#include <vector>

class Scene
{
private:
	position	 m_ScenarioOffset;
	std::vector<Obstacle*> walls;
	std::vector<Boid*> players;
	std::vector<Bullet*> bullets;
	int timeSinceLastBullet = 0;

public:
	Scene();
	~Scene();

	bool	Initialise();
	void	Render();
	void	Update(int a_deltaTime);								
	
private:
	void	SetUpScenario();
	bool	wallExistsAt(position pos);
	void	generateMap();
	void	placePlayers(int numPlayers);

	void	DrawScenario();

	void    UpdateScenario(int a_deltaTime);
	void	keyboardUpdate(int thisPlayerIndex);
};

#endif //_SCENE_H_
