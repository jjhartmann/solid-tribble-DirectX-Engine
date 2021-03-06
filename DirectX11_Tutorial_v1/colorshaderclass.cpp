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
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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
    // Release resources
    if (m_matrixBuffer) {
        m_matrixBuffer->Release();
        m_matrixBuffer = NULL;
    }

    if (m_layout) {
        m_layout->Release();
        m_layout = NULL;
    }

    if (m_pixelShader) {
        m_pixelShader->Release();
        m_pixelShader = NULL;
    }

    if (m_vertextShader) {
        m_vertextShader->Release();
        m_vertextShader = NULL;
    }
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob *errorMessage, HWND hwnd, WCHAR *shaderFilename)
{
    char *compileError;
    unsigned long long bufferSize, i;
    ofstream fout;

    // Get pointer to error message
    compileError = (char*)(errorMessage->GetBufferPointer());

    // Get length of message
    bufferSize = errorMessage->GetBufferSize();
    fout.open("shader_error.txt");

    // Write out
    for (i = 0; i < bufferSize; ++i) {
        fout << compileError[i];
    }

    // close
    fout.close();

    // release mesage
    errorMessage->Release();
    errorMessage = NULL;

    // Print to pop-up window
    MessageBox(hwnd, L"Error compiling Shader. Check shader_error.txt for message.", shaderFilename, MB_OK);
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext *deviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType *dataPtr;
    unsigned int bufferNumber;

    // Transpose matrices to perpare them for the shader (Required for DirectX 11)
    world = XMMatrixTranspose(world);
    view = XMMatrixTranspose(view);
    projection = XMMatrixTranspose(projection);


    // Lock constant buffer so it can be written to
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
        return false;
    }

    // Get pointer to data in constant buffer
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // Copy matrices into the constant buffer
    dataPtr->world = world;
    dataPtr->view = view;
    dataPtr->projection = projection;

    // UNLOCK Constant buffer
    deviceContext->Unmap(m_matrixBuffer, 0);

    // Set position of constant buffer in vertex shader
    bufferNumber = 0;

    //Set constant buffer invetex shader with updated values
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

    return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext *deviceContext, int i)
{
    // Set the vertex input layout
    deviceContext->IASetInputLayout(m_layout);

    // Set vertex and pixel shader that will be used to render triangle
    deviceContext->VSSetShader(m_vertextShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

    // Render triangle
    deviceContext->DrawIndexed(i, 0, 0);
}
