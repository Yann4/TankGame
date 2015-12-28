#ifndef _CAMERA_H_
#define _CAMERA_H_
#include <windows.h>
#include "gl/glut.h"
#include "Enums.h"

class Camera
{
private:
	myEnum  m_eType;
	bool	m_bChanged[2];
	bool	m_bKeepMatrix;

	float	m_fPosition[3];
	float	m_fAimAt[3];
	float	m_fUpDirection[3];
	
	float	m_fNear;
	float	m_fFar;
	float	m_fLeft;
	float	m_fRight;
	float	m_fTop;
	float	m_fBottom;
	float	m_fHeight;
	float	m_fAspect;
	float	m_fYAngle;

public:
	Camera(myEnum Cameratype);
  
	void Render();
	void SetType(myEnum Ctype);
	void SetValuev(myEnum Pname, float *v);
	void SetValue(myEnum PName, float v1, float v2, float v3);
	void SetValue(myEnum PName, float v); 
};

#endif //_CAMERA_H_
