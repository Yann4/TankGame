#include "Camera.h"

//--------------------------------------------------------------------------------------------------------

Camera::Camera(myEnum CameraType)
{
	m_eType = CameraType;
	m_bKeepMatrix = true;

	for(int i=0; i<3; i++)
    {
		if(i<2)
        {
			m_bChanged[i]	  = false;
			m_fPosition[i]	  = 0.0f;
			m_fAimAt[i]		  = 0.0f;
			m_fUpDirection[i] = 0.0f;
		}
    }
	
	m_fUpDirection[1] = 1.0f;
	m_fAimAt[2] = -100.0f;  
}

//--------------------------------------------------------------------------------------------------------

void Camera::SetValuev(myEnum PName, float *v)
{
	float *pTemp;

	switch(PName)
	{
		case POSITION:
			m_bChanged[0] = true;
			pTemp = m_fPosition;
		break;

		case AIMAT:
			m_bChanged[0] = true;
			pTemp = m_fAimAt;
		break;

		case UPDIRECTION:
			m_bChanged[0] = true;
			pTemp = m_fUpDirection;
		break;

		default:
		break;
	}

	for(int i=0; i<3; i++)
	{
		pTemp[i] = v[i];
	}
}

//--------------------------------------------------------------------------------------------------------

void Camera::SetValue(myEnum PName, float v1, float v2, float v3)
{
	float temp[] = {v1, v2, v3};
	SetValuev(PName, temp);
}

//--------------------------------------------------------------------------------------------------------

void Camera::SetValue(myEnum PName, float v)
{
	switch(PName)
	{
		case ASPECT:
			m_bChanged[1] = true;
			m_fAspect = v;
		break;

		case MLEFT:
			m_bChanged[1] = true;
			m_fLeft = v;
		break;

		case MRIGHT:
			m_bChanged[1] = true;
			m_fRight = v;
		break;

		case MTOP:
			m_bChanged[1] = true;
			m_fTop = v;
		break;

		case MBOTTOM:
			m_bChanged[1] = true;
			m_fBottom = v;
		break;

		case MNEAR:
			m_bChanged[1] = true;
			m_fNear = v;
		break;

		case MFAR:
			m_bChanged[1] = true;
			m_fFar = v;
		break;

		case HEIGHT:
			if(m_eType == ORTHO)
			{
				m_bChanged[1] = true;
				m_fHeight = v;
			}
		break;

		case YANGLE:
			if(m_eType == PERSPECTIVE)
			{
				m_bChanged[1] = true;
				m_fYAngle = v;
			}
		break;

		default:
		break;
	}
}

//--------------------------------------------------------------------------------------------------------

void Camera::SetType(myEnum Ctype)
{
	m_eType=Ctype;
}

//--------------------------------------------------------------------------------------------------------

void Camera::Render()
{
	if(m_bChanged[1])
    {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
      
		if(m_eType==ORTHO)
		{
			glOrtho(m_fLeft, m_fRight, m_fBottom, m_fTop, m_fNear, m_fFar);
		}
		else
		{
			gluPerspective(m_fYAngle, m_fAspect, m_fNear, m_fFar);
		}

		glMatrixMode(GL_MODELVIEW);
    }
  
	if(m_bChanged[0])
	{
		glLoadIdentity();
		gluLookAt(m_fPosition[0], m_fPosition[1], m_fPosition[2], 
			      m_fAimAt[0], m_fAimAt[1], m_fAimAt[2], 
				  m_fUpDirection[0], m_fUpDirection[1], m_fUpDirection[2]);
	}
}

//--------------------------------------------------------------------------------------------------------