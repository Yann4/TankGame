#ifndef _SCENE_H_
#define _SCENE_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <stdlib.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "gl/glut.h"
#include "Obstacle.h"
#include "ObjectTypes.h"
#include "Boid.h"
#include "MathHelper.h"
#include "Bullet.h"

#include "Server.h"
#include "Client.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Scene
{
private:
	position	 m_ScenarioOffset;
	std::vector<Obstacle*> walls;
	std::vector<Boid*> players;
	std::vector<Bullet*> bullets;
	int timeSinceLastBullet = 0;

	bool server;
	Server sInstance;
	Client cInstance;
	int playerIndex;

	WORD latestUpdateTick;
public:
	Scene();
	~Scene();

	bool	Initialise();
	void	Render();
	void	Update(int a_deltaTime);								
	
private:
	void	setupProgram();
	void	SetUpScenario();
	void	setUpFromExtern(std::string state);
	bool	wallExistsAt(position pos);
	void	generateMap();
	void	placePlayers(int numPlayers);

	void	DrawScenario();

	void    UpdateScenario(int a_deltaTime);
	void	UpdateFromServer(std::string state);
	std::string	keyboardUpdate(int thisPlayerIndex);

	std::string	serialiseInitialState();
	std::string serialiseCurrentState();
};

#endif //_SCENE_H_
