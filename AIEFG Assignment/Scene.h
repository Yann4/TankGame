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
#include <regex>
#include <conio.h>

class Scene
{
private:
	position	 m_ScenarioOffset;
	std::vector<Obstacle*> walls;
	std::vector<Boid*> players;
	std::vector<Bullet*> bullets;
	int timeSinceLastBullet = 0;

	std::vector<std::string> chat;
	int numLines = 15;
	bool textMode;
	std::string typedString;

	bool server;
	int sendUpdate;
	int shouldSendUpdate = 2;
	Server sInstance;
	Client cInstance;
	int playerIndex;

	unsigned long long latestUpdateTick;
public:
	Scene();
	~Scene();

	bool	Initialise();
	void	Render();
	void	Update(ULONGLONG a_deltaTime);								
	
private:
	void	setupProgram();
	void	SetUpScenario();
	void	setUpFromExtern(std::string state);
	bool	wallExistsAt(position pos);
	void	generateMap();
	void	placePlayers(int numPlayers);

	void	DrawScenario();

	//Returns the number of lines drawn
	int		DrawString(std::string text, position pos);
	bool	type();

	void    UpdateScenario(double a_deltaTime);
	void	UpdateFromServer(std::string state);
	std::string	keyboardUpdate(int thisPlayerIndex);

	std::string	serialiseInitialState();
	std::string serialiseCurrentState();
};

#endif //_SCENE_H_
