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

	bullets = std::vector<Bullet*>();
	walls = std::vector<Obstacle*>();
	players = std::vector<Boid*>();

	return;
}

//--------------------------------------------------------------------------------------------------------

Scene::~Scene()
{
	//Delete the memory and references.

	for (auto b : bullets)
	{
		delete b;
	}

	for (auto b : players)
	{
		delete b;
	}

	for (auto b : walls)
	{
		delete b;
	}
	return;
}

//Initialise----------------------------------------------------------------------------------------------

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
		if (wall->GetX() == pos.x && wall->GetZ() == pos.z)
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
			walls.push_back(new Obstacle(wallRoot.x, 0.5f, wallRoot.z));
		}

		if (right)
		{
			if (!wallExistsAt(position(wallRoot.x + wallSize, wallRoot.z)))
			{
				walls.push_back(new Obstacle(wallRoot.x + wallSize, 0.5f, wallRoot.z));
			}

			if (down)
			{
				if (!wallExistsAt(position(wallRoot.x + wallSize, wallRoot.z + wallSize)))
				{
					walls.push_back(new Obstacle(wallRoot.x + wallSize, 0.5f, wallRoot.z + wallSize));
				}

				if (!wallExistsAt(position(wallRoot.x, wallRoot.z + wallSize)))
				{
					walls.push_back(new Obstacle(wallRoot.x, 0.5f, wallRoot.z + wallSize));
				}
			}
			else
			{
				if (!wallExistsAt(position(wallRoot.x + wallSize, wallRoot.z - wallSize)))
				{
					walls.push_back(new Obstacle(wallRoot.x + wallSize, 0.5f, wallRoot.z - wallSize));
				}

				if (!wallExistsAt(position(wallRoot.x, wallRoot.z - wallSize)))
				{
					walls.push_back(new Obstacle(wallRoot.x, 0.5f, wallRoot.z - wallSize));
				}
			}
		}
		else
		{
			if (!wallExistsAt(position(wallRoot.x - wallSize, wallRoot.z)))
			{
				walls.push_back(new Obstacle(wallRoot.x - wallSize, 0.5f, wallRoot.z));
			}

			if (down)
			{
				if (!wallExistsAt(position(wallRoot.x - wallSize, wallRoot.z + wallSize)))
				{
					walls.push_back(new Obstacle(wallRoot.x - wallSize, 0.5f, wallRoot.z + wallSize));
				}

				if (!wallExistsAt(position(wallRoot.x, wallRoot.z + wallSize)))
				{
					walls.push_back(new Obstacle(wallRoot.x, 0.5f, wallRoot.z + wallSize));
				}
			}
			else
			{
				if (!wallExistsAt(position(wallRoot.x - wallSize, wallRoot.z - wallSize)))
				{
					walls.push_back(new Obstacle(wallRoot.x - wallSize, 0.5f, wallRoot.z - wallSize));
				}

				if (!wallExistsAt(position(wallRoot.x, wallRoot.z - wallSize)))
				{
					walls.push_back(new Obstacle(wallRoot.x, 0.5f, wallRoot.z - wallSize));
				}
			}
		}

		//If node already contains wall, just skip this wall
	}

	for (auto wall : walls)
	{
		wall->SetOffset(m_ScenarioOffset);
	}
}

void Scene::placePlayers(int numPlayers)
{
	//21 is mapWidth
	int sliceWidth = 21 / numPlayers;

	for (int i = 0; i < numPlayers; i++)
	{
		int count = 0;
		position playerLoc = position(randomInRange(i * sliceWidth, i* sliceWidth + sliceWidth), randomInRange(i * sliceWidth, i* sliceWidth + sliceWidth));
		Boid* player = new Boid(i, playerLoc);
		for (int j = 0; j < walls.size(); j++)
		{
			Collision::MTV trans;
			trans.direction = position();
			trans.magnitude = 0;

			if (Collision::collision(Collision::BoundingBox(playerLoc.x, playerLoc.z, 0.5, 0.5), walls.at(j)->getBoundingBox(), trans))
			{
				position mb = trans.direction * trans.magnitude;
				player->resolveCollision(mb);
				j = 0;
				count++;
				if (count > 20)
				{
					players.clear();
					walls.clear();
					generateMap();
					delete player;
					placePlayers(numPlayers);
					return;
				}
			}
		}

		players.push_back(player);
	}
}

void Scene::SetUpScenario()
{
	//Set no offset, so drawing takes place in corner.
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;

	generateMap();

	placePlayers(2);
}

//Render--------------------------------------------------------------------------------------------------

void Scene::Render()
{
	glClearColor(0.2, 0.5, 1, 0);

	//Draw the Scenario.
	DrawScenario();
}

void Scene::DrawScenario()
{
	for (auto wall : walls)
	{
		wall->Render();
	}

	for (auto player : players)
	{
		player->Render();
	}

	for (auto bullet : bullets)
	{
		bullet->Render();
	}
}

//Update--------------------------------------------------------------------------------------------------

void Scene::Update(int a_deltaTime)
{
	//Update the Scenario.
	UpdateScenario(a_deltaTime);
}

void Scene::UpdateScenario(int a_deltaTime)
{
	keyboardUpdate(0);
	for (int i = 0; i < players.size(); ++i)
	{
		
		players.at(i)->Update(a_deltaTime);

		for (int j = 0; j < walls.size(); j++)
		{
			Collision::MTV trans;
			trans.direction = position();
			trans.magnitude = 0;

			if (Collision::collision(players.at(i)->getBoundingBox(), walls.at(j)->getBoundingBox(), trans))
			{
				position mb = trans.direction * trans.magnitude;
				players.at(i)->resolveCollision(mb);
			}
		}
	}

	for (int i = 0; i < bullets.size(); ++i)
	{
		if (bullets.at(i)->Update(a_deltaTime, players, walls))
		{
			bullets[i] = bullets.back();
			bullets.pop_back();
		}
	}
}

void Scene::keyboardUpdate(int thisPlayerIndex)
{
	//Builds up a string to be passed to the player which is used in the update
	std::string pressedKeys = "";

	if (GetAsyncKeyState('W'))
	{
		pressedKeys += "W";
	}

	if (GetAsyncKeyState('S'))
	{
		pressedKeys += "S";
	}

	if (GetAsyncKeyState('A'))
	{
		pressedKeys += "A";
	}

	if (GetAsyncKeyState('D'))
	{
		pressedKeys += "D";
	}

	if (GetAsyncKeyState(VK_SPACE) && timeSinceLastBullet > 100)
	{
		timeSinceLastBullet = 0;
		pressedKeys += "F";
	}
	timeSinceLastBullet++;

	players.at(thisPlayerIndex)->giveUpdateString(pressedKeys, bullets);
}