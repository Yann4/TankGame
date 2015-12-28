#include <windows.h>
#include "Enums.h"
#include "Worldmanager.h"
#include "Camera.h"
#include "Viewer.h"

//--------------------------------------------------------------------------------------------------------

Camera*		  Viewer::m_pCamera;
WorldManager* Viewer::m_pWorldManager;
int			  Viewer::m_iBufType;
int			  Viewer::m_iWinWidth;
int			  Viewer::m_iWinHeight;
float		  Viewer::m_fAim[3];
float		  Viewer::m_fEye[3];
float		  Viewer::m_fUpright[3];

//--------------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
	Viewer *myViewer = new Viewer();
	myViewer->Init(argc, argv);
	myViewer->InitWorld();
	myViewer->SetValue(BUFFER, MDOUBLE);
	myViewer->CreateWin("AIEFG Assignment", 1000, 700);
	myViewer->InitCamera();
	myViewer->Show();

	return 0;
}

//--------------------------------------------------------------------------------------------------------