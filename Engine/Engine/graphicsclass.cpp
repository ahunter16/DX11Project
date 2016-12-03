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
	//m_gameobjectclass = 0;
	m_rotation = 1;
	m_Bitmap = 0;
	m_TextureShader = 0;
	m_Text = 0;
	m_ModelList = 0;
	m_Frustum = 0;
	m_MultiTextureShader = 0;
	m_LightMapShader = 0;
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

		
	//Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	//Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	//Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
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
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_objects.push_back(new GameObjectClass);
	if (!m_objects.back())
	{
		return false;
	}

	//m_gameobjectclass = new GameObjectClass;
	//if (!m_gameobjectclass)
	//{
	//	return false;
	//}

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	result = m_objects.back()->Initialize(m_D3D, "../Engine/data/sphere.txt", L"../Engine/data/stone01.dds", L"../Engine/data/dirt01.dds", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the game object.", L"Error", MB_OK);
		return false;
	}

	m_objects.push_back(new GameObjectClass);
	if (!m_objects.back())
	{
		return false;
	}

	
	result = m_objects.back()->Initialize(m_D3D, "../Engine/data/sphere.txt", L"../Engine/data/stone01.dds", L"../Engine/data/light01.dds", pos, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the game object.", L"Error", MB_OK);
		return false;
	}

	//Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	//Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	//Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	//Initialize the light object.
	m_Light->SetDiffuseColor(0.0f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f); //changed from 1,0,0 to 0,0,1
	m_Light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

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

	//Create the frustum object.
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

	//Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the multitexture shader object.
	m_MultiTextureShader = new MultiTextureShaderClass;
	if (!m_MultiTextureShader)
	{
		return false;
	}

	// Initialize the multitexture shader object.
	result = m_MultiTextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the multitexture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light map shader object.
	m_LightMapShader = new LightMapShaderClass;
	if (!m_LightMapShader)
	{
		return false;
	}

	// Initialize the light map shader object.
	result = m_LightMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light map shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{

	// Release the light map shader object.
	if (m_LightMapShader)
	{
		m_LightMapShader->Shutdown();
		delete m_LightMapShader;
		m_LightMapShader = 0;
	}

	// Release the multitexture shader object.
	if (m_MultiTextureShader)
	{
		m_MultiTextureShader->Shutdown();
		delete m_MultiTextureShader;
		m_MultiTextureShader = 0;
	}

	//Release the frustum object.
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	//Release the model list object.
	if (m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}
	//Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	//Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	//Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	//Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	//Release the light shader object.
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


	//Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}



	//Release the D3D object.
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

	//Update the rotation variable each frame.
	rotation += (float)D3DX_PI *0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	delta = 0.0f;

	//Render the graphics scene.
	result = Render(rotation, delta);
	if (!result)
	{
		return false;
	}
	//	difftime()
	D3DXVECTOR3	cameraPos = m_Camera->GetPosition() + cameraOffset;
	m_Camera->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
	m_Camera->SetRotation(mouseX/800, rotationY, 0.0f);

	return true;
}


bool GraphicsClass::Render(float rotation, float deltavalue)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, extraMatrix;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;
	D3DXVECTOR3 cameraPosition;
	bool renderModel, result;


	//Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Generate the view matrix based on the camera's position.
	m_Camera->Render();

	//Get the view, projection, and world matrices from the camera and D3D objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);



	//Construct the frustum
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	//Get the number of models that will be rendered
	modelCount = m_ModelList->GetModelCount();

	//Initialize the count of models that have been rendered
	renderCount = 0;

	//Go through all the models and render them only if they can be seen by the camera view

	for (index = 0; index < modelCount; index++)
	{
		//Get the position and color of the sphere model at this index
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		//Set the radius of the sphere to 1.0 since this is already known
		radius = 1.0f;

		//Check if the sphere model is in the view frustum
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		//if it can be seen then render it, if not, skipthis model and check the next sphere
		if (renderModel)
		{
			//Rotate the world matrix by the rotation value so that the triangle will spin.
			D3DXMatrixRotationY(&worldMatrix, rotation);

			//Move the modelto the location it should be rendered at
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ);

			//Put the model vertex and index buffers on teh graphics pipeline to prepare them for drawing
			m_objects[0]->Render(m_D3D->GetDeviceContext());


			//Render the model using the light shader. 
			/*m_LightShader->Render(m_D3D->GetDeviceContext(), m_gameobjectclass->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Light->GetAmbientColor(), m_Light->GetDirection(), color, deltavalue, m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_gameobjectclass->m_Model->GetTextureArray()[1]);*/

			//Reset to the original world matrix
			m_D3D->GetWorldMatrix(worldMatrix);

			//Since this model was rendered then increase the count for this frame
			renderCount++;
		}
	}

	//Set the number of models that was actually rendered this frame
	result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}


	//Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	//get the position of the camera
	cameraPosition = m_Camera->GetPosition();

	//Get backup world matrix
	m_D3D->GetWorldMatrix(extraMatrix);

	//Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);

	/////////Render TEXT
	//Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	//Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	//Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), extraMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	//Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	//Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	////////////End TEXT
	////////////Start 3D

	//Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_objects[0]->Render(m_D3D->GetDeviceContext()); //REQUIRED BEFORE RENDERING WITH A SHADER


	////Render the model using the light shader. 
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_gameobjectclass->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_Light->GetAmbientColor(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), deltavalue, m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), m_gameobjectclass->m_Model->GetTextureArray()[1]);
	//if (!result)
	//{
	//	return false;
	//}

	// Render the model using the multitexture shader.
	//m_MultiTextureShader->Render(m_D3D->GetDeviceContext(), m_objects[0]->m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
	//	m_objects[0]->m_Model->GetTextureArray());

	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_objects[1]->m_Model->Render(m_D3D->GetDeviceContext());//REQUIRED BEFORE RENDERING WITH A SHADER

	
	m_LightMapShader->Render(m_D3D->GetDeviceContext(), m_objects[1]->m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_objects[1]->m_Model->GetTextureArray());

	return true;
}