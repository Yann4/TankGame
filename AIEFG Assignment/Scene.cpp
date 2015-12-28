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

//--------------------------------------------------------------------------------------------------------

void Scene::Render()
{
	// Draw a gridded ground
    float length  = 21.0f;
    float spacing = 1.0f;
    float yPos	  = 0.0f;
    float iLine;
    //Change colour to yellow.
	glColor3f(0.6f,0.5f,0.2f);

    glBegin(GL_LINES);
       for(iLine = 0; iLine <= length; iLine += spacing)
       {
			//Draw forward lines
			glVertex3f(iLine, yPos, length);
			glVertex3f(iLine, yPos, 0);
    
			//Draw crossing lines
			glVertex3f(length, yPos, iLine);
			glVertex3f(0, yPos, iLine);
       }
    glEnd();

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
}

//--------------------------------------------------------------------------------------------------------

void Scene::UpdateScenario(int a_deltaTime)
{
}