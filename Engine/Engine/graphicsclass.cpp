////////////////////////////////////////////////////////////////////////////////
//Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_rotation = 1;
	m_Bitmap = 0;
	m_TextureShader = 0;
	m_Text = 0;
	m_ModelList = 0;
	m_Frustum = 0;
	m_MultiTextureShader = 0;
	m_LightMapShader = 0;
	m_RenderTexture = 0;
	m_DebugWindow = 0;
	m_ReflectionShader = 0;
	m_DepthShader = 0;
	m_ShadowShader = 0;
	m_ShadowMapTexture = 0;
	m_objects.clear();
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;


	//Create the Direct3D object
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	//Initialize the Direct3D object
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Create the camera object
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	//Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 2.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	//Create the texture shader object
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	//Initialize the texture shader object
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object", L"Error", MB_OK);
		return false;
	}


	//Create the light shader object
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	//Initialize the light shader object
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object", L"Error", MB_OK);
		return false;
	}

	//Create the light object
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	//Initialize the light object
	m_Light->SetDiffuseColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f); //changed from 1,0,0 to 0,0,1
	m_Light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);
	m_Light->SetLookAt(0.0f, 0.0f, 0.0f);
	m_Light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	//Create the model list object
	m_ModelList = new ModelListClass;
	if (!m_ModelList)
	{
		return false;
	}

	//Initialize the model list object
	result = m_ModelList->Initialize(25);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object", L"Error", MB_OK);
		return false;
	}

	//Create the frustum object
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}


	//Create the bitmap object
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	//initialize the bitmap object
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/seafloor.dds", 256, 256);
	if (!result)
	{
		return false;
	}

	//Create the text object
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	//Initialize the text object
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object", L"Error", MB_OK);
		return false;
	}

	// Create the multitexture shader object
	m_MultiTextureShader = new MultiTextureShaderClass;
	if (!m_MultiTextureShader)
	{
		return false;
	}

	// Initialize the multitexture shader object
	result = m_MultiTextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the multitexture shader object", L"Error", MB_OK);
		return false;
	}

	// Create the light map shader object
	m_LightMapShader = new LightMapShaderClass;
	if (!m_LightMapShader)
	{
		return false;
	}

	// Initialize the light map shader object
	result = m_LightMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light map shader object", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Create the render to texture object.
	m_ShadowMapTexture = new ShadowMapClass;
	if (!m_ShadowMapTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_ShadowMapTexture->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shadow map texture object.", L"Error", MB_OK);
		return false;
	}



	// Create the debug window object
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// Initialize the debug window object
	result = m_DebugWindow->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, 100, 100);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the debug window object", L"Error", MB_OK);
		return false;
	}

	// Create the reflection shader object
	m_ReflectionShader = new ReflectionShaderClass;
	if (!m_ReflectionShader)
	{
		return false;
	}

	// Initialize the reflection shader object
	result = m_ReflectionShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the reflection shader object", L"Error", MB_OK);
		return false;
	}

	// Create the depth shader object
	m_DepthShader = new DepthShaderClass;
	if (!m_DepthShader)
	{
		return false;
	}

	// Initialize the depth shader object
	result = m_DepthShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object", L"Error", MB_OK);
		return false;
	}

	// Create the shadow shader object.
	m_ShadowShader = new ShadowShaderClass;
	if (!m_ShadowShader)
	{
		return false;
	}

	// Initialize the shadow shader object.
	result = m_ShadowShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the shadow shader object.", L"Error", MB_OK);
		return false;
	}


	//initialize all objects in the scene at once
	result = InitializeObjects(hwnd);
	if (!result)
	{
		return false;
	}


	return true;
}

bool GraphicsClass::InitializeObjects(HWND hwnd)
{
	bool result;
	D3DXVECTOR3 pos = D3DXVECTOR3(-4.0f, -1.0f, 0.0f);
	result = InitializeObject("../Engine/data/nature/stonePit.txt", L"../Engine/data/nature/grey.png", L"../Engine/data/dirt01.dds", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the game object", L"Error", MB_OK);
		return false;
	}

	pos.x = -1.5f;
	pos.y = 0.0f;
	pos.z = 1.5f;
	result = InitializeObject("../Engine/data/sphere.txt", L"../Engine/data/nature/basketball.png", L"../Engine/data/light01.dds", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the game object", L"Error", MB_OK);
		return false;
	}

	pos.x = 0;
	pos.y = -2.5;
	pos.z = 0.0f;
	//Floor model object
	result = InitializeObject("../Engine/data/floor.txt", L"../Engine/data/blue01.dds", L"../Engine/data/blue01.dds", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the floor gameobject.", L"Error", MB_OK);
		return false;
	}

	pos.x = -1.0f;
	pos.y = -1.44f;
	pos.z = -1.0f;
	result = InitializeObject("../Engine/data/nature/flat.txt", L"../Engine/data/nature/blue.png", L"../Engine/data/nature/blue.png", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the plane gameobject.", L"Error", MB_OK);
		return false;
	}

	pos.x = -1.0f;
	pos.y = -1.3f;
	pos.z = -1.0f;
	result = InitializeObject("../Engine/data/nature/river.txt", L"../Engine/data/nature/green.png", L"../Engine/data/nature/blue.png", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the plane gameobject.", L"Error", MB_OK);
		return false;
	}

	pos.x = 3.0f;
	pos.y = -1.3f;
	pos.z = 0.0f;
	result = InitializeObject("../Engine/data/nature/bigboat.txt", L"../Engine/data/nature/wood.png", L"../Engine/data/nature/wood.png", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the  boat gameobject.", L"Error", MB_OK);
		return false;
	}

	pos.x = 15.0f;
	pos.y = -1.0f;
	pos.z = 0.0f;
	result = InitializeObject("../Engine/data/nature/tree1.txt", L"../Engine/data/nature/tree.png", L"../Engine/data/nature/tree.png", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the  boat gameobject.", L"Error", MB_OK);
		return false;
	}


	return true;
}

bool GraphicsClass::InitializeObject(char* modelName, WCHAR* filename1, WCHAR* filename2, D3DXVECTOR3 pos, HWND hwnd)
{
	bool result;
	m_objects.push_back(new GameObjectClass);
	if (!m_objects.back())
	{
		return false;
	}

	result = m_objects.back()->Initialize(m_D3D, modelName, filename1, filename2, pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the game object", L"Error", MB_OK);
		return false;
	}


	return true;
}


void GraphicsClass::Shutdown()
{
	//Release all game objects
	for (int i = 0; i < m_objects.size(); i++)
	{
		m_objects[i]->Shutdown();
	}

	//Release the shadow map texture object
	if (m_ShadowMapTexture)
	{
		m_ShadowMapTexture->Shutdown();
		delete m_ShadowMapTexture;
		m_ShadowMapTexture = 0;
	}

	// Release the shadow shader object.
	if (m_ShadowShader)
	{
		m_ShadowShader->Shutdown();
		delete m_ShadowShader;
		m_ShadowShader = 0;
	}


	// Release the depth shader object.
	if (m_DepthShader)
	{
		m_DepthShader->Shutdown();
		delete m_DepthShader;
		m_DepthShader = 0;
	}

	// Release the reflection shader object
	if (m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

	// Release the debug window object
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// Release the render to texture object
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the light map shader object
	if (m_LightMapShader)
	{
		m_LightMapShader->Shutdown();
		delete m_LightMapShader;
		m_LightMapShader = 0;
	}

	// Release the multitexture shader object
	if (m_MultiTextureShader)
	{
		m_MultiTextureShader->Shutdown();
		delete m_MultiTextureShader;
		m_MultiTextureShader = 0;
	}

	//Release the frustum object
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	//Release the model list object
	if (m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}
	//Release the text object
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	//Release the bitmap object
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	//Release the texture shader object
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	//Release the light object
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	//Release the light shader object
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	//if (m_gameobjectclass)
	//{
	//	delete m_gameobjectclass;
	//	m_gameobjectclass = 0;
	//}


	//Release the camera object
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}



	//Release the D3D object
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(D3DXVECTOR3 cameraOffset, int mouseX, int mouseY, int fps, int cpu, float frameTime, float dt, float rotationY)
{
	bool result;
	static float rotation = 0.0f;
	static float delta = 0.0f;
	static float lightPositionX = -5.0f;
	static float lightPositionZ = -5.0f;
	static int lightdirectionX = 1;

	// Update the position of the light each frame.
	//lightPositionX += lightdirectionX*0.05f;
	//if (lightPositionX > 10.0f || lightPositionX < -10.0f)
	//{
	//	lightdirectionX *= -1;//lightPositionX = -5.0f;
	//}
	rotation += 0.02f;
	if (rotation >= 360)
	{
		rotation = rotation - 360;
	}
	lightPositionX = 20 * cos(rotation);
	lightPositionZ = 20 * sin(rotation);

	// Update the position of the light.
	m_Light->SetPosition(lightPositionX, 10.0f, lightPositionZ);


	//SEt the frames per second
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	//Set the cpu usage
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());


	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}



	//	difftime()
	D3DXVECTOR3	cameraPos = m_Camera->GetPosition() + cameraOffset;
	m_Camera->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	m_Camera->SetRotation(mouseX/800, rotationY, 0.0f);

	result = RenderToTexture();
	if (!result)
	{
		return false;
	}

	result = Render();
	if (!result)
	{
		return false;
	}


	return true;
}


bool GraphicsClass::RenderSceneToTexture()
{
	D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix, translateMatrix, scalingMatrix;
	float posX, posY, posZ;
	bool result;


		// Set the render target to be the render to texture.
		m_ShadowMapTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_ShadowMapTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);



		// Generate the light view matrix based on the light's position.
		m_Light->GenerateViewMatrix();

	// Get the world matrix from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Get the view and orthographic matrices from the light object.
	m_Light->GetViewMatrix(lightViewMatrix);
	m_Light->GetProjectionMatrix(lightProjectionMatrix);


		// Setup the translation matrix for the cube model.
		m_objects[0]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the cube model with the depth shader.
	m_objects[0]->Render(m_D3D->GetDeviceContext());
	result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_objects[0]->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_objects[1]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Render the sphere model with the depth shader.
	m_objects[1]->Render(m_D3D->GetDeviceContext());
	result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_objects[1]->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetWorldMatrix(scalingMatrix);

	D3DXMatrixScaling(&scalingMatrix, 10.0f, 1.0f, 10.0f);

	// Setup the translation matrix for the ground model.
	m_objects[3]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);
	worldMatrix = worldMatrix*scalingMatrix;

	// Render the ground model with the depth shader.
	m_objects[3]->Render(m_D3D->GetDeviceContext());
	result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_objects[3]->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if (!result)
	{
		return false;
	}


		// Reset the render target back to the original back buffer and not the render to texture anymore.
		m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, translateMatrix, scalingMatrix, reflectionMatrix;
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	bool result;
	float posX, posY, posZ;


		// First render the scene to a texture.
		result = RenderSceneToTexture();
	if (!result)
	{
		return false;
	}

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.5f, 1.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

		// Generate the light view matrix based on the light's position.
		m_Light->GenerateViewMatrix();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);


		// Get the light's view and projection matrices from the light object.
	m_Light->GetViewMatrix(lightViewMatrix);
	m_Light->GetProjectionMatrix(lightProjectionMatrix);

		// Setup the translation matrix for the cube model.
	m_objects[0]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Put the cube model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_objects[0]->Render(m_D3D->GetDeviceContext());

	// Render the model using the shadow shader.
	result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_objects[0]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, m_objects[0]->m_Model->GetTextureArray()[0], m_ShadowMapTexture->GetShaderResourceView(), m_Light->GetPosition(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_objects[1]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_objects[1]->Render(m_D3D->GetDeviceContext());
	result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_objects[1]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, m_objects[1]->m_Model->GetTextureArray()[0], m_ShadowMapTexture->GetShaderResourceView(), m_Light->GetPosition(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_objects[5]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_objects[5]->Render(m_D3D->GetDeviceContext());
	result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_objects[5]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, m_objects[5]->m_Model->GetTextureArray()[0], m_ShadowMapTexture->GetShaderResourceView(), m_Light->GetPosition(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_objects[6]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_objects[6]->Render(m_D3D->GetDeviceContext());
	result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_objects[6]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, m_objects[6]->m_Model->GetTextureArray()[0], m_ShadowMapTexture->GetShaderResourceView(), m_Light->GetPosition(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetWorldMatrix(scalingMatrix);

	D3DXMatrixScaling(&scalingMatrix, 10.0f, 1.0f, 10.0f);

	// Setup the translation matrix for the ground model.
	m_objects[3]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);
	worldMatrix = worldMatrix*scalingMatrix;
	// Render the ground model using the shadow shader.
	//m_objects[3]->Render(m_D3D->GetDeviceContext());
	//result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_objects[3]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
	//	lightProjectionMatrix, m_objects[3]->m_Model->GetTextureArray()[1], m_ShadowMapTexture->GetShaderResourceView(), m_Light->GetPosition(),
	//	m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	//if (!result)
	//{
	//	return false;
	//}
	//Get the camera reflection view matrix
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	//Put the floor model vertex and index buffers on the pipeline
	m_objects[3]->Render(m_D3D->GetDeviceContext());

	//Render the floor model using the reflection shader, reflection texture, and reflection view matrix
	result = m_ReflectionShader->Render(m_D3D->GetDeviceContext(), m_objects[3]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_objects[3]->m_Model->GetTextureArray()[0],
		m_RenderTexture->GetShaderResourceView(), reflectionMatrix);

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetWorldMatrix(scalingMatrix);

	D3DXMatrixScaling(&scalingMatrix, 10.0f, 1.0f, 10.0f);

	// Setup the translation matrix for the ground model.
	m_objects[4]->GetPosition(posX, posY, posZ);
	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);
	worldMatrix = worldMatrix*scalingMatrix;
	// Render the ground model using the shadow shader.
	m_objects[4]->Render(m_D3D->GetDeviceContext());
	result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_objects[4]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix,
		lightProjectionMatrix, m_objects[4]->m_Model->GetTextureArray()[0], m_ShadowMapTexture->GetShaderResourceView(), m_Light->GetPosition(),
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}



	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderToTexture()
{
	D3DXMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;
	bool result;

	//Set the render target to be the render-to-texture
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	//Clear render-to-texture
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	//Use the camera to calulate the reflection matrix
	m_Camera->RenderReflection(-1.5f);

	//Get the camera reflection view matrix instead of the normal one;
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	//Get the world and projection matrices
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//D3DXMatrixRotationY(&worldMatrix, rotation);
	D3DXVECTOR3 objectPos = m_objects[5]->GetPosition();

	D3DXMatrixTranslation(&worldMatrix, objectPos.x, objectPos.y, objectPos.z);
	//Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_objects[5]->Render(m_D3D->GetDeviceContext());

	//Render the model using the texture shader and the reflection view shader
	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_objects[5]->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, m_objects[5]->m_Model->GetTextureArray()[0]);

	//Get the world and projection matrices
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	//D3DXMatrixRotationY(&worldMatrix, rotation);
	objectPos = m_objects[6]->GetPosition();

	D3DXMatrixTranslation(&worldMatrix, objectPos.x, objectPos.y, objectPos.z);
	//Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_objects[6]->Render(m_D3D->GetDeviceContext());

	//Render the model using the texture shader and the reflection view shader
	m_TextureShader->Render(m_D3D->GetDeviceContext(), m_objects[6]->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, m_objects[6]->m_Model->GetTextureArray()[0]);

	//Reset the render target back to the original back buffer and not the render-to-texture anymore
	m_D3D->SetBackBufferRenderTarget();

	return true;

}