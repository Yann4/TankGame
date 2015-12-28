#ifndef _WORLDMANAGER_H_
#define _WORLDMANAGER_H_

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include "Camera.h"
#include "gl/glut.h"
#include "Scene.h"
#include "Obstacle.h"
#include "Enums.h"
#include <math.h>
#include <vector>

using std::vector;

// Game Manager Object. 
// This object runs the game. 

class WorldManager
{
public:
	myEnum					m_eCameraType;

private:
	Scene*					m_pScenario;							
	Camera*					m_pCamera;
	
	int						m_iTimeThisFrame;			//Amount of time passed since last frame.
	int						m_iTimeLastFrame;			//Keeps track of time passed.

public:
	WorldManager();										//default constructor
	~WorldManager();	

	void Initialise();									//routine that starts up the world.  
	void Exec_loop();									//routine that sets up the game loop
	void Render();										//routine that draws everything 
	void Update();										//routine that updates everything 

	//Links to the standard Open GL Call back funcions. 
	void Keyboard(unsigned char key,int x,int y);		//process game specific input
	void SpecialKey(int key, int x, int y);
	void Mouse(int button, int state, int x,int y); 
	void Idle();										//Program executes when not waiting for input. 

	void SetCameraPointer(Camera* a_camera)				{m_pCamera = a_camera;}
};

#endif //_GAMEMANAGER_H_
