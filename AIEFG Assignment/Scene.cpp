#include "Scene.h"
#include "gl/gl.h"
#include "gl/glu.h"


//--------------------------------------------------------------------------------------------------------

Scene::Scene()
{
	//Current scenario
	m_ScenarioOffset.x = 0.0f;
	m_ScenarioOffset.z = 0.0f;

	bullets = std::vector<Bullet*>();
	walls = std::vector<Obstacle*>();
	players = std::vector<Boid*>();

	latestUpdateTick = 0;

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

void Scene::setupProgram()
{
	std::cout << "Setup as server or client? (S/C): ";
	std::string input;
	
	bool set = false;
	while (!set)
	{
		std::cin >> input;
		if (input.find("S") != std::string::npos || input.find("s") != std::string::npos)
		{
			server = true;
			set = true;
		}
		else if (input.find("C") != std::string::npos || input.find("c") != std::string::npos)
		{
			server = false;
			set = true;
		}
	}

	if (!server)
	{
		std::cout << "Enter server name: ";
		std::string name;
		std::cin >> name;
		std::cout << "Enter port number: ";
		std::cin >> input;
		
		cInstance.Initialise(name, input);
	}
	else
	{
		sInstance.turnOn();
	}
}

bool Scene::Initialise()
{
	glEnable(GL_DEPTH_TEST);

	setupProgram();

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

	if (server)
	{
		generateMap();
		placePlayers(2);
		std::string mess = serialiseInitialState();
		sInstance.setWelcomeMessage(mess);
		sInstance.broadcast(Message(mess, -1));
	}else
	{
		int initialised = 0;
		while (initialised < 2)
		{
			std::string initialiseMessage;
			auto messages = cInstance.getMessages();
			//recieve string to pass to this function

			while (!messages.empty())
			{
				std::string message = messages.front();
				messages.pop();
				if (!message.empty() && message.find("\\I\\") != std::string::npos)
				{
					setUpFromExtern(message);
					initialised++;
				}

				if (!message.empty() && message.find("ID") != std::string::npos)
				{
					playerIndex = atoi(message.substr(message.find("ID") + 3, 1).c_str());
					initialised++;
				}
			}	
		}
	}
}

std::vector<std::string> split(const std::string &s, char delim) 
{
	std::vector<std::string> elems;

	std::stringstream ss(s);
	std::string item;
	
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}

	return elems;
}

void Scene::setUpFromExtern(std::string state)
{
	std::vector<std::string> tokens = split(state, '\\');
	
	for (auto w : walls)
	{
		delete w;
	}
	walls.clear();

	for (auto p : players)
	{
		delete p;
	}
	players.clear();

	for (std::string token : tokens)
	{
		if (token.empty())
		{
			continue;
		}
		
		//Format of wall token is "W:[PosX],[PosZ]\" where posx and posz are both 4 characters long
		//This means that posX starts at position 2 and posZ starts at position 7 in the substring
		if (token.at(0) == 'W')
		{
			walls.push_back(new Obstacle(atof(token.substr(2, 4).c_str()), 0.5, atof(token.substr(7, 4).c_str())));
		}
		//"P:[id],[xPos],[zPos],[rotation]"
		//Initial state of player is at rotation 0, so can ignore that part of the string
		if (token.at(0) == 'P')
		{
			std::cout << token << std::endl;
			players.push_back(new Boid(atoi(token.substr(2, 1).c_str()), position((float)atof(token.substr(4, 6).c_str()), (float)atof(token.substr(11, 4).c_str()))));
		}
	}
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

	if (server)
	{
		sInstance.broadcast(Message(serialiseCurrentState(), -1));
	}
	else
	{
		auto messages = cInstance.getMessages();
		std::string latestUpdate;

		while (!messages.empty())
		{
			latestUpdate = messages.front();
			messages.pop();

			if (!latestUpdate.empty())
			{
				UpdateFromServer(latestUpdate);
			}
		}
		cInstance.update();
	}
	
}

void Scene::UpdateScenario(int a_deltaTime)
{
	if (!server)
	{
		std::string input = keyboardUpdate(playerIndex);
		if (!input.empty())
		{
			cInstance.pushMessage("K:" + input);
		}
	}
	else
	{
		std::queue<std::string> playerInputs = sInstance.getMessages();

		while (!playerInputs.empty())
		{
			std::string inputString = playerInputs.front();
			playerInputs.pop();
			int index = atoi(inputString.substr(0, 1).c_str());

			if (inputString.find("K:") != std::string::npos)
			{
				players.at(index)->giveUpdateString(inputString.substr(5, std::string::npos), bullets);
			}
		}
	}

	for (int i = 0; i < players.size(); ++i)
	{
		
		players.at(i)->Update(a_deltaTime);
		
		//Make sure stays on screen. Should definitely be prettier - not very extensible
		position pPos = players.at(i)->getInfo().pos;
		if (pPos.x >= 21)
		{
			players.at(i)->resolveCollision(position(21 - pPos.x, 0));
		}
		if (pPos.x <= 0)
		{
			players.at(i)->resolveCollision(position(-pPos.x, 0));
		}

		if (pPos.z >= 21)
		{
			players.at(i)->resolveCollision(position(0, 21 - pPos.z));
		}
		if (pPos.z <= 0)
		{
			players.at(i)->resolveCollision(position(0, -pPos.z));
		}

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

void Scene::UpdateFromServer(std::string state)
{
	std::vector<std::string> tokens = split(state, '\\');

	for (std::string token : tokens)
	{
		if (token.empty())
		{
			continue;
		}

		if (token.at(0) == 'P')
		{
			int id = atoi(token.substr(2, 1).c_str());
			position p = position(atof(token.substr(4, 6).c_str()), atof(token.substr(11, 6).c_str()));
			float rotation = atof(token.substr(18, 6).c_str());
			std::cout << token << std::endl;
			players.at(id)->UpdateState(p, rotation);
		}
		else if (token.at(0) == 'B')
		{
			int id = atoi(token.substr(2, 2).c_str());
			position p = position(atof(token.substr(5, 6).c_str()), atof(token.substr(12, 6).c_str()));
			float rotation = atof(token.substr(19, 6).c_str());
			
			for (Bullet* b : bullets)
			{
				if (b->getID() == id)
				{
					b->UpdateState(p, rotation);
				}
			}
		}
		else if (token.at(0) == 'U')
		{
			unsigned long long frameNumber = std::strtoull(token.substr(1, std::string::npos).c_str(), '\0', 10);
			if (frameNumber < latestUpdateTick)
			{
				std::cout << "Throw away update" << std::endl;
				break;
			}
			latestUpdateTick = frameNumber;
		}
	}
}

std::string Scene::keyboardUpdate(int thisPlayerIndex)
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

	return pressedKeys;
}

std::string Scene::serialiseInitialState()
{
	std::string message = "\\I\\";

	//Wall data in format "\W:xx.x,zz.z\"
	for (Obstacle* w : walls)
	{
		message += "W:" + std::to_string(w->GetX()).substr(0, 4) + "," + std::to_string(w->GetZ()).substr(0, 4) + "\\";
	}

	for (Boid* p : players)
	{
		message += p->getInfoString() + "\\";
	}

	return message;
}

std::string Scene::serialiseCurrentState()
{
	std::string message = "\\U";

	message += std::to_string(GetTickCount64()) + "\\";

	for (Boid* p : players)
	{
		message += p->getInfoString() + "\\";
	}

	for (Bullet* b : bullets)
	{
		message += b->getInfoString() + "\\";
	}
	std::cout << message << std::endl;
	return message;
}