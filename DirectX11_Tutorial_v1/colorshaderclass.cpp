//////////////////////////////////////////////////////////////////////////
// Filename: colorshadercalss.h
// Function: Handles the Color Shaders
//////////////////////////////////////////////////////////////////////////
#include "colorshaderclass.h"


ColorShaderClass::ColorShaderClass() :
    m_vertextShader(NULL), 
    m_pixelShader(NULL), 
    m_layout(NULL),
    m_matrixBuffer(NULL)
{
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{
    ; // Empty
}


ColorShaderClass::~ColorShaderClass()
{
    ; // Empty
}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    bool result = false;

    // Initialize the vertex and pixel shaders
    result = InitializeShader(device, hwnd, L"./color.vs.hlsl", L"./color.ps.hlsl");
    if (!result)
        return false;

    return true;
}

void ColorShaderClass::Shutdown()
{
    // Shutdown vertex and pixel shaders. 
    ShutdownShader();
}

bool ColorShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
    bool result = false;

    // Set the shader params
    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
    if (!result)
        return false;

    // Render Buffers with shaders
    RenderShader(deviceContext, indexCount);

    return true;
}

//////////////////////////////////////////////////////////////////////////
// Compile Shader
// - Need: (1) filename, (2) Shader name, (3) Shader version (5.0), and (4) buffer to compile to
//////////////////////////////////////////////////////////////////////////
bool ColorShaderClass::InitializeShader(ID3D11Device *device, HWND hwnd, WCHAR *vsFile, WCHAR *psFile)
{
    HRESULT result;
    ID3D10Blob *errorMessage = NULL;
    ID3D10Blob *vertexShaderBuffer = NULL;
    ID3D10Blob *pixelShaderBuffer = NULL;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;


    // Compile vertex shader
    result = D3DCompileFromFile(vsFile, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
    if (FAILED(result)) {
        // Display Error
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFile);
        }
        else {
            MessageBox(hwnd, vsFile, L"Missing Shader File", MB_OK);
        }

        return false;
    }

    // Compile pixel shader
    result = D3DCompileFromFile(psFile, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
    if (FAILED(result)) {
        // Print message if failed
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, hwnd, psFile);
        }
        else {
            MessageBox(hwnd, psFile, L"Missing Shader File", MB_OK);
        }

        return false;
    }

    // Create vertex shader object from buffer
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertextShader);
    if (FAILED(result)) {
        return false;
    }

    // Create the pixel shader from buffer
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
    if (FAILED(result)) {
        return false;
    }


    // Create the vertex input layout description
    // Setup need to match the VertexType Structure n the modelclass and the shaders
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "COLOR";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;


    //////////////////////////////////////////////////////
    // Create Input layout from polygon layout description
    //////////////////////////////////////////////////////
    
    // Get a count of elements in layout
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    // Create vertex input layout
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result)) {
        return false;
    }

    // Release vertex shader buffer and pixel shader buffer (no longer needed)
    vertexShaderBuffer->Release();
    vertexShaderBuffer = NULL;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = NULL;

    // Setup the description of the dyanmic matrix constant buffer
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // Create constan buffer pointer to access teh vertex shader constan buffer in class
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    if (FAILED(result)) {
        return false;
    }

    return true;


}

void ColorShaderClass::ShutdownShader()
{

}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob *blob, HWND hwnd, WCHAR *str)
{

}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext *deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{

}

void ColorShaderClass::RenderShader(ID3D11DeviceContext *deviceContext, int i)
{

}
