#include "gameobjectclass.h"

bool GameObjectClass::Initialize(D3DClass* m_D3D, char* modelName, WCHAR* filename1, WCHAR* filename2, D3DXVECTOR3 pos, HWND hwnd)
{


	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}
	

	this->m_position = pos;
	std::string mystring = std::string(modelName);
	//char* mychars = const_cast<char*>(mystring.c_str());

	bool result = m_Model->Initialize(m_D3D->GetDevice(), const_cast<char*>(mystring.c_str()), filename1, filename2, hwnd);//const_cast<WCHAR*>(data));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	return true;
}

void GameObjectClass::Shutdown()
{
	m_Model->Shutdown();
	m_Model = 0;
}

void GameObjectClass::Render(ID3D11DeviceContext* deviceContext)
{
	m_Model->Render(deviceContext);
}

int GameObjectClass::GetIndexCount()
{
	return m_Model->GetIndexCount();
}

D3DXVECTOR3 GameObjectClass::GetPosition()
{
	return m_position;
}

void GameObjectClass::GetPosition(float& x, float& y, float& z)
{
	x = m_position.x;
	y = m_position.y;
	z = m_position.z;
	return;
}

GameObjectClass::GameObjectClass()
{
	
	m_Model = 0;
}

