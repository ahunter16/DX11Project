
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer VariableBuffer
{
    float delta;
	float3 padding;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float morePadding;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};


////////////////////////////////////////////////////////////////////////////////
//Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

	//Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	//Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    
	//Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	//Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    //Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    //Calculate position of vertex in the world
    worldPosition = mul(input.position, worldMatrix);

    //Determine viweing direction from camera to the world
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

    //Normalize view direction (don't need its magnitude)
    output.viewDirection = normalize(output.viewDirection);

    return output;
}