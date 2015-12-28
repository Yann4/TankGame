#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include <windows.h>
//#include "../3dsLoader/3dsLoader.h"
#include "ObjectTypes.h"
#include "gl/glut.h"
#include <string>
#include "Collision.h"

using std::string;

class Obstacle
{
private:
	float		m_fX, m_fY, m_fZ;
	float		m_fRed, m_fGreen, m_fBlue;
	float		m_fSize;

	position    m_CurrentPosition;
	position    m_Offset;

 public:
	Obstacle(float a_x, float a_y, float a_z);
	~Obstacle();

	void	 Render();

	//data management
	float	 GetX()												{return m_fX;} 
	void	 SetX(float a_x)									{m_fX = a_x + NUDGE; m_CurrentPosition.x = a_x + NUDGE;}
	float	 GetY()												{return m_fY;} 
	void	 SetY(float a_y)									{m_fY = a_y;}
	float	 GetZ()												{return m_fZ;}
	void	 SetZ(float a_z)									{m_fZ = a_z - NUDGE; m_CurrentPosition.z = a_z - NUDGE;}

	position GetPosition();
	float	 GetSize()											{return m_fSize;}

	void	 SetColor(float a_red, float a_green, float a_blue)	{m_fRed = a_red; m_fGreen = a_green; m_fBlue = a_blue;}
	void	 SetOffset(position a_offset)						{m_Offset = a_offset;}

	Collision::BoundingBox getBoundingBox() { return Collision::BoundingBox(m_fX, m_fZ, m_fSize / 2, m_fSize / 2); }
};
#endif //_OBSTACLE_H_
