#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

//const bool FULL_SCREEN = true;
//const bool VSYNC_ENABLED = true;
//const float SCREEN_DEPTH = 1000.0f;
//const float SCREEN_NEAR = 0.1f;

#include "inputclass.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "terrainclass.h"
#include "graphicsclass.h"
#include "colorshaderclass.h"
#include "timerclass.h"
#include "positionclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "fontshaderclass.h"
#include "textclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "gameobjectclass.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include "modellistclass.h"
#include "frustumclass.h"
#include "multitextureshaderclass.h"
#include "lightmapshaderclass.h"

class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame(D3DXVECTOR3, int, int, int, int, float);

private:
	bool HandleInput(float);
	bool RenderGraphics();

private:
	HWND m_hwnd;
	InputClass* m_Input;
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	TerrainClass* m_Terrain;
	ColorShaderClass* m_ColorShader;
	TimerClass* m_Timer;
	PositionClass* m_Position;
	FpsClass* m_Fps;
	CpuClass* m_Cpu;
	FontShaderClass* m_FontShader;
	TextClass* m_Text;

	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	//GameObjectClass* m_gameobjectclass;
	TextureShaderClass* m_TextureShader;
	int m_rotation;

	BitmapClass* m_Bitmap;

	GraphicsClass* m_Graphics;

	ModelListClass* m_ModelList;
	FrustumClass* m_Frustum;
	MultiTextureShaderClass* m_MultiTextureShader;
	LightMapShaderClass* m_LightMapShader;

	float m_timeAtLastFrame;
};


#endif