////////////////////////////////////////////////////////////////////////////////
//Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
//MY CLASS INCLUDES //
///////////////////////
#include<vector>

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "gameobjectclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include "textclass.h"
#include "modellistclass.h"
#include "frustumclass.h"
#include "multitextureshaderclass.h"
#include "lightmapshaderclass.h"
#include "debugwindowclass.h"
#include "rendertextureclass.h"
#include "reflectionshaderclass.h"
#include "depthshaderclass.h"
#include "shadowshaderclass.h"
//effectively a modified rendertextureclass
#include "shadowmapclass.h"

/////////////
//GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 1.0f;
const int SHADOWMAP_WIDTH = 1024;
const int SHADOWMAP_HEIGHT = 1024;


////////////////////////////////////////////////////////////////////////////////
//Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(D3DXVECTOR3, int, int, int, int, float, float, float);
	//bool Render(float, float);
	bool Render();

private:
	bool InitializeObjects(HWND hwnd);
	bool InitializeObject(char* modelName, WCHAR* filename1, WCHAR* filename2, D3DXVECTOR3 pos, HWND hwnd);
	bool RenderScene(float, float);
	bool RenderToTexture();
	bool RenderSceneToTexture();
	//bool RenderShadows();

private:
	
	std::vector<GameObjectClass*> m_objects;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightClass* m_Light;
	RenderTextureClass* m_RenderTexture;
	DebugWindowClass* m_DebugWindow;

	int m_rotation;

	BitmapClass* m_Bitmap;
	TextClass* m_Text;
	ModelListClass* m_ModelList;
	FrustumClass* m_Frustum;

	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;
	MultiTextureShaderClass* m_MultiTextureShader;
	LightMapShaderClass* m_LightMapShader;

	ReflectionShaderClass* m_ReflectionShader;
	
	DepthShaderClass* m_DepthShader;
	ShadowShaderClass* m_ShadowShader;
	ShadowMapClass* m_ShadowMapTexture;
};

#endif