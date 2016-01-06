#include "Worldmanager.h"
#include "gl/glut.h"
#include "ObjectTypes.h"
#include <iostream>
#include <cstdlib> 
#include <ctime> 

//--------------------------------------------------------------------------------------------------------

WorldManager::WorldManager()
{
	//Default Constructor.
}

//--------------------------------------------------------------------------------------------------------

WorldManager::~WorldManager()
{
	if(m_pScenario != NULL)
	{
		delete m_pScenario;
		m_pScenario = NULL;
	}

	//Camera will be deleted elswehere.
	m_pCamera = NULL;

}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Initialise()
{
	//Set time varialbes.
	m_iTimeThisFrame = GetTickCount64();
	m_iTimeLastFrame = m_iTimeThisFrame;
	
	//Set the initial camera type to perspective.
	m_eCameraType = ORTHO;
	
	//Initialise the scene, but if an error occurs, then exit.
	m_pScenario = new Scene();
	m_pScenario->Initialise();
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Exec_loop()
{
	//Get time.
	m_iTimeThisFrame = GetTickCount64();

	//exectuion loop.
	Update();
	Render();

	if (GetTickCount64() - m_iTimeThisFrame < timePerFrame)
	{
		Sleep(GetTickCount64() - m_iTimeThisFrame);
	}
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Update()
{
	//Get the delta time.
	ULONGLONG deltaTime = (m_iTimeThisFrame - m_iTimeLastFrame);
	//Set time last frame to the current time.
	m_iTimeLastFrame = m_iTimeThisFrame;

	//Update the scene.
	m_pScenario->Update(deltaTime);
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Render()
{
	//Draw the scene
	m_pScenario->Render();

	//render the camera.
	m_pCamera->Render();
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
		case 27: 
			exit(0); 
		break;

		case 'q': case 'Q': //Move forward
		break;

		case 'a': case 'A': //Move backward
		break;
	
		case 'z': case 'Z': //Move left
		break;
	
		case 'x': case 'X': //Move right
		break;
	
		case ' ': 
		break;
  }
  return;
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::SpecialKey(int key, int x, int y)
{
	//Find out which key was pressed and make the relevant changes.
	switch(key)
	{
		case GLUT_KEY_F1:
			//Rerun this scenario.
			Initialise();
		break;

		case GLUT_KEY_F2:
		break;

		case GLUT_KEY_F3:
		break;
	}

	return;
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Mouse(int button, int state, int x,int y)
{
	return;
}

//--------------------------------------------------------------------------------------------------------

void WorldManager::Idle()
{
	return;
}

//--------------------------------------------------------------------------------------------------------