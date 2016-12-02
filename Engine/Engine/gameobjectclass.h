#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include "modelclass.h"
#include "d3dclass.h"
using namespace std;


class GameObjectClass
{

public:
	GameObjectClass();

	bool Initialize(D3DClass* m_D3D, char* modelName, WCHAR*, WCHAR*, D3DXVECTOR3 pos, HWND hwnd);
	void Shutdown();
	bool Frame();

	void Render(ID3D11DeviceContext* deviceContext);
	int GetIndexCount();
	ModelClass* m_Model;
private:

	D3DXVECTOR3 m_position;
};
#endif
