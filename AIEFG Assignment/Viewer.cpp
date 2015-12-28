#include "Viewer.h"
#include "gl/glut.h"

//--------------------------------------------------------------------------------------------------------

Viewer::Viewer()
{ 
	m_pWinName="AIEFG Assignment";
	m_iBufType=GLUT_SINGLE;
  
	for(int i=0;i<3;i++)
	{
		m_fBackground[i]=0.0f;
	}
}

//--------------------------------------------------------------------------------------------------------

Viewer::~Viewer()
{
	return;
}

//--------------------------------------------------------------------------------------------------------

void Viewer::Init(int argc,char **argv)
{    
	m_fUpright[0]=0.0f;
	m_fUpright[1]=0.0f;
	m_fUpright[2]=-1.0f;
	
	m_fAim[0]=20.0f;
	m_fAim[1]=-1.0f;
	m_fAim[2]=20.0f;
	
	m_fEye[0]=20.0f;
	m_fEye[1]=50.0f;
	m_fEye[2]=20.0f;

	glutInit(&argc,argv);
}

//--------------------------------------------------------------------------------------------------------

void Viewer::CreateWin(char *Name,int Width,int Height)
{
	m_pWinName   = Name;
	m_iWinWidth  = Width;
	m_iWinHeight = Height;

	GLInit();
}

//--------------------------------------------------------------------------------------------------------

void Viewer::SetValue(myEnum PName,myEnum Type)
{
	switch(PName)
	{
		case BUFFER:
			if(Type==MDOUBLE)
			{
				m_iBufType=GLUT_DOUBLE;
			}
			else if(Type==SINGLE)
			{
				m_iBufType=GLUT_SINGLE;
			}
		break;

		case BACKCOLOUR:
		break;
  
		default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------------

void Viewer::InitWorld()
{
	m_pWorldManager = new WorldManager();
	m_pWorldManager->Initialise();
}

//--------------------------------------------------------------------------------------------------------

void Viewer::InitCamera()
{
	m_pCamera = new Camera(ORTHO);

	m_pWorldManager->SetCameraPointer(m_pCamera);
	m_pWorldManager->m_eCameraType = ORTHO;

	m_fUpright[0] = 0.0f;
	m_fUpright[1] = 0.0f;
	m_fUpright[2] = -1.0f;

	m_fAim[0] = 5.0f;
	m_fAim[1] = -1.0f;
	m_fAim[2] = 35.0f;
	
	m_fEye[0] = 5.0f;
	m_fEye[1] = 12.0f;
	m_fEye[2] = 35.0f;

	ReinitCamera(ORTHO, -6.0f, 17.0f, 13.0f, 36.0f, 1.0f, 1000.0f);
}

//--------------------------------------------------------------------------------------------------------

void Viewer::ReinitCamera(myEnum a_cameraType, float a_near, float a_far, float a_angle, float a_aspect)
{
	  m_pCamera->SetType(a_cameraType);
	  m_pWorldManager->m_eCameraType = a_cameraType;

	  m_pCamera->SetValue(MNEAR, a_near);
	  m_pCamera->SetValue(YANGLE, a_angle);

	  m_pCamera->SetValuev(AIMAT, m_fAim);
	  m_pCamera->SetValuev(UPDIRECTION, m_fUpright);
	  m_pCamera->SetValuev(POSITION, m_fEye);
	  
	  m_pCamera->SetValue(MFAR, a_far);
	  m_pCamera->SetValue(HEIGHT, m_iWinHeight);
	  m_pCamera->SetValue(ASPECT, a_aspect);
}

//--------------------------------------------------------------------------------------------------------

void Viewer::ReinitCamera(myEnum a_cameraType, float a_left, float a_right, float a_bottom, float a_top, float a_near, float a_far)
{
	  m_pCamera->SetType(a_cameraType);
	  m_pWorldManager->m_eCameraType = a_cameraType;

	  m_pCamera->SetValue(MLEFT, a_left);
	  m_pCamera->SetValue(MRIGHT, a_right);
	  m_pCamera->SetValue(MTOP, a_top);
	  m_pCamera->SetValue(MBOTTOM, a_bottom);
	  m_pCamera->SetValue(MNEAR, a_near);
	  m_pCamera->SetValue(MFAR, a_far);

	  m_pCamera->SetValuev(AIMAT, m_fAim);
	  m_pCamera->SetValuev(UPDIRECTION, m_fUpright);
	  m_pCamera->SetValuev(POSITION, m_fEye);
}

//--------------------------------------------------------------------------------------------------------

void Viewer::SetCamera(float nvalue,float viewangle)
{
	m_pCamera->SetValue(MNEAR, nvalue);
	m_pCamera->SetValue(YANGLE, viewangle);
}

//--------------------------------------------------------------------------------------------------------

void Viewer::Show()
{
	glutMainLoop();
}

//--------------------------------------------------------------------------------------------------------

void Viewer::GLInit()
{
	glutInitDisplayMode(m_iBufType |GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(m_iWinWidth, m_iWinHeight);
	glutCreateWindow(m_pWinName);

	//Call method to set up lighting solution.
	SetLightingSolution(0.0f,20.0f,-15.0f);

	//Functions to be called.
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutMouseFunc(Mouse);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey); 
}

//--------------------------------------------------------------------------------------------------------

void Viewer::SetLightingSolution(float posX, float posY, float posZ)
{
	GLfloat lightPosition[] = {posX, posY, posZ};

	//Lighting solutions.
	float ambientLight[]		= {0.5f,0.5f,0.5f,1.0f};//Ambient light to illumninate scene.
	float diffuseLight[]		= {1.0f,1.0f,1.0f,1.0f};//Diffuse lighting.
	float specularLight[]		= {0.5f,0.5f,0.5f,1.0f};//Specular lighting.
	float specularReflectance[] = {0.8f,0.8f,0.8f,1.0f};

	glEnable(GL_CULL_FACE);								//Stop calculation of inside faces
	glEnable(GL_DEPTH_TEST);							//Hidden surface removal
	glEnable(GL_LIGHTING);								//Enable lighting.

	glEnable(GL_TEXTURE_2D);							//This Enable the Texture mapping
	glShadeModel(GL_SMOOTH);							//Enable smooth shading model - Phong.

	//Set up and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);

	//Set up and enable color material.
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularReflectance);
	glMateriali(GL_FRONT,GL_SHININESS,128);

	//clear background colour.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}

//--------------------------------------------------------------------------------------------------------

void Viewer::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pWorldManager->Exec_loop();
	glFlush();

	if(m_iBufType == GLUT_DOUBLE)
	{
		glutSwapBuffers();
	}
}

//--------------------------------------------------------------------------------------------------------

void Viewer::Reshape(int width,int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	m_iWinWidth  = width;
	m_iWinHeight = height;
	m_pCamera->SetValuev(POSITION, m_fEye);
	m_pCamera->SetValuev(AIMAT, m_fAim);
	m_pCamera->Render();
}

//--------------------------------------------------------------------------------------------------------

void Viewer::Mouse(int button, int state, int x, int y)
{
	m_pWorldManager->Mouse(button, state, x, y);
	Display();
}

//--------------------------------------------------------------------------------------------------------

void Viewer::Idle()
{
	m_pWorldManager->Idle();
	Display();
}

//--------------------------------------------------------------------------------------------------------

void Viewer::Keyboard(unsigned char key, int x, int y)
{
	m_pWorldManager->Keyboard((unsigned char)key, x, y);  
	Display();
}

//--------------------------------------------------------------------------------------------------------

void Viewer::SpecialKey(int key, int x, int y)
{
	//Check if there is any change via the world object.
	m_pWorldManager->SpecialKey(key, x, y);

	switch(key)
	{
		case GLUT_KEY_F1:
		break;

		default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------------