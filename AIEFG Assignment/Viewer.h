#ifndef _VIEWER_H_
#define _VIEWER_H_

#include <windows.h>
#include <iostream>
#include "gl/glut.h"

#include "Enums.h"
#include "Camera.h"
#include "Worldmanager.h"

class Viewer
{
private:
	char*				 m_pWinName;
	float				 m_fBackground[3];

	static float		 m_fEye[3];
	static float		 m_fAim[3];
	static float		 m_fUpright[3];
  
	static WorldManager* m_pWorldManager;
	static Camera*		 m_pCamera;
   
	static int			 m_iBufType;
	static int			 m_iWinWidth;
	static int			 m_iWinHeight;
	
 public:
	Viewer();
	~Viewer();
	
	void InitCamera();
	void SetCamera(float nvalue, float viewangle);
	static void ReinitCamera(myEnum a_cameraType, float a_left, float a_right, float a_bottom, float a_top, float a_near, float a_far);
	static void ReinitCamera(myEnum a_cameraType, float a_near, float a_far, float a_angle, float a_aspect);
  
	void InitWorld();
	void SetValue(myEnum PName, myEnum Type);
	void Init(int argc, char **argv);
	void Show();
	void CreateWin(char *Name, int Width, int Height);

private:
	void GLInit();
	
	static void Reshape(int width, int height);
	static void Display();
	static void Mouse(int button, int state, int x, int y);
	static void Idle();
	static void Keyboard(unsigned char key, int x, int y);
	static void SpecialKey(int key, int x, int y);
	static void SetLightingSolution(float posX, float posY, float posZ);

};

#endif //_VIEWER_H_
