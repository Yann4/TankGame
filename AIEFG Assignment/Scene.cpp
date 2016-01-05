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
	textMode = false;
	typedString = "";
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
		std::cout << "Enter server name/IP: ";
		std::string name;
		std::cin >> name;
		std::cout << "Enter port number: ";
		std::cin >> input;
		cInstance.Initialise(name, input);
	}
	else
	{
		sendUpdate = shouldSendUpdate;
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
					int otherPlayer = playerIndex == 0 ? 1 : 0;
					players.at(otherPlayer)->setInterpolationMode(true);
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

	int lines = 0;
	position textPos = position(0.6, 0.9); //In screen coordinates -1 to 1, (0,0) being the centre of the screen

	for (int i = chat.size() - 1; i >= 0; i--)
	{
		int thisOne = DrawString(chat.at(i), textPos);
		lines += thisOne;
		textPos.z -= (0.05 * thisOne);
		
		if (lines >= numLines)
		{
			break;
		}
	}
	
	DrawString(typedString, position(-0.1, -0.3));

	DrawString("Player 0 lives: " + std::to_string(players.at(0)->getLives()), position(-0.9, 0.9));
	DrawString("Player 1 lives: " + std::to_string(players.at(1)->getLives()), position(-0.9, 0.85));

	if (server)
	{
		DrawString("SERVER INSTANCE", position(-0.1, 0.9));
	}
}

int Scene::DrawString(std::string text, position pos)
{
	glColor3i(0, 0, 0); //Black text
	glMatrixMode(GL_PROJECTION);
		glPushMatrix(); // save
		glLoadIdentity();// and clear
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
	glDisable(GL_DEPTH_TEST); // also disable the depth test so renders on top

	void* font = GLUT_BITMAP_HELVETICA_12;
	float rightOfScreen = glutGet(GLUT_WINDOW_WIDTH);

	std::vector<std::string> sentence = split(text, ' ');

	glRasterPos2f(pos.x, pos.z);
	
	int lines = 1;
	double rasterPosition[4]; //GL raster position returns 4 doubles. It just does, and it needs to go into an array

	for (auto word : sentence)
	{
		int wordWidth = 0;
		for (char c : word)
		{
			wordWidth += glutBitmapWidth(font, c);
		}

		glGetDoublev(GL_CURRENT_RASTER_POSITION, rasterPosition);
		if (rasterPosition[0] + wordWidth > rightOfScreen)
		{
			pos.z -= 0.05;
			glRasterPos2f(pos.x, pos.z);
			lines++;
		}

		for (char c : word)
		{
			glutBitmapCharacter(font, c);
		}
		
		glutBitmapCharacter(font, ' ');
	}

	glEnable(GL_DEPTH_TEST); // Turn depth testing back on
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); //revert to the matrix from before.
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	return lines;
}


//Update--------------------------------------------------------------------------------------------------

void Scene::Update(ULONGLONG a_deltaTime)
{
	double deltaInS = a_deltaTime / 1000.0;

	//Update the Scenario.
	UpdateScenario(deltaInS);

	if (server)
	{
		if (sendUpdate++ >= shouldSendUpdate)
		{
			sInstance.broadcast(Message(serialiseCurrentState(), -1));
			sendUpdate = 0;
		}
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

void Scene::UpdateScenario(double a_deltaTime)
{
	if (!server)
	{
		if (!textMode)
		{
			std::string input = keyboardUpdate(playerIndex);
			if (!input.empty() && !textMode)
			{
				cInstance.pushMessage("K:" + input);
			}
		}
		else
		{
			if (!type() && !typedString.empty())
			{
				cInstance.pushMessage("M:" + typedString);
				typedString = "";
			}
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

				if (index == 0)
				{
					sInstance.whisper(Message(inputString, 1));
				}else
				{
					sInstance.whisper(Message(inputString, 0));
				}
			}
			else if (inputString.find("M:") != std::string::npos)
			{
				std::string text = inputString.substr(0, 2) + inputString.substr(inputString.find("M:") + 2, std::string::npos);
				chat.push_back(text);

				sInstance.broadcast(Message("M:" + text, -1));
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
		std::regex playerUpdate("P:[0-9],-*[0-9]+.[0-9]+,-*[0-9]+.[0-9]+,-*[0-9]+.[0-9]+,[0-9]+");
		if (std::regex_match(token, playerUpdate))
		{
			int id = atoi(token.substr(2, 1).c_str());
			position p = position(atof(token.substr(4, 6).c_str()), atof(token.substr(11, 6).c_str()));
			float rotation = atof(token.substr(18, 6).c_str());
			int lives = atoi(token.substr(25, std::string::npos).c_str());

			players.at(id)->UpdateState(p, rotation);
			
			if (players.at(id)->getLives() != lives)
			{
				players.at(id)->setLives(lives);
			}
		}
		else if (token.at(0) == 'U')
		{
			unsigned long long frameNumber = std::strtoull(token.substr(1, std::string::npos).c_str(), '\0', 10);
			if (frameNumber < latestUpdateTick)
			{
				break;
			}
			latestUpdateTick = frameNumber;
		}
		else if (token.find("M:") != std::string::npos)
		{
			chat.push_back(token.substr(token.find("M:") + 2, std::string::npos));
		}
		else if (token.find("K:") != std::string::npos)
		{
			int ind = playerIndex == 0 ? 1 : 0;
			players.at(ind)->giveUpdateString(token.substr(token.find("K:") + 2, std::string::npos), bullets);
		}
	}
}

std::string Scene::keyboardUpdate(int thisPlayerIndex)
{
	//Builds up a string to be passed to the player which is used in the update
	std::string pressedKeys = "";
	if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP))
	{
		pressedKeys += "W";
	}

	if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN))
	{
		pressedKeys += "S";
	}

	if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT))
	{
		pressedKeys += "A";
	}

	if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT))
	{
		pressedKeys += "D";
	}

	if (GetAsyncKeyState(VK_SPACE) && timeSinceLastBullet > 100)
	{
		timeSinceLastBullet = 0;
		pressedKeys += "F";
	}
	timeSinceLastBullet++;

	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		textMode = true;
		return "E";
	}

	players.at(thisPlayerIndex)->giveUpdateString(pressedKeys, bullets);

	return pressedKeys;
}

bool Scene::type()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x0001)
	{
		typedString += ' ';
	}

	if (GetAsyncKeyState(VK_BACK) & 0x001)
	{
		if (!typedString.empty())
		{
			typedString.pop_back();
		}
	}

	if (GetAsyncKeyState(VK_OEM_PERIOD) & 0x0001)
	{
		typedString += '.';
	}

	if (GetAsyncKeyState(VK_OEM_COMMA) & 0x0001)
	{
		typedString += ',';
	}

	if (GetAsyncKeyState(VK_OEM_2) & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += '?';
		}
		else
		{
			typedString += '/';
		}
	}

	if (GetAsyncKeyState('A') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'A';
		}
		else
		{
			typedString += 'a';
		}
	}

	if (GetAsyncKeyState('B') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'B';
		}
		else
		{
			typedString += 'b';
		}
	}

	if (GetAsyncKeyState('C') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'C';
		}
		else
		{
			typedString += 'c';
		}
	}

	if (GetAsyncKeyState('D') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'D';
		}
		else
		{
			typedString += 'd';
		}
	}

	if (GetAsyncKeyState('E') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'E';
		}
		else
		{
			typedString += 'e';
		}
	}

	if (GetAsyncKeyState('F') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'F';
		}
		else
		{
			typedString += 'f';
		}
	}

	if (GetAsyncKeyState('G') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'G';
		}
		else
		{
			typedString += 'g';
		}
	}

	if (GetAsyncKeyState('H') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'H';
		}
		else
		{
			typedString += 'h';
		}
	}

	if (GetAsyncKeyState('I') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'I';
		}
		else
		{
			typedString += 'i';
		}
	}

	if (GetAsyncKeyState('J') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'J';
		}
		else
		{
			typedString += 'j';
		}
	}

	if (GetAsyncKeyState('K') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'K';
		}
		else
		{
			typedString += 'k';
		}
	}

	if (GetAsyncKeyState('L') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'L';
		}
		else
		{
			typedString += 'l';
		}
	}

	if (GetAsyncKeyState('M') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'M';
		}
		else
		{
			typedString += 'm';
		}
	}

	if (GetAsyncKeyState('N') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'N';
		}
		else
		{
			typedString += 'n';
		}
	}

	if (GetAsyncKeyState('O') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'O';
		}
		else
		{
			typedString += 'o';
		}
	}

	if (GetAsyncKeyState('P') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'P';
		}
		else
		{
			typedString += 'p';
		}
	}

	if (GetAsyncKeyState('Q') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'Q';
		}
		else
		{
			typedString += 'q';
		}
	}

	if (GetAsyncKeyState('R') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'R';
		}
		else
		{
			typedString += 'r';
		}
	}

	if (GetAsyncKeyState('S') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'S';
		}
		else
		{
			typedString += 's';
		}
	}

	if (GetAsyncKeyState('T') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'T';
		}
		else
		{
			typedString += 't';
		}
	}

	if (GetAsyncKeyState('U') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'U';
		}
		else
		{
			typedString += 'u';
		}
	}

	if (GetAsyncKeyState('V') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'V';
		}
		else
		{
			typedString += 'v';
		}
	}

	if (GetAsyncKeyState('W') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'W';
		}
		else
		{
			typedString += 'w';
		}
	}

	if (GetAsyncKeyState('X') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'X';
		}
		else
		{
			typedString += 'a';
		}
	}

	if (GetAsyncKeyState('Y') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'Y';
		}
		else
		{
			typedString += 'y';
		}
	}

	if (GetAsyncKeyState('Z') & 0x0001)
	{
		if (GetAsyncKeyState(VK_SHIFT))
		{
			typedString += 'Z';
		}
		else
		{
			typedString += 'z';
		}
	}

	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		textMode = false;
		return false;
	}
	return true;
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
	return message;
}