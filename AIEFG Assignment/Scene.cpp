#include "Scene.h"
#include "gl/gl.h"
#include "gl/glu.h"
#include <string>

//--------------------------------------------------------------------------------------------------------

Scene::Scene()
{
	//Current scenario
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;
	return;
}

//--------------------------------------------------------------------------------------------------------

Scene::~Scene()
{
	//Delete the memory and references.
	return;
}

//--------------------------------------------------------------------------------------------------------

bool Scene::Initialise()
{
	glEnable(GL_DEPTH_TEST);

	SetUpScenario();

	return true;
}

bool Scene::wallExistsAt(position pos)
{
	for (auto wall : walls)
	{
		if (wall.GetX() == pos.x && wall.GetZ() == pos.z)
		{
			return true;
		}
	}
	return false;
}

void Scene::generateMap()
{
	unsigned int numWalls = 20;
	unsigned int wallClumpWidth = 2;
	unsigned int areaWidth = 20;
	unsigned int areaHeight = 20;
	unsigned int wallSize = 1;

	for (int i = 0; i < numWalls; ++i)
	{
		//Pick a random position
		position wallRoot = position(randomInRange(0, areaWidth), randomInRange(0, areaHeight));
		//Clamp it to the grid (Centre of each square is at .5)
		wallRoot = position(round(wallRoot.x), round(wallRoot.z));

		bool right = true;
		bool down = true;
		
		//Decide which direction to expand in
		if (wallRoot.x + wallClumpWidth <= areaWidth + 0.5f)
		{
			//Expand right
			right = true;
			if (wallRoot.z + wallClumpWidth <= areaHeight + 0.5f)
			{
				down = true;
			}
			else
			{
				//Expand up
				down = false;
			}
		}
		else
		{
			//Expand left
			right = false;
			if (wallRoot.z + wallClumpWidth <= areaHeight + 0.5f)
			{
				//Expand down
				down = true;
			}
			else
			{
				//Expand up
				down = false;
			}
		}
		
		//Expand!

		if (!wallExistsAt(wallRoot))
		{
			walls.push_back(Obstacle(wallRoot.x, 0.5f, wallRoot.z));
		}

		if (right)
		{
			if (!wallExistsAt(position(wallRoot.x + wallSize, wallRoot.z)))
			{
				walls.push_back(Obstacle(wallRoot.x + wallSize, 0.5f, wallRoot.z));
			}

			if (down)
			{
				if (!wallExistsAt(position(wallRoot.x + wallSize, wallRoot.z + wallSize)))
				{
					walls.push_back(Obstacle(wallRoot.x + wallSize, 0.5f, wallRoot.z + wallSize));
				}

				if (!wallExistsAt(position(wallRoot.x, wallRoot.z + wallSize)))
				{
					walls.push_back(Obstacle(wallRoot.x, 0.5f, wallRoot.z + wallSize));
				}
			}
			else
			{
				if (!wallExistsAt(position(wallRoot.x + wallSize, wallRoot.z - wallSize)))
				{
					walls.push_back(Obstacle(wallRoot.x + wallSize, 0.5f, wallRoot.z - wallSize));
				}

				if (!wallExistsAt(position(wallRoot.x, wallRoot.z - wallSize)))
				{
					walls.push_back(Obstacle(wallRoot.x, 0.5f, wallRoot.z - wallSize));
				}
			}
		}
		else
		{
			if (!wallExistsAt(position(wallRoot.x - wallSize, wallRoot.z)))
			{
				walls.push_back(Obstacle(wallRoot.x - wallSize, 0.5f, wallRoot.z));
			}

			if (down)
			{
				if (!wallExistsAt(position(wallRoot.x - wallSize, wallRoot.z + wallSize)))
				{
					walls.push_back(Obstacle(wallRoot.x - wallSize, 0.5f, wallRoot.z + wallSize));
				}

				if (!wallExistsAt(position(wallRoot.x, wallRoot.z + wallSize)))
				{
					walls.push_back(Obstacle(wallRoot.x, 0.5f, wallRoot.z + wallSize));
				}
			}
			else
			{
				if (!wallExistsAt(position(wallRoot.x - wallSize, wallRoot.z - wallSize)))
				{
					walls.push_back(Obstacle(wallRoot.x - wallSize, 0.5f, wallRoot.z - wallSize));
				}

				if (!wallExistsAt(position(wallRoot.x, wallRoot.z - wallSize)))
				{
					walls.push_back(Obstacle(wallRoot.x, 0.5f, wallRoot.z - wallSize));
				}
			}
		}

		//If node already contains wall, just skip this wall
	}

	for (auto wall : walls)
	{
		wall.SetOffset(m_ScenarioOffset);
	}
}

//--------------------------------------------------------------------------------------------------------

void Scene::Render()
{
	glClearColor(0.2, 0.5, 1, 0);
	for (auto wall : walls)
	{
		wall.Render();
	}

	//Draw the Scenario.
	DrawScenario();
}

//--------------------------------------------------------------------------------------------------------

void Scene::Update(int a_deltaTime)
{
	//Update the Scenario.

	UpdateScenario(a_deltaTime);
}

//--------------------------------------------------------------------------------------------------------

void Scene::DrawScenario()
{
}

//Methods to set up pointer arrays to all the wall pieces.
void Scene::SetUpScenario()
{
	//Set no offset, so drawing takes place in corner.
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;

	generateMap();
}

//--------------------------------------------------------------------------------------------------------

void Scene::UpdateScenario(int a_deltaTime)
{
}