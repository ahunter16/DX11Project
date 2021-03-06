////////////////////////////////////////////////////////////////////////////////
//Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"


LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float blue, float green, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
	return;
}


void LightClass::SetLookAt(float x, float y, float z)
{
	m_lookAt.x = x;
	m_lookAt.y = y;
	m_lookAt.z = z;
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}
void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
	m_specularColor = D3DXVECTOR4(r, g, b, a);
}
void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}


D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

D3DXVECTOR4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

D3DXVECTOR3 LightClass::GetPosition()
{
	return m_position;
}

void LightClass::GenerateOrthoMatrix(float width, float depthPlane, float nearPlane)
{
	// Create the orthographic matrix for the light.
	D3DXMatrixOrthoLH(&m_orthoMatrix, width, width, nearPlane, depthPlane);

	return;
}


void LightClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void LightClass::GenerateViewMatrix()
{
	D3DXVECTOR3 up;


	// upward pointing vector
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//Create the view matrix from the three vectors
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_lookAt, &up);

	return;
}




void LightClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}
