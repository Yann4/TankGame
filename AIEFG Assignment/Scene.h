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

#include <vector>

class Scene
{
private:
	position	 m_ScenarioOffset;
	std::vector<Obstacle> walls;

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

	void	DrawScenario();
	void    UpdateScenario(int a_deltaTime);
};

#endif //_SCENE_H_
