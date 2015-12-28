#include "Obstacle.h"

//--------------------------------------------------------------------------------------------------------

Obstacle::Obstacle(float a_x, float a_y, float a_z)
{
	//Positioning.
	m_fX = a_x + NUDGE;
	m_fY = a_y;
	m_fZ = a_z - NUDGE;
	
	//Offset.
	m_Offset.x = 0.0f;
	m_Offset.z = 0.0f;
	
	//Position.
	m_CurrentPosition.x = a_x + NUDGE;
	m_CurrentPosition.z = a_z - NUDGE;
	
	//Color
	m_fRed   = 1.0f;
	m_fGreen = 1.0f;
	m_fBlue  = 1.0f;
	
	//Size.
	m_fSize = 1.0f;
}

//--------------------------------------------------------------------------------------------------------

Obstacle::~Obstacle()
{
}

//--------------------------------------------------------------------------------------------------------

void Obstacle::Render()
{
	glPushMatrix();
		glTranslatef(m_CurrentPosition.x + m_Offset.x, m_fY, m_CurrentPosition.z - m_Offset.z);
		glColor3f(m_fRed, m_fGreen, m_fBlue);
		glutSolidCube(m_fSize);
	glPopMatrix();	
}

//--------------------------------------------------------------------------------------------------------

position Obstacle::GetPosition()
{
	position tempPosition;

	tempPosition.x = m_CurrentPosition.x + m_Offset.x;
	tempPosition.z = m_CurrentPosition.z - m_Offset.z;

	return tempPosition;
}

//--------------------------------------------------------------------------------------------------------
