///////////////////////////////////////////////
// Filename: color.hlsl
// Type: Vertex Shader
//////////////////////////////////////////////

// GLOBALS
// - Used to mediate interaction between HLSL, GPU, and application.
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


// TYPE DEFINITIONS
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};


struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};


///////////////////////////////////////////////////////
// VERTEXT SHADER
///////////////////////////////////////////////////////
PixelInputType ColorVertexSharder(VertexInputType input) {
    PixelInputType output;
    
    // Change the position vector to be 4 Units
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and proejctioon matrixces. 
    output.position = mul(input.position, worldMatrix); // WORLD
    output.position = mul(output.position, viewMatrix); // CAMERA
    output.position = mul(output.position, projectionMatrix);

    // Store the input color fo the pixel shader to use
    output.color = input.color;

    return output;
}